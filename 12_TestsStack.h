#include "7_Stack.h"
#include "6_DataTypes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <memory>

void testStack() {
    // Открываем файл для записи результатов тестов
    std::ofstream outFile("outputStack.txt");
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
        return;
    }

    // Сохраняем оригинальный буфер cout и перенаправляем вывод в файл
    std::streambuf* originalCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(outFile.rdbuf());

    try {
        // Тест 1: Создание стека и базовые операции
        std::cout << "=== Test 1: Basic Stack Operations ===" << std::endl;
        Stack<int> stack1;
        stack1.Push(10);
        stack1.Push(20);
        stack1.Push(30);
        
        std::cout << "Stack after pushes: ";
        stack1.Print();
        
        std::cout << "Peek: " << stack1.Peek() << std::endl;
        std::cout << "Pop: " << stack1.Pop() << std::endl;
        std::cout << "Stack after pop: ";
        stack1.Print();
        std::cout << "Length: " << stack1.GetLength() << std::endl;
        std::cout << "Is empty: " << (stack1.IsEmpty() ? "true" : "false") << std::endl;

        // Тест 2: Конструкторы
        std::cout << "\n=== Test 2: Constructors ===" << std::endl;
        int items[] = {40, 50, 60};
        Stack<int> stack2(items, 3);
        std::cout << "Stack from array: ";
        stack2.Print();
        
        Stack<int> stack3(stack2);
        std::cout << "Copied stack: ";
        stack3.Print();

        // Тест 3: Функциональные операции
        std::cout << "\n=== Test 3: Functional Operations ===" << std::endl;
        auto square = [](int x) { return x * x; };
        auto isEven = [](int x) { return x % 2 == 0; };
        auto sum = [](int a, int b) { return a + b; };
        
        Stack<int>* mappedStack = stack2.Map(square);
        std::cout << "Mapped stack (squares): ";
        mappedStack->Print();
        delete mappedStack;
        
        Stack<int>* filteredStack = stack2.Where(isEven);
        std::cout << "Filtered stack (even numbers): ";
        filteredStack->Print();
        delete filteredStack;
        
        int total = stack2.Reduce(sum, 0);
        std::cout << "Reduced (sum): " << total << std::endl;

        // Тест 4: Операции с последовательностями
        std::cout << "\n=== Test 4: Sequence Operations ===" << std::endl;
        Stack<int>* concatenated = stack1.Concat(stack2);
        std::cout << "Concatenated stacks: ";
        concatenated->Print();
        delete concatenated;
        
        Stack<int>* subsequence = stack2.GetSubsequence(1, 2);
        std::cout << "Subsequence (1-2): ";
        subsequence->Print();
        delete subsequence;
        
        int sub[] = {50, 60};
        bool contains = stack2.ContainsSubsequence(sub, 2);
        std::cout << "Contains subsequence [50, 60]: " << (contains ? "true" : "false") << std::endl;

        // Тест 5: Исключения
        std::cout << "\n=== Test 5: Exceptions ===" << std::endl;
        try {
            Stack<int> emptyStack;
            emptyStack.Pop();
        } catch (const std::exception& e) {
            std::cout << "Exception caught (Pop on empty): " << e.what() << std::endl;
        }
        
        try {
            stack2.GetSubsequence(-1, 5);
        } catch (const std::exception& e) {
            std::cout << "Exception caught (Invalid index): " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cout << "\n!!! Exception caught in test: " << e.what() << " !!!" << std::endl;
    }

    // Восстанавливаем оригинальный буфер cout
    std::cout.rdbuf(originalCoutBuffer);
    outFile.close();

    std::cout << "Тесты стека завершены. Результаты записаны в outputStack.txt" << std::endl;
}
