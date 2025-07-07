/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include<iostream>
#include <conio.h>
#include <windows.h>
#include <windows.h>
#include <mmsystem.h>
#include <string>

#pragma comment(lib, "winmm.lib")
using namespace std;


static int slices = 16;
static int stacks = 16;
float angle = 0.0f;
float startpointspx=.3,startpointsqx=-.3,startpointspy=-1,startpointsqy=-1,startscalesp=0.15,startscalesq=0.15,speed=0.025,speedscale=(0.005*0.25);
bool isFallingSponge = false;
bool isFallingSquare = false;
const float initialSpongeX = 0.3f;
const float initialSpongeY = -1.0f;
const float initialSpongeScale = 0.15f;

const float initialSquareX = -0.3f;
const float initialSquareY = -1.0f;
const float initialSquareScale = 0.15f;


bool isplaying=true;


/* GLUT callback Handlers */
void race();
void road();
void render();
void update(int value);
void drawCircle(float cx, float cy, float radius, int num_segments);
void drawSquareCharacterBack(float xOffset, float yOffset,float scale);
void drawSpongeBobBack(float xOffset, float yOffset, float scale);
void drawRect(float x1, float y1, float x2, float y2);
void keyboard(unsigned char c,int x,int y);
void special(int key, int x, int y);
bool stickHitsCharacter(float charX, float charY, float charScale);
void checkStickCollisionAndPush();
void checkForWinner();
void drawWinnerText(const string& text, float x, float y, float r, float g, float b);
bool isBlockedByRotatingStick(float nextX, float nextY);
string winner = "";

void inOutTestandfall();
using namespace std;

int main(int argc, char *argv[])
{


    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
    glutCreateWindow("FALL Spongebob,SquareMan");
    glutDisplayFunc(render);
    glutTimerFunc(0, update, 0);

    glutKeyboardFunc(keyboard);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutDisplayFunc(render);
    glutMainLoop();

}

void render()
{
    if(isplaying){
    road();}
    else if(!isplaying){
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        if(winner== "Square"){

            drawSquareCharacterBack(0, 0,1);
                    drawWinnerText("SquareMan ًWins!", -0.3f, -0.8f, 1.0f, 0.5f, 0.0f);


        }
        else if(winner=="SpongeBob"){
                  drawSpongeBobBack(0, 0, 1);
                  drawWinnerText("SpongeBob Wins!", -0.4f, 0.8f, 0.55f, 0.27f, 0.07f);

                }
                   glutSwapBuffers();
    }


}
void checkStickCollisionAndPush()
{
    float len = 0.58f;
    float cx = 0.0f, cy = -0.1f;
    float amount = 0.025f;
    float rad = angle * M_PI / 180.0f;
    float x1 = cx + len * cos(rad);
    float y1 = cy + len * sin(rad);
    float x2 = cx - len * cos(rad);
    float y2 = cy - len * sin(rad);

    const int steps = 50;

    for (int i = 0; i <= steps; i++)
    {
        float t = (float)i / steps;
        float px = x1 + t * (x2 - x1);
        float py = y1 + t * (y2 - y1);

        // ==== SpongeBob ====
        float dx1 = px - startpointspx;
        float dy1 = py - startpointspy;

        if (sqrt(dx1 * dx1 + dy1 * dy1) < 0.05)
        {
            string direction1 = (abs(dx1) > abs(dy1)) ?
                ((dx1 > 0) ? "right" : "left") :
                ((dy1 > 0) ? "top" : "down");

            float nextX = startpointspx;
            float nextY = startpointspy;

            if (direction1 == "right") nextX += amount;
            else if (direction1 == "left") nextX -= amount;
            else if (direction1 == "top") nextY -= amount;
            else if (direction1 == "down") nextY += amount;

            float distToCenter = sqrt((nextX - cx) * (nextX - cx) + (nextY - cy) * (nextY - cy));

            if (distToCenter > 0.1f)  // لو مش بيعدي من النص
            {
                startpointspx = nextX;
                startpointspy = nextY;

                if (direction1 == "top") startscalesp += (0.005 * 0.25);
                else if (direction1 == "down") startscalesp -= (0.005 * 0.25);
            }

            break;
        }

        // ==== SquareMan ====
        float dx2 = px - startpointsqx;
        float dy2 = py - startpointsqy;

        if (sqrt(dx2 * dx2 + dy2 * dy2) < 0.05)
        {
            string direction2 = (abs(dx2) > abs(dy2)) ?
                ((dx2 > 0) ? "right" : "left") :
                ((dy2 > 0) ? "top" : "down");

            float nextX = startpointsqx;
            float nextY = startpointsqy;

            if (direction2 == "right") nextX += amount;
            else if (direction2 == "left") nextX -= amount;
            else if (direction2 == "top") nextY -= amount;
            else if (direction2 == "down") nextY += amount;

            float distToCenter = sqrt((nextX - cx) * (nextX - cx) + (nextY - cy) * (nextY - cy));

            if (distToCenter > 0.1f)
            {
                startpointsqx = nextX;
                startpointsqy = nextY;

                if (direction2 == "top") startscalesq += (0.005 * 0.25);
                else if (direction2 == "down") startscalesq -= (0.005 * 0.25);
            }

            break;
        }
    }
}



