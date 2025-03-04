#include <cassert>
#include "PriorityQueue.h"

const size_t magic_base[ROW_LENGTH] = { 1, 1, 4, 5, 1, 4, 1, 9, 1, 9 };

void testEmptyQueue()
{
    PriorityQueue pq(4);
    // pq.printQueue();
    assert(pq.empty());
    // pq.printQueue();
}

void testPushAndPop()
{
    PriorityQueue pq(2);
    // pq.printQueue();

    Key key1 = Key();
    Key key2 = Key();
    Key key3 = Key();

    Key key_output_1 = Key();
    Key key_output_2 = Key();
    Key key_output_3 = Key();

    Key output = Key();

    const std::vector<size_t> data_1 = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 9 };
    const std::vector<size_t> data_2 = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 1 };
    const std::vector<size_t> data_3 = { 1, 1, 4, 5, 1, 4, 1, 9, 10, 9 };

    key1.setOVC(2, 5);
    key1.setData(data_1);
    key2.setOVC(2, 5);
    key2.setData(data_2);
    key3.setOVC(2, 10);
    key3.setData(data_3);

    key_output_1.setOVC(1, 9);
    key_output_1.setData(data_1);
    key_output_2.setOVC(2, 5);
    key_output_2.setData(data_2);
    key_output_3.setOVC(2, 10);
    key_output_3.setData(data_3);

    pq.push(1, key1);
    // pq.printQueue();
    pq.push(2, key2);
    pq.push(3, key3);
    // pq.printQueue();

    // std::cout << "testPushAndPop(): Top element: " << pq.top() << std::endl;
    assert(pq.top(output) == 2); // Expect minimum value
    // pq.printQueue();
    assert(pq.pop(output) == 2);
    assert(output == key_output_2);

    // std::cout << "testPushAndPop(): Top element after pop: " << pq.top() << std::endl;
    assert(pq.top(output) == 1);
    // pq.printQueue();
    assert(pq.pop(output) == 1);
    std::cout << output << " " << key_output_1 << std::endl;
    assert(output == key_output_1);

    // std::cout << "testPushAndPop(): Top element after second pop: " << pq.top() << std::endl;
    assert(pq.top(output) == 3);
    // pq.printQueue();
    assert(pq.pop(output) == 3);
    assert(output == key_output_3);
}

void testUpdatePriority()
{
    PriorityQueue pq(4);
    Key key1 = Key();
    Key key2 = Key();
    Key key1_update = Key();

    Key key_output = Key();

    Key output = Key();

    const std::vector<size_t> data_1 = { 1, 1, 4, 5, 1, 4, 1, 9, 10, 9 };
    const std::vector<size_t> data_2 = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 9 };
    const std::vector<size_t> data_1_update = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 1 };

    key1.setOVC(2, 10);
    key1.setData(data_1);
    key2.setOVC(2, 5);
    key2.setData(data_2);
    key1_update.setOVC(2, 5);
    key1_update.setData(data_1_update);

    key_output.setOVC(2, 5);
    key_output.setData(data_1_update);

    pq.push(1, key1);
    pq.push(2, key2);
    // pq.printQueue();
    pq.update(1, key1_update); // Lower priority value makes it top in max-heap
    // pq.printQueue();
    assert(pq.top(output) == 1);
    assert(output == key_output);
}

void testRemove()
{
    PriorityQueue pq(4);

    Key key1 = Key();
    Key key2 = Key();
    Key key3 = Key();

    Key key_output_1 = Key();
    Key key_output_3 = Key();

    Key output = Key();

    const std::vector<size_t> data_1 = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 9 };
    const std::vector<size_t> data_2 = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 1 };
    const std::vector<size_t> data_3 = { 1, 1, 4, 5, 1, 4, 1, 9, 10, 9 };

    key1.setOVC(2, 5);
    key1.setData(data_1);
    key2.setOVC(2, 5);
    key2.setData(data_2);
    key3.setOVC(2, 10);
    key3.setData(data_3);

    // Notice we directly call remove key 2 here, so there is no comparasion
    // happened between key 1 and key 2.
    key_output_1.setOVC(2, 5);
    key_output_1.setData(data_1);
    key_output_3.setOVC(2, 10);
    key_output_3.setData(data_3);

    pq.push(1, key1);
    pq.push(2, key2);
    pq.push(3, key3);
    pq.remove(2);

    assert(pq.top(output) == 1); // Item 1 has next smallest priority
    assert(pq.pop(output) == 1);
    assert(output == key_output_1);
    // pq.printQueue();

    assert(pq.top(output) == 3);
    assert(pq.pop(output) == 3);
    assert(output == key_output_3);
}

void testMultipleItemsSamePriority()
{
    PriorityQueue pq(4);
    Key key1 = Key();
    Key key2 = Key();
    Key key3 = Key();
    Key output = Key();

    const std::vector<size_t> data = { 1, 1, 4, 5, 1, 4, 1, 9, 5, 9 };

    key1.setOVC(2, 5);
    key1.setData(data);
    key2.setOVC(2, 5);
    key2.setData(data);
    key3.setOVC(2, 5);
    key3.setData(data);

    pq.push(1, key1);
    pq.push(2, key2);
    pq.push(3, key3);
    assert(pq.top(output) == 1 || pq.top(output) == 2 || pq.top(output) == 3);
    pq.pop(output);
    assert(pq.top(output) == 1 || pq.top(output) == 2 || pq.top(output) == 3);
    pq.pop(output);
    assert(pq.top(output) == 1 || pq.top(output) == 2 || pq.top(output) == 3);
    pq.pop(output);
}

void testOverlappingIndices()
{
    PriorityQueue pq(5);

    Key output = Key();

    int array[][4] =
    {
        {0, 4, 5, 8},
        {2, 9, 10, 13},
        {1, 6, 11, 12},
        {7, 14, 15, 16},
        {3, 17, 18, 19}
    };
    const std::vector<size_t> data = { 1, 1, 4, 5, 1, 4, 1, 9, 1, 9 };

    unsigned int indexTracker[] = { 0, 0, 0, 0, 0 };

    // Pop and verify order
    unsigned int expectedIndices[] = { 0, 2, 1, 4, 0, 0, 2, 3, 0, 1, 1, 2, 2, 1, 3, 3, 3, 4, 4, 4 };
    unsigned int popCount = 0;

    for (size_t i = 0; i < 5; i++) {
        Key key = Key();
        key.setData(data);
        key.setOVC(1, array[i][0]);
        pq.push(i, key);
    }

    while (!pq.empty())
    {
        // pq.printQueue();
        unsigned int poppedValue = pq.pop(output);
        assert(poppedValue == expectedIndices[popCount]);
        popCount += 1;

        indexTracker[poppedValue] += 1;
        if (indexTracker[poppedValue] < 4)
        {
            Key key = Key();
            key.setData(data);
            key.setOVC(1, array[poppedValue][indexTracker[poppedValue]]);
            pq.push(poppedValue, key);
        }
    }
}

int priorityQueueTest()
{
    testEmptyQueue();
    testPushAndPop();
    testUpdatePriority();
    testRemove();
    testMultipleItemsSamePriority();
    testOverlappingIndices();
    std::cout << "All Priority Queue Tests PASSED!" << std::endl;
    return 0;
}