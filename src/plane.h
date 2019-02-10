#include "main.h"

#ifndef PLANE_H
#define PLANE_H

class Plane {
    public:
        Plane() {}
        Plane(float x, float y, float z, color_t color);
        glm::vec3 position, back, front;
        void draw(glm::mat4 VP);
        void set_position(float x, float y, float z);
        void tick();
        void move_z(float, float, int);
        void tilt(int, float&, float);
        void ghum(int);
        void rise(int);
        void update_fuel();
        float fuel, length, radius, tilt_speed, tilt_accl, move_accl, move_speed, ghum_speed, ghum_accl, ver_speed, ver_accl, shift_speed, shift_accl, rise_speed, g_speed, g_accl, angle_y, angle_z, angle_x, angle_g;
        glm::mat4 rotation_y, rotation_z, rotation_x;
        // bounding_box_plane box;
        bounding_box_t box;
    private:
        VAO* object;
};

#endif