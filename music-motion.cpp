// File: music-motion.cpp
// Author: Edward Ly
// Last Updated: 16 September 2016
//
// A PC rhythm game developed with OpenGL where you move a controller (e.g. mouse) in time with the music.
// The game may support additional peripherals for the purposes of music making or conducting.

// OpenGL Libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

// C++ Libraries
#include <cmath>

struct GLintPoint {
  int x, y;
};

struct Window {
  int left, right, bottom, top;
  int width, height;
};

// global variables
GLintPoint currentPosition;
const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

void ngon(int n, GLintPoint center, double radius, double rotAngle) {
  if (n < 3) return; // bad number of sides

  double angle = rotAngle * M_PI / 180.0; // initial angle
  double angleInc = 2.0 * M_PI / n; // angle increment

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < n; i++) {
    glVertex2d(radius * cos(angle) + (double)center.x, radius * sin(angle) + (double)center.y);
    angle += angleInc;
  }
  glEnd();
}

void drawRoom() {
  glColor3f(0.7f, 0.7f, 0.7f); // set the drawing color

  Window backWall;
  backWall.right = WINDOW_WIDTH * 4 / 5;
  backWall.left = WINDOW_WIDTH * 1 / 5;
  backWall.top = WINDOW_HEIGHT * 4 / 5;
  backWall.bottom = WINDOW_HEIGHT * 1 / 5;
  backWall.width = backWall.right - backWall.left;
  backWall.height = backWall.top - backWall.bottom;

  int horizontalSpace = (WINDOW_WIDTH - backWall.width) / 2;
  int verticalSpace = (WINDOW_HEIGHT - backWall.height) / 2;

  for (int i = 0; i < 4; i++) {
    glBegin(GL_LINE_LOOP);
    glVertex2i(backWall.right + (i * horizontalSpace / 4), backWall.top + (i * verticalSpace / 4));
    glVertex2i(backWall.left - (i * horizontalSpace / 4),  backWall.top + (i * verticalSpace / 4));
    glVertex2i(backWall.left - (i * horizontalSpace / 4),  backWall.bottom - (i * verticalSpace / 4));
    glVertex2i(backWall.right + (i * horizontalSpace / 4), backWall.bottom - (i * verticalSpace / 4));
    glEnd();
  }

  glBegin(GL_LINES);
  glVertex2i(backWall.right, backWall.top);
  glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);

  glVertex2i(backWall.left, backWall.top);
  glVertex2i(0, WINDOW_HEIGHT);

  glVertex2i(backWall.left, backWall.bottom);
  glVertex2i(0, 0);

  glVertex2i(backWall.right, backWall.bottom);
  glVertex2i(WINDOW_WIDTH, 0);
  glEnd();

  for (int i = 1; i < 9; i++) {
    glBegin(GL_LINE_STRIP);
    glVertex2i(WINDOW_WIDTH * i / 9, 0);
    glVertex2i(backWall.width * i / 9 + backWall.left, backWall.bottom);
    glVertex2i(backWall.width * i / 9 + backWall.left, backWall.top);
    glVertex2i(WINDOW_WIDTH * i / 9, WINDOW_HEIGHT);
    glEnd();
  }

  for (int i = 1; i < 4; i++) {
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, WINDOW_HEIGHT * i / 4);
    glVertex2i(backWall.left, backWall.height * i / 4 + backWall.bottom);
    glVertex2i(backWall.right, backWall.height * i / 4 + backWall.bottom);
    glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT * i / 4);
    glEnd();
  }
}

void drawCursor() {
  glColor3f(0.2f, 0.2f, 0.8f); // set the drawing color
  glBegin(GL_POINTS);
  glVertex2i(currentPosition.x, currentPosition.y);
  glEnd();

  // draw a circle around the mouse
  ngon(30, currentPosition, 24.0, 0.0);
}

void myInit() {
  glClearColor(0.9, 0.9, 0.9, 0.0); // set light gray background color
  glPointSize(3.0); // a ‘dot’ is 3 by 3 pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double)WINDOW_WIDTH, 0.0, (double)WINDOW_HEIGHT);
}

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT); // clear the screen
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawRoom();
  drawCursor();

  glutSwapBuffers();
}

void myMovedMouse(int x, int y) {
  currentPosition.x = x;
  currentPosition.y = WINDOW_HEIGHT - y;

  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv); // initialize the Open-GL toolkit
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // set the display mode
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // set window size
  glutInitWindowPosition(43, 24); // centered for 1366 x 768 monitors
  glutCreateWindow("Music Motion Demo"); // open the screen window

  // register the callback functions
  glutDisplayFunc(myDisplay);
  // glutReshapeFunc(myReshape);
  // glutMouseFunc(myMouse);
  glutPassiveMotionFunc(myMovedMouse);
  // glutKeyboardFunc(myKeyboard);

  myInit(); // additional initializations as necessary
  glutMainLoop(); // go into a perpetual loop
  return 0;
}
