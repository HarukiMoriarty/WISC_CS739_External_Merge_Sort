#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cstdio>
#include <cassert>
#include <vector>
#include <string>
#include <fstream>

typedef uint8_t byte;

#define slotsof(a)	(sizeof (a) / sizeof (a[0]))

#define nullptr	((void *) NULL)

#define yesno(b)	((b) ? "yes" : "no")

// call-through to assert() from <assert.h>
//
void Assert(bool const predicate,
	char const* const file, int const line);
//
#if defined ( _DEBUG )  ||  defined (DEBUG)
#define DebugAssert(b)	Assert ((b), __FILE__, __LINE__)
#else // _DEBUG DEBUG
#define DebugAssert(b)	(void) (0)
#endif // _DEBUG DEBUG
//
#define FinalAssert(b)	Assert ((b), __FILE__, __LINE__)
//	#define FinalAssert(b)	(void) (0)
//
#define ParamAssert(b)	FinalAssert (b)

#define traceprintf printf ("%s:%d:%s ", \
		__FILE__, __LINE__, __FUNCTION__), \
	printf

// -----------------------------------------------------------------

class Trace
{
public:

	Trace(bool const trace, char const* const function,
		char const* const file, int const line);
	~Trace();

private:

	void _trace(char const lead[]);

	bool const _output;
	char const* const _function;
	char const* const _file;
	int const _line;
}; // class Trace

#define TRACE(trace)	Trace __trace (trace, __FUNCTION__, __FILE__, __LINE__)

// -----------------------------------------------------------------

template <class Value> inline bool odd(
	Value const value, int bits_from_lsb = 0)
{
	return ((value >> bits_from_lsb) & Value(1)) != 0;
}
//
template <class Value> inline bool even(
	Value const value, int bits_from_lsb = 0)
{
	return ((Value(1) << bits_from_lsb) & value) == 0;
}

// templates for generic min and max operations
//
template <class Val> inline Val min(Val const a, Val const b)
{
	return a < b ? a : b;
}
//
template <class Val> inline Val max(Val const a, Val const b)
{
	return a > b ? a : b;
}
//
template <class Val> inline Val between(Val const value, Val const low, Val const high)
// 'value' but no less than 'low' and no more than 'high'
{
	return (value < low ? low : value > high ? high : value);
}
//
template <class Val> inline void extremes(Val const value, Val& low, Val& high)
// retain 'value' in 'low' or 'high' keeping track of extremes
{
	if (value < low) low = value;
	if (value > high) high = value;
}

// templates for generic division
// should work for any integer type
//
template <class Val> inline Val divide(Val const a, Val const b)
{
	return 1 + (a - 1) / b;
} // divide
//
template <class Val> inline Val roundup(Val const a, Val const b)
{
	return b * divide(a, b);
} // roundup

template <class Val> inline void exchange(Val& a, Val& b)
{
	Val const c = a;
	a = b;
	b = c;
} // exchange

// -----------------------------------------------------------------

template <class Val> inline Val mask(int const from, int const to)
{
	// from: index of lsb set to 1, range 0..31
	// to: index of msb set to 1, range from..31
	// from==to ==> single bit set
	//
	return (((Val)2) << to) - (((Val)1) << from);
} // Value mask (int from, int to)

size_t Random(size_t const range);
size_t Random(size_t const low_incl, size_t const high_incl);
size_t RoundDown(size_t const x, size_t const y);
size_t RoundUp(size_t const x, size_t const y);
bool IsPowerOf2(size_t const x);
size_t lsb(size_t const x);
size_t msb(size_t const x);
int msbi(size_t const x);
char const* YesNo(bool const b);
char const* OkBad(bool const b);

struct Predicate {
	size_t _index;
	size_t _value;

	Predicate(size_t index, size_t value) : _index(index), _value(value) {}
};

// Following are experiment config
/// Row Length: normally, the length of a record is fixed
#define ROW_LENGTH 10 		
/// Cache Capacity, we actually compute CACHE_FAN_IN from CACHE_CAPACITY
#define CACHE_CAPACITY 16	
/// Memory Capacity: for simplification, we assume memory capacity is integer times of cache capacity.
#define MEMORY_CAPACITY 1024
/// Memory Fan In: Maximum fan in for memory-disk priority queue, for simplification, we assume it is power of 2.
/// Question: How to build relation with MEMORY_CAPACITY and MEMORY_FAN_IN?
#define MEMORY_FAN_IN 32  
