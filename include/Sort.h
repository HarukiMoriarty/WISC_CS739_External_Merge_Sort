#include <cmath>
#include <queue>

#include "Iterator.h"
#include "PriorityQueue.h"

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

private:
	SortPlan const* const _plan;
	Iterator* const _input;
	RowCount _consumed, _produced;

	size_t _runIndex;

	// Cache Memory Level External Sort
	PriorityQueue _cache_priority_queue;
	std::vector<std::queue<Row> > _memory;
	/// Tricky:
	/// 1. Normally we will have one page of output buffer, but for simplify, we hold a
	/// big array here and flush this array when the whole exteral sort for current memory run is done.
	std::vector<Row> _output_buffer;

	std::ifstream _output;

	/**
	 * @brief Sort the data currently presented in cache (quick-sort)
	 */
	void externalSortCacheMemory(size_t cache_run_cnt);

	/**
	 * @brief Flush all data from memory to disk
	 *
	 */
	void flushMemory();
}; // class SortIterator
