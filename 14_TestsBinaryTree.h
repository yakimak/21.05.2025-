#include "9_BinaryTree.h"
#include <cassert>
#include <fstream>
#include <random>
#include <functional>

void runUnitTests() {
    // Открываем файл для записи
    std::ofstream outFile("outputTree.txt");
    if (!outFile) {
        std::cerr << "Не удалось открыть файл output.txt для записи" << std::endl;
        return;
    }

    BinaryTree<int> bigTree;
    
    // Заполняем дерево 30 случайными значениями
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 90);
    
    for (int i = 0; i < 90; ++i) {
        bigTree.insert(distrib(gen));
    }
    
    // Выводим основное дерево
    outFile << "Основное дерево:" << std::endl;
    // Для PrintPretty нужно перенаправить вывод в файл
    // Сохраняем оригинальный буфер cout
    std::streambuf* origBuf = std::cout.rdbuf();
    // Перенаправляем cout в файл
    std::cout.rdbuf(outFile.rdbuf());
    bigTree.PrintPretty();
    // Восстанавливаем оригинальный буфер cout
    std::cout.rdbuf(origBuf);
    outFile << std::endl;
    
    // Выбираем корень для поддерева (например, значение из середины)
    int subtreeRootValue = bigTree.getByPath("L"); // Левый потомок корня
    auto subtree = bigTree.extractSubtree(subtreeRootValue);
    
    // Выводим поддерево
    outFile << "\nПоддерево с корнем " << subtreeRootValue << ":" << std::endl;
    if (subtree) {
        // Перенаправляем вывод для PrintPretty
        std::cout.rdbuf(outFile.rdbuf());
        subtree->PrintPretty();
        std::cout.rdbuf(origBuf);
        
        // Проверяем, содержится ли поддерево в основном дереве
        bool contains = bigTree.containsSubtree(*subtree);
        outFile << "\nПоддерево " << (contains ? "содержится" : "не содержится") 
                << " в основном дереве" << std::endl;
    } else {
        outFile << "Не удалось извлечь поддерево" << std::endl;
    }
    
    // Дополнительная информация
    outFile << "\nДополнительная информация:" << std::endl;
    outFile << "KPL сериализация основного дерева: " << bigTree.serialize("KPL") << std::endl;
    outFile << "LKP сериализация поддерева: " << subtree->serialize("LKP") << std::endl;

    outFile.close();
    std::cout << "Тесты завершены. Результаты записаны" << std::endl;
}