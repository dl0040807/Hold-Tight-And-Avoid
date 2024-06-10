#ifndef ITEM_HPP
#define ITEM_HPP

#include "user.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

using namespace std;



class Item
{

public:
    bool existing;
    Vector2 position;
    Color mycolor;
    int function;
    int sizing;
    Texture2D iimage;

    Item()
    {
        existing = false;
        position = {0,0};
        mycolor = PURPLE;
        function = 0;
        sizing =15;
        iimage.width =50;
        iimage.height =50;
    }
    Item makeItem()
    {
        Item newItem = Item();
        newItem.existing = true;
        newItem.position = {(float)GetRandomValue(80, 700), (float)GetRandomValue(40,350)};
        newItem.function = GetRandomValue(1,2);
        return newItem;
    }
    
    void DrawItem(Item items){
        DrawTexture(iimage, items.position.x, items.position.y, WHITE);
    }
    Rectangle returnBorder(){
        return {position.x + iimage.width/4, position.y + iimage.height/4, (float)iimage.width/4, (float)iimage.height/4};
    }
};
class ItemStack{
    private:
    void increaseCapacityItem()
    {
        if (top == capacity)
        {
            Item *newArray = new Item[capacity * 2];
            for (int i = 0; i < capacity; i++)
            {
                newArray[i] = itemlist[i];
            }
            capacity = 2 * capacity;
            delete[] itemlist;
            itemlist = newArray;
        }
    }

    public: 
    int capacity;
    int top;
    Item* itemlist;

    ItemStack(){
        capacity = 10;
        top = 0;
        itemlist = new Item[capacity];
    }
    ~ItemStack(){
        delete [] itemlist;
        itemlist = nullptr;
    }
    void useItem()
    {
        if(top!=-0)
        {
            Item usedItem = itemlist[top];
            top--;
        }

    }
    Item addItem(Item iTem){
        increaseCapacityItem();
        itemlist[top] = iTem;
        top++;
        return iTem;
    }
    void exchange(int index){
        if(index <= top-1){
            Item temp = itemlist[index];
            itemlist[index] = itemlist[top-1];
            itemlist[top-1] = temp;
        }
    }
};
#endif