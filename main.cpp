#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<math.h>
#include<iostream>
#include<bits/stdc++.h>
#include <vector>
using namespace std;
class BmpLoader
{
public:
    unsigned char* textureData;
    int iWidth, iHeight;

    BmpLoader(const char*);
    ~BmpLoader();

private:
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
};

BmpLoader::BmpLoader(const char* filename)
{
    FILE *file=0;
    file=fopen(filename, "rb");
    if(!file)
        cout<<"File not found"<<endl;
    fread(&bfh, sizeof(BITMAPFILEHEADER),1,file);
    if(bfh.bfType != 0x4D42)
        cout<<"Not a valid bitmap"<<endl;
    fread(&bih, sizeof(BITMAPINFOHEADER),1,file);
    if(bih.biSizeImage==0)
        bih.biSizeImage=bih.biHeight*bih.biWidth*3;
    textureData = new unsigned char[bih.biSizeImage];
    fseek(file, bfh.bfOffBits, SEEK_SET);
    fread(textureData, 1, bih.biSizeImage, file);
    unsigned char temp;
    for(int i=0; i<bih.biSizeImage; i+=3)
    {
        temp = textureData[i];
        textureData[i] = textureData[i+2];
        textureData[i+2] = temp;

    }

    iWidth = bih.biWidth;
    iHeight = bih.biHeight;
    fclose(file);
}

BmpLoader::~BmpLoader()
{
    delete [] textureData;
}

const int wWidth = 1920/2;
const int wHeight = 1080/2;
const float ar = float(wWidth)/float(wHeight);
bool look_point= true;


//main
GLfloat eyeX = 50;
GLfloat eyeY = 120;
GLfloat eyeZ = -10;

GLfloat lookX = 0;
GLfloat lookY = 120;
GLfloat lookZ = 0;

//upper
GLfloat eyeX1 = 100;
GLfloat eyeY1 = 500;
GLfloat eyeZ1 = -100;

GLfloat lookX1 = 0;
GLfloat lookY1 = 120;
GLfloat lookZ1 = 0;




/*
GLfloat eyeX;
GLfloat eyeY;
GLfloat eyeZ;

GLfloat lookX ;
GLfloat lookY ;
GLfloat lookZ ;
*/

GLfloat  axis_x=0.0, axis_y=0.0;
float rotX = 0, rotY = 0, rotZ = 0, theta = 0,theta1 = 0;

bool bRotate= false;
//bool light0 = false, light1 = false, light2 = false, light3= false;
bool light0 = true, light1 = true, light2 = true, light3= true;
bool amb = true, dif = true, spec = true, em = true;
bool Bool_day = true;
int rotate_sky =1;


unsigned int ID[25];
int grass = 0, floor1 = 1, floor2= 2, floor3 = 3, floor4=4, flowers = 5, flowers1 = 6, flowers2 = 7, red = 8,white=9, water=10, alpona=11, black=12,night=13;
int wood=14,leaf=15,building=16,field=17, day=18,pool=19,poster1=20,badminton=21;
float col = 0;

const double PI = 3.14159265389;


/* GLUT callback Handlers */


int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt=1;
int animat = 0;
const int L=5;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 100;
const int L1=9;
const int dgre1=5;
int ncpt1=L1+1;
const int ntheta1 = 20;


GLfloat ctrlpoints[L+1][3] =
{

    { 0.0, 0.0, 0.0},
    { 0.0, 0.54, 0.0},
    { 0.0, 1.01, 0.0},
    { 0.54, 1.01, 0.0},
    { 0.55, 0.55, 0.0},
    { 0.55, 0.0, 0.0}

};


double ex=0, ey=0, ez=15, lx=0,ly=0,lz=0, hx=0,hy=1,hz=0;

float wcsClkDn[3],wcsClkUp[3];
///////////////////////////////
class point1
{
public:
    point1()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];
int flag=0;
GLint viewport[4]; //var to hold the viewport info
GLdouble modelview[16]; //var to hold the modelview info
GLdouble projection[16]; //var to hold the projection matrix info

//////////////////////////
void scsToWcs(float sx,float sy, float wcsv[3] );
void processMouse(int button, int state, int x, int y);
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);
///////////////////////////

void scsToWcs(float sx,float sy, float wcsv[3] )
{

    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

    winX = sx;
    winY = (float)viewport[3] - (float)sy;
    winZ = 0;

    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX;
    wcsv[1]=worldY;
    wcsv[2]=worldZ;


}
void processMouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        if(flag!=1)
        {
            flag=1;
            clkpt[0].x=x;
            clkpt[0].y=y;
        }


        scsToWcs(clkpt[0].x,clkpt[0].y,wcsClkDn);
        cout<<"\nD: "<<x<<" "<<y<<" wcs: "<<wcsClkDn[0]<<" "<<wcsClkDn[1];
    }
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if (flag==1)
        {
            clkpt[1].x=x;
            clkpt[1].y=y;
            flag=0;
        }
        float wcs[3];
        scsToWcs(clkpt[1].x,clkpt[1].y,wcsClkUp);
        cout<<"\nU: "<<x<<" "<<y<<" wcs: "<<wcsClkUp[0]<<" "<<wcsClkUp[1];

        clikd=!clikd;
    }
}


//polynomial interpretation for N points

///////////////////////
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}


