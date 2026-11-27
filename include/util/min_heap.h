/**
 * File: min_heap.h
 * 
 * Author: Kevin Aroche Rios
 * Date: November 25, 2026
 * Description: a binary tree where parent nodes are smaller than their child nodes
 */

#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T, typename Less = std::less<T>>
class MinHeap {
public:
    explicit MinHeap(Less less = Less()) : less_(std::move(less)) {}

    /**
     * Function: push(value)
     * 
     * Author: Kevin Aroche Rios
     * Date: November 26, 2026
     * Description: push a new node into the right spot in a tree
     */
    void push(T value) {
        data_.push_back(std::move(value));      // add new node to the bottom of the tree
        siftUp(data_.size() - 1);               // sift up to the correct spot in the tree
    }

    /**
     * Function: top()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 26, 2026
     * Description: return root node of the tree unless empty
     */
    const T& top() const {
        // empty tree
        if (empty()){
            throw std::out_of_range("MinHeap::top heap is empty");
        }
        return data_[0];                        // root node
    }

    /**
     * Function: pop()
     *
     * Author: Kevin Aroche Rios
     * Date: November 26, 2026
     * Description: removes the root node and then fixes tree
     */
    T pop() {
        T root = top();                 // use top() to find root
        data_.front() = data_.back();   // last node temporarily replaces the root node
        data_.pop_back();               // remove last node

        // move temp root node down the tree to its correct spot
        if (!empty()){
            siftDown(0);
        }

        return root;
    }

    std::size_t size() const noexcept { return data_.size(); }
    bool        empty() const noexcept { return data_.empty(); }
    void        clear() noexcept { data_.clear(); }

private:
    /**
     * Function: siftUp()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 26, 2026
     * Description: sift a node up the tree to its correct spot
     */
    void siftUp(std::size_t i) {
        while (i > 0) {                     // stop at the root
            std::size_t parentIndex = (i - 1) / 2;

            // check if the child node should be above the parent
            if (!less_(data_[i], data_[parentIndex])) { break; }

            std::swap(data_[parentIndex], data_[i]);
            i = parentIndex;                // repeat at the new index
        }
    }

    /**
     * Function: siftDown()
     *
     * Author: Kevin Aroche Rios
     * Date: November 26, 2026
     * Description: sift a node down the tree to its correct spot
     */
    void siftDown(std::size_t i) {
        for (;;) {
            std::size_t leftChildIndex = 2*i + 1, rightChildIndex = leftChildIndex + 1, smallestChildIndex = i;

            // if the left child is smaller than the current smallest, set index to it
            if (leftChildIndex < data_.size() && less_(data_[leftChildIndex], data_[smallestChildIndex])){
                smallestChildIndex = leftChildIndex;
            }
            // if the right child is smaller than the current smallest, set index to it
            if (rightChildIndex < data_.size() && less_(data_[rightChildIndex], data_[smallestChildIndex])){
                smallestChildIndex = rightChildIndex;
            }
            // stop when no more smaller nodes are encountered
            if (smallestChildIndex == i) break;
            
            std::swap(data_[i], data_[smallestChildIndex]);     // swap node with smaller node
            i = smallestChildIndex;                             // follow the node to its new index
        }
    }

    std::vector<T> data_;
    Less           less_;
};

/**
 * Function: top_k(items, k, less)
 *
 * Author: Kevin Aroche Rios
 * Date: November 27, 2026
 * Description: find k largest items using a tree
 */
template <typename T, typename Less = std::less<T>>
std::vector<T> top_k(const std::vector<T>& items, std::size_t k, Less less = Less()) {
    // nothing to return and empty heap throws
    if (k == 0 || items.empty()) { return {}; }

    MinHeap<T, Less> heap(less);

    // check through every item
    for (std::size_t i = 0; i < items.size(); i++) {
        const T & currentItem = items[i];

        // size the tree to k nodes
        if (heap.size() < k) {
            heap.push(currentItem);     // new node added
            continue;                   // move on to next item
        }

        // top/root node is less than the current item
        if (less(heap.top(), currentItem)) {
            heap.pop();                 // remove root node
            heap.push(currentItem);     // add larger item
        }
    }

    std::vector<T> largestItems;

    // empty tree into largestItems
    while (!heap.empty()) {
        largestItems.push_back(heap.pop());
    }

    // flip order from smallest -> largest to largest -> smallest
    std::reverse(largestItems.begin(), largestItems.end());

    return largestItems;
}
