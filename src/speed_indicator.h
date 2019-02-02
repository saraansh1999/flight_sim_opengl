#include "main.h"

#ifndef SPEED_IND_H
#define SPEED_IND_H


class Speed_ind {
public:
    Speed_ind() {}
    Speed_ind(float x, float y, float z, color_t color);
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

#endif // SPEED_IND_H