void curve()
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
    const double a = t*90.0;

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(wired)
    {
        glPolygonMode( GL_FRONT, GL_LINE ) ;
        glPolygonMode( GL_BACK, GL_LINE ) ;

    }
    else
    {
        glPolygonMode( GL_FRONT,GL_FILL ) ;
        glPolygonMode( GL_BACK, GL_FILL ) ;
    }

    glPushMatrix();

    if(animat)
        glRotated(a,0,0,1);

    glRotatef( anglex, 1.0, 0.0, 0.0);
    glRotatef( angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
    glRotatef( anglez, 0.0, 0.0, 1.0);

    glRotatef( 90, 0.0, 0.0, 1.0);
    glTranslated(-3.5,0,0);
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    glPushMatrix();

    glPopMatrix();

    if(shcpt)
    {
        matColor(0.0,0.0,0.9,20);
    }

    glPopMatrix();

}

static GLfloat v_cube[8][3] =
{
    {0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2}, {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}
};

static GLbyte c_ind [6][4] =
{
    {0, 2, 6, 4}, {1, 3, 7, 5}, {0, 4, 5, 1}, {2, 6, 7,3}, {0, 1, 3, 2}, {4, 5, 7, 6}
};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}


void cube(float R = 255, float G = 255, float B = 255, float alpha = 1)
{
    float colR = R/255, colG = G/255, colB = B/255;
    //glColor3f(1,1,1);
    GLfloat noMat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat matAmb [] = {colR*.2, colG*.2, colB*.2, 1};
    GLfloat matDif [] = {colR, colG, colB, 1};
    GLfloat matSpec [] = {1, 1, 1, 1};
    GLfloat matEm [] = {0.1, 0.1, 0.1, 1};
    GLfloat matShin[] = {30};

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShin);

    if(em)
    {
        glMaterialfv(GL_FRONT, GL_EMISSION, matEm);
    }
    else
    {
        glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
    }



    glBegin(GL_QUADS);
    for(GLint i = 0; i<6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
        glTexCoord2f(0,1);

        for(GLint j =0; j<4; j++)
        {
            glVertex3fv(&v_cube[c_ind[i][j]][0]);
        }
    }
    glEnd();
}


void light()
{
    //Light
    glEnable(GL_LIGHTING);
    GLfloat noLight[] = {0, 0, 0, 1};
    GLfloat lightAmb[] = {0.5, 0.5, 0.5, 1};
    GLfloat lightDif[] = {1, 1, 1, 1};
    GLfloat lightSpec[] = {1, 1, 1, 1};
    GLfloat light1Pos[] = {0, -500, 0, 1};
    GLfloat light4Pos[] = {0,500,0, 1};

    // GLfloat light1Pos[] = {90, 90, 90, 1};
    //  GLfloat light4Pos[] = {90, 90, -90, 1};
    GLfloat light2Pos[] = {683, 300, -350, 1}; //spot light
    GLfloat light3Pos[] = {-380, 400, -50, 1}; //spot light  GLfloat light2Pos[] = {15, 40, -45, 1}; //spot light
    // GLfloat light3Pos[] = {15, 40, 45, 1

    glEnable(GL_LIGHT0);  //1
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, light1Pos);


    glEnable(GL_LIGHT1); //2 spot light
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT1, GL_POSITION, light2Pos);

    glEnable(GL_LIGHT2); //3 spot light
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT2, GL_POSITION, light3Pos);

    glEnable(GL_LIGHT3); //4
    glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT3, GL_POSITION, light4Pos);


    GLfloat spotDirection[] = {0, -1, 0, 1};   //2
    GLfloat spotCutOff[] = {60};

    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutOff);

    GLfloat spotDirection2[] = {0, -1, 0, 1}; //3
    GLfloat spotCutOff2[] = {60};

    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection2);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutOff2);


    if(light0)
    {
        glEnable(GL_LIGHT0);
    }
    else
    {
        glDisable(GL_LIGHT0);
    }

    if(light1)
    {
        glEnable(GL_LIGHT1);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    if(light2)
    {
        glEnable(GL_LIGHT2);
    }
    else
    {
        glDisable(GL_LIGHT2);
    }
    if(light3)
    {
        glEnable(GL_LIGHT3);
    }
    else
    {
        glDisable(GL_LIGHT3);
    }


    if(amb)
    {
        glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmb);
        glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmb);
        glLightfv(GL_LIGHT2,GL_AMBIENT,lightAmb);
        glLightfv(GL_LIGHT3,GL_AMBIENT,lightAmb);

    }
    else
    {
        glLightfv(GL_LIGHT0,GL_AMBIENT,noLight);
        glLightfv(GL_LIGHT1,GL_AMBIENT,noLight);
        glLightfv(GL_LIGHT2,GL_AMBIENT,noLight);
        glLightfv(GL_LIGHT3,GL_AMBIENT,noLight);
    }

    if(dif)
    {
        glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDif);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDif);
        glLightfv(GL_LIGHT2,GL_DIFFUSE,lightDif);
        glLightfv(GL_LIGHT3,GL_DIFFUSE,lightDif);
    }
    else
    {
        glLightfv(GL_LIGHT0,GL_DIFFUSE,noLight);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,noLight);
        glLightfv(GL_LIGHT2,GL_DIFFUSE,noLight);
        glLightfv(GL_LIGHT3,GL_DIFFUSE,noLight);
    }

    if(spec)
    {
        glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpec);
        glLightfv(GL_LIGHT1,GL_SPECULAR,lightSpec);
        glLightfv(GL_LIGHT2,GL_SPECULAR,lightSpec);
        glLightfv(GL_LIGHT3,GL_SPECULAR,lightSpec);
    }
    else
    {
        glLightfv(GL_LIGHT0,GL_SPECULAR,noLight);
        glLightfv(GL_LIGHT1,GL_SPECULAR,noLight);
        glLightfv(GL_LIGHT2,GL_SPECULAR,noLight);
        glLightfv(GL_LIGHT3,GL_SPECULAR,noLight);
    }

}

