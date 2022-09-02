#include <stdbool.h>
#include <math.h>

//Vector Struct
typedef struct {
  float x, y, z;
}
vector;

//Sphere Struct
typedef struct {
  vector pos;
  float radius;
  int material;
}
sphere;

//Ray Struct
typedef struct {
  vector start;
  vector dir;
}
ray;

// Colour
typedef struct {
  float red, green, blue;
}
colour;

//Material Struct
typedef struct {
  colour diffuse;
  float reflection;
}
material;

// Lightsource Struct
typedef struct {
  vector pos;
  colour intensity;
}
light;

// Subtracts two vectors
vector vectorSub(vector * v1, vector * v2) ;
// Multiplies two vectors and returns the dot product
float vectorDot(vector * v1, vector * v2) ;
// Multiplies the vector by the dot product
vector vectorScale(float c, vector * v) ;
// Adds two vectors
vector vectorAdd(vector * v1, vector * v2) ;

//Determines if the ray and the sphere intersect
bool intersectRaySphere(ray * r, sphere * s, float * t);
void saveppm(char * filename, unsigned char * img, int width, int height);
