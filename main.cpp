#include <string>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <float.h>
#include <cmath>
#include <cstring>
#include <fstream>

#include "FreeImage.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

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
#define RESET_TFM "xfz"

#define BPP 24
// Raytracer recursive depth limit
#define MAX_DEPTH 3


/* RayTracer Methods */
Color PhongShading(Vector normal, Vector light, Color light_c, BRDF b, LocalGeo g);
Color RayTrace(Ray r, int depth);
Vector reflection(Vector normal, Vector v);

/* Globals */
Camera camera = Camera();
std::vector<Light> lights;
std::vector<Shape *> shapes;
float WIDTH = 1000;
float HEIGHT = 500;

using namespace std;
using namespace boost;

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
  ifstream sceneFile;
  sceneFile.open(argv[1]);

  /* Declare tokenizer & delimiters */
  typedef tokenizer< char_separator<char> > Tokenizer;
  char_separator<char> sep(" \t");
  string line;

  /* Parse each line from file & break into tokens */
  while (getline(sceneFile, line)) {
    Tokenizer tok(line,sep);
    for(Tokenizer::iterator it(tok.begin()), end(tok.end()); it != end; ) {
      const char * command = (*it).c_str();
      if (strcmp(command, CAMERA) == 0) {
        printf("%s\n", "CAMERA");
        Point eye = Point(stof((++it)->c_str()), stof((++it)->c_str()),stof((++it)->c_str()));
        Point ll = Point(stof((++it)->c_str()), stof((++it)->c_str()),stof((++it)->c_str()));
        Point lr = Point(stof((++it)->c_str()), stof((++it)->c_str()),stof((++it)->c_str()));
        Point ul = Point(stof((++it)->c_str()), stof((++it)->c_str()),stof((++it)->c_str()));
        Point ur = Point(stof((++it)->c_str()), stof((++it)->c_str()),stof((++it)->c_str()));
        camera = Camera(eye, ll, lr, ul, ur);
      } else if (strcmp(command, SPHERE) == 0) {
        printf("%s\n", "SPHERE");
        Point center = Point(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        float rad = stof((++it)->c_str());
        Ellipsoid * sphere = new Ellipsoid(center, rad, f);
        shapes.push_back(sphere);
      } else if (strcmp(command, TRIANGLE) == 0) {
        printf("%s\n", "TRIANGLE");
        Point a = Point(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        Point b = Point(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        Point c = Point(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        Triangle * tri = new Triangle(a, b, c, f);
        shapes.push_back(tri);
      } else if (strcmp(command, OBJ_FILE) == 0) {
        printf("%s\n", "OBJ_FILE");
        fprintf(stdout, "FILENAME: %s\n", (++it)->c_str());

      } else if (strcmp(command, POINT_LIGHT) == 0) {
        printf("%s\n", "POINT_LIGHT");
        Light pl = Light(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()), 
                         stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()),
                         0.0);
        if (++it != tok.end()) {
          pl.setFalloff(stof((it)->c_str()));
        }
        lights.push_back(pl);
      } else if (strcmp(command, DIRECT_LIGHT) == 0) {
        printf("%s\n", "DIRECT_LIGHT");
        Light dl = Light(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()), 
                         stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        lights.push_back(dl);
      } else if (strcmp(command, AMB_LIGHT) == 0) {
        printf("%s\n", "AMB_LIGHT");
        Light al = Light(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        lights.push_back(al);
      } else if (strcmp(command, MATERIAL) == 0) {
        printf("%s\n", "Material");
        Color ka = Color(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        Color kd = Color(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        Color ks = Color(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        float sp = stof((++it)->c_str());
        Color kr = Color(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()));
        f = BRDF(ka, kd, ks, sp, kr);
      } else if (strcmp(command, TRANSLATE) == 0) {
        printf("%s\n", "Transform");
        Matrix trans = Matrix(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()), 1);
      } else if (strcmp(command, ROTATE) == 0) {
        printf("%s\n", "Rotate");
        Matrix rot = Matrix(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()), 2);
      } else if (strcmp(command, SCALE) == 0) {
        printf("%s\n", "Scale");
        Matrix scale = Matrix(stof((++it)->c_str()), stof((++it)->c_str()), stof((++it)->c_str()), 3);
      } else if (strcmp(command, RESET_TFM) == 0) {
        printf("%s\n", "RESET_TFM");
      } else {
        //Unrecognized type
        fprintf(stderr, "Unrecognized command: %s\n", command);
      }
      break;
    }
  }
  sceneFile.close();
  /* End Parsing */

  /* ViewPlane Corners used to determine ViewPlane Intersection Point*/
  Vector ll = Vector(camera.getLL(), Point());
  Vector lr = Vector(camera.getLR(), Point());
  Vector ul = Vector(camera.getUL(), Point());
  Vector ur = Vector(camera.getUR(), Point());

  /* Shade each pixel */
  for (int i = 0; i < WIDTH; i++) {
    float u = (i + 0.5)/WIDTH;
    for (int j = 0; j < HEIGHT; j++) {
      //Calculate the coordinate of pixel's position on image plane
      float v = (j + 0.5)/HEIGHT;

      // Point on plane = u(v * LL + (1-v)UL) + (1 - u)(v*LR + (1-v)UR)
      Vector temp = ((ur * v) + lr * (1 - v)) * u + ((ul * v) + ll * (1-v)) * (1-u);
      Point p = Point(temp.getX(), temp.getY(), temp.getZ());

      Vector r_dir = Vector(p, camera.getEye());
      //R(t) = E + t(P-E)
      Ray r = Ray(camera.getEye(), r_dir, -2000.0, 2000.0);

      Color fin = RayTrace(r, 0);
      float fR = fmaxf(0.0, fminf(fin.getR(), 1.0));
      float fG = fmaxf(0.0, fminf(fin.getG(), 1.0));
      float fB = fmaxf(0.0, fminf(fin.getB(), 1.0));
      color.rgbRed = floor(fR == 1.0 ? 255 : fR * 256.0);
      color.rgbGreen = floor(fG == 1.0 ? 255 : fG * 256.0);
      color.rgbBlue = floor(fB == 1.0 ? 255 : fB * 256.0);

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
  LocalGeo g = LocalGeo();
  LocalGeo closest;
  bool hit_check;
  Color c (0.0,0.0,0.0); //Set color to black
  // Loop through objects to check if intersection exists (HIT_CHECK)
  // If HIT_CHECK, then check if its closer & update HITOBJECT

  for (int i = 0; i < num_obj; i++) {
    hit_check = shapes[i]->intersection(r, &g);
    if (hit_check) {  
      Vector ray_obj_vect (r.getStart(), g.getPoint());
      ray_obj_dist = ray_obj_vect.getLength();
      if (ray_obj_dist < dist_max) {
        closest.setPoint(g.getPoint());
        closest.setNormal(g.getNormal());
        dist_max = ray_obj_dist;
        hitobject = shapes[i];
      }
    }
  }

  if (dist_max != FLT_MAX) {
    //Set position var PT to nearest inersection point of R & I_S
    Vector light, norm;

    norm = Vector(closest.getNormal());

    for (int j = 0; j < num_lights; ++j) {
      // TODO: FIGURE OUT SHADOW RAYS? 
      // Light Vector Calculation; POINT (-1), DIRECT (-2), AMB (0)
      if (lights[j].getType() == 0) {
        c = c + (lights[j].getIntensity() * hitobject->getBRDF().getKA());
      } else {
        if (lights[j].getType() == -1) {

          light = Vector (lights[j].getPoint(), closest.getPoint());
        } else if (lights[j].getType() == -2) {
          light = Vector (lights[j].getPoint(), Point(0, 0, 0));
        }
          
        light.normalize();
        //Calc perceived color of obj at pt due to this light source
        Color result = PhongShading(norm, light, lights[j].getIntensity(), hitobject->getBRDF(), closest);
        c = c + result;
      }
     
    }
    //Check recursive depth
    if (depth < MAX_DEPTH) {      
      Ray ray_reflect = Ray(camera.getEye(), reflection(norm, r.getDir()), -2000, 000);
      c = c + RayTrace(ray_reflect, depth+1) * hitobject->getBRDF().getKR();
    } else {
      //TODO: Set total color C to background color (currently defaults to black)
      //c = Color(0.0,0.0,0.0);
    }
  }
  return c;
}

/* Phong Shade the colors 
 * -- Ignoring Ka since we multiply ka by AMBIENT LIGHT (not Point or Directional)
 */
Color PhongShading(Vector normal, Vector light, Color light_c, BRDF b, LocalGeo g) {
  //Diffuse Color
  float diffuseColor = fmaxf(light.dot(normal), 0.0);

  //Specular Color
  Vector reflect = reflection(normal, light);
  reflect.normalize();
  Vector view (g.getPoint(), camera.getEye());
  view.normalize();
  float specularColor = pow(fmaxf(view.dot(reflect), 0.0), b.getSP());

  return ((b.getKS() * specularColor) + (b.getKD() * diffuseColor)) * light_c;
}


/* Return reflection vector of the V about the NORMAL */
Vector reflection(Vector normal, Vector v) {
  return v - (normal * (2.0 * v.dot(normal)));
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