void axes()
{
    float length = 30;
    float width = 0.3;

    //X-axis
    glPushMatrix();
    glTranslatef(length/2, 0, 0);
    glScalef(length, width, width);
    glTranslatef(-0.5, -0.5, -0.5);
    cube(255, 0, 0);
    glPopMatrix();

    //Y-axis
    glPushMatrix();
    glTranslatef(0, length/2, 0);
    glScalef(width, length, width);
    glTranslatef(-0.5, -0.5, -0.5);
    cube(0, 255, 0);
    glPopMatrix();

    //Z-axis
    glPushMatrix();
    glTranslatef(0, 0, length/2);
    glScalef(width, width, length);
    glTranslatef(-0.5, -0.5, -0.5);
    cube(0, 0, 255);
    glPopMatrix();
}



static void resize(int width, int height)
{
    //const float ar = (float) width / (float) height;
    glViewport(0, 0, width, width/ar);

}

void cylinder(float w, float h, float l,float r,float g,float b)
{
    int numberOfCube = 360;
    float width = w;
    float height= h;
    float length = l;
    for(double i=0; i<=360; i=i+1.1)
    {
        glPushMatrix();
        glRotated(i, 0, 1, 0);
        glScalef(width,height,length);
        glTranslatef(-0.5, -0.5, -0.5);
        cube(r,g,b);
        glPopMatrix();
    }
}

void main_floor()
{
    glBindTexture(GL_TEXTURE_2D, ID[white]);//main floor
    glPushMatrix();
    glScalef(900,1,1040);
    glTranslatef(-1,-1,-1);
    cube(	139, 69, 19);  //165, 42, 42
    glPopMatrix();

}

void circle_3D(GLdouble radius)
{
    GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);

    glRotatef(270, 1, 0, 0);
    gluSphere(qobj, radius, 20, 20);
    gluDeleteQuadric(qobj);

}

void cylinder_3D(GLdouble height,GLdouble rad,GLdouble r2)
{
    GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    glRotatef(90, 1, 0, 0);

    gluCylinder(qobj,  rad, r2, height, 20, 20);
    gluDeleteQuadric(qobj);

}
void sub_tree()
{
    glBindTexture(GL_TEXTURE_2D, ID[wood]);
    glPushMatrix();
    glTranslatef(0,40,0);
    cylinder_3D(25,1,1);//base
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[leaf]);
    glPushMatrix();
    glTranslatef(0,40,0);
    circle_3D(10); //leaf
    glPopMatrix();

}

void sub_tree_upper()
{
    glPushMatrix();
    glTranslatef(0,90,0);
    glRotatef(90,0,1,0);
    glScalef(2,1,2);
    sub_tree();  //1
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,88,0);
    glRotatef(10,1,0,0);
    glScalef(2,1,2);
    sub_tree();  //1
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,88,0);
    glRotatef(-10,1,0,0);
    glScalef(2,1,2);
    sub_tree();  //1
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,90,0);
    glRotatef(10,0,0,1);
    glScalef(2,1,2);
    sub_tree();  //1
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0,90,0);
    glRotatef(-10,0,0,1);
    glScalef(2,1,2);
    sub_tree();  //1
    glPopMatrix();
}
void tree()
{

//11111111111111111111111111111111111111


    glPushMatrix();
    glTranslatef(0,95,-8);
    glRotatef(55,1,0,0);
    glScalef(0.7,0.7,0.7);
    sub_tree();  //1
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,95,8);
    glRotatef(-55,1,0,0);
    glScalef(0.7,0.7,0.7);
    sub_tree();  //1
    glPopMatrix();


//3333333333333333333333333

    glPushMatrix();
    sub_tree_upper();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,10,0);
    glScalef(0.8,1,0.8);
    sub_tree_upper();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, ID[wood]);
    glPushMatrix();
    glTranslatef(0,120,0);
    cylinder_3D(80,4,8);  //tree base
    glPopMatrix();

}

floor_grass()

{
    glPushMatrix();
    glScalef(250,5,250);
    glTranslatef(-1,-1,-1);
    cube(0, 255, 0);
    glPopMatrix();


}

floor_num1()
{
    glPushMatrix();
    glTranslatef(0,12,0);
    //glTranslatef(-25,1,-20);
    glScalef(100,5,150);
    glTranslatef(-1,-1,-1);
    cube(128, 0, 0);
    glPopMatrix();
}

floor_num2()
{
    glPushMatrix();
    glTranslatef(-3.9,21,0);
    glScalef(96,5,142);
    glTranslatef(-1,-1,-1);
    cube(40,40,40);
    glPopMatrix();
}
floor_num3()
{

    glPushMatrix();
    glTranslatef(-8.8,30,0);
    glScalef(90,5,132);
    glTranslatef(-1,-1,-1);
    cube(128, 10, 10);
    glPopMatrix();


}
pond()
{
    glPushMatrix();
    glTranslatef(-355,1,-30);
    glScalef(200,5,252);
    glTranslatef(-1,-1,-1);
    cube(255, 255, 255);
    glPopMatrix();
}

