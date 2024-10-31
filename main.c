#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<raylib/raylib.h>

#include "includes/collections.h"
#include "includes/OBJECT.h"

#define WIND_W 500
#define WIND_H 500

typedef struct particle particle;
struct particle{
    OBJECT obj;
    float size;
    float sizediff_persec;
    Color color;
    float velx,vely;
}; 

extern void debug_obj(OBJECT obj);

int main(){
    srand(time(NULL));
    InitWindow(WIND_W,WIND_H,"Hello Raylib!");

    SetTargetFPS(240);

    void l_list_debug_particle(particle t){
        printf("DEBUG LINKED LIST PARTICLE SIZE : %i\n",t.size);
        debug_obj(t.obj);
    }

    L_LIST(particle);

    int particlecounter = 0;

    l_list_particle* particles = malloc(sizeof(particle));
    *particles = (l_list_particle){.x=(particle){.obj=
                                       (OBJECT){.imgpath = INVALID_NAME,
                                        .x = (float)250,
                                        .y = (float)250 
                                       },
                                .size=5,
                                .color=GRAY,
                                .velx = 0,
                                .vely =0.05f,
                                .sizediff_persec = 0.01f
                                }, .next = NULL};
    void UpdateGenericParticle(particle* particle,int i){
        particlecounter++;
        (*particle).obj.x += particle->velx;
        (*particle).obj.y += particle->vely;
        (*particle).size -= particle->sizediff_persec;
        if(strcmp(particle->obj.imgpath,INVALID_NAME) == 0){
            DrawCircle(round(particle->obj.x),round(particle->obj.y), particle->size, particle->color);
        }else{
            printf("DRAWING TEXTURES IS NOT SUPPORTED YET, filepath: %s\n",particle->obj.imgpath); 
        }
        if (particle->size < -0.5f){
            l_list_pop_n_particle(&particles,0);
        }
    }

    bool mbL_down = false;
    Vector2 mousepos = (Vector2){0,0};

    void SpawnGenericParticle(int x,int y,float velx,float vely,float sizediff_persec,float size,char* imgpath,Color color){
        l_list_push_particle(&particles,(particle){.obj=
                                       (OBJECT){.imgpath = imgpath,
                                        .x = (float)x,
                                        .y = (float)y 
                                       },
                                .size=size,
                                .color=color,
                                .velx = velx,
                                .vely = vely,
                                .sizediff_persec = sizediff_persec
                                });
    }

            //SpawnGenericParticle(50,50,0,0.1f,0.05f,5,INVALID_NAME,GRAY);
            //l_list_pop_n_particle(particles,0);
    while(!WindowShouldClose()){
        particlecounter = 0;
        mbL_down = IsMouseButtonDown(0);
        mousepos = GetMousePosition();
        if (mbL_down){
            SpawnGenericParticle(mousepos.x,mousepos.y,(float)(rand() % (101))/100-0.5f,(float)(rand() % (101))/100-1,0.05f,5,INVALID_NAME,GRAY);
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(20,20);
            DrawText("Hello Raylib!",190,200,20,LIGHTGRAY);
            l_list_pass_particle(particles,&UpdateGenericParticle);

            DrawText(TextFormat("particles_length : %i",particlecounter),20,50,20,LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
