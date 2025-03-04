#include "PriorityQueue.h"

PriorityQueue::Node::Node() : index(0), key(Key()) {}

PriorityQueue::Node::Node(Index idx, Key k) : index(idx), key(k) {}

void PriorityQueue::Node::swap(Node& other)
{
    std::swap(index, other.index);
    std::swap(key, other.key);
}

bool PriorityQueue::Node::less(Node& other)
{
    // This offset value is just used to track what offset matters in a full comparision in case of same OVCs
    // e.g. If 2 OVCs are both starting with offset 4, we ignore the first 4 offsets in full_comp (we discussed this)
    Offset offset;
    if (key != other.key) return (key < other.key);
    else offset = key.getOffset();

    // Full comparision if OVC is same
    return key.less(other.key, offset);
}

bool PriorityQueue::Node::sibling(Node& other, Level level)
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
    // Do not do full_comp
    return Key(true, index);
}

Key PriorityQueue::late_fence(Index index) const
{
    // Do not do full_comp
    return Key(false, index);
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
    // This function is special, if root node is early_fence, then we re-order the tree to kick out this node
    Node const& hr = heap[root()];
    while (hr.key == early_fence(hr.index))
    {
        pass(hr.index, late_fence(hr.index));
    }
    return hr.key == late_fence(hr.index);
}

Index PriorityQueue::poptop(bool invalidate, Key& key)
{
    if (empty()) return -1; // badIndex
    key = heap[root()].key;
    if (invalidate)
        heap[root()].key = early_fence(heap[root()].index);
    return heap[root()].index;
}

Index PriorityQueue::top(Key& key)
{
    return poptop(false, key);
}

Index PriorityQueue::pop(Key& key)
{
    return poptop(true, key);
}

void PriorityQueue::push(Index index, Key key)
{
    pass(index, key);
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

inline void setMax(Key& x, Key const y) { if (x < y) x = y; }

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

            // Now remember that dest is at the beginning location of slot
            // When we keep doing parent(dest), we are basically doing repair loop
            while (parent(dest), dest != slot)
                setMax(heap[dest].key, heap[slot].key);
        } while (slot != root());
    heap[dest] = candidate;
}

void PriorityQueue::clear() {
    for (Index i = 0; i < capacity(); i++) {
        pass(i, early_fence(i));
    }
    return;
}

void PriorityQueue::printQueue() {
    // This both help check if queue is empty and also re-order to tree to a presentable form
    if (empty()) {
        std::cout << "Priority Queue is empty" << std::endl;
        return;
    }

    std::cout << "Priority Queue Contents:" << std::endl;
    std::cout << "Winner Root" << ": Index " << heap[0].index << ", Key " << heap[0].key << std::endl;
    printQueueRecursive(root() + 1, 4, "Loser Root");
}

void PriorityQueue::printQueueRecursive(Index index, int indent, std::string label) {
    if (index >= capacity()) return;

    // Print scaled key
    std::cout << std::string(indent, ' ') << label << ": Index " << heap[index].index << ", Key " << heap[index].key << std::endl;

    // Calculate left and right children
    Index left = 2 * index;
    Index right = 2 * index + 1;

    // Recursively print left and right children
    printQueueRecursive(left, indent + 4, "L");
    printQueueRecursive(right, indent + 4, "R");
}

void PriorityQueue::printQueueWOReorder() {
    std::cout << "Priority Queue Contents:" << std::endl;
    std::cout << "Winner Root" << ": Index " << heap[0].index << ", Key " << heap[0].key << std::endl;
    printQueueRecursive(root() + 1, 4, "Loser Root");
}