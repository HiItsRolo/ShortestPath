/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef Frog_H
#define Frog_H

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

class Frog
{
    public:
        Frog();                          // Constructor
        virtual ~Frog();                 // Deconstructor

        void drawFrog();                 // Draw Frog image

        void initFrog(int, char *,int);  // initialize the Frog with grid size, image, frames
        void placeFrog(int,int);         // place the Frog
        void moveFrog(char *,int);       // move Frog left,right,up,down


        GridLoc getFrogLoc();            // Frog current location

        float steps;                       // steps to reach next square

        float unitWidth;                   // Unit width of the grid

        int frames;                        // Number of frames for animation
        char *FrogDir;                   // direction of Frog face
        int action;                        // What action Frog do
        bool activePlayer;                 // Frog is selected

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
        GLuint plyTex;                     // Frog image handler
        int arrXdir;                       // arrow direction x;
        int arrYdir;                       // arrow direction y
        float arrAngle;                    // animation direction of the arrow
        loc plyLoc;                        // viewport location of the Frog
        loc converter(int, int);           // convert Grid location to viewport
        int stepsPerMove;                  // animation steps

};

#endif // Frog_H
