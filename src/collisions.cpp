#include "collisions.h"

bool Collisions_detector::cuboid_cylinder_collision(bounding_box_t obj, bounding_box_t plane)
{
    if((2*fabs(plane.pos.x - obj.pos.x) < plane.width + obj.width) && (2*fabs(plane.pos.y - obj.pos.y) < plane.height + obj.height) && (2*fabs(plane.pos.z - obj.pos.z) < plane.breadth + obj.breadth))
        return true;
    return false;
}

bool Collisions_detector::circle2DZ_line_collision(glm::vec3 front, glm::vec3 back, glm::vec3 center, float radius)
{
    if((front.z > center.z + 5 && back.z < center.z - 5) || (front.z < center.z - 5 && back.z > center.z + 5))
    {
        float val = glm::length(glm::cross(front - back, center - back))/glm::length(front - back);
        if(val <= radius)
        {
            return true; 
        }
        return false;
    }
    return false;
}

bool Collisions_detector::Ycircular_proximity(glm::vec3 plane, glm::vec3 center, float radius)
{
    if(glm::length(plane - center) <= radius)
        return true;
    return false;
}