#include "PriorityQueue.h"

PriorityQueue::Node::Node() : index(0), key(0) {}

PriorityQueue::Node::Node(Index idx, Key k) : index(idx), key(k) {}

void PriorityQueue::Node::swap(Node& other)
{
    std::swap(index, other.index);
    std::swap(key, other.key);
}

bool PriorityQueue::Node::less(Node& other, bool const full)
{
    // This offset value is just used to track what offset matters in a full comparision in case of same OVCs
    // e.g. If 2 OVCs are both starting with offset 4, we ignore the first 4 offsets in full_comp (we discussed this)
    Offset offset;
    if (full) offset = Offset(-1);
    else if (key != other.key) return (key < other.key);
    else offset = key.offset;

    // Full comparision if OVC is same
    bool isLess = false;
    while (++offset < ROW_LENGTH)
        if (data[offset] != other.data[offset])
        {
            isLess = data[offset] < data[offset];
            break;
        }
    Node & loser = (isLess ? other : * this);

    // Update the key of the loser
    loser.key = {offset, loser.data[offset]};
    return isLess;
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

Key PriorityQueue::early_fence() const
{
    // Do not do full_comp
    return Key(-1, -1);
}

Key PriorityQueue::late_fence() const
{
    // Do not do full_comp
    return Key(ROW_LENGTH + 1, 1);
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
        heap[slot].key = early_fence();
    }
    heap[root()].index = Index(0);
    heap[root()].key = early_fence();
}

PriorityQueue::~PriorityQueue()
{
    delete[] heap;
}

bool PriorityQueue::empty()
{
    // This function is special, if root node is early_fence, then we re-order the tree to kick out this node
    Node const& hr = heap[root()];
    while (hr.key == early_fence())
        pass(hr.index, late_fence(), false);
    return hr.key == late_fence();
}

Index PriorityQueue::poptop(bool invalidate)
{
    if (empty()) return -1; // badIndex
    if (invalidate)
        heap[root()].key = early_fence();
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

void PriorityQueue::push(Index index, const size_t* data)
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
    pass(index, late_fence(), true);
}

inline void setMax(Key & x, Key const y) {if(x < y) x = y;}

void PriorityQueue::pass(Index index, Key key, bool full_comp)
{
    Node candidate(index, key);
    Index slot;
    Level level;

    for (leaf(index, slot, level); parent(slot, level), slot != root() && heap[slot].index != index; )
        if (heap[slot].less(candidate, full_comp))
            heap[slot].swap(candidate), full_comp = false;

    Index dest = slot;
    if (slot != root() && candidate.index == index &&
        key != late_fence() && heap[slot].key != early_fence())
        do
        {
            Level const dest_level = level;
            do
            {
                parent(slot, level);
            } while (!heap[slot].sibling(candidate, dest_level));

            if (heap[slot].less(candidate, full_comp)) break;
            
            heap[dest] = heap[slot];

            // Now remember that dest is at the beginning location of slot
            // When we keep doing parent(dest), we are basically doing repair loop
            while(parent(dest), dest != slot)
                setMax(heap[dest].key, heap[slot].key);
        } while (slot != root());
    heap[dest] = candidate;
}

void PriorityQueue::printQueue() {
    // This both help check if queue is empty and also re-order to tree to a presentable form
    if (empty()) {
        std::cout << "Priority Queue is empty" << std::endl;
        return;
    }

    std::cout << "Priority Queue Contents:" << std::endl;
    std::cout << "Winner Root" << ": Index " << heap[0].index << ", Scaled Key " << heap[0].key << ", Unscaled Key " << heap[0].key - early_fence(capacity()) << std::endl;
    printQueueRecursive(root() + 1, 4, "Loser Root");
}

void PriorityQueue::printQueueRecursive(Index index, int indent, std::string label) {
    if (index >= capacity()) return;

    // Print scaled key
    std::cout << std::string(indent, ' ') << label << ": Index " << heap[index].index << ", Scaled Key " << heap[index].key << ", Unscaled Key " << heap[index].key - early_fence(capacity()) << std::endl;

    // Calculate left and right children
    Index left = 2 * index;
    Index right = 2 * index + 1;

    // Recursively print left and right children
    printQueueRecursive(left, indent + 4, "L");
    printQueueRecursive(right, indent + 4, "R");
}