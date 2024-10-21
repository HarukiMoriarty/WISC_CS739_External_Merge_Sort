#include "Iterator.h"

Row::Row()
{
	TRACE(false);
	for (auto& item : data) {
		item = 0;
	}
	for (auto& item : offset) {
		item = 0;
	}
} // Row::Row

Row::~Row()
{
	TRACE(false);
} // Row::~Row

void Row::initData(size_t range)
{
	TRACE(false);
	for (auto& item : data) {
		item = Random(range);
	}
} // Row::setData

size_t Row::getData(size_t index) const
{
	assert(index < ROW_LENGTH);
	return data[index];
} // Row::getData

void Row::setOffset(const Row& other)
{
	TRACE(false);

	for (size_t i = 0; i < ROW_LENGTH; ++i)
	{
		if (data[i] != other.data[i])
		{
			offset[0] = ROW_LENGTH - i;
			offset[1] = data[i];
			return;
		}
	}

	// Need to set OVC to 0 if equal.
	offset[0] = 0;
	offset[1] = 0;
} // Row::setOffset

bool Row::operator<=(Row& other)
{
	// First compare the offset arrays element by element.
	// Tricky: if two rows build offset based on same basement, and there offset
	// is different, after comparasion the loser offset based on winner keep same.
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

	// If the offset arrays are equal, then compare the data arrays element by element 
	// start from the same offset postion.
	for (size_t i = offset[0] + 1; i < ROW_LENGTH; ++i)
	{
		if (data[i] < other.data[i])
		{
			other.offset[0] = i;
			other.offset[1] = other.data[i];
			return true;
		}
		else if (data[i] > other.data[i])
		{
			offset[0] = i;
			offset[1] = data[i];
			return false;
		}
	}

	// If both offset and data arrays are equal, return true.
	// Suppose we always choose the current one as the winner.
	other.offset[0] = 0;
	other.offset[1] = 0;
	return true;
} // Row::operator<=

bool Row::operator>=(const Row& other) const
{
	// Compare the data arrays element by element.
	for (size_t i = 0; i < ROW_LENGTH; ++i)
	{
		if (data[i] > other.data[i]) {
			return true;
		}
		else if (data[i] < other.data[i]) {
			return false;
		}
	}
	return true;
}
// Row::operator>=

void Row::printRow() const
{
	printf("Row data: ");
	for (size_t i = 0; i < ROW_LENGTH; ++i)
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

void Row::writeToDisk(std::ofstream& file) const
{
	// Write data array
	for (size_t val : data)
	{
		file << val << " ";
	}

	// Write offset array
	for (size_t val : offset)
	{
		file << val << " ";
	}

	// End with a newline to seperate rows
	file << "\n";
}

bool Row::readFromDisk(std::ifstream& file)
{
	// Read data array
	for (auto& item : data) {
		if (!(file >> item)) {
			return false;
		}
	}

	// Read offset array
	for (auto& item : offset) {
		if (!(file >> item)) {
			return false;
		}
	}

	return true;
}

Plan::Plan(char const* const name)
	: _name(name)
{
	TRACE(false);
} // Plan::Plan

Plan::~Plan()
{
	TRACE(false);
} // Plan::~Plan

Iterator::Iterator() : _rows(0)
{
	TRACE(false);
} // Iterator::Iterator

Iterator::~Iterator()
{
	TRACE(false);
} // Iterator::~Iterator

void Iterator::run()
{
	TRACE(false);

	for (Row row; next(row); free(row)) ++_rows;

	traceprintf("entire plan produced %lu rows\n", (unsigned long)_rows);
} // Iterator::run
