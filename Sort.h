#include "Iterator.h"

// Helper function
int partition(std::vector<Row*>& rows, int low, int high);
void quickSort(std::vector<Row*>& rows, int low, int high);

class SortPlan : public Plan
{
	friend class SortIterator;
public:
	SortPlan(char const* const name, Plan* const input);
	~SortPlan();
	Iterator* init() const;
private:
	Plan* const _input;
}; // class SortPlan

class SortIterator : public Iterator
{
public:
	SortIterator(SortPlan const* const plan);
	~SortIterator();
	bool next(Row& row);
	void free(Row& row);
	void internalSort();
	void writeRunToDisk();
	void clearMemory();
private:
	SortPlan const* const _plan;
	Iterator* const _input;
	RowCount _consumed, _produced;

	size_t _runIndex;
	std::vector<Row*> _memory;

	std::ifstream _output;
}; // class SortIterator
