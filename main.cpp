#include <string>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <float.h>
#include <cmath>
#include "FreeImage.h"

#include "point.h"
#include "color.h"
#include "vector.h"
#include "camera.h"
#include "light.h"
#include "matrix.h"
#include "shape.h"
#include "BRDF.h"
#include "tests.h"
#include "ellipsoid.h"
#include "ray.h"


#define CAMERA "cam"
#define SPHERE "sph"
#define TRIANGLE "tri"
#define OBJ_FILE "obj"
#define POINT_LIGHT "ltp"
#define DIRECT_LIGHT "ltd"
#define AMB_LIGHT "lta"
#define MATERIAL "mat"
#define TRANSLATE "xft"
#define ROTATE "xfr"
#define SCALE "xfs"

#define BPP 24
// Raytracer recursive depth limit
#define MAX_DEPTH 1


/* RayTracer Methods */
Color PhongShading(Light l, BRDF b, LocalGeo g);
Color RayTrace(Ray r, int depth);

/* Globals */
Camera camera = Camera();
std::vector<Light> lights;
std::vector<Shape *> shapes;
float WIDTH = 1000;
float HEIGHT = 500;


int main(int argc, char const *argv[])
{
  int count;

  count = 0;

  BRDF f = BRDF();

  /* Set up image output -- using FreeImage Library */
  FreeImage_Initialise();
  FIBITMAP * bitmap = FreeImage_Allocate(WIDTH, HEIGHT, BPP);
  RGBQUAD color;
  if (!bitmap)
    exit(1);

  /* Parse all inputs */
  while (count != argc) {
    if (strcmp(argv[count], "test") == 0) {
      Tests t = Tests();
      t.checkAll();
      exit(0);
    } else if (strcmp(argv[count], CAMERA) == 0) {
      Point eye = Point(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      count += 3;
      Point ll = Point(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      count += 3;
      Point lr = Point(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      count += 3;
      Point ul = Point(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      Point ur = Point(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      count += 3;
      camera = Camera(eye, ll, lr, ul, ur);
    } else if (strcmp(argv[count], SPHERE) == 0) {
      float cx = std::stof(argv[count + 1]);
      float cy = std::stof(argv[count + 2]);
      float cz = std::stof(argv[count + 3]);
      float rad = std::stof(argv[count + 4]);
      Point center = Point(cx, cy, cz);
      
      Ellipsoid sphere = Ellipsoid(center, rad, f);
      shapes.push_back(&sphere);

      count += 4;
    } else if (strcmp(argv[count], TRIANGLE) == 0) {
      Point a = Point(std::stof(argv[count+1]), std::stof(argv[count+2]), std::stof(argv[count+3]));
      Point b = Point(std::stof(argv[count+4]), std::stof(argv[count+5]), std::stof(argv[count+6]));
      Point c = Point(std::stof(argv[count+7]), std::stof(argv[count+8]), std::stof(argv[count+9]));

      // Shape tri = Shape(Point(), 0.0, a, b, c, f, 0);

      //TODO: implement triangle class fo dis shit

      // shapes.push_back(tri);
      
      count += 9;
    } else if (strcmp(argv[count], OBJ_FILE) == 0) {

    } else if (strcmp(argv[count], POINT_LIGHT) == 0) {

      Light pl = Light(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), std::stof(argv[count + 4]), std::stof(argv[count + 5]), std::stof(argv[count + 6]), std::stof(argv[count + 7]));
      count += 7;

      lights.push_back(pl);

    } else if (strcmp(argv[count], DIRECT_LIGHT) == 0) {
      
      Light dl = Light(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), std::stof(argv[count + 4]), std::stof(argv[count + 5]), std::stof(argv[count + 6]));
      count += 6;

      lights.push_back(dl);
    } else if (strcmp(argv[count], AMB_LIGHT) == 0) {
      
      Light al = Light(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      count += 3;

      lights.push_back(al);

    } else if (strcmp(argv[count], MATERIAL) == 0) {
      Color ka = Color(std::stof(argv[count+1]), std::stof(argv[count+2]), std::stof(argv[count+3]));
      Color kd = Color(std::stof(argv[count+4]), std::stof(argv[count+5]), std::stof(argv[count+6]));
      Color ks = Color(std::stof(argv[count+7]), std::stof(argv[count+8]), std::stof(argv[count+9]));
      float sp = std::stof(argv[count+10]);
      Color kr = Color(std::stof(argv[count+11]), std::stof(argv[count+12]), std::stof(argv[count+13]));
      f = BRDF(ka, kd, ks, sp, kr);
      count += 13;
    } else if (strcmp(argv[count], TRANSLATE) == 0) {
      Matrix trans = Matrix(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), 1);
      // PUT THIS MATRIX INTO SOME SORT OF LIST
      count += 3;
    } else if (strcmp(argv[count], ROTATE) == 0) {
      Matrix rot = Matrix(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), 2);
      // PUT THIS MATRIX INTO SOME SORT OF LIST
      count += 3;
    } else if (strcmp(argv[count], SCALE) == 0) {
      Matrix scale = Matrix(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), 3);
      // PUT THIS MATRIX INTO SOME SORT OF LIST
      count += 3;
    }
    count += 1;
  }

  WIDTH = camera.getWidth();
  HEIGHT = camera.getHeight();
  //Used to map pixels to image plane
  float r = camera.getWidth() / 2.0;
  float l = -1.0 * r;
  float t = camera.getHeight() / 2.0;
  float b = -1.0 * t;

  /* Shade each pixel */
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      //TODO: Rays? Shading?
      //Calculate the coordinate of pixel's position on image plane
      float u = l + (r - l)*(x + 0.5)/WIDTH;
      float v = b + (t - b)*(y + 0.5)/HEIGHT;
      //TODO: Figure out focal distance from eye to plane
      float d = 0.0;
      //TODO: Figure out which axes -d, u, v coordinate to in XYZ
      //-dw + uu + vv
      Vector r_dir (-d, u, v);
      Ray r (camera.getEye(), r_dir, 0.0, 0.0);
      
      Color fin = RayTrace(r, 0);
      color.rgbRed = fin.getR();
      color.rgbGreen = fin.getG();
      color.rgbBlue = fin.getB();
      FreeImage_SetPixelColor(bitmap, x, y, &color);
    }
  }

  /* Save bitmap to PNG format */
  if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)) {
    fprintf(stdout, "%s\n", "Image successfully saved!");
  }
  FreeImage_DeInitialise();

  return 0;
}

