#pragma once

#include <cstdlib>
#include <iostream>
#include <raylib/raylib.h>

using namespace std;

struct AABB{
    Vector2 up_left,down_right;

    bool inside(Vector2);
    bool intersects(AABB);

    AABB(Vector2 _uleft, Vector2 _dright){
        up_left = _uleft;
        down_right = _dright;
    }
};

/* Only works in areas where X+ is to the right and Y+ is up */
bool AABB::inside(Vector2 point){
    if (this->up_left.x > point.x) { return false; } /* P ---- | */

    if (this->down_right.x <= point.x) { return false; } /* | ---- P */

    if (this->up_left.y < point.y) { return false; }

    if (this->down_right.y >= point.y) { return false; }

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

/*template<typename T>
struct quadtree{
    T value;  HAS to have an x and y member 

    quadtree<T>* up_left,up_right,down_left,down_right;
};*/
