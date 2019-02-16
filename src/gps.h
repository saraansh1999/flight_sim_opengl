#include "main.h"

#ifndef GPS_H
#define GPS_H


class Gps {
public:
    Gps() {}
    Gps(float x, float y, float z);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, float, glm::vec3, glm::vec3);
    void set_position(float x, float y, float z);
    void tick();
    float radius, value_angle;
private:
    VAO *object_ring;
    VAO *object_stick_north;
    VAO *object_stick_checkpoint;
};

#endif // GPS_H
