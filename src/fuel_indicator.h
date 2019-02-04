#include "main.h"

#ifndef FUEL_IND_H
#define FUEL_IND_H


class Fuel_ind {
public:
    Fuel_ind() {}
    Fuel_ind(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, float);
    void set_position(float x, float y, float z);
    void tick();
    float radius, value_angle;
private:
    VAO *object_ring;
    VAO *object_stick;
};

#endif // FUEL_IND_H
