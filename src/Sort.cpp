#include "Sort.h"

SortPlan::SortPlan(char const* const name, Plan* const input)
	: Plan(name), _input(input)
{
	TRACE(false);
} // SortPlan::SortPlan

SortPlan::~SortPlan()
{
	TRACE(false);
	delete _input;
} // SortPlan::~SortPlan

Iterator* SortPlan::init() const
{
	TRACE(false);
	return new SortIterator(this);
} // SortPlan::init

SortIterator::SortIterator(SortPlan const* const plan) :
	_plan(plan),
	_input(plan->_input->init()),
	_consumed(0),
	_produced(0),
	_runIndex(0),
	_in_cache_priority_queue(PriorityQueue(static_cast<size_t>(ceil(log2(CACHE_CAPACITY))))),
	_memory_disk_priority_queue(PriorityQueue(static_cast<size_t>(ceil(log2(MEMORY_FAN_IN))))),
	_flush_count(0)
{
	TRACE(false);
	/// Tricky: 
	/// 1. we assume here memory capacity is interger multiple of cache capacity
	_memory.resize(MEMORY_CAPACITY / CACHE_CAPACITY);

	size_t cache_priority_queue_index = 0;
	size_t cache_run_cnt = 0;
	Row output_row;

	// Step 1: We collect data and output as memory runs on disk
	for (Row row; _input->next(row); _input->free(row))
	{
		++_consumed;
		_in_cache_priority_queue.push(cache_priority_queue_index, row);
		++cache_priority_queue_index;

		if (cache_priority_queue_index >= CACHE_CAPACITY)
		{
			// Step 1.1: Cache priority queue is full, apply internal sort
			while (cache_priority_queue_index > 0)
			{
				_in_cache_priority_queue.pop(output_row);
				_memory[cache_run_cnt].push(output_row);
				cache_priority_queue_index--;
			}
			assert(_in_cache_priority_queue.empty());
			_in_cache_priority_queue.clear();
			cache_run_cnt++;
		}

		// Step 1.2: Memory is full, apply external sort
		if (cache_run_cnt * CACHE_CAPACITY >= MEMORY_CAPACITY)
		{
			externalSortCacheMemory(cache_run_cnt);
			flushMemory();
			cache_run_cnt = 0;
		}
	}

	// Step 1.3: There might be some offset row left in cache priority queue
	// or some offset left in memory, sort and flush them to disk
	bool additional_cache_run = false;
	while (cache_priority_queue_index > 0)
	{
		additional_cache_run = true;
		_in_cache_priority_queue.pop(output_row);
		_memory[cache_run_cnt].push(output_row);
		cache_priority_queue_index--;
	}
	if (additional_cache_run) cache_run_cnt++;

	if (!_memory[0].empty()) {
		externalSortCacheMemory(cache_run_cnt);
		flushMemory();
		cache_run_cnt = 0;
	}

	delete _input;

	// Step 2: do memory disk external sort
	// Notice now we do not do external sort, so we just copy run_0 here for debugging.
	externalSortMemoryDisk();

	traceprintf("Output: disk/run_%d", _runIndex);
	_output.open("disk/run_" + std::to_string(_runIndex-1));
	traceprintf("%s consumed %lu rows\n", _plan->_name, (unsigned long)(_consumed));
} // SortIterator::SortIterator

SortIterator::~SortIterator()
{
	TRACE(false);
	_output.close();

	traceprintf("%s produced %lu of %lu rows\n", _plan->_name, (unsigned long)(_produced), (unsigned long)(_consumed));
	traceprintf("total writes to disk (in rows unit): %lu\n", (unsigned long)_flush_count);
} // SortIterator::~SortIterator

bool SortIterator::next(Row& row)
{
	TRACE(false);

	if (_produced >= _consumed) return false;
	assert(row.readFromDisk(_output));
	++_produced;
	return true;
} // SortIterator::next

void SortIterator::free(Row& row)
{
	TRACE(false);
} // SortIterator::free

