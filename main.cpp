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
#include "triangle.h"
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
#define MAX_DEPTH 2


/* RayTracer Methods */
Color PhongShading(Vector normal, Vector light, Color light_c, BRDF b, LocalGeo g);
Color RayTrace(Ray r, int depth);
Vector reflect(Vector normal, Vector light);

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
  // Change defaults WIDTH X HEIGHT of output image
  WIDTH = 1000;
  HEIGHT = 1000;
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
      count += 3;
      Point ur = Point(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]));
      count += 3;
      camera = Camera(eye, ll, lr, ul, ur);
      fprintf(stdout, "%s\n", "camera");
    } else if (strcmp(argv[count], SPHERE) == 0) {
      float cx = std::stof(argv[count + 1]);
      float cy = std::stof(argv[count + 2]);
      float cz = std::stof(argv[count + 3]);
      float rad = std::stof(argv[count + 4]);
      Point center = Point(cx, cy, cz);
      
      Ellipsoid sphere = Ellipsoid(center, rad, f);
      shapes.push_back(&sphere);

      count += 4;
      fprintf(stdout, "%s\n", "sphere");
    } else if (strcmp(argv[count], TRIANGLE) == 0) {
      Point a = Point(std::stof(argv[count+1]), std::stof(argv[count+2]), std::stof(argv[count+3]));
      Point b = Point(std::stof(argv[count+4]), std::stof(argv[count+5]), std::stof(argv[count+6]));
      Point c = Point(std::stof(argv[count+7]), std::stof(argv[count+8]), std::stof(argv[count+9]));

      Triangle tri = Triangle(a, b, c, f);
      shapes.push_back(&tri);
      
      count += 9;
    } else if (strcmp(argv[count], OBJ_FILE) == 0) {
      // TODO: obj file parsing

    } else if (strcmp(argv[count], POINT_LIGHT) == 0) {

      Light pl = Light(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), std::stof(argv[count + 4]), std::stof(argv[count + 5]), std::stof(argv[count + 6]), std::stof(argv[count + 7]));
      count += 7;

      lights.push_back(pl);

    } else if (strcmp(argv[count], DIRECT_LIGHT) == 0) {
      
      Light dl = Light(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), std::stof(argv[count + 4]), std::stof(argv[count + 5]), std::stof(argv[count + 6]));
      count += 6;

      lights.push_back(dl);
      fprintf(stdout, "%s\n", "direct light");
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
      fprintf(stdout, "%s\n", "BRDF");
    } else if (strcmp(argv[count], TRANSLATE) == 0) {
      Matrix trans = Matrix(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), 1);
      // TODO: PUT THIS MATRIX INTO SOME SORT OF LIST
      count += 3;
    } else if (strcmp(argv[count], ROTATE) == 0) {
      Matrix rot = Matrix(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), 2);
      // TODO: PUT THIS MATRIX INTO SOME SORT OF LIST
      count += 3;
    } else if (strcmp(argv[count], SCALE) == 0) {
      Matrix scale = Matrix(std::stof(argv[count + 1]), std::stof(argv[count + 2]), std::stof(argv[count + 3]), 3);
      // TODO: PUT THIS MATRIX INTO SOME SORT OF LIST
      count += 3;
    }
    count += 1;
  }

  /* Shade each pixel */
  for (int i = 0; i < WIDTH; i++) {
    float u = (i + 0.5)/WIDTH;
    for (int j = 0; j < HEIGHT; j++) {
      //Calculate the coordinate of pixel's position on image plane
      float v = (j + 0.5)/HEIGHT;

      Vector ll (camera.getViewPlane().getLL().getX(), camera.getViewPlane().getLL().getY(), camera.getViewPlane().getLL().getZ());
      Vector lr (camera.getViewPlane().getLR().getX(), camera.getViewPlane().getLR().getY(), camera.getViewPlane().getLR().getZ());
      Vector ul (camera.getViewPlane().getUL().getX(), camera.getViewPlane().getUL().getY(), camera.getViewPlane().getUL().getZ());
      Vector ur (camera.getViewPlane().getUR().getX(), camera.getViewPlane().getUR().getY(), camera.getViewPlane().getUR().getZ());
      // Point on plane = u(v * LL + (1-v)UL) + (1 - u)(v*LR + (1-v)UR)
      Vector temp = ((ur * v) + lr * (1 - v)) * u + ((ul * v) + ll * (1-v)) * (1-u);
      // Vector temp = ((ll * v) + ul * (1 - v)) * u + ((lr * v) + ur * (1-v)) * (1-u);
      Point p (temp.getX(), temp.getY(), temp.getZ());

      Vector r_dir (p, camera.getEye());
      //R(t) = E + t(P-E)
      Ray r (camera.getEye(), r_dir, -1000.0, 1000.0);

      Color fin = RayTrace(r, 0);

      color.rgbRed = fmaxf(fin.getR() * 255, 0);
      color.rgbGreen = fmaxf(fin.getG() * 255, 0);
      color.rgbBlue = fmaxf(fin.getB() * 255, 0);

      FreeImage_SetPixelColor(bitmap, i, j, &color);
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
  Shape * hitobject;
  float dist_max = FLT_MAX;
  float ray_obj_dist = 0.0;
  std::vector<int>::size_type num_obj = shapes.size();
  std::vector<int>::size_type num_lights = lights.size();
  LocalGeo g;
  bool hit_check;
  Color c (0.0,0.0,0.0); //Set color to black

  // Loop through objects to check if intersection exists (HIT_CHECK)
  // If HIT_CHECK, then check if its closer & update HITOBJECT
  for (int i = 0; i < num_obj; i++)
  {
    hit_check = shapes[i]->intersection(r, &g);
    if (hit_check) {  
      Vector ray_obj_vect (r.getStart(), g.getPoint());
      ray_obj_dist = ray_obj_vect.getLength();
      if (ray_obj_dist < dist_max) {
        dist_max = ray_obj_dist;
        hitobject = shapes[i];
      }
    }
  }

  if (dist_max != FLT_MAX) {
    //Set position var PT to nearest inersection point of R & I_S
    Vector light, norm;

    norm = Vector(g.getNormal().getX(), g.getNormal().getY(), g.getNormal().getZ());

    for (int j = 0; j < num_lights; j++) {
      // TODO: FIGURE OUT SHADOW RAYS? 
      // Light Vector Calculation; POINT (-1), DIRECT (-2), AMB (0)
      if (lights[j].getType() == -1) {
        light = Vector (g.getPoint(),lights[j].getPoint());
      } else if (lights[j].getType() == -2) {
        light = Vector (-lights[j].getPoint().getX(), -lights[j].getPoint().getY(), -lights[j].getPoint().getZ());
      }
        
      light.normalize();
      //Calc perceived color of obj at pt due to this light source
      Color result = PhongShading(norm, light, lights[j].getIntensity(), hitobject->getBRDF(), g);
      c = c + result;
    }
    //Check recursive depth
    if (depth < MAX_DEPTH) {      
      Ray reflection (camera.getEye(), reflect(norm, light), -1000, 1000);
      c = c + RayTrace(reflection, depth+1) * hitobject->getBRDF().getKR();
    } else {
      //Set total color C to background color
      c = Color(0.0,0.0,0.0);
    }
  }
  return c;
}

/* Phong Shade the colors */
Color PhongShading(Vector normal, Vector light, Color light_c, BRDF b, LocalGeo g) {
  //Diffuse Color
  float diffuseColor = fmaxf(light.dot(normal), 0.0);

  //Specular Color
  Vector reflect = (normal * (2.0 * light.dot(normal))) - light;
  reflect.normalize();
  Vector view (g.getPoint(), camera.getEye());
  view.normalize();
  float specularColor = pow(fmaxf(view.dot(reflect*-1.0), 0.0), b.getSP());

  return ((b.getKS() * specularColor) + (b.getKD() * diffuseColor)) * light_c;
}


/* Return reflection vector of the LIGHT about the NORMAL */
Vector reflect(Vector normal, Vector light) {
  return (normal * (2.0 * light.dot(normal))) - light;
}

/* OPTIONAL : NOT DONE */
/* Return refraction vector of the LIGHT about the NORMAL */
Vector refract(Vector normal, Vector light, double n1, double n2) {
  float n = n1/n2;
  float angle = light.dot(normal);
  float check = pow(n,2) * (1.0 - pow(angle,2));
  if (check > 1.0) {
    return normal;
  }
  return light*n - (normal * (n + sqrt(1.0 - check)));
}
