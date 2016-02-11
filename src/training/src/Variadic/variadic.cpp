#include <iostream>
#include <string>

#include <tuple>
#include <utility>

#include <typeinfo>
#include <cxxabi.h>

/*
** Variadic template type: template <typename... Args> MyClass {}
** Variadic template function: template <typename... Args> void MyFunction(Args... args) {}
** Args and args are parameter packs : neither a type nor a value, this is a list of types alias.
** (can only count with sizeof... or expand in another expression)
** 
** Expansion rules:
** Args... => Arg1, Arg2, ..., ArgN
** Args&... => Arg1&, Arg2&, ..., ArgN&
** Args&&... => Arg1&&, Arg2&&, ..., ArgN&&
** func(5, Args)... => func(5, Arg1), func(5, Arg2), ..., func(5, ArgN)
*/

// example 1
template <class T>
constexpr auto size(const T&)
{
    return std::tuple_size<T>::value;
}

template <typename... Args>
constexpr auto size2(const std::tuple<Args...>&)
{
        return sizeof...(Args);
}

// example 2
// terminal case: one parameter left (order is important)
template<class T>
constexpr T SumArgs(const T& arg)
{
// uncomment to see intermediate cout but need to comment constexpr in results below:
//    std::cout << "(1:" << arg << ") ";
    return arg;
}
template<class T, class... Args>
constexpr T SumArgs(const T& arg, const Args&... args)
{
// uncomment to see intermediate cout (and new operator sizeof... behavior) but need to comment constexpr in results below:
//    std::cout << "(" << sizeof...(args)+1 << ":" << arg << ") ";
    return arg+SumArgs(args...);
}

// example 3
template<typename... Ints> class Stack;
template<> class Stack<> // terminal case (order is important)
{
public:
    void print() const 
    {
        std::cout << std::endl;
    }
}; 
template<typename Int, typename... Ints> 
class Stack<Int, Ints...> : public Stack<Ints...>
{
    Int head;
public:
    constexpr Stack(Int i, Ints... is) : Stack<Ints...>{is...}, head(i) {}
    void print() const
    {
        std::cout << head << ' ';
        Stack<Ints...>::print();
    }
};

// helper function to simplify stack creation
template<typename... Ints> 
constexpr auto makeStack(Ints... is) -> Stack<Ints...> 
{ 
    return Stack<Ints...>(is...); 
}

// example 4
template<int... Ints> struct Seq
{
};

template<int i, typename T> struct expand;
template<int i, int... is>
struct expand<i, Seq<is...>>
{
    typedef Seq<is..., i> type;
};

template<typename T, typename S> struct add;
template<int... is, int... js>
struct add<Seq<is...>, Seq<js...>>
{
    typedef Seq<is..., js...> type;
};

template<int n, typename... T> struct get;
template<int i, int... is> struct get<0, Seq<i, is...>> { static constexpr int value = i; };
template<int n, int i, int... is> struct get<n, Seq<i, is...>> { static constexpr int value = get<n-1, Seq<is...>>::value; };

// example 5
template<size_t> struct SeqPos
{
};

template <class Tuple, size_t Pos>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, SeqPos<Pos> ) 
{
  out << std::get<std::tuple_size<Tuple>::value-Pos>(t) << ", ";
  return print_tuple(out, t, SeqPos<Pos-1>());
}

template <class Tuple>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, SeqPos<1> ) 
{
  return out << std::get<std::tuple_size<Tuple>::value-1>(t);
}

template <class... Args>
std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& t) 
{
  out << '('; 
  print_tuple(out, t, SeqPos<sizeof...(Args)>());
  return out << ')';
}

