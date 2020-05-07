 /*
 * Shortest path implementation by Rogelio Romero Spring 2020
 * Based on the Game Framework Written by Dr. Dhanyu Amarasinghe
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>                         //for characters (fish, human, etc)

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <math.h>

#include <beach.h>
#include <desert.h>
#include <foothills.h>
#include <forest.h>
#include <hmountain.h>
#include <mmountain.h>
#include <ocean.h>
#include <plain.h>
#include <Graph.h>
#include <fstream>
#include <tuple>

#include <bits/stdc++.h>

#include <vector>

/* GLUT callback Handlers */

using namespace std;

const int num = 9;
Maze *M = new Maze(num);                       // Set Maze grid size
Player *Bird = new Player();
Player *Fish = new Player();
Player *Frog = new Player();
Player *Human = new Player();


char level[num][num];

vector<beach>BT;                             //beach array for graphics
vector<desert>DT;                            //desert array for graphics
vector<foothills>HT;                        //foothills array for graphics
vector<forest>FT;                           //forest array for graphics
vector<hmountain>TT;                        //high mountain array for graphics
vector<mmountain>MT;                        //mid mountain array for graphics
vector<ocean>OT;                            //ocean array for graphics
vector<plain>PT;                            //plain array for graphics

vector<beach>BTHighlight;                             //beach terrain highlight
vector<desert>DTHighlight;                            //desert terrain highlight
vector<foothills>HTHighlight;                        //foothills terrain highlight
vector<forest>FTHighlight;                           //forest terrain highlight
vector<hmountain>TTHighlight;                        //high mountain terrain highlight
vector<mmountain>MTHighlight;                        //mid mountain terrain highlight
vector<ocean>OTHighlight;                            //ocean terrain highlight
vector<plain>PTHighlight;                            //plain terrain highlight

        int BTcount = 0; //# of beach tiles
        int DTcount = 0; //^ but for desert...
        int HTcount = 0;//'
        int FTcount = 0;//'
        int TTcount = 0;//'
        int MTcount = 0;//'
        int OTcount = 0;//'
        int PTcount = 0;//' but for plains

Timer *T0 = new Timer();                     // animation timer

Timer *T1 = new Timer();                     // animation timer


float wWidth, wHeight;                       // display window width and Height
int xPos,yPos;                               // Viewport mapping

string dir[num*num];                             //direction movement array. (num*num should be able to hold any potential paths)

int counter = 0;                        //counter for actions = 0

int dircount = 0;                           //count of how much actions are to be taken

int destX, destY;
bool playerSelected = false;


void display(void);                          // Main Display : this runs in a loop
void playerActions();