void inOutTestandfall()
{
    float xs11=0.75,
          xf11=0.45,
          ys1=-1,
          yf1=0.55,
          xs12=-0.75,
          xf12=-0.45,
          speedy1=(0.05)*0.5,
          speedx1=0.0096774*0.5,
          xs21=0.45,
          xf21=0.38,
          ys2=0.6,
          yf2=1,
          xs22=-0.45,
          xf22=-0.38,
          speedy2=(0.05)*0.5,
          speedx2=0.004375;

    if((startpointspy-0.1)<=0.6&&(startpointspy-0.1)>=0.54)
    {

        PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);




        while(startscalesp>0)
        {
            startscalesp-=0.0001;
            startpointspy-=0.0001;

            render();
        }
        startpointspy = initialSpongeY;
        startpointspx = initialSpongeX;
        startscalesp = initialSpongeScale;


    }

    if((startpointsqy-0.1)<=0.6&&(startpointsqy-0.1)>=0.54)
    {

        PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

        while(startscalesq>0)
        {
            startscalesq-=0.0001;
            startpointsqy-=0.0001;

            render();
        }
        startpointsqy = initialSquareY;
        startpointsqx = initialSquareX;
        startscalesq =  initialSquareScale;


    }
    for(int i=0; i<62; i++)
    {


        if(((startpointspx)> xs11 &&(startpointspx)>xs12&&abs(startpointspy - ys1) < 0.01
           ))
        {

            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

            startpointspx+=0.1;

            while(startscalesp>0)
            {
                startscalesp-=0.0001;
                startpointspy-=0.0001;

                render();
            }
            startpointspy = initialSpongeY;
            startpointspx = initialSpongeX;
            startscalesp = initialSpongeScale;


        }

        else if(((startpointspx)< xs11 &&(startpointspx)<xs12&&abs(startpointspy - ys1) < 0.01
                ))
        {

            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

            startpointspx-=0.1;
            while(startscalesp>0)
            {
                startscalesp-=0.0001;
                startpointspy-=0.0001;

                render();
            }
            startpointspy = initialSpongeY;
            startpointspx = initialSpongeX;
            startscalesp = initialSpongeScale;

        }

        if((startpointsqx> xs11 &&startpointsqx>xs12&&abs(startpointsqy - ys1) < 0.01
           ))
        {

            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

            startpointsqx+=0.1;
            while(startscalesq>0)
            {
                startscalesq-=0.0001;
                startpointsqy-=0.0001;

                render();
            }
            startpointsqy = initialSquareY;
            startpointsqx = initialSquareX;
            startscalesq =  initialSquareScale;

        }


        else if((startpointsqx< xs11 &&startpointsqx<xs12&&abs(startpointsqy - ys1) < 0.01
                ))
        {

            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

            startpointsqx-=0.1;
            while(startscalesq>0)
            {
                startscalesq-=0.0001;
                startpointsqy-=0.0001;

                render();
            }
            startpointsqy = initialSquareY;
            startpointsqx = initialSquareX;
            startscalesq =  initialSquareScale;

        }


        xs11-=speedx1;
        xs12+=speedx1;
        ys1+=speedy1;
    }


    for(int i=0; i<16; i++)
    {


        if((startpointspx> xs21 &&startpointspx>xs22&&abs(startpointspy - ys2) < 0.01
           ))
        {
            isFallingSponge = true;
            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

            startpointspx+=0.1;
            while(startscalesp>0)
            {
                startscalesp-=0.0001;
                startpointspy-=0.0001;

                render();
            }
            startpointspy = initialSpongeY;
            startpointspx = initialSpongeX;
            startscalesp = initialSpongeScale;
        }

        else if((startpointspx< xs21 &&startpointspx<xs22&&abs(startpointspy - ys2) < 0.01
                ))
        {
            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);

            startpointspx-=0.1;
            while(startscalesp>0)
            {
                startscalesp-=0.0001;
                startpointspy-=0.0001;

                render();
            }
            startpointspy = initialSpongeY;
            startpointspx = initialSpongeX;
            startscalesp = initialSpongeScale;
        }

        if((startpointsqx> xs21 &&startpointsqx>xs22&&abs(startpointsqy - ys2) < 0.01
           ))
        {
            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
            startpointsqx+=0.1;
            while(startscalesq>0)
            {
                startscalesq-=0.0001;
                startpointsqy-=0.0001;

                render();
            }
            startpointsqy = initialSquareY;
            startpointsqx = initialSquareX;
            startscalesq =  initialSquareScale;
        }

        else if((startpointsqx< xs21 &&startpointsqx<xs22&&abs(startpointsqy - ys2) < 0.01
                ))
        {
            PlaySound(TEXT("C:/Users/PC/Documents/test6/fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
            startpointsqx-=0.1;
            while(startscalesq>0)
            {
                startscalesq-=0.0001;
                startpointsqy-=0.0001;

                render();
            }
            startpointsqy = initialSquareY;
            startpointsqx = initialSquareX;
            startscalesq =  initialSquareScale;
        }


        xs21-=speedx1;
        xs22+=speedx1;
        ys2+=speedy1;
    }






}

