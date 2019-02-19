#include "missile.h"
#include "main.h"

Missile::Missile(float x, float y, float z, glm::vec3 dir, float speed) {
    this->position = glm::vec3(x, y, z);
    this->radius = 7;
    this->ttl = Timer(20);
    this->length = 80;
    this->speed = speed;
    this->direction = dir;
    this->rotation = glm::mat4(1.0f);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float xval = 0, newxval;
    float yval = this->radius, newyval;
    float zval = this->length/2;
    int N = 25;
    float angle = glm::radians(360.0f/N);
    GLfloat vertex_buffer_data[27*N];
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[18*i + 0] = xval;
        vertex_buffer_data[18*i + 1] = yval;
        vertex_buffer_data[18*i + 2] = 0;

        vertex_buffer_data[18*i + 3] = xval;
        vertex_buffer_data[18*i + 4] = yval;
        vertex_buffer_data[18*i + 5] = -zval;

        newxval = xval*cos(angle) - yval*sin(angle);
        newyval = xval*sin(angle) + yval*cos(angle);
        xval = newxval;
        yval = newyval;
        
        vertex_buffer_data[18*i + 6] = xval;
        vertex_buffer_data[18*i + 7] = yval;
        vertex_buffer_data[18*i + 8] = 0;

        vertex_buffer_data[18*i + 9] = xval;
        vertex_buffer_data[18*i + 10] = yval;
        vertex_buffer_data[18*i + 11] = 0;

        vertex_buffer_data[18*i + 12] = vertex_buffer_data[18*i + 3];
        vertex_buffer_data[18*i + 13] = vertex_buffer_data[18*i + 4];
        vertex_buffer_data[18*i + 14] = vertex_buffer_data[18*i + 5];

        vertex_buffer_data[18*i + 15] = xval;
        vertex_buffer_data[18*i + 16] = yval;
        vertex_buffer_data[18*i + 17] = -zval;
    }
    xval = 0;
    yval = this->radius;
    zval = 0;
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[18*(N) - 1 + 9*i + 1] = 0;
        vertex_buffer_data[18*(N) - 1 + 9*i + 2] = 0;
        vertex_buffer_data[18*(N) - 1 + 9*i + 3] = 0 + this->length/2;

        vertex_buffer_data[18*(N) - 1 + 9*i + 4] = xval;
        vertex_buffer_data[18*(N) - 1 + 9*i + 5] = yval;
        vertex_buffer_data[18*(N) - 1 + 9*i + 6] = zval;

        newxval = xval*cos(angle) - yval*sin(angle);
        newyval = xval*sin(angle) + yval*cos(angle);
        xval = newxval;
        yval = newyval;

        vertex_buffer_data[18*(N) - 1 + 9*i + 7] = xval;
        vertex_buffer_data[18*(N) - 1 + 9*i + 8] = yval;
        vertex_buffer_data[18*(N) - 1 + 9*i + 9] = zval;
    }
    this->rotation = glm::rotate((float)(acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(this->direction)))), glm::cross(glm::vec3(0, 0, 1), this->direction));
    this->box.width = this->box.height = 2*radius;
    this->box.breadth = this->length;
    this->object = create3DObject(GL_TRIANGLES, 6*N + 3*N, vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Missile::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    Matrices.model = glm::translate (Matrices.model, this->position);    // glTranslatef
    Matrices.model = Matrices.model * this->rotation;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Missile::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Missile::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
    this->position += this->direction*this->speed*delta_time;
    this->box.pos = this->position;
}
