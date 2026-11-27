// include/util/lru_cache.h
//
// YOU IMPLEMENT (this whole file; it is a template). Spec §4.2.
//
// A fixed-capacity cache that evicts the Least Recently Used entry.
// Both get() and put() must be O(1) (expected), so it is built from two
// structures that point at the same nodes:
//
//   std::unordered_map<Key, Node*>   finds a key's node in O(1)
//   a doubly linked list of Nodes    keeps recency order:
//        head_ = most recently used ... tail_ = least recently used
//
// Rules for this file:
//   * Write the doubly linked list yourself (Node with prev/next pointers,
//     new/delete inside this class only). Do NOT use std::list, std::deque
//     or std::forward_list (don't even #include their headers), or a
//     std::vector you shift around (spec §7.3).
//   * std::unordered_map is allowed (and expected) for the key -> Node* index.
//   * No leaks, no double frees: ASan runs on every test.

#pragma once
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class LruCache {
public:
    struct Stats {
        std::size_t hits = 0;        // get() found the key
        std::size_t misses = 0;      // get() did not find the key
        std::size_t evictions = 0;   // put() removed the LRU entry to make room
    };

    // A cache holding at most `capacity` entries. Capacity 0 is legal: such a
    // cache never stores anything (every get() is a miss).
    explicit LruCache(std::size_t capacity) : capacity_(capacity) {}

    // Frees every node.
    ~LruCache() { clear(); }

    // Copying a cache of raw-pointer nodes is not needed; forbid it.
    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;

    // Hit: marks the entry most recently used, counts a hit, and returns a
    //      pointer to its value (valid until the next put/erase/clear).
    // Miss: counts a miss and returns nullptr.
    const Value* get(const Key& key) {
        // TODO
        // Clue: look keys up with  auto it = map_.find(key);  and compare
        // with map_.end(). Do NOT use map_[key] to look up: on a miss it
        // INSERTS the key (with a nullptr node), size() grows, and a later
        // put() crashes far away from the real bug.
        (void)key;
        return nullptr;
    }

    // Key present: replaces its value and marks it most recently used (no
    //   eviction, no hit/miss counted).
    // Key absent: if the cache is full, evicts the least recently used entry
    //   (counting one eviction), then inserts the new entry as most recently
    //   used. With capacity 0, does nothing.
    void put(const Key& key, Value value) {
        // TODO
        // Clue (eviction): victim = tail_; unlink it; erase victim->key from
        // the map; THEN delete victim. Reading victim->key after delete is a
        // heap-use-after-free (ASan may show it as a SEGV inside hashtable
        // code: look for the first stack frame in lru_cache.h).
        // Clue: create nodes with  new Node{key, std::move(value), nullptr, nullptr}.
        (void)key;
        (void)value;
    }

    // True if the key is cached. Does NOT change recency or stats.
    bool contains(const Key& key) const {
        // TODO
        // Clue: this function is const, so map_[key] does not even compile
        // ("discards qualifiers"). Use map_.find(key) != map_.end() or
        // map_.count(key).
        (void)key;
        return false;
    }

    // Removes the key if present (not an eviction). Returns whether it was.
    bool erase(const Key& key) {
        // TODO
        (void)key;
        return false;
    }

    // Removes every entry. Stats are kept.
    void clear() {
        // TODO: delete every node, empty the map, reset head_/tail_.
        // Clue: save n->next in a local BEFORE  delete n;  a loop like
        //   for (Node* n = head_; n; n = n->next) delete n;
        // reads n->next from freed memory.
    }

    std::size_t size() const noexcept { return map_.size(); }
    std::size_t capacity() const noexcept { return capacity_; }
    Stats       stats() const noexcept { return stats_; }

    // Keys from most to least recently used, by walking the list from head_.
    // Used by the tests to check your recency order.
    std::vector<Key> keys_mru_to_lru() const {
        std::vector<Key> out;
        for (Node* n = head_; n != nullptr; n = n->next) out.push_back(n->key);
        return out;
    }

private:
    struct Node {
        Key   key;
        Value value;
        Node* prev;
        Node* next;
    };
    
    /**
     * Function: unlink(node)
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: removes given node from a linked list
     */
    void unlink(Node* node) {
        // if node->prev exists
        if (node->prev != nullptr) {
            node->prev->next = node->next;      // prev->next skips over node
        } else {
            head_ = node->next;                 // else, reassign head to node->next
        }

        // if node->next exists
        if (node->next != nullptr) {
            node->next->prev = node->prev;      // next->prev skips over node
        } else {
            tail_ = node->prev;                 // else, reassign tail to node->prev
        }
    }

    /**
     * Function: pushFront(node)
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: insert given node as the new head node
     */
    void pushFront(Node* node) {
        // empty list
        if (head_ == nullptr) {
            head_ = node;           // node become head and tail
            tail_ = node;
            node->prev = nullptr;   // node points to nothing
            node->next = nullptr;
        }
        
        // non-empty list
        else {
            node->next = head_;     // node points to head
            head_->prev = node;     // head points to node; creates doubly link
            head_ = node;           // reassign head to different node
            node->prev = nullptr;   // node doesn't point to prev
        }
    }

    /**
     * Function: moveToFront(node)
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: moves a node in a list to the head
     */
    void moveToFront(Node* node) {
        // return if node is already the head
        if (head_ == node) {
            return;
        }

        // else, unlink then push front
        else {
            unlink(node);
            pushFront(node);
        }
    }

    std::unordered_map<Key, Node*, Hash> map_;
    Node*       head_ = nullptr;
    Node*       tail_ = nullptr;
    std::size_t capacity_;
    Stats       stats_;
};