void race()
{
    road();

}
void update(int value)
{
    angle += 2.0f;
    if (angle > 360.0f)
        angle -= 360.0f;
    checkStickCollisionAndPush();

    inOutTestandfall();


    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
      checkForWinner();
}


void road()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex2f(-0.75,-1);
    glVertex2f(0.75,-1);

    glVertex2f(0.45,0.54);
    glVertex2f(-0.45,0.54);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);

    glVertex2f(0.45,0.6);
    glVertex2f(-0.45,0.6);
    glVertex2f(-0.38,1);
    glVertex2f(0.38,1);

    glEnd();


    float startX = -0.41f;
    float startY = 0.8f;
    float tileSize = 0.048f;
    int rows = 2;
    int cols = 17;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if ((i + j) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f);  // White
            else
                glColor3f(0.0f, 0.0f, 0.0f);  // Black

            float x = startX + j * tileSize;
            float y = startY + i * tileSize;

            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + tileSize, y);
            glVertex2f(x + tileSize, y + tileSize);
            glVertex2f(x, y + tileSize);
            glEnd();
        }
    }

    drawSquareCharacterBack(startpointsqx, startpointsqy,startscalesq);
    drawSpongeBobBack(startpointspx, startpointspy, startscalesp);


    glColor3f(0,0,1);
    drawCircle(0.0, -0.1, 0.05, 100);

    glPushMatrix();

    glTranslatef(0.0f, -0.1f, 0.0f);

    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glTranslatef(0.0f, 0.1f, 0.0f);

    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);

    glVertex2f(-0.575,-.08);
    glVertex2f(0.575,-.08);
    glVertex2f(0.58,-.120);
    glVertex2f(-0.58,-.120);
    glEnd();

    glPopMatrix();






    glutSwapBuffers();

}

