#include "main.h"

#ifndef SEGMENT_H
#define SEGMENT_H


class Segment {
public:
    Segment() {}
    Segment(float x, float y, float z);
    glm::vec3 position;
    void draw(glm::mat4 VP, int dig);
    void set_position(float x, float y, float z);
    void tick();
    bounding_box_t box;
private:
    VAO *object[7];
};

#endif // SEGMENT_H
