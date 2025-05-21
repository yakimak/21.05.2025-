#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdexcept>

template <class T> class Sequence
    {
        public:
            virtual ~Sequence() = default;

            virtual T GetFirst() const = 0;
            virtual T GetLast() const = 0;
            virtual T Get(int index) const = 0;
            virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
            virtual int GetLength() const = 0;

            virtual Sequence<T>* Append(T item) = 0;
            virtual Sequence<T>* Prepend(T item) = 0;
            virtual Sequence<T>* InsertAt(T item, int index) = 0;
            virtual Sequence<T>* Concat(Sequence<T>* list) = 0;
            virtual std::string ToString() const = 0;
    };

#endif 