void keyboard(unsigned char key,int x,int y)
{
    if(key==27)
    {
        cout<<"LLLLLLL";
        exit(0);

    }

     if(key=='r')
    {
         startpointspx=.3;
         startpointsqx=-.3;
         startpointspy=-1;
         startpointsqy=-1;
         startscalesp=0.15;
         startscalesq=0.15;
         winner = "";
      isplaying=true;

    }


    if ((key == 'w')&&(1>=startpointsqy) &&
    !isBlockedByRotatingStick(startpointsqx, startpointsqy + speed))
    {
        cout << "Arrow UP pressed";
        cout<<startpointsqy;
        startpointsqy += speed;

        if(startscalesq<1)
        {
            startscalesq-= speedscale;
        }


    }
    else if ((key == 's')&&(startpointsqy>=-1) &&
    !isBlockedByRotatingStick(startpointsqx, startpointsqy - speed))
    {
        cout << "Arrow DOWN pressed";
        cout<<startpointsqy;

        startpointsqy -= speed;

        if(startscalesq<1)
        {
            startscalesq += speedscale;
        }


    }

    else if ((key == 'd')&&(1>=startpointsqx) &&
    !isBlockedByRotatingStick(  startpointsqx + speed, startpointsqy ))
    {
        cout << "Arrow right pressed";
        cout<<startpointsqx;
        startpointsqx += speed;



    }
    else if ((key == 'a')&&(startpointsqx>=-1) &&
    !isBlockedByRotatingStick(      startpointsqx - speed, startpointsqy))
    {
        cout << "Arrow left pressed";
        cout<<startpointsqy;
        cout<<startpointsqx;
        startpointsqx -= speed;


    }
    else if(key=='e'&&(1-startpointsqy)>0.3 )
    {

        for(int i=1; i<=15&&startpointsqy<0.9; i++)
        {

            startscalesq-=(0.005);
            startscalesq-=((0.0005));
            startpointsqy+=0.01;

            glutDisplayFunc(render);
            Sleep(1);


        }


        for(int i=1; i<=15&&startpointsqy; i++)
        {
            startscalesq+=(0.005);
            startscalesq-=(0.0005);
            startpointsqy+=0.01;
            glutDisplayFunc(render);
            Sleep(1);
        }
    }


    if (key == '8'&&(1>=startpointspy) &&
    !isBlockedByRotatingStick(startpointspx, startpointspy +speed))
    {
        cout << "Arrow UP pressed";
        cout<<startpointspy;
        startpointspy += speed;

        if(startscalesp<1)
        {
            startscalesp-= speedscale;
        }


    }
    else if (key == '5'&&(startpointspy>=-1) &&
    !isBlockedByRotatingStick(startpointspx,  startpointspy - speed ))
    {
        cout << "Arrow DOWN pressed";
        cout<<startpointspy;

        startpointspy -= speed;

        if(startscalesp<1)
        {
            startscalesp += speedscale;
        }


    }

    else if (key == '6'&&(1>=startpointspx) &&
    !isBlockedByRotatingStick(startpointspx +speed, startpointspy ))
    {
        cout << "Arrow right pressed";
        cout<<startpointspx;
        startpointspx += speed;



    }
    else if (key == '4'&&(startpointspx>=-1) &&
    !isBlockedByRotatingStick(  startpointspx - speed, startpointspy))
    {
        cout << "Arrow left pressed";
        cout<<startpointspy;
        cout<<startpointspx;
        startpointspx -= speed;


    }
    else if(key=='7'&&(1-startpointspy)>0.3)
    {

        for(int i=1; i<=15&&startpointspy<0.9; i++)
        {

            startscalesp-=(0.005);
            startscalesp-=((0.0005));
            startpointspy+=0.01;

            glutDisplayFunc(render);
            Sleep(1);
        }


        for(int i=1; i<=15&&startpointspy<0.9; i++)
        {
            startscalesp+=(0.005);
            startscalesp-=(0.0005);
            startpointspy+=0.01;
            glutDisplayFunc(render);
            Sleep(1);
        }

    }


    inOutTestandfall();
    checkStickCollisionAndPush();
}

