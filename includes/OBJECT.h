#include<stdlib.h>
#include<stdbool.h>

#ifndef __RA_OBJECT_H__
#define __RA_OBJECT_H__

    #define INVALID_NAME "/<INVALID?NAME>/"
    typedef struct OBJECT OBJECT;
    struct OBJECT{
        char* imgpath;
        float x,y;
        bool render;
    };

    static inline void debug_obj(OBJECT obj){
        printf("DEBUG OBJ X: %i Y: %i RENDER: %i IMAGEPATH: %s",obj.x,obj.y,obj.render,obj.imgpath);
    }

#endif
