#include <stdexcept>
#include<stdio.h>
#include <cstdlib>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include <raylib/raylib.h>
#include <thread>
#include <random>
#include <vector>

#include "includes/OBJECT.hpp"
#include "includes/linkedlist.hpp"
#include "includes/particle.hpp"
#include "includes/shared.hpp"
#include "includes/quadtree.hpp"

using namespace std;
using namespace rayaddon_particle;
using namespace rayaddon_shared;
using namespace OBJECT;

std::random_device rnd;
std::mt19937 rng(rnd());
std::uniform_int_distribution<std::mt19937::result_type> dist(1,100000); //why is this so weird
 
float RandFloat(float max){ 
    return (dist(rng)/100000.0f)*max;
}
struct moving_point : Vector2 {
    float velx, vely;
    moving_point(float _x,float _y,float _velx, float _vely) : Vector2({0,0}){
        x = _x;
        y = _y;
        velx = _velx;
        vely = _vely;
    }

    bool operator==(moving_point a){
        if(x == a.x && y == a.y && velx == a.velx && vely == a.vely){
            return true;
        }
        return false;
    }
};
int framecount = 0;
int boxcount = 0;

template<typename T>
void tree_itt_mov(quadtree<T> &node,qtree_value<T>& p){
    /*if(node.values.empty() == true){
        return;
    }*/

    if(node.tree_up_left == nullptr){
        return;
    }
    /* Should ensure that every node is in a leaf */
    place_if_possible(&node,node.tree_up_left,p.data);
    place_if_possible(&node,node.tree_up_right,p.data);
    place_if_possible(&node,node.tree_down_left,p.data);
    place_if_possible(&node,node.tree_down_right,p.data);
}

template<typename T>
void tree_itt_mov_wrap(quadtree<T> &node){
    for (qtree_value<T>& p : node.values){
        tree_itt_mov(node, p);
    }
}

unsigned char randomify_uchar(int in){
    int loc_in = in;
    loc_in ^= (loc_in >> 5);
    loc_in += (loc_in << 3);
    loc_in ^= (loc_in >> 4);
    return (unsigned char) (loc_in);
}

Color colors_diff[15] = {RED,BLUE,GREEN,
                        MAGENTA,ORANGE,YELLOW,
                        MAROON,LIME,SKYBLUE,
                        PURPLE,BROWN,DARKGREEN,
                        DARKBLUE,DARKPURPLE,DARKBROWN};

Color get_different_color(int idx){
    if(idx > 14){
        int mod = idx % 14;
        return {(unsigned char)(colors_diff[idx].r-mod),(unsigned char)(colors_diff[idx].r+mod),(unsigned char)(colors_diff[idx].b-mod*1.5f),255};
    }else{
        return colors_diff[idx];
    }
}

bool overall_change;
template<typename T>
void tree_itt_func(quadtree<T> &node){
    float width = node.width();
    float height = node.height();
    float x_calc = node.up_left.x+(float)WIND_W/2-CAM_X;
    float y_calc = -node.up_left.y+(float)WIND_H/2+CAM_Y;
    Color part_col = get_different_color(boxcount);
    //{randomify_uchar((boxcount+5)*20),randomify_uchar(boxcount*9),randomify_uchar((boxcount+13)*77),255};
    if (DEBUG_MODE){
        DrawRectangleLines(x_calc, y_calc, width, height, part_col);
    }
    if ((int)node.values.size() > node.bucket_size){
        node.split();
    }
    quadtree<T>* deepest_root = node.get_deepest_root();
    vector<qtree_value<T>> insert_after_clear;
    bool merge = false;
    if (node.values.empty() == false){
        for (qtree_value<T>& p : node.values){
            p.data.x += p.data.velx;
            p.data.y += p.data.vely;
            if(node.cond_split || node.tree_up_left != nullptr){
                tree_itt_mov(node, p);
            }
             // SPLIT THIS INTO TWO PHASES, UPDATING AND MOVING, THEN IF WE DETECT A CHANGE WE ONLY CALL THE MOVE FUNCTION FOR THE SECOND TIME
            particle_count++;
            DrawCircle(p.data.x+(float)WIND_W/2-CAM_X,-p.data.y+(float)WIND_H/2+CAM_Y,5,part_col);
            if(deepest_root != nullptr){
                auto found = find(node.values.begin(),node.values.end(),p.data);
                bool inside_root = inside(*deepest_root,p.data);
                int parent_count = (node.parent != nullptr) ? node.parent->tally_branch_values() : node.tally_branch_values();
                if (inside(node,p.data) == false && inside_root){
                    //overall_change = true;
                    if (node.parent != nullptr){
                        if(inside(*node.parent,p.data)){
                            parent_count++;
                        }
                        if (parent_count < node.parent->bucket_size){
                            cout << "MERGE NODES" << endl;
                            merge = true;
                        }
                    }
                    if(node.cond_split){
                        insert_after_clear.insert(insert_after_clear.begin()+insert_after_clear.size(),p);
                    }else{
                        deepest_root->values.insert(deepest_root->values.begin()+deepest_root->values.size(),p);
                    }
                    node.values.erase(found);
                }else if(inside_root == false){
                    if ((parent_count-1) < node.parent->bucket_size){
                            cout << "MERGE ES" << endl;
                            merge = true;
                    }
                    node.values.erase(found);
                }
            }
        }
        if(node.cond_split || node.tree_up_left != nullptr){ // ITTERATE MOVE AFTER THIS
            node.values.clear();
            
            insert_after_clear.clear();
        }
        for(qtree_value<T>& ins : insert_after_clear){
                deepest_root->values.insert(deepest_root->values.begin()+deepest_root->values.size(),ins);
        }
            }
    boxcount++;
    node.cond_split = false;
    if(merge == true){
        quadtree<T>* parent = (node.parent != nullptr) ? node.parent : &node;

        quadtree<T>* uleft = parent->tree_up_left;
        quadtree<T>* uright = parent->tree_up_right;
        quadtree<T>* dleft = parent->tree_down_left;
        quadtree<T>* dright = parent->tree_down_right;

        parent->values.insert(parent->values.end(),uleft->values.begin(),uleft->values.end());
        parent->values.insert(parent->values.end(),uright->values.begin(),uright->values.end());
        parent->values.insert(parent->values.end(),dleft->values.begin(),dleft->values.end());
        parent->values.insert(parent->values.end(),dright->values.begin(),dright->values.end());

        free(uleft);
        free(uright);
        free(dleft);
        free(dright);

        parent->tree_up_left = nullptr;
        parent->tree_up_right = nullptr;
        parent->tree_down_left = nullptr;
        parent->tree_down_right = nullptr;
    }
}


