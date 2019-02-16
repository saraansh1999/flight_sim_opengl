#include "fuel_indicator.h"
#include "main.h"

Fuel_ind::Fuel_ind(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    //this->rotation = 0;
    this->radius = 75;
    this->value_angle = 0;
    int N = 25;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data_ring[6*N];
    GLfloat vertex_buffer_data_stick[3*3];

    float angle = 0;
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data_ring[6*i + 0] = this->radius * cos(angle*M_PI/180);
        vertex_buffer_data_ring[6*i + 1] = this->radius * sin(angle*M_PI/180);
        vertex_buffer_data_ring[6*i + 2] = 0.0f;
        angle += 180.0f/N;
        vertex_buffer_data_ring[6*i + 3] = this->radius * cos(angle*M_PI/180);
        vertex_buffer_data_ring[6*i + 4] = this->radius * sin(angle*M_PI/180);
        vertex_buffer_data_ring[6*i + 5] = 0.0f;
    }

    vertex_buffer_data_stick[0] = 0.0f;
    vertex_buffer_data_stick[1] = 5.0f;
    vertex_buffer_data_stick[2] = 0.0f;
    vertex_buffer_data_stick[3] = 0.0f;
    vertex_buffer_data_stick[4] = -5.0f;
    vertex_buffer_data_stick[5] = 0.0f;
    vertex_buffer_data_stick[6] = -this->radius;
    vertex_buffer_data_stick[7] = 0.0f;
    vertex_buffer_data_stick[8] = 0.0f;

    this->object_ring = create3DObject(GL_LINES, 2*N, vertex_buffer_data_ring, COLOR_PITCH_BLACK, GL_FILL);
    this->object_stick = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data_stick, COLOR_RED, GL_FILL);
}

void Fuel_ind::draw(glm::mat4 VP, float fuel) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_ring);

    Matrices.model = glm::mat4(1.0f);
    translate = glm::translate (this->position);
    glm::mat4 rotate = glm::rotate(glm::radians((-fuel)*180/400), glm::vec3(0, 0, 1));
    Matrices.model *= (translate*rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_stick);
}

void Fuel_ind::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Fuel_ind::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}

