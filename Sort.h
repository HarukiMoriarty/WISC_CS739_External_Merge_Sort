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

private:
	SortPlan const* const _plan;
	Iterator* const _input;
	RowCount _consumed, _produced;

	size_t _runIndex;
	std::vector<Row*> _memory;

	std::ifstream _output;

	/**
	 * @brief Sort the data currently presented in cache (quick-sort)
	 */
	void sortCache();

	/**
	 * @brief Flush all data from cache to memory
	 */
	void flushCache();

	/**
	 * @brief Clear cache
	 */
	void clearCache();

	/**
	 * @brief Sort the data currently presented in memory
	 * (Merge-join multiple sorted cache-sized chunks)
	 */
	void sortMemory();

	/**
	 * @brief Flush all data from memory to disk
	 * 
	 */
	void flushMemory();

	/**
	 * @brief Clear memory
	 */
	void clearMemory();

	/**
	 * @brief @brief Sort the data currently presented in disk
	 * (Merge-join multiple sorted memory-sized files)
	 */
	void sortDisk();
}; // class SortIterator
