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
private:
	WitnessPlan const* const _plan;
	Iterator* const _input;
	RowCount _rows;
}; // class WitnessIterator
