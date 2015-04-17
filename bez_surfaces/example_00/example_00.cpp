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
bool FILL = 0, FIRST_RENDER = 1, SMOOTH = 0, ADAPTIVE = 0;
Matrix4f M;


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
      
        // float tempXu = 0, tempYu = 0, tempZu = 0, total = 0;
        // float tempXv = 0, tempYv = 0, tempZv = 0;
        Matrix<float, 1, 3> tempVectu, tempVectv;


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

        tempVectu(0, 0) = uDeriv * M * p.mx * M.transpose() * vVect.transpose();
        tempVectu(0, 1) = uDeriv * M * p.my * M.transpose() * vVect.transpose();
        tempVectu(0, 2) = uDeriv * M * p.mz * M.transpose() * vVect.transpose();
        tempVectv(0, 0) = uVect * M * p.mx * M.transpose() * vDeriv.transpose();
        tempVectv(0, 1) = uVect * M * p.my * M.transpose() * vDeriv.transpose();
        tempVectv(0, 2) = uVect * M * p.mz * M.transpose() * vDeriv.transpose();

        tempVectu = tempVectu.cross(tempVectv);
        tempVectu.normalize();

        xnorm(ucount, vcount) = tempVectu(0, 0);
        ynorm(ucount, vcount) = tempVectu(0, 1);
        znorm(ucount, vcount) = tempVectu(0, 2);
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

void adaptiveDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  // cout << "in adap disp \n";

  float xMax = -FLT_MAX, xMin = FLT_MAX, yMax = -FLT_MAX, yMin = FLT_MAX, zMax = -FLT_MAX, zMin = FLT_MAX;

  for (int p = 0; p < OBJECT.patches.size(); p++) {

    // cout << "getting patch \n";
    patch pat = OBJECT.patches[p];

    // cout << "making triangle \n";
    
    triangle tri1, tri2;
    tri1.pat = pat;
    tri1.p0 << pat.mx(0, 0), pat.my(0, 0), pat.mz(0, 0);
    tri1.p1 << pat.mx(0, 3), pat.my(0, 3), pat.mz(0, 3);
    tri1.p2 << pat.mx(3, 0), pat.my(3, 0), pat.mz(3, 0);

    assignTriSteps(&tri1, 0, 0, 0, 1, 1, 0);

    tri2.p0 << pat.mx(0, 3), pat.my(0, 3), pat.mz(0, 3);
    tri2.p1 << pat.mx(3, 3), pat.my(3, 3), pat.mz(3, 3);
    tri2.p2 << pat.mx(3, 0), pat.my(3, 0), pat.mz(3, 0);
    assignTriSteps(&tri2, 0, 1, 1, 1, 1, 0);
    
    vector<triangle> temp;
    temp.push_back(tri1);
    temp.push_back(tri2);

    Matrix3f blah;
    for (int i = 0; i < temp.size(); i++) {
      blah = flat_test(temp[i], 5);
      xMin = min(xMin, blah(0, 0));
      yMin = min(yMin, blah(0, 1));
      zMin = min(zMin, blah(0, 2));
      xMax = min(xMax, blah(1, 0));
      yMax = min(yMax, blah(1, 1));
      zMax = min(zMax, blah(1, 2));
    }
  }

  X_MID = (xMax + xMin) / 2;
  Y_MID = (yMax + yMin) / 2;
  Z_MID = (zMax + zMin) / 2;

  // if (FIRST_RENDER) {
  //   FIRST_RENDER = 0;
  //   glTranslatef(-X_MID, -Y_MID, -Z_MID);
  // }

  glFlush();
  glutSwapBuffers();  
}


