#include <iostream>
using namespace std;

/*
Initialization
1. Default Constructor                    Vector<int> v1;
2. Parameterized Constructor              Vector<int> v2(6);
3. Constructor with Initializer List      Vector<int> v3 = {1, 2, 3, 4};
4. Copy Constructor                       Vector<int> v4 = v3;
5. Copy Assignment Operator               Vector<int> v5 = {1, 2, 3, 4};
                                          v5 = v4;
6. Move Copy Constructor                       Vector<int> v6 = move(v5);
7. Move Assignment Operator               Vector<int> v7 = {1, 2, 3, 4};
                                          v7 = move(v6);
8. Iterator Initialization                Vector<int> v8(v7.begin(), v7.end());

Functionality
1. Push Back    v.push_back(1);
2. Pop Back     int res = v.pop_back();
3. Resize       v.resize(20);
4. Capacity     v.capacity();
5. Size         v.size();
6. Index        v[index] = 1; int res = v[index];
7. Erase        v.erase(1);
8. Is Empty     v.empty();
9. Iterator     Vector<int>::Iterator it = v.begin();
*/

template <typename T>
class Vector {
private:
    T* data;
    int size;
    int capacity;

public:
    // Default Constructor
    Vector(): data(nullptr), size(0), capacity(0) {}

    // Parameterized Constructor
    Vector(int initialCapacity): data(new T[initialCapacity]), capacity(initialCapacity), size(0) {}

    // Constructor with Initializer List
    Vector(initializer_list <T> list): Vector(list.size()) {
        for(auto &itr: list) {
            push_back(itr);
        }
    }

    // Copy Constructor
    Vector(const Vector &src) {
        size = src.size;
        data = src.capacity;

        data = new T[capacity];
        for(int i = 0; i < size; i++) {
            data[i] = src.data[i];
        }
    }

    // Copy Assignment Operator

    // Pass src by value to create a copy using the copy constructor.
    // Swapping with this copy leaves the original source object unchanged.
    // When src is destroyed, it releases the old resources of *this.
    Vector operator&=(Vector src) {
        // CAS: Compare and Swap Idioms
        std::swap(data, src.data);
        std::swap(size, src.size);
        std::swap(capacity, src.capacity);

        return *this;
    }

    /*
    Vector& operator=(const Vector &src) {
        if(this != &src) {
            size = src.size;
            capacity = src.capacity;

            delete[] data;
            data = new T[capacity];
            for(int i = 0; i < size; i++) {
                data[i] = src.data[i];
            }
        }

        return *this;
    }
    */

    // Move Copy Constructor
    Vector(Vector &&src) {
        size = src.size;
        capacity = src.capacity;
        data = src.data;

        src.size = 0;
        src.capacity = 0;
        src.data = nullptr;
    }

    // Move Assignment Operator
    Vector& operator=(Vector &&src) {
        if(this != &src) {
            delete[] data;

            size = src.size;
            capacity = src.capacity;
            data = src.data;

            src.size = 0;
            src.capacity = 0;
            src.data = nullptr;
        }
        return *this;
    }


    void push_back(T element) {
        if(capacity <= size) {
            reserve(capacity == 0 ? 1 : 2 * capacity);
        }
        data[size++] = element;
    }

    void reserve(int newCapacity) {
        if(newCapacity > capacity) {
            T *temp = new T[newCapacity];
            for(int i = 0; i < size; i++) {
                temp[i] = std::move(data[i]);
            }
            delete[] data;
            data = temp;
            capacity = newCapacity;
        }
    }


    class iterator {
    private:
        T *element;
    public:
        iterator(T *ptr): element(ptr) {}
    };

    iterator begin() {
        return iterator(data);
    }

    iterator end() {
        return iterator(data + size);
    }
};

int main() {
    return 0;
}