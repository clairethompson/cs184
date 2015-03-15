#ifndef BRDF_H
#define BRDF_H

#include "color.h"
class BRDF {
  Color ka, kd, ks, kr;
  float sp;

  public:
    BRDF(Color a, Color d, Color s, float sp, Color r);
    BRDF();

    /* Getter Functions */
    Color getKA();
    Color getKD();
    Color getKS();
    Color getKR();
    float getSP();
};

#endif