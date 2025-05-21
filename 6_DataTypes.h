#include <iostream>
#include <string>
#include <ctime>
#include "1_ArraySequence.h"

class Complex {
private:
    double Re;
    double Im;

public:
    Complex(double Re, double Im) : Re(Re), Im(Im) {}
    Complex() : Re(0), Im(0) {}

    double GetRe() const { return Re; }
    double GetIm() const { return Im; }
    void Print() const { std::cout << Re << " + i*" << Im; }

    bool operator==(const Complex& other) const {
        return Re == other.Re && Im == other.Im;
    }
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
};


/////////////////////////////////////////////////////////////////////////////////////////

class Person {
protected:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    time_t birthDate;

public:
    Person(const std::string& fName, const std::string& mName, 
          const std::string& lName, time_t bDate)
        : firstName(fName), middleName(mName), lastName(lName), birthDate(bDate) {}

    virtual ~Person() = default;

    virtual std::string GetID() const = 0;
    std::string GetFullName() const { return firstName + " " + middleName + " " + lastName; }
    time_t GetBirthDate() const { return birthDate; }
    virtual void Print() const = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class Student : public Person {
private:
    int studentId;

public:
    Student(int id, const std::string& fName, const std::string& mName,
           const std::string& lName, time_t bDate)
        : Person(fName, mName, lName, bDate), studentId(id) {}

    std::string GetID() const override { return std::to_string(studentId); }
    
    void Print() const override {
        std::cout << "Student: " << GetFullName() 
                 << ", ID: " << GetID() 
                 << ", Birth: " << std::ctime(&birthDate);
    }
};

class Teacher : public Person {
private:
    size_t teacherId;
    size_t department;

public:
    Teacher(size_t id, const std::string& fName, const std::string& mName,
           const std::string& lName, time_t bDate, size_t dept)
        : Person(fName, mName, lName, bDate), teacherId(id), department(dept) {}

    std::string GetID() const override { return std::to_string(teacherId); }
    std::string GetDepartment() const { return std::to_string(department); }
    
    void Print() const override {
        std::cout << "Teacher: " << GetFullName() 
                 << ", ID: " << GetID()
                 << ", Dept: " << GetDepartment()
                 << ", Birth: " << std::ctime(&birthDate);
    }
};

class FunctionContainer {
private:
    MutableArraySequence<int(*)(int)> functions;

public:
    void AddFunction(int(*func)(int)) {
        functions.Append(func);
    }

    int ExecuteAll(int initialValue) {
        int result = initialValue;
        for (int i = 0; i < functions.GetLength(); ++i) {
            result = functions.Get(i)(result);
        }
        return result;
    }
};

