#include<stdio.h>
#include <cstdlib>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include <raylib/raylib.h>
#include <thread>
#include <random>

#include "includes/OBJECT.hpp"
#include "includes/linkedlist.hpp"
#include "includes/particle.hpp"
#include "includes/shared.hpp"
#include "includes/quadtree.hpp"

using namespace std;
using namespace rayaddon_particle;
using namespace rayaddon_shared;
using namespace OBJECT;

std::random_device rnd;
std::mt19937 rng(rnd());
std::uniform_int_distribution<std::mt19937::result_type> dist(1,100000); //why is this so weird
 
float RandFloat(float max){ 
    return (dist(rng)/100000.0f)*max;
}

int framecount = 0;

int main(){
//    srand(time(NULL));
    InitWindow(WIND_W,WIND_H,"Hello Raylib!");

    Vector2 m_pos;
    
    SetTargetFPS(240);

    int numbg_lines = 10;

    AABB b1 = AABB((Vector2){-100,100},(Vector2){100,-100});
    AABB b2 = AABB((Vector2){-150,150},(Vector2){50,-50});

    Vector2 p1 = (Vector2){-99,-99};

    int holding = -1;

    while(!WindowShouldClose()){
        particle_count = 0;
        if (IsKeyDown(KEY_D)){
            CAM_X += 1.0f;
        }if (IsKeyDown(KEY_A)){
            CAM_X -= 1.0f;
        }if (IsKeyDown(KEY_W)){
            CAM_Y += 1.0f;
        }if (IsKeyDown(KEY_S)){
            CAM_Y -= 1.0f;
        }
        m_pos = GetMousePosition();
        m_pos.x -= CAM_X;
        m_pos.y -= CAM_Y;
        if (IsKeyPressed(KEY_K)){
            holding++;
        }if (IsKeyPressed(KEY_L)){
            holding--;
        }
        if (holding == 0){
            b2.up_left = m_pos;
            b2.up_left.x -= (float)WIND_W/2;
            b2.up_left.y -= (float)WIND_H/2;
        }else if (holding == 1){
            b2.down_right = m_pos;
            b2.down_right.x -= (float)WIND_W/2;
            b2.down_right.y -= (float)WIND_H/2;
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            float spacing_w = (float)WIND_W/numbg_lines;
            float spacing_h = (float)WIND_H/numbg_lines;
            for(int i=-1;i<numbg_lines+1;i++){
                float endx = i*spacing_w - fmod((float)CAM_X, spacing_w);
                float endy = i*spacing_h + fmod((float)CAM_Y, spacing_h);
                DrawLineEx((Vector2){endx,0.0f},(Vector2){endx,WIND_H},2.0f,LIGHTGRAY);
                DrawLineEx((Vector2){0.0f,endy},(Vector2){WIND_H,endy},2.0f,LIGHTGRAY);
            }
            cout << " INSIDE : " << b1.inside(p1) << " AABB INTERSECT : " << (b1.intersects(b2) || b2.intersects(b1)) << endl;// ADD 250 to each
            float width = b1.down_right.x-b1.up_left.x;
            float height = b1.up_left.y-b1.down_right.y;
            DrawRectangleLines(b1.up_left.x+(float)WIND_W/2, b1.up_left.y+(float)WIND_H/2-height, width, height, BLACK);
            float width2 = b2.down_right.x-b2.up_left.x;
            float height2 = b2.up_left.y-b2.down_right.y;
            DrawRectangleLines(b2.up_left.x+(float)WIND_W/2, b2.up_left.y+(float)WIND_H/2-height2, width2, height2, BLUE);
            DrawCircle(p1.x+(float)WIND_W/2,p1.y+(float)WIND_H/2,2,GREEN);        
            if(DEBUG_MODE){
                DrawText("DEBUG MODE",2,2,20,RED);
                DrawFPS(2,20);
                DrawText(TextFormat("particle_count : %i",particle_count),2,40,20,LIGHTGRAY);
                DrawText(TextFormat("x : %f | y : %f",CAM_X,CAM_Y),2,60,20,LIGHTGRAY);
                DrawText(TextFormat("holding : %i",holding),2,80,20,LIGHTGRAY);
            }
        EndDrawing();
        framecount++;
    }

    CloseWindow();
    
    return 0;
}
