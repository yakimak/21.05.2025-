#ifndef STACK_H
#define STACK_H

#include "1_ArraySequence.h"

template <class T>
class Stack {
private:
   MutableArraySequence<T>* buffer;

public:
    
    Stack();
    Stack(T* items, int count);
    Stack(const Stack<T>& other);
    Stack(const MutableArraySequence<T>& seq);
    ~Stack();

   
    int GetLength() const;
    bool IsEmpty() const;
    void Push(T item);
    T Pop();
    T Peek() const;

    
    Stack<T>* Map(T (*f)(T)) const;
    Stack<T>* Where(bool (*f)(T)) const;
    T Reduce(T (*f)(T, T), T initial) const;

    
    Stack<T>* Concat(const Stack<T>& other) const;
    Stack<T>* GetSubsequence(int startIndex, int endIndex) const;
    bool ContainsSubsequence(const T* items, int count) const;

   
    void Print() const;

private:
    void ValidateIndex(int index) const;
};



template <class T>
Stack<T>::Stack() : buffer(new MutableArraySequence<T>()) {}

template <class T>
Stack<T>::Stack(T* items, int count) : buffer(new MutableArraySequence<T>(items, count)) {}

template <class T>
Stack<T>::Stack(const Stack<T>& other) : buffer(new MutableArraySequence<T>(*other.buffer)) {}

template <class T>
Stack<T>::Stack(const MutableArraySequence<T>& seq) : buffer(new MutableArraySequence<T>(seq)) {}

template <class T>
Stack<T>::~Stack() {
    delete buffer;
}

template <class T>
int Stack<T>::GetLength() const {
    return buffer->GetLength();
}

template <class T>
bool Stack<T>::IsEmpty() const {
    return buffer->GetLength() == 0;
}

template <class T>
void Stack<T>::Push(T item) {
    buffer->Append(item);
}

template <class T>
T Stack<T>::Pop() {
    if (IsEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    T item = buffer->GetLast();
    
    auto newBuffer = buffer->GetSubsequence(0, buffer->GetLength() - 2);
    delete buffer;
    buffer = dynamic_cast<MutableArraySequence<T>*>(newBuffer);
    return item;
}

template <class T>
T Stack<T>::Peek() const {
    if (IsEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    return buffer->GetLast();
}

template <class T>
Stack<T>* Stack<T>::Map(T (*f)(T)) const {
    MutableArraySequence<T>* newBuffer = new MutableArraySequence<T>();
    for (int i = 0; i < buffer->GetLength(); ++i) {
        newBuffer->Append(f(buffer->Get(i)));
    }
    return new Stack<T>(*newBuffer);  
}

template <class T>
Stack<T>* Stack<T>::Where(bool (*f)(T)) const {
    MutableArraySequence<T>* newBuffer = new MutableArraySequence<T>();
    for (int i = 0; i < buffer->GetLength(); ++i) {
        T item = buffer->Get(i);
        if (f(item)) {
            newBuffer->Append(item);
        }
    }
    return new Stack<T>(*newBuffer);  
}

template <class T>
T Stack<T>::Reduce(T (*f)(T, T), T initial) const {
    if (IsEmpty()) {
        return initial;
    }
    
    T result = f(buffer->Get(0), initial);
    for (int i = 1; i < buffer->GetLength(); ++i) {
        result = f(buffer->Get(i), result);
    }
    return result;
}

template <class T>
Stack<T>* Stack<T>::Concat(const Stack<T>& other) const {
   MutableArraySequence<T>* newBuffer = new MutableArraySequence<T>(*buffer);
    for (int i = 0; i < other.buffer->GetLength(); ++i) {
        newBuffer->Append(other.buffer->Get(i));
    }
    return new Stack<T>(*newBuffer);
}

template <class T>
Stack<T>* Stack<T>::GetSubsequence(int startIndex, int endIndex) const {
    ValidateIndex(startIndex);
    ValidateIndex(endIndex - 1);
    if (startIndex > endIndex) {
        throw std::invalid_argument("Start index > end index");
    }
    
    MutableArraySequence<T>* subBuffer = dynamic_cast<MutableArraySequence<T>*>(buffer->GetSubsequence(startIndex, endIndex));
    Stack<T>* result = new Stack<T>(*subBuffer);
    delete subBuffer;
    return result;
}

template <class T>
bool Stack<T>::ContainsSubsequence(const T* items, int count) const {
    if (count == 0) return true;
    if (count > buffer->GetLength()) return false;
    
    for (int i = 0; i <= buffer->GetLength() - count; ++i) {
        bool match = true;
        for (int j = 0; j < count; ++j) {
            if (buffer->Get(i + j) != items[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

template <class T>
void Stack<T>::Print() const {
    std::cout << "[ ";
    for (int i = 0; i < buffer->GetLength(); ++i) {
        std::cout << buffer->Get(i) << " ";
    }
    std::cout << "]" << std::endl;
}

template <class T>
void Stack<T>::ValidateIndex(int index) const {
    if (index < 0 || index >= buffer->GetLength()) {
        throw std::out_of_range("Index out of range");
    }
}

#endif 