floor_set()
{
    glBindTexture(GL_TEXTURE_2D, ID[grass]);
    floor_grass();
    glBindTexture(GL_TEXTURE_2D, ID[water]);
    pond();


    glPushMatrix();   //main_stage
    glScalef(1,1,1.25);
    glBindTexture(GL_TEXTURE_2D, ID[floor4]);
    floor_num1();
}


void road()
{
    glBindTexture(GL_TEXTURE_2D, ID[floor4]);
    glPushMatrix();
    glTranslatef(100,5,-50);
    glScalef(75,1,35);
    cube(165, 42, 42);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[white]);

    glPushMatrix();
    glTranslatef(100,5.5,-55);
    glScalef(75,1,3);
    cube(255,255,255);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(100,5.5,15);
    glScalef(75,1,3);
    cube(255,255,255);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(180,-60,-150);
    glScalef(1.5,1.5,1.5);
    tree();   //sohid minar tree
    glPopMatrix();

    glPushMatrix();
    glTranslatef(180,-60,130);
    glScalef(1.5,1.5,1.5);
    tree();   //sohid minar tree2
    glPopMatrix();


}


void spot_light_position()
{
    glBindTexture(GL_TEXTURE_2D, ID[black]);   //spot_light_tube1
    glPushMatrix();
    glTranslatef(90,0,0);
    glRotatef(-35,0,1,0);
    glTranslatef(50,35,135);
    glScalef(2,5,5);
    glRotatef(-25,0,0,1);
    cube(0,0,0);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[black]);   //spot_light_tube2
    glPushMatrix();
    glTranslatef(120,0,0);
    glRotatef(35,0,1,0);
    glTranslatef(0,30,-165);
    glScalef(2,5,5);
    glRotatef(25,0,0,1);
    cube(0,0,0);
    glPopMatrix();
}

void sohid_minar()
{
    floor_set();
    road();

    glPopMatrix();
    spot_light_position();

}



void goal_bar()
{
    glPushMatrix();  //goal bar1
    glTranslatef(0,0,-25);
    glScalef(2,25,2);
    cube(255,255,255);
    glPopMatrix();

    glPushMatrix();//goal bar2
    glTranslatef(0,0,25);
    glScalef(2,25,2);
    cube(255,255,255);
    glPopMatrix();

    glPushMatrix();//goal mid bar
    glTranslatef(0,50,-24.25);
    glScalef(2,2,26.5);
    cube(255,255,255);
    glPopMatrix();

}


void gallery()
{

//treeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
    glPushMatrix();
    glTranslatef(270,-60,190);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();

    glPushMatrix();
    glTranslatef(291,-60,-150);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();



    glPushMatrix();
    glTranslatef(291,-60,30);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();
//downs side tree
    glPushMatrix();
    glTranslatef(-270,-60,210);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-290,-60,100);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-290,-60,-70);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-291,-60,-150);
    glScalef(1.5,1.5,1.5);
    tree();   //field tree
    glPopMatrix();

}

void football_field()
{

    glBindTexture(GL_TEXTURE_2D, ID[grass]);//football field

    glPushMatrix();
    glTranslatef(0,5,0);
    glScalef(400,5,280);
    glTranslatef(-1,-1,-1);
    cube(0, 255, 0);
    glPopMatrix();




    glBindTexture(GL_TEXTURE_2D, ID[field]);//football field
    glPushMatrix(); //field
    glRotatef(90,0,1,0);
    glTranslatef(0,6,0);
    glScalef(200,5,250);
    glTranslatef(-1,-1,-1);
    cube(255, 255, 255);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[white]);//football field
    glPushMatrix();
    glTranslatef(-235,0,0);  //bar1
    goal_bar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(235,0,0); //bar2
    goal_bar();
    glPopMatrix();



    glPushMatrix();

    gallery();
    glPopMatrix();

}

void minar_field()
{
    sohid_minar();
    glPushMatrix();
    glTranslatef(-150,-2,500);
    football_field();
    glPopMatrix();
}

void main_roads1()
{
    glBindTexture(GL_TEXTURE_2D, ID[black]);
    glPushMatrix();
    glTranslatef(100,5,-50);
    glScalef(585,1,90);
    cube(165, 42, 42);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[white]);

    glPushMatrix();
    glTranslatef(100,5.5,-55);
    glScalef(585,1,3);
    cube(255,255,255);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(100,5.5,125);
    glScalef(630,1,3);
    cube(255,255,255);
    glPopMatrix();

//treeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
    glPushMatrix(); //roads1 tree
    glTranslatef(150,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(290,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(330,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(460,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(520,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();



    glPushMatrix(); //roads1 tree
    glTranslatef(690,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();


    glPushMatrix(); //roads1 tree
    glTranslatef(860,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(920,-55.5,-100);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();
//-------------------------------------------------------
    glPushMatrix(); //roads1 tree
    glTranslatef(150,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(290,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(330,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(460,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(520,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(550,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(690,-55.5,170);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();


    glPushMatrix(); //roads1 tree
    glTranslatef(1160,-55.5,170);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads1 tree
    glTranslatef(1120,-55.5,170);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();


}

void main_roads2()
{
    glBindTexture(GL_TEXTURE_2D, ID[black]);
    glPushMatrix();
    glTranslatef(100,5,-50);
    glScalef(505,1,90);
    cube(165, 42, 42);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[white]);

    glPushMatrix();
    glTranslatef(100,5.5,-55);
    glScalef(418,1,3);
    cube(255,255,255);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(100,5.5,125);
    glScalef(415,1,3);
    cube(255,255,255);
    glPopMatrix();



    glPushMatrix(); //roads2 tree
    glTranslatef(400,-55.5,-85);
    glScalef(1.5,1.5,1.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads2 tree
    glTranslatef(720,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads2 tree
    glTranslatef(580,-55.5,-85);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

}

void main_roads3()
{
    glBindTexture(GL_TEXTURE_2D, ID[black]);
    glPushMatrix();
    glTranslatef(100,5,-50);
    glScalef(505,1,90);
    cube(165, 42, 42);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, ID[white]);

    glPushMatrix();
    glTranslatef(100,5.5,-55);
    glScalef(505,1,3);
    cube(255,255,255);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(56,0,0);
    glTranslatef(100,5.5,125);
    glScalef(480,1,3);
    cube(255,255,255);
    glPopMatrix();


    glPushMatrix(); //roads3 tree
    glTranslatef(180,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(280,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(360,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(780,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(980,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(680,-55.5,-95);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();
//---------------------------


    glPushMatrix(); //roads3 tree
    glTranslatef(190,-55.5,185);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(295,-55.5,185);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(390,-55.5,185);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(880,-55.5,185);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(980,-55.5,185);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

    glPushMatrix(); //roads3 tree
    glTranslatef(780,-55.5,185);
    glScalef(2.5,1.5,2.5);
    tree();
    glPopMatrix();

}

void roads()
{
    glPushMatrix(); //main_roads1
    glTranslatef(-22,0,1120);
    glRotatef(90,0,1,0);
    main_roads1();
    glPopMatrix();

    glPushMatrix(); //main_roads2
    glTranslatef(-1000,0,-280);
    //glRotatef(90,0,1,0);
    main_roads2();
    glPopMatrix();

    glPushMatrix(); //main_roads3
    glTranslatef(-100,0,-220);
    glRotatef(45,0,1,0);
    main_roads3();
    glPopMatrix();


}

void swimming_pool()
{
    glBindTexture(GL_TEXTURE_2D, ID[pool]);
    glPushMatrix(); //swiming pool
    glTranslatef(50,0,-70);
    glTranslatef(-910,0,-925);
    glScalef(410,1,295);
    cube(255,255,255);
    glPopMatrix();
}


void sky()
{
    glRotatef(theta1, 1,0,0);
    glPushMatrix();
    circle_3D(1300);
    glPopMatrix();

}


float rot;

// ghooooooooooor

void drawpyramid()
{
    static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.2},
    {1.2, 0.0, 1.2},
    {1.2, 0.0, 0.0},
    {0.6, 0.5, 0.6},
};
static GLfloat v_square[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.1, 0.0, 0.0},
    {0.1, 0.1, 0.0},
    {0.0, 0.1, 0.0},


};
static GLfloat v_door[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.3, 0.0, 0.0},
    {0.3, 0.5, 0.0},
    {0.0, 0.5, 0.0},


};
static GLfloat v_cube[8][3]=
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0},

    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0}

};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};


static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

static GLbyte q_Indices[6][4]=
{
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {1, 2, 6, 5},
    {2, 3, 7, 6},
    {0, 3, 7, 4},
    {0, 1, 5, 4}
};

static GLfloat colors[7][3] =
{
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.5, 1.0, 1.0},
    {1.0, 0.0, 0.0}
};

    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        GLfloat mat_ambient[] = { colors[i][0],colors[i][1],colors[i][2] };
        GLfloat mat_diffuse[] = { colors[i][0]/2,colors[i][1]/2,colors[i][2]/2 };
        GLfloat mat_specular[] = { 1.0,1.0,1.0 };
        GLfloat mat_shininess[] = {70};

        glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv( GL_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_BACK, GL_SHININESS, mat_shininess);

//        glColor3f(colors[i][0],colors[i][1],colors[i][2]);
        glVertex3fv(&v_pyramid[p_Indices[i][0]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][1]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][2]][0]);
    }
    glEnd();

    glBegin(GL_QUADS);
//    glColor3f(0.1f, 0.1f, 0.1f);
    glVertex3fv(&v_pyramid[quadIndices[0][0]][0]);
    glVertex3fv(&v_pyramid[quadIndices[0][1]][0]);
    glVertex3fv(&v_pyramid[quadIndices[0][2]][0]);
    glVertex3fv(&v_pyramid[quadIndices[0][3]][0]);

    glEnd();


}
void drawcube()
{
//    glColor3f(1,0,0);

static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.2},
    {1.2, 0.0, 1.2},
    {1.2, 0.0, 0.0},
    {0.6, 0.5, 0.6},
};
static GLfloat v_square[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.1, 0.0, 0.0},
    {0.1, 0.1, 0.0},
    {0.0, 0.1, 0.0},


};
static GLfloat v_door[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.3, 0.0, 0.0},
    {0.3, 0.5, 0.0},
    {0.0, 0.5, 0.0},


};
static GLfloat v_cube[8][3]=
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0},

    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0}

};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};


static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

static GLbyte q_Indices[6][4]=
{
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {1, 2, 6, 5},
    {2, 3, 7, 6},
    {0, 3, 7, 4},
    {0, 1, 5, 4}
};

