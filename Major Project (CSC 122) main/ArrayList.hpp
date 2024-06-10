#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

using namespace std;


#include "obstacle.hpp"
#include <iostream>

class ArrayList{
    public: 
    int capacity;
    Obstacle* elements;
    int size;

    ArrayList() {
        capacity = 5;
        elements = new Obstacle[capacity];
        size = 0;
    }    
    ~ArrayList() {
        delete[] elements;
        elements = nullptr;
    }

    void increaseCapacity() {
        Obstacle* newArray = new Obstacle[capacity + 2];
        for (int i = 0; i < capacity; i++) {
            newArray[i] = elements[i];
        }
        capacity = 2 + capacity;
        delete [] elements;
        elements = newArray;
    }
    void insert (Obstacle value, int index) {
        increaseCapacity();
        for (int i = size; i > index; i--) {
            elements [i] = elements [i-1];
        }
        elements[index] = value;
        size++;
    }
    void prepend (Obstacle value) {
        insert(value, 0);
    }
    void append (Obstacle value) {
        insert(value,capacity);
    }
    void remove (int index) {
        if (size != 0) {
            Obstacle previous = elements[index];
            for (int i = index; i < size; i++) {
                elements [i] = elements[i+1];
            }
            size--;
        }
    }
    void removeFirst() {
        remove(0);
    }
    void removeLast() {
        remove(size-1);
    }
    int getSize() {
        return size;
    }
    Obstacle get(int index) {
        return elements[index];
    }
    void set (Obstacle value, int index) {
        elements[index] = value;
    }
    bool isEmpty() {
        if (size == 0) {
            return true;
        }
        else {
            return false;
        }
    }
};
#endif 