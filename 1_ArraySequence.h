#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "0_Sequence.h"
#include "3_DynamicArray.h"

template <class T> class ArraySequence;
template <class T> class MutableArraySequence;
template <class T> class ImmutableArraySequence;


template <class T> class ArraySequence: public Sequence<T>
    {
        public:
            ArraySequence(T* items, int count);
            ArraySequence();
            ArraySequence(const DynamicArray<T>& arr);
            ArraySequence(const ArraySequence& other);

            virtual ~ArraySequence();

            T GetFirst() const override;
            T GetLast() const override;
            T Get(int index) const override;
            int GetLength() const override;
            ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override;

            virtual ArraySequence<T>* Instance() = 0;
            virtual ArraySequence<T>* Clone() const = 0;

            ArraySequence<T>* Append(T item) override;
            ArraySequence<T>* Prepend(T item) override;
            ArraySequence<T>* InsertAt(T item, int index) override;
            ArraySequence<T>* Concat(Sequence<T>* other) override;
            std::string ToString() const;
            
        protected:
            DynamicArray<T>* items;

            void AppendImpl(T item);
            void PrependImpl(T item);
            void InsertAtImpl(T item, int index);
    };

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> class MutableArraySequence : public ArraySequence<T>
    {
        public:
            MutableArraySequence();
            MutableArraySequence(T* items, int count);
            MutableArraySequence(const DynamicArray<T>& arr);
            MutableArraySequence(const MutableArraySequence& other);
            ArraySequence<T>* Instance() override;
            ArraySequence<T>* Clone() const override;
    };

template <class T> class ImmutableArraySequence : public ArraySequence<T> 
    {
        public:
            ImmutableArraySequence();
            ImmutableArraySequence(T* items, int count);
            ImmutableArraySequence(const DynamicArray<T>& arr);
            ArraySequence<T>* Instance() override;
            ArraySequence<T>* Clone() const override;
    };

///////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
ArraySequence<T>::ArraySequence(T* items, int count) : items(new DynamicArray<T>(items, count)) {}

template <class T>
ArraySequence<T>::ArraySequence() : items(new DynamicArray<T>(0)) {}

template <class T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& arr) : items(new DynamicArray<T>(arr)) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence& other) : items(new DynamicArray<T>(*other.items)) {}

template <class T>
ArraySequence<T>::~ArraySequence() { delete items; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> T ArraySequence<T>::GetFirst() const {
    if (items->GetSize() == 0) throw std::out_of_range("Sequence is empty");
    return items->Get(0);
}

template <class T> T ArraySequence<T>::GetLast() const {
    if (items->GetSize() == 0) throw std::out_of_range("Sequence is empty");
    return items->Get(items->GetSize() - 1);
}

template <class T> T ArraySequence<T>::Get(int index) const {
    return items->Get(index);
}

template <class T> int ArraySequence<T>::GetLength() const {
    return items->GetSize();
}

template <class T> ArraySequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= items->GetSize() || startIndex > endIndex) {
        throw std::out_of_range("Invalid subsequence range");
    }

    int subSize = endIndex - startIndex + 1;
    T* subItems = new T[subSize];
    for (int i = 0; i < subSize; i++) {
        subItems[i] = items->Get(startIndex + i);
    }

    ArraySequence<T>* result = new MutableArraySequence<T>(subItems, subSize); // ! (можгно спрятать внутрь)
    delete[] subItems;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////

template <class T> void ArraySequence<T>::AppendImpl(T item) {
    items->Resize(items->GetSize() + 1);
    items->Set(items->GetSize() - 1, item);
}

template <class T> void ArraySequence<T>::PrependImpl(T item) {
    items->Resize(items->GetSize() + 1);
    for (int i = items->GetSize() - 1; i > 0; i--) {
        items->Set(i, items->Get(i - 1));
    }
    items->Set(0, item);
}

template <class T> void ArraySequence<T>::InsertAtImpl(T item, int index) {
    if (index < 0 || index > items->GetSize()) {
        throw std::out_of_range("Index out of range");
    }
    items->Resize(items->GetSize() + 1);
    for (int i = items->GetSize() - 1; i > index; i--) {
        items->Set(i, items->Get(i - 1));
    }
    items->Set(index, item);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template <class T> ArraySequence<T>* ArraySequence<T>::Append(T item)  {
    ArraySequence<T>* newseq = Instance();
    newseq->AppendImpl(item);
    return newseq;
}

template <class T> ArraySequence<T>* ArraySequence<T>::Prepend(T item) {
    ArraySequence<T>* newseq = Instance();
    newseq->PrependImpl(item);
    return newseq;
}

template <class T> ArraySequence<T>* ArraySequence<T>::InsertAt(T item, int index) {
    ArraySequence<T>* newseq = Instance();
    newseq->InsertAtImpl(item, index);
    return newseq;
}

template <class T> ArraySequence<T>* ArraySequence<T>::Concat(Sequence<T>* other) {
    if (!other) throw std::invalid_argument("Other sequence cannot be null");
    
    int totalSize = this->GetLength() + other->GetLength();
    T* newItems = new T[totalSize];
    
    for (int i = 0; i < this->GetLength(); i++) {
        newItems[i] = this->Get(i);
    }
    
    for (int i = 0; i < other->GetLength(); i++) {
        newItems[this->GetLength() + i] = other->Get(i);
    }
    
    ArraySequence<T>* result = new MutableArraySequence<T>(newItems, totalSize);
    delete[] newItems;
    
    return result;
}

template <class T> std::string ArraySequence<T>::ToString() const {
    return items->ToString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> MutableArraySequence<T>::MutableArraySequence() 
    : ArraySequence<T>() {}

template <class T> MutableArraySequence<T>::MutableArraySequence(T* items, int count) 
    : ArraySequence<T>(items, count) {}

template <class T> MutableArraySequence<T>::MutableArraySequence(const DynamicArray<T>& arr) 
    : ArraySequence<T>(arr) {}

template <class T> MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence& other) 
    : ArraySequence<T>(other) {}

template <class T> ArraySequence<T>* MutableArraySequence<T>::Instance() {
    return this;
}

template <class T> ArraySequence<T>* MutableArraySequence<T>::Clone() const {
    return new MutableArraySequence<T>(*this->items);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> ImmutableArraySequence<T>::ImmutableArraySequence() 
    : ArraySequence<T>() {}

template <class T> ImmutableArraySequence<T>::ImmutableArraySequence(T* items, int count) 
    : ArraySequence<T>(items, count) {}

template <class T> ImmutableArraySequence<T>::ImmutableArraySequence(const DynamicArray<T>& arr) 
    : ArraySequence<T>(arr) {}

template <class T> ArraySequence<T>* ImmutableArraySequence<T>::Instance() {
    return this->Clone();
}

template <class T> ArraySequence<T>* ImmutableArraySequence<T>::Clone() const {
    return new ImmutableArraySequence<T>(*this->items);
}

#endif