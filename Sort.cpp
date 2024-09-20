#include "Sort.h"

// Helper function
int partition(std::vector<Row*>& rows, int low, int high)
{
	Row* pivot = rows[high];
	int i = low - 1;

	for (int j = low; j < high; ++j) {
		if (*rows[j] <= *pivot) {
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
	_consumed(0), _produced(0)
{
	TRACE(false);

	for (Row row; _input->next(row); _input->free(row))
	{
		++_consumed;
		Row* newRow = new Row(row);
		_data.push_back(newRow);
	}

	// It might looks like we perform all sort things here and keep it in some array
	delete _input;

	internalSort();

	traceprintf("%s consumed %lu rows\n",
		_plan->_name,
		(unsigned long)(_consumed));
} // SortIterator::SortIterator

SortIterator::~SortIterator()
{
	TRACE(false);

	for (Row* row : _data)
	{
		delete row;
	}

	traceprintf("%s produced %lu of %lu rows\n",
		_plan->_name,
		(unsigned long)(_produced),
		(unsigned long)(_consumed));
} // SortIterator::~SortIterator

bool SortIterator::next(Row& row)
{
	TRACE(false);

	if (_produced >= _consumed)  return false;
	row = *_data[_produced];
	++_produced;
	return true;
} // SortIterator::next

void SortIterator::free(Row& row)
{
	TRACE(false);
} // SortIterator::free

void SortIterator::internalSort()
{
	if (!_data.empty())
	{
		quickSort(_data, 0, _consumed - 1);
	}
}