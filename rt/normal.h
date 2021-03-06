#ifndef NORMAL_H
#define NORMAL_H

class Normal {
private:
  float x, y, z;
public:
  /* Init normal */
  Normal(float x, float y, float z);
  Normal();

  float getX();
  float getY();
  float getZ();

  /* Add two normals */
  Normal operator+(Normal n1);

  /* Subtract two normals */
  Normal operator-(Normal n1);

  /* Assign normal */
  void operator=(Normal n);
};

#endif
