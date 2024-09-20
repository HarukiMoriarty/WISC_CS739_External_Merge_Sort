#include "Iterator.h"

struct Predicate {
	size_t _index;
	size_t _value;

	Predicate(size_t index, size_t value) : _index(index), _value(value) {}
};

class FilterPlan : public Plan
{
	friend class FilterIterator;
public:
	FilterPlan(char const* const name, Plan* const input, std::vector<Predicate> predicates);
	~FilterPlan();
	Iterator* init() const;
private:
	Plan* const _input;
	std::vector<Predicate> _predicates;
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
