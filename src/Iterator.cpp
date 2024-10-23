#include "Iterator.h"

Row::Row()
{
	TRACE(false);

	data.resize(ROW_LENGTH, 0);
	offset.resize(2, 0);
} // Row::Row

Row::~Row()
{
	TRACE(false);
} // Row::~Row

void Row::initData(size_t range)
{
	TRACE(false);
	for (size_t i = 0; i < data.size(); i++) {
		data[i] = Random(range);
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
	for (size_t i = 0; i < data.size(); i++)
	{
		file << data[i] << " ";
	}

	// Write offset array
	for (size_t i = 0; i < offset.size(); i++)
	{
		file << offset[i] << " ";
	}

	// End with a newline to seperate rows
	file << "\n";
}

bool Row::readFromDisk(std::ifstream& file)
{
	// Read data array
	for (size_t i = 0; i < data.size(); i++) {
		if (!(file >> data[i])) {
			return false;
		}
	}

	// Read offset array
	for (size_t i = 0; i < offset.size(); i++) {
		if (!(file >> offset[i])) {
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
