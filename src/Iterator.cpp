#include "Iterator.h"

Row::Row()
{
	TRACE(false);

	data.resize(ROW_LENGTH, 0);
	ovc.resize(3, 0);
	ovc[0] = 1;
} // Row::Row

Row::Row(bool fence, size_t offset)
{
	TRACE(false);

	data.resize(ROW_LENGTH, 0);
	ovc.resize(3, 0);
	if (fence) ovc[0] = 0;
	else ovc[0] = 2;
	ovc[1] = offset;
} // Row::Row

Row::~Row()
{
	TRACE(false);
} // Row::~Row

bool Row::less(Row& other, size_t& offset) {
	bool isLess = false;
	while (--offset > 0)
	{
		if (data[ROW_LENGTH - offset] != other.data[ROW_LENGTH - offset])
		{
			isLess = data[ROW_LENGTH - offset] < other.data[ROW_LENGTH - offset];
			break;
		}
	}
	Row& loser = (isLess ? other : *this);

	if (!other.isFence() && !isFence())
	{
		loser.ovc[1] = offset;
		loser.ovc[2] = loser.data[offset];
	}
	return isLess;
}

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

void Row::setData(std::vector<size_t> content)
{
	data = content;
}

size_t Row::getFence() const
{
	return ovc[0];
} // Row::getFence 

size_t Row::getOffset() const
{
	return ovc[1];
} // Row::getOffset

size_t Row::getValue() const
{
	return ovc[2];
} // Row::getValue

bool Row::isFence() const
{
	return ovc[0] != 1;
} // Row:: isFence

void Row::setOVC(size_t offset, size_t value)
{
	ovc[0] = 1;
	ovc[1] = offset;
	ovc[2] = value;
} // Row::setOVC

void Row::calOVC(const Row& base)
{
	TRACE(false);

	for (size_t i = 0; i < ROW_LENGTH; ++i)
	{
		if (data[i] != base.data[i])
		{
			ovc[0] = ROW_LENGTH - i;
			ovc[1] = data[i];
			return;
		}
	}

	// Need to set OVC to 0 if equal.
	ovc[0] = 0;
	ovc[1] = 0;
} // Row::calOVC

void Row::writeToDisk(std::ofstream& file) const
{
	// Write data array
	for (size_t i = 0; i < data.size(); i++)
	{
		file << data[i] << " ";
	}

	// Write ovc array
	for (size_t i = 0; i < ovc.size(); i++)
	{
		file << ovc[i] << " ";
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

	// Read ovc array
	for (size_t i = 0; i < ovc.size(); i++) {
		if (!(file >> ovc[i])) {
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
