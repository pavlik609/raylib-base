#pragma once

#include <iostream>
#include <unordered_map>
#include <raylib/raylib.h>

#include "OBJECT.hpp"

using namespace std;
namespace rayaddon_shared {

    #define WIND_W 500
    #define WIND_H 500

    #define DEBUG_MODE true

    unordered_map<char*, Texture2D> textures;

    Texture2D GetTexture(char* path){
        Texture2D localTexture;
        if (textures.find(path) == textures.end()){
            localTexture = LoadTexture(path);
            cout << "texurre alreaydy  loaded " << endl;
            textures[path] = localTexture;
        }else{
            localTexture = textures.find(path)->second;
        }
        return localTexture;
    }
    float CAM_X = 0;
    float CAM_Y = 0;

void DrawTextureCenter(char* imgpath,float x,float y,float rotation,float size,Color color){
    Texture2D tex = GetTexture(imgpath);
    if (strcmp(imgpath,::OBJECT::INVALID_NAME) == 0){
        TraceLog(LOG_WARNING,"RAYADDON | TEXTURE OF PATH '%s' NOT FOUND",imgpath);
    }
    float tex_width_calc = ((float)tex.width/(float)tex.height)*size;
    float tex_height_calc = ((float)tex.height/(float)tex.width)*size;
    float x_calc = x-tex_width_calc/2+tex_width_calc/2.0f   -rayaddon_shared::CAM_X+(float)WIND_W/2; 
    float y_calc = y-tex_height_calc/2+tex_height_calc/2.0f  +rayaddon_shared::CAM_Y+(float)WIND_H/2; 
    DrawTexturePro(tex,(Rectangle){0,0,(float)tex.width,(float)tex.height},(Rectangle){x_calc,y_calc,tex_width_calc,tex_height_calc},(Vector2){tex.width/2.0f,tex.height/2.0f},rotation,color);
    }
}
