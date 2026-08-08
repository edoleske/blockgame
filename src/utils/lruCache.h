#ifndef BLOCKGAME_LRUCACHE_H
#define BLOCKGAME_LRUCACHE_H

#include "common.h"


template <typename K, typename V>
class LRUCache {
    struct Node {
        K key;
        shared_ptr<V> value;
        Node(K k, shared_ptr<V> v) : key(std::move(k)), value(std::move(v)) {}
    };

public:
    explicit LRUCache(const size_t capacity) : capacity(capacity) {}

    V* get(const K& key) {
        auto it = m.find(key);
        if (it == m.end()) {
            return nullptr;
        }

        // Move to front of list
        l.splice(l.begin(), l, it->second);
        return (*it->second).value.get();
    }

    void add(const K& key, shared_ptr<V> value) {
        if (auto it = m.find(key); it != m.end()) {
            *it->second = Node(key, value);
            l.splice(l.begin(), l, it->second);
            return;
        }

        l.emplace_front(key, value);
        m[key] = l.begin();

        if (l.size() > capacity) {
            evict();
        }
    }

private:
    list<Node> l;
    unordered_map<K, typename list<Node>::iterator> m;
    size_t capacity;

    void evict() {
        m.erase(l.back().key);
        l.pop_back();
    }
};

#endif //BLOCKGAME_LRUCACHE_H
