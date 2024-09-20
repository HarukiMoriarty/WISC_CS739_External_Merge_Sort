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

void Row::initData(size_t range)
{
	TRACE(true);
	for (auto& item : data) {
		item = Random(range);
	}
} // Row::setData

size_t Row::getData(size_t index) const
{
	assert(index < ROW_LENTH);
	return data[index];
} // Row::getData

void Row::setOffset(const Row& other)
{
	TRACE(true);

	for (size_t i = 0; i < ROW_LENTH; ++i)
	{
		if (data[i] != other.data[i])
		{
			offset[0] = ROW_LENTH - i;
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
	for (size_t i = 0; i < 2; ++i)
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
	for (size_t i = 0; i < ROW_LENTH; ++i)
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

void Row::printRow() const
{
	printf("Row data: ");
	for (size_t i = 0; i < ROW_LENTH; ++i)
	{
		printf("%zu ", data[i]);
	}

	printf(" Row offset: ");
	for (size_t i = 0; i < 2; ++i)
	{
		printf("%zu ", offset[i]);
	}
	printf("\n");
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