int main()
{    
    // first example of variadic : template<class... Args> class tuple;
        
    // two ways to create a tuple:
    std::tuple<int, std::string, double> foo (1000, "hi", 3.14);
    auto bar = std::make_tuple("hello", 1.42, 'a', 75);
    
    // get sizes
    auto size_foo = size(foo);
    auto size_bar = size(bar);
    std::cout << "Size of foo=" << size_foo << " bar=" << size_bar << std::endl;
    
    size_foo = size2(foo);
    size_bar = size2(bar);
    std::cout << "Size (v2) of foo=" << size_foo << " bar=" << size_bar << std::endl;
    
    // display elements of tuple
    std::cout << "foo=(" << std::get<0>(foo) << ", " << std::get<1>(foo) << ", " << std::get<2>(foo) << ')' << std::endl;
    
    // swap tuples
    std::tuple<int, std::string, double> foo2 (2000, "hi there", 1.0/3);
    std::cout << "foo2=(" << std::get<0>(foo2) << ", " << std::get<1>(foo2) << ", " << std::get<2>(foo2) << ')' << std::endl;
    foo.swap(foo2);
    std::cout << "after swap, foo=(" << std::get<0>(foo) << ", " << std::get<1>(foo) << ", " << std::get<2>(foo) << ')' << std::endl;
    std::cout << "after swap, foo2=(" << std::get<0>(foo2) << ", " << std::get<1>(foo2) << ", " << std::get<2>(foo2) << ')' << std::endl;
    
    // modify element (need same type)
    std::cout << "before modify, bar=(" 
        << std::get<0>(bar) << ", " << std::get<1>(bar) << ", " << std::get<2>(bar) << ", " << std::get<3>(bar) << ')' << std::endl;
    std::get<0>(bar) = "hello world";
    std::get<1>(bar) = 2.0;
    std::cout << "after modify, bar=(" 
        << std::get<0>(bar) << ", " << std::get<1>(bar) << ", " << std::get<2>(bar) << ", " << std::get<3>(bar) << ')' << std::endl;
    
    // unpack elements
    auto myint = 0;
    auto mydouble = 0.0;
    std::tie(myint, std::ignore, mydouble) = foo;
    std::cout << "myint=" << myint << " mydouble=" << mydouble << std::endl;
    
    // second example : sum of args (compile time)
    
    // strip off all the parameters passed to SumArgs using a template consuming a parameter and a pack.
    // SumAgrs does not call itself recursively : it calls another instantiation with one less parameter (no recursion).
    constexpr auto sumInts = SumArgs(1, 2, 3, 4, 5);
    constexpr auto sumFloats = SumArgs(1.1, 2.2, 3.3, 4.4, 5.5);
    auto sumStrings = SumArgs(std::string("hello "), std::string("world !")); // constexpr result not allowed for such type
    std::cout << "SumArgs(int)=" << sumInts << std::endl;
    std::cout << "SumArgs(float)=" << sumFloats << std::endl;
    std::cout << "SumArgs(string)=" << sumStrings << std::endl;
    
    // third example : variable-sized integer sequences
    
    constexpr auto stack = Stack<int, int, int>(10, 20, 30);
    stack.print();
    constexpr auto stack2 = makeStack(40, 50, 60);
    stack2.print();
    
    // fourth example: manipulate integer sequences (compile time)
    typedef Seq<10,20,30> MySeq;
    auto status = 0;
    std::cout << abi::__cxa_demangle(typeid(MySeq).name(), 0, 0, &status) << std::endl;
    typedef expand<40, MySeq>::type MySeq2;
    std::cout << abi::__cxa_demangle(typeid(MySeq2).name(), 0, 0, &status) << std::endl;
    typedef add<MySeq2, MySeq>::type MySeq3;
    std::cout << abi::__cxa_demangle(typeid(MySeq3).name(), 0, 0, &status) << std::endl;
    std::cout << "MySeq=" 
        << get<0, MySeq>::value << ' ' << get<1, MySeq>::value << ' ' << get<2, MySeq>::value 
        << std::endl;
    std::cout << "MySeq3="
        << get<0, MySeq3>::value << ' ' << get<1, MySeq3>::value << ' ' << get<2, MySeq3>::value << ' '
        << get<3, MySeq3>::value << ' ' << get<4, MySeq3>::value << ' ' << get<5, MySeq3>::value << ' '
        << get<6, MySeq3>::value << std::endl;


    // fifth example : display tuple
    std::cout << "foo=" << foo << std::endl;
    std::cout << "foo2=" << foo2 << std::endl;
    std::cout << "bar=" << bar << std::endl;
    return 0;
}