void special(int key, int x, int y)
{





}


void drawSquareCharacterBack(float xOffset, float yOffset, float scale)
{
    glColor3f(0.5f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f((-1.0f * scale) + xOffset, (1.5f * scale) + yOffset);
    glVertex2f((1.0f * scale) + xOffset, (1.5f * scale) + yOffset);
    glVertex2f((1.0f * scale) + xOffset, (-1.0f * scale) + yOffset);
    glVertex2f((-1.0f * scale) + xOffset, (-1.0f * scale) + yOffset);
    glEnd();

    glColor3f(1.0f, 0.5f, 0.0f);

    glBegin(GL_QUADS);
    glVertex2f((-0.6f * scale) + xOffset, (1.0f * scale) + yOffset);
    glVertex2f((0.6f * scale) + xOffset, (1.0f * scale) + yOffset);
    glVertex2f((0.6f * scale) + xOffset, (0.4f * scale) + yOffset);
    glVertex2f((-0.6f * scale) + xOffset, (0.4f * scale) + yOffset);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f((1.0f * scale) + xOffset, (1.0f * scale) + yOffset);
    glVertex2f((1.3f * scale) + xOffset, (0.5f * scale) + yOffset);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f((-1.0f * scale) + xOffset, (1.0f * scale) + yOffset);
    glVertex2f((-1.3f * scale) + xOffset, (0.5f * scale) + yOffset);
    glEnd();

    /* legs */
    glColor3f(0.0f, 0.0f, 0.0f);

// Right leg
    glBegin(GL_LINES);
    glVertex2f((0.5f * scale) + xOffset, (-1.0f * scale) + yOffset);  // من تحت الجسم
    glVertex2f((0.5f * scale) + xOffset, (-1.5f * scale) + yOffset);  // لتحت
    glEnd();

// Left leg
    glBegin(GL_LINES);
    glVertex2f((-0.5f * scale) + xOffset, (-1.0f * scale) + yOffset);  // من تحت الجسم
    glVertex2f((-0.5f * scale) + xOffset, (-1.5f * scale) + yOffset);  // لتحت
    glEnd();


}


void drawSpongeBobBack(float xOffset, float yOffset, float scale)
{
    auto drawRect = [&](float x1, float y1, float x2, float y2)
    {
        glBegin(GL_QUADS);
        glVertex2f(xOffset + (x1 * scale), yOffset + (y1 * scale));
        glVertex2f(xOffset + (x2 * scale), yOffset + (y1 * scale));
        glVertex2f(xOffset + (x2 * scale), yOffset + (y2 * scale));
        glVertex2f(xOffset + (x1 * scale), yOffset + (y2 * scale));
        glEnd();
    };

    auto drawCircle = [&](float cx, float cy, float r, int segments = 100)
    {
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= segments; ++i)
        {
            float angle = 2.0f * 3.14159f * i / segments;
            float x = r * cos(angle);
            float y = r * sin(angle);
            glVertex2f(xOffset + (cx + x) * scale, yOffset + (cy + y) * scale);
        }
        glEnd();
    };

    glColor3f(1.0, 1.0, 0.4);
    drawRect(-1.0f, -1.0f, 1.0f, 1.5f);

    glColor3f(1.0, 1.0, 1.0);
    drawRect(-1.0f, -0.5f, 1.0f, -0.3f);

    glColor3f(0.4, 0.2, 0.0);
    drawRect(-1.0f, -0.7f, 1.0f, -0.5f);

    glColor3f(0, 0, 0);
    drawRect(-0.9f, -0.7f, -0.6f, -0.5f);
    drawRect(-0.3f, -0.7f, 0.0f, -0.5f);
    drawRect(0.3f, -0.7f, 0.6f, -0.5f);
    drawRect(0.6f, -0.7f, 0.9f, -0.5f);


    glColor3f(1.0, 1.0, 0.4);
    glBegin(GL_QUADS);
    glVertex2f(xOffset + (-1.0f * scale), yOffset + (1.0f * scale));
    glVertex2f(xOffset + (-0.7f * scale), yOffset + (1.0f * scale));
    glVertex2f(xOffset + (-1.5f * scale), yOffset + (-0.4f * scale));
    glVertex2f(xOffset + (-1.8f * scale), yOffset + (-0.4f * scale));
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(xOffset + (0.7f * scale), yOffset + (1.0f * scale));
    glVertex2f(xOffset + (1.0f * scale), yOffset + (1.0f * scale));
    glVertex2f(xOffset + (1.8f * scale), yOffset + (-0.4f * scale));
    glVertex2f(xOffset + (1.5f * scale), yOffset + (-0.4f * scale));
    glEnd();
    // Legs (yellow)
    glColor3f(1.0, 1.0, 0.4);  

// Left leg
    drawRect(-0.55f, -1.0f, -0.35f, -1.5f);

// Right leg
    drawRect(0.35f, -1.0f, 0.55f, -1.5f);


    glColor3f(0.3, 0.1, 0);
    drawRect(-0.7f, -1.5f, -0.2f, -1.8f);
    drawRect(0.2f, -1.5f, 0.7f, -1.8f);

    glColor3f(0, 0, 0);
    drawCircle(-0.45f, -1.65f, 0.02f, 20);
    drawCircle(0.45f, -1.65f, 0.02f, 20);
}

