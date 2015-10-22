#include <iostream>
#include <typeinfo>

double onefunction();
double secondfunction(double x) { return x*x; }
class toto
{
};

template<class F, class X>
auto wrapper(F func, X x) -> decltype(func(x))
{
    std::cout << "Func called" << std::endl;
    return func(x);
}

int main()
{
    // auto is more convenient than decltype (used to get return type if not statically known)
    decltype(onefunction()) f1;
    std::cout << "type is " << typeid(f1).name() << std::endl; // d
    
    decltype(onefunction) f1b;
    std::cout << "type is " << typeid(f1b).name() << std::endl; // FdvE
    
    decltype(secondfunction) f2;
    std::cout << "type is " << typeid(f2).name() << std::endl; // FddE
    
    std::cout << "type is " << typeid(toto).name() << std::endl;
    
    // return type
    auto res = wrapper(secondfunction, 2);
    std::cout << "type is " << typeid(res).name() << " and res=" << res << std::endl;
}

