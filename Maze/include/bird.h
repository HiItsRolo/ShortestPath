/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef Bird_H
#define Bird_H

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

class Bird
{
    public:
        Bird();                          // Constructor
        virtual ~Bird();                 // Deconstructor

        void drawBird();                 // Draw Bird image

        void initBird(int, char *,int);  // initialize the Bird with grid size, image, frames
        void placeBird(int,int);         // place the Bird
        void moveBird(char *,int);       // move Bird left,right,up,down


        GridLoc getBirdLoc();            // Bird current location

        float steps;                       // steps to reach next square

        float unitWidth;                   // Unit width of the grid

        int frames;                        // Number of frames for animation
        char *BirdDir;                   // direction of Bird face
        int action;                        // What action Bird do
        bool activePlayer;                 // Bird is selected

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
        GLuint plyTex;                     // Bird image handler
        int arrXdir;                       // arrow direction x;
        int arrYdir;                       // arrow direction y
        float arrAngle;                    // animation direction of the arrow
        loc plyLoc;                        // viewport location of the Bird
        loc converter(int, int);           // convert Grid location to viewport
        int stepsPerMove;                  // animation steps

};

#endif // Bird_H
