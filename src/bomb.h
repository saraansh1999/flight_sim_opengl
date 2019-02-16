#include "main.h"

#ifndef BOMB_H
#define BOMB_H


class Bomb {
public:
    Bomb() {}
    Bomb(float x, float y, float z);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    double speed, accl, impact_radius;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // BOMB_H
