#include "Scan.h"

ScanPlan::ScanPlan(char const* const name, RowCount const count)
	: Plan(name), _count(count)
{
	TRACE(true);
} // ScanPlan::ScanPlan

ScanPlan::~ScanPlan()
{
	TRACE(true);
} // ScanPlan::~ScanPlan

Iterator* ScanPlan::init() const
{
	TRACE(true);
	return new ScanIterator(this);
} // ScanPlan::init

ScanIterator::ScanIterator(ScanPlan const* const plan) :
	_plan(plan), _count(0)
{
	TRACE(true);
} // ScanIterator::ScanIterator

ScanIterator::~ScanIterator()
{
	TRACE(true);
	traceprintf("produced %lu of %lu rows\n",
		(unsigned long)(_count),
		(unsigned long)(_plan->_count));
} // ScanIterator::~ScanIterator

bool ScanIterator::next(Row& row)
{
	TRACE(true);

	if (_count >= _plan->_count)
		return false;

	row.setData(VALUE_RANGE);
	row.printRow();
	++_count;
	return true;
} // ScanIterator::next

void ScanIterator::free(Row& row)
{
	TRACE(true);
} // ScanIterator::free