/* Return Color C of raytrace */
Color RayTrace(Ray r, int depth) {
  float ray_obj_dist;
  int i, j, k;
  Shape hitobject;

  float dist_max = FLT_MAX;
  std::vector<int>::size_type num_obj = shapes.size();
  std::vector<int>::size_type num_lights = lights.size();
  LocalGeo g;
  //Look through objects
  for (i = 0; i < num_obj; i++)
  {
    //ray_obj_dist = set distance of nearest intersection of ray & shapes[i]
    if (ray_obj_dist < dist_max) {
      dist_max = ray_obj_dist;
      hitobject = shapes[i];
    }
  }

  if (!hitobject) {
    //Set position var PT to nearest inersection point of R & I_S
    Color c (0.0,0.0,0.0); //Set color to black
    for (j = 0; j < num_lights; j++) {
      for (k = 0; k < num_obj; k++) {
        //If OBJ blocks light coming from light source to PT
          //Attenuate intesnsity by transmittivity
      }
      //Calc perceived color of obj at pt due to this light source
      Color block_color = PhongShading(lights[j], shapes[k]->getBRDF(), g);
      //Add to c
      c = c + block_color;
    }
    //Check recursive depth
    if (depth < MAX_DEPTH) {
      //Generate 2 rays: reflection  and refraction
      //Ray reflection();
      //Ray refraction()
      //c = c + RayTrace(reflection, depth+1) * intersect_shape.;
      //c = c + RayTrace(refraction, depth+1) * intersect_shape.;

    } else {
      //Set total color C to background color
    }
    return c;
  }
}

/* Phong Shade the colors */
Color PhongShading(Light l, BRDF b, LocalGeo g) {
  Vector normal (g.getNormal().getX(), g.getNormal().getX(), g.getNormal().getZ());
  Vector view;

  //TODO: Change light XYZ vals if Point or Directonal Light
  Vector light (l.getPoint().getX(), l.getPoint().getY(), l.getPoint().getZ());
  light.normalize();

  //Diffuse Color
  float diffuseColor = fmaxf(light.dot(normal), 0.0);

  //Specular Color
  Vector reflect = (normal * (2.0 * light.dot(normal))) - light;
  reflect.normalize();
  float specularColor = pow(fmax(view.dot(reflect), 0.0), b.getSP());

  return ((b.getKS() * specularColor) + (b.getKD() * diffuseColor) + b.getKA()) * l.getIntensity();
}
