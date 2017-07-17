// File: music-motion.cpp
// Author: Edward Ly
// Last Updated: 26 September 2016
//
// A PC application developed with OpenGL where you move a controller (e.g. mouse) in time with the music.
// The game may support additional peripherals for the purposes of music making or conducting.

// OpenGL Libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

// PortAudio Library
#include "portaudio.h"

// C/C++ Libraries
// #include <fstream>
// #include <iostream>
#include <cmath>

// global variables for PortAudio
#define NUM_SECONDS 5
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64
#define TABLE_SIZE 200

// structs for PortAudio
struct paTestData {
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
};

// Functions for Port Audio

// test callback function from paex_sine.c
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->sine[data->left_phase];  /* left */
        *out++ = data->sine[data->right_phase];  /* right */
        data->left_phase += 1;
        if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
        data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
        if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
    }

    return paContinue;
}

// called when playback is done
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: %s\n", data->message );
}

// report error and exit program
PaError error(PaError err) {
  Pa_Terminate();
  fprintf(stderr, "An error occured while using the portaudio stream.\n");
  fprintf(stderr, "Error number: %d\n", err);
  fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  return err;
}

// structs for OpenGL
struct GLintPoint {
  int x, y;
};

struct Window {
  int left, right, bottom, top;
  int width, height;
};

// global variables for OpenGL
GLintPoint currentPosition;
const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

// Functions using OpenGL

// draw an n-gon
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

// draw a test room
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

// draw a custom mouse cursor
void drawCursor() {
  glColor3f(0.2f, 0.2f, 0.8f); // set the drawing color
  glBegin(GL_POINTS);
  glVertex2i(currentPosition.x, currentPosition.y);
  glEnd();

  // draw a circle around the mouse
  ngon(30, currentPosition, 24.0, 0.0);
}

// redraw window
void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT); // clear the screen
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawRoom();
  drawCursor();

  glutSwapBuffers();
}

// get mouse position and update window
void myMovedMouse(int x, int y) {
  currentPosition.x = x;
  currentPosition.y = WINDOW_HEIGHT - y;
  glutPostRedisplay();
}

// some initialization settings
void myInit() {
  glClearColor(0.9, 0.9, 0.9, 0.0); // set light gray background color
  glPointSize(3.0); // a ‘dot’ is 3 by 3 pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double)WINDOW_WIDTH, 0.0, (double)WINDOW_HEIGHT);
}

// main function
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

  // from sample program paex_sine.c

  // initialize PortAudio
  PaStreamParameters outputParameters;
  PaStream *stream;
  PaError err;
  paTestData data;
  int i;

    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    // initialise sinusoidal wavetable
    for( i=0; i<TABLE_SIZE; i++ )
    {
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.left_phase = data.right_phase = 0;

  err = Pa_Initialize();
  if (err != paNoError) return error(err);

  outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device
  if (outputParameters.device == paNoDevice) {
    fprintf(stderr,"Error: No default output device.\n");
    error(err);
  }
  outputParameters.channelCount = 2; // stereo output
  outputParameters.sampleFormat = paFloat32; // 32 bit floating point output
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(
              &stream,
              NULL, // no input
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff, // we won't output out of range samples so don't bother clipping them
              patestCallback,
              &data );
  if( err != paNoError ) return error(err);

  sprintf( data.message, "No Message" );
  err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
  if( err != paNoError ) return error(err);

  err = Pa_StartStream( stream );
  if( err != paNoError ) return error(err);

  printf("Play for %d seconds.\n", NUM_SECONDS );
  Pa_Sleep( NUM_SECONDS * 1000 );

  err = Pa_StopStream( stream );
  if( err != paNoError ) return error(err);

  err = Pa_CloseStream( stream );
  if( err != paNoError ) return error(err);

  myInit(); // additional OpenGL initializations as necessary
  glutMainLoop(); // go into a perpetual loop

  Pa_Terminate();
  return err;
}
