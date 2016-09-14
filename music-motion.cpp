// File: music-motion.cpp
// Author: Edward Ly
// Last Updated: 13 September 2016
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

// global variables
GLintPoint currentPosition;
const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

void drawDiamond(GLintPoint center, int radius) {
  glBegin(GL_LINE_LOOP);
  glVertex2i(center.x + radius, center.y);
  glVertex2i(center.x, center.y - radius);
  glVertex2i(center.x - radius, center.y);
  glVertex2i(center.x, center.y + radius);
  glEnd();
}

void myInit() {
  glClearColor(0.8, 0.8, 0.8, 0.0); // set light gray background color
  glColor3f(0.2f, 0.2f, 0.8f); // set the drawing color
  glPointSize(4.0); // a ‘dot’ is 4 by 4 pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double)WINDOW_WIDTH, 0.0, (double)WINDOW_HEIGHT);
}

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT); // clear the screen
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_POINTS);
  glVertex2i(currentPosition.x, currentPosition.y);
  glEnd();

  drawDiamond(currentPosition, 20);

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