void resize(int width, int height)           // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_COLOR_MATERIAL);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();
    T1->Start();                                       // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    M->generateTerrain(); //calls the terrain generation function

    M->loadBackgroundImage("images/bak.jpg");           // Load maze background image



    for(int i = 0; i < 1; i++){ //loads level from text file and will add graphics according to characters in text file
        int y = num-1; //start at top left (gridsize-1)

        ifstream read; //reads file
      //  read.open("level.txt");
        read.open("terrain.txt"); //open terrain.txt file (can open other levels through here as well
        string line; //to read a line

        while (getline(read,line) && y >= 0){ //while the file hasn't finished reading lines, or y positions in grid haven't been filled
        int x = 0; //reset x when moving down a Y pos
            for(char c : line){ //read character by character

            if(x < num){ //if x of grid hasn't been filled

                if (c == 'O'){ //if ocean

                    OT.push_back(ocean());
                    OT[OTcount].oceanInit(M->getGridSize(),"images/ocean.png");// Load ocean
                    OT[OTcount].placeocean(x,y);

                    OTHighlight.push_back(ocean());
                    OTHighlight[OTcount].oceanInit(M->getGridSize(),"images/ocean.png");// Load ocean
                    OTHighlight[OTcount].placeocean(-1,-1);

                    OTHighlight[OTcount].red= 1.0; //set highlighter color
                    OTHighlight[OTcount].green= 0.2;
                    OTHighlight[OTcount].blue= 1.0;
                    OTHighlight[OTcount].alpha= 0.6;
                    OTcount++;
                }

                if (c == 'B'){

                    BT.push_back(beach());
                    BT[BTcount].beachInit(M->getGridSize(),"images/beach.png");// Load beach
                    BT[BTcount].placebeach(x,y);

                    BTHighlight.push_back(beach());
                    BTHighlight[BTcount].beachInit(M->getGridSize(),"images/beach.png");// Load beach
                    BTHighlight[BTcount].placebeach(-1,-1);

                    BTHighlight[BTcount].red= 1.0;
                    BTHighlight[BTcount].green= 0.2;
                    BTHighlight[BTcount].blue= 1.0;
                    BTHighlight[BTcount].alpha= 0.6;
                    BTcount++;

                }

                if (c == 'D'){

                    DT.push_back(desert());
                    DT[DTcount].desertInit(M->getGridSize(),"images/desert.png");// Load desert
                    DT[DTcount].placedesert(x,y);

                    DTHighlight.push_back(desert());
                    DTHighlight[DTcount].desertInit(M->getGridSize(),"images/desert.png");// Load desert
                    DTHighlight[DTcount].placedesert(-1,-1);

                    DTHighlight[DTcount].red= 1.0;
                    DTHighlight[DTcount].green= 0.2;
                    DTHighlight[DTcount].blue= 1.0;
                    DTHighlight[DTcount].alpha= 0.6;
                    DTcount++;
                }

                if (c == 'F'){

                    FT.push_back(forest());
                    FT[FTcount].forestInit(M->getGridSize(),"images/forest.png");// Load forest
                    FT[FTcount].placeforest(x,y);

                    FTHighlight.push_back(forest());
                    FTHighlight[FTcount].forestInit(M->getGridSize(),"images/forest.png");// Load forest
                    FTHighlight[FTcount].placeforest(-1,-1);

                    FTHighlight[FTcount].red= 1.0;
                    FTHighlight[FTcount].green= 0.2;
                    FTHighlight[FTcount].blue= 1.0;
                    FTHighlight[FTcount].alpha= 0.6;
                    FTcount++;
                }

                if (c == 'P'){

                    PT.push_back(plain());
                    PT[PTcount].plainInit(M->getGridSize(),"images/plains.png");// Load plains
                    PT[PTcount].placeplain(x,y);

                    PTHighlight.push_back(plain());
                    PTHighlight[PTcount].plainInit(M->getGridSize(),"images/plains.png");// Load plains
                    PTHighlight[PTcount].placeplain(-1,-1);

                    PTHighlight[PTcount].red= 1.0;
                    PTHighlight[PTcount].green= 0.2;
                    PTHighlight[PTcount].blue= 1.0;
                    PTHighlight[PTcount].alpha= 0.6;
                    PTcount++;
                }
                if (c == 'H'){

                    HT.push_back(foothills());
                    HT[HTcount].foothillsInit(M->getGridSize(),"images/foothills.png");// Load foothills
                    HT[HTcount].placefoothills(x,y);

                    HTHighlight.push_back(foothills());
                    HTHighlight[HTcount].foothillsInit(M->getGridSize(),"images/foothills.png");// Load foothills
                    HTHighlight[HTcount].placefoothills(-1,-1);

                    HTHighlight[HTcount].red= 1.0;
                    HTHighlight[HTcount].green= 0.2;
                    HTHighlight[HTcount].blue= 1.0;
                    HTHighlight[HTcount].alpha= 0.6;
                    HTcount++;
                }
                if (c == 'M'){

                    MT.push_back(mmountain());
                    MT[MTcount].mmountainInit(M->getGridSize(),"images/mountain.png"); // Load mountain
                    MT[MTcount].placemmountain(x,y);

                    MTHighlight.push_back(mmountain());
                    MTHighlight[MTcount].mmountainInit(M->getGridSize(),"images/mountain.png");// Load mountain
                    MTHighlight[MTcount].placemmountain(-1,-1);

                    MTHighlight[MTcount].red= 1.0;
                    MTHighlight[MTcount].green= 0.2;
                    MTHighlight[MTcount].blue= 1.0;
                    MTHighlight[MTcount].alpha= 0.6;
                    MTcount++;
                }
                if (c == 'T'){

                    TT.push_back(hmountain());
                    TT[TTcount].hmountainInit(M->getGridSize(),"images/mountainpeak.png"); // Load highmountain
                    TT[TTcount].placehmountain(x,y);

                    TTHighlight.push_back(hmountain());
                    TTHighlight[TTcount].hmountainInit(M->getGridSize(),"images/mountainpeak.png");// Load highmountain
                    TTHighlight[TTcount].placehmountain(-1,-1);

                    TTHighlight[TTcount].red= 1.0;
                    TTHighlight[TTcount].green= 0.2;
                    TTHighlight[TTcount].blue= 1.0;
                    TTHighlight[TTcount].alpha= 0.6;
                    TTcount++;
                }
                level[x][y] = c; //set level 2-D array to current character

                x++;
                }

            }
            y--;
        }
    }



    Frog->initPlayer(M->getGridSize(),"images/frog.png",4);


    if(TT.size() + OT.size() != (num*num)){ //check is there is an element on grid that is not high mountain or ocean
        for(int i = 0; i < num; i++){
            for(int j = 0; j < num; j++)
            if(level[i][j] != 'T' || level[i][j] != 'O'){
                if ((Bird->getPlayerLoc().x != i && Bird->getPlayerLoc().y != j)&&(Frog->getPlayerLoc().x != i && Frog->getPlayerLoc().y != j)){
                    Human->initPlayer(M->getGridSize(),"images/human.png",4);
                    Human->placePlayer(i,j);   // Place human
                    i = num;
                    j = num;
                }
            }
        }
    }


    if(OT.size() + BT.size() != 0){ //check is there is an element on grid that is ocean or beach
        for(int i = 0; i < num; i++){
            for(int j = 0; j < num; j++)
            if(level[i][j] == 'O'){//place fish in ocean
                if ((Bird->getPlayerLoc().x != i && Bird->getPlayerLoc().y != j)&&(Frog->getPlayerLoc().x != i && Frog->getPlayerLoc().y != j)){//check if other character exists on space
                    Fish->initPlayer(M->getGridSize(),"images/fish.png",4);
                    Fish->placePlayer(i,j); // Place fish
                    i = num;
                    j = num;
                }
            }
        }
    }

