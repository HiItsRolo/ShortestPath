/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef Fish_H
#define Fish_H

#include <CommonThings.h>
#include<string.h>
#include<gl/gl.h>
#include<GL/glut.h>
#include<SOIL.h>
#include<iostream>
#include <Timer.h>
#include <math.h>       /* ceil */
#include <Graph.h>
#include <stack>
#include <tuple>



using namespace std;

class Fish
{
    public:
        Fish();                          // Constructor
        virtual ~Fish();                 // Deconstructor

        void drawFish();                 // Draw Fish image

        void initFish(int, char *,int);  // initialize the Fish with grid size, image, frames
        void placeFish(int,int);         // place the Fish
        void moveFish(char *,int);       // move Fish left,right,up,down


        GridLoc getFishLoc();            // Fish current location

        float steps;                       // steps to reach next square

        float unitWidth;                   // Unit width of the grid

        int frames;                        // Number of frames for animation
        char *FishDir;                   // direction of Fish face
        int action;                        // What action Fish do
        bool activePlayer;                 // Fish is selected

        tuple<int,string*,int*, int*> shortestPath(Graph* G, int startloc, int tarloc, int num);

        int Dmat(char a, char b);

        Timer *T = new Timer();            // Set Timer for animation

    protected:

        float t;                           // Ticker for animation
        void timer();                      // Timer for animation
        int gridSize;                      // Grid Size

    private:

    private:
        float xmax, xmin,ymax,ymin;        // animation texture map
        GLuint plyTex;                     // Fish image handler
        int arrXdir;                       // arrow direction x;
        int arrYdir;                       // arrow direction y
        float arrAngle;                    // animation direction of the arrow
        loc plyLoc;                        // viewport location of the Fish
        loc converter(int, int);           // convert Grid location to viewport
        int stepsPerMove;                  // animation steps

};

#endif // Fish_H
