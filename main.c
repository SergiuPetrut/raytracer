/* A simple ray tracer */
//I'm sorry it took so long to make this, but this year was a bit of an imbroglio for everyone.
//I hope this works as intended, since I spent way too much time making sure it works properly with more than 2 threads.

#include <stdio.h>
#include <stdlib.h>
#include "vectormath.h"
#include <stdbool.h> // Needed for boolean algebra
#include <pthread.h> //Needed for pthread
#include <math.h>//Maths.
#define NUMTHREADS 10 //How many threads are going to be created ; To be changed if needed/wanted.

#define min(a, b)(((a) < (b)) ? (a) : (10
// Width and height of the PPM image

/*#define WIDTH 1920
#define HEIGHT 1080
Tried using above resolution to test performance increases/decreses depending on the number of threads.*/
#define WIDTH 800
#define HEIGHT 600


pthread_mutex_t pixelcount;

light lights[3];
sphere spheres[3];
  material materials[3];


    void init(){
  materials[0].diffuse.red = 1;
  materials[0].diffuse.green = 0;
  materials[0].diffuse.blue = 0;
  materials[0].reflection = 0.2;

  materials[1].diffuse.red = 0;
  materials[1].diffuse.green = 1;
  materials[1].diffuse.blue = 0;
  materials[1].reflection = 0.5;

  materials[2].diffuse.red = 0;
  materials[2].diffuse.green = 0;
  materials[2].diffuse.blue = 1;
  materials[2].reflection = 0.9;


  spheres[0].pos.x = 200;
  spheres[0].pos.y = 300;
  spheres[0].pos.z = 0;
  spheres[0].radius = 100;
  spheres[0].material = 0;

  spheres[1].pos.x = 400;
  spheres[1].pos.y = 400;
  spheres[1].pos.z = 0;
  spheres[1].radius = 100;
  spheres[1].material = 1;

  spheres[2].pos.x = 500;
  spheres[2].pos.y = 140;
  spheres[2].pos.z = 0;
  spheres[2].radius = 100;
  spheres[2].material = 2;


  lights[0].pos.x = 0;
  lights[0].pos.y = 240;
  lights[0].pos.z = -100;
  lights[0].intensity.red = 1;
  lights[0].intensity.green = 1;
  lights[0].intensity.blue = 1;

  lights[1].pos.x = 3200;
  lights[1].pos.y = 3000;
  lights[1].pos.z = -1000;
  lights[1].intensity.red = 0.6;
  lights[1].intensity.green = 0.7;
  lights[1].intensity.blue = 1;

  lights[2].pos.x = 600;
  lights[2].pos.y = 0;
  lights[2].pos.z = -100;
  lights[2].intensity.red = 0.3;
  lights[2].intensity.green = 0.5;
  lights[2].intensity.blue = 1;

  }
typedef struct{
  unsigned char* img;
  int* imagePosition;
} Thrimg; //Thread Image



  void* work( void *n){
    Thrimg* pos = (Thrimg*) n;

    pthread_mutex_lock(&pixelcount);

ray r;
  int x, y;
  for (y = *pos->imagePosition; y < HEIGHT/NUMTHREADS + *pos->imagePosition; y++) {
    for (x = 0; x < WIDTH; x++) {

      float red = 0;
      float green = 0;
      float blue = 0;

      int level = 0;
      float coef = 1.0;

      r.start.x = x;
      r.start.y = y;
      r.start.z = -2000;

      r.dir.x = 0;
      r.dir.y = 0;
      r.dir.z = 1;

      do {
        //Finds closest intersection
        float t = 20000.0f;
        int currentSphere = -1;

        unsigned int i;
        for (i = 0; i < 3; i++) {
          if (intersectRaySphere( &r, &spheres[i], &t))
            currentSphere = i;
        }
        if (currentSphere == -1) break;

        vector scaled = vectorScale(t, &r.dir);
        vector newStart = vectorAdd( &r.start, &scaled);

        // Finds the normal for the new vector at the intersection
        vector n = vectorSub( &newStart, &spheres[currentSphere].pos);
        float temp = vectorDot( &n, &n);
        if (temp == 0) break;

        temp = 1.0f / sqrtf(temp);
        n = vectorScale(temp, &n);

        // Checks "material" value in order to determine the colour
        material currentMat = materials[spheres[currentSphere].material];

        // Determines value for "light" at this point
        unsigned int j;
        for (j = 0; j < 3; j++) {
          light currentLight = lights[j];
          vector dist = vectorSub( &currentLight.pos, &newStart);
          if (vectorDot( &n, &dist) <= 0.0f) continue;
          float t = sqrtf(vectorDot( &dist, &dist));
          if (t <= 0.0f) continue;

          ray lightRay;
          lightRay.start = newStart;
          lightRay.dir = vectorScale((1 / t), &dist);

          // Lambert diffusion
          float lambert = vectorDot( &lightRay.dir, &n) * coef;
          red += lambert * currentLight.intensity.red * currentMat.diffuse.red;
          green += lambert * currentLight.intensity.green * currentMat.diffuse.green;
          blue += lambert * currentLight.intensity.blue * currentMat.diffuse.blue;
        }
        /* Iterate over the reflection */
        coef *= currentMat.reflection;

        /* The reflected ray start and direction */
        r.start = newStart;
        float reflect = 2.0f * vectorDot( &r.dir, &n);
        vector tmp = vectorScale(reflect, &n);
        r.dir = vectorSub( &r.dir, &tmp);

        level++;

      } while ((coef > 0.0f) && (level < 15));

      pos->img[(x + y * WIDTH) * 3 + 0] = (unsigned char) min(red * 255.0f, 255.0f);
      pos->img[(x + y * WIDTH) * 3 + 1] = (unsigned char) min(green * 255.0f, 255.0f);
      pos->img[(x + y * WIDTH) * 3 + 2] = (unsigned char) min(blue * 255.0f, 255.0f);
    }
  }
pthread_mutex_unlock(&pixelcount);

return 0;
  }



int main(int argc, char * argv[]) {
    unsigned char img[3 * WIDTH * HEIGHT];

init();

pthread_t thr[NUMTHREADS];
   for (int i = 0; i < NUMTHREADS; i++)
  {
    Thrimg* pos = malloc(sizeof(Thrimg));
    pos->img = img;
    pos->imagePosition = malloc(sizeof(int));
    *pos->imagePosition = i * HEIGHT/NUMTHREADS;

    if (pthread_create(&thr[i], NULL, &work, (void*)pos) != 0)
      {
        perror("Error, cannot create thread.");
      }
      else{
        printf("Initialised thread # %d\n",i+1);
      }
  }

  for (int i = 0; i < NUMTHREADS; i++)
  {
    if (pthread_join(thr[i], NULL) != 0)
      {
        perror("Error, cannot join thread.");
      }
      else{
                printf("Joining thread # %d\n",i+1);
      }
  }



  // Saves the raw data as a PPM

  saveppm("result.ppm", img, WIDTH, HEIGHT);
    pthread_mutex_destroy(&pixelcount);
  return 0;
}
