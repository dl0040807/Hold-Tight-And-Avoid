/***********************************
Team: Yehwan Lee, Jennifer Lee, Hyeseong Oh
Team Code: YL7HO3JL9
 */
#include "raylib.h"
#include "raymath.h"
#include "obstacle.hpp"
#include "ArrayList.hpp"
#include "user.hpp"
#include "item.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
using namespace std;


void UpdateDrawFrame(ArrayList &array, User &userone, ItemStack &utem, ItemStack &ctem, Texture2D *texting, Texture2D obs, Texture2D ite);
void AddObstacle(ArrayList &array, Vector2 position, float time);
bool processCollision(ArrayList &array, User &user);
int processCollisionItem(User &user, ItemStack &iTem);
Item usingItem(ItemStack &iTem, User &user);
void difficulty(ArrayList &array, float time);
Vector2 GetPositioning();


#define random_angle 30 * DEG2RAD
#define delay 0.25f

const int screenWidth = 800;
const int screenHeight = 450;
const Vector2 screenCenter = {screenWidth / 2, screenHeight / 2};
static float lastObstacleCreationTime = -1.0f;

int main(void)
{
    SetTargetFPS(200);

    InitWindow(screenWidth, screenHeight, "Major Project");

    ArrayList arr_obstacle = ArrayList();
    ItemStack usingitem = ItemStack();
    ItemStack creatingitem = ItemStack();
    User main_user = User(screenCenter, 2, {0, 0});
    main_user.timage.width = 120;
    main_user.timage.height = 75;
    Texture2D obstacletexture = LoadTexture("Imagesource/Ball.png");
    Texture2D itemtexture = LoadTexture("Imagesource/Item.png");
    Texture2D* textures = new Texture2D[3];
    textures[0] = LoadTexture("Imagesource/Normal.png");
    textures[1] = LoadTexture("Imagesource/Angry.png");
    textures[2] = LoadTexture("Imagesource/happy.png");

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
       
        UpdateDrawFrame(arr_obstacle, main_user, usingitem, creatingitem, textures, obstacletexture, itemtexture);
    }

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(ArrayList &array, User &userone, ItemStack &utem, ItemStack &ctem, Texture2D *texting, Texture2D obs, Texture2D ite)
{
    static float capacityIncreaseTimer = 0.0f;
    static float ItemTimer = 0.0f;
    static float ItemusingTimer = 0.0f;
    float frametime = GetFrameTime();
    float time = GetTime();
    float timer = GetTime();
    bool isGameOver = false;
    //Checking user life
    if (userone.life == 0)
    {
        isGameOver = true;
    }
    //Checking hittimer 
    if (userone.hitTimer > 0.0f)
    {
        userone.hitTimer -= frametime;
    }
    capacityIncreaseTimer += frametime; // increasing capacity timer
    ItemTimer += frametime; // increasing itemtimer
    //updating position of obstacle on the obstacle arraylist
    for (int i = 0; i < array.capacity; i++)
    {
        if (!array.elements[i].active)
        {
            continue;
        }
        array.elements[i].ObstacleUpdate2(frametime, time);
    }
    //Increase capacity of obstacle arraylist every 10 seconds
    if (capacityIncreaseTimer > 10.0f)
    {
        array.increaseCapacity();
        capacityIncreaseTimer = 0.0f;
    }
    //making Item every 10 seconds; 4 max
    if (ItemTimer > 10.0f && ctem.top < 4)
    {
        ctem.itemlist[ctem.top] = ctem.itemlist[ctem.top].makeItem();
        ctem.top++;
        ItemTimer = 0.0f;
    }
    //getting the last obstacle creation time
    if (time > lastObstacleCreationTime + delay)
    {
        AddObstacle(array, GetPositioning(), timer);
        lastObstacleCreationTime = time;
    }
    //If not game over
    if (!isGameOver)
    {
        //change the color of user when it detects collision between user and obstacle
        if (!processCollision(array, userone))
        {
            userone.color = WHITE;
        }
        else
        {
            userone.color = RED;
            if (userone.hitTimer <= 0.0f)
            {
                userone.life--;
                std::cout << "Remaining life: " << userone.life << endl;
                userone.hitTimer = 1.0f;
            }
        }
        //check the collision between user and item and put the item on the stack
        if (processCollisionItem(userone, ctem) != -1)
        {
            utem.addItem(ctem.itemlist[processCollisionItem(userone, ctem)]);
            ctem.exchange(processCollisionItem(userone, ctem));
            ctem.top--;
        }
        //Using item
        static int textitem = 0;
        if (IsKeyPressed(KEY_SPACE))
        {
            Item check = usingItem(utem, userone);
            if (check.function == 1)
            {
                ItemusingTimer = 5.0f;
                userone.life++;
                textitem = 1;
                userone.timage = texting[2];
                userone.timage.width = 120;
                userone.timage.height = 105;
            }
            if (check.function == 2)
            {
                ItemusingTimer = 5.0f;
                userone.velocity = 5;
                textitem = 2;
                userone.timage = texting[1];
                userone.timage.width = 100;
                userone.timage.height = 70;
            }
            cout << "Textitem:" << textitem << endl;
        }
        //Going back to normal velocity after using item
        if (ItemusingTimer > 0.0f)
        {
            ItemusingTimer -= frametime;
            if (ItemusingTimer <= 0.0f)
            {
                userone.velocity = 2;
                userone.timage = texting[0];
                userone.timage.width = 120;
                userone.timage.height = 75;
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        difficulty(array, timer);
        userone.UserUpdate();
        string lifeMsg = "LIFE: " + to_string(userone.life);
        string whatItem = "";
        if (textitem == 1)
        {
            whatItem = "More HP!!";
        }
        else if (textitem == 2)
        {
            whatItem = "Speed Up!!";
        }
        string Itemleft = "ITEM: " + to_string(utem.top);
        string timeMsg = to_string(time);
        string instruction = "Use Arrow To Move";
        string instructionTwo = "Use Spacebar To Use Item";
        DrawText(lifeMsg.c_str(), 10, 20, 20, BLACK);
        DrawText(Itemleft.c_str(), 10, 50, 20, BLACK);
        DrawText(whatItem.c_str(), 10, 75, 10, RED);
        DrawText(timeMsg.c_str(), 300, 200, 50, LIGHTGRAY);
        DrawText(instruction.c_str(), 340, 260, 15, LIGHTGRAY);
        DrawText(instructionTwo.c_str(), 320, 280, 15, LIGHTGRAY);
        userone.UserDraw();
        for (int i = 0; i < array.capacity; i++)
        {
            array.elements[i].bimage = obs;
            array.elements[i].bimage.width =50;
            array.elements[i].bimage.height =50;
            array.elements[i].ObstacleDraw();
        }
        for (int i = 0; i < ctem.top; i++)
        {
            ctem.itemlist[i].iimage = ite;
            ctem.itemlist[i].iimage.height = 50;
            ctem.itemlist[i].iimage.width = 50;
            ctem.itemlist[i].DrawItem(ctem.itemlist[i]);
        }
        EndDrawing();
    }
    else
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        string message = "GAME OVER";
        DrawText(message.c_str(), 100, 100, 40, DARKGRAY);
        EndDrawing();
    }
}
// add obstacle to the obstacle arraylist
void AddObstacle(ArrayList &array, Vector2 position, float time)
{
    bool created = false;
    Vector2 velocity = Vector2Subtract(screenCenter, position);
    velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(array.elements[0].velocity_min, array.elements[0].velocity_max));
    velocity = Vector2Rotate(velocity, (float)GetRandomValue(-random_angle, random_angle));
    for (int i = 0; i < array.capacity; i++)
    {
        if (array.elements[i].active)
        {
            continue;
        }
        array.set(Obstacle(position, velocity), i);
        created = true;
        break;
    }
}
//check if the collision of obstacles and user is detected
bool processCollision(ArrayList &array, User &user)
{
    for (int i = 0; i < array.capacity; i++)
    {   
        //At the very first, I altered the border 
        //through adding timage.width & timage.height. But I ran game
        //several times and decided to set the fixed number to create a border
        Rectangle border = {
            user.position.x + 30,
            user.position.y + 20,
            (float)40,
            (float)40,
        };
        if (CheckCollisionRecs(array.elements[i].returnBorder(), border))
        {
            return true;
        }
    }
    return false;
}
//check the collision between item and user
int processCollisionItem(User &user, ItemStack &iTem)
{
    for (int i = 0; i < iTem.top; i++)
    {
        Rectangle border = {
            user.position.x + 30,
            user.position.y + 20,
            (float)40,
            (float)40,
        };
        if (CheckCollisionRecs(iTem.itemlist[i].returnBorder(), border))
        {
            return i;
        }
    }
    return -1;
}
//Function when the user uses the item
Item usingItem(ItemStack &iTem, User &user)
{
    if (iTem.top != 0)
    {
        Item used = iTem.itemlist[iTem.top - 1];
        iTem.top--;
        return used;
    }
    else
    {
        Item nullitem = Item();
        return nullitem;
    }
}
//setting the difficulty level based on time
void difficulty(ArrayList &array, float time)
{
    if (time > 15)
    {
        array.elements[0].velocity_max = 250;
    }
    if (time > 45)
    {
        array.elements[0].velocity_max = 350;
        array.elements[0].velocity_min = 200;
    }
    if (time > 60)
    {
        array.elements[0].velocity_max = 400;
    }
    if (time > 75)
    {
        array.elements[0].velocity_min = 300;
    }
}
//Getting random position
Vector2 GetPositioning()
{
    int padding = 128;
    Vector2 result = {-128, -128};

    if (GetRandomValue(0, 3))
    {
        if (GetRandomValue(0, 3))
        {
            if (GetRandomValue(0, 3) > 1)
            {
                result.y = screenHeight + padding;
            }
            result.x = GetRandomValue(-padding, screenWidth + padding);
        }
        else
        {
            if (GetRandomValue(0, 3) <= 1)
            {
                result.x = screenWidth + padding;
            }
            result.y = GetRandomValue(-padding, screenHeight + padding);
        }
    }
    return result;
}