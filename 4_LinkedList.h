#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>
#include <sstream>
#include <string>

template <class T> class LinkedList
    {
        public:
            LinkedList();
            LinkedList(T* items, int count);
            LinkedList(const LinkedList <T> & list);
            
            ~LinkedList();

            T GetFirst() const;
            T GetLast() const;
            T Get(int index) const;
            LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
            int GetLength() const;

            void Append(T item);
            void Prepend(T item);
            void InsertAt(T item, int index);
            LinkedList<T>* Concat(LinkedList<T>* list) const;
            LinkedList& operator= (const LinkedList<T>& list);
            std::string ToString() const;
            
        private:
            struct Node
                {
                    T data;
                    Node* next;
                    Node(T data) : data(data), next(nullptr) {}
                };
        
            Node* head;
            Node* tail;
            int size;
    };

////////////////////////////////////////////////////////////////////////////

template <class T> LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <class T> LinkedList<T>::LinkedList(T* items, int count) : LinkedList() {
    for (int i = 0; i < count; ++i) {
        Append(items[i]);
    }
}

template <class T> LinkedList<T>::LinkedList(const LinkedList<T>& list) : LinkedList() {
    Node* current = list.head;
    while (current != nullptr) {
        Append(current->data);
        current = current->next;
    }
}

template <class T> LinkedList<T>::~LinkedList() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

////////////////////////////////////////////////////////////////////////////

template <class T> T LinkedList<T>::GetFirst() const{
    if (size == 0) {
        throw std::out_of_range("List is empty");
    }
    return head->data;
}

template <class T> T LinkedList<T>::GetLast() const{
    if (size == 0) {
        throw std::out_of_range("List is empty");
    }
    return tail->data;
}

template <class T> T LinkedList<T>::Get(int index) const{
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }

    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <class T> LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const{
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
        throw std::out_of_range("Invalid index range");
    }

    LinkedList<T>* subList = new LinkedList<T>();
    Node* current = head;
    for (int i = 0; i < startIndex; ++i) {
        current = current->next;
    }
    for (int i = startIndex; i <= endIndex; ++i) {
        subList->Append(current->data);
        current = current->next;
    }
    return subList;
}

template <class T> int LinkedList<T>::GetLength() const{
    return size;
}

////////////////////////////////////////////////////////////////////////////

template <class T> void LinkedList<T>::Append(T item) {
    Node* newNode = new Node(item);
    if (tail == nullptr) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template <class T> void LinkedList<T>::Prepend(T item) {
    Node* newNode = new Node(item);
    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

template <class T> void LinkedList<T>::InsertAt(T item, int index) {
    if (index < 0 || index > size) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        Prepend(item);
    }
    else if (index == size) {
        Append(item);
    }
    else {
        Node* newNode = new Node(item);
        Node* prev = head;
        for (int i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        newNode->next = prev->next;
        prev->next = newNode;
        size++;
    }
}

template <class T> LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list) const{
    if (list == nullptr) {
        throw std::invalid_argument("List cannot be null");
    }

    LinkedList<T>* newList = new LinkedList<T>(*this);
    Node* current = list->head;
    while (current != nullptr) {
        newList->Append(current->data);
        current = current->next;
    }
    return newList;
}

template <class T> LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list) {
    if (this != &list) {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;

        Node* current = list.head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <class T> std::string LinkedList<T>::ToString() const {
    std::ostringstream oss;
    oss << "[";
    Node* current = head;
    while (current != nullptr) {
        oss << current->data;
        if (current->next != nullptr) oss << ", ";
        current = current->next;
    }
    oss << "]";
    return oss.str();
}

#endif 