/* if flat enough, draw, else divide triangle and recurse */
Matrix3f flat_test(triangle tri, int depth) {
  float epsilon = u;

  bool mid0Pass = 1, mid1Pass = 1, mid2Pass = 2;

  RowVector3f triMid0 = (tri.p0 + tri.p1) / 2;
  RowVector3f triMid1 = (tri.p1 + tri.p2) / 2;
  RowVector3f triMid2 = (tri.p2 + tri.p0) / 2;

  float temp0U = (tri.steps0(0) + tri.steps1(0)) / 2;
  float temp0V = (tri.steps0(1) + tri.steps1(1)) / 2;
  float temp1U = (tri.steps1(0) + tri.steps2(0)) / 2;
  float temp1V = (tri.steps1(1) + tri.steps2(1)) / 2;
  float temp2U = (tri.steps2(0) + tri.steps0(0)) / 2;
  float temp2V = (tri.steps2(1) + tri.steps0(1)) / 2;

  if (distToSurf(tri, triMid0, temp0U, temp0V) > epsilon) {
    mid0Pass = 0;
  }
  if (distToSurf(tri, triMid1, temp1U, temp1V) > epsilon) {
    mid1Pass = 0;
  }
  if (distToSurf(tri, triMid2, temp2U, temp2V) > epsilon) {
    mid2Pass = 0;
  }

  vector<triangle> triangles; 
  
  if ((mid0Pass && mid1Pass && mid2Pass) || depth <= 0) {
    return drawTri(tri);
  } else if (!mid0Pass && mid1Pass && mid2Pass) {

    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = tri.p0;
    tri1.p1 = triMid0;
    tri1.p2 = tri.p2;
    assignTriSteps(&tri1, tri.steps0(0), tri.steps0(1),
                      temp0U, temp0V,
                      tri.steps2(0), tri.steps2(1));

    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = triMid0;
    tri2.p1 = tri.p1;
    tri2.p2 = tri.p2;
    assignTriSteps(&tri2, temp0U, temp0V,
                      tri.steps1(0), tri.steps1(1), 
                      tri.steps2(0), tri.steps2(1));

    triangles.push_back(tri1);
    triangles.push_back(tri2);
  } else if (mid0Pass && !mid1Pass && mid2Pass) {
    
    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = tri.p0;
    tri1.p1 = triMid1;
    tri1.p2 = tri.p2;
    assignTriSteps(&tri1, tri.steps0(0), tri.steps0(1),
                      temp1U, temp1V,
                      tri.steps2(0), tri.steps2(1));

    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = triMid1;
    tri2.p1 = tri.p0;
    tri2.p2 = tri.p1;
    assignTriSteps(&tri2, temp1U, temp1V,
                      tri.steps0(0), tri.steps0(1), 
                      tri.steps1(0), tri.steps1(1));

    triangles.push_back(tri1);
    triangles.push_back(tri2);
  } else if (mid0Pass && mid1Pass && !mid2Pass) {
    
    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = tri.p0;
    tri1.p1 = tri.p1;
    tri1.p2 = triMid2;
    assignTriSteps(&tri1, tri.steps0(0), tri.steps0(1),
                      tri.steps1(0), tri.steps1(1),
                      temp2U, temp2V);

    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = triMid2;
    tri2.p1 = tri.p1;
    tri2.p2 = tri.p2;
    assignTriSteps(&tri2, temp2U, temp2V,
                      tri.steps1(0), tri.steps1(1), 
                      tri.steps2(0), tri.steps2(1));

    triangles.push_back(tri1);
    triangles.push_back(tri2);
  } else if (!mid0Pass && !mid1Pass && mid2Pass) {
    
    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = triMid0;
    tri1.p1 = tri.p1;
    tri1.p2 = triMid1;
    assignTriSteps(&tri1, temp0U, temp0V,
                      tri.steps1(0), tri.steps1(1),
                      temp1U, temp1V);

    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = triMid0;
    tri2.p1 = tri.p1;
    tri2.p2 = triMid2;
    assignTriSteps(&tri2, temp0U, temp0V,
                      tri.steps1(0), tri.steps1(1),
                      temp2U, temp2V);

    triangle tri3;
    tri3.pat = tri.pat;
    tri3.p0 = tri.p0;
    tri3.p1 = triMid1;
    tri3.p2 = tri.p2;
    assignTriSteps(&tri3, tri.steps0(0), tri.steps0(1),
                      temp1U, temp1V, 
                      tri.steps2(0), tri.steps2(1));

    triangles.push_back(tri1);
    triangles.push_back(tri2);
    triangles.push_back(tri3);
  } else if (!mid0Pass && mid1Pass && !mid2Pass) {
    
    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = triMid0;
    tri1.p1 = tri.p1;
    tri1.p2 = triMid2;
    assignTriSteps(&tri1, temp0U, temp0V,
                       tri.steps1(0), tri.steps1(1),
                       temp2U, temp2V);

    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = triMid0;
    tri2.p1 = triMid2;
    tri2.p2 = tri.p0;
    assignTriSteps(&tri2, temp0U, temp0V,
                      temp2U, temp2V, 
                      tri.steps0(0), tri.steps0(1));

    triangle tri3;
    tri3.pat = tri.pat;
    tri3.p0 = triMid2;
    tri3.p1 = tri.p1;
    tri3.p2 = tri.p2;
    assignTriSteps(&tri3, temp2U, temp2V,
                      tri.steps1(0), tri.steps1(1), 
                      tri.steps2(0), tri.steps2(1));

    triangles.push_back(tri1);
    triangles.push_back(tri2);
    triangles.push_back(tri3);
  } else if (mid0Pass && !mid1Pass && !mid2Pass) {
    
    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = triMid2;
    tri1.p1 = tri.p0;
    tri1.p2 = triMid1;
    assignTriSteps(&tri1, temp2U, temp2V,
                      tri.steps0(0), tri.steps0(1),
                      temp1U, temp1V);

    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = triMid2;
    tri2.p1 = triMid1;
    tri2.p2 = tri.p2;
    assignTriSteps(&tri2, temp2U, temp2V,
                      temp1U, temp1V, 
                      tri.steps2(0), tri.steps2(1));

    triangle tri3;
    tri3.pat = tri.pat;
    tri3.p0 = triMid1;
    tri3.p1 = tri.p0;
    tri3.p2 = tri.p1;
    assignTriSteps(&tri3, temp1U, temp1V,
                      tri.steps0(0), tri.steps0(1), 
                      tri.steps1(0), tri.steps1(1));

    triangles.push_back(tri1);
    triangles.push_back(tri2);
    triangles.push_back(tri3);
  } else if (!mid0Pass && !mid1Pass && !mid2Pass) {
   
    triangle tri1;
    tri1.pat = tri.pat;
    tri1.p0 = tri.p0;
    tri1.p1 = triMid0;
    tri1.p2 = triMid2;
    assignTriSteps(&tri1, tri.steps0(0), tri.steps1(0), 
                      temp0U, temp0V,
                      temp2U, temp2V);
    
    triangle tri2;
    tri2.pat = tri.pat;
    tri2.p0 = tri.p1;
    tri2.p1 = triMid1;
    tri2.p2 = triMid0;
    assignTriSteps(&tri2, tri.steps1(0), tri.steps1(0), 
                temp1U, temp1V,
                temp0U, temp0V);

    triangle tri3;
    tri3.pat = tri.pat;
    tri3.p0 = tri.p2;
    tri3.p1 = triMid2;
    tri3.p2 = triMid1;
    assignTriSteps(&tri3, tri.steps2(0), tri.steps2(0), 
                temp2U, temp2V,
                temp1U, temp1V);

    triangle tri4;
    tri4.pat = tri.pat;
    tri4.p0 = triMid0;
    tri4.p1 = triMid1;
    tri4.p2 = triMid2;
    assignTriSteps(&tri4, temp0U, temp0V,
                          temp1U, temp1V,
                          temp2U, temp2V);

    triangles.push_back(tri1);
    triangles.push_back(tri2);
    triangles.push_back(tri3);
    triangles.push_back(tri4);
  } else {
    cout << "SOMETHING WENT VERY WRONG \n";
    exit(1);
  }
  
  vector<Matrix3f> temp;
  for (int i = 0; i < triangles.size(); i++) {
    temp.push_back(flat_test(triangles[i], depth - 1));
  }

  Matrix3f toReturn;
  toReturn(0, 0) = FLT_MAX;
  toReturn(0, 1) = FLT_MAX;
  toReturn(0, 2) = FLT_MAX;
  toReturn(1, 0) = -FLT_MAX;
  toReturn(1, 1) = -FLT_MAX;
  toReturn(1, 2) = -FLT_MAX;
  for (int i = 0; i < temp.size(); i++) {
    toReturn(0, 0) = min(toReturn(0, 0), temp[i](0, 0));
    toReturn(0, 1) = min(toReturn(0, 1), temp[i](0, 1));
    toReturn(0, 2) = min(toReturn(0, 2), temp[i](0, 2));
    toReturn(1, 0) = max(toReturn(1, 0), temp[i](1, 0));
    toReturn(1, 1) = max(toReturn(1, 1), temp[i](1, 1));
    toReturn(1, 2) = max(toReturn(1, 2), temp[i](1, 2));
  }

  return toReturn;

}

