#include <iostream>
#include <vector>
#include <memory>

class AnyType
{
    struct TypeErasure
    {
        virtual void display(std::ostream& os) const = 0;
    };
    
    template<typename T> 
    struct Wrapper : TypeErasure 
    {
        Wrapper(const T& obj) : wrappedObj_(obj) {}        
        virtual void display(std::ostream& os) const { os << wrappedObj_; }
        
    private:
        T wrappedObj_;
    };
    
    std::shared_ptr<TypeErasure> object_;

    public:
        template<typename T> 
        AnyType(T&& obj) noexcept : object_(new Wrapper<T>(obj)) {}
        
        friend std::ostream& operator<<(std::ostream& os, const AnyType& anyType);
};

std::ostream& operator<<(std::ostream& os, const AnyType& anyType)
{
    anyType.object_->display(os);
    return os;
}

int addition(int a, int b) { return a + b; }
int substraction(int a, int b) { return a - b; }
int multiplication(int a, int b) { return a * b; }
int division(int a, int b) { return a / b; }

int main() {
    std::vector<AnyType> myVect;
    myVect.push_back(1);
    myVect.push_back(1.5);
    myVect.push_back('h');
    myVect.push_back("hello");
    std::string str("hi");
    myVect.push_back(str);
    for (auto& elem : myVect)
    {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
    
    std::vector<std::function<int()>> calculator;
    calculator.push_back(std::bind(addition, 5, 6));
    calculator.push_back(std::bind(substraction, 5, 6));
    calculator.push_back(std::bind(multiplication, 5, 6));
    calculator.push_back(std::bind(division, 5, 6));
    for (auto& elem : calculator)
    {
        std::cout << elem() << ' ';
    }
    std::cout << std::endl;
    
    std::vector<int> results;
    auto add = std::bind(addition, std::placeholders::_1, std::placeholders::_2);
    std::cout << add(7, 7) << std::endl;
    auto sub = std::bind(substraction, std::placeholders::_1, std::placeholders::_2);
    auto mult = std::bind(multiplication, std::placeholders::_1, std::placeholders::_2);
    auto div = std::bind(division, std::placeholders::_1, std::placeholders::_2);
    results.push_back(add(7, 7));
    results.push_back(sub(7, 7));
    results.push_back(mult(7, 7));
    results.push_back(div(7, 7));
    for (auto& elem : results)
    {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
    
    return 0;
}

