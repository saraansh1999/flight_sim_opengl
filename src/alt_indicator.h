#include "main.h"

#ifndef ALT_IND_H
#define ALT_IND_H


class Alt_ind {
public:
    Alt_ind() {}
    Alt_ind(float x, float y, float z);
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

#endif // ALT_IND_H
