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
#include "includes_game/asteroid.hpp"

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

    linked_list<particle>* particles = NULL;
    linked_list<ASTEROID>* asteroids = NULL;

    int numbg_lines = 10;

    push(&asteroids,ASTEROID((char*)"textures/dev.png"));
    (*asteroids).value.velx = 0.1f;
    (*asteroids).value.vely = 0.1f;

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
        if (framecount % 5 == 0){
            //push(&particles,*(new particle((char*)INVALID_NAME,m_pos.x,m_pos.y,true,GRAY,5.0f,0.01f,RandFloat(1)-0.5f,RandFloat(0.5f))));
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

            pass(&particles,&UpdateParticle);
            pass(&asteroids,&UpdateAsteroid);
            //thread particle_upd(pass<particle>,&particles,&UpdateParticle);
            //particle_upd.join();
            
            if(DEBUG_MODE){
                DrawText("DEBUG MODE",2,2,20,RED);
                DrawFPS(2,20);
                DrawText(TextFormat("particle_count : %i",particle_count),2,40,20,LIGHTGRAY);
                DrawText(TextFormat("x : %f | y : %f",CAM_X,CAM_Y),2,60,20,LIGHTGRAY);
            }
        EndDrawing();
        framecount++;
    }

    CloseWindow();
    
    return 0;
}
