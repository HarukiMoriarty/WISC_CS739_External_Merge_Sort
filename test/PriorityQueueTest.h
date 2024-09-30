#include <cassert>
#include "PriorityQueue.h"

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
    pq.push(1, 10);
    // pq.printQueue();
    pq.push(2, 5);
    pq.push(3, 15);
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
    pq.push(1, 10);
    pq.push(2, 5);
    pq.update(1, 0); // Lower priority value makes it top in max-heap
    assert(pq.top() == 1);
}

void testRemove()
{
    PriorityQueue pq(4);
    pq.push(1, 10);
    pq.push(2, 5);
    pq.push(3, 15);
    pq.remove(2);
    assert(pq.top() == 1); // Item 1 has next smallest priority
    assert(pq.pop() == 1);
    assert(pq.top() == 3);
    assert(pq.pop() == 3);
}

void testMultipleItemsSamePriority()
{
    PriorityQueue pq(4);
    pq.push(1, 10);
    pq.push(2, 10);
    pq.push(3, 10);
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
    unsigned int indexTracker[] = { 0, 0, 0, 0, 0 };

    // Pop and verify order
    unsigned int expectedIndices[] = { 0, 2, 1, 4, 0, 0, 2, 3, 0, 1, 1, 2, 2, 1, 3, 3, 3, 4, 4, 4 };
    unsigned int popCount = 0;

    pq.push(0, array[0][0]);
    pq.push(1, array[1][0]);
    pq.push(2, array[2][0]);
    pq.push(3, array[3][0]);
    pq.push(4, array[4][0]);

    while (!pq.empty())
    {
        pq.printQueue();
        unsigned int poppedValue = pq.pop();
        assert(poppedValue == expectedIndices[popCount]);
        popCount += 1;

        indexTracker[poppedValue] += 1;
        if (indexTracker[poppedValue] < 4)
        {
            pq.push(poppedValue, array[poppedValue][indexTracker[poppedValue]]);
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