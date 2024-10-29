#include<stdio.h>
#include<stdlib.h>
#include<raylib/raylib.h>

#include "includes/collections.h"


int main(){
    InitWindow(500,500,"Hello Raylib!");

    void l_list_debug_int(int t){
        printf("DEBUG LINKED LIST INT : %i\n",t);
    }

    L_LIST(int);

    l_list_int node1 = {.x = 5, .next = NULL};
    l_list_int* head = &node1;
    l_list_push_int(head,78);
    l_list_push_int(head,99);

    l_list_read_int(head);
    while(!WindowShouldClose()){

        BeginDrawing();
            
            ClearBackground(RAYWHITE);

            DrawText("Hello Raylib!",190,200,20,LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
