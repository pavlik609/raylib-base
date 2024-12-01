#pragma once

#include <raylib/raylib.h>

#include "../includes/linkedlist.hpp"
#include "../includes/shared.hpp"
#include "../includes/OBJECT.hpp"

struct ASTEROID : OBJECT::OBJECT {
    float velx,vely,velrot,size,hp,mhp;
    bool splits;
    
    /* Full constructor */
    ASTEROID(char* _imgpath,float _x,float _y,float _rotation,bool _render,Color _color,float _hp,float _size,float _velrot,float _velx, float _vely) : OBJECT::OBJECT((char*)::OBJECT::INVALID_NAME,0,0,true,PURPLE) {
        imgpath = _imgpath;
        x = _x;
        y = _y;
        rotation = _rotation;
        render = _render;
        color = _color;
        hp = _hp;
        mhp = _hp;
        size = _size;
        velx = _velx;
        vely = _vely;
        velrot = _velrot;
    }

    /* Barebones constructor */
    ASTEROID(char* _imgpath) : OBJECT::OBJECT((char*)::OBJECT::INVALID_NAME,0,0,true,PURPLE) {
        imgpath = _imgpath;
        x = 0;
        y = 0;
        rotation = 0.0f;
        render = true;
        color = WHITE;
        hp = 100.0f;
        mhp = 100.0f;
        size = 64.0f;
        velx = 0.0f;
        vely = 0.0f;
        velrot = 0.1f;
    }
};

    void UpdateAsteroid(linked_list<ASTEROID>** list,linked_list<ASTEROID>* node,int i){
        ASTEROID* asteroid = &node->value;
        rayaddon_shared::DrawTextureCenter(asteroid->imgpath,asteroid->x,asteroid->y,asteroid->rotation,asteroid->size,asteroid->color);
        (*asteroid).x += asteroid->velx;
        (*asteroid).y -= asteroid->vely;
        (*asteroid).rotation += asteroid->velrot;
        if(asteroid->rotation >= 360) { (*asteroid).rotation -= 360; }
        if(asteroid->rotation <= -360) { (*asteroid).rotation += 360; }
        if (asteroid->hp <= 0.0f){
            pop_n(list,i);
            /* TODO: add splitting here */
        }
        if (DEBUG_MODE && (*asteroid).render){
            DrawText(TextFormat("x : %.2f, y : %.2f, rot : %.2f\nimg : %s",(*asteroid).x,(*asteroid).y,(*asteroid).rotation,(*asteroid).imgpath),(*asteroid).x-40-rayaddon_shared::CAM_X+250,(*asteroid).y+30+rayaddon_shared::CAM_Y+250,20,DARKGRAY);
        }
    }
