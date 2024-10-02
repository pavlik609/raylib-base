#include <stdio.h>
#include <raylib/raylib.h>

int main(){
    InitWindow(500,500,"Hello Raylib!");

    while(!WindowShouldClose()){
        BeginDrawing();
            
            ClearBackground(RAYWHITE);

            DrawText("Hello Raylib!",190,200,20,LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
