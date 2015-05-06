#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <Box2D/Box2D.h>
#include "drawboard.h"
#include "startscreen.h"

/*
 * Greater the number, the lesser the sensitivity.
 * Must be greater than 0.
 */
#define MOUSE_SENSITIVITY 4.0
#define BOARD_ROTATION_LIMIT 10.0

#define INIT_WINDOW_WIDTH 1024
#define INIT_WINDOW_HEIGHT 768

double rotate_y = 0, rotate_x = 0;
GLuint wood_t, start_t;
b2World world(b2Vec2(0, 0));    // Create Box2D world with 0 gravity

void specialKeys(int key, int x, int y)
{
        if ((key == GLUT_KEY_RIGHT) && rotate_y <= 10)
                rotate_y += 5;
        else if ((key == GLUT_KEY_LEFT) && rotate_y >= -10)
                rotate_y -= 5;
        else if ((key == GLUT_KEY_UP) && rotate_x >= -10)
                rotate_x -= 5;
        else if ((key == GLUT_KEY_DOWN) && rotate_x <= 10)
                rotate_x += 5;
        glutPostRedisplay();
}

void controlBoard(int current_x, int current_y)
{
        static int previous_x = (INIT_WINDOW_WIDTH / 2);
        static int previous_y = (INIT_WINDOW_HEIGHT / 2);
        float delta_x, delta_y;

        /*
         * y coordinate provided has origin at top-left and not bottom-left.
         */
        current_y = glutGet(GLUT_WINDOW_HEIGHT) - current_y;
        delta_x = (float) (current_x - previous_x) / MOUSE_SENSITIVITY;
        delta_y = (float) (current_y - previous_y) / MOUSE_SENSITIVITY;

        if ((rotate_y + delta_x) < BOARD_ROTATION_LIMIT &&
            (rotate_y + delta_x) > -BOARD_ROTATION_LIMIT)
                rotate_y += delta_x;
        if ((rotate_x - delta_y) < BOARD_ROTATION_LIMIT &&
            (rotate_x - delta_y) > -BOARD_ROTATION_LIMIT)
                rotate_x -= delta_y;

        previous_x = current_x;
        previous_y = current_y;
}

void display()
{
        static int start = 0;

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluPerspective(5.0, w/h, 0.1, 100.0);
        if (!start) {
                gluLookAt(0, 0, 50, 0, 0, 40, 0, 1, 0);
                start = displayStart(start_t);
        } else {
                glutPassiveMotionFunc(controlBoard);
                gluLookAt(0, 0, 80, 0, 0, 60, 0, 1, 0);
                glRotatef(rotate_x, 1.0, 0.0, 0.0);
                glRotatef(rotate_y, 0.0, 1.0, 0.0);
                drawBoard(wood_t);
        }
        glutSwapBuffers();
}

void initTextures()
{
        wood_t = SOIL_load_OGL_texture
                (
                        "res/woodtexture.jpg",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                        );
        start_t = SOIL_load_OGL_texture
                (
                        "res/startscreen.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                        );
}

void step()
{
        if (glutGet(GLUT_ELAPSED_TIME) % (1000/60)) {
                world.Step(1.0f/60.0f, 6, 2);
                glutPostRedisplay();
        }
}

int main(int argc, char *argv[])
{
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Labyrinth");
        glutDisplayFunc(display);
        glutSpecialFunc(specialKeys);
        glEnable(GL_DEPTH_TEST);
        initTextures();
        glutIdleFunc(step);
        glutMainLoop();
        return 0;
}
