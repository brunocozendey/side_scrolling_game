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
#include <random>
#include <string>

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
bool fallen = false;

float angle = 0.0;

long int score = 0.0;

random_device rd;
mt19937 gen(rd());

int lower_x = -8;
int upper_x = 14;

//float coins_x[5] = {-10,-5,0,2, 4};
//float holes_x[10] = {-10,-5,0,2, 4,5,8,9,10};

float coins_x[5];
float holes_x[10];

bool debug_mode = false;

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

//Generate Objects
// From -20 to 20
void generateObjects(){

    uniform_int_distribution<int> dist(lower_x, upper_x);

    for (int i = 0; i<10; i++){
        if (i<5) coins_x[i] = dist(gen);
        holes_x[i] = dist(gen);
    }
}

void renderText(float x, float y, void* font, const char* text) {
    glRasterPos2f(x, y);  // Set the starting position for text rendering
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(font, *c);  // Render each character of the text
    }
}

void printScore(){
    if (game_started) score++;
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0f,1.0f,1.0f);
    renderText(x_min+1.7, 3.65f, GLUT_BITMAP_TIMES_ROMAN_24, "Score:");
    glPopMatrix();
    glPushMatrix();
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%li", score);
    renderText(x_min+2.2, 3.65f, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
    glPopMatrix();
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
    //coinBox();
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

void drawHead(){
    glPushMatrix();
    glTranslatef(0.1,0.6,0);
    // Draw the head
    glColor3f(0.957, 0.643, 0.376);
    drawDisk(0.15);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.2,0.6,0);
    drawDisk(0.06);
    glPopMatrix();

    glPushMatrix();
    glLineWidth(2);  // Set the line width to be 6 pixels.
    glBegin(GL_LINES);
    glVertex2f(0.0,0.6);
    glVertex2f(0.2,0.6);
    glEnd();
    glPopMatrix();


}
void drawMan(){
//    manBox();
    glTranslatef(0,0.1,0);
    glRotatef(angle, 0.0f,0.0f,1.0f);
    drawHead();
    glPushMatrix();
    // Draw the body
    glColor3f(1, 0, 0);
    glLineWidth(12);  // Set the line width to be 6 pixels.
    glBegin(GL_LINES);
    glVertex2f(0.1,0.45);
    glVertex2f(0.1,-0.1);
    glEnd();

    // Draw the arms
    glLineWidth(6);
    glBegin(GL_LINES);
    glVertex2f(0.1, 0.3);
    glVertex2f(0.4, 0.3);
    glVertex2f(0.1, 0.2);
    glVertex2f(0.4, 0.2);
    glEnd();

    // Draw the legs
    glColor3f(0.0f,0.0f,1.0f);
    glLineWidth(12);
    glBegin(GL_LINES);
    if (frameNumber%5==0){
        glVertex2f(0.0, -0.4);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.2, -0.4);
    }
    else{
        glVertex2f(0.1, -0.4);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.1, 0.0);
        glVertex2f(0.1, -0.4);
    }
    glEnd();
    glColor3f(0.647f,0.165f,0.165f);
    if (frameNumber%5==0){
    glTranslatef(0.025f,-0.4f,0);
    drawDisk(0.07);
    glTranslatef(0.19f,0,0);
    drawDisk(0.07);
    }
    else{
        glTranslatef(0.1f,-0.4f,0);
        drawDisk(0.07);
    }
    glPopMatrix();
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

