#include "gps.h"

float angle_between(glm::vec3 v1, glm::vec3 v2)
{
    float val = acos(glm::dot(v1, v2));
    float dir = asin(glm::cross(v1, v2))[1];
    if(dir>=0)
        return -val;
    else
        return val;

}

Gps::Gps(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    //this->rotation = 0;
    this->radius = 75;
    this->value_angle = 0;
    int N = 25;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data_ring[6*N];
    GLfloat vertex_buffer_data_stick_north[3*3];
    GLfloat vertex_buffer_data_stick_checkpoint[3*3];

    float angle = 0;
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data_ring[6*i + 0] = this->radius * cos(glm::radians(angle));
        vertex_buffer_data_ring[6*i + 1] = this->radius * sin(glm::radians(angle));
        vertex_buffer_data_ring[6*i + 2] = 0.0f;
        angle += 360.0f/N;
        vertex_buffer_data_ring[6*i + 3] = this->radius * cos(glm::radians(angle));
        vertex_buffer_data_ring[6*i + 4] = this->radius * sin(glm::radians(angle));
        vertex_buffer_data_ring[6*i + 5] = 0.0f;
    }

    vertex_buffer_data_stick_north[0] = -5.0f;
    vertex_buffer_data_stick_north[1] = 0.0f;
    vertex_buffer_data_stick_north[2] = 0.0f;
    vertex_buffer_data_stick_north[3] = 5.0f;
    vertex_buffer_data_stick_north[4] = 0.0f;
    vertex_buffer_data_stick_north[5] = 0.0f;
    vertex_buffer_data_stick_north[6] = 0.0f;
    vertex_buffer_data_stick_north[7] = this->radius;
    vertex_buffer_data_stick_north[8] = 0.0f;

    vertex_buffer_data_stick_checkpoint[0] = -5.0f;
    vertex_buffer_data_stick_checkpoint[1] = 0.0f;
    vertex_buffer_data_stick_checkpoint[2] = 0.0f;
    vertex_buffer_data_stick_checkpoint[3] = 5.0f;
    vertex_buffer_data_stick_checkpoint[4] = 0.0f;
    vertex_buffer_data_stick_checkpoint[5] = 0.0f;
    vertex_buffer_data_stick_checkpoint[6] = 0.0f;
    vertex_buffer_data_stick_checkpoint[7] = this->radius;
    vertex_buffer_data_stick_checkpoint[8] = 0.0f;

    this->object_ring = create3DObject(GL_LINES, 2*N, vertex_buffer_data_ring, COLOR_PITCH_BLACK, GL_FILL);
    this->object_stick_north = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data_stick_north, COLOR_RED, GL_FILL);
    this->object_stick_checkpoint = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data_stick_checkpoint, COLOR_PITCH_BLACK, GL_FILL);
}

void Gps::draw(glm::mat4 VP, float north, glm::vec3 v1, glm::vec3 v2) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_ring);

    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    glm::mat4 rotate = glm::rotate(-glm::radians(north), glm::vec3(0, 0, 1));
    Matrices.model *= (translate*rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_stick_north);

    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    rotate = glm::rotate(angle_between(v1, v2), glm::vec3(0, 0, 1));
    Matrices.model *= (translate*rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_stick_checkpoint);
}

void Gps::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Gps::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}

