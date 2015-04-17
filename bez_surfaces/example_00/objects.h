#include "eigen/Dense"
#include <vector>

using namespace std;
using namespace Eigen;

struct patch {
  Matrix4f mx, my, mz;
};

struct obj {
  int numPatches;
  vector<patch> patches;
};

struct triangle {
  RowVector3f p0,p1,p2;
  vector<RowVector3f> points;
  patch pat; // the patch that the triangle lays on

  /* steps is of the following form: 
    u1  v1
    u2  v2
    u3  v3
  */
  RowVector2f steps0, steps1, steps2;
};

void parse(char* filename);
void keyPressed (unsigned char key, int x, int y);
void specialPressed (int key, int x, int y);
Matrix3f flat_test(triangle tri, int depth);
void adaptiveDisplay();
float distToSurf(triangle tri, RowVector3f mid, float tempU, float tempV);
Matrix3f drawTri(triangle tri);
void assignTriSteps(triangle* tri, float s0u, float s0v, float s1u, float s1v, float s2u, float s2v);