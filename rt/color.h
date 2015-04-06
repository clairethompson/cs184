#ifndef COLOR_H
#define COLOR_H

class Color
{
  public:
    float r,g,b;
    Color(float r, float g, float b);
    Color();

    /* Getter functions */
    float getR();
    float getG();
    float getB();

    /* Compute the dot product between c1 and c2 */
    float dotProduct(Color c1, Color c2);

    /* Add color with c1 */
    Color operator+(Color c1);

    /* Multiply color by scalar f*/
    Color operator*(float f);

    /* Multiply color by other color */
    Color operator*(Color c1);

    /* Assign color to c1 */
    void operator=(Color c1);
};

#endif