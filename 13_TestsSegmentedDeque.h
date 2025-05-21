#include "8_SegmentedDeque.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <functional>

void testSegmentedDeque() {
    // Открываем файл для записи
    std::ofstream outFile("outputSegm.txt");
    if (!outFile) {
        std::cerr << "Не удалось открыть файл outputSegm.txt для записи" << std::endl;
        return;
    }

    outFile << "=== Starting SegmentedDeque tests ===" << std::endl;
    
    // Тест 1: Добавление элементов в конец
    outFile << "\nTest 1: Push back 49 elements...";
    SegmentedDeque<int> deque;
    for (int i = 0; i < 49; ++i) {
        deque.push_back(i);
    }
    outFile << " size = " << deque.size();
    assert(deque.size() == 49);
    outFile << " ✓" << std::endl;
    
    // Проверка сегментов
    outFile << "Checking segment distribution...";
    outFile << "\n  Segment 0 [0-15]: " << deque[0] << ".." << deque[15];
    outFile << "\n  Segment 1 [16-31]: " << deque[16] << ".." << deque[31];
    outFile << "\n  Segment 2 [32-47]: " << deque[32] << ".." << deque[47];
    outFile << "\n  Segment 3 [48]: " << deque[48];
    assert(deque[0] == 0);
    assert(deque[15] == 15);
    assert(deque[16] == 16);
    assert(deque[31] == 31);
    assert(deque[32] == 32);
    assert(deque[48] == 48);
    outFile << " ✓" << std::endl;
    
    // Тест 2: Добавление в начало
    outFile << "\nTest 2: Push front...";
    deque.push_front(-1);
    outFile << " new size = " << deque.size();
    outFile << ", first element = " << deque[0];
    assert(deque.size() == 50);
    assert(deque[0] == -1);
    outFile << " ✓" << std::endl;
    
    // Тест 3: Удаление элементов
    outFile << "\nTest 3: Pop operations...";
    int front_val = deque.pop_front();
    int back_val = deque.pop_back();
    outFile << "\n  Popped front: " << front_val;
    outFile << "\n  Popped back: " << back_val;
    outFile << "\n  New size: " << deque.size();
    assert(front_val == -1);
    assert(back_val == 48);
    assert(deque.size() == 48);
    outFile << " ✓" << std::endl;
    
    // Тест 4: Функциональные операции
    outFile << "\nTest 4: Functional operations...";
    
    // Map
    auto squared = deque.map<int>([](const int& x) { return x * x; });
    outFile << "\n  Map (square): input=" << deque[5] << ", output=" << squared[5];
    assert(squared.size() == 48);
    assert(squared[5] == 25);
    
    // Where
    auto evens = deque.where([](const int& x) { return x % 2 == 0; });
    outFile << "\n  Where (even): count=" << evens.size() << "/" << deque.size();
    assert(evens.size() == 24);
    
    // Reduce
    int sum = deque.reduce<int>([](int acc, const int& x) { return acc + x; }, 0);
    int expected_sum = (0 + 47) * 48 / 2;
    outFile << "\n  Reduce (sum): " << sum << " (expected " << expected_sum << ")";
    assert(sum == expected_sum);
    outFile << " ✓" << std::endl;
    
    // Тест 5: Конкатенация
    outFile << "\nTest 5: Concatenation...";
    SegmentedDeque<int> other;
    other.push_back(100);
    other.push_back(101);
    auto combined = deque.concat(other);
    outFile << "\n  Combined size: " << combined.size();
    outFile << "\n  Last elements: " << combined[48] << ", " << combined[49];
    assert(combined.size() == 50);
    assert(combined[48] == 100);
    assert(combined[49] == 101);
    outFile << " ✓" << std::endl;
    
    // Тест 6: Сортировка
    outFile << "\nTest 6: Sorting...";
    SegmentedDeque<int> unsorted;
    for (int i = 0; i < 20; ++i) {
        unsorted.push_back((i * 17) % 20); // Создаем неупорядоченную последовательность
    }
    
    outFile << "\n  Before sort:";
    for (int i = 0; i < 5; ++i) outFile << " " << unsorted[i];
    outFile << " ...";
    for (int i = 15; i < 20; ++i) outFile << " " << unsorted[i];
    
    unsorted.sort(); // Сортируем
    
    outFile << "\n  After sort:";
    for (int i = 0; i < 20; ++i) outFile << " " << unsorted[i];
    
    bool is_sorted = true;
    for (int i = 0; i < 19; ++i) {
        if (unsorted[i] > unsorted[i+1]) {
            is_sorted = false;
            break;
        }
    }
    assert(is_sorted);
    outFile << " ✓" << std::endl;
    
    outFile << "\n=== All SegmentedDeque tests passed successfully! ===" << std::endl;
    
    outFile.close();
    std::cout << "Тесты завершены. Результаты записаны в outputSegm.txt" << std::endl;
}