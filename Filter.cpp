#include "Filter.h"

FilterPlan::FilterPlan(char const* const name, Plan* const input)
	: Plan(name), _input(input)
{
	TRACE(true);
} // FilterPlan::FilterPlan

FilterPlan::~FilterPlan()
{
	TRACE(true);
	delete _input;
} // FilterPlan::~FilterPlan

Iterator* FilterPlan::init() const
{
	TRACE(true);
	return new FilterIterator(this);
} // FilterPlan::init

FilterIterator::FilterIterator(FilterPlan const* const plan) :
	_plan(plan), _input(plan->_input->init()),
	_consumed(0), _produced(0)
{
	TRACE(true);
} // FilterIterator::FilterIterator

FilterIterator::~FilterIterator()
{
	TRACE(true);

	delete _input;

	traceprintf("produced %lu of %lu rows\n",
		(unsigned long)(_produced),
		(unsigned long)(_consumed));
} // FilterIterator::~FilterIterator

bool FilterIterator::next(Row& row)
{
	TRACE(true);

	for (;;)
	{
		if (!_input->next(row))  return false;

		++_consumed;
		if (_consumed % 2 != 0) // the fake filter predicate
			break;

		_input->free(row);
	}

	++_produced;
	return true;
} // FilterIterator::next

void FilterIterator::free(Row& row)
{
	TRACE(true);
	_input->free(row);
} // FilterIterator::free
