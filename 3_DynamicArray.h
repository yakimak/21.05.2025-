#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>
#include <sstream>
#include <string>

template <class T> class DynamicArray
    {
        public:
            DynamicArray(T* items, int count);
            DynamicArray(int size);
            DynamicArray(const DynamicArray<T>& other);

            ~DynamicArray();

            T Get(int index) const;
            int GetSize() const;
            void Set(int index, T value);
            void Resize(int newSize);

            DynamicArray& operator= (const DynamicArray<T>& other);
            std::string ToString() const;

        private:
            T* buffer;
            int size;    
    };

//////////////////////////////////////////////////////////////////////

template <class T> DynamicArray<T>::DynamicArray(T* items, int count) : size(count) {
    buffer = new T[size];
    for (int i = 0; i < size; ++i) {
        buffer[i] = items[i];
    }
}

template <class T> DynamicArray<T>::DynamicArray(int size) : size(size) {
    buffer = new T[size];
}

template <class T> DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) : size(other.size) {
    buffer = new T[size];
    for (int i = 0; i < size; ++i) {
        buffer[i] = other.buffer[i];
    }
}

template <class T> DynamicArray<T>::~DynamicArray() {
    delete[] buffer;
}

//////////////////////////////////////////////////////////////////////

template <class T> T DynamicArray<T>::Get(int index) const{
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return buffer[index];
}

template <class T> int DynamicArray<T>::GetSize() const{
    return size;
}

template <class T> void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    buffer[index] = value;
}

//////////////////////////////////////////////////////////////////////

template <class T> void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0) {
        throw std::invalid_argument("Invalid size");
    }

    if (newSize == size) {
        return;
    }

    T* newBuffer = new T[newSize];
    int elementsToCopy = (size < newSize) ? size : newSize;

    for (int i = 0; i < elementsToCopy; ++i) {
        newBuffer[i] = buffer[i];
    }

    delete[] buffer;
    buffer = newBuffer;
    size = newSize;
}

template <class T> DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
    if (this != &other) {
        delete[] buffer;
        size = other.size;
        buffer = new T[size];
        for (int i = 0; i < size; ++i) {
            buffer[i] = other.buffer[i];
        }
    }
    return *this;
}

template <class T> std::string DynamicArray<T>::ToString() const {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < size; ++i) {
        oss << buffer[i];
        if (i < size - 1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}

#endif