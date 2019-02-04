#include "main.h"
#include "timer.h"
#include "plane.h"
#include "sea.h"
#include "ground.h"
#include "missile.h"
#include "fuel_up.h"
#include "speed_indicator.h"
#include "alt_indicator.h"
#include "fuel_indicator.h"
#include "ball.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

vector<Ground> grounds;
Plane plane;
Sea sea;
Speed_ind speed_ind;
Alt_ind alt_ind;
Fuel_ind fuel_ind;
vector<Missile> missiles;
vector<Fuel_up> fuel_ups;

int no_fuel_ups = 100;
int no_grounds = 1000;
float heli_speed = 10000;
float world_breadth = 1000000000, world_height = 1000000000, world_width = 1000000000;
float xscaler, yscaler, zscaler;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float height = 800, width = 1000, breadth = 1000;
float fov = 45;
glm::vec3 face(0, 0, -1);
glm::vec3 eye(0, 200, -500), target(0, 0, 0), up(0, 1, 0);
// glm::mat4 world_rotation = glm::mat4(1.0f);
// glm::mat4 world_translation = glm::mat4(1.0f);
int view = 1;
double x_mouse = 500, y_mouse=500, mouse_x_angle=0, mouse_y_angle=0, sensitivity = 0.05;
float curFrame=0, lastFrame=0, delta_time;
bool start = false;
Timer missile_timer(0.2);
Timer t60(1.0 / 60);

void cursor_callback(GLFWwindow *window, double posx, double posy)
{
    if(!start && view==5)
    {
        posx = x_mouse;
        posy = y_mouse;
        start = true;
    }
    double chg_x_mouse = (posx - x_mouse)*sensitivity;
    double chg_y_mouse = (posy - y_mouse)*sensitivity;
    x_mouse = posx;
    y_mouse = posy;
    mouse_x_angle += chg_x_mouse;
    mouse_y_angle += chg_y_mouse;
}

void scroll_callback(GLFWwindow *window, double x, double y)
{
    if(view == 5)
    {
        eye -= glm::normalize(eye-target)*heli_speed*delta_time*(float)y;
    }
}

void set_camera()
{
    if(view == 1)
    {
        xscaler = sin(glm::radians(plane.angle_y));
        yscaler = cos(glm::radians(plane.angle_z));
        zscaler = cos(glm::radians(plane.angle_y));
        eye.x = plane.back.x + 500*xscaler;
        eye.y = plane.back.y + 300*yscaler;
        eye.z = plane.back.z + 500*zscaler;
        target.x = plane.front.x - (breadth/2)*xscaler;
        target.y = plane.front.y;
        target.z = plane.front.z - (breadth/2)*zscaler;
        up = glm::vec3(0, cos(glm::radians(plane.angle_x)), -sin(glm::radians(plane.angle_x)));
        face = eye - target;
    }
    else if(view == 2)
    {
        xscaler = sin(glm::radians(plane.angle_y));
        yscaler = 1;
        zscaler = cos(glm::radians(plane.angle_y));
        eye.x = plane.back.x + 600*xscaler;
        eye.y = plane.back.y + 10000*yscaler;
        eye.z = plane.back.z + 600*zscaler;
        target.x = plane.front.x - (breadth/2)*xscaler;
        target.y = plane.front.y;
        target.z = plane.front.z - (breadth/2)*zscaler;
        up = glm::vec3(0, 1, 0);
        face = eye - target;
    }
    else if(view == 3)
    {
        eye = glm::vec3(0 , 1000, 500);
        target = plane.position;
        up = glm::vec3(0, 1, 0);
        face = plane.back - plane.front;
    }
    else if(view == 4)
    {
        xscaler = sin(glm::radians(plane.angle_y));
        yscaler = sin(glm::radians(plane.angle_x));
        zscaler = cos(glm::radians(plane.angle_y));
        eye = plane.front;
        target.x = plane.front.x - (breadth/2)*xscaler;
        target.y = plane.front.y + (breadth/2)*yscaler;
        target.z = plane.front.z - (breadth/2)*zscaler;
        up = glm::vec3(-sin(glm::radians(plane.angle_z)), cos(glm::radians(plane.angle_z))*cos(glm::radians(plane.angle_x)), -sin(glm::radians(plane.angle_x)));
        face = eye - target;
    }
    else if(view == 5)
    {
        target.z = eye.z - 100*cos(glm::radians(mouse_x_angle))*cos(glm::radians(mouse_y_angle));
        target.y = eye.y - 100*sin(glm::radians(mouse_y_angle));
        target.x = eye.x + 100*sin(glm::radians(mouse_x_angle));  
        face = plane.back - plane.front;
    }
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    set_camera();

    //speed_ind.set_mag(glm::length(eye-target));
    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    for(int i=0;i<grounds.size();i++)
        grounds[i].draw(VP);
    plane.draw(VP);
    sea.draw(VP);
    speed_ind.draw(Matrices.projection, plane.move_speed);
    alt_ind.draw(Matrices.projection, plane.position.y);
    fuel_ind.draw(Matrices.projection, plane.fuel);
    for(int i=0;i<missiles.size();i++)
        missiles[i].draw(VP);
}