float distToSurf(triangle tri, RowVector3f mid, float tempU, float tempV){

  RowVector4f uVect;
  RowVector4f vVect;
  uVect << pow(tempU, 3), pow(tempU, 2), tempU, 1;
  vVect << pow(tempV, 3), pow(tempV, 2), tempV, 1;

  float tempX, tempY, tempZ;
  tempX = uVect * M * tri.pat.mx * M.transpose() * vVect.transpose();
  tempY = uVect * M * tri.pat.my * M.transpose() * vVect.transpose();
  tempZ = uVect * M * tri.pat.mz * M.transpose() * vVect.transpose();

  RowVector3f onSurface;
  onSurface << tempX, tempY, tempZ;

  return (mid - onSurface).norm();
}

/*
  draws triangle and returns matrix of the form
    minx  miny  minz
    maxx  maxy  maxz
    0     0     0
*/
Matrix3f drawTri(triangle tri) {
  // cout << "drawing \n";

  // uDeriv * M * p.mx * M.transpose() * vVect.transpose()
  RowVector3f p0 = tri.p0, p1 = tri.p1, p2 = tri.p2, tempUvect, tempVvect; 
  RowVector4f uVect, vVect, uDeriv, vDeriv;
  Matrix3f toReturn;
  toReturn(0, 0) = min(p0(0), min(p1(0), p2(0)));
  toReturn(0, 1) = min(p0(1), min(p1(1), p2(1)));
  toReturn(0, 2) = min(p0(2), min(p1(2), p2(2)));
  toReturn(1, 0) = max(p0(0), max(p1(0), p2(0)));
  toReturn(1, 1) = max(p0(1), max(p1(1), p2(1)));
  toReturn(1, 2) = max(p0(2), max(p1(2), p2(2)));

  // uVect << pow(tri.steps0(0), 3), pow(tri.steps0(0), 2), tri.steps0(0), 1;
  // vVect << pow(tri.steps0(1), 3), pow(tri.steps0(1), 2), tri.steps0(1), 1;
  // uDeriv << 3 * pow(tri.steps0(0), 2), 2 * tri.steps0(0), 1, 0;
  // vDeriv << 3 * pow(tri.steps0(1), 2), 2 * tri.steps0(1), 1, 0;


  // tempUvect << uDeriv * M * tri.pat.mx * M.transpose() * vVect.transpose(),
  //              uDeriv * M * tri.pat.my * M.transpose() * vVect.transpose(),
  //              uDeriv * M * tri.pat.mz * M.transpose() * vVect.transpose();
  // tempVvect << uVect * M * tri.pat.mx * M.transpose() * vDeriv.transpose(),
  //              uVect * M * tri.pat.my * M.transpose() * vDeriv.transpose(),
  //              uVect * M * tri.pat.mz * M.transpose() * vDeriv.transpose();

  glBegin(GL_POLYGON);
  glVertex3f(p0(0, 0), p0(0, 1), p0(0, 2));
  glVertex3f(p1(0, 0), p1(0, 1), p1(0, 2));
  glVertex3f(p2(0, 0), p2(0, 1), p2(0, 2));
  glEnd();

  return toReturn;
}

