#include <GL/glfw.h>
#include <stdio.h>
char titlestring[200];
void showFPS(int t0, int frames) {

    double t, fps;
    t = glfwGetTime();
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        sprintf(titlestring, "Din mamma (%.1f FPS)", fps);
        glfwSetWindowTitle(titlestring);
        t0 = t;
        frames = 0;
    }
    frames ++;
}