static GLfloat colors[7][3] =
{
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.5, 1.0, 1.0},
    {1.0, 0.0, 0.0}
};

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
//        glColor3f(colors[i][0],colors[i][1],colors[i][2]);
        GLfloat mat_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
        GLfloat mat_diffuse[] = { 0.5,0,0, 0.0 ,1.0};
        GLfloat mat_specular[] = { 1.0,1.0,1.0, 1.0 };
//        GLfloat mat_specular[] = { 0.0,0.0,0.0, 0.0 };
        GLfloat mat_shininess[] = {30};

        glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

        glMaterialfv( GL_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_BACK, GL_SHININESS, mat_shininess);

        glVertex3fv(&v_cube[q_Indices[i][0]][0]);
        glVertex3fv(&v_cube[q_Indices[i][1]][0]);
        glVertex3fv(&v_cube[q_Indices[i][2]][0]);
        glVertex3fv(&v_cube[q_Indices[i][3]][0]);
    }
    glEnd();
}
void fenchcube()
{

static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.2},
    {1.2, 0.0, 1.2},
    {1.2, 0.0, 0.0},
    {0.6, 0.5, 0.6},
};
static GLfloat v_square[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.1, 0.0, 0.0},
    {0.1, 0.1, 0.0},
    {0.0, 0.1, 0.0},


};
static GLfloat v_door[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.3, 0.0, 0.0},
    {0.3, 0.5, 0.0},
    {0.0, 0.5, 0.0},


};
static GLfloat v_cube[8][3]=
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0},

    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0}

};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};


static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

static GLbyte q_Indices[6][4]=
{
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {1, 2, 6, 5},
    {2, 3, 7, 6},
    {0, 3, 7, 4},
    {0, 1, 5, 4}
};

static GLfloat colors[7][3] =
{
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.5, 1.0, 1.0},
    {1.0, 0.0, 0.0}
};

//    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
//        glColor3f(colors[i+1][0],colors[i+1][1],colors[i+1][2]);

        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { colors[i+1][0],colors[i+1][1],colors[i+1][2] , 1};
        GLfloat mat_diffuse[] = { colors[i+1][0]/2,colors[i+1][1]/2,colors[i+1][2]/2, 1 };
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = {20};

        glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

        glMaterialfv( GL_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_BACK, GL_SHININESS, mat_shininess);

        glVertex3fv(&v_cube[q_Indices[i][0]][0]);
        glVertex3fv(&v_cube[q_Indices[i][1]][0]);
        glVertex3fv(&v_cube[q_Indices[i][2]][0]);
        glVertex3fv(&v_cube[q_Indices[i][3]][0]);
    }
    glEnd();
}

void drawsquare()
{
//    glColor3f(1,1,0);
static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.2},
    {1.2, 0.0, 1.2},
    {1.2, 0.0, 0.0},
    {0.6, 0.5, 0.6},
};
static GLfloat v_square[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.1, 0.0, 0.0},
    {0.1, 0.1, 0.0},
    {0.0, 0.1, 0.0},


};
static GLfloat v_door[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.3, 0.0, 0.0},
    {0.3, 0.5, 0.0},
    {0.0, 0.5, 0.0},


};
static GLfloat v_cube[8][3]=
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0},

    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0}

};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};


static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

static GLbyte q_Indices[6][4]=
{
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {1, 2, 6, 5},
    {2, 3, 7, 6},
    {0, 3, 7, 4},
    {0, 1, 5, 4}
};

static GLfloat colors[7][3] =
{
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.5, 1.0, 1.0},
    {1.0, 0.0, 0.0}
};



    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 1.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.25, 0.5, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {20};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_BACK, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <4; i++)
    {

        glVertex3fv(&v_square[quadIndices[0][i]][0]);

    }
    glEnd();
}

void drawdoor()
{
//    glColor3f(1,1,0);
static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.2},
    {1.2, 0.0, 1.2},
    {1.2, 0.0, 0.0},
    {0.6, 0.5, 0.6},
};
static GLfloat v_square[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.1, 0.0, 0.0},
    {0.1, 0.1, 0.0},
    {0.0, 0.1, 0.0},


};
static GLfloat v_door[4][3] =
{
    {0.0, 0.0, 0.0},
    {0.3, 0.0, 0.0},
    {0.3, 0.5, 0.0},
    {0.0, 0.5, 0.0},


};
static GLfloat v_cube[8][3]=
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0},

    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0}

};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};


static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

static GLbyte q_Indices[6][4]=
{
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {1, 2, 6, 5},
    {2, 3, 7, 6},
    {0, 3, 7, 4},
    {0, 1, 5, 4}
};

static GLfloat colors[7][3] =
{
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {0.5, 1.0, 1.0},
    {1.0, 0.0, 0.0}
};

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.5, 0.5, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {20};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glMaterialfv( GL_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_BACK, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <4; i++)
    {
        glVertex3fv(&v_door[quadIndices[0][i]][0]);
    }
    glEnd();
}



