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

void parse(char* filename);
void keyPressed (unsigned char key, int x, int y);