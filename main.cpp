#include "11_TestsLinearform.h"
#include "12_TestsStack.h"
#include "13_TestsSegmentedDeque.h"
#include "14_TestsBinaryTree.h"
#include "15_TestsSkipList.h"
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Starting tests linearform...\n\n";
    
    TestConstructors();
    TestOperations();
    TestEvaluation();
    TestGradient();
    TestEdgeCases();
    RunFileTests();
    
    std::cout << "BinaryTree tests...\n\n";
    runUnitTests();

    std::cout << "SegmentedDeque tests...\n\n";
    testSegmentedDeque();

    std::cout << "SkipList tests...\n\n";
    test();

    std::cout << "Stack tests...\n\n";
    testStack();

    return 0;
}