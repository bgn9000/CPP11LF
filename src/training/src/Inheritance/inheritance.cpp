#include <iostream>

class DelegatingConstructor
{
    double val;
    
public:
    DelegatingConstructor(int v) : DelegatingConstructor(static_cast<double>(v)) { std::cout << "DelegatingConstructor: int= " << val << std::endl; }
    DelegatingConstructor(std::string str) : DelegatingConstructor(std::stod(str)) { std::cout << "DelegatingConstructor: string=" << val << std::endl; }
    DelegatingConstructor(double v = 0.0) : val(v) { std::cout << "DelegatingConstructor: double=" << val << std::endl; }
};

class InheritingConstructor : public DelegatingConstructor
{
public:
    // Instead of rewriting above constructors which is modification later error prone
    using DelegatingConstructor::DelegatingConstructor;
};

class OverrideAndFinal
{
public:
    virtual void f() const { std::cout << "call OverrideAndFinal::f()" << std::endl; }
    virtual void f(int) const { std::cout << "call OverrideAndFinal::f(int) : overloading f" << std::endl; }
    virtual void f(float) const { std::cout << "call OverrideAndFinal::f(float) : overloading f" << std::endl; }
    virtual void g() final { std::cout << "call OverrideAndFinal::g()" << std::endl; }
};

class OverrideAndFinal2 : public OverrideAndFinal
{
public:
    // override (special identifier): the compiler will check the base class(es) to see if there is a virtual function with this exact signature. 
    // And if there is not, the compiler will error out.
    void f() const override { std::cout << "call OverrideAndFinal2::f()" << std::endl; }
    //errors raised: 
    //  void f() override {}
    //  void f(int) override {}
    //  void h() override {}
    
    //error: no further overriding because of final
    //  void g() {}
    
    // comment this will produce errors below
    using OverrideAndFinal::f;
};

int main()
{
    DelegatingConstructor delegate;
    DelegatingConstructor delegate2(100);
    DelegatingConstructor delegate3(std::string("500.1"));
    InheritingConstructor inherite(200);
    InheritingConstructor inherite2("600.35");
    
    OverrideAndFinal overrideFinal;
    overrideFinal.f();
    overrideFinal.f(1);
    overrideFinal.f(1.0f);
    overrideFinal.g();
    OverrideAndFinal2 overrideFinal2;
    overrideFinal2.f();
    //if comment using then 2 errors (because overriding hides overloading): 
    overrideFinal2.f(1);
    overrideFinal2.f(1.0f);
    overrideFinal2.g();
    return 0;
}

