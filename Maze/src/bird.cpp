/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Bird.h>



Bird::Bird()
{
    //ctor
    BirdDir = "up";
    action= 0;
    activePlayer = false;
}

Bird::~Bird()
{
    //dtor
}

void Bird::drawBird()
{

    glColor3f(1.0,1.0,1.0);

    glTranslatef(plyLoc.x ,plyLoc.y,0.0);

    glBindTexture(GL_TEXTURE_2D,plyTex);
    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);


    glBegin(GL_QUADS);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
     glEnd();

}


void Bird::initBird(int gSize,char *FileName,int frms)
{
    gridSize = gSize;
    frames = frms;

    xmax =1/(float)frames;
    ymax =0.25;
    xmin =0;
    ymin =0;

    unitWidth = (float)2/gridSize;
    stepsPerMove =frames;

    T->Start();
    plyLoc.x= -unitWidth;
    plyLoc.y= -unitWidth;

    t= unitWidth/stepsPerMove;
    steps =0;
    plyTex = TextureLoader(FileName);
}

void Bird::placeBird(int x, int y)
{
    plyLoc.x =  converter(x,y).x;
    plyLoc.y =  converter(x,y).y;
}

void Bird::moveBird(char* dir, int frames)
{
   /*
   if(steps >= unitWidth) {
   placeBird(getBirdLoc().x,getBirdLoc().y);
   steps =0;
   }
   */

  if(strcmp(dir, "left")==0)
   {   BirdDir = "left";

       if(T->GetTicks()>100)
        {   steps +=t;
            if(plyLoc.x>-1+unitWidth/2)
              {
                 steps<unitWidth? plyLoc.x -= t:NULL;
              }
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin += 1/(float)frames;
            xmax += 1/(float)frames;
            ymin =0.75;
            ymax =1.0;
            T->Reset();
          }  // end if
       }

   else if(strcmp(dir, "right")==0)
   {
        BirdDir = "right";

        if(T->GetTicks()>100)
        {
            steps +=t;
            if(plyLoc.x<1-unitWidth/2)
            steps<unitWidth?plyLoc.x += t:NULL;

            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;
            T->Reset();
        }  // end if
   }


   else if(strcmp(dir, "up")==0)
   {
        BirdDir = "up";

        if(T->GetTicks()>100)
        {
            steps +=t;
            if(plyLoc.y< 1-unitWidth/2)
            steps<unitWidth?plyLoc.y += t:NULL;

            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;

            ymin =0.25;
            ymax =0.50;
            T->Reset();
         }  // end if

    }

   else if(strcmp(dir, "down")==0)
   {
         BirdDir = "down";

        if(T->GetTicks()>100)
        {
            steps +=t;
            if(plyLoc.y > -1+unitWidth/2)
                steps<unitWidth?plyLoc.y -= t:NULL;
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.0;
            ymax =0.25;
            T->Reset();
        }  // end if

     }

}

