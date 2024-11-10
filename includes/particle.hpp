#pragma once
#include <raylib/raylib.h>

#include "shared.hpp"
#include "OBJECT.hpp"
#include "linkedlist.hpp"

namespace rayaddon_particle {

typedef struct particle particle;
struct particle : OBJECT::OBJECT{
    float size;
    float sizediff_persec;
    float velx,vely;

    particle(char* _imgpath,int _x,int _y,bool _render,Color _color,float _size,float _sizediff_persec,float _velx, float _vely) : OBJECT::OBJECT((char*)::OBJECT::INVALID_NAME,0,0,true,PURPLE) {
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
int particle_count = 0;

    void DrawParticle(particle particle){
    if (particle.render == false) { return; };
        if (strcmp(particle.imgpath,::OBJECT::INVALID_NAME) == 0){
            DrawCircle(particle.x+rayaddon_shared::CAM_X, particle.y+rayaddon_shared::CAM_Y, particle.size, particle.color);
        }else{
            Texture2D tex = rayaddon_shared::GetTexture(particle.imgpath);
            float tex_width_calc = ((float)tex.width/(float)tex.height)*particle.size;
            float tex_height_calc = ((float)tex.height/(float)tex.width)*particle.size;
            float x_calc = particle.x-tex_width_calc/2+tex.width/2.0f  -rayaddon_shared::CAM_X+(float)WIND_W/2*-1; 
            float y_calc = particle.y-tex_height_calc/2+tex.height/2.0f  +rayaddon_shared::CAM_Y+(float)WIND_H/2;
            DrawTexturePro(tex,(Rectangle){0,0,(float)tex.width,(float)tex.height},(Rectangle){x_calc,y_calc,tex_width_calc,tex_height_calc},(Vector2){tex.width/2.0f,tex.height/2.0f},0.0f,particle.color);
        }
    }

    void UpdateParticle(linked_list<particle>** list,linked_list<particle>* node,int i){
        particle_count++;
        particle* particle = &node->value;
        DrawParticle(*particle);
        (*particle).x += particle->velx;
        (*particle).y -= particle->vely;
        (*particle).size -= particle->sizediff_persec;
        if (particle->size <= 0.0f){
            pop_n(list,i);
        }
    }
}
