#include "main.h"
#include "missile.h"

#ifndef GROUND_H
#define GROUND_H


class Ground {
public:
    Ground() {}
    Ground(float x, float y, float z, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    int hit();
    Missile create_missile(glm::vec3);
    int lives;
    bounding_box_t box;
private:
    VAO *object_ground;
    VAO *object_shooter;
};

#endif // GROUND_H