GridLoc Bird::getBirdLoc()
{
   GridLoc val;

   val.x = (int)(ceil((plyLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((plyLoc.y +(1-unitWidth))/unitWidth));

   return val;

}

loc Bird::converter(int x, int y)
{
  loc val;
        x+=1;
        y+=1;

       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}

tuple<int,string*,int*, int*> Bird::shortestPath(Graph* G,int startloc,int tarloc,int num){ //return the number of steps, travel direction, and grid spaces that character will travel
    int* d; // distance array pt
    int* pi; //previous value
    int dircount = 0; //count of steps
    string* dir = new string[num*num]; //stores direction character will travel at each step
    int* x = new int[num*num]; //x position character is at each step
    int* y = new int[num*num]; //y pos character is at each step

    G->Dijkstra(startloc, d, pi); //performs dijkstra's search algorithm
    stack<int> path; //reverse order of pi to find the path from starting location
    while (tarloc != startloc){
        cout << tarloc<< "->";
        path.push(tarloc);
        tarloc = pi[tarloc];
    }

    delete d;
    delete pi;
    x[0] = startloc%num; //find initial x position
    y[0] = startloc/num; //find initial y position
    while (path.size() != 0){
        if (startloc == path.top()-num){ //if moving to a node up from current location
            dir[dircount] = "up";
            dircount++;
        }
        else if (startloc == path.top()+num){ //if moving to a node down from current location
            dir[dircount] = "down";
            dircount++;
        }
        else if (startloc == path.top()-1){//if moving to a node right from current location
            dir[dircount] = "right";
            dircount++;
        }
        else if (startloc == path.top()+1){////if moving to a node left from current location
            dir[dircount] = "left";
            dircount++;
        }
        startloc = path.top();
        x[dircount] = startloc % num;//find current x pos
        y[dircount] = startloc / num;//find current y pos
        path.pop();
    }
        return make_tuple(dircount,dir,x,y); //return the tuple
}

int Bird::Dmat(char a, char b) {    //INEFFICIENT, IMPLEMENT HASH TABLE           //return the distance between two terrains

    if (a == 'T') {
        if (b == 'T') { //distance between high mountain and high mountain
            return 1000; //human cannot travel reach high mountains so travel to and from is prohibited
        }
        else if (b == 'M') { // distance between high mountain and mid mountain
            return 1000;
        }
        else if (b == 'H') { // distance between high mountain and foothills
            return 1000;
        }
        else if (b == 'P') { // distance between high mountain and plains
            return 1000;
        }
        else if (b == 'F') { // distance between high mountain and forest
            return 1000;
        }
        else if (b == 'D') { // distance between high mountain and desert
            return 1000;
        }
        else if (b == 'B') { // distance between high mountain and beach
            return 1000;
        }
        else if (b == 'O') { // distance between high mountain and ocean
            return 1000;
        }
    }
	else if (a == 'M') {
        if(b == 'T') { // distance between mid mountain and high mountain
            return 1000;
        }
        else if (b == 'M') { // distance between mid mountain and mid mountain
            return 4;
        }
        else if (b == 'H') { // distance between mid mountain and foothills
            return 2;
        }
        else if (b == 'P') { // distance between mid mountain and plains
            return 3;
        }
        else if (b == 'F') { // distance between mid mountain and forest
            return 4;
        }
        else if (b == 'D') { // distance between mid mountain and desert
            return 5;
        }
        else if (b == 'B') { // distance between mid mountain and beach
            return 5;
        }
        else if (b == 'O') { // distance between mid mountain and ocean
            return 1000;
        }
	}
	else if (a == 'H') {
        if(b == 'T') { // distance between foothills and high mountain
            return 1000;
        }
        else if (b == 'M') {
            return 5;
        }
        else if (b == 'H') {
            return 2;
        }
        else if (b == 'P') {
            return 3;
        }
        else if (b == 'F') {
            return 4;
        }
        else if (b == 'D') {
            return 2;
        }
        else if (b == 'B') {
            return 3;
        }
        else if (b == 'O') {
            return 1000;
        }
	}
	else if (a == 'P') {
        if (b == 'T') {
            return 1000;
        }
        else if (b == 'M') {
            return 6;
        }
        else if (b == 'H') {
            return 3;
        }
        else if (b == 'P') {
            return 1;
        }
        else if (b == 'F') {
            return 2;
        }
        else if (b == 'D') {
            return 2;
        }
        else if (b == 'B') {
            return 2;
        }
        else if (b == 'O') {
            return 1000;
        }
	}
	else if (a == 'F') {
        if (b == 'T') {
            return 1000;
        }
        else if (a == 'F' && b == 'M') {
            return 8;
        }
        else if (a == 'F' && b == 'H') {
            return 3;
        }
        else if (a == 'F' && b == 'P') {
            return 1;
        }
        else if (a == 'F' && b == 'F') {
            return 2;
        }
        else if (a == 'F' && b == 'D') {
            return 1;
        }
        else if (a == 'F' && b == 'B') {
            return 3;
        }
        else if (a == 'F' && b == 'O') {
            return 1000;
        }
	}
	else if (a == 'D'){
        if(b == 'T') {
            return 1000;
        }
        else if (a == 'D' && b == 'M') {
            return 9;
        }
        else if (a == 'D' && b == 'H') {
            return 2;
        }
        else if (a == 'D' && b == 'P') {
            return 2;
        }
        else if (a == 'D' && b == 'F') {
            return 2;
        }
        else if (a == 'D' && b == 'D') {
            return 2;
        }
        else if (a == 'D' && b == 'B') {
            return 1;
        }
        else if (a == 'D' && b == 'O') {
            return 1000;
        }
	}
	else if (a == 'B'){
        if(b == 'T') {
            return 1000;
        }
        else if (b == 'M') {
            return 15;
        }
        else if (b == 'H') {
            return 15;
        }
        else if (b == 'P') {
            return 3;
        }
        else if (b == 'F') {
            return 7;
        }
        else if (b == 'D') {
            return 4;
        }
        else if (b == 'B') {
            return 2;
        }
        else if (b == 'O') {
            return 1000;
        }
	}
	else if (a == 'O'){
        if (b == 'T') {
            return 1000;
        }
        else if (a == 'O' && b == 'M') {
            return 1000;
        }
        else if (a == 'O' && b == 'H') {
            return 1000;
        }
        else if (a == 'O' && b == 'P') {
            return 1000;
        }
        else if (a == 'O' && b == 'F') {
            return 1000;
        }
        else if (a == 'O' && b == 'D') {
            return 1000;
        }
        else if (a == 'O' && b == 'B') {
            return 1000;
        }
        else if (a == 'O' && b == 'O') {
            return 1000;
        }
	}
	return 1000;
}