//

     if(DT.size() != (num*num)){ //check is there is an element on grid that is not Desert

        for(int i = num - 1; i >= 0; i--){
            for(int j = 0; j < num; j++){
                if(level[i][j] != 'D'){
                    if ((Human->getPlayerLoc().x != i && Human->getPlayerLoc().y != j)&&(Frog->getPlayerLoc().x != i && Frog->getPlayerLoc().y != j)&&(Fish->getPlayerLoc().x != i && Fish->getPlayerLoc().y != j)){
                        Bird->initPlayer(M->getGridSize(),"images/bird.png",4); //initialize Bird character
                        Bird->placePlayer(i,j);   // Place bird
                        i = -1;
                        j = num;
                    }
                }
            }
        }
    }


        if(DT.size() + MT.size() + TT.size() != (num*num)){ //check is there is an element on grid that Frog can be placed on

        for(int i = 0; i < num; i++){
            for(int j = num - 1; j >= 0; j--){
                if(level[i][j] != 'T' && level[i][j] != 'M' && level[i][j] != 'D'){
                    if ((Human->getPlayerLoc().x != i && Human->getPlayerLoc().y != j)&&(Bird->getPlayerLoc().x != i && Bird->getPlayerLoc().y != j)&&(Fish->getPlayerLoc().x != i && Fish->getPlayerLoc().y != j)){
                        Frog->initPlayer(M->getGridSize(),"images/frog.png",4); //initialize Frog character
                        Frog->placePlayer(i,j);   // Place frog
                        i = num;
                        j = -1;
                    }
                }
            }
        }
    }


}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        glPushMatrix();
         M->drawBackground();           // Display Background
        glPopMatrix();

        for(int i=0;i< OT.size();i++){
            OT[i].drawocean();           //display ocean tiles
        }

        for(int i=0;i< BT.size();i++){
            BT[i].drawbeach();          //display beach tiles
        }

        for(int i=0;i< DT.size();i++){
            DT[i].drawdesert();         //display desert tiles
        }

        for(int i=0;i< FT.size();i++){
            FT[i].drawforest();         //display forest tiles
        }

        for(int i=0;i< PT.size();i++){
            PT[i].drawplain();          //display plain tiles
        }

        for(int i=0;i< HT.size();i++){
            HT[i].drawfoothills();      //display foothills tiles
        }

        for(int i=0;i< MT.size();i++){
            MT[i].drawmmountain();      //display mid mountain tiles
        }


        for(int i=0;i< TT.size();i++){
            TT[i].drawhmountain();      //display high mountain tiles
        }

        // highlighter tiles are drawn after texture tiles so that they can be displayed in front

        for(int i=0;i< OT.size();i++){
            OTHighlight[i].drawocean();  //display ocean highlight tiles
        }

        for(int i=0;i< BT.size();i++){
            BTHighlight[i].drawbeach(); //display beach highlight tiles
        }

        for(int i=0;i< DT.size();i++){
            DTHighlight[i].drawdesert();//display desert highlight tiles
        }

        for(int i=0;i< FT.size();i++){
            FTHighlight[i].drawforest();//display forest highlight tiles
        }

        for(int i=0;i< PT.size();i++){
            PTHighlight[i].drawplain(); //display plain highlight tiles
        }

        for(int i=0;i< HT.size();i++){
            HTHighlight[i].drawfoothills();//display foothills highlight tiles
        }

        for(int i=0;i< MT.size();i++){
            MTHighlight[i].drawmmountain();//display mid mountain highlight tiles
        }

        for(int i=0;i< TT.size();i++){
            TTHighlight[i].drawhmountain();//display high mountain highlight tiles
        }


        glPushMatrix();
            M->drawGrid();              // Draw the grid
        glPopMatrix();

        glPushMatrix();
             Human->drawPlayer();           // Draw Human move actions
        glPopMatrix();

        glPushMatrix();
             Fish->drawPlayer();           // Draw Fish move actions
        glPopMatrix();

        glPushMatrix();
             Bird->drawPlayer();           // Draw Bird move actions
        glPopMatrix();

        glPushMatrix();
             Frog->drawPlayer();           // Draw Frog move actions
        glPopMatrix();



    glutSwapBuffers();
}

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':

        break;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }
    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)          // Mouse map coordinates
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =(int) (posX *M->getGridSize()/2 +M->getGridSize()/2); // update mouse position X
    yPos =(int) (posY *M->getGridSize()/2 +M->getGridSize()/2);

    cout<<"Mouse Click location : "<< xPos<<" "<<yPos <<endl;   // print out grid value
    cout<<"Element at space is: " <<level[xPos][yPos] <<endl; // print out texture value at click location
}

