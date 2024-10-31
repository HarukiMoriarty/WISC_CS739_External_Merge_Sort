#include "Sort.h"

// Internal Sort Helper function
//===================================================================
int partition(std::vector<Row*>& rows, int low, int high)
{
	Row* pivot = rows[high];
	int i = low - 1;

	for (int j = low; j < high; ++j) {
		if (*pivot >= *rows[j]) {
			++i;
			std::swap(rows[i], rows[j]);
		}
	}
	std::swap(rows[i + 1], rows[high]);
	return i + 1;
}

void quickSort(std::vector<Row*>& rows, int low, int high)
{
	if (low < high) {
		int pi = partition(rows, low, high);
		quickSort(rows, low, pi - 1);
		quickSort(rows, pi + 1, high);
	}
}
//=====================================================================

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
	/// Tricky: 
	/// 1. we ignore the inner node capacity here
	/// 2. we assume the cache capacity is power of 2 of rows
	/// Normally, the level should be floor(log2(CACHE_CAPACITY)) - 1
	_cache_priority_queue(PriorityQueue(static_cast<size_t>(log2(CACHE_CAPACITY))))
{
	TRACE(false);
	/// Tricky: 
	/// 1. we assume here memory capacity is interger multiple of cache capacity
	_memory.resize(MEMORY_CAPACIY / CACHE_CAPACITY);

	size_t cache_priority_queue_index = 0;
	size_t cache_run_cnt = 0;
	Row output_row;

	// Step 1: We collect data and output as memory runs on disk
	for (Row row; _input->next(row); _input->free(row))
	{
		++_consumed;
		_cache_priority_queue.push(cache_priority_queue_index, row);
		++cache_priority_queue_index;

		if (cache_priority_queue_index >= CACHE_CAPACITY)
		{
			// Step 1.1: Cache priority queue is full, apply internal sort
			while (cache_priority_queue_index > 0)
			{
				_cache_priority_queue.pop(output_row);
				_memory[cache_run_cnt].push(output_row);
				cache_priority_queue_index--;
			}
			cache_run_cnt++;
		}

		// Step 1.2: Memory is full, apply external sort
		if (cache_run_cnt * CACHE_CAPACITY >= MEMORY_CAPACIY)
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
		_cache_priority_queue.pop(output_row);
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

	// Notice now we do not do external sort, so we just copy run_0 here for debugging.
	_output.open("disk/run_0");
	traceprintf("%s consumed %lu rows\n", _plan->_name, (unsigned long)(_consumed));
} // SortIterator::SortIterator

SortIterator::~SortIterator()
{
	TRACE(false);
	_output.close();

	traceprintf("%s produced %lu of %lu rows\n", _plan->_name, (unsigned long)(_produced), (unsigned long)(_consumed));
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
	std::cout << "flush" << std::endl;
	if (file.is_open()) {
		for (size_t i = 0; i < _output_buffer.size(); i++) {
			_output_buffer[i].writeToDisk(file);
		}
		file.close();
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
	/// can fit in cache, thus we do not apply multi level external sort at this part
	size_t level = static_cast<size_t>(ceil(log2(cache_run_cnt)));
	PriorityQueue CacheToMemory(level);

	for (size_t index = 0; index < cache_run_cnt; index++)
	{
		assert(!_memory[index].empty());
		std::cout << _memory[index].front() << std::endl;
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