void assignTriSteps(triangle* tri, float s0u, float s0v, float s1u, float s1v, float s2u, float s2v){
  tri->steps0 << s0u, s0v;
  tri->steps1 << s1u, s1v;
  tri->steps2 << s2u, s2v;
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

  if (argc > 3 && strcmp(argv[3], "-a") == 0) { 
    cout << "ADAPTIVE\n";
    ADAPTIVE = 1; }


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


  M << -1,  3, -3, 1,
        3, -6,  3, 0,
       -3,  3,  0, 0, 
        1,  0,  0, 0;



  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);

  GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 0.0, 0.0, 3.0, 0.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);


  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"

  initScene();                   // quick function to set up scene



  glColor3f(1.0f,0.0f,0.0f);                   // setting the color to pure red 90% for the rect
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  glShadeModel(GL_SMOOTH);

  if (ADAPTIVE) {
    glutDisplayFunc(adaptiveDisplay);
  } else {
    glutDisplayFunc(myDisplay);    // function to run when its time to draw something    
  }
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
      glRotated(5, 0.0, X_MID, 0.0);
    } else if (key == GLUT_KEY_RIGHT) {
      glRotated(-5, 0.0, X_MID, 0.0);
    } else if (key == GLUT_KEY_UP) {
      glRotated(-5, Y_MID, 0.0, 0.0);
    } else if (key == GLUT_KEY_DOWN) {
      glRotated(5, Y_MID, 0.0, 0.0);
    }
  }
  
}

