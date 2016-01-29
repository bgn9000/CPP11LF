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
    
    return 0;
}

