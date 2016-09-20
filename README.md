# Music Motion

A PC game developed as an Earlham College senior research project in collaboration with the Music and Computer Science Departments. More info to come soon.

## Technical Overview

* [OpenGL](https://www.opengl.org/)
* [PortAudio](http://www.portaudio.com/)

## Setup

As of now, the program can simply be compiled by cloning the repository and entering the following command.

```shell
$ gcc -Wall -ggdb music-motion.cpp libportaudio.a -o music-motion -lm -lGL -lglut -lGLU -lpthread
```

Make sure you are in the same directory as the `music-motion.cpp` source file and that the OpenGL libraries are installed on your computer.
