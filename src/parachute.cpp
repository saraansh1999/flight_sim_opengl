#include "parachute.h"
#include "main.h"

Parachute::Parachute(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int N = 25;
    float angle = -glm::radians(180.0f/N);
    this->speed = -150;
    this->accl = -10;
    this->length = 100;
    this->radius = 100;
    float xval = -this->radius, newxval;
    float yval = 0, newyval;
    float zval = this->length/2;
    GLfloat vertex_buffer_data_para[18*N];
    GLfloat vertex_buffer_data_human[]={
        -20.0f,-20.0f,-20.0f, // triangle 1 : begin
        -20.0f,-20.0f, 20.0f,
        -20.0f, 20.0f, 20.0f, // triangle 1 : end
        20.0f, 20.0f,-20.0f, // triangle 2 : begin
        -20.0f,-20.0f,-20.0f,
        -20.0f, 20.0f,-20.0f, // triangle 2 : end
        20.0f,-20.0f, 20.0f,
        -20.0f,-20.0f,-20.0f,
        20.0f,-20.0f,-20.0f,
        20.0f, 20.0f,-20.0f,
        20.0f,-20.0f,-20.0f,
        -20.0f,-20.0f,-20.0f,
        -20.0f,-20.0f,-20.0f,
        -20.0f, 20.0f, 20.0f,
        -20.0f, 20.0f,-20.0f,
        20.0f,-20.0f, 20.0f,
        -20.0f,-20.0f, 20.0f,
        -20.0f,-20.0f,-20.0f,
        -20.0f, 20.0f, 20.0f,
        -20.0f,-20.0f, 20.0f,
        20.0f,-20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f,-20.0f,-20.0f,
        20.0f, 20.0f,-20.0f,
        20.0f,-20.0f,-20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f,-20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f, 20.0f,-20.0f,
        -20.0f, 20.0f,-20.0f,
        20.0f, 20.0f, 20.0f,
        -20.0f, 20.0f,-20.0f,
        -20.0f, 20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        -20.0f, 20.0f, 20.0f,
        20.0f,-20.0f, 20.0f
    };
    GLfloat vertex_buffer_data_strings[]={
        0, 0, 0,
        -this->radius, 100, -this->length/2,
        0, 0, 0,
        this->radius, 100, -this->length/2,
        0, 0, 0,
        -this->radius, 100, this->length/2,
        0, 0, 0,
        this->radius, 100, this->length/2,
    };
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data_para[18*i + 0] = xval;
        vertex_buffer_data_para[18*i + 1] = 100 + yval;
        vertex_buffer_data_para[18*i + 2] = zval;

        vertex_buffer_data_para[18*i + 3] = xval;
        vertex_buffer_data_para[18*i + 4] = 100 + yval;
        vertex_buffer_data_para[18*i + 5] = -zval;

        newxval = xval*cos(angle) - yval*sin(angle);
        newyval = xval*sin(angle) + yval*cos(angle);
        xval = newxval;
        yval = newyval;
        
        vertex_buffer_data_para[18*i + 6] = xval;
        vertex_buffer_data_para[18*i + 7] = 100 + yval;
        vertex_buffer_data_para[18*i + 8] = zval;

        vertex_buffer_data_para[18*i + 9] = xval;
        vertex_buffer_data_para[18*i + 10] = 100 + yval;
        vertex_buffer_data_para[18*i + 11] = zval;

        vertex_buffer_data_para[18*i + 12] = vertex_buffer_data_para[18*i + 3];
        vertex_buffer_data_para[18*i + 13] = vertex_buffer_data_para[18*i + 4];
        vertex_buffer_data_para[18*i + 14] = vertex_buffer_data_para[18*i + 5];

        vertex_buffer_data_para[18*i + 15] = xval;
        vertex_buffer_data_para[18*i + 16] = 100 + yval;
        vertex_buffer_data_para[18*i + 17] = -zval;
    }

    this->object_human = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data_human, COLOR_BLACK, GL_FILL);
    this->object_para = create3DObject(GL_TRIANGLES, 6*N, vertex_buffer_data_para, COLOR_RED, GL_FILL);
    this->object_strings = create3DObject(GL_LINES, 4*2, vertex_buffer_data_strings, COLOR_BLACK, GL_FILL);
    this->box.width = 40;
    this->box.height = 40;
    this->box.breadth = 40;
}

void Parachute::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(100.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));

    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_para);
    draw3DObject(this->object_human);
    draw3DObject(this->object_strings);
}

void Parachute::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Parachute::tick() {
    this->position.y += this->speed*delta_time;
    this->speed += this->accl*delta_time;
    this->box.pos = this->position;
}

