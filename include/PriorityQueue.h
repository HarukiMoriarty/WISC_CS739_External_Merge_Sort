#pragma once

#include <iostream>

typedef unsigned int Index;
typedef unsigned int Key;
typedef char Level;

struct Node {
    Index index;
    Key key;

    Node();

    Node(Index idx, Key k);

    void swap(Node &other);
    bool less(Node& other);
    bool sibling(Node& other, Level level);
};

class PriorityQueue {
private:
    Level height;
    Node* heap;
    unsigned int size;

    bool even(Index index, Level shift);
    Index capacity() const;
    Index root() const;
    void leaf(Index index, Index& slot) const;
    void leaf(Index index, Index& slot, Level& level) const;
    void parent(Index& slot);
    void parent(Index& slot, Level& level);
    void pass(Index index, Key key);
    Key early_fence(Index index) const;
    Key late_fence(Index index) const;
    void printQueueRecursive(Index index, int indent, std::string label);

public:
    PriorityQueue(Level h);
    ~PriorityQueue();

    bool empty();
    Index poptop(bool invalidate);
    Index top();
    Index pop();
    void push(Index index, Key key);
    void insert(Index index, Key key);
    void update(Index index, Key key);
    void remove(Index index);
    void printQueue();
};