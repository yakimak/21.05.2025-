#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H

#include "0_Sequence.h"
#include "4_LinkedList.h"

template <class T> class ListSequence;
template <class T> class MutableListSequence;
template <class T> class ImmutableListSequence;

template <class T> class ListSequence : public Sequence<T> 
    {
        public:
            ListSequence(T* items, int count);
            ListSequence();
            ListSequence(const LinkedList<T>& linkedList);
            
            virtual ~ListSequence();

            T GetFirst() const override;
            T GetLast() const override;
            T Get(int index) const override;
            int GetLength() const override;
            ListSequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
            
            virtual ListSequence<T>* Instance() = 0;
            virtual ListSequence<T>* Clone() const = 0;
            
            ListSequence<T>* Append(T item) override;
            ListSequence<T>* Prepend(T item) override;
            ListSequence<T>* InsertAt(T item, int index) override;
            ListSequence<T>* Concat(Sequence<T>* other) override;
            std::string ToString() const;
            
        protected:
            LinkedList<T>* list;
            virtual ListSequence<T>* AppendInternal(T item);
            virtual ListSequence<T>* PrependInternal(T item);
            virtual ListSequence<T>* InsertAtInternal(T item, int index);
            virtual ListSequence<T>* ConcatInternal(Sequence<T>* other);
};

////////////////////////////////////////////////////////////////////////////////////

template <class T> class MutableListSequence : public ListSequence<T> 
    {
        public:
            using ListSequence<T>::ListSequence;
            ListSequence<T>* Instance() override;
            ListSequence<T>* Clone() const override;
    };

template <class T> class ImmutableListSequence : public ListSequence<T> 
    {
        public:
            using ListSequence<T>::ListSequence;
            ListSequence<T>* Instance() override;
            ListSequence<T>* Clone() const override;
    };

//////////////////////////////////////////////////////////////////////////////////////

template <class T>
ListSequence<T>::ListSequence(T* items, int count) : list(new LinkedList<T>(items, count)) {}

template <class T>
ListSequence<T>::ListSequence() : list(new LinkedList<T>()) {}

template <class T>
ListSequence<T>::ListSequence(const LinkedList<T>& linkedList) : list(new LinkedList<T>(linkedList)) {}

template <class T>
ListSequence<T>::~ListSequence() { delete list; }

////////////////////////////////////////////////////////////////////////////////////

template <class T> T ListSequence<T>::GetFirst() const {
    return list->GetFirst();
}

template <class T> T ListSequence<T>::GetLast() const {
    return list->GetLast();
}

template <class T> T ListSequence<T>::Get(int index) const {
    return list->Get(index);
}

template <class T> int ListSequence<T>::GetLength() const {
    return list->GetLength();
}

template <class T> ListSequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);
    ListSequence<T>* result = new MutableListSequence<T>(*subList);
    delete subList;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////

template <class T> ListSequence<T>* ListSequence<T>::AppendInternal(T item) {
    list->Append(item);
    return this;
}

template <class T> ListSequence<T>* ListSequence<T>::PrependInternal(T item) {
    list->Prepend(item);
    return this;
}

template <class T> ListSequence<T>* ListSequence<T>::InsertAtInternal(T item, int index) {
    list->InsertAt(item, index);
    return this;
}

template <class T> ListSequence<T>* ListSequence<T>::ConcatInternal(Sequence<T>* other) {
    if (!other) throw std::invalid_argument("Other sequence cannot be null");
    
    ListSequence<T>* result = static_cast<ListSequence<T>*>(this->Clone());
    
    for (int i = 0; i < other->GetLength(); i++) {
        result->Append(other->Get(i));
    }
    
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////

template <class T> ListSequence<T>* ListSequence<T>::Append(T item) {
    return Instance()->AppendInternal(item);
}

template <class T> ListSequence<T>* ListSequence<T>::Prepend(T item) {
    return Instance()->PrependInternal(item);
}

template <class T> ListSequence<T>* ListSequence<T>::InsertAt(T item, int index) {
    return Instance()->InsertAtInternal(item, index);
}

template <class T> ListSequence<T>* ListSequence<T>::Concat(Sequence<T>* other) {
    return Instance()->ConcatInternal(other);
}

////////////////////////////////////////////////////////////////////////////////////////

template <class T> ListSequence<T>* MutableListSequence<T>::Instance() {
    return this;
}

template <class T> ListSequence<T>* MutableListSequence<T>::Clone() const {
    return new MutableListSequence<T>(*this->list);
}

template <class T> ListSequence<T>* ImmutableListSequence<T>::Instance() {
    return this->Clone();
}

template <class T> ListSequence<T>* ImmutableListSequence<T>::Clone() const {
    return new ImmutableListSequence<T>(*this->list);
}

template <class T> std::string ListSequence<T>::ToString() const {
    return list->ToString();
}

#endif