void house()
{
    glBindTexture(GL_TEXTURE_2D, ID[building]);
    glPushMatrix();
    glScalef(3,3,3);
    drawcube();
    glTranslatef(-0.1, 1,-0.1);
    drawpyramid();
    glTranslatef(0.1, -1,0.1);
    glTranslatef(.3, 0, 1.01);
    drawdoor();
    glTranslatef(.4,.4,0);
    drawsquare();
    glTranslatef(.11,0,0);
    drawsquare();
    glTranslatef(0,.11,0);
    drawsquare();
    glTranslatef(-.11,0,0);
    drawsquare();
    glPopMatrix();

    for(GLfloat i=-1; i<=4; i+=.5)
    {
        glPushMatrix();
        glTranslatef(i,0,4);
        glScalef(.1,1,.1);
        fenchcube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i,0,-1);
        glScalef(.1,1,.1);
        fenchcube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1,0,i);
        glScalef(.1,1,.1);
        fenchcube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(4,0,i);
        glScalef(.1,1,.1);
        fenchcube();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(-1.2,0.3,4);
    glScalef(5.4,0.1,0.1);
    fenchcube();
    glTranslatef(0,3,0);
    fenchcube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.2,.3,-1);
    glScalef(5.4,.1,.1);
    fenchcube();
    glTranslatef(0,3,0);
    fenchcube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1,.3,-1.2);
    glScalef(.1,.1,5.4);
    fenchcube();
    glTranslatef(0,3,0);
    fenchcube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4,.3,-1.2);
    glScalef(.1,.1,5.4);
    fenchcube();
    glTranslatef(0,3,0);
    fenchcube();
    glPopMatrix();

}


//ghoooooooooooooor


// variable
GLfloat dxx =0, dyy = 0, dzz = 0;

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-3, 3, -3, 3, 2.0, 2500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glViewport(0, 0, width, height);

        if(look_point==false)
        {
            gluLookAt(eyeX1, eyeY1, eyeZ1, lookX1, lookY1, lookZ1, 0, 1, 0);
        }
        else
        {

            gluLookAt( eyeX, eyeY, eyeZ,lookX+dxx, lookY+dyy, lookZ+dzz, 0, 1, 0);  //eyeX, eyeY, eyeZ,
        }


    glRotatef(theta, axis_x,axis_y,0);

    // glClearColor(65, 105, 225,1); // clear

    rot=2.3;
    glEnable(GL_TEXTURE_2D);


   glClearColor(1,1,1,1); // clear
    // glBindTexture(GL_TEXTURE_2D, ID[]);

    //main elements
    main_floor();

    // ghor 1
    glPushMatrix();
    glTranslatef(-300,0, 0);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();

    // ghor 2
    glPushMatrix();
    glTranslatef(200,0, 0);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();


    // ghor 3
    glPushMatrix();
    glTranslatef(300,0, 0);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();


    // ghor 3
    glPushMatrix();
    glTranslatef(400,0, 10);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();

    //ghor 4
    glPushMatrix();
    glTranslatef(400,0, 10);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();

    //ghor 5
    glPushMatrix();
    glTranslatef(50,0, -770);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(200,0, -900);
    glScalef(10, 60, 50);
    house();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-330,0,100);
    minar_field();
    glPopMatrix();

    roads();


    glPushMatrix();
    swimming_pool();
    glPopMatrix();


    if(Bool_day==true)
    {
        glBindTexture(GL_TEXTURE_2D, ID[day]);
        glPushMatrix();
        sky();
        glPopMatrix();
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, ID[night]);
        glPushMatrix();
        sky();
        glPopMatrix();

    }

    glBindTexture(GL_TEXTURE_2D, ID[badminton]);
    glPushMatrix();
    glTranslated(165,5,530);
    glScalef(350,1,230);
    cube(	50, 205, 50);
    glPopMatrix();



    /*--------------------------------------------------
    */


    glDisable(GL_TEXTURE_2D);
    // axes();
    light();

    glFlush();
    glutSwapBuffers();
}

void LoadTexture(const char*filename, int index)
{
    glGenTextures(1, &ID[index]);
    glBindTexture(GL_TEXTURE_2D, ID[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void texture()
{

    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\sky2.bmp", day);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\night2.bmp", night);

    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\grass.bmp", grass);



    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\white2.bmp", white);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\water.bmp", water);

    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\brick.bmp", floor1);

    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\brick4.bmp", floor2);

    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\water.bmp", water);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\red.bmp", red); //
    //  LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\flowers3.bmp", flowers1);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\flowers4.bmp", flowers2);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\black.bmp", black);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\building.bmp", building);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\tree_base2.bmp", wood);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\leaf2.bmp", leaf);

    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\pool2.bmp",pool);
    LoadTexture("E:\\Academics\\4-2\\CSE 4208 Grpahics Lab\\project_rough\\bmp\\field2.bmp", field);

    //

}
GLfloat upX = 0, upY = 1, upZ=0;
float yaw = 0, pitch = 0, roll = 0;

