#include "crosshair.h"
#include "main.h"

Crosshair::Crosshair(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -1, 2, 0,
        -1, 7, 0,
        1, 2, 0,
        1, 2, 0,
        1, 7, 0,
        -1, 7, 0,

        -1, -2, 0,
        -1, -7, 0,
        1, -2, 0,
        1, -2, 0,
        1, -7, 0,
        -1, -7, 0,

        2, 1, 0,
        7, 1, 0,
        2, -1, 0,
        2, -1, 0,
        7, -1, 0,
        7, 1, 0,

        -2, 1, 0,
        -7, 1, 0,
        -2, -1, 0,
        -2, -1, 0,
        -7, -1, 0,
        -7, 1, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3*4, vertex_buffer_data, COLOR_MID_RED, GL_FILL);
}

void Crosshair::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 rotate_y = glm::rotate(glm::radians(retain_y), glm::vec3(0, 1, 0));
    glm::mat4 rotate_z = glm::rotate(glm::radians(retain_z), glm::vec3(0, 0, 1));
    Matrices.model *= (translate*rotate_y*rotate_z);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Crosshair::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Crosshair::tick(glm::vec3 pos, float y, float z) {
    this->position = pos;
    this->retain_y = y;
    this->retain_z = z;
}

