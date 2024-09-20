#include "Iterator.h"

class WitnessPlan : public Plan
{
	friend class WitnessIterator;
public:
	WitnessPlan(char const* const name, Plan* const input);
	~WitnessPlan();
	Iterator* init() const;
private:
	Plan* const _input;
}; // class WitnessPlan

class WitnessIterator : public Iterator
{
public:
	WitnessIterator(WitnessPlan const* const plan);
	~WitnessIterator();
	bool next(Row& row);
	void free(Row& row);

	// functions used for checking correctness
	void calParity(Row& row);
	void printParity();
	void checkOrder(Row& row);
private:
	WitnessPlan const* const _plan;
	Iterator* const _input;
	RowCount _rows;

	// variables used for checking correctness
	size_t _parity[ROW_LENTH];
	bool in_order;
	Row last_row;
}; // class WitnessIterator
