#pragma once

#include "defs.h"

typedef uint64_t RowCount;

class Row
{
public:
	Row();
	virtual ~Row();
	void setData(size_t range);
	void setOffset(const Row& other);
	bool operator<(const Row& other) const;

	// below are format function
	void printRow() const;
private:
	size_t data[ROW_LENTH];
	size_t offset[2];
}; // class Row

class Plan
{
	friend class Iterator;
public:
	Plan(char const* const name);
	virtual ~Plan();
	virtual class Iterator* init() const = 0;
protected:
	char const* const _name;
private:
}; // class Plan

class Iterator
{
public:
	Iterator();
	virtual ~Iterator();
	void run();
	virtual bool next(Row& row) = 0;
	virtual void free(Row& row) = 0;
private:
	RowCount _rows;
}; // class Iterator
