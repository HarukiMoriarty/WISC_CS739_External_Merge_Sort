#include "PriorityQueue.h"

Node::Node() : index(0), key(0) {}

Node::Node(Index idx, Key k) : index(idx), key(k) {}

void Node::swap(Node& other)
{
    std::swap(index, other.index);
    std::swap(key, other.key);
}

bool Node::less(Node& other)
{
    return key < other.key;
}

bool Node::sibling(Node& other, Level level)
{
    return (index >> level) == (other.index >> level);
}

bool PriorityQueue::even(Index index, Level shift)
{
    // Check if index is at a certain level
    return ((index >> shift) & Index(1)) == Index(0);
}

Index PriorityQueue::capacity() const
{
    // Capacity = 2^height
    return Index(1 << height);
}

Index PriorityQueue::root() const
{
    return Index(0);
}

void PriorityQueue::leaf(Index index, Index& slot) const
{
    slot = capacity() + index;
}

void PriorityQueue::leaf(Index index, Index& slot, Level& level) const
{
    level = 0;
    leaf(index, slot);
}

void PriorityQueue::parent(Index& slot)
{
    slot /= 2;
}

void PriorityQueue::parent(Index& slot, Level& level)
{
    ++level;
    parent(slot);
}

Key PriorityQueue::early_fence(Index index) const
{
    return index;
}

Key PriorityQueue::late_fence(Index index) const
{
    return ~index;
}

PriorityQueue::PriorityQueue(Level h) : height(h), heap(new Node[1 << h])
{
    for (Index index = capacity(); --index > 0; )
    {
        Level level;
        Index slot;

        leaf(index, slot, level); // Initialize leaf nodes
        do
        {
            parent(slot, level);
        } while (even(index, level - 1));

        heap[slot].index = index;
        heap[slot].key = early_fence(index);
    }
    heap[root()].index = Index(0);
    heap[root()].key = early_fence(Index(0));
}

PriorityQueue::~PriorityQueue()
{
    delete[] heap;
}

bool PriorityQueue::empty()
{
    Node const& hr = heap[root()];
    while (hr.key == early_fence(hr.index))
        pass(hr.index, late_fence(hr.index));
    return hr.key == late_fence(hr.index);
}

Index PriorityQueue::poptop(bool invalidate)
{
    if (empty()) return -1; // badIndex
    if (invalidate)
        heap[root()].key = early_fence(heap[root()].index);
    return heap[root()].index;
}

Index PriorityQueue::top()
{
    return poptop(false);
}

Index PriorityQueue::pop()
{
    return poptop(true);
}

void PriorityQueue::push(Index index, Key key)
{
    pass(index, early_fence(capacity()) + key);
}

void PriorityQueue::insert(Index index, Key key)
{
    push(index, key);
}

void PriorityQueue::update(Index index, Key key)
{
    push(index, key);
}

void PriorityQueue::remove(Index index)
{
    pass(index, late_fence(index));
}

void PriorityQueue::pass(Index index, Key key)
{
    Node candidate(index, key);
    Index slot;
    Level level;

    for (leaf(index, slot, level); parent(slot, level), slot != root() && heap[slot].index != index; )
        if (heap[slot].less(candidate))
            heap[slot].swap(candidate);

    Index dest = slot;
    if (slot != root() && candidate.index == index &&
        key != late_fence(index) && heap[slot].key != early_fence(index))
        do
        {
            Level const dest_level = level;
            do
            {
                parent(slot, level);
            } while (!heap[slot].sibling(candidate, dest_level));

            if (heap[slot].less(candidate)) break;
            heap[dest] = heap[slot];
            dest = slot;
        } while (slot != root());
    heap[dest] = candidate;
}

void PriorityQueue::printQueue() {
    std::cout << "Priority Queue Contents:" << std::endl;
    printQueueRecursive(1, 0, "Root");
}

void PriorityQueue::printQueueRecursive(Index index, int indent, std::string label) {
    if (index >= Index(1 << (height + 1))) return;

    // Print current node
    std::cout << std::string(indent, ' ') << label << ": Index " << heap[index].index << ", Key " << heap[index].key << std::endl;

    // Print tree connections
    Index left = 2 * index;
    Index right = 2 * index + 1;

    // Recursively print left and right children
    printQueueRecursive(left, indent + 4, "L");
    printQueueRecursive(right, indent + 4, "R");
}