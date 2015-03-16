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

  // WIDTH = camera.getWidth();
  // HEIGHT = camera.getHeight();
  WIDTH = 1000;
  HEIGHT = 500;
  fprintf(stdout, "Width:%f Height:%f\n", WIDTH, HEIGHT);
  //Used to map pixels to image plane
  if (WIDTH < 1 || HEIGHT < 1) 
    fprintf(stderr, "%s\n", "FUCK");
  float r = WIDTH / 2.0;
  //float r = 1000/2.0;
  float l = -1.0 * r;
  float t = HEIGHT / 2.0;
  //float t = 500/2.0;
  float b = -1.0 * t;
  fprintf(stdout, "R:%f\tL:%f\tT:%f\tB:%f\n", r, l, t, b);

  //Vector camera_position (camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ());
  Vector camera_u (camera.getViewPlane().getLL(), camera.getViewPlane().getUL());
  camera_u.normalize();

  fprintf(stdout, "WIDTH: %f, HEIGHT: %f\n", WIDTH, HEIGHT );
  /* Shade each pixel */
  for (int i = 0; i < WIDTH; i++) {
    //fprintf(stdout, "In LOOP 1\n");
    for (int j = 0; j < HEIGHT; j++) {
      //fprintf(stdout, "IN LOOP\n");
      //TODO: Rays? Shading?
      //Calculate the coordinate of pixel's position on image plane
      float u = l + (r - l)*(i + 0.5)/WIDTH;
      float v = b + (t - b)*(j + 0.5)/HEIGHT;
      //TODO: Figure out focal distance from eye to plane
      //float d = 0.0;
      Point q (u, v, 0.0);
      Vector camera_w (q, camera.getEye());
      camera_w.normalize();
      float d = camera_w.getLength();
      Vector camera_v = camera_w.cross(camera_u);
      camera_v.normalize();
      //TODO: Figure out which axes -d, u, v coordinate to in XYZ
      //-dw + uu + vv
      Vector r_dir = camera_w * -d + camera_u * u + camera_v * v;
      Ray r (camera.getEye(), r_dir, 0.0, 0.0);
      //fprintf(stdout, "%s\n", "calling color RayTrace..");
      Color fin = RayTrace(r, 0);
      //Color fin (255.0, 0.0, 0.0);
      color.rgbRed = fmaxf(fin.getR() * 255, 0);
      color.rgbGreen = fmaxf(fin.getG() * 255, 0);
      color.rgbBlue = fmaxf(fin.getB() * 255, 0);
      //printf("%d %d %d\n", fin.getR(), fin.getG(), fin.getB());
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
  int i, j, k;
<<<<<<< HEAD
  Shape * hitobject;
=======
  Shape* hitobject;
>>>>>>> 23611fcb28cc6b8173476c5e79c331fa41a79a3f

  float dist_max = FLT_MAX;
  float ray_obj_dist = 0.0;
  std::vector<int>::size_type num_obj = shapes.size();
  std::vector<int>::size_type num_lights = lights.size();
  LocalGeo g;
  bool hit_check;

  // Loop through objects to check if intersection exists (HIT_CHECK)
  // If HIT_CHECK, then check if its closer & update HITOBJECT
  for (i = 0; i < num_obj; i++)
  {
    hit_check = shapes[i]->intersection(r, &g);
    if (hit_check) {  
      Vector ray_obj_vect (g.getPoint(), r.getStart());
      ray_obj_dist = ray_obj_vect.getLength();
      if (ray_obj_dist < dist_max) {
        dist_max = ray_obj_dist;
        hitobject = shapes[i];
      }
    }
  }

  if (!hitobject) {
    //Set position var PT to nearest inersection point of R & I_S
    Vector light, norm, reflection, refraction;

    norm  = Vector(g.getNormal().getX(), g.getNormal().getX(), g.getNormal().getZ());
    Color c (0.0,0.0,0.0); //Set color to black
    for (j = 0; j < num_lights; j++) {
      // TODO: FIGURE OUT SHADOW RAYS? IF THIS IS WHAT ITS FOR
      // for (k = 0; k < num_obj; k++) {
      //   //If OBJ blocks light coming from light source to PT
      //     //Attenuate intensity by transmittivity
      // }

      // Light Vector Calculation; Falloff = 0 for DL, > 0 for PL
      if (lights[j].getFalloff() > 0.0) {
        Vector light (lights[j].getPoint(), g.getPoint());
      } else {
        Vector light (-lights[j].getPoint().getX(), -lights[j].getPoint().getY(), -lights[j].getPoint().getZ());
      }
        
      light.normalize();
      //Calc perceived color of obj at pt due to this light source
      Color block_color = PhongShading(norm, light, lights[j].getIntensity(), shapes[k]->getBRDF(), g);
      c = c + block_color;
    }
    //Check recursive depth
    if (depth < MAX_DEPTH) {
      //Generate 2 rays: reflection  and refraction (optional)
      //Ray reflection();
      
      
      Ray reflection (camera.getEye(), reflect(norm, light), 0, 0);
      c = c + RayTrace(reflection, depth+1) * hitobject->getBRDF().getKR();
      //c = c + RayTrace(refraction, depth+1) * intersect_shape.;

    } else {
      //Set total color C to background color
      fprintf(stdout, "%s\n", "Total color to backgroudn?");
    }
    return c;
  }
}

/* Phong Shade the colors */
Color PhongShading(Vector normal, Vector light, Color light_c, BRDF b, LocalGeo g) {
  //Diffuse Color
  float diffuseColor = fmaxf(light.dot(normal), 0.0);

  //Specular Color
  Vector reflect = (normal * (2.0 * light.dot(normal))) - light;
  reflect.normalize();
  Vector view (g.getPoint(), camera.getEye());
  float specularColor = pow(fmax(view.dot(reflect), 0.0), b.getSP());

  return ((b.getKS() * specularColor) + (b.getKD() * diffuseColor) + b.getKA()) * light_c;
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
