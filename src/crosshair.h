#include "main.h"

#ifndef CROSSHAIR_H
#define CROSSHAIR_H


class Crosshair {
public:
    Crosshair() {}
    Crosshair(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(glm::vec3, float, float);
    float retain_y, retain_z;
    double speed;
private:
    VAO *object;
};

#endif // CROSSHAIR_H
