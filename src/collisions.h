#include "main.h"

#ifndef COLLISIONS_H
#define COLLISIONS_H

class Collisions_detector{
    public:
        Collisions_detector(){}
        bool cuboid_cylinder_collision(bounding_box_t obj, bounding_box_t plane);

};

#endif