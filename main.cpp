#include<stdio.h>
#include <cstdlib>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include <raylib/raylib.h>
#include <unordered_map>

#include "includes/OBJECT.hpp"
#include "includes/linkedlist.hpp"

using namespace std;

#define WIND_W 500
#define WIND_H 500

const char* INVALID_NAME =  "/<INVALID?NAME>/";

typedef struct particle particle;
struct particle : OBJECT::OBJECT{
    float size;
    float sizediff_persec;
    float velx,vely;

    particle(char* _imgpath,int _x,int _y,bool _render,Color _color,float _size,float _sizediff_persec,float _velx, float _vely) : OBJECT::OBJECT((char*)INVALID_NAME,0,0,true,PURPLE) {
        imgpath = _imgpath;
        x = _x;
        y = _y;
        render = _render;
        color = _color;
        size = _size;
        sizediff_persec = _sizediff_persec;
        velx = _velx;
        vely = _vely;
    }
}; 

void foo(linked_list<int> node, int i){
    printf("index %i and value %i\n",i,node.value);
}


unordered_map<char*, Texture2D> textures;

Texture2D GetTexture(char* path){
    auto exists = textures.find(path);
    Texture2D localTexture;
    if (memcmp((void*)&localTexture, (void*)&textures.end()->second,sizeof(Texture2D)) == 0){
        localTexture = LoadTexture(path);
        textures[path] = localTexture;
    }
    return localTexture;
}

void DrawParticle(particle particle){
    if (particle.render == false) { return; };
    if (strcmp(particle.imgpath,INVALID_NAME) == 0){
        DrawCircle(particle.x, particle.y, particle.size, particle.color);
    }else{
        DrawTexture(GetTexture(particle.imgpath),particle.x,particle.y,particle.color);
    }
}

int particle_count = 0;

void UpdateParticle(linked_list<particle>** list,linked_list<particle>* node,int i){
    particle_count++;
    particle* particle = &node->value;
    DrawParticle(*particle);
    (*particle).x += particle->velx;
    (*particle).y -= particle->vely;
    (*particle).size -= particle->sizediff_persec;
    if (particle->size <= -0.5f){

        pop_n(list,i);
    }
}

int main(){
//    srand(time(NULL));
    InitWindow(WIND_W,WIND_H,"Hello Raylib!");

    Vector2 m_pos;

    SetTargetFPS(240);

    int draw_mode = 0;

    linked_list<particle>* particles = NULL;
    while(!WindowShouldClose()){
        particle_count = 0;
        if (IsKeyPressed(KEY_W)){
            draw_mode++;
        }
        m_pos = GetMousePosition();
        if (IsMouseButtonDown(0)){
            switch(draw_mode){
                case 0:        
                    push(&particles,*(new particle((char*)INVALID_NAME,m_pos.x,m_pos.y,true,GRAY,5.0f,0.01f,0.0f,0.1f)));
                    break;
                case 1:        
                    push(&particles,*(new particle((char*)INVALID_NAME,m_pos.x,m_pos.y,true,GRAY,5.0f,0.01f,0.0f,0.0f)));
                    break;
                default:
                    push(&particles,*(new particle((char*)INVALID_NAME,m_pos.x,m_pos.y,true,GRAY,5.0f,0.01f,0.0f,0.1f)));
            }
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(20,20);
            DrawText("Hello Raylib!",190,200,20,LIGHTGRAY);
            particles->pass(&particles,&UpdateParticle);
            DrawText(TextFormat("particle_count : %i",particle_count),20,40,20,LIGHTGRAY);
            DrawText(TextFormat("draw_mode : %i",draw_mode),20,60,20,LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
/*     std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); // distribution in range [1, 6]

    std::cout << dist6(rng) << std::endl;*/