static void keyBoard(unsigned char key, int x, int y)
{
    float x1, z1, r, theta, dx, dz, dx_norm, dz_norm, r1=1, turn_angle_step=10, height_diff_one_less, height_diff_thresh_dist;

    x1=lookX-eyeX;
    z1=lookZ-eyeZ;
    r=sqrt(x1*x1+z1*z1);

    if(x1==0)
    {
        if(z1>0)
        {
            theta = 90;
        }
        else if(z1<0)
        {
            theta = -90;
        }
    }
    else
        theta=atan(z1/x1) * 180 / 3.1416;

    if((z1>0 && theta<0) || (z1<0 && theta>0))
        theta += 180;
    else if(z1<0 && theta<0)
    {
        theta += 360;
    }

    dx = r1*cos(theta * 3.1416 / 180);
    dz = r1*sin(theta * 3.1416 / 180);

    dx_norm = r1*cos((theta-90) * 3.1416 / 180);
    dz_norm = r1*sin((theta-90) * 3.1416 / 180);


    switch (key)
    {
    case 'R':
    case 'r':
        bRotate = !bRotate;
        axis_x=0.0;
        axis_y=1.0;
        break;


    // move look point
    case 'a':
        theta-=turn_angle_step;
        theta = theta * 3.1416 / 180;
        lookX=r*cos(theta)+eyeX;
        lookZ=r*sin(theta)+eyeZ;
        break;
    case 'w':
        lookY=lookY+3;
        break;
    case 's':
        lookY=lookY-3;
        break;
    case 'd':
        theta+=turn_angle_step;
        theta = theta * 3.1416 / 180;
        lookX=r*cos(theta)+eyeX;
        lookZ=r*sin(theta)+eyeZ;
        break;

    // Moving the camera front-back-left-right
    case 'j':
        eyeX += dx_norm*4;
        eyeZ += dz_norm*4;


        lookX += dx_norm*4;
        lookZ += dz_norm*4;
        break;
    case 'i':
        eyeX += dx*4;
        eyeZ += dz*4;
        lookX += dx*4;
        lookZ += dz*4;
        break;
    case 'k':
        eyeX -= dx*4;
        eyeZ -= dz*4;

        lookX -= dx*4;
        lookZ -= dz*4;
        break;
    case 'l':
        eyeX -= dx_norm*4;
        eyeZ -= dz_norm*4;

        lookX -= dx_norm*4;
        lookZ -= dz_norm*4;
        break;

    case '5':
        eyeY += 0.5;
        lookY +=0.5;
        break;
    case '6':
        eyeY -= 0.5;
        lookY -=0.5;
        break;
    case '\'':
        rotY++;
        if(rotY>360)
        {
            rotY = rotY - 360;
        }
        break;
    case '7':
        rotY--;
        if(rotY < 0)
        {
            rotY = rotY + 360;
        }
        break;

    case 'y':

        yaw+=1;

        if( yaw==90 || yaw ==0)
        {
            yaw=0;
            dxx=0;
            break;
        }
        dxx =  abs(eyeZ-lookZ)*tan(yaw*3.1415/180);
        break;

    case 'u':

        yaw-=1;

        if(yaw==-90  || yaw==0)
        {
            yaw = 0;
            dxx=0;
            break;
        }

        dxx = abs(eyeZ-lookZ)*tan(yaw*3.1415/180);

        break;
    //------------------------------ yaw ses -----------------------------//

    //------------------------------------pitch ------------------//
    case 'p':

        pitch+=1;

        if( pitch==90  || pitch ==0)
        {
            pitch=0;
            dyy=0;
            break;
        }
        dyy =  abs(eyeZ-lookZ)*tan(pitch*3.1415/180);
        break;

    case 't':
        pitch-=1;
        if(pitch==-90  || pitch==0)
        {
            pitch = 0;
            dyy=0;
            break;
        }
        dyy = abs(eyeZ-lookZ)*tan(pitch*3.1415/180);

        break;
    //------------------------------------pitch ses ------------------//

    //----------------------------- roll ----------------------------//
    case 'v':
        roll+=0.5;
        upX = cos(roll);
        upY= sin(roll);

        break;
    case 'o':
        roll-=0.5;

        upX = cos(roll);
        upY= sin(roll);
        break;

    case '1':
        light0 = !light0;
        break;
    case '2':
        light1 = !light1;
        break;
    case '3':
        light2 = !light2;
        break;
    case '4':
        light3 = !light3;
        break;
    case 'z':
        amb = !amb;
        break;
    case 'c':
        dif = !dif;
        break;
    case 'x':
        spec = !spec;
        break;
    case '.':
        em = !em;
        break;
    case 'q':
        look_point=!look_point;

        break;
    }
    glutPostRedisplay();
}


bool flag_dir= true;

static void idle(void)
{
    if (bRotate == true)
    {
        theta += rot;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (rotate_sky)
    {
        theta1 += 2;
        if( theta1>360  )
        {
            theta1=0;
            Bool_day=!Bool_day;

        }

    }

    glutPostRedisplay();
}



const GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 1.0f };

void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back, float ambFactor, float specFactor)
{

    const GLfloat mat_ambient[]    = { kdr*ambFactor, kdg*ambFactor, kdb*ambFactor, 1.0f };
    const GLfloat mat_diffuse[]    = { kdr, kdg, kdb, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f*specFactor, 1.0f*specFactor, 1.0f*specFactor, 1.0f };
    const GLfloat high_shininess[] = { shiny };
    if(frnt_Back==0)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==1)
    {
        glMaterialfv(GL_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_BACK, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==2)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
    }

}

/* Program entry point */
void myInit()
{
    // glClearColor(.1,.1,.1,1);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(wWidth, wHeight);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("1507099-Project");

    texture();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyBoard);
    glutIdleFunc(idle);
    glutReshapeFunc(resize);
    //glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);

    myInit();


    // glutKeyboardFunc(key);
    glutMouseFunc(processMouse);
    glutIdleFunc(idle);


    printf("Use 'w' to look up, 's' to look down, 'd' to look right, and 'a' to look left.\n");
    printf("Use 'i' to move camera up, 'k' to move camera down, 'l' to move camera right, and 'j' to move camera left with the look at point fixed.\n");
    //printf("Use '+' to zoom in and '-' to zoom out.\n\n\n");

    glutMainLoop();

    return 0;

}
