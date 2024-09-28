#pragma once

#include "defs.h"

typedef uint64_t RowCount;

class Row
{
public:
	Row();
	virtual ~Row();

	/**
	 * @brief Initializes the data array with random values based on the input range.
	 * @param range The maximum value for generating random data in the array.
	 */
	void initData(size_t range);

	/**
	 * @brief Retrieves the data at the specified index.
	 * @param index The index of the data array to access.
	 * @return The value at the specified index in the data array.
	 */
	size_t getData(size_t index) const;

	/**
	 * @brief Sets OVC based on the values of another Row object.
	 * @param other Another Row object from which to calculate the offset values.
	 */
	void setOffset(const Row& other);

	/**
	 * @brief Overloading operator <=, notice used for OVC comparation.
	 * @note We update corresponding offset during comparasion (side effect).
	 */
	bool operator<=(Row& other);

	/**
	 * @brief Overloading operator >=, notice used for data comparation.
	 */
	bool operator>=(const Row& other) const;

	/**
	 * @brief Prints the contents of the Row (both data and offset arrays).
	 */
	void printRow() const;

	/**
	 * @brief Write the data to the file.
	 * @param file Reference to a file stream.
	 */
	void writeToDisk(std::ofstream& file) const;

	/**
	 * @brief Read the data from the disk.
	 * @param file Reference to a file stream.
	 */
	bool readFromDisk(std::ifstream& file);
private:
	size_t data[ROW_LENTH];	// Array to store the data.
	size_t offset[2];		// Array to store the OVC.
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
