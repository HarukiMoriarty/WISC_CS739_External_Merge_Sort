#pragma once

#include "defs.h"
#include "iostream"

typedef uint64_t RowCount;

class Row
{
public:
	Row();

	/**
	 * @brief Generate a fence key.
	 */
	Row(bool fence, size_t offset);
	virtual ~Row();

	friend std::ostream& operator<<(std::ostream& os, const Row& row) {
		if (row.ovc[0] == 0) os << "early_fence(" << row.ovc[1] << ")";
		else if (row.ovc[0] == 2) os << "late_fence(" << row.ovc[1] << ")";
		else {
			os << row.ovc[1] << "@" << row.ovc[2] << " | ";
			for (size_t i = 0; i < row.data.size(); i++)
			{
				os << row.data[i] << " ";
			}
		}

		return os;
	}

	bool operator!=(const Row& other) const {
		return ovc[0] != other.ovc[0] || ovc[1] != other.ovc[1] || ovc[2] != other.ovc[2];
	}

	bool operator==(const Row& other) const {
		return ovc[0] == other.ovc[0] && ovc[1] == other.ovc[1] && ovc[2] == other.ovc[2];
	}

	/**
	 * @brief Overloading opeartor <, notice used for OVC comparasion in external sort.
	 */
	bool operator<(const Row& other) const {
		for (size_t i = 0; i < ovc.size(); i++) {
			if (ovc[i] < other.ovc[i]) {
				return true;
			}
			else if (ovc[i] > other.ovc[i]) {
				return false;
			}
		}
		return false;
	}

	/**
	 * @brief Overloading operator >=, notice used for full data comparasion in Witness.
	 */
	bool operator>=(const Row& other) const {
		// Compare the data arrays element by element.
		for (size_t i = 0; i < ROW_LENGTH; ++i)
		{
			if (data[i] > other.data[i]) {
				return true;
			}
			else if (data[i] < other.data[i]) {
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief Data comparasion given the same OVC.
	 * @param other
	 * @param offset
	 */
	bool less(Row& other, size_t& offset);

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
	 * @brief Sets data by a given vector, notice this function should only be used in test.
	 * @param content
	 */
	void setData(std::vector<size_t> content);

	/**
	 * @brief Gets fence bit in OVC code
	 */
	size_t getFence() const;

	/**
	 * @brief Gets offset in OVC code
	 */
	size_t getOffset() const;

	/**
	 * @brief Gets value in OVC code
	 */
	size_t getValue() const;

	/**
	 * @brief Check if current row is a early/late fence
	 */
	bool isFence() const;

	/**
	 * @brief Directly set OVC codes
	 * @param offset
	 * @param value
	 */
	void setOVC(size_t offset, size_t value);

	/**
	 * @brief Sets OVC based on the values of another Row object.
	 * @param base Another Row object from which to calculate the ovc values.
	 */
	void calOVC(const Row& base);

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
	std::vector<size_t> data;		// Array to store the data.
	std::vector<size_t> ovc;		// Array to store the OVC.
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
