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

void Row::setData()
{
	TRACE(true);
	for (auto& item : data) {
		item = Random(SIZE_MAX);
	}
} // Row::setData

void Row::setOffset(Row& other)
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

bool Row::compare(Row& other) {
	// Compare the OVC first, element by element
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

	// If the OVC are equal, compare the data
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
	}

	// If both offset and data arrays are equal, return whatever
	return true;
}

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
