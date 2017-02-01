# Music Motion

A Linux application developed as an Earlham College senior research project in collaboration with the Music and Computer Science Departments.

This repository is obsolete. You can view the updated source code at [https://github.com/edward-ly/kinect-konductor](https://github.com/edward-ly/kinect-konductor).

## Technical Overview

* [OpenGL](https://www.opengl.org/)
* [PortAudio](http://www.portaudio.com/)

## Setup

1. Clone this repository.
2. Install the PortAudio library and copy the compiled `libportaudio.a` file to the source directory.
3. Compile the program with the following command:

```shell
$ gcc -Wall -ggdb music-motion.cpp libportaudio.a -o music-motion -lm -lGL -lglut -lGLU -lpthread
```
