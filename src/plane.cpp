#include "main.h"
#include "plane.h"


Plane::Plane(float x, float y, float z, color_t color){
    this->position = glm::vec3(x, y, z);
    this->length = 100;
    this->radius = 20;
    this->tilt_speed = 0;
    this->tilt_accl = 10;
    this->move_speed = 500;
    this->move_accl = 100;
    this->ghum_speed = 20;
    this->shift_speed = 0;
    this->shift_accl = 10;
    this->rise_speed = 30;
    this->rotation_y=glm::mat4(1.0f);
    this->rotation_z=glm::mat4(1.0f);
    this->rotation_x=glm::mat4(1.0f);
    this->angle_y = 0;
    this->angle_z = 0;
    this->angle_x = 0;
    float xval = 0, newxval;
    float yval = this->radius, newyval;
    float zval = this->length/2;
    int N = 25;
    float angle = glm::radians(360.0f/N);
    GLfloat vertex_buffer_data[(N+1)*18];
    for(int i=0;i<N;i++)
    {
        vertex_buffer_data[18*i + 0] = xval;
        vertex_buffer_data[18*i + 1] = yval;
        vertex_buffer_data[18*i + 2] = zval;

        vertex_buffer_data[18*i + 3] = xval;
        vertex_buffer_data[18*i + 4] = yval;
        vertex_buffer_data[18*i + 5] = -zval;

        newxval = xval*cos(angle) - yval*sin(angle);
        newyval = xval*sin(angle) + yval*cos(angle);
        xval = newxval;
        yval = newyval;
        
        vertex_buffer_data[18*i + 6] = xval;
        vertex_buffer_data[18*i + 7] = yval;
        vertex_buffer_data[18*i + 8] = zval;

        vertex_buffer_data[18*i + 9] = xval;
        vertex_buffer_data[18*i + 10] = yval;
        vertex_buffer_data[18*i + 11] = zval;

        vertex_buffer_data[18*i + 12] = vertex_buffer_data[18*i + 3];
        vertex_buffer_data[18*i + 13] = vertex_buffer_data[18*i + 4];
        vertex_buffer_data[18*i + 14] = vertex_buffer_data[18*i + 5];

        vertex_buffer_data[18*i + 15] = xval;
        vertex_buffer_data[18*i + 16] = yval;
        vertex_buffer_data[18*i + 17] = -zval;
    }
    vertex_buffer_data[18*N - 1 + 1] = this->radius;
    vertex_buffer_data[18*N - 1 + 2] = this->position.y;
    vertex_buffer_data[18*N - 1 + 3] = this->position.z - this->length/5;
    vertex_buffer_data[18*N - 1 + 4] = this->radius;
    vertex_buffer_data[18*N - 1 + 5] = this->position.y;
    vertex_buffer_data[18*N - 1 + 6] = this->position.z + this->length/5;
    vertex_buffer_data[18*N - 1 + 7] = this->radius + this->length/3;
    vertex_buffer_data[18*N - 1 + 8] = this->position.y;
    vertex_buffer_data[18*N - 1 + 9] = this->position.z + this->length/5;
    vertex_buffer_data[18*N - 1 + 10] = -this->radius;
    vertex_buffer_data[18*N - 1 + 11] = this->position.y;
    vertex_buffer_data[18*N - 1 + 12] = this->position.z - this->length/5;
    vertex_buffer_data[18*N - 1 + 13] = -this->radius;
    vertex_buffer_data[18*N - 1 + 14] = this->position.y;
    vertex_buffer_data[18*N - 1 + 15] = this->position.z + this->length/5;
    vertex_buffer_data[18*N - 1 + 16] = -this->radius - this->length/3;
    vertex_buffer_data[18*N - 1 + 17] = this->position.y;
    vertex_buffer_data[18*N - 1 + 18] = this->position.z + this->length/5;
    this->object = create3DObject(GL_TRIANGLES, N*6 + 2*3, vertex_buffer_data, color, GL_FILL);
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    //glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * this->rotation_y * this->rotation_z * this->rotation_x);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plane::move_z(float xdir, float zdir, int dir)
{
    this->position -= glm::normalize(glm::vec3(xdir, 0, zdir))*this->move_speed*delta_time;
    if(dir == 1 && this->move_speed<2000)    
        this->move_speed += this->move_accl*delta_time;
    else if(dir == -1 && this->move_speed>500)
        this->move_speed -= 3*this->move_accl*delta_time;
}

void Plane::tilt(int dir, float &speed, float accl)
{
    this->angle_y += speed*delta_time;
    if(dir == 1 && speed<50)
        speed += accl*delta_time;
    else if(dir == -1 && speed > -50)
        speed -= accl*delta_time;
    else if(dir == 0 && speed>0)
        speed -= 5*accl*delta_time;
    else if(dir == 0 && speed<0)
        speed += 5*accl*delta_time;
    this->rotation_y = glm::rotate(glm::radians(this->angle_y), glm::vec3(0, 1, 0));
}

void Plane::ghum(int dir)
{
    if((dir == 1 && this->angle_z<45) || (dir==-1 && this->angle_z>-45))
    {
        this->angle_z += dir*this->ghum_speed*delta_time;
    }
    else if(dir == 0)
    {
        if(this->angle_z>1)
            this->angle_z -= this->ghum_speed*delta_time;
        else if(this->angle_z<-1)
            this->angle_z += this->ghum_speed*delta_time;
        else
            this->angle_z = 0;
    }
    this->rotation_z = glm::rotate(glm::radians(this->angle_z), glm::vec3(0, 0, 1));
    this->tilt(dir, this->shift_speed, this->shift_accl);
}

void Plane::rise(int dir)
{
    this->angle_x += dir*this->rise_speed*delta_time;
    this->rotation_z = glm::rotate(glm::radians(this->angle_x), glm::vec3(1, 0, 0));
}

void Plane::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}
void Plane::tick() {
    // this->position.x -= speed;
    // this->position.y -= speed;
    this->back.x = this->position.x + (this->length/2)*sin(glm::radians(this->angle_y));
    this->back.y = this->position.y;
    this->back.z = this->position.z + (this->length/2)*cos(glm::radians(this->angle_y));
    this->front.x = this->position.x - (this->length/2)*sin(glm::radians(this->angle_y));
    this->front.y = this->position.y;
    this->front.z = this->position.z - (this->length/2)*cos(glm::radians(this->angle_y));
}