int main(){
//    srand(time(NULL));
    InitWindow(WIND_W,WIND_H,"Quadtree test - changing nodes");

    Vector2 m_pos;
    
    SetTargetFPS(240);

    int numbg_lines = 10;
    int holding = -1;
    quadtree<moving_point> tree = quadtree<moving_point>({-100,100},{100,-100},NULL);
    tree.insert_qtree({50,50,0,0});
    tree.insert_qtree({-20,70,0,0});
    tree.insert_qtree({50,-90,0,0});
    tree.insert_qtree({-60,-80,0.03f,0});
    tree.insert_qtree({10,10,0,0});
    while(!WindowShouldClose()){
        boxcount = 0;
        particle_count = 0;
        //overall_change = false;
        if(framecount == 10){
            tree.insert_qtree({20,-50,0,0.010f});
            tree.insert_qtree({20,-80,0,0.025f});
            tree.insert_qtree({30,-80,0,0.025f});
        }
        if (IsKeyDown(KEY_D)){
            CAM_X += 1.0f;
        }if (IsKeyDown(KEY_A)){
            CAM_X -= 1.0f;
        }if (IsKeyDown(KEY_W)){
            CAM_Y += 1.0f;
        }if (IsKeyDown(KEY_S)){
            CAM_Y -= 1.0f;
        }
        m_pos = GetMousePosition();
        m_pos.x -= CAM_X;
        m_pos.y -= CAM_Y;
        if (IsKeyPressed(KEY_K)){
            holding++;
        }if (IsKeyPressed(KEY_L)){
            holding--;
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            float spacing_w = (float)WIND_W/numbg_lines;
            float spacing_h = (float)WIND_H/numbg_lines;
            for(int i=-1;i<numbg_lines+1;i++){
                float endx = i*spacing_w - fmod((float)CAM_X, spacing_w);
                float endy = i*spacing_h + fmod((float)CAM_Y, spacing_h);
                DrawLineEx((Vector2){endx,0.0f},(Vector2){endx,WIND_H},2.0f,LIGHTGRAY);
                DrawLineEx((Vector2){0.0f,endy},(Vector2){WIND_H,endy},2.0f,LIGHTGRAY);
            }
            tree.itterate(&(tree_itt_func<moving_point>));
            /*if(overall_change){
                tree.itterate(&(tree_itt_mov_wrap<moving_point>));
            }*/
            if(DEBUG_MODE){
                DrawText("DEBUG MODE",2,2,20,RED);
                DrawFPS(2,20);
                DrawText(TextFormat("particle_count : %i",particle_count),2,40,20,GRAY);
                DrawText(TextFormat("x : %f | y : %f",CAM_X,CAM_Y),2,60,20,GRAY);
                DrawText(TextFormat("holding : %i",holding),2,80,20,GRAY);
            }
    //cout << inside(*tree.tree_up_right,quadtree_value<Vector2>({50,-50}).data) << endl;
        EndDrawing();
        framecount++;
    }

    CloseWindow();
    
    return 0;
}
