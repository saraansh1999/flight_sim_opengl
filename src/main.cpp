#include "main.h"
#include "timer.h"
#include "plane.h"
#include "sea.h"
#include "ground.h"
#include "volcano.h"
#include "missile.h"
#include "bomb.h"
#include "parachute.h"
#include "fuel_up.h"
#include "marker.h"
#include "crosshair.h"
#include "ring.h"
#include "speed_indicator.h"
#include "alt_indicator.h"
#include "fuel_indicator.h"
#include "gps.h"
#include "collisions.h"
#include "segment_display.h"
#include "ball.h"

using namespace std;

GLMatrices Matrices;
glm::mat4 projection_normal;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Collisions_detector detector;
map<int, Ground> grounds;
vector<Volcano> volcanoes;
map<int, Ground>::iterator checkpoint;
Plane plane;
Sea sea;
Marker marker;
Crosshair crosshair;
Speed_ind speed_ind;
Alt_ind alt_ind;
Fuel_ind fuel_ind;
Gps gps;
vector<Parachute> parachutes;
vector<Missile> missiles;
vector<Missile> enemy_missiles;
vector<Bomb> bombs;
map<int ,Fuel_up> fuel_ups;
map<int ,Ring> rings;
Segment lives[3];
Segment points[6];

int no_fuel_ups = 500;
int no_grounds = 100;
int no_volcanoes = 20;
int no_rings = 500;
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
Timer bomb_timer(0.2);
Timer enemy_missile_timer(5);
Timer t60(1.0 / 60);
Timer parachute_timer(5);

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
    else if(view == 4)
    {
        fov -= float(y);
        if(fov<10.0f)
            fov = 10.0f;
        else if(fov > 45.0f)
            fov = 45.0f;
        reset_screen();
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
        //up = glm::vec3(0, cos(glm::radians(plane.angle_x)), -sin(glm::radians(plane.angle_x)));
        up = glm::vec3(0, 1, 0);
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
        int dir = -1;
        xscaler = sin(glm::radians(plane.angle_y));
        yscaler = sin(glm::radians(plane.angle_x));
        zscaler = cos(glm::radians(plane.angle_y));
        eye = plane.front;
        target.x = plane.front.x + (breadth/2)*glm::normalize(plane.front - plane.back).x;
        target.y = plane.front.y + (breadth/2)*yscaler;
        target.z = plane.front.z + (breadth/2)*glm::normalize(plane.front - plane.back).z;
        up = glm::vec3(-(sin(glm::radians(plane.angle_z)))*cos(glm::radians(-plane.angle_y)), cos(glm::radians(plane.angle_z)),-(sin(glm::radians(plane.angle_z)))*sin(glm::radians(-plane.angle_y)));
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
    for(map<int, Ground>::iterator i = grounds.begin(); i!=grounds.end(); i++)
        i->second.draw(VP);
    for(int i=0;i<volcanoes.size();i++)
        volcanoes[i].draw(VP);
    plane.draw(VP);
    sea.draw(VP);
    marker.draw(VP, plane.position);
    if(view==4)
        crosshair.draw(projection_normal*Matrices.view);
    for(int i=0;i<missiles.size();i++)
        missiles[i].draw(VP);
    for(int i=0;i<enemy_missiles.size();i++)
        enemy_missiles[i].draw(VP);
    for(int i=0;i<bombs.size();i++)
        bombs[i].draw(VP);
    for(map<int, Fuel_up>::iterator i = fuel_ups.begin(); i!=fuel_ups.end(); i++)
        i->second.draw(VP);
    for(map<int, Ring>::iterator i = rings.begin(); i!=rings.end(); i++)
        i->second.draw(VP);
    for(int i=0;i<parachutes.size();i++)
        parachutes[i].draw(VP);
    speed_ind.draw(projection_normal, plane.move_speed);
    alt_ind.draw(projection_normal, plane.position.y);
    fuel_ind.draw(projection_normal, plane.fuel);
    gps.draw(projection_normal, plane.angle_y, glm::normalize(glm::vec3(checkpoint->second.position.x - plane.position.x, 0, checkpoint->second.position.z - plane.position.z)), glm::normalize(glm::vec3(-face.x, 0, -face.z)));
    lives[0].draw(projection_normal, 10);
    for(int i=2;i>=1;i--)
    {
        lives[i].draw(projection_normal, (plane.lives/(int)pow(10, 3-i-1))%10);
    }
    points[0].draw(projection_normal, 11);
    for(int i=5;i>=1;i--)
    {
        points[i].draw(projection_normal, (plane.points/(int)pow(10, 6-i-1))%10);
    }
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
    int right = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    
    if (f_cam) {
        fov = 45.0f;
        reset_screen();
        view = 1;
    }
    if(top_cam){
        fov = 45.0f;
        reset_screen();
        view = 2;
    }
    if(tower_cam){
        fov = 45.0f;
        reset_screen();
        view = 3;
    }
    if(fps_cam){
        fov = 45.0f;
        reset_screen();
        view = 4;
    }
    if(heli_cam){
        fov = 45.0f;
        reset_screen();
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
        missiles.push_back(Missile(plane.front.x, plane.front.y, plane.front.z, plane.front - plane.back, 50));
    }
    if(right == GLFW_PRESS && bomb_timer.processTick()){
        bombs.push_back(Bomb(plane.position.x, plane.position.y - plane.box.height/2, plane.position.z));
    }
}

