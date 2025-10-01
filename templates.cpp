/*
Templates are basically the equivalent of generics for
cpp, like in other languages

Templates can be used for functions, classes and variables

Also the templates can be used using argument deduction but just 
after C++17:
    Max<int int>(int int) <- without argument deduction
    Max(int, int) <- with argument deduction
*/

#include <iostream>

// Templates using typename to infer the type
template <typename T> T Max(T x, T y) { return (x > y) ? x : y; };

// Class template
template <typename T> class C {
public:
  T x;
  T y;

  C(T x, T y) : x(x), y(y) {};
  void getValues() { std::cout << "x: " << x << " y " << y; };
};

/*
Templates can be passed on multiple data types
also the params passed to the templates can have default values
and non type arguments
 
*/
template <int T, typename T2 = double,
	 typename T3 = std::string> class M {
public:
  T x;
  T2 y;
  T3 z;

  M(T x, T2 y, T3 z) : x(x), y(y), z(z) {};
  void getValues() { std::cout << "x: " << x <<
      " y " << y << " z " << z; };
};

// Variable templates
template <typename T> constexpr T pi = T(3.14);


// template metaprogramming
// the templates does computations on compile time rather than runtime
template <int N> struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <> struct Factorial<0> {
    static const int value = 1;
};


/* 
Modern version, constexpr evaluates the value or 
the body of the function and if it can be proccess at compile time 
it does it else it just works as a normal function
*/ 
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : (n * factorial(n - 1));
}
















