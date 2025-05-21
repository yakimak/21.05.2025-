/*
#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "0_Sequence.h"
#include "1_ArraySequence.h"
#include "2_ListSequence.h"
#include "3_DynamicArray.h"
#include "4_LinkedList.h"

class Tests {
public:
    static void RunAllTests(std::ostream& out, std::istream& in);

private:
    static std::vector<int> read_numbers(std::istringstream& iss);

    static void TestDynamicArray(std::ostream& out, std::istringstream& iss);
    static void TestLinkedList(std::ostream& out, std::istringstream& iss);
    static void TestMutableArraySequence(std::ostream& out, std::istringstream& iss);
    static void TestImmutableArraySequence(std::ostream& out, std::istringstream& iss);
    static void TestMutableListSequence(std::ostream& out, std::istringstream& iss);
    static void TestImmutableListSequence(std::ostream& out, std::istringstream& iss);

    static void TestSequenceFromSequence(std::ostream& out, std::istringstream& iss);

    static void TestLinearForm(std::ostream& out);
    static void TestLinearFormInt(std::ostream& out);
    static void TestLinearFormDouble(std::ostream& out);
    static void TestLinearFormComplex(std::ostream& out);
    static void TestLinearFormOperations(std::ostream& out, const char* typeName);
};

//////////////////////////////////////////////////////////////////////////////////////////////////

void Tests::RunAllTests(std::ostream& out, std::istream& in) 
{
    std::string line;
    
    while (std::getline(in, line)) 
    {
        if (line.empty() || line[0] == '#') 
            continue;
        try {

        std::istringstream iss(line);
        std::string test_type;
        iss >> test_type;

        if (test_type == "DynamicArray") {
            TestDynamicArray(out, iss);
        } 
        else if (test_type == "LinkedList") {
            TestLinkedList(out, iss);
        }
        else if (test_type == "MutableArraySequence") {
            TestMutableArraySequence(out, iss);
        }
        else if (test_type == "ImmutableArraySequence") {
            TestImmutableArraySequence(out, iss);
        }
        else if (test_type == "MutableListSequence") {
            TestMutableListSequence(out, iss);
        }
        else if (test_type == "ImmutableListSequence") {
            TestImmutableListSequence(out, iss);
        }
        else if (test_type == "SequenceFromSequence") {
            TestSequenceFromSequence(out, iss);
        }
        else {
            out << "Unknown test type: " << test_type << "\n";
        }
    }
    catch (const std::exception& e) {
        out << "Test failed: " << e.what() << "\n";
    }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<int> Tests::read_numbers(std::istringstream& iss) 
{
    std::vector<int> numbers;
    int num;
    
    while (iss >> num) {
        numbers.push_back(num);
    }
    
    return numbers;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tests::TestDynamicArray(std::ostream& out, std::istringstream& iss) 
{
    auto numbers = read_numbers(iss);
    DynamicArray<int> da(numbers.data(), numbers.size());
    
    out << "\n=== DynamicArray Test ===\n";
    out << "Created: " << da.ToString() << "\n";
}

void Tests::TestLinkedList(std::ostream& out, std::istringstream& iss) 
{
    auto numbers = read_numbers(iss);
    LinkedList<int> ll(numbers.data(), numbers.size());
    
    out << "\n=== LinkedList Test ===\n";
    out << "Created: " << ll.ToString() << "\n";
}

void Tests::TestMutableArraySequence(std::ostream& out, std::istringstream& iss) 
{
    auto numbers = read_numbers(iss);
    MutableArraySequence<int> seq;
    
    for (int num : numbers) 
        seq.Append(num);
    
    out << "\n=== MutableArraySequence Test ===\n";
    out << "Sequence: " << seq.ToString() << "\n";
}

void Tests::TestImmutableArraySequence(std::ostream& out, std::istringstream& iss) 
{
    auto numbers = read_numbers(iss);
    ImmutableArraySequence<int> seq;
    Sequence<int>* current = &seq;
    
    for (int num : numbers) {
        current = current->Append(num);
    }
    
    out << "\n=== ImmutableArraySequence Test ===\n";
    out << "Final sequence: " << current->ToString() << "\n";
    
    delete current;
}

void Tests::TestMutableListSequence(std::ostream& out, std::istringstream& iss) 
{
    auto numbers = read_numbers(iss);
    MutableListSequence<int> seq;
    
    for (int num : numbers) 
        seq.Append(num);
    
    out << "\n=== MutableListSequence Test ===\n";
    out << "Sequence: " << seq.ToString() << "\n";
}

void Tests::TestImmutableListSequence(std::ostream& out, std::istringstream& iss) 
{
    auto numbers = read_numbers(iss);
    ImmutableListSequence<int> seq;
    Sequence<int>* current = &seq;
    
    for (int num : numbers) {
        current = current->Append(num);
    }
    
    out << "\n=== ImmutableListSequence Test ===\n";
    out << "Final sequence: " << current->ToString() << "\n";
    
    delete current;
}

void Tests::TestSequenceFromSequence(std::ostream& out, std::istringstream& iss) {
    Sequence<int>* src = nullptr;
    Sequence<int>* dest = nullptr;
    Sequence<int>* combined = nullptr;

    try {
        std::string src_type, dest_type;
        iss >> src_type >> dest_type;
        
        auto numbers = read_numbers(iss);
        
        out << "\n=== Sequence From Sequence Test ===\n";
        
        if (src_type == "ArraySequence") {
            src = new MutableArraySequence<int>(numbers.data(), numbers.size());
        } 
        else if (src_type == "ListSequence") {
            src = new MutableListSequence<int>(numbers.data(), numbers.size());
        }
        else {
            throw std::invalid_argument("Unknown source type: " + src_type);
        }
        
        out << "Source (" << src_type << "): " << src->ToString() << "\n";
        
        
        if (dest_type == "ArraySequence") {
            dest = new MutableArraySequence<int>();
        } 
        else if (dest_type == "ListSequence") {
            dest = new MutableListSequence<int>();
        }
        else {
            throw std::invalid_argument("Unknown destination type: " + dest_type);
        }
        
        
        for (int i = 0; i < src->GetLength(); i++) {
            dest->Append(src->Get(i));
        }
        
        out << "Converted to " << dest_type << ": " << dest->ToString() << "\n";
        
        
        combined = dest->Concat(src);
        out << "Concatenated result: " << combined->ToString() << "\n";
        
    } catch (const std::exception& e) {
        out << "Error: " << e.what() << "\n";
        delete src;
        delete dest;
        delete combined;
        throw; 
    }
    
    delete src;
    delete dest;
    delete combined;
}

#endif

*/ 