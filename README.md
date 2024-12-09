
# CS764 Final Project: External Merge Sort

**Authors:** Zhenghong Yu, Minh Phan

## Overview
We implemented external sort, simulating a real machine's environment with cache, memory and disk. These are our configurations, also in `./include/defs.h`:

```
ROW_LENGTH = 10 		
CACHE_CAPACITY = 16	
MEMORY_CAPACITY = 1024
MEMORY_FAN_IN = 32
```

## How to run our program
### Setup
Make sure you have a `./disk` folder created, then run the `Makefile`:
```bash
make clean && make
```

### Executables
```bash
./build/Main.exe
./build/Test.exe

# Optional
./tools/test.py
```

### `Main.exe` Usage

```bash
./build/Main.exe --row_number=100000
```
```bash
./build/Main.exe --row_number=66560 --value_range=100 --predicates=0:50,1:20
```
#### Command-Line Arguments

- **`--row_number=<num>`**:  
  Specifies how many rows to generate and process.  
  Optional. Default value is 100.

- **`--value_range=<num>`**:  
  Specifies the range of generated values (0 to `<num>`).  
  Optional. If not specified, the default is typically used.

- **`--predicates=index:value,index:value,...`**:  
  Specifies a list of predicates to filter rows.  
  Each predicate is given as `index:value`, where both are integers.  
  Optional. If not specified, no filtering is applied.

## Testing
### 1. Correctness
- Tree of Loser Priority Queue (used for both internal and external sorting): run Unit Test `./build/Test.exe`.
- You can run:
    - `./build/Main.exe --row_number=16` internal cache sort
    - `./build/Main.exe --row_number=1000` cache to memory sort
    - `./build/Main.exe --row_number=10000` (or larger) memory to disk sort
- A file `witness.output` should be created containing Witness Iterator's output (sort order check & parity check).

### 2. Performance
- Row Comparisions: In progress
- Graceful Degradation: We make sure our optimization works as expected by checking the total amount of disk flushs. These are some example:
    - `./build/Main.exe --row_number=66560` should print out ... (Printing out Disk flusks is a little tricky because our implementation is flushing the whole big run instead of memory sized runs I think, will fix this)

## Rubriks Reflection
We reflected the stuffs we have implemented with the given rubriks:

#### 1. Internal Sort
- We initially implemented quick sort, but later switch to merge sort after verifying that our Priority Queue functions correctly.
- We can test the correctness by sorting a small number of records.

#### 2. External Sort
- We implemented Tree of Loser Priority Queue and OVC in `./src/PriorityQueue.cpp`.
- We have Unit Tests for this in `./test/PriorityQueueTest.h`.

#### 3. Graceful Degradation
Our implementation is as follow: Say we start with `W` runs, `F` fan-ins. Then we calculate the initial merge `(W-2) % (F-1) + 2`, then merge with fan-in `F` for the rest of runs. After 1st merge step, say we obtain `W_1` new runs, then we redo the same calculation and continue merging until we merge everything.
- Internal → External
- 1-step merge → n-step merge
- Merge Planning

#### 4. Cache Runs
- We use internal sort for cache runs, then the result is stored in memory.

#### 5. Memory Runs
- If the runs from cache fills memory, then we do external merge sort (merge the cache runs currently stored in memomry) then flush them to disk.

#### 6. Recycling the Priority Queue
- Priority Queue is recycled in our code, we have one for cache→memory and one for memory→disk.

#### 7. Minimum number of row comparisions
- We used Tree of Loser Priority Queue with OVC, and this helps minimize full row comparision.

#### 8. Minimum number of column comparisions
- In case of similar OVC, we start comparing from the similar offset.

#### 9. Test Cases for Correctness
- Mentioned above.

#### 10. Test Cases for Performance
- Mentioned above.

#### 11. Iterators
- Scan: Initialize random records
- Witness: Row count, parity check, order correctness check
- Filter: We did a simple filter that takes in `index` and `value` and returns only record with `row[index] > value`.