// include/util/min_heap.h
//
// YOU IMPLEMENT (this whole file; it is a template, so the code lives in the
// header). Spec §4.1.
//
// A binary MIN-heap stored in an array (a std::vector), plus top_k(), which
// uses a size-k min-heap to pick the k largest items in O(n log k).
//
// Rules for this file:
//   * Write sift-up and sift-down yourself.
//   * Do NOT use std::priority_queue, std::make_heap / push_heap / pop_heap /
//     sort_heap, std::sort / stable_sort / partial_sort / nth_element, or
//     std::set / multiset / map / multimap (full list: spec §7.3).
//   * std::vector is fine as the storage array; std::swap, std::reverse and
//     std::pair are fine too.
//
// `Less` is a "less-than" function object, exactly like the one std::sort
// takes: less(a, b) is true when a should come before b. The heap keeps the
// SMALLEST element (by less) at the top.

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

// Returns the k LARGEST items of `items` (by `less`), largest first.
//   * k == 0 or items empty -> empty result
//   * k >= items.size()     -> every item, largest first
// Must keep a MinHeap of at most k items (the k largest seen so far, with
// the smallest of them on top), so it runs in O(n log k) time and O(k) extra
// space. Items that compare equal may come out in any order.
template <typename T, typename Less = std::less<T>>
std::vector<T> top_k(const std::vector<T>& items, std::size_t k, Less less = Less()) {
    // TODO: keep a MinHeap<T, Less> of at most k items. For each item: if
    // the heap has fewer than k items, push it; otherwise, if the heap's top
    // is less than the item, pop the top and push the item. At the end, pop
    // everything (smallest first) and return it reversed.
    //
    // Clues:
    //  * k == 0: return {} BEFORE the loop (top() on an empty heap throws).
    //  * Build the heap WITH the comparator:  MinHeap<T, Less> heap(less);
    //    Writing  MinHeap<T, Less> heap;  fails for lambdas ("use of deleted
    //    function ... lambda") because a lambda can't be default-constructed.
    //  * "top is less than the item" means  less(heap.top(), item),  not
    //    heap.top() < item.  Read less(a, b) as "a is WORSE than b": the heap
    //    keeps the k best items with the worst of them on top.
    //  * std::reverse (from <algorithm>) flips a vector.
    (void)items;
    (void)k;
    (void)less;
    return {};
}
