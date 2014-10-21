/*-----------------------------------------------------------*
 * plane.c                                                   *
 * Sample OpenGL application to draw a plane with texture.   *
 * Author: MH                                                *
 * Updated by: HL             Date: Sept. 25, 2014;          *
 * Note:                                                     *
 *      compile and build                                    *
 *      g++   -lGL -lglut -lGLU -lm                          *
 *-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "readBMP.h"

#define PI 3.141592653589793238462643

void display(void);
void createWorld(void);
void createObjects(void);
void createLighting(void);
void createCamera(void);
void drawAxes(GLfloat length);
void drawPlane(GLfloat width, GLfloat height, GLfloat depth);
void handleKeyboard(unsigned char key, int x, int y);
void handleSpecialKeyboard(int key, int x, int y);
void handleVisibility(int visible);
void handleIdle(void);
void printUsage(void);
int  LoadBmpTexture(char * filename, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

// Globals
char *textureFile = "grape12.bmp";
GLuint textureID;
double verticalRotation = 60;
double horizontalRotation = 0;

/*----------------------------------------------*
 * main
 * Main entry function for the application.
 *----------------------------------------------*/
int main(int argc, char **argv)
{
    int i;

    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-t") == 0)
        {
            if(i+1 >= argc)
                printUsage();
            textureFile = argv[i+1];
        }
    }

    // OpenGL initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGL: Textured Plane");
    glutDisplayFunc(display);
    glutVisibilityFunc(handleVisibility);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeyboard);

    // Create world
    createWorld();

    // main loop
    glutMainLoop();
    return 0;
}

/**
 * createWorld
 * Creates the world and its objects.
 */
void createWorld(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);

    glLineWidth(1.0);
    glPointSize(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFrontFace(GL_CW);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_COLOR_MATERIAL);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    LoadBmpTexture(textureFile, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

/**
 * display
 * This is the display function. Creates lighting, camera, and geometry.
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    createCamera();
    createLighting();
    createObjects();
    glPopMatrix();

    glutSwapBuffers();
}

/**
 * createObjects
 * Creates the world objects.
 */
void createObjects(void)
{
   int z;

   drawAxes(500);

   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, textureID);

   for (z = -100; z <= 100; z += 10) {
      drawPlane(100.0, 100.0, z);
   }

   glDisable(GL_TEXTURE_2D);
}

/**
 * drawAxes
 * Draws the x, y, and z axes.
 */
void drawAxes(GLfloat length)
{
    int i;
    // axes
    GLfloat origin[3] = {0, 0, 0};
    GLfloat xpos[3] = { 1,  0,  0};
    GLfloat xneg[3] = {-1,  0,  0};
    GLfloat ypos[3] = { 0,  1,  0};
    GLfloat yneg[3] = { 0, -1,  0};
    GLfloat zpos[3] = { 0,  0,  1};
    GLfloat zneg[3] = { 0,  0, -1};

    glLineWidth(2.0);
    glBegin(GL_LINES);
    // x-axis (red)
    glColor3f(1, 0, 0);
    glVertex3fv(origin);
    glVertex3f(xpos[0] * length,
               xpos[1] * length,
               xpos[2] * length);
    glVertex3fv(origin);
    glVertex3f(xneg[0] * length,
               xneg[1] * length,
               xneg[2] * length);
    // y-axis (green)
    glColor3f(0, 1, 0);
    glVertex3fv(origin);
    glVertex3f(ypos[0] * length,
               ypos[1] * length,
               ypos[2] * length);
    glVertex3fv(origin);
    glVertex3f(yneg[0] * length,
               yneg[1] * length,
               yneg[2] * length);
    // z-axis (blue)
    glColor3f(0, 0, 1);
    glVertex3fv(origin);
    glVertex3f(zpos[0] * length,
               zpos[1] * length,
               zpos[2] * length);
    glVertex3fv(origin);
    glVertex3f(zneg[0] * length,
               zneg[1] * length,
               zneg[2] * length);
    glEnd();
}

/**
 * drawPlane
 * Draws a plane with the given dimensions.
 */