void SortIterator::flushMemory()
{
	std::ofstream file("disk/run_" + std::to_string(_runIndex));
	if (file.is_open()) {
		for (size_t i = 0; i < _output_buffer.size(); i++) {
			_output_buffer[i].writeToDisk(file);
			_flush_count++;
		}
		file.close();
		_output_buffer.clear();
		_run_queue.push_back("disk/run_" + std::to_string(_runIndex));
		_runIndex++;
	}
	else {
		printf("Error: Unable to open file run_%ld\n", _runIndex);
	}
}

void SortIterator::externalSortCacheMemory(size_t cache_run_cnt)
{
	/// Tricky:
	/// 1. We ignore cache capacity here and always assume the cache to memory priority queue
	/// can fit in cache, thus we do NOT apply multi level external sort at this part
	size_t level = static_cast<size_t>(ceil(log2(cache_run_cnt)));
	PriorityQueue CacheToMemory(level);

	for (size_t index = 0; index < cache_run_cnt; index++)
	{
		assert(!_memory[index].empty());
		CacheToMemory.push(index, _memory[index].front());
		_memory[index].pop();
	}

	Row output;
	size_t output_index;
	size_t output_cnt = 0;
	size_t total_rows = CACHE_CAPACITY * (cache_run_cnt - 1) + _memory[cache_run_cnt - 1].size();

	while (output_cnt <= total_rows)
	{
		output_index = CacheToMemory.pop(output);
		_output_buffer.push_back(output);
		++output_cnt;

		if (!_memory[output_index].empty()) {
			CacheToMemory.push(output_index, _memory[output_index].front());
			_memory[output_index].pop();
		}
	}

	for (size_t index = 0; index < cache_run_cnt; index++)
	{
		assert(_memory[index].empty());
	}
}

void SortIterator::externalSortMemoryDisk() {
	while (_run_queue.size() > 1) {
		size_t current_fan_in = compute_graceful_degradation(_run_queue.size());

        // Gather smallest runs and pop them from the run queue
		// NOTE: We will push the merged run back to the run queue at the end of this loop
        std::vector<std::string> chosen_runs;
        for (size_t i = 0; i < current_fan_in; i++) {
            chosen_runs.push_back(_run_queue.front());
            _run_queue.pop_front();
        }

		// Merge these runs
		// size_t prefix_sum_fan_in = i == 0 ? 0 : _graceful_degradation_vector[0] + MEMORY_FAN_IN * (i - 1);
		_fan_in_file_handlers.clear();
		Row input_row;
		for (size_t run_index = 0; run_index < current_fan_in; run_index++) {
			std::ifstream file;
			file.open(chosen_runs[run_index]);
			assert(file.is_open());
			assert(input_row.readFromDisk(file));
			_memory_disk_priority_queue.push(run_index, input_row);
			_fan_in_file_handlers.push_back(std::move(file));
		}

		Row output_row;
		int output_index = 0;
		while (true) {
			output_index = _memory_disk_priority_queue.pop(output_row);
			if (output_index == -1) break;
			_output_buffer.push_back(output_row);
			if (input_row.readFromDisk(_fan_in_file_handlers[output_index])) {
				_memory_disk_priority_queue.push(output_index, input_row);
			}
		}
		_memory_disk_priority_queue.clear();

		// Cleanup intermediate runs
		for (const std::string& fname : chosen_runs) {
			if (remove(fname.c_str()) != 0) {
				printf("Warning: Unable to delete file %s.\n", fname.c_str());
			}
		}

		// Now this will write the merged run to disk, and also push the run to the run queue
		flushMemory();
	}
}

size_t SortIterator::compute_graceful_degradation(size_t memory_run) {
    if (memory_run <= 1) return memory_run;
	else if (memory_run <= MEMORY_FAN_IN) return memory_run;
    return (memory_run - 2) % (MEMORY_FAN_IN - 1) + 2;
}
