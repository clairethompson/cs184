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
  vector<RowVector3f> points;
  patch pat; // the patch that the triangle lays on

  /* steps is of the following form: 
    u1  v1
    u2  v2
    u3  v3
  */
  typedef Matrix<float, 2, 3> stepMat;
  stepMat steps;
};

void parse(char* filename);
void keyPressed (unsigned char key, int x, int y);
void specialPressed (int key, int x, int y);
void flat_test(triangle tri, int depth);
void adaptiveDisplay();
float distToSurf(triangle tri, RowVector3f mid, int i);
void drawTri(triangle tri);