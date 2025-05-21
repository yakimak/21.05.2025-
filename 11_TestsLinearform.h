#include <cassert>
#include <iostream>
#include <fstream>
#include "5_LinearForm.h"

void TestConstructors() {
    std::cout << "Running TestConstructors... ";
    
    // Тест конструктора с массивом
    int arr1[] = {1, 2, 3};
    LinearForm<int> lf1(arr1, 3);
    assert(lf1.GetLength() == 3);
    assert(lf1.Get(0) == 1);
    assert(lf1.Get(1) == 2);
    assert(lf1.Get(2) == 3);

    // Тест конструктора по умолчанию
    LinearForm<int> lf2;
    assert(lf2.GetLength() == 0);

    // Тест конструктора копирования
    LinearForm<int> lf3(lf1);
    assert(lf3.GetLength() == 3);
    assert(lf3.Get(0) == 1);
    
    std::cout << "Passed!\n";
}

void TestOperations() {
    std::cout << "Running TestOperations... ";
    
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6};
    LinearForm<int> lf1(arr1, 3);
    LinearForm<int> lf2(arr2, 3);

    // Тест сложения
    auto sum = lf1 + lf2;
    assert(sum->GetLength() == 3);
    assert(sum->Get(0) == 5);
    assert(sum->Get(1) == 7);
    assert(sum->Get(2) == 9);
    delete sum;

    // Тест вычитания
    auto diff = lf2 - lf1;
    assert(diff->GetLength() == 3);
    assert(diff->Get(0) == 3);
    assert(diff->Get(1) == 3);
    assert(diff->Get(2) == 3);
    delete diff;

    // Тест умножения на скаляр
    auto mult = lf1 * 2;
    assert(mult->GetLength() == 3);
    assert(mult->Get(0) == 2);
    assert(mult->Get(1) == 4);
    assert(mult->Get(2) == 6);
    delete mult;
    
    std::cout << "Passed!\n";
}

void TestEvaluation() {
    std::cout << "Running TestEvaluation... ";
    
    double arr[] = {1.5, 2.0, 3.5};
    LinearForm<double> lf(arr, 3);
    
    // Тест вычисления с переменными
    double vars[] = {2.0, 3.0};
    MutableArraySequence<double> vars_seq(vars, 2);
    assert(lf.Evaluate(vars_seq) == 1.5 + 2.0*2.0 + 3.5*3.0);
    
    // Тест вычисления с одним x
    assert(lf.Evaluate(2.0) == 1.5 + 2.0*2.0 + 3.5*4.0);
    
    std::cout << "Passed!\n";
}

void TestGradient() {
    std::cout << "Running TestGradient... ";
    
    float arr[] = {1.0f, 2.0f, 3.0f, 4.0f};
    LinearForm<float> lf(arr, 4);
    
    auto grad = lf.Gradient();
    assert(grad->GetLength() == 3);
    assert(grad->Get(0) == 2.0f);
    assert(grad->Get(1) == 3.0f);
    assert(grad->Get(2) == 4.0f);
    delete grad;
    
    std::cout << "Passed!\n";
}

void TestEdgeCases() {
    std::cout << "Running TestEdgeCases... ";
    
    // Тест пустой формы
    LinearForm<int> empty;
    assert(empty.GetLength() == 0);
    
    // Тест исключений
    bool exceptionThrown = false;
    try {
        empty.GetFirst();
    } catch (const std::out_of_range&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    
    std::cout << "Passed!\n";
}

void RunFileTests() {
    std::cout << "Running FileTests... ";
    std::ofstream out("outputLin.txt");
    
    // Тест 1: Создание и базовые операции
    {
        int arr[] = {1, 2, 3};
        LinearForm<int> lf(arr, 3);
        
        out << "Test 1: Basic operations\n";
        out << "Linear form: " << lf.ToString() << "\n";
        out << "Length: " << lf.GetLength() << "\n";
        out << "First coefficient: " << lf.GetFirst() << "\n";
        out << "Last coefficient: " << lf.GetLast() << "\n\n";
    }
    
    // Тест 2: Математические операции
    {
        int arr1[] = {1, 2, 3};
        int arr2[] = {4, 5, 6};
        LinearForm<int> lf1(arr1, 3);
        LinearForm<int> lf2(arr2, 3);
        
        auto sum = lf1 + lf2;
        auto diff = lf2 - lf1;
        auto mult = lf1 * 2;
        
        out << "Test 2: Math operations\n";
        out << "Sum: " << sum->ToString() << "\n";
        out << "Difference: " << diff->ToString() << "\n";
        out << "Multiplication: " << mult->ToString() << "\n\n";
        
        delete sum;
        delete diff;
        delete mult;
    }
    
    // Тест 3: Вычисление значений
    {
        double arr[] = {1.5, 2.0, 3.5};
        LinearForm<double> lf(arr, 3);
        
        double vars[] = {2.0, 3.0};
        MutableArraySequence<double> vars_seq(vars, 2);
        
        out << "Test 3: Evaluation\n";
        out << "Form: " << lf.ToString() << "\n";
        out << "Evaluate with vars [2.0, 3.0]: " << lf.Evaluate(vars_seq) << "\n";
        out << "Evaluate with x=2.0: " << lf.Evaluate(2.0) << "\n\n";
    }
    
    // Тест 4: Градиент и производные
    {
        float arr[] = {1.0f, 2.0f, 3.0f, 4.0f};
        LinearForm<float> lf(arr, 4);
        
        auto grad = lf.Gradient();
        
        out << "Test 4: Gradient and derivatives\n";
        out << "Form: " << lf.ToString() << "\n";
        out << "Gradient: " << grad->ToString() << "\n";
        out << "Partial derivative by x1: " << lf.PartialDerivative(0) << "\n";
        out << "Partial derivative by x2: " << lf.PartialDerivative(1) << "\n\n";
        
        delete grad;
    }
    
    // Тест 5: Краевые случаи
    {
        LinearForm<int> empty;
        
        out << "Test 5: Edge cases\n";
        out << "Empty form length: " << empty.GetLength() << "\n";
        out << "Empty form string: " << empty.ToString() << "\n";
        
        try {
            empty.GetFirst();
        } catch (const std::exception& e) {
            out << "Exception caught: " << e.what() << "\n";
        }
    }
    
    out.close();
    std::cout << "Passed! Results written\n";
}