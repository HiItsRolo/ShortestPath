/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef Human_H
#define Human_H

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

class Human
{
    public:
        Human();                          // Constructor
        virtual ~Human();                 // Deconstructor

        void drawPlayer();                 // Draw Human image

        void initPlayer(int, char *,int);  // initialize the Player with grid size, image, frames
        void placePlayer(int,int);         // place the Player
        void movePlayer(char *,int);       // move Player left,right,up,down


        GridLoc getPlayerLoc();            // Player current location

        float steps;                       // steps to reach next square

        float unitWidth;                   // Unit width of the grid

        int frames;                        // Number of frames for animation
        char *PlayerDir;                   // direction of Player face
        int action;                        // What action Player do
        bool activePlayer;                 // Player is selected

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
        GLuint plyTex;                     // Player image handler
        int arrXdir;                       // arrow direction x;
        int arrYdir;                       // arrow direction y
        float arrAngle;                    // animation direction of the arrow
        loc plyLoc;                        // viewport location of the Player
        loc converter(int, int);           // convert Grid location to viewport
        int stepsPerMove;                  // animation steps

};

#endif // Human_H
