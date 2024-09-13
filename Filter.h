#include "Iterator.h"

class FilterPlan : public Plan
{
	friend class FilterIterator;
public:
	FilterPlan(char const* const name, Plan* const input);
	~FilterPlan();
	Iterator* init() const;
private:
	Plan* const _input;
}; // class FilterPlan

class FilterIterator : public Iterator
{
public:
	FilterIterator(FilterPlan const* const plan);
	~FilterIterator();
	bool next(Row& row);
	void free(Row& row);
private:
	FilterPlan const* const _plan;
	Iterator* const _input;
	RowCount _consumed, _produced;
}; // class FilterIterator
