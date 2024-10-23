#include <cassert>
#include "PriorityQueue.h"

const size_t magic_base[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 1, 9};

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

    Key key1 = Key(2, 5);
    Key key2 = Key(2, 5);
    Key key3 = Key(2, 10);

    const size_t data_1[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 9};
    const size_t data_2[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 1};
    const size_t data_3[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 10, 9};


    pq.push(1, key1, data_1);
    // pq.printQueue();
    pq.push(2, key2, data_2);
    pq.push(3, key3, data_3);
    // pq.printQueue();

    // std::cout << "testPushAndPop(): Top element: " << pq.top() << std::endl;
    assert(pq.top() == 2); // Expect minimum value
    // pq.printQueue();

    pq.pop();
    // std::cout << "testPushAndPop(): Top element after pop: " << pq.top() << std::endl;
    assert(pq.top() == 1);
    // pq.printQueue();

    pq.pop();
    // std::cout << "testPushAndPop(): Top element after second pop: " << pq.top() << std::endl;
    assert(pq.top() == 3);

    pq.pop();
}

void testUpdatePriority()
{
    PriorityQueue pq(4);
    Key key1 = Key(2, 5);
    Key key2 = Key(2, 10);
    Key key1_update = Key(2, 5);

    const size_t data_1[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 9};
    const size_t data_2[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 10, 9};
    const size_t data_1_update[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 1};

    pq.push(1, key1, data_1);
    pq.push(2, key2, data_2);
    // pq.printQueue();
    pq.update(1, key1_update, data_1_update); // Lower priority value makes it top in max-heap
    // pq.printQueue();
    assert(pq.top() == 1);
}

void testRemove()
{
    PriorityQueue pq(4);

    Key key1 = Key(2, 5);
    Key key2 = Key(2, 5);
    Key key3 = Key(2, 10);

    const size_t data_1[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 9};
    const size_t data_2[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 1};
    const size_t data_3[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 10, 9};

    pq.push(1, key1, data_1);
    pq.push(2, key2, data_2);
    pq.push(3, key3, data_3);
    pq.remove(2);
    assert(pq.top() == 1); // Item 1 has next smallest priority
    assert(pq.pop() == 1);
    // pq.printQueue();
    assert(pq.top() == 3);
    assert(pq.pop() == 3);
}

void testMultipleItemsSamePriority()
{
    PriorityQueue pq(4);
    Key key1 = Key(2, 5);
    Key key2 = Key(2, 5);
    Key key3 = Key(2, 5);

    const size_t data[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 5, 9};

    pq.push(1, key1, data);
    pq.push(2, key2, data);
    pq.push(3, key3, data);
    assert(pq.top() == 1 || pq.top() == 2 || pq.top() == 3);
    pq.pop();
    assert(pq.top() == 1 || pq.top() == 2 || pq.top() == 3);
    pq.pop();
    assert(pq.top() == 1 || pq.top() == 2 || pq.top() == 3);
    pq.pop();
}

void testOverlappingIndices()
{
    PriorityQueue pq(5);

    int array[][4] =
    {
        {0, 4, 5, 8},
        {2, 9, 10, 13},
        {1, 6, 11, 12},
        {7, 14, 15, 16},
        {3, 17, 18, 19}
    };
    const size_t data[ROW_LENGTH] = {1, 1, 4, 5, 1, 4, 1, 9, 1, 9};

    unsigned int indexTracker[] = { 0, 0, 0, 0, 0 };

    // Pop and verify order
    unsigned int expectedIndices[] = { 0, 2, 1, 4, 0, 0, 2, 3, 0, 1, 1, 2, 2, 1, 3, 3, 3, 4, 4, 4 };
    unsigned int popCount = 0;

    pq.push(0, Key(1, array[0][0]), data);
    pq.push(1, Key(1, array[1][0]), data);
    pq.push(2, Key(1, array[2][0]), data);
    pq.push(3, Key(1, array[3][0]), data);
    pq.push(4, Key(1, array[4][0]), data);

    while (!pq.empty())
    {
        // pq.printQueue();
        unsigned int poppedValue = pq.pop();
        assert(poppedValue == expectedIndices[popCount]);
        popCount += 1;

        indexTracker[poppedValue] += 1;
        if (indexTracker[poppedValue] < 4)
        {
            pq.push(poppedValue, Key(1, array[poppedValue][indexTracker[poppedValue]]), data);
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