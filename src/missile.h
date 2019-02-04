#include "main.h"

#ifndef MISSILE_H
#define MISSILE_H


class Missile {
public:
    Missile() {}
    Missile(float x, float y, float z, glm::vec3 dir, color_t color);
    glm::vec3 position;
    glm::mat4 rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    float radius, length, speed;
    glm::vec3 direction;
private:
    VAO *object;
};

#endif // MISSILE_H