void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}


void drawCircle(float cx, float cy, float radius, int num_segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);

        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void checkForWinner() {
    float topRectangleY = 1.0f;

    // SpongeBob check
    if (!isFallingSponge && startpointspy + 0.1 >= topRectangleY && winner == "") {
        winner = "SpongeBob";
        isplaying = false;
        cout << "SpongeBob Wins!" << endl;
        PlaySound(TEXT("C:/Users/PC/Documents/test6/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }

    // SquareMan check
    if (!isFallingSquare && startpointsqy + 0.1 >= topRectangleY && winner == "") {
        winner = "Square";
        isplaying = false;
        cout << "Square Wins!" << endl;
        PlaySound(TEXT("C:/Users/PC/Documents/test6/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
}


void drawWinnerText(const string& text, float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);

    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
string text2 = "To restart press r";
glRasterPos2f(x, y - 0.08f);
for (char c : text2) {
    glutBitmapCharacter(font, c);
}


}

bool isBlockedByRotatingStick(float nextX, float nextY) {
    float len = 0.58f;
    float cx = 0.0f, cy = -0.1f;  
    float rad = angle * M_PI / 180.0f;

    float x1 = cx + len * cos(rad);
    float y1 = cy + len * sin(rad);
    float x2 = cx - len * cos(rad);
    float y2 = cy - len * sin(rad);

    
    const int steps = 100;
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float px = x1 + t * (x2 - x1);
        float py = y1 + t * (y2 - y1);

        float dx = px - nextX;
        float dy = py - nextY;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 0.06f) {  
            return true;  
        }
    }

    return false;  
}



