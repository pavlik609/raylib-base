#include<stdlib.h>

#ifdef __RA_OBJECT_H__

#define __RA_OBJECT_H__

    typedef struct OBJECT OBJECT;
    struct OBJECT{
        char imgpath[50];
        int x,y;
    };

    static OBJECT NULL_OBJ = {.imgpath=NULL,
                              .x=NULL,
                              .y=NULL}; 

#endif
