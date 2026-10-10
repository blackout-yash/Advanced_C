#include <iostream>

using namespace std;

# if 0
// ---------- 1. Basic function template, two types ----------

// typename and class mean the same here.
// Only exception: typename is a must for T::value_type (see block 2).

// Old way: return type must be written by hand
// template <typename T1, typename T2>
// T1 add(T1 a, T2 b) { return a + b; }

// Trailing return type.
// Gotcha: decltype(a, b) is the comma operator, so it gives type of b only.
// add(3.3, 6) would return int. Use decltype(a + b) instead.
// template <typename T1, typename T2>
// auto add(T1 a, T2 b) -> decltype(a + b) { return a + b; }

// C++14 onwards: plain auto, compiler finds the return type
template <typename T1, typename T2>
auto add(T1 a, T2 b) {
    return a + b;   
}

// Compiler makes a separate function for every type combo you call.
// See the generated code: https://cppinsights.io/

// Template definition must be in the header,
// else use explicit instantiation in the .cpp.

int main() {
    cout << add(3.3, 6) << endl;     // double + int -> double
    cout << add(3, 6.6) << endl;     // int + double -> double

    string str1 = "abc", str2 = "def";
    cout << add(str1, str2) << endl; // string + string works

    // Error: "abc" is const char*, and pointer + pointer is not allowed
    // cout << add("abc", "def") << endl;

    return 0;
}

#elif 0
// ---------- 2. T::value_type with STL containers ----------

// Every STL container defines a type called value_type inside itself.
// It is the type of the elements the container stores:
//   vector<int>::value_type    -> int
//   vector<string>::value_type -> string
// This lets sum1 work for any container and pick the right result type.

// Problem: T is unknown until the function is called, so the compiler
// can't look inside T yet. T::value_type could be:
//   1. a type      (vector<int>::value_type is int)
//   2. a variable  (some other class could have a static member named value_type)
// The compiler can't tell which one, so it assumes "not a type" by default.
//
// typename says: "trust me, this is a type".
// If you skip it, you get an error like:
//   "need 'typename' before 'T::value_type' because 'T' is a dependent scope"

// Why not "class T::value_type"?
// "class X" means "X is a class type". But value_type is often not a class:
// vector<int>::value_type is int. So class is wrong here.
// typename works for any type (int, double, string, ...).
template <typename T>
typename T::value_type sum1(T arr) {
    // {} = value-initialize: 0 for int, empty for string
    // typename T::value_type result = typename T::value_type {};
    typename T::value_type result {};
    for(auto it: arr) {
        result += it;
    }
    return result;
}

// Same thing, but auto hides the long return type
template <typename T>
auto sum2(T arr) {
    auto result = typename T::value_type {};
    for(auto it: arr) {
        result += it;
    }
    return result;
}

int main() {
    vector <int> arr = {1, 2, 3, 4};
    cout << sum1(arr) << " " << sum2(arr) << endl;   // 10 10

    vector <string> str = {"abc", "def"};
    cout << sum1(str) << " " << sum2(str) << endl;   // abcdef abcdef

    return 0;
}

# elif 0
// ---------- 3. Class template ----------

template <typename T1, typename T2>
class Pair {
    T1 a; 
    T2 b;
public:
    Pair(T1 a, T2 b): a(a), b(b) {}

    void print() {
        cout << a << " " << b << endl;
    }
};

class Person {
    int age;
    string name;

public:
    Person(int age, string name) : age(age), name(name) {}

    // Needed so cout can print a Person (Pair::print uses <<)
    friend ostream& operator<<(ostream &os, const Person &p) {
        os << p.age << " " << p.name << endl;
        return os;
    }
};

int main() {
    // Before C++17 you must write the types: Pair<int, string>.
    // From C++17 the compiler can guess them:
    // g++ -std=c++17 main.cpp -o app
    // Pair p(2, "abc"); 
    
    Pair <int, string> p1(2, "abc");
    p1.print();

    Person p(4, "Josep");
    Pair <int, Person> p2(5, p);   // works with any type that has <<
    p2.print();

    return 0;
}

#else
// ---------- 4. Template specialization ----------

// General version: works for int, double, string...
// Don't use auto here: for two pointers, a + b is invalid,
// and the compiler fails before it reaches the specialization.
template <typename T>
T sum(T a, T b) {
    return a + b;
}

// Special version for const char*, since pointer + pointer is not allowed.
template <>
const char* sum<const char*>(const char* s1, const char* s2) {
    int len1 = strlen(s1),
        len2 = strlen(s2),
        len = len1 + len2 + 1;
    char *res = new char[len];
    strcpy(res, s1);
    strcpy(res + len1, s2);             
    return res;
}

int main() {
    cout << sum(4, 5) << endl;
    cout << sum("abc", "def") << endl;  // T = const char*, special version
    return 0;
}

#endif

// g++ main.cpp -o app
// g++ -std=c++17 main.cpp -o app