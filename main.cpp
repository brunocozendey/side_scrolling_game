#if defined(__linux__)
#include <GL/glut.h>
#endif
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGl/glu.h>
    #include <GLUT/glut.h>
#endif
#include <math.h>
#include<cstdlib>
using namespace std;

const double PI = 3.141592654;

int frameNumber = 0;

float width = 700;
float height = 500;
float aspect = width / height;

float x_min = 0;
float x_max = 7;

float h = 0.4;

int life = 3;

bool game_started = false;
bool on_jump = false;
bool hit = false;

float angle = 0.0;

/*
 * Draw a 32-sided regular polygon as an approximation for a circular disk.
 * (This is necessary since OpenGL has no commands for drawing ovals, circles,
 * or curves.)  The disk is centered at (0,0) with a radius given by the
 * parameter.
 */
void drawDisk(double radius) {
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++) {
        double angle = (2*PI/32) * d;
        glVertex2d( radius*cos(angle), radius*sin(angle));
    }
    glEnd();
}

void drawSquare()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.0,-1.0);
    glVertex2f(1.0,-1.0);
    glVertex2f(1.0,1.0);
    glVertex2f(-1.0,1.0);
    glEnd();
}

/*
 * Draw a wheel, centered at (0,0) and with radius 1. The wheel has 15 spokes
 * that rotate in a clockwise direction as the animation proceeds.
 */
void drawWheel() {
    int i;
    glColor3f(0,0,0);
    drawDisk(1);
    glColor3f(0.75f, 0.75f, 0.75f);
    drawDisk(0.8);
    glColor3f(0,0,0);
    drawDisk(0.2);
    glRotatef(frameNumber*20,0,0,1);
    glBegin(GL_LINES);
    for (i = 0; i < 15; i++) {
        glVertex2f(0,0);
        glVertex2d(cos(i*2*PI/15), sin(i*2*PI/15));
    }
    glEnd();
}
//Generate How many coins will get
void generateCoin(){
    int offset = 1;
    int range = 1.5;
    int random = offset + (rand() % range);
}

void coinBox(){
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex2f(-0.2,-0.2);
    glVertex2f(0.2,-0.2);
    glVertex2f(0.2,0.2);
    glVertex2f(-0.2,0.2);
    glEnd();
}

void drawHeart(){
    glColor3f(1, 0, 0);
    drawDisk(0.15);
    glTranslatef(0.20,0,0);
    drawDisk(0.15);
    glTranslatef(-0.50,-0.31f,0);
    glBegin(GL_POLYGON);
    glVertex2f(0.2f,0.2f);
    glVertex2f(0.4f,-0.1f);
    glVertex2f(0.6f,0.2f);
    glEnd();
}

void drawCoin(){
    coinBox();
    glColor3f(1, 1, 0);
    drawDisk(0.2);
    glColor3f(0.75f, 0.75f, 0.25f);
    drawDisk(0.18);
    glTranslatef(-0.04f,0.04f,0);
    glScalef(0.4,0.4,1);
    drawHeart();
}

void manBox(){
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2f(-0.15,-0.4);
    glVertex2f(0.15,-0.4);
    glVertex2f(0.15,0.9);
    glVertex2f(-0.15,0.9);
    glEnd();
}

void drawMan(){
    manBox();
    glRotatef(angle, 0.0f,0.0f,1.0f);
    glColor3f(1, 0, 0);
    // Draw the head
    glTranslatef(0,0.7,0);
    drawDisk(0.15);

    // Draw the body
    glTranslatef(-0.1,-0.8,0);
    glLineWidth(6);  // Set the line width to be 6 pixels.
    glBegin(GL_LINES);
    glVertex2f(0.1,0.7);
    glVertex2f(0.1,-0.1);
    glEnd();

    // Draw the arms
    glBegin(GL_LINES);
    glVertex2f(0.1, 0.4);
    glVertex2f(0.4, 0.4);
    glVertex2f(0.1, 0.2);
    glVertex2f(0.4, 0.2);
    glEnd();

    // Draw the legs
    glBegin(GL_LINES);
    if (frameNumber%5==0){
        glVertex2f(0.0, -0.3);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.2, -0.3);
    }
    else{
        glVertex2f(0.1, -0.3);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.1, -0.3);
    }
    glEnd();
    glLineWidth(1);
}