void drawPlane(GLfloat width, GLfloat height, GLfloat depth)
{
    int i;
    GLfloat normal[3] = {-1.0, 0.0, 0.0};
    GLfloat verts[4][3] = {
        {-1.0, -1.0, 0.0},
        { 1.0, -1.0, 0.0},
        { 1.0,  1.0, 0.0},
        {-1.0,  1.0, 0.0}
    };
    GLfloat texcoords[4][3] = {
        { 0.0,  0.0, 0.0},
        { 1.0,  0.0, 0.0},
        { 1.0,  1.0, 0.0},
        { 0.0,  1.0, 0.0}
    };

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    for(i = 0; i < 4; i++)
    {
        verts[i][0] *= width;
        verts[i][1] *= height;
        verts[i][2] += depth;

        glNormal3fv(&normal[0]);
        glTexCoord3fv(&texcoords[i][0]);
        glVertex3fv(&verts[i][0]);
    }
    glEnd();
}

/**
 * createLighting
 * Creates the lighting for the scene.
 */
void createLighting(void)
{
    GLfloat ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat pos0[] = {1.0, 1.0, 0.0, 0.0};
    GLfloat dif0[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat pos1[] = {5.0, -5.0, 0.0, 0.0};
    GLfloat dif1[] = {0.4, 0.4, 0.4, 1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

/**
 * createCamera
 * Creates the camera for the scene.
 */
void createCamera(void)
{
    GLint viewport[4];

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0,    // field of view
                   1.0,     // aspect ratio
                   0.1,     // near
                   1000.0); // far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(300 * cos(horizontalRotation * PI/180) * sin(verticalRotation * PI / 180),
              300 * cos(verticalRotation * PI/180),
              300 * sin(horizontalRotation * PI/180) * sin(verticalRotation * PI / 180),
              0.0, 0.0, 0.0,    // focus
              0.0, 1.0, 0.0);   // up

    horizontalRotation += 0.2;
}

/**
 * handleKeyboard
 * Handles keyboard input.
 */
void handleKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: // escape key
        case 'Q':
        case 'q':
            exit(0);
            break;

        default:
            // nothing
            break;
    }
}

/**
 * handleSpecialKeyboard
 * Handles special keys from the keyboard.
 */
void handleSpecialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            verticalRotation -= 2;
            break;

        case GLUT_KEY_DOWN:
            verticalRotation += 2;
            break;
    }
}

/**
 * handleVisibility
 * Handle visibility changes.
 */
void handleVisibility(int visible)
{
    if(visible == GLUT_VISIBLE)
    {
        glutIdleFunc(handleIdle);
    }
    else
    {
        glutIdleFunc(NULL);
    }
}

/**
 * handleIdle
 * Handles idle events.
 */
void handleIdle(void)
{
    glutPostRedisplay();
}

/**
 * printUsage
 * Prints the program usage to the user.
 */
void printUsage(void)
{
    printf("Usage: plane [-t <texturefile>]\n");
    printf("     -t <texturefile>    Filename for the texture.\n");
    printf("Controls:\n");
    printf("     q: Quit\n");
    printf("  left: Rotate left\n");
    printf(" right: Rotate right\n");
    printf("    up: Rotate up\n");
    printf("  down: Rotate down\n");
    exit(0);
}

/**
 * LoadBmpTexture
 * Loads a texture from a BMP file.
 */
static Image * image = NULL;
int LoadBmpTexture(char * filename, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
    // read the texture bits
    if(image == NULL)
    {
        image = (Image *) malloc(sizeof(Image));
        if(image == NULL)
        {
            fprintf(stderr, "Error allocating memory for image.\n");
            return 0;
        }

        if(!ImageLoad(filename, image))
        {
            fprintf(stderr, "Error loading image: %s\n", filename);
            return 0;
        }
    }

    // set wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    // set filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);

/*
    // generate mipmap (if necessary)
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        magFilter == GL_LINEAR_MIPMAP_LINEAR ||
        magFilter == GL_LINEAR_MIPMAP_NEAREST)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
*/
    return 1;
}

