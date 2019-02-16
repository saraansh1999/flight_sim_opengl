#include "volcano.h"
#include "main.h"

Volcano::Volcano(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    int N = 25;
    this->box.height = 500;
    this->box.width = 500;
    this->box.breadth = 500;
    this->radius = 200;
    float angle = glm::radians(360.0f/N);
    float xval=0, yval=-this->box.height/2, zval = this->radius, newxval, newzval;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[9*N];
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[9*i + 0] = 0;
        vertex_buffer_data[9*i + 1] = this->box.height/2;
        vertex_buffer_data[9*i + 2] = 0;

        vertex_buffer_data[9*i + 3] = xval;
        vertex_buffer_data[9*i + 4] = yval;
        vertex_buffer_data[9*i + 5] = zval;

        newxval = xval*cos(angle) - zval*sin(angle);
        newzval = xval*sin(angle) + zval*cos(angle);
        xval = newxval;
        zval = newzval;

        vertex_buffer_data[9*i + 6] = xval;
        vertex_buffer_data[9*i + 7] = yval;
        vertex_buffer_data[9*i + 8] = zval;
    }



    this->object = create3DObject(GL_TRIANGLES, N*3, vertex_buffer_data, COLOR_MID_RED, GL_FILL);
}

void Volcano::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Volcano::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Volcano::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}