void drawHole() {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawDisk(.2);
}

/*
 * Draw a sun with radius 0.5 centered at (0,0).  There are also 13 rays which
 * extend outside from the sun for another 0.25 units.
 */
void drawSun() {
    int i;
    glColor3f(1,1,0);
    for (i = 0; i < 13; i++) { // Draw 13 rays, with different rotations.
        glRotatef( 360 / 13, 0, 0, 1 ); // Note that the rotations accumulate!
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0.75f, 0);
        glEnd();
    }
    drawDisk(0.5);
    glColor3f(0,0,0);
}

/*
 * Draw a windmill, consisting of a pole and three vanes.  The pole extends from the
 * point (0,0) to (0,3).  The vanes radiate out from (0,3).  A rotation that depends
 * on the frame number is applied to the whole set of vanes, which causes the windmill
 * to rotate as the animation proceeds.  Note that this method changes the current
 * transform in the GL context gl!  The caller of this subroutine should take care
 * to save and restore the original transform, if necessary.
 */
void drawWindmill() {
    int i;
    glColor3f(0.8f, 0.8f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0);
    glVertex2f(0.05f, 0);
    glVertex2f(0.05f, 3);
    glVertex2f(-0.05f, 3);
    glEnd();
    glTranslatef(0, 3, 0);
    glRotatef(float(frameNumber)/5,0,0,1);
    glColor3f(0.4f, 0.4f, 0.8f);
    for (i = 0; i < 3; i++) {
        glRotated(120, 0, 0, 1);  // Note: These rotations accumulate.
        glBegin(GL_POLYGON);
        glVertex2f(0,0);
        glVertex2f(0.5f, 0.1f);
        glVertex2f(1.5f,0);
        glVertex2f(0.5f, -0.1f);
        glEnd();
    }
}

/*
 * This function is called when the image needs to be redrawn.
 * It is installed by main() as the GLUT display function.
 * It draws the current frame of the animation.
 */
void display() {


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    x_min = (-10.0+float(frameNumber)/100);
    x_max = (-6.0 +float(frameNumber)/100);

    if (x_max>=13)frameNumber = 0.0;


    if (aspect >= 1.0) {
        x_min = x_min* aspect;
        x_max = x_max* aspect;
//x_max = 10.5;
//x_min = -4;
        glOrtho(x_min, x_max, -1.0, 4.0, 1.0, -1.0);
    }
    else
     glOrtho(x_min, x_max, -1.0 / aspect, 4.0 / aspect, 1.0, -1.0);




    glClear(GL_COLOR_BUFFER_BIT); // Fills the scene with blue.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-25,-1);
    glVertex2f(1,-1);
    glVertex2f(1,1);
    glVertex2f(-25,1);
    glEnd();
    /* Draw three green triangles to form a ridge of hills in the background */
//    glColor3f(0, 0.6f, 0.2f);
//    glBegin(GL_POLYGON);
//    glVertex2f(-10,-1);
//    glVertex2f(-6,3.25f);
//    glVertex2f(6,-1);
//    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(-3,-1);
    glVertex2f(1.5f,1.65f);
    glVertex2f(5,-1);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(-3,-1);
    glVertex2f(3,2.1f);
    glVertex2f(7,-1);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0,-1);
    glVertex2f(6,2.5f);
    glVertex2f(20,-1);
    glEnd();
