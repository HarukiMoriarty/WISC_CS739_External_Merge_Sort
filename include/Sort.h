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

	/// Tricky:
	/// 1. Normally we will have one page of output buffer, but for simplify, we hold a
	/// big array here and flush this array when the whole sort for current run is done.
	std::vector<Row> _output_buffer;

	/// Cache Memory Level External Sort
	PriorityQueue _in_cache_priority_queue;	// Used for generate sorted cache run in memory
	std::vector<std::queue<Row> > _memory;	// Used to store cache run

	// Memory Disk Level External Sort
	PriorityQueue _memory_disk_priority_queue;
	std::vector<size_t> _graceful_degradation_vector;
	std::vector<std::ifstream> _fan_in_file_handlers;
	size_t sort_level;

	std::ifstream _output;

	/**
	 * @brief External Sort the data currently presented in cache
	 */
	void externalSortCacheMemory(size_t cache_run_cnt);

	/**
	 *
	 */
	void externalSortMemoryDisk();

	/**
	 *
	 */
	void compute_graceful_degradation(size_t memory_run);

	/**
	 * @brief Flush all data from memory to disk
	 *
	 */
	void flushMemory();
}; // class SortIterator