void drawHouse(unsigned char color){
    // Desenhar o corpo da casa (quadrado)
    glBegin(GL_QUADS);
    switch (color) {
        case 'p':
            glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
            break;
        case 'r':
            glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
            break;
        case 'g':
            glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
            break;
        case 'b':
            glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
            break;
    }

    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.5, 0.200);
    glVertex2f(-0.5, 0.200);
    glEnd();

    // Desenhar o telhado da casa (triângulo)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.5, 0.2);
    glVertex2f(0.0, 0.6);
    glVertex2f(0.5, 0.2);
    glEnd();

    // Desenhar a porta da casa (quadrado)
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(-0.1, -0.480);
    glVertex2f(0.1, -0.480);
    glVertex2f(0.1, 0.0);
    glVertex2f(-0.1, 0.0);
    glEnd();

    // Desenhar a janela da casa (quadrado)
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.290, -0.090);
    glVertex2f(0.170, -0.090);
    glVertex2f(0.170, 0.0);
    glVertex2f(0.290, 0.0);
    glEnd();

    // Desenhar a janela da casa (quadrado)
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.430, -0.090);
    glVertex2f(0.300, -0.090);
    glVertex2f(0.300, 0.0);
    glVertex2f(0.430, 0.0);
    glEnd();

    // Desenhar a janela da casa (quadrado)
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.290, -0.100);
    glVertex2f(0.170, -0.100);
    glVertex2f(0.170, -0.2);
    glVertex2f(0.290, -0.2);
    glEnd();

    // Desenhar a janela da casa (quadrado)
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.430, -0.100);
    glVertex2f(0.300, -0.100);
    glVertex2f(0.300, -0.2);
    glVertex2f(0.430, -0.2);
    glEnd();
}