void tick_input(GLFWwindow *window) {
    int f_cam  = glfwGetKey(window, GLFW_KEY_1);
    int top_cam = glfwGetKey(window, GLFW_KEY_2);
    int tower_cam = glfwGetKey(window, GLFW_KEY_3);
    int fps_cam = glfwGetKey(window, GLFW_KEY_4);
    int heli_cam = glfwGetKey(window, GLFW_KEY_5);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int q = glfwGetKey(window, GLFW_KEY_Q);
    int e = glfwGetKey(window, GLFW_KEY_E);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    int left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    
    if (f_cam) {
        view = 1;
    }
    if(top_cam){
        view = 2;
    }
    if(tower_cam){
        view = 3;
    }
    if(fps_cam){
        view = 4;
    }
    if(heli_cam){
        target = glm::vec3(0, 0, 0);
        eye = glm::vec3(0, 0, 100);
        view = 5;
    }

    if(w){
        plane.move_z(face.x, face.z, 1);
    }
    else{
        plane.move_z(face.x, face.z, -1);
    }
    if(a){
        plane.tilt(1, plane.tilt_speed, plane.tilt_accl);
    }
    else if(d){
        plane.tilt(-1, plane.tilt_speed, plane.tilt_accl);
    }
    else{
        plane.tilt(0, plane.tilt_speed, plane.tilt_accl);
    }
    if(q){
        plane.ghum(1);
    }
    else if(e){
        plane.ghum(-1);
    }
    else{
        plane.ghum(0);
    }
    if(up){
        plane.rise(1);
    }
    else if(down){
        plane.rise(-1);
    }
    else{
        plane.rise(0);
    }

    if(left == GLFW_PRESS && missile_timer.processTick()){
        missiles.push_back(Missile(plane.front.x, plane.front.y, plane.front.z, plane.front - plane.back, COLOR_BLACK));
    }
}

void tick_elements() {
    // ball1.tick();
    if(plane.position.y < -2000.0f)
        quit(window);
    if(plane.fuel<=0)
        quit(window);
    plane.tick();
    sea.tick();
    for(int i=0;i<grounds.size();i++)
        grounds[i].tick();
    for(int i=0;i<missiles.size();)
    {
        missiles[i].tick();
        // if(missiles[i].position.z > world_breadth)
        //     missiles.erase(missiles.begin() + i);
        // else
            i++;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // ball1       = Ball(0, 0, COLOR_RED);
    for(int i=0;i<no_grounds;i++)
    {
        grounds.push_back(Ground(-world_width/10000 + rand()%(int)(2*world_width/10000), -2000, -world_breadth/10000 + rand()%(int)(2*world_breadth/10000), COLOR_GREEN));
    }
    // for(int i=0;i<no_fuel_ups;i++)
    // {
    //     fuel_ups.push_back(Fuel_up())
    // }
    plane = Plane(0, 0, 0, COLOR_RED);
    sea = Sea(0.0f, -2000.0f, 0.0f, COLOR_BLUE);
    speed_ind = Speed_ind(0, 0, 0, COLOR_RED);
    speed_ind.set_position(-250, -350, -1000);
    alt_ind = Alt_ind(0, 0, 0, COLOR_RED);
    alt_ind.set_position(250, -350, -1000);
    fuel_ind = Fuel_ind(0, 0, 0, COLOR_RED);
    fuel_ind.set_position(0, 350, -1000);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            curFrame = glfwGetTime();
            delta_time = curFrame - lastFrame;
            lastFrame = curFrame;
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::perspective(glm::radians(fov), height/width, 0.1f, world_breadth/4);
}
