#ifndef USER_HPP
#define USER_HPP

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

#define rotation_speed 360

using namespace std;

class User{
    public:
    Vector2 position;
    int velocity;
    Vector2 direction;
    Color color;
    int life;
    float hitTimer;
    Texture2D timage;


    User(){
        position = {0,0};
        velocity = 0;
        direction = Vector2Zero();
        color = WHITE;
        life = 3;
        hitTimer = 0.0f;
        timage = LoadTexture("Imagesource/Normal.png");

    }
    User(Vector2 set_position, int set_velocity, Vector2 set_direction){
        position = set_position;
        velocity = set_velocity;
        direction = set_direction;
        color = WHITE;
        life = 3;
        hitTimer = 0.0f;
        timage = LoadTexture("Imagesource/Normal.png");
    }
    void UserDraw(){
        DrawTexture(timage, position.x, position.y, color);
    }
    void setDirection(Vector2 newdirection){
        direction = newdirection;
    }
    void UserUpdate(){
        direction = Vector2Zero();

        if(IsKeyDown(KEY_UP)){
            direction.y -= 1;
        }
        if(IsKeyDown(KEY_DOWN)){
            direction.y += 1;
        }
        if(IsKeyDown(KEY_RIGHT)){
            direction.x+= 1;
        }
        if(IsKeyDown(KEY_LEFT)){
            direction.x-= 1;
        }

    Vector2 newPosition = Vector2Add(position, Vector2Scale(direction, velocity/2));
    //  if (newPosition.x >= 0 && newPosition.x <= 800  && newPosition.y >= 0  && newPosition.y <= 450) {
    //     position = newPosition;
    // } This is the command that I used for drawing circle
     if (newPosition.x + 35 > 0 && newPosition.x + 60 < 800  && newPosition.y + 20 > 0  && newPosition.y + 50 < 450) {
        position = newPosition;
    } // I adjusted after I load the texture
    } 
};
#endif