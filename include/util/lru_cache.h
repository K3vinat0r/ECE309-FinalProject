/**
 * File: lru_cache.h
 * 
 * Author: Kevin Aroche Rios
 * Date: November 27, 2026
 * Description: a cache that removes the least recently used (lru)
 * entry in it when full
 */

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

    /**
     * Function: get()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: search for a key in a linked list; return pointer to its 
     * value if found or nullptr if not found
     */
    const Value* get(const Key& key) {
        auto findKey = map_.find(key);      // search for key

        // key is present in the cache
        if (findKey != map_.end()) {
            moveToFront(findKey->second);   // now most recently used, so move to the front
            stats_.hits++;                  // counter for keys found +1
            return &findKey->second->value; // return key's value
        }

        // key isn't present; return nullptr
        else {
            stats_.misses++;                // counter for keys not found +1
            return nullptr;
        }
    }

    /**
     * Function: put()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: puts a new node into the map or updates an existing node
     */
    void put(const Key& key, Value value) {
        // zero capacity, so nothing can be put
        if (capacity_ == 0) { return; }

        // key is present in the cache
        if (auto findKey = map_.find(key); findKey != map_.end()) {
            findKey->second->value = value; // change the nodes value
            moveToFront(findKey->second);   // now most recently used, so move to the front
            return;
        }

        // cache is full
        if (map_.size() == capacity_) {
            Node* victim = tail_;           // least recently used
            unlink(victim);                 // remove from the list
            map_.erase(victim->key);        // remove key from the map
            delete victim;                  // free the victim
            stats_.evictions++;             // counter for keys removed +1
        }

        Node* newNode = new Node{key, std::move(value), nullptr, nullptr};  // new node
        pushFront(newNode);                                                 // move new node to the front
        map_[key] = newNode;                                                // save in to the map
    }

    /**
     * Function: contains()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: search if a key is in the cache
     */
    bool contains(const Key& key) const {
        // key was found/cached
        if(auto findKey = map_.find(key); findKey != map_.end()) {
            return true;
        }

        return false;   // key was not found
    }

    /**
     * Function: erase()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: removes key if found in the cache
     */
    bool erase(const Key& key) {
        // key was found/cached
        if (auto findKey = map_.find(key); findKey != map_.end()) {
            Node* oldNode = findKey->second;    // node to be removed
            unlink(oldNode);                    // remove the key's node
            map_.erase(findKey);                // remove key from the map
            delete oldNode;                     // free node
            return true;
        }

        return false;   // key was not found
    }

    /**
     * Function: clear()
     * 
     * Author: Kevin Aroche Rios
     * Date: November 27, 2026
     * Description: clears cache
     */
    void clear() {
        Node* nextNode = nullptr;   // holds next node

        // iterate through the list of nodes and delete each one
        for (Node* node = head_; node; node = nextNode) {
            nextNode = node->next;  // next node reassigned
            delete node;            // delete current node
        }

        map_.clear();               // clear map
        head_ = nullptr;            // set head and tail as null
        tail_ = nullptr;
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
