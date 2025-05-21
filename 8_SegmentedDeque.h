#ifndef SEGMENTED_DEQUE_H
#define SEGMENTED_DEQUE_H

#include <vector>
#include <stdexcept>
#include <functional>
#include <memory>

template <typename T>
class SegmentedDeque {
private:
    static constexpr int SEGMENT_SIZE = 16;
    std::vector<std::unique_ptr<std::vector<T>>> segments;
    int front_segment = 0;
    int front_offset = 0;
    int back_segment = 0;
    int back_offset = 0;
    int total_size = 0;

    void allocate_segment() {
        segments.emplace_back(std::make_unique<std::vector<T>>(SEGMENT_SIZE));
    }

    void deallocate_unused_segments() {
        while (front_segment > 0 && !segments.empty()) {
            segments.erase(segments.begin());
            front_segment--;
            back_segment--;
        }
    }

public:
    SegmentedDeque() {
        allocate_segment();
    }

    SegmentedDeque(SegmentedDeque&& other) noexcept
        : segments(std::move(other.segments)),
          front_segment(other.front_segment),
          front_offset(other.front_offset),
          back_segment(other.back_segment),
          back_offset(other.back_offset),
          total_size(other.total_size) {
        other.front_segment = 0;
        other.front_offset = 0;
        other.back_segment = 0;
        other.back_offset = 0;
        other.total_size = 0;
    }

    SegmentedDeque& operator=(SegmentedDeque&& other) noexcept {
        if (this != &other) {
            segments = std::move(other.segments);
            front_segment = other.front_segment;
            front_offset = other.front_offset;
            back_segment = other.back_segment;
            back_offset = other.back_offset;
            total_size = other.total_size;

            other.front_segment = 0;
            other.front_offset = 0;
            other.back_segment = 0;
            other.back_offset = 0;
            other.total_size = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (back_offset == SEGMENT_SIZE) {
            allocate_segment();
            back_segment++;
            back_offset = 0;
        }
        (*segments[back_segment])[back_offset++] = value;
        total_size++;
    }

    void push_front(const T& value) {
        if (front_offset == 0) {
            if (front_segment == 0) {
                segments.insert(segments.begin(), std::make_unique<std::vector<T>>(SEGMENT_SIZE)); //сегмент -1
                back_segment++;
            } else {
                front_segment--;
            }
            front_offset = SEGMENT_SIZE;
        }
        (*segments[front_segment])[--front_offset] = value;
        total_size++;
    }

    T pop_back() {
        if (empty()) throw std::out_of_range("Deque is empty");
        if (back_offset == 0) {
            segments.pop_back();
            back_segment--;
            back_offset = SEGMENT_SIZE;
        }
        total_size--;
        T value = (*segments[back_segment])[--back_offset];
        deallocate_unused_segments();
        return value;
    }

    T pop_front() {
        if (empty()) throw std::out_of_range("Deque is empty");
        T value = (*segments[front_segment])[front_offset++];
        total_size--;
        if (front_offset == SEGMENT_SIZE && front_segment != back_segment) {
            front_segment++;
            front_offset = 0;
            deallocate_unused_segments();
        }
        return value;
    }

    T& operator[](int index) {
        if (index < 0 || index >= total_size) {
            throw std::out_of_range("Index out of range");
        }
        index += front_offset;
        int segment = front_segment + index / SEGMENT_SIZE;
        int offset = index % SEGMENT_SIZE;
        return (*segments[segment])[offset];
    }

    int size() const { return total_size; }
    bool empty() const { return total_size == 0; }

    template <typename U>
    SegmentedDeque<U> map(std::function<U(const T&)> func) const {
        SegmentedDeque<U> result;
        for (int i = 0; i < size(); ++i) {
            result.push_back(func((*this)[i]));
        }
        return result;
    }

    SegmentedDeque<T> where(std::function<bool(const T&)> predicate) const {
        SegmentedDeque<T> result;
        for (int i = 0; i < size(); ++i) {
            if (predicate((*this)[i])) {
                result.push_back((*this)[i]);
            }
        }
        return result;
    }

    template <typename U>
    U reduce(std::function<U(U, const T&)> func, U initial) const {
        U result = initial;
        for (int i = 0; i < size(); ++i) {
            result = func(result, (*this)[i]);
        }
        return result;
    }

    SegmentedDeque<T> concat(const SegmentedDeque<T>& other) const {
        SegmentedDeque<T> result;
        
        for (int i = 0; i < size(); ++i) {
            result.push_back((*this)[i]);
        }
    
        for (int i = 0; i < other.size(); ++i) {
            result.push_back(other[i]);
        }
        return result;
    }

    SegmentedDeque<T> subseq(int start, int end) const {
        if (start < 0 || end >= size() || start > end) {
            throw std::out_of_range("Invalid subsequence range");
        }
        SegmentedDeque<T> result;
        for (int i = start; i <= end; ++i) {
            result.push_back((*this)[i]);
        }
        return result;
    }

    void sort(std::function<bool(const T&, const T&)> comparator = nullptr) {
        if (!comparator) {
            comparator = [](const T& a, const T& b) { return a < b; };
        }
        
        for (int i = 0; i < size() - 1; ++i) {
            for (int j = 0; j < size() - i - 1; ++j) {
                if (!comparator((*this)[j], (*this)[j+1])) {
                    std::swap((*this)[j], (*this)[j+1]);
                }
            }
        }
    }
};

template <typename T>
constexpr int SegmentedDeque<T>::SEGMENT_SIZE;

#endif 