void tick_elements() {
    if(plane.lives == 0)
    {
        printf("GAME OVER!\nYour lives got over!");
        quit(window);
    }
    if(plane.position.y < -2000.0f)
    {
        printf("GAME OVER!You drowned!!");
        quit(window);
    }
    if(plane.fuel<=0)
    {
        printf("GAME OVER!\nYou ran out of fuel!!");
        quit(window);
    }
    plane.tick();
    sea.tick();
    crosshair.tick(plane.front + glm::normalize(plane.front - plane.back)*200.0f, plane.angle_y, plane.angle_z);
    marker.set_position(checkpoint->second.position.x, checkpoint->second.box.height/2 + checkpoint->second.position.y, checkpoint->second.position.z);
    for(map<int, Ground>::iterator i=grounds.begin(), next=i; i!=grounds.end(); i=next)
    {
        next++;
        if(i->first==checkpoint->first && enemy_missile_timer.processTick())
        {
            enemy_missiles.push_back(i->second.create_missile(plane.position));
        }
        for(int j=0;j<missiles.size();)
        {
            if(detector.cuboid_cylinder_collision(i->second.box, missiles[j].box))
            {
                missiles.erase(missiles.begin() + j);
                if(i->second.hit())
                {
                    if(i->first == checkpoint->first)
                        checkpoint++;           //finish game if all over add karna hai 
                    plane.points += 10;
                    grounds.erase(i);
                }
                break;
            }
            else
                j++;
        }
        for(int j=0;j<bombs.size();)
        {
            if(detector.cuboid_cylinder_collision(i->second.box, bombs[j].box) || (bombs[j].position.y <= -2000 && glm::length(glm::vec3(i->second.position.x, 0, i->second.position.z) - glm::vec3(bombs[j].position.x, 0, bombs[j].position.z)) < bombs[j].impact_radius))
            {
                bombs.erase(bombs.begin() + j);
                if(i->second.hit())
                {
                    if(i->first == checkpoint->first)
                        checkpoint++;           //finish game if all over add karna hai 
                    plane.points += 10;
                    grounds.erase(i);
                }
                break;
            }
            else
                j++;
        }
        if(detector.cuboid_cylinder_collision(i->second.box, plane.box))
        {
            printf("GAME OVER!\nYou flew straight in the enemy building!");
            quit(window);
        }
    }
    for(int i=0;i<volcanoes.size();i++)
    {
        if(detector.Ycircular_proximity(glm::vec3(plane.position.x, 0, plane.position.z), glm::vec3(volcanoes[i].position.x, 0, volcanoes[i].position.z), volcanoes[i].radius))
        {
            printf("GAME OVER!\nYou flew right above a volcano!!");
            quit(window);
        }
    }
    for(int i=0;i<missiles.size();)
    {
        missiles[i].tick();
        if(missiles[i].ttl.processTick())
        {
            missiles.erase(missiles.begin() + i);
        }
        else
            i++;
    }
    for(int i=0;i<enemy_missiles.size();)
    {
        enemy_missiles[i].tick();
        if(detector.cuboid_cylinder_collision(enemy_missiles[i].box, plane.box))
        {
            enemy_missiles.erase(enemy_missiles.begin() + i);
            plane.lives--;
        }
        if(enemy_missiles[i].ttl.processTick())
        {
            enemy_missiles.erase(enemy_missiles.begin() + i);
        }
        else
            i++;
    }
    for(int i=0;i<bombs.size();)
    {
        bombs[i].tick();
        if(bombs[i].position.y < -2100)
            bombs.erase(bombs.begin() + i);
        else
            i++;
    }
    for(map<int, Fuel_up>::iterator i=fuel_ups.begin(), next=i; i!=fuel_ups.end(); i=next)
    {
        next++;
        if(detector.cuboid_cylinder_collision(i->second.box, plane.box))
        {
            fuel_ups.erase(i);
            plane.update_fuel();
        }
    }
    for(map<int, Ring>::iterator i=rings.begin(), next=i;i!=rings.end(); i=next)
    {
        next++;
        if(detector.circle2DZ_line_collision(plane.front, plane.back, i->second.position, i->second.radius))
        {
            plane.points += 5;
            rings.erase(i);
        }
    }

    if(parachute_timer.processTick())
    {
        int no = rand()%21;
        for(int i=0;i<no;i++)
            parachutes.push_back(Parachute(-world_width/10000 + rand()%(int)(2*world_width/10000), 2000.0f, -world_breadth/10000 + rand()%(int)(2*world_breadth/10000)));
    }
    for(int i=0;i<parachutes.size();)
    {
        int flag=0;
        parachutes[i].tick();
        if(parachutes[i].position.y < -2000)
        {
            parachutes.erase(parachutes.begin() + i);
            flag++;   
        }
        if(!flag)
        {
            for(int j=0;j<missiles.size();)
            {
                if(detector.cuboid_cylinder_collision(parachutes[i].box, missiles[j].box))
                {
                    plane.points += 20;
                    missiles.erase(missiles.begin() + j);
                    parachutes.erase(parachutes.begin() + i);
                    flag++;
                }
                else
                    j++;
            }
        }
        if(flag==0)
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
        grounds.insert(make_pair(i, Ground(-world_width/30000 + rand()%(int)(2*world_width/30000), -1749, -world_breadth/30000 + rand()%(int)(2*world_breadth/30000))));
    }
    checkpoint = grounds.begin();
    for(int i=0;i<no_volcanoes;i++)
    {
        volcanoes.push_back(Volcano(-world_width/30000 + rand()%(int)(2*world_width/30000), -1749, -world_breadth/30000 + rand()%(int)(2*world_breadth/30000)));
    }
    for(int i=0;i<no_fuel_ups;i++)
    {
        fuel_ups.insert(make_pair(i, Fuel_up(-world_width/30000 + rand()%(int)(2*world_width/30000), rand()%1000, -world_breadth/30000 + rand()%(int)(2*world_breadth/30000))));
    }
    for(int i=0;i<no_rings;i++)
    {
        rings.insert(make_pair(i, Ring(-world_width/30000 + rand()%(int)(2*world_width/30000), rand()%1000, -world_breadth/30000 + rand()%(int)(2*world_breadth/30000))));
    }
    for(int i=0;i<3;i++)
    {
        lives[i] = Segment(0, 0, 0);
        lives[i].set_position(-50 + i*1.1*(lives[i].box.width), 350, -1000);
    }
    for(int i=0;i<6;i++)
    {
        points[i] = Segment(0, 0, 0);
        points[i].set_position(150 - (6-i)*1.1*lives[i].box.width, -350, -1000);
    }
    plane = Plane(0, 0, 0);
    sea = Sea(0.0f, -2000.0f, 0.0f);
    speed_ind = Speed_ind(0, 0, 0);
    speed_ind.set_position(-250, -350, -1000);
    alt_ind = Alt_ind(0, 0, 0);
    alt_ind.set_position(250, -350, -1000);
    fuel_ind = Fuel_ind(0, 0, 0);
    fuel_ind.set_position(250, 300, -1000);
    gps = Gps(0, 0, 0);
    gps.set_position(-250, 300, -1000);
    marker = Marker(0, 0, 0);
    crosshair = Crosshair(0, 0, 0);
    detector = Collisions_detector();
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    projection_normal = glm::perspective(glm::radians(45.0f), height/float(width), 0.1f, world_breadth/4);


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

// bool detect_collision(bounding_box_t a, bounding_box_t b) {
//     return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
//            (abs(a.y - b.y) * 2 < (a.height + b.height));
// }

void reset_screen() {
    Matrices.projection = glm::perspective(glm::radians(fov), height/width, 0.1f, world_breadth/4);
}
