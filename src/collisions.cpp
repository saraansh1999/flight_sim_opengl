#include "collisions.h"

bool Collisions_detector::cuboid_cylinder_collision(bounding_box_t obj, bounding_box_t plane)
{
    if((2*fabs(plane.pos.x - obj.pos.x) < plane.width + obj.width) && (2*fabs(plane.pos.y - obj.pos.y) < plane.height + obj.height) && (2*fabs(plane.pos.z - obj.pos.z) < plane.breadth + obj.breadth))
        return true;
    return false;
}