//    glBegin(GL_POLYGON);
//    glVertex2f(6,-1);
//    glVertex2f(14,2.5f);
//    glVertex2f(15,-1);
//    glEnd();
    glColor3f(0, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(11,-1);
    glVertex2f(25,-1);
    glVertex2f(25,1);
    glVertex2f(11,1);
    glEnd();

    /* Draw a bluish-gray rectangle to represent the road. */

    glColor3f(0.4f, 0.4f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-25,-0.4f);
    glVertex2f(20,-0.4f);
    glVertex2f(20,0.4f);
    glVertex2f(-25,0.4f);
    glEnd();

    /* Draw a white line to represent the stripe down the middle
     * of the road. */

    glLineWidth(6);  // Set the line width to be 6 pixels.
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2f(-25,0);
    glVertex2f(20,0);
    glEnd();
    glLineWidth(1);  // Reset the line width to be 1 pixel.

    /* Draw the sun.  The drawSun method draws the sun centered at (0,0).  A 2D translation
     * is applied to move the center of the sun to (5,3.3).   A rotation makes it rotate*/

    glPushMatrix();
    glTranslated(x_max-0.8,3.2,0);
    glRotatef(float(-frameNumber)/4,0,0,1);
    glScalef(-0.8,-0.8,1);
    drawSun();
    glPopMatrix();

    glPushMatrix();
    float coin_x = -10;
    float coin_y = 1.7;
    glTranslated(coin_x,coin_y,0);
    glRotatef(float(-frameNumber)*10,0,1,0);
    drawCoin();
    glPopMatrix();

    glPushMatrix();
    float hole_x = -12;
    float hole_y = 0.0;
    glTranslated(hole_x,hole_y,0);
    drawHole();
    glPopMatrix();

    //Life Controll
    for (int i=0;i<life;i++){
        glPushMatrix();
        glTranslated(x_min+0.2+i*0.3,3.8,0);
        glScalef(0.55,0.55,0);
        drawHeart();
        glPopMatrix();
    }


    /* Draw three windmills.  The drawWindmill method draws the windmill with its base
     * at (0,0), and the top of the pole at (0,3).  Each windmill is first scaled to change
     * its size and then translated to move its base to a different paint.  In the animation,
     * the vanes of the windmill rotate.  That rotation is done with a transform inside the
     * drawWindmill method. */

//    glPushMatrix();
//    glTranslated(0.75,1,0);
//    glScaled(0.6,0.6,1);
//    drawWindmill();
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslated(2.2,1.6,0);
//    glScaled(0.4,0.4,1);
//    drawWindmill();
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslated(3.7,0.8,0);
//    glScaled(0.7,0.7,1);
//    drawWindmill();
//    glPopMatrix();

    glPushMatrix();
    glTranslated(1+x_min, h, 0);
    drawMan();
    glPopMatrix();

    //Check Hearth Contact
    if ((1+x_min+0.3 >= coin_x) and (1+x_min<=coin_x+0.4)){
        if (h+1.3>=coin_y){
            if (life < 5)
            life++;
        }
    }

    //Check Hole Contact
    if ((1+x_min+0.3 >= hole_x) and (1+x_min<=hole_x+0.2)){
        if (h<=0.5){
            if (life >1){
                angle =-45.0f;
                game_started = false;
                hit = true;
                h = 0.0f;
            }
        }
    }
    glutSwapBuffers();

}  // end display


/*
 * This method is called from main() to initialize OpenGL.
 */
void init() {
    glClearColor(0.5f, 0.5f, 1, 1);
    // The next three lines set up the coordinates system.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( x_min,x_max, -1.0, 4.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void doFrame(int v) {
        if (game_started) frameNumber++;
        // Gambiarra para volta do pulo
        if (h>0.5  and frameNumber%5==0) h=h-0.05;
        if (h<=0.5) on_jump = false;
        glutPostRedisplay();
        glutTimerFunc(30,doFrame,0);
}

void reshape ( int w, int h) {

    width = w;
    height = h;

    glViewport(0,0,w,h);
    aspect = (GLfloat)w / (GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (aspect >= 1.0){
        x_min = x_min*aspect;
        x_max = x_max*aspect;
        glOrtho( x_min,x_max, -1.0, 4.0, 1.0, -1.0);
    }
    else
        glOrtho(x_min, x_max, -1.0/aspect, 4.0/aspect, 1.0, -1.0);

}

void startGame(){
    glutTimerFunc(30,doFrame,0);
}
void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 'j' : case 'J' :
            if (h < 1.0 and game_started and on_jump==false)
                h = 1.0;
                on_jump = true;
            break;
        case 's' : case 'S' :
            if (hit){
                life--;
                game_started = true;
                hit = false;
                angle = 0.0f;
                h = 0.4f;
                frameNumber = frameNumber + 40;
                break;
            }
            game_started = true;
            startGame();
            break;
    }
    display();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Scrolling Game");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