void movePlayer(Player* character, string direction, int frames){

   char* buffer = new char[direction.length()];


   strcpy(buffer,direction.c_str());

    if(character->steps > character->unitWidth){
        character->steps =0;
        strcpy(buffer,"stand"); //makes character stand when action is done
        character->placePlayer(character->getPlayerLoc().x,character->getPlayerLoc().y);
        if(T1->GetTicks()>500){ // wait 500 ms between movements
            dir[counter].clear(); //clear direction array at current index
            counter++; // increase counter
            T1->Reset(); //reset timer
        }
    }
    else{
        if(character->activePlayer){
            character->movePlayer(buffer,frames); //move player in the required direction
        }
        delete []buffer; // garbage collection for buffer
    }
}

 void idle(void)
{
    if(T0->GetTicks()>30 && counter<dircount)
      {
         if(Human->activePlayer)
         movePlayer(Human,dir[counter],3);//if human active, move human
         if(Fish->activePlayer)
         movePlayer(Fish,dir[counter],3);//if fish active, move fish
         if(Bird->activePlayer)
         movePlayer(Bird,dir[counter],3);//if bird active, move bird
        if(Frog->activePlayer)
         movePlayer(Frog,dir[counter],3);//if frog active, move frog

         T0->Reset();
      }
    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON: // left mouse button for picking player

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);

              if (xPos == Human->getPlayerLoc().x && yPos == Human->getPlayerLoc().y){
                // set player at clicked location to active
                Fish->activePlayer = false;
                Frog->activePlayer = false;
                Bird->activePlayer = false;
                playerSelected = true;
                Human->activePlayer = true;
                //set all other players to false
              }
              else if (xPos == Fish->getPlayerLoc().x && yPos == Fish->getPlayerLoc().y){
                Frog->activePlayer = false;
                Bird->activePlayer = false;
                Human->activePlayer = false;
                playerSelected = true;
                Fish->activePlayer = true;
                //set all other players to false
              }
                else if (xPos == Bird->getPlayerLoc().x && yPos == Bird->getPlayerLoc().y){
                Frog->activePlayer = false;
                Fish->activePlayer = false;
                Human->activePlayer = false;
                playerSelected = true;
                Bird->activePlayer = true;
                //set all other players to false
              }
              else if (xPos == Frog->getPlayerLoc().x && yPos == Frog->getPlayerLoc().y){
                Fish->activePlayer = false;
                Bird->activePlayer = false;
                Human->activePlayer = false;
                playerSelected = true;
                Frog->activePlayer = true;
                //set all other players to false
              }


             }
            break;


      case GLUT_RIGHT_BUTTON: // right mouse button for picking destination

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
              if(playerSelected){
                destX = xPos;
                destY = yPos;

                    if (Human->activePlayer){

                    Graph*G = new Graph(num*num);
                        // matrix elements will be added to graph as nodes using the following encoding: (y * num) + x
                        for (int x = 0; x < num; x++){ //adds all edges of grid to the graph (NEED TO IMPLEMENT UNREACHABLE TERRAIN AVOIDANCE, Currently just tries to avoid but can still traverse if trapped)
                            for (int y = 0; y < num; y++){
                                if (x - 1 >= 0){ //checks bounds of grid
                                   //will check if other character occupy a space
                                    if (!(((x - 1 == Bird->getPlayerLoc().x) && (y == Bird->getPlayerLoc().y)) || ((x - 1 == Frog->getPlayerLoc().x) && (y == Frog->getPlayerLoc().y))||((x - 1 == Fish->getPlayerLoc().x) && (y == Fish->getPlayerLoc().y)))){ // checks if space is occupied by other characters

                                                int a = (y * num) + x;
                                                int b = ((y * num) + (x - 1)); //add edge to left adjacent node
                                                G->addEdge(a, b, Human->Dmat(level[x][y],level[x-1][y],"Human")); //adds an edge and searches value in human matrix

                                    }
                                }

                                if (x + 1 < num){//checks bounds of grid

                                    if (!(((x + 1 == Bird->getPlayerLoc().x) && (y == Bird->getPlayerLoc().y)) || ((x + 1 == Frog->getPlayerLoc().x) && (y == Frog->getPlayerLoc().y))||((x + 1 == Fish->getPlayerLoc().x) && (y == Fish->getPlayerLoc().y)))){  // checks if space is occupied by other characters

                                                int a = (y * num) + x;
                                                int b = ((y * num) + (x + 1)); //add edge to right adjacent node
                                                G->addEdge(a, b, Human->Dmat(level[x][y],level[x+1][y],"Human"));

                                    }
                                }
                                if (y - 1 >= 0){//checks bounds of grid

                                    if (!(((x == Bird->getPlayerLoc().x) && (y - 1 == Bird->getPlayerLoc().y)) || ((x == Frog->getPlayerLoc().x) && (y - 1 == Frog->getPlayerLoc().y))||((x == Fish->getPlayerLoc().x) && (y - 1 == Fish->getPlayerLoc().y)))){  // checks if space is occupied by other characters
                                                                                       int a = (y * num) + x;
                                                int b = ((y - 1) * num) + x; //add edge to down adjacent node
                                                G->addEdge(a, b, Human->Dmat(level[x][y],level[x][y-1],"Human"));

                                    }
                                }
                                if (y + 1 < num){//checks bounds of grid

                                    if (!(((x == Bird->getPlayerLoc().x) && (y + 1 == Bird->getPlayerLoc().y)) || ((x == Frog->getPlayerLoc().x) && (y + 1 == Frog->getPlayerLoc().y))||((x == Fish->getPlayerLoc().x) && (y + 1 == Fish->getPlayerLoc().y)))){  // checks if space is occupied by other characters

                                                int a = (y * num) + x;
                                                int b = ((y + 1) * num) + x; //add edge to up adjacent node
                                                G->addEdge(a, b, Human->Dmat(level[x][y],level[x][y+1],"Human"));

                                    }
                                }
                            }
                        }

                        int startloc = (Human->getPlayerLoc().y * num) + Human->getPlayerLoc().x; // starting location for search
                        //cout << "starting location" << startloc << endl;
                        int tarloc = (destY * num) + destX; //target location for search
                        //cout << "target location" << tarloc << endl;

                        tuple<int,string*,int*, int*> movepath = Human->shortestPath(G,startloc, tarloc, num); //call shortest path function
                        G->~Graph(); //delete graph (garbage collection)

                        dircount = get<0>(movepath); // count of how many moves character will make

                        for(int i=0;i< OTcount;i++){
                            OTHighlight[i].placeocean(-1,-1);  //place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< BTcount;i++){
                            BTHighlight[i].placebeach(-1,-1); //place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< DTcount;i++){
                            DTHighlight[i].placedesert(-1,-1);//place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< FTcount;i++){
                            FTHighlight[i].placeforest(-1,-1);//place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< PTcount;i++){
                            PTHighlight[i].placeplain(-1,-1); //place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< HTcount;i++){
                            HTHighlight[i].placefoothills(-1,-1);//place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< MTcount;i++){
                            MTHighlight[i].placemmountain(-1,-1);//place highlighter tiles from previous movement off-screen
                        }

                        for(int i=0;i< TTcount;i++){
                            TTHighlight[i].placehmountain(-1,-1);//place highlighter tiles from previous movement off-screen
                        }

                        int BTcount = 0;
                        int DTcount = 0;
                        int HTcount = 0;
                        int FTcount = 0;
                        int TTcount = 0;
                        int MTcount = 0; // reset count for highlighting
                        int OTcount = 0;
                        int PTcount = 0;

                        for (int i = 0; i < dircount; i++){
                            dir[i] = get<1>(movepath)[i]; // direction character will move at each step i
                            int x = get<2>(movepath)[i+1]; // x value of move
                            int y = get<3>(movepath)[i+1]; //y value of move
                            char c = level[x][y]; // used to highlight path

                            if (c == 'O'){
                                OTHighlight[OTcount].placeocean(x,y); //highlight a ocean tile
                                OTcount++;
                            }

                            if (c == 'B'){
                                BTHighlight[BTcount].placebeach(x,y); //highlight a beach tile
                                BTcount++;
                            }

                            if (c == 'D'){
                                DTHighlight[DTcount].placedesert(x,y); //highlight a desert tile
                                DTcount++;
                            }

                            if (c == 'F'){
                                FTHighlight[FTcount].placeforest(x,y); //highlight a forest tile
                                FTcount++;
                            }

                            if (c == 'P'){
                                PTHighlight[PTcount].placeplain(x,y); //highlight a plains tile
                                PTcount++;
                            }
                            if (c == 'H'){
                                HTHighlight[HTcount].placefoothills(x,y); //highlight a foothills tile
                                HTcount++;
                            }
                            if (c == 'M'){
                                MTHighlight[MTcount].placemmountain(x,y); //highlight a mid mountain tile
                                MTcount++;
                            }
                            if (c == 'T'){
                                TTHighlight[TTcount].placehmountain(x,y); // highlight a high mountain tile
                                TTcount++;
                            }

                        }
                      }

                    else if (Fish->activePlayer){

                    Graph*G = new Graph(num*num);

                        for (int x = 0; x < num; x++){ //adds all edges of grid to the graph (NEED TO IMPLEMENT OTHER CHARACTER AVOIDANCE AND UNREACHABLE TERRAIN AVOIDANCE)
                            for (int y = 0; y < num; y++){
                                if (x - 1 >= 0){ //checks bounds of grid
                                  if (!(((x - 1 == Bird->getPlayerLoc().x) && (y == Bird->getPlayerLoc().y)) || ((x - 1 == Frog->getPlayerLoc().x) && (y == Frog->getPlayerLoc().y))||((x - 1 == Human->getPlayerLoc().x) && (y == Human->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y * num) + (x - 1));
                                        G->addEdge(a, b, Fish->Dmat(level[x][y],level[x-1][y],"Fish"));
                                    }
                                }

                                if (x + 1 < num){//checks bounds of grid
                                    if (!(((x + 1 == Bird->getPlayerLoc().x) && (y == Bird->getPlayerLoc().y)) || ((x + 1 == Frog->getPlayerLoc().x) && (y == Frog->getPlayerLoc().y))||((x + 1 == Human->getPlayerLoc().x) && (y == Human->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y * num) + (x + 1));
                                        G->addEdge(a, b, Fish->Dmat(level[x][y],level[x+1][y],"Fish"));
                                     }
                                }
                                if (y - 1 >= 0){//checks bounds of grid
                                  if (!(((x == Bird->getPlayerLoc().x) && (y - 1 == Bird->getPlayerLoc().y)) || ((x == Frog->getPlayerLoc().x) && (y - 1 == Frog->getPlayerLoc().y))||((x == Human->getPlayerLoc().x) && (y - 1 == Human->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y - 1) * num) + x;
                                        G->addEdge(a, b, Fish->Dmat(level[x][y],level[x][y-1],"Fish"));
                                    }
                                }
                                if (y + 1 < num){//checks bounds of grid
                                 if (!(((x == Bird->getPlayerLoc().x) && (y + 1 == Bird->getPlayerLoc().y)) || ((x == Frog->getPlayerLoc().x) && (y + 1 == Frog->getPlayerLoc().y))||((x == Human->getPlayerLoc().x) && (y + 1 == Human->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y + 1) * num) + x;
                                        G->addEdge(a, b, Fish->Dmat(level[x][y],level[x][y+1],"Fish"));
                                    }
                                }
                            }
                        }

                        int startloc = (Fish->getPlayerLoc().y * num) + Fish->getPlayerLoc().x; // starting location for search
                        cout << "starting location" << startloc << endl;
                        int tarloc = (destY * num) + destX; //target location for search
                        cout << "target location" << tarloc << endl;

                        tuple<int,string*,int*, int*> movepath = Fish->shortestPath(G,startloc, tarloc, num);
                        G->~Graph();

                        dircount = get<0>(movepath);


                        for(int i=0;i< OTcount;i++){
                            OTHighlight[i].placeocean(-1,-1);  //display ocean highlight tiles
                        }

                        for(int i=0;i< BTcount;i++){
                            BTHighlight[i].placebeach(-1,-1); //display beach highlight tiles
                        }

                        for(int i=0;i< DTcount;i++){
                            DTHighlight[i].placedesert(-1,-1);//display desert highlight tiles
                        }

                        for(int i=0;i< FTcount;i++){
                            FTHighlight[i].placeforest(-1,-1);//display forest highlight tiles
                        }

                        for(int i=0;i< PTcount;i++){
                            PTHighlight[i].placeplain(-1,-1); //display plain highlight tiles
                        }

                        for(int i=0;i< HTcount;i++){
                            HTHighlight[i].placefoothills(-1,-1);//display foothills highlight tiles
                        }

                        for(int i=0;i< MTcount;i++){
                            MTHighlight[i].placemmountain(-1,-1);//display mid mountain highlight tiles
                        }

                        for(int i=0;i< TTcount;i++){
                            TTHighlight[i].placehmountain(-1,-1);//display high mountain highlight tiles
                        }

                        int BTcount = 0;
                        int DTcount = 0;
                        int HTcount = 0;
                        int FTcount = 0;
                        int TTcount = 0;
                        int MTcount = 0; // for highlighting
                        int OTcount = 0;
                        int PTcount = 0;

                            for (int i = 0; i < dircount; i++){
                                dir[i] = get<1>(movepath)[i]; // direction character will move at each step i
                                int x = get<2>(movepath)[i+1]; // x value of move
                                int y = get<3>(movepath)[i+1]; //y value of move
                                char c = level[x][y]; // used to highlight path

                                if (c == 'O'){
                                    OTHighlight[OTcount].placeocean(x,y);
                                    OTcount++;
                                }

                                if (c == 'B'){
                                    BTHighlight[BTcount].placebeach(x,y);
                                    BTcount++;
                                }

                                if (c == 'D'){
                                    DTHighlight[DTcount].placedesert(x,y);
                                    DTcount++;
                                }

                                if (c == 'F'){
                                    FTHighlight[FTcount].placeforest(x,y);
                                    FTcount++;
                                }

                                if (c == 'P'){
                                    PTHighlight[PTcount].placeplain(x,y);
                                    PTcount++;
                                }
                                if (c == 'H'){
                                    HTHighlight[HTcount].placefoothills(x,y);
                                    HTcount++;
                                }
                                if (c == 'M'){
                                    MTHighlight[MTcount].placemmountain(x,y);
                                    MTcount++;
                                }
                                if (c == 'T'){
                                    TTHighlight[TTcount].placehmountain(x,y);
                                    TTcount++;
                                }

                            }
                        }


                    else if (Bird->activePlayer){

                    Graph*G = new Graph(num*num);

                        for (int x = 0; x < num; x++){ //adds all edges of grid to the graph (NEED TO IMPLEMENT OTHER CHARACTER AVOIDANCE AND UNREACHABLE TERRAIN AVOIDANCE)
                            for (int y = 0; y < num; y++){
                                if (x - 1 >= 0){ //checks bounds of grid
                                    if (!(((x - 1 == Human->getPlayerLoc().x) && (y == Human->getPlayerLoc().y)) || ((x - 1 == Frog->getPlayerLoc().x) && (y == Frog->getPlayerLoc().y))||((x - 1 == Fish->getPlayerLoc().x) && (y == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y * num) + (x - 1));
                                        G->addEdge(a, b, Bird->Dmat(level[x][y],level[x-1][y],"Bird"));
                                    }
                                }

                                if (x + 1 < num){//checks bounds of grid
                                    if (!(((x + 1 == Human->getPlayerLoc().x) && (y == Human->getPlayerLoc().y)) || ((x + 1 == Frog->getPlayerLoc().x) && (y == Frog->getPlayerLoc().y))||((x + 1 == Fish->getPlayerLoc().x) && (y == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y * num) + (x + 1));
                                        G->addEdge(a, b, Bird->Dmat(level[x][y],level[x+1][y],"Bird"));
                                     }
                                }
                                if (y - 1 >= 0){//checks bounds of grid
                                    if (!(((x == Human->getPlayerLoc().x) && (y - 1 == Human->getPlayerLoc().y)) || ((x == Frog->getPlayerLoc().x) && (y - 1 == Frog->getPlayerLoc().y))||((x == Fish->getPlayerLoc().x) && (y - 1 == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y - 1) * num) + x;
                                        G->addEdge(a, b, Bird->Dmat(level[x][y],level[x][y-1],"Bird"));
                                    }
                                }
                                if (y + 1 < num){//checks bounds of grid
                                    if (!(((x == Human->getPlayerLoc().x) && (y + 1 == Human->getPlayerLoc().y)) || ((x == Frog->getPlayerLoc().x) && (y + 1 == Frog->getPlayerLoc().y))||((x == Fish->getPlayerLoc().x) && (y + 1 == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y + 1) * num) + x;
                                        G->addEdge(a, b, Bird->Dmat(level[x][y],level[x][y+1],"Bird"));
                                    }
                                }
                            }
                        }

                        int startloc = (Bird->getPlayerLoc().y * num) + Bird->getPlayerLoc().x; // starting location for search
                        cout << "starting location" << startloc << endl;
                        int tarloc = (destY * num) + destX; //target location for search
                        cout << "target location" << tarloc << endl;

                        tuple<int,string*,int*, int*> movepath = Bird->shortestPath(G,startloc, tarloc, num);
                        G->~Graph();

                        dircount = get<0>(movepath);

                        for(int i=0;i< OTcount;i++){
                            OTHighlight[i].placeocean(-1,-1);  //display ocean highlight tiles
                        }

                        for(int i=0;i< BTcount;i++){
                            BTHighlight[i].placebeach(-1,-1); //display beach highlight tiles
                        }

                        for(int i=0;i< DTcount;i++){
                            DTHighlight[i].placedesert(-1,-1);//display desert highlight tiles
                        }

                        for(int i=0;i< FTcount;i++){
                            FTHighlight[i].placeforest(-1,-1);//display forest highlight tiles
                        }

                        for(int i=0;i< PTcount;i++){
                            PTHighlight[i].placeplain(-1,-1); //display plain highlight tiles
                        }

                        for(int i=0;i< HTcount;i++){
                            HTHighlight[i].placefoothills(-1,-1);//display foothills highlight tiles
                        }

                        for(int i=0;i< MTcount;i++){
                            MTHighlight[i].placemmountain(-1,-1);//display mid mountain highlight tiles
                        }

                        for(int i=0;i< TTcount;i++){
                            TTHighlight[i].placehmountain(-1,-1);//display high mountain highlight tiles
                        }

                        int BTcount = 0;
                        int DTcount = 0;
                        int HTcount = 0;
                        int FTcount = 0;
                        int TTcount = 0;
                        int MTcount = 0; // for highlighting
                        int OTcount = 0;
                        int PTcount = 0;

                            for (int i = 0; i < dircount; i++){
                                dir[i] = get<1>(movepath)[i]; // direction character will move at each step i
                                int x = get<2>(movepath)[i+1]; // x value of move
                                int y = get<3>(movepath)[i+1]; //y value of move
                                char c = level[x][y]; // used to highlight path

                                if (c == 'O'){
                                    OTHighlight[OTcount].placeocean(x,y);
                                    OTcount++;
                                }

                                if (c == 'B'){
                                    BTHighlight[BTcount].placebeach(x,y);
                                    BTcount++;
                                }

                                if (c == 'D'){
                                    DTHighlight[DTcount].placedesert(x,y);
                                    DTcount++;
                                }

                                if (c == 'F'){
                                    FTHighlight[FTcount].placeforest(x,y);
                                    FTcount++;
                                }

                                if (c == 'P'){
                                    PTHighlight[PTcount].placeplain(x,y);
                                    PTcount++;
                                }
                                if (c == 'H'){
                                    HTHighlight[HTcount].placefoothills(x,y);
                                    HTcount++;
                                }
                                if (c == 'M'){
                                    MTHighlight[MTcount].placemmountain(x,y);
                                    MTcount++;
                                }
                                if (c == 'T'){
                                    TTHighlight[TTcount].placehmountain(x,y);
                                    TTcount++;
                                }

                            }
                    }

                    else if (Frog->activePlayer){

                    Graph*G = new Graph(num*num);

                        for (int x = 0; x < num; x++){ //adds all edges of grid to the graph (NEED TO IMPLEMENT OTHER CHARACTER AVOIDANCE AND UNREACHABLE TERRAIN AVOIDANCE)
                            for (int y = 0; y < num; y++){
                                if (x - 1 >= 0){ //checks bounds of grid
                                    if (!(((x - 1 == Human->getPlayerLoc().x) && (y == Human->getPlayerLoc().y)) || ((x - 1 == Bird->getPlayerLoc().x) && (y == Bird->getPlayerLoc().y))||((x - 1 == Fish->getPlayerLoc().x) && (y == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y * num) + (x - 1));
                                        G->addEdge(a, b, Frog->Dmat(level[x][y],level[x-1][y],"Frog"));
                                    }
                                }

                                if (x + 1 < num){//checks bounds of grid
                                    if (!(((x + 1 == Human->getPlayerLoc().x) && (y == Human->getPlayerLoc().y)) || ((x + 1 == Bird->getPlayerLoc().x) && (y == Bird->getPlayerLoc().y))||((x + 1 == Fish->getPlayerLoc().x) && (y == Fish->getPlayerLoc().y)))){                                        int a = (y * num) + x;
                                        int b = ((y * num) + (x + 1));
                                        G->addEdge(a, b, Frog->Dmat(level[x][y],level[x+1][y],"Frog"));
                                     }
                                }
                                if (y - 1 >= 0){//checks bounds of grid
                                    if (!(((x == Human->getPlayerLoc().x) && (y - 1 == Human->getPlayerLoc().y)) || ((x == Bird->getPlayerLoc().x) && (y - 1 == Bird->getPlayerLoc().y))||((x == Fish->getPlayerLoc().x) && (y - 1 == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y - 1) * num) + x;
                                        G->addEdge(a, b, Frog->Dmat(level[x][y],level[x][y-1],"Frog"));
                                    }
                                }
                                if (y + 1 < num){//checks bounds of grid
                                    if (!(((x == Human->getPlayerLoc().x) && (y + 1 == Human->getPlayerLoc().y)) || ((x == Bird->getPlayerLoc().x) && (y + 1 == Bird->getPlayerLoc().y))||((x == Fish->getPlayerLoc().x) && (y + 1 == Fish->getPlayerLoc().y)))){
                                        int a = (y * num) + x;
                                        int b = ((y + 1) * num) + x;
                                        G->addEdge(a, b, Frog->Dmat(level[x][y],level[x][y+1],"Frog"));
                                    }
                                }
                            }
                        }

                        int startloc = (Frog->getPlayerLoc().y * num) + Frog->getPlayerLoc().x; // starting location for search
                        cout << "starting location" << startloc << endl;
                        int tarloc = (destY * num) + destX; //target location for search
                        cout << "target location" << tarloc << endl;

                        tuple<int,string*,int*, int*> movepath = Frog->shortestPath(G,startloc, tarloc, num);
                        G->~Graph();

                        dircount = get<0>(movepath);


                       for(int i=0;i< OTcount;i++){
                            OTHighlight[i].placeocean(-1,-1);  //display ocean highlight tiles
                        }

                        for(int i=0;i< BTcount;i++){
                            BTHighlight[i].placebeach(-1,-1); //display beach highlight tiles
                        }

                        for(int i=0;i< DTcount;i++){
                            DTHighlight[i].placedesert(-1,-1);//display desert highlight tiles
                        }

                        for(int i=0;i< FTcount;i++){
                            FTHighlight[i].placeforest(-1,-1);//display forest highlight tiles
                        }

                        for(int i=0;i< PTcount;i++){
                            PTHighlight[i].placeplain(-1,-1); //display plain highlight tiles
                        }

                        for(int i=0;i< HTcount;i++){
                            HTHighlight[i].placefoothills(-1,-1);//display foothills highlight tiles
                        }

                        for(int i=0;i< MTcount;i++){
                            MTHighlight[i].placemmountain(-1,-1);//display mid mountain highlight tiles
                        }

                        for(int i=0;i< TTcount;i++){
                            TTHighlight[i].placehmountain(-1,-1);//display high mountain highlight tiles
                        }

                        int BTcount = 0;
                        int DTcount = 0;
                        int HTcount = 0;
                        int FTcount = 0;
                        int TTcount = 0;
                        int MTcount = 0; // for highlighting
                        int OTcount = 0;
                        int PTcount = 0;

                            for (int i = 0; i < dircount; i++){
                                dir[i] = get<1>(movepath)[i]; // direction character will move at each step i
                                int x = get<2>(movepath)[i+1]; // x value of move
                                int y = get<3>(movepath)[i+1]; //y value of move
                                char c = level[x][y]; // used to highlight path

                                if (c == 'O'){
                                    OTHighlight[OTcount].placeocean(x,y);
                                    OTcount++;
                                }

                                if (c == 'B'){
                                    BTHighlight[BTcount].placebeach(x,y);
                                    BTcount++;
                                }

                                if (c == 'D'){
                                    DTHighlight[DTcount].placedesert(x,y);
                                    DTcount++;
                                }

                                if (c == 'F'){
                                    FTHighlight[FTcount].placeforest(x,y);
                                    FTcount++;
                                }

                                if (c == 'P'){
                                    PTHighlight[PTcount].placeplain(x,y);
                                    PTcount++;
                                }
                                if (c == 'H'){
                                    HTHighlight[HTcount].placefoothills(x,y);
                                    HTcount++;
                                }
                                if (c == 'M'){
                                    MTHighlight[MTcount].placemmountain(x,y);
                                    MTcount++;
                                }
                                if (c == 'T'){
                                    TTHighlight[TTcount].placehmountain(x,y);
                                    TTcount++;
                                }

                            }
                    }

                        counter = 0;


                        playerSelected = false;
                      }
            }
            break;
    }
     glutPostRedisplay();
};

void Specialkeys(int key, int x, int y)
{
    // cout<<"Player Location  :  "<<Human->getPlayerLoc().x<< "    "<<Human->getPlayerLoc().y<<endl;
    switch(key)
    {
 /*   case GLUT_KEY_UP:
        P->action =1;
         break;

    case GLUT_KEY_DOWN:
        P->action =2;
         break;

    case GLUT_KEY_LEFT:
        P->action =3;
         break;

    case GLUT_KEY_RIGHT:
        P->action =4;
         break;
   */
   }
   // comment this after you debug


  glutPostRedisplay();

}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
