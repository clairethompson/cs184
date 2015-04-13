// CS184 Simple OpenGL Example
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "eigen/Dense"
#include "objects.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

using namespace std;
using namespace Eigen;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;
obj OBJECT;
float u, v;

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                // loading the identity matrix for the screen

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {


  glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"

  //----------------------- code to draw objects --------------------------
  Matrix<float, 1, 4> uVect;
  Matrix<float, 1, 4> vVect;

  Matrix4f M;
  M(0, 0) = -1;
  M(0, 1) = 3;
  M(0, 2) = -3;
  M(0, 3) = 1;
  M(1, 0) = 3;
  M(1, 1) = -6;
  M(1, 2) = 3;
  M(1, 3) = 0;
  M(2, 0) = -3;
  M(2, 1) = 3;
  M(2, 2) = 0;
  M(2, 3) = 0;
  M(3, 0) = 1;
  M(3, 1) = 0;
  M(3, 2) = 0;
  M(3, 3) = 0;

  for (int i = 0; i < OBJECT.patches.size(); i++) {

    patch p = OBJECT.patches[i];
    MatrixXf xpoints((int)(floor(1/u) + 1), (int)(floor(1/v) + 1));
    MatrixXf ypoints((int)(floor(1/u) + 1), (int)(floor(1/v) + 1));
    MatrixXf zpoints((int)(floor(1/u) + 1), (int)(floor(1/v) + 1));


    // cout << xpoints.rows() << "\n";
    for (int ucount = 0; ucount < xpoints.rows(); ucount++) {
      uVect(0, 0) = pow(u * ucount, 3);
      uVect(0, 1) = pow(u * ucount, 2);
      uVect(0, 2) = u * ucount;
      uVect(0, 3) = 1;

      for (int vcount = 0; vcount < xpoints.cols(); vcount++) {
      
        vVect(0, 0) = pow(v * vcount, 3);
        vVect(0, 1) = pow(v * vcount, 2);
        vVect(0, 2) = v * vcount;
        vVect(0, 3) = 1;

        // x(u, v) = U · MB · GBx · MBT · VT
        xpoints(ucount, vcount) = uVect * M * p.mx * M.transpose() * vVect.transpose();
        ypoints(ucount, vcount) = uVect * M * p.my * M.transpose() * vVect.transpose();
        zpoints(ucount, vcount) = uVect * M * p.mz * M.transpose() * vVect.transpose();

      }
    }

    for (int j = 0; j < xpoints.rows() - 1; j++) {
      for (int k = 0; k < xpoints.cols() - 1; k++) {

        glColor3f(1.0f,0.0f,0.0f);                   // setting the color to pure red 90% for the rect
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glBegin(GL_POLYGON);                         // draw rectangle 
        glVertex3f(xpoints(j + 1, k), ypoints(j + 1, k), zpoints(j + 1, k));   // bottom left corner 
        glVertex3f(xpoints(j, k), ypoints(j, k), zpoints(j, k));               // top left corner 
        glVertex3f(xpoints(j, k + 1), ypoints(j,k + 1), zpoints(j,k + 1));     // top right corner
        glVertex3f(xpoints(j+1,k+1), ypoints(j+1,k+1), zpoints(j+1,k+1));      // bottom right corner
        glEnd();

      }
    }

  }

  glFlush();
  glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  // glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  parse(argv[1]);
  cout << OBJECT.numPatches << "\n";

  u = stof(argv[2]);
  v = u;


  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 600;
  viewport.h = 600;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184!");

  initScene();                   // quick function to set up scene

  glutKeyboardFunc(keyPressed);
  glutDisplayFunc(myDisplay);    // function to run when its time to draw something
  glutReshapeFunc(myReshape);    // function to run when the window gets resized
  glutIdleFunc(myFrameMove);     // function to run when not handling any other task
  glutMainLoop();                // infinite loop that will keep drawing and resizing and whatever else


  return 0;
}

void parse(char *filename) {

  ifstream inputFile;
  inputFile.open(filename);

  int numPat;
  inputFile >> numPat;

  vector<patch> patches;

  for (int i = 0; i < numPat; i++) {
    patch p;
    Matrix4f x, y, z;
    for (int k = 0; k < 4; k++) {
      string x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3;
      if (inputFile >> x0 >> y0 >> z0 >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3) {
        x(k, 0) = stof(x0);
        x(k, 1) = stof(x1);
        x(k, 2) = stof(x2);
        x(k, 3) = stof(x3);
        y(k, 0) = stof(y0);
        y(k, 1) = stof(y1);
        y(k, 2) = stof(y2);
        y(k, 3) = stof(y3);
        z(k, 0) = stof(z0);
        z(k, 1) = stof(z1);
        z(k, 2) = stof(z2);
        z(k, 3) = stof(z3);
      }
    }
    p.mx = x;
    p.my = y;
    p.mz = z;
    patches.push_back(p);
  }

  OBJECT.patches = patches;
  OBJECT.numPatches = numPat;

  cout << OBJECT.patches[0].mx << "\n";
}

void keyPressed (unsigned char key, int x, int y) {  
  if (key == 0x20) {  // space
    exit(0);
  } else if (key == 0x25) { // left arrow
    glTranslated(-0.01, 0.0, 0.0);
  } else if (key == 72) { // up arrow
    exit(0);
    glTranslated(0.0, 0.1, 0.0);
  } else if (key == 0x27) { // right arrow
    
  } else if (key == 0x28) { // down arrow

  }
}

