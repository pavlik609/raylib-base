#pragma once

#include "shared.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <variant>
#include <any>
#include <vector>
#include <raylib/raylib.h>

using namespace std;
/* Wrapper */
struct AABB{
    Vector2 up_left,down_right; /* theese members NEED to have a x and y member */

    bool intersects(AABB);

    float width(void);
    float height(void);

    AABB(Vector2 _uleft, Vector2 _dright){
        up_left = _uleft;
        down_right = _dright;
    }
};

template<typename T>
/* Only works in areas where X+ is to the right and Y+ is up */
bool inside(AABB aabb, T point){
    if (aabb.up_left.x > point.x) { return false; } /* P ---- | */

    if (aabb.down_right.x <= point.x) { return false; } /* | ---- P */

    if (aabb.up_left.y < point.y) { return false; }

    if (aabb.down_right.y >= point.y) { return false; }

    return true;
}

/* Only works in areas where X+ is to the right and Y+ is up */
bool AABB::intersects(AABB box){
    if (( (box.up_left.x <= this->up_left.x && this->up_left.x <= box.down_right.x) ||
            (box.up_left.x <= this->down_right.x && this->down_right.x <= box.down_right.x)) &&
            ((box.up_left.y >= this->up_left.y && this->up_left.y >= box.down_right.y) ||
            (box.up_left.y >= this->down_right.y && this->down_right.y >= box.down_right.y))) { return true; }

    return false;
}
float AABB::width(void){
    return this->down_right.x - this->up_left.x;
}

float AABB::height(void){
    return this->up_left.y - this->down_right.y;
}

template<typename T>
struct quadtree;

template<typename T>
struct quadtree : AABB{
    vector<T> values;  /*HAS to have an x and y member*/ 

    quadtree<T>* tree_up_left;
    quadtree<T>* tree_up_right;
    quadtree<T>* tree_down_left;
    quadtree<T>* tree_down_right;

    int shift;
    int min_shift;
    int bucket_size;
    bool cond_split;

    quadtree<T>(Vector2 _uleft, Vector2 _dright) : AABB({0,0},{0,0}){
        this->up_left = _uleft;
        this->down_right = _dright;
        bucket_size = 4;
        tree_up_left = NULL;
        tree_up_right = NULL;
        tree_down_left = NULL;
        tree_down_right = NULL;
        shift = 0;
        min_shift = 99999;
    }
    void split(void);
    void insert_qtree(T);
    void itterate(void (*fptr)(quadtree<T>&));
};

template<typename T>
void quadtree<T>::insert_qtree(T value){
    int i = this->values.size();
    this->values.insert(this->values.begin()+i,value);
    //cout << "IDX (ADD BEGIN IF NOT WORK): " << this->values.size()-1 << endl;
}

template<typename T>
void quadtree<T>::itterate(void (*fptr)(quadtree<T>&)){
    if(DEBUG_MODE || this->values.empty() == false){
        fptr(*this); 
    }

    if (tree_up_left != NULL) { tree_up_left->itterate(fptr); }
    if (tree_up_right != NULL) { tree_up_right->itterate(fptr); }
    if (tree_down_left != NULL) { tree_down_left->itterate(fptr); }
    if (tree_down_right != NULL) { tree_down_right->itterate(fptr); }
}

template<typename T>
bool place_if_possible(quadtree<T>* tree,quadtree<T>* node,T value){
    if (tree == NULL || node == NULL){
        return false;
    }
    if(tree->values.empty() == true){
        return false;
    }
    if(inside(*node,value) == false){
        cout << "test" << endl;
        return false;
    }
    if (value.velx != 0){
        cout << value.x << " t" << endl;
    }
    //cout << inside(*node,value.data) << " " << value.data.x << " " << value.data.y << endl;
    int i = node->values.size();
    node->values.insert((node->values.begin()+i),value);

    //tree->values.erase(find(tree->values.begin(),tree->values.end(),value));
    return true;
}

template<typename T>
void quadtree<T>::split(void){
    this->cond_split = true;
    Vector2 middle = {this->up_left.x+this->width()/2,this->up_left.y-this->height()/2};
    this->tree_up_left = new quadtree<T>(this->up_left,middle);
    this->tree_up_right = new quadtree<T>({middle.x,this->up_left.y},{this->down_right.x,middle.y});
    this->tree_down_left = new quadtree<T>({this->up_left.x,middle.y},{middle.x,this->down_right.y});
    this->tree_down_right = new quadtree<T>({middle.x,middle.y},this->down_right);
}
