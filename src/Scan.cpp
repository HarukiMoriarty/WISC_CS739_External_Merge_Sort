#include "Scan.h"

ScanPlan::ScanPlan(char const* const name, RowCount const count, size_t value_range)
	: Plan(name), _count(count), _value_range(value_range)
{
	TRACE(false);
} // ScanPlan::ScanPlan

ScanPlan::~ScanPlan()
{
	TRACE(false);
} // ScanPlan::~ScanPlan

Iterator* ScanPlan::init() const
{
	TRACE(false);
	return new ScanIterator(this);
} // ScanPlan::init

ScanIterator::ScanIterator(ScanPlan const* const plan) :
	_plan(plan), _count(0)
{
	TRACE(false);
} // ScanIterator::ScanIterator

ScanIterator::~ScanIterator()
{
	TRACE(false);
	traceprintf("produced %lu of %lu rows\n",
		(unsigned long)(_count),
		(unsigned long)(_plan->_count));
} // ScanIterator::~ScanIterator

bool ScanIterator::next(Row& row)
{
	TRACE(false);

	if (_count >= _plan->_count) return false;

	row.initData(_plan->_value_range); // Initialize row data.
	// row.printRow();
	++_count;
	return true;
} // ScanIterator::next

void ScanIterator::free(Row& row)
{
	// Since we are passing reference, we do not need to free anything here
	TRACE(false);
} // ScanIterator::free
