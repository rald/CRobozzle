#include <stdio.h>
#include <math.h>
#include <glfw.h>

#include <gl2d.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480



int main(int argc, char *argv[]) {

    glScreenInit( SCREEN_WIDTH, SCREEN_HEIGHT );
    glfwSwapInterval( 1 );


    while(!quit)
    {
        glClearScreen();

        glBlendMode( GL2D_SOLID );
        glBoxFilled( 400, 40, 639, 479, GL2D_RGBA( 0,0,0,0 ) );

        glLine( 620, 290, 620, 425, GL2D_RGBA( 255, 255, 0, 255 ) );

        glBox( 400, 40, 639, 479, GL2D_RGBA( 220, 255, 55, 0 ) );

        glTriangle( 480, 100, 630, 50, 560, 200, GL2D_RGBA(25,2,255,255) );
        glTriangleFilled( 490, 105, 610, 60, 590, 135, GL2D_RGBA(255,255,255,255) );

        float TimeStart = glfwGetTime();
        while( (glfwGetTime() - TimeStart) < (1.0/60.0) ){};

        glfwSwapBuffers();

        finished = glfwGetKey( GLFW_KEY_ESC )| !glfwGetWindowParam( GLFW_OPENED );
    }

    glfwTerminate();

    return 0;
}

