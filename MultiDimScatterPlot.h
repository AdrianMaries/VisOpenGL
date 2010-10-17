/**********
notes:
VS users: include opengl/glut.h below instead of GL/glut.h
OSX users: include glut/glut.h below instead of GL/glut.h
**********/
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void glut_setup();
void gl_setup();
void my_setup();
void my_display();
void my_reshape(int w, int h);
void read_data(std::string input_file, int rows, int columns);
void output_data(int rows, int columns);