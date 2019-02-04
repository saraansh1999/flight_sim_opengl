#include "fuel_up.h"
#include "main.h"

Fuel_up::Fuel_up(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -50, -50, -5,
        -50, 50, -5, 
        50, -50, -5,
        50, -50, -5,
        50, 50, -5,
        -50, 50, -5,

        -50, -50, 5,
        -50, 50, 5, 
        50, -50, 5,
        50, -50, 5,
        50, 50, 5,
        -50, 50, 5,

        -50, 50, -5,
        -50, 50, 5,
        50, 50, -5,
        50, 50, -5,
        50, 50, 5,
        -50, 50, 5,

        -50, -50, -5,
        -50, -50, 5,
        50, -50, -5,
        50, -50, -5,
        50, -50, 5,
        -50, -50, 5,

        50, 50, -5,
        50, 50, 5,
        50, -50, -5,
        50, -50, -5,
        50, -50, 5,
        50, 50, 5,

        -50, 50, -5,
        -50, 50, 5,
        -50, -50, -5,
        -50, -50, -5,
        -50, -50, 5,
        -50, 50, 5,
    };

    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Fuel_up::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(100.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));

    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Fuel_up::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, 0);
}

void Fuel_up::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}
