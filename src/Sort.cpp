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
	_plan(plan), _input(plan->_input->init()),
	_consumed(0), _produced(0), _runIndex(0)
{
	TRACE(false);

	// Collect data during initialization.
	for (Row row; _input->next(row); _input->free(row))
	{
		++_consumed;
		Row* newRow = new Row(row);
		_memory.push_back(newRow);

		if (_memory.size() >= MEMORY_CAPACIY)
		{
			sortMemory();
			flushMemory();
			clearMemory();
		}
	}

	if (!_memory.empty())
	{
		sortMemory();
		flushMemory();
		clearMemory();
	}
	delete _input;

	// Notice now we do not do external sort, so we just copy run_0 here for debugging.
	_output.open("disk/run_0");
	traceprintf("%s consumed %lu rows\n", _plan->_name, (unsigned long)(_consumed));
} // SortIterator::SortIterator

SortIterator::~SortIterator()
{
	TRACE(false);

	for (size_t i = 0; i < _memory.size(); i++)
	{
		delete _memory[i];
	}
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

void SortIterator::sortMemory()
{
	if (!_memory.empty())
	{
		quickSort(_memory, 0, _memory.size() - 1);
		// We calculate offset for each run.
		for (size_t i = 1; i < _memory.size(); i++) {
			_memory[i]->calOVC(*_memory[0]);
		}
	}
}

void SortIterator::flushMemory()
{
	std::ofstream file("disk/run_" + std::to_string(_runIndex));
	if (file.is_open()) {
		for (size_t i = 0; i < _memory.size(); i++) {
			_memory[i]->writeToDisk(file);  // Replace this with actual Row serialization logic
		}
		file.close();
		_runIndex++;
	}
	else {
		printf("Error: Unable to open file run_%ld\n", _runIndex);
	}
}

void SortIterator::clearMemory()
{
	for (size_t i = 0; i < _memory.size(); i++) {
		delete _memory[i];
	}
	_memory.clear();
}