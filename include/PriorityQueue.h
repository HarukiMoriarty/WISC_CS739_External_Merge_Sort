#pragma once

#include <defs.h>
#include <iostream>
#include "Iterator.h"

typedef size_t Index;
typedef size_t Offset;
typedef char Level;
typedef Row Key;

class PriorityQueue {
private:
    /**
     * @brief Represents a node in the Priority Queue
     *
     */
    struct Node {
        Index index;
        Key key;

        Node();
        Node(Index idx, Key k);

        /**
         * @brief Swap values with another node
         *
         * @param other
         */
        void swap(Node& other);

        /**
         * @brief Compare with another node (TODO: need extension for OVC)
         *
         * @param other
         * @return true
         * @return false
         */
        bool less(Node& other);

        /**
         * @brief Check if two nodes are siblings by comparing parent's index (bit shifting)
         *
         * @param other
         * @param level
         * @return true
         * @return false
         */
        bool sibling(Node& other, Level level);
    };

    Level height;
    Node* heap;

    /**
     * @brief Check if index is even at a certain level.
     * Basically just help to init the tree with early_fence.
     *
     * @param index
     * @param shift
     * @return true
     * @return false
     */
    bool even(Index index, Level shift);

    /**
     * @brief Return the capacity of the tree (2^height)
     *
     * @return Index
     */
    Index capacity() const;

    /**
     * @brief Return the root of the tree (index 0)
     *
     * @return Index
     */
    Index root() const;

    /**
     * @brief Convert index (fan-in index) to actually index in the priority heap (slot)
     *
     * @param index
     * @param slot
     */
    void leaf(Index index, Index& slot) const;

    /**
     * @brief Init a leaf with level = 0
     *
     * @param index
     * @param slot
     * @param level
     */
    void leaf(Index index, Index& slot, Level& level) const;

    /**
     * @brief Just divide by 2 to find parent's index
     *
     * @param slot
     */
    void parent(Index& slot);

    /**
     * @brief Just a wrapper to add on level
     *
     * @param slot
     * @param level
     */
    void parent(Index& slot, Level& level);

    /**
     * @brief Give a pass to the node with a new key
     * (Ex: After we pop the winner, we push another node with similar index --> This is a Loser Pass)
     *
     * @param index
     * @param key
     */
    void pass(Index index, Key key);

    /**
     * @brief Key(early_fence) = index_value
     * The key to doing this is we do a little scaling to the actual key value to distinguish it from the real key.
     * We scale the key in the push() function and in pop() set the value back to early_fence.
     * Try printing out the tree and you will see the difference.
     *
     * @param index
     * @return Key
     */
    Key early_fence(Index index) const;

    /**
     * @brief Do a bit wise operation to get late_fence, this usually get a huge value without much effort
     *
     * @param index
     * @return Key
     */
    Key late_fence(Index index) const;

    /**
     * @brief Recursive function to print the queue
     *
     * @param index
     * @param indent
     * @param label
     */
    void printQueueRecursive(Index index, int indent, std::string label);

    /**
     * @brief Helper function for pop() or check top()
     *
     * @param invalidate
     * @param key
     * @return Index
     */
    Index poptop(bool invalidate, Key& key);

public:
    PriorityQueue(Level h);
    ~PriorityQueue();

    bool empty();
    Index top(Key& key);
    Index pop(Key& key);

    /**
     * @brief Basically add new node to the tree
     * Need to implement when-to-add logic in the Sort iterator
     *
     * @param index
     * @param key
     */
    void push(Index index, Key key);
    void insert(Index index, Key key);
    void update(Index index, Key key);

    /**
     * @brief Clear queue
     */
    void clear();

    /**
     * @brief Remove a node from the tree (using late_fence)
     * Use this when we want to remove a source of input
     *
     * @param index
     */
    void remove(Index index);

    /**
     * @brief Print the queue
     *
     */
    void printQueue();

    void printQueueWOReorder();
};