void game_over(){
    game_started = false;
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glScalef(100,100,1);
    drawSquare();
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);  // Set the color for rendering (red)
    renderText(x_min+2.5, 2.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Game Over");
    glPopMatrix();

    glPushMatrix();
    renderText(x_min+2.5, 1.7f, GLUT_BITMAP_TIMES_ROMAN_24, "Your Score:");
    glPopMatrix();

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%li", score);
    renderText(x_min+3.5, 1.7f, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
}

void axis_print(){
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);  // Set the color for rendering (red)
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%f", x_min);
    renderText(x_min+0.5, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
    snprintf(buffer, sizeof(buffer), "%f", x_max);
    renderText(x_min+2.0, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
    snprintf(buffer, sizeof(buffer), "%i", frameNumber);
    renderText(x_min+3.5, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
    glPopMatrix();
}

/*
 * This function is called when the image needs to be redrawn.
 * It is installed by main() as the GLUT display function.
 * It draws the current frame of the animation.
 */
void display() {


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    x_min = (-10.0f+float(frameNumber)/100);
    x_max = (-6.0f +float(frameNumber)/100);

    if (frameNumber >= 2050){
        frameNumber = 0.0;
        generateObjects();
    }

    if (aspect >= 1.0) {
        x_min = x_min* aspect;
        x_max = x_max* aspect;
        glOrtho(x_min, x_max, -1.0, 4.0, 1.0, -1.0);
    }
    else
     glOrtho(x_min, x_max, -1.0 / aspect, 4.0 / aspect, 1.0, -1.0);


    glClear(GL_COLOR_BUFFER_BIT); // Fills the scene with blue.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    printScore();
    //Céu mudar de cor
    //Melhorar a variação de cores e também remover o sol
    //glPushMatrix();
    //glScalef(100,100,0);
    //glColor3f(0.5f-frameNumber/1000, 0.5f-frameNumber/1000, 1.0f);
    //drawSquare();
    //glPopMatrix();

    /* Draw three green triangles to form a ridge of hills in the background */
    glColor3f(0, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-40,-1);
    glVertex2f(1,-1);
    glVertex2f(1,1);
    glVertex2f(-40,1);
    glEnd();
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
    glBegin(GL_POLYGON);
    glVertex2f(11,-1);
    glVertex2f(40,-1);
    glVertex2f(40,1);
    glVertex2f(11,1);
    glEnd();

    /* Draw a bluish-gray rectangle to represent the road. */

    glColor3f(0.4f, 0.4f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-40,-0.4f);
    glVertex2f(40,-0.4f);
    glVertex2f(40,0.4f);
    glVertex2f(-25,0.4f);
    glEnd();

    /* Draw a white line to represent the stripe down the middle
     * of the road. */

    glLineWidth(6);  // Set the line width to be 6 pixels.
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2f(-40,0);
    glVertex2f(40,0);
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

    //Put hole at x position
    float hole_y = 0.0;
    for (int i=0;i<10;i++) {
        glPushMatrix();
        float hole_x = holes_x[i];
        glTranslated(hole_x, hole_y, 0);
        drawHole();
        glPopMatrix();
    }
    //Life Control
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

    glPushMatrix();
    glTranslated(0.75,1,0);
    glScaled(0.6,0.6,1);
    drawWindmill();
    glPopMatrix();

    glPushMatrix();
    glTranslated(2.2,1.6,0);
    glScaled(0.4,0.4,1);
    drawWindmill();
    glPopMatrix();

    glPushMatrix();
    glTranslated(3.7,0.8,0);
    glScaled(0.7,0.7,1);
    drawWindmill();
    glPopMatrix();

    glPushMatrix();
    glTranslated(5.5,1.6,0);
    glScaled(0.4,0.4,1);
    drawWindmill();
    glPopMatrix();

    glPushMatrix();
    glTranslated(7.5,0.8,0);
    glScaled(0.6,0.6,1);
    drawWindmill();
    glPopMatrix();

    glPushMatrix();
    glTranslated(3.7,0.8,0);
    glScaled(0.7,0.7,1);
    drawWindmill();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 1.3,0);
    glScaled(0.7,0.7,0);
    drawHouse('p');
    glPopMatrix();

    glPushMatrix();
    glTranslated(-1.5, 1.3,0);
    glScaled(0.7,0.7,0);
    drawHouse('r');
    glPopMatrix();

    glPushMatrix();
    glTranslated(-8, 1.3,0);
    glScaled(0.7,0.7,0);
    drawHouse('g');
    glPopMatrix();

    glPushMatrix();
    glTranslated(-6, 1.3,0);
    glScaled(0.7,0.7,0);
    drawHouse('b');
    glPopMatrix();

    glPushMatrix();
    glTranslated(-8, 1.3,0);
    glScaled(0.7,0.7,0);
    drawHouse('p');
    glPopMatrix();

    //Draw Stick Man
    glPushMatrix();
    glTranslated(1+x_min, h, 0);
    drawMan();
    glPopMatrix();

    // Put coin's at x position
    float coin_y = 1.7;
    for (int i=0;i<5;i++){
        float coin_x = coins_x[i];
        glPushMatrix();
        glTranslated(coin_x,coin_y,0);
        glRotatef(float(-frameNumber)*10,0,1,0);
        drawCoin();
        glPopMatrix();
    }

    //Check Hearth Contact
    for (int i = 0; i<5;i++){
        if ((1+x_min+0.3 >= coins_x[i]) and (1+x_min<=coins_x[i]+0.4) and !debug_mode){
            if (h+1.0>=coin_y){
                coins_x[i] = -50;
                if (life < 5)
                    life++;
            }
        }
    }

    //Check Hole Contact
    for (int i = 0; i<10;i++) {
        if ((1 + x_min + 0.2  > holes_x[i]) and (1 + x_min < holes_x[i] + 0.2) and !debug_mode) {
            if (h <= 0.5) {
                if (life > 0) {
                    angle = -45.0f;
                    game_started = false;
                    fallen = true;
                    h = 0.0f;
                }
                else{
                    game_over();
                }
            }
        }
    }

    //To debug
    if (debug_mode) axis_print();

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
    generateObjects();
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
void jump(){
    if (h < 1.0 and game_started and on_jump==false)
        h = 1.0;
    on_jump = true;
}

void stand_up(){
    life--;
    fallen = false;
    angle = 0.0f;
    h = 0.4f;
    frameNumber = frameNumber + 40;
}
void startGame(){
    glutTimerFunc(30,doFrame,0);
}


void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 'j' : case 'J' :
            jump();
            break;
        case 's' : case 'S' :
            if (fallen){
                stand_up();
                game_started = true;
                break;
            }
            game_started = true;
            startGame();
            break;

        case 'd':
            debug_mode = !debug_mode;
            break;
        case 'n':
            if (debug_mode) frameNumber = frameNumber+5;
            break;
        case 'b':
            if (debug_mode) frameNumber = frameNumber-5;
            break;
        case 'N':
            if (debug_mode) frameNumber = frameNumber+1;
            break;
        case 'B':
            if (debug_mode) frameNumber = frameNumber-1;
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

