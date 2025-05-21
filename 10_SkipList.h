#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
#include <stdexcept>

template <typename T>
class SkipList {
private:
    struct Node {
        T key;
        Node* next;
        Node* down;
        
        Node(const T& k, Node* n = nullptr, Node* d = nullptr) 
            : key(k), next(n), down(d) {}
    };

    Node* head;
    int maxLevel;
    float probability;
    size_t size;

public:
    
    class Iterator {
        private:
    Node* current;
    
    Node* getBottom(Node* node) const {
        while (node && node->down) {
            node = node->down;
        }
        return node;
    }
    
public:
    explicit Iterator(Node* node = nullptr) : current(getBottom(node)) {
    
        if (current && current->key == std::numeric_limits<T>::min()) {
            current = current->next;
        }
    }
    
    Iterator& operator++() {
        if (current) {
            current = current->next;
    
            if (current && current->key == std::numeric_limits<T>::max()) {
                current = nullptr;
            }
        }
        return *this;
    }
    
    T operator*() const {
        if (!current) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return current->key;
    }
    
    bool operator!=(const Iterator& other) const {
        return current != other.current;
    }
};

    explicit SkipList(int maxLvl = 16, float p = 0.5);
    ~SkipList();

    bool contains(const T& key) const;
    void insert(const T& key);
    void remove(const T& key);
    void print() const;
    bool empty() const { return size == 0; }
    size_t getSize() const { return size; }

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }

private:
    int randomLevel() const;
    void clear();
    Node* findPredecessors(const T& key, std::vector<Node*>& predecessors) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
SkipList<T>::SkipList(int maxLvl, float p) 
    : maxLevel(maxLvl > 0 ? maxLvl : 16), 
      probability(p > 0 && p < 1 ? p : 0.5),
      head(nullptr),
      size(0) {
    
    if (maxLvl <= 0) throw std::invalid_argument("Max level must be positive");
    if (p <= 0 || p >= 1) throw std::invalid_argument("Probability must be between 0 and 1");

    Node* prevHead = nullptr;
    for (int i = 0; i < maxLevel; ++i) {
        Node* newTail = new Node(std::numeric_limits<T>::max());
        head = new Node(std::numeric_limits<T>::min(), newTail, prevHead);
        prevHead = head;
    }
    srand(static_cast<unsigned>(time(nullptr)));
}

template <typename T>
SkipList<T>::~SkipList() {
    clear();
}

template <typename T>
typename SkipList<T>::Node* SkipList<T>::findPredecessors(const T& key, std::vector<Node*>& predecessors) const {
    Node* current = head;
    Node* found = nullptr;

    for (int i = maxLevel - 1; i >= 0; --i) {
        while (current->next && current->next->key < key) {
            current = current->next;
        }
        
        if (current->next && current->next->key == key) {
            found = current->next;
        }
        
        predecessors[i] = current;
        current = current->down;
    }
    
    return found;
}

template <typename T>
bool SkipList<T>::contains(const T& key) const {
    std::vector<Node*> predecessors(maxLevel);
    return findPredecessors(key, predecessors) != nullptr;
}

template <typename T>
void SkipList<T>::insert(const T& key) {
    std::vector<Node*> predecessors(maxLevel);
    Node* existing = findPredecessors(key, predecessors);
    
    if (existing) return; // Дубликаты не допускаются

    int levels = randomLevel();
    Node* downNode = nullptr;
    
    for (int i = 0; i < levels; ++i) {
        Node* newNode = new Node(key, predecessors[i]->next, downNode);
        predecessors[i]->next = newNode;
        downNode = newNode;
    }
    
    size++;
}

template <typename T>
void SkipList<T>::remove(const T& key) {
    std::vector<Node*> predecessors(maxLevel);
    Node* toDelete = findPredecessors(key, predecessors);
    
    if (!toDelete) {
        throw std::runtime_error("Key not found");
    }

    // Удаляем все узлы на всех уровнях
    Node* current = toDelete;
    int currentLevel = 0;
    while (current && currentLevel < maxLevel) {
        Node* nextDown = current->down;
        
        // Находим предыдущий узел на текущем уровне
        Node* prev = predecessors[currentLevel];
        while (prev->next != current) {
            prev = prev->next;
        }
        
        prev->next = current->next;
        delete current;
        current = nextDown;
        currentLevel++;
    }
    
    size--;

    // Удаляем пустые верхние уровни
    while (maxLevel > 1 && head->next->key == std::numeric_limits<T>::max()) {
        Node* oldHead = head;
        head = head->down;
        
        // Удаляем хвостовой узел уровня
        Node* tail = oldHead->next;
        delete tail;
        delete oldHead;
        
        maxLevel--;
    }
}


template <typename T>
void SkipList<T>::print() const {
    Node* levelHead = head;
    for (int i = maxLevel - 1; i >= 0; --i) {
        std::cout << "Level " << i << ": ";
        Node* current = levelHead;
        while (current) {
            if (current->key != std::numeric_limits<T>::min() && 
                current->key != std::numeric_limits<T>::max()) {
                std::cout << current->key << " ";
            }
            current = current->next;
        }
        std::cout << std::endl;
        levelHead = levelHead->down;
    }
}

template <typename T>
int SkipList<T>::randomLevel() const {
    int level = 1;
    while (static_cast<float>(rand()) / RAND_MAX < probability && level < maxLevel) {
        level++;
    }
    return level;
}

template <typename T>
void SkipList<T>::clear() {
    Node* currentLevel = head;
    while (currentLevel) {
        Node* currentNode = currentLevel;
        currentLevel = currentLevel->down;
        
        while (currentNode) {
            Node* temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
    
    head = nullptr;
    size = 0;
    maxLevel = 0;
}

#endif