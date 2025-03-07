#ifndef LIST
#define LIST
#include <iostream>
#include "error.cpp"

using namespace std;

template <typename T>
class List
{
private:
    T *data;      // Dynamic array
    int capacity; // Current allocated size
    int size;     // Number of elements

    void resize(); // Function to resize list

public:
    List(int cap = 100); // Constructor
    ~List();            // Destructor

    void push(T value); // Add element
    void pop();         // Remove last element

    T &operator[](int index);             // Overload [] operator for access
    const T &operator[](int index) const; // Read-only version

    int getSize() const; // Get current size

    bool empty() const { return size == 0; } // Check if list is empty
    bool contains(T value) const;            // Check if list contains element
};

// Constructor
template <typename T>
List<T>::List(int cap) : capacity(cap), size(0)
{
    data = new T[capacity];
}

// Destructor
template <typename T>
List<T>::~List()
{
    delete[] data;
}

// Resize function (doubles capacity)
template <typename T>
void List<T>::resize()
{
    capacity *= 2;
    T *newData = new T[capacity];
    for (int i = 0; i < size; i++)
    {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

// Add element to the list
template <typename T>
void List<T>::push(T value)
{
    if (size == capacity)
        resize();
    data[size++] = value;
}

// Remove last element
template <typename T>
void List<T>::pop()
{
    if (size > 0)
        size--;
}

// Overload [] for accessing elements
template <typename T>
T &List<T>::operator[](int index)
{
    if (index < 0 || index >= size)
    {
        throw Error("Index out of range");
    }
    return data[index];
}

// Overloaded [] for read-only access (const version)
template <typename T>
const T &List<T>::operator[](int index) const
{
    if (index < 0 || index >= size)
    {
        throw Error("Index out of range");
    }
    return data[index];
}

// Get the current number of elements
template <typename T>
int List<T>::getSize() const
{
    return size;
}

template <typename T>
bool List<T>::contains(T value) const
{
    for (int i = 0; i < size; i++)
    {
        if (data[i] == value)
        {
            return true;
        }
    }
    return false;
}
#endif