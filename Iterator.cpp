#include "Iterator.h"

Row::Row()
{
	TRACE(true);
	for (auto& item : data) {
		item = 0;
	}
	for (auto& item : offset) {
		item = 0;
	}
} // Row::Row

Row::~Row()
{
	TRACE(true);
} // Row::~Row

void Row::setData(size_t range)
{
	TRACE(true);
	for (auto& item : data) {
		item = Random(range);
	}
} // Row::setData

void Row::setOffset(const Row& other)
{
	TRACE(true);

	for (size_t i = 0; i < 5; ++i)
	{
		if (data[i] != other.data[i])
		{
			offset[0] = 5 - i;
			offset[1] = data[i];
			return;
		}
	}

	// Remember to set OVC to 0 if equal
	offset[0] = 0;
	offset[1] = 0;
} // Row::setOffset

bool Row::operator<(const Row& other) const
{
	// First compare the offset arrays element by element
	for (size_t i = 0; i < 3; ++i)
	{
		if (offset[i] < other.offset[i])
		{
			return true;
		}
		else if (offset[i] > other.offset[i])
		{
			return false;
		}
	}

	// If the offset arrays are equal, compare the data arrays
	for (size_t i = 0; i < 5; ++i)
	{
		if (data[i] < other.data[i])
		{
			return true;
		}
		else if (data[i] > other.data[i])
		{
			return false;
		}
		// If equal, continue to the next element
	}

	// If both offset and data arrays are equal, return false (this is not less than the other)
	return false;
} // Row::operator<

Plan::Plan(char const* const name)
	: _name(name)
{
	TRACE(true);
} // Plan::Plan

Plan::~Plan()
{
	TRACE(true);
} // Plan::~Plan

Iterator::Iterator() : _rows(0)
{
	TRACE(true);
} // Iterator::Iterator

Iterator::~Iterator()
{
	TRACE(true);
} // Iterator::~Iterator

void Iterator::run()
{
	TRACE(true);

	for (Row row; next(row); free(row))
		++_rows;

	traceprintf("entire plan produced %lu rows\n",
		(unsigned long)_rows);
} // Iterator::run
