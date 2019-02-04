#include "main.h"

#ifndef FUEL_UP_H
#define FUEL_UP_H


class Fuel_up {
public:
    Fuel_up() {}
    Fuel_up(float x, float y, float z, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
private:
    VAO *object;
};

#endif // FUEL_UP_H
