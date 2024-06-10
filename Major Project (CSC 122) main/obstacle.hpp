#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "user.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

using namespace std;


#define screenW 800
#define screenH 450
//#define velocity_min 100
//#define velocity_max 100 

class Obstacle{
    public:
    bool active;
    Vector2 position;
    Vector2 velocity;
    Color myColor;
    int sizing;
    float creationTime;
    int velocity_min;
    int velocity_max;
    float timeadded;
    Texture2D bimage;

    Obstacle(){
        active = false;
        position = {0,0};
        velocity = {0,0};
        myColor = BLACK;
        sizing = 15;
        velocity_min = 100;
        velocity_max = 100;
        timeadded = 10.0f;
    }


    Obstacle(Vector2 set_position, Vector2 set_velocity){
        active = true;
        position = set_position;
        velocity = set_velocity;
        myColor = BLACK;
        sizing = 15;
        creationTime = GetTime();
        velocity_min = 100;
        velocity_max = 100;
        timeadded = 10.0f;
    }

    void ObstacleUpdate2(float frametime, float time){
        if(time > creationTime + timeadded){   
            active = false;
            return;
        }
        position = Vector2Add(position, Vector2Scale(velocity, frametime));
    }
    void ObstacleDraw(){
        if(!active)
        {
            return;
        }
        DrawTexture(bimage, position.x, position.y, WHITE);
    }
    Rectangle returnBorder(){
        return {position.x + bimage.width/4, position.y + bimage.height/4, (float)bimage.width/4, (float)bimage.height/4};
    }
};
#endif