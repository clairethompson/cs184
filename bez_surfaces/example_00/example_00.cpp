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
#include <float.h>

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
float X_MID, Y_MID, Z_MID;
bool FILL = 0, FIRST_RENDER = 1, SMOOTH = 0;

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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  //----------------------- code to draw objects --------------------------

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


  float xMax = -FLT_MAX, yMax = -FLT_MAX, zMax = -FLT_MAX;
  float xMin = FLT_MAX, yMin = FLT_MAX, zMin = FLT_MAX;

  Matrix<float, 1, 4> uVect;
  Matrix<float, 1, 4> vVect;
  Matrix<float, 1, 4> uDeriv;
  Matrix<float, 1, 4> vDeriv;

  for (int i = 0; i < OBJECT.patches.size(); i++) {
    patch p = OBJECT.patches[i];

    MatrixXf xpoints((int)(ceil(1/u) + 1), (int)(ceil(1/v) + 1));
    MatrixXf ypoints((int)(ceil(1/u) + 1), (int)(ceil(1/v) + 1));
    MatrixXf zpoints((int)(ceil(1/u) + 1), (int)(ceil(1/v) + 1));
    MatrixXf xnorm((int)(ceil(1/u) + 1), (int)(ceil(1/v) + 1));
    MatrixXf ynorm((int)(ceil(1/u) + 1), (int)(ceil(1/v) + 1));
    MatrixXf znorm((int)(ceil(1/u) + 1), (int)(ceil(1/v) + 1));

    /* compute u * v points */
    for (int ucount = 0; ucount < xpoints.rows(); ucount++) {
      uVect(0, 0) = pow(u * ucount, 3);
      uVect(0, 1) = pow(u * ucount, 2);
      uVect(0, 2) = u * ucount;
      uVect(0, 3) = 1;

      uDeriv(0, 0) = 3 * pow(u * ucount, 2);
      uDeriv(0, 1) = 2 * u * ucount;
      uDeriv(0, 2) = 1;
      uDeriv(0, 3) = 0;

      for (int vcount = 0; vcount < xpoints.cols(); vcount++) {
      
        vVect(0, 0) = pow(v * vcount, 3);
        vVect(0, 1) = pow(v * vcount, 2);
        vVect(0, 2) = v * vcount;
        vVect(0, 3) = 1;

        vDeriv(0, 0) = 3 * pow(v * vcount, 2);
        vDeriv(0, 1) = 2 * v * vcount;
        vDeriv(0, 2) = 1;
        vDeriv(0, 3) = 0;

        // x(u, v) = U · MB · GBx · MBT · VT
        xpoints(ucount, vcount) = uVect * M * p.mx * M.transpose() * vVect.transpose();
        ypoints(ucount, vcount) = uVect * M * p.my * M.transpose() * vVect.transpose();
        zpoints(ucount, vcount) = uVect * M * p.mz * M.transpose() * vVect.transpose();

        xnorm(ucount, vcount) = uDeriv * M * p.mx * M.transpose() * vDeriv.transpose();
        ynorm(ucount, vcount) = uDeriv * M * p.my * M.transpose() * vDeriv.transpose();
        znorm(ucount, vcount) = uDeriv * M * p.mz * M.transpose() * vDeriv.transpose();

      }
    }


    /* for finding center of the object to rotate about */
    if (xpoints.maxCoeff() > xMax) { xMax = xpoints.maxCoeff(); }
    if (xpoints.minCoeff() < xMin) { xMin = xpoints.minCoeff(); }
    if (ypoints.maxCoeff() > yMax) { yMax = ypoints.maxCoeff(); }
    if (ypoints.minCoeff() < yMin) { yMin = ypoints.minCoeff(); }
    if (zpoints.maxCoeff() > zMax) { zMax = zpoints.maxCoeff(); }
    if (zpoints.minCoeff() < zMin) { zMin = zpoints.minCoeff(); }


    for (int j = 0; j < xpoints.rows() - 1; j++) {
      for (int k = 0; k < xpoints.cols() - 1; k++) {

        glBegin(GL_POLYGON);                         // draw rectangle 
        glVertex3f(xpoints(j + 1, k), ypoints(j + 1, k), zpoints(j + 1, k));   // bottom left corner 
        glNormal3f(xnorm(j + 1, k), ynorm(j + 1, k), znorm(j + 1, k));         // bottom left normal

        glVertex3f(xpoints(j, k), ypoints(j, k), zpoints(j, k));               // top left corner 
        glNormal3f(xnorm(j, k), ynorm(j, k), znorm(j, k));                     // top left normal 

        glVertex3f(xpoints(j, k + 1), ypoints(j,k + 1), zpoints(j,k + 1));     // top right corner
        glNormal3f(xnorm(j, k + 1), ynorm(j,k + 1), znorm(j,k + 1));           // top right normal

        glVertex3f(xpoints(j+1,k+1), ypoints(j+1,k+1), zpoints(j+1,k+1));      // bottom right corner
        glNormal3f(xnorm(j+1,k+1), ynorm(j+1,k+1), znorm(j+1,k+1));           // bottom right normal

        glEnd();

      }
    }
  }

  /* for finding center of the object to rotate about */
  X_MID = (xMax + xMin) / 2.0;
  Y_MID = (yMax + yMin) / 2.0;
  Z_MID = (zMax + zMin) / 2.0;

  if (FIRST_RENDER) {
    FIRST_RENDER = 0;
    glTranslatef(-X_MID, -Y_MID, -Z_MID);
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
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport size
  viewport.w = 600;
  viewport.h = 600;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184!");


  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"

  initScene();                   // quick function to set up scene



  glColor3f(1.0f,0.0f,0.0f);                   // setting the color to pure red 90% for the rect
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


  // GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  // GLfloat mat_shininess[] = { 50.0 };
  // GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

  // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  // glLightfv(GL_LIGHT0, GL_POSITION, light_position);


  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);

  GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 2.0, 2.0, 3.0, 0.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glShadeModel(GL_SMOOTH);


  glutDisplayFunc(myDisplay);    // function to run when its time to draw something
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialPressed);
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
  if (key == ' ') { exit(0); }
  if (key == '+') { glScalef(1.1, 1.1, 1.1); }
  if (key == '-') { glScalef(0.8, 0.8, 0.8); }
  if (key == 'w') { 
    if (FILL) {
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    FILL = !FILL; 
  }
  if (key == 's') { 
    if (SMOOTH) {
      glShadeModel(GL_FLAT);
    } else {
      glShadeModel(GL_SMOOTH);
    }
    SMOOTH = !SMOOTH; 
  }

}

void specialPressed(int key, int x, int y) {
  /* OBJ TRANSLATE */
  if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
    if (key == GLUT_KEY_LEFT) {
      glTranslatef(-0.01, 0.0, 0.0);
    } else if (key == GLUT_KEY_RIGHT) {
      glTranslatef(0.01, 0.0, 0.0);
    } else if (key == GLUT_KEY_UP) {
      glTranslatef(0.0, 0.01, 0.0);
    } else if (key == GLUT_KEY_DOWN) {
      glTranslatef(0.0, -0.01, 0.0);
    }
  } else {
    /* OBJ ROTATE */
    if (key == GLUT_KEY_LEFT) {
      glRotated(1, 0.0, X_MID, 0.0);
    } else if (key == GLUT_KEY_RIGHT) {
      glRotated(-1, 0.0, X_MID, 0.0);
    } else if (key == GLUT_KEY_UP) {
      glRotated(-1, Y_MID, 0.0, 0.0);
    } else if (key == GLUT_KEY_DOWN) {
      glRotated(1, Y_MID, 0.0, 0.0);
    }
  }
  
}

