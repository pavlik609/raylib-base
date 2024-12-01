#pragma once

#include <iostream>
#include<stdbool.h>
#include <raylib/raylib.h>

namespace OBJECT
{
    struct OBJECT{
        char* imgpath;
        float x,y,rotation;
        bool render;
        Color color;

        OBJECT(char* _imgpath,int _x,int _y,bool _render,Color _color){
            imgpath = _imgpath;
            x = _x;
            y = _y;
            render = _render;
            color = _color;
        }
    };

    const char* INVALID_NAME = "/<INVALID?NAME>/";

    inline void debug_obj(OBJECT obj){
        printf("DEBUG OBJ X: %f Y: %f RENDER: %i IMAGEPATH: %s",obj.x,obj.y,obj.render,obj.imgpath);
    }
}
