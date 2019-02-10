#include "main.h"

#ifndef MARKER_H
#define MARKER_H


class Marker {
public:
    Marker() {}
    Marker(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, glm::vec3);
    void set_position(float x, float y, float z);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // MARKER_H
