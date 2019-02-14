#include "main.h"

#ifndef COLLISIONS_H
#define COLLISIONS_H

class Collisions_detector{
    public:
        Collisions_detector(){}
        bool cuboid_cylinder_collision(bounding_box_t obj, bounding_box_t plane);
        bool circle2DZ_line_collision(glm::vec3, glm::vec3, glm::vec3, float);
        bool Ycircular_proximity(glm::vec3, glm::vec3, float);

};

#endif