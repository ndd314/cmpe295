/********************************************************************
OpenGL to display a bmp image. For reading bmp
image pixels, the library from
http://www.cs.ucdavis.edu/~amenta/s06/proj0/portrait.h
tml. has been reused

Note:
1. In addtion to display a bmp image, this program picks a region of the
image, modifies the RGB value and writes the pixels back to the buffer.
2. To compile build: gcc pixelrw.c readBMPV2.c -o main.o -lGL -lGLU -lglut
3. To run the executable: ./main test.bmp

**********************************************************************/

#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "readBMP.h"

int window;
Image *image;
int n,m;
char *filename;

//**************************************
//get image data
//**************************************
void getImage()
{
    image = (Image *) malloc(sizeof(Image));
    if (image == NULL) {
        printf("Error allocating space for the image");
        exit(-1);
    }
    if (!ImageLoad(filename, image)) {
        exit(-2);
    }
}

//***********************************
// reshape
//***********************************
void reshape(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat) n, 0.0, (GLfloat) m);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,width,height);
}

//***********************************
// pixel manipulations
//***********************************
void performPixelManipulation()
{
    int i;
    GLbyte *pbits = NULL;     //signed 8 bit integer
    GLbyte ptemp;             //
    GLint viewport[4];
    GLenum lastBuffer;
    unsigned long imageSize;

    unsigned int x = 64;
    unsigned int y = 64;
    unsigned int width = 220;   // width of the window
    unsigned int height =180;

    // get the viewport dimensions
    glGetIntegerv(GL_VIEWPORT, viewport);

    // calculate image size (3 components per pixel)
    imageSize = viewport[2] * viewport[3] * 3;

    // allocate memory
    pbits = (GLbyte *) malloc(imageSize);
    if(pbits == NULL)
    {
        printf("Error: couldn't allocate memory in %s!", __func__);
        exit(-1);
    }

    // read the pixels from the buffer
    glPixelStorei(GL_PACK_ALIGNMENT, 1);   //1 byte
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

    // save the buffer setting (used for restoration later)
    glGetIntegerv(GL_READ_BUFFER, &lastBuffer);

    // switch to front buffer and read pixels
    glReadBuffer(GL_FRONT);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pbits);

    // restore buffer
    glReadBuffer(lastBuffer);

    // run through the pixels for modification
    for(i = 0; i < imageSize; i = i+3)
    {
        //ptemp = 0.333*(pbits[i] + pbits[i+1] + pbits[i+2]);   //rgb to grey scale

        //if (ptemp < 0) ptemp = abs(ptemp);  //has to check this out
        //pbits[i+2] = 0; //b
        pbits[i+1] = 0; //g

        //pbits[i]   = ptemp;  //r
        //pbits[i+1] = ptemp; //g
        //pbits[i+2] = ptemp; //b
    }

    // save buffer setting
    glGetIntegerv(GL_READ_BUFFER, &lastBuffer);

    // write pixels
    glReadBuffer(GL_FRONT);
    glRasterPos2i(x, y); // offset
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pbits);

    // restore buffer
    glReadBuffer(lastBuffer);
}

//************************************
// Display
//************************************
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    //draw image
    glDrawPixels(n,m,GL_RGB, GL_UNSIGNED_BYTE, image->data);
    glFlush();

    // pixel manipulation
    performPixelManipulation();
    glFlush();
}

//*************************************
// main module
//*************************************
int main(int argc, char **argv)
{
    //Get the filename from command line
    switch (argc) {
        case 1:
            printf("Usage: program__name <image filename>\n");
            exit(-1);
        case  2:
            filename = argv[1];
            break;
        default:
            printf("usage: program_name <image filename>\n");
    }
    // Intialize GLUT
    glutInit(&argc, argv);
    // Confiure GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    getImage(image);  //get image size
    n = image->sizeX;
    m = image->sizeY;
    glutInitWindowSize(n, m);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Image Read and Write Demo");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // call back funtions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //Pass control to GLUT
    glutMainLoop();
    return(0);
}
