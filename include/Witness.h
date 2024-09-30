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

	/**
	 * @brief Calculates and updates the parity for the provided row.
	 * @param row Reference to the Row object for which to calculate parity.
	 */
	void calculateParity(Row& row);

	/**
	 * @brief Writes values for verification.
	 */
	void writeObservation();

	/**
	 * @brief Checks if the current row is in the correct order compared to the last row.
	 * @param row Reference to the current Row object to be checked.
	 */
	void checkOrder(Row& row);
private:
	WitnessPlan const* const _plan;
	Iterator* const _input;
	RowCount _rows;

	size_t _parity[ROW_LENTH]; 	// Array to store the calculated parity values.
	bool in_order;				// Flag indicating if rows are being sorted.	
	Row last_row;				// Stores the last row to verify the order.
}; // class WitnessIterator
