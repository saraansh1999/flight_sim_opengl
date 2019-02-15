#include "main.h"

#ifndef PARACHUTE_H
#define PARACHUTE_H


class Parachute {
public:
    Parachute() {}
    Parachute(float x, float y, float z);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    float length, radius, speed, accl;
    bounding_box_t box;
private:
    VAO *object_para;
    VAO *object_human;
    VAO *object_strings;
};

#endif // PARACHUTE_H
