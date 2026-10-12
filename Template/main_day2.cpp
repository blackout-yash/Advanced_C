#include <numeric>
#include <iostream>
#include <vector>

using namespace std;

// Only one block compiles. The first "1" wins (now: block 8).

#if 0 // ---------- 1. Explicit (full) specialization ----------

template <typename T>
class Box {
public:
    void print() { cout << "Generic Box\n"; }
};

// Special version for exactly one type: int
template <>
class Box<int> {
public:
    void print() { cout << "Integer Box\n"; }
};

int main() {
    Box<double> b1;
    b1.print();  // Generic Box

    Box<int> b2;
    b2.print();  // Integer Box
    return 0;
}

#elif 0 // ---------- 2. Partial specialization (class) ----------

template <typename T>
class Box {
public:
    void print() { cout << "Generic Box\n"; }
};

// Special version for a pattern of types: any pointer T*
template <typename T>
class Box<T*> {
public:
    void print() { cout << "Pointer Box\n"; }
};

int main() {
    Box<int> b1;
    b1.print();   // Generic Box

    Box<int*> b2;
    b2.print();   // Pointer Box

    Box<double*> b3;
    b3.print();   // Pointer Box
    return 0;
}

#elif 0 // ---------- 3. Partial specialization in functions ----------

// Classes can be partially specialized. Functions can't.
// For functions, use overloading instead: the compiler picks the best match.

template <typename T>
void show(T value) { cout << "Generic\n"; }

// Full specialization: allowed
template <>
void show<int>(int value) { cout << "Integer\n"; }

// Partial specialization: NOT allowed
// template <typename T>
// void show<T*>(T* value) { cout << "Pointer\n"; }

// Overload: allowed, does the job of partial specialization
template <typename T>
void show(T* value) { cout << "Pointer\n"; }

int main() {
    int x = 1;
    show(2.5);   // Generic
    show(x);     // Integer
    show(&x);    // Pointer (overload wins for pointers)
    return 0;
}

#elif 0 // ---------- 4. Type deduction ----------

template <typename T1, typename T2>
auto sum(T1 a, T2 b) {
    return a + b;
}

int main() {
    // C++17: class template types can be deduced too:
    // vector arr = {1, 2, 3, 4};

    cout << sum(2.3, 5) << endl;            // 7.3 (T1=double, T2=int)
    cout << sum<int, int>(2.3, 5) << endl;  // 7   (types forced, 2.3 becomes 2)
    return 0;
}

#elif 0 // ---------- 5. Non-type template parameter ----------

// The template parameter is a value (N), not a type.
// Computed at compile time, so no cost at runtime.
// See generated code: https://cppinsights.io/

template <int N>
class Square {
public:
    static const int value = N * N;
};

int main() {
    // N must be known at compile time, so a normal variable is not allowed:
    // int n = 10;
    const int n = 10;
    cout << Square<n>::value << endl;   // 100
    return 0;
}

#elif 0 // ---------- 6. Default template parameters ----------

// Default is used only when the type is not given and can't be deduced.
template <typename T1, typename T2 = int>
auto sum(T1 a, T2 b) {
    return a + b;
}

template <int N = 10>
class Square {
public:
    static const int value = N * N;
};

int main() {
    cout << Square<>::value << endl;    // 100 (<> = use the default N)
    cout << Square<5>::value << endl;   // 25
    return 0;
}

#elif 0 // ---------- 7. Template template parameter ----------

// A template parameter that is itself a template (Comp is Comparator, not Comparator<int>).
template <typename T>
class Comparator {
public:
    bool compare(T a, T b) { return a <= b; }
};

// "template <typename> class Comp" = Comp takes one type argument
template <template <typename> class Comp, typename T>
bool check(T a, T b) {
    Comp<T> c;               // Comp is filled with T here
    return c.compare(a, b);
}

int main() {
    cout << check<Comparator>(3, 5) << endl;   // 1
    return 0;
}

#elif 1 // ---------- 8. Variadic templates ----------

// Base case: one argument left, stop here.
// Must be written BEFORE the variadic version.
template <typename T>
auto sum(T a) {
    return a;
}

// Args... = any number of types, rest... = the remaining arguments.
// Takes the first one and recurses on the rest:
// sum(1,2,3.3,4) -> 1 + sum(2,3.3,4) -> 1 + (2 + sum(3.3,4)) -> ...
template <typename T, typename... Args>
auto sum(T a, Args... rest) {
    return a + sum(rest...);
}

int main() {
    cout << sum(1, 2, 3.3, 4) << endl;   // 10.3
    return 0;
}

#endif

// g++ main_day2.cpp -o app
// g++ -std=c++17 main_day2.cpp -o app