#ifndef LINEARFORM_H
#define LINEARFORM_H

#include <stdexcept>
#include <sstream>
#include <vector>
#include "1_ArraySequence.h"

template <class T>
class LinearForm {
public:
    // Конструкторы
    LinearForm(T* items, int count) {
        coefficients = new MutableArraySequence<T>(items, count);
    }
    
    LinearForm() {
        coefficients = new MutableArraySequence<T>();
    }
    
    LinearForm(const LinearForm<T>& other) {
        coefficients = other.coefficients->Clone();
    }
    
    ~LinearForm() {
        delete coefficients;
    }

    // Основные методы доступа
    T GetFirst() const { 
        if (GetLength() == 0) throw std::out_of_range("Form is empty");
        return coefficients->GetFirst(); 
    }
    
    T GetLast() const { 
        if (GetLength() == 0) throw std::out_of_range("Form is empty");
        return coefficients->GetLast(); 
    }
    
    T Get(int index) const { 
        return coefficients->Get(index); 
    }
    
    int GetLength() const { 
        return coefficients->GetLength(); 
    }
    
    std::string ToString() const;

    // Математические операции
    LinearForm<T>* Add(const LinearForm<T>& other) const;
    LinearForm<T>* Subtract(const LinearForm<T>& other) const;
    LinearForm<T>* Multiply(const T& scalar) const;
    LinearForm<T>* Divide(const T& scalar) const;

    // Вычисление значений
    T Evaluate(const ArraySequence<T>& variables) const;
    T Evaluate(T x) const;
    ArraySequence<T>* Gradient() const;
    T PartialDerivative(int variableIndex) const;

    // Операторы
    LinearForm<T>* operator+(const LinearForm<T>& other) const { return Add(other); }
    LinearForm<T>* operator-(const LinearForm<T>& other) const { return Subtract(other); }
    LinearForm<T>* operator*(const T& scalar) const { return Multiply(scalar); }
    LinearForm<T>* operator/(const T& scalar) const { return Divide(scalar); }

private:
    ArraySequence<T>* coefficients;

    void CheckDimensions(const LinearForm<T>& other) const {
        if (GetLength() != other.GetLength()) {
            throw std::invalid_argument("Linear forms have different dimensions");
        }
    }
};

// Реализации методов
template <class T> 
std::string LinearForm<T>::ToString() const {
    if (GetLength() == 0) return "F(x) = 0";
    
    std::ostringstream oss;
    oss << "F(x) = " << Get(0);
    for (int i = 1; i < GetLength(); ++i) {
        oss << " + " << Get(i) << "*x" << i;
    }
    return oss.str();
}

template <class T> 
LinearForm<T>* LinearForm<T>::Add(const LinearForm<T>& other) const {
    CheckDimensions(other);
    if (GetLength() == 0) return new LinearForm<T>();
    
    try {
        T* new_coeffs = new T[GetLength()];
        for (int i = 0; i < GetLength(); ++i) {
            new_coeffs[i] = Get(i) + other.Get(i);
        }
        LinearForm<T>* result = new LinearForm<T>(new_coeffs, GetLength());
        delete[] new_coeffs;
        return result;
    } catch (...) {
        throw std::runtime_error("Memory allocation failed in Add operation");
    }
}

template <class T> 
LinearForm<T>* LinearForm<T>::Subtract(const LinearForm<T>& other) const {
    CheckDimensions(other);
    T* new_coeffs = new T[GetLength()];
    for (int i = 0; i < GetLength(); ++i) {
        new_coeffs[i] = Get(i) - other.Get(i);
    }
    LinearForm<T>* result = new LinearForm<T>(new_coeffs, GetLength());
    delete[] new_coeffs;
    return result;
}

template <class T> 
LinearForm<T>* LinearForm<T>::Multiply(const T& scalar) const {
    T* new_coeffs = new T[GetLength()];
    for (int i = 0; i < GetLength(); ++i) {
        new_coeffs[i] = Get(i) * scalar;
    }
    LinearForm<T>* result = new LinearForm<T>(new_coeffs, GetLength());
    delete[] new_coeffs;
    return result;
}

template <class T> 
LinearForm<T>* LinearForm<T>::Divide(const T& scalar) const {
    if (scalar == T(0)) {
        throw std::invalid_argument("Division by zero");
    }
    return Multiply(T(1) / scalar);
}

template <class T> 
T LinearForm<T>::Evaluate(const ArraySequence<T>& variables) const {
    if (variables.GetLength() != GetLength() - 1) {
        throw std::invalid_argument("Number of variables doesn't match form dimension");
    }

    T result = Get(0); 
    for (int i = 1; i < GetLength(); ++i) {
        result += Get(i) * variables.Get(i - 1);
    }
    return result;
}

template <class T> 
T LinearForm<T>::Evaluate(T x) const {
    T result = Get(0);
    T x_power = x;
    for (int i = 1; i < GetLength(); ++i) {
        result += Get(i) * x_power;
        x_power *= x;
    }
    return result;
}

template <class T> 
ArraySequence<T>* LinearForm<T>::Gradient() const {              
    if (GetLength() <= 1) {
        return new MutableArraySequence<T>(); 
    }
    
    T* grad_coeffs = new T[GetLength() - 1];
    for (int i = 1; i < GetLength(); ++i) {
        grad_coeffs[i - 1] = Get(i);
    }
    
    ArraySequence<T>* result = new MutableArraySequence<T>(grad_coeffs, GetLength() - 1);
    delete[] grad_coeffs;
    return result;
}

template <class T> 
T LinearForm<T>::PartialDerivative(int variableIndex) const {
    if (variableIndex < 0 || variableIndex >= GetLength() - 1) {
        throw std::out_of_range("Variable index out of range");
    }
    return Get(variableIndex + 1);
}

#endif