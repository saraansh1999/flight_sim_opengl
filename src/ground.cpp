#include "ground.h"
#include "main.h"   

Ground::Ground(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->lives = 3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data1[] = {
        -300, -250, -300,
        -300, -250, 300,
        300, -250, 300,
        300, -250 , 300,
        -300, -250, -300,
        300, -250, -300,

        -300, -200, -300,
        -300, -200, 300,
        300, -200, 300,
        300, -200 , 300,
        -300, -200, -300,
        300, -200, -300,

        -300, -250, -300, 
        -300, -200, -300,
        -300, -250, 300,
        -300, -250, 300,
        -300, -200, 300,
        -300, -200, -300,

        300, -250, -300, 
        300, -200, -300,
        300, -250, 300,
        300, -250, 300,
        300, -200, 300,
        300, -200, -300,

        -300, -250, 300,
        300, -250, 300,
        300, -200, 300,
        -300, -250, 300,
        -300, -200, 300,
        300, -200, 300,

        -300, -250, -300,
        300, -250, -300,
        300, -200, -300,
        -300, -250, -300,
        -300, -200, -300,
        300, -200, -300
    };
    static const GLfloat vertex_buffer_data2[] = {
        -50, 0, -50,
        -50, 0, 50,
        50, 0, 50,
        50, 0 , 50,
        -50, 0, -50,
        50, 0, -50,

        -50, 250, -50,
        -50, 250, 50,
        50, 250, 50,
        50, 250 , 50,
        -50, 250, -50,
        50, 250, -50,

        -50, -250, -50, 
        -50, 250, -50,
        -50, -250, 50,
        -50, -250, 50,
        -50, 250, 50,
        -50, 250, -50,

        50, -250, -50, 
        50, 250, -50,
        50, -250, 50,
        50, -250, 50,
        50, 250, 50,
        50, 250, -50,

        -50, -250, 50,
        50, -250, 50,
        50, 250, 50,
        -50, -250, 50,
        -50, 250, 50,
        50, 250, 50,

        -50, -250, -50,
        50, -250, -50,
        50, 250, -50,
        -50, -250, -50,
        -50, 250, -50,
        50, 250, -50,

    };
    this->box.width = this->box.breadth = 100;
    this->box.height = 500;
    this->box.pos = position;
    this->object_ground = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data1, COLOR_GREEN, GL_FILL);
    this->object_shooter = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data2, COLOR_LIGHT_PURPLE, GL_FILL);
}

void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(100.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));

    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_ground);
    draw3DObject(this->object_shooter);
}

void Ground::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

Missile Ground::create_missile(glm::vec3 plane)
{
    return Missile(this->position.x, this->position.y + (0 + rand()%3)*20, this->position.z, glm::vec3(plane - this->position), 1.0f);
}        

int Ground::hit()
{
    this->lives--;
    if(this->lives == 0)
        return 1;
    return 0;
}

void Ground::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
}

