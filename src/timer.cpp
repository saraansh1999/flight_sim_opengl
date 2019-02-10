#include "timer.h"
#include <GLFW/glfw3.h>

Timer::Timer(double interval) {
    prev = glfwGetTime();
    cur = glfwGetTime();
    this->interval = interval;
}

bool Timer::processTick() {
    cur = glfwGetTime();
    if (cur - prev >= interval) {
        prev = cur;
        return true;
    } else
        return false;
}
