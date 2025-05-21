#include "10_SkipList.h"
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime>  

void test() {
    std::ofstream outFile("outputSkip.txt");
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
        return;
    }


    // Сохраняем оригинальный буфер cout
    std::streambuf *coutBuffer = std::cout.rdbuf();
    
    // Перенаправляем вывод cout в файл
    std::cout.rdbuf(outFile.rdbuf());

    // Инициализация генератора случайных чисел
    std::srand(std::time(nullptr));

    SkipList<int> sl(4, 0.5);
    
    // Тест вставки
    for (int i = 0; i < 30; ++i) {
        sl.insert(std::rand() % 100);
    }
    sl.print(); // Предполагается, что print принимает ostream
    
    // Тест поиска
    outFile << "Contains 50: " << sl.contains(50) << std::endl;
    outFile << "Contains 25: " << sl.contains(25) << std::endl;
    outFile << "Contains 75: " << sl.contains(75) << std::endl;
    
    // Тест удаления
    try {
        sl.remove(50);
    } catch (const std::exception& e) {
        outFile << e.what() << std::endl;
    }
    
    // Тест итератора
    outFile << "All elements via iterator: ";
    for (auto it = sl.begin(); it != sl.end(); ++it) {
        outFile << *it << " ";
    }
    outFile << "\nTotal elements: " << sl.getSize() << std::endl;

    // Восстанавливаем оригинальный буфер cout перед закрытием файла
    std::cout.rdbuf(coutBuffer);
    outFile.close();
    
    // Теперь этот вывод пойдет в консоль, а не в файл
    std::cout << "Тесты завершены. Результаты записаны в outputSkip.txt" << std::endl;
}