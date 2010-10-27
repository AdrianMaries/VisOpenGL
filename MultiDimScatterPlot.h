/**********
notes:
VS users: include opengl/glut.h below instead of GL/glut.h
OSX users: include glut/glut.h below instead of GL/glut.h
**********/
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef min
#define min(a,b) ((a) < (b)? a:b)
#endif

#define BLACK   0
#define RED     1
#define YELLOW  2
#define MAGENTA 3
#define GREEN   4
#define CYAN    5
#define BLUE    6
#define GREY    7
#define WHITE   8

GLfloat colors [][4] = {
  {0.0, 0.0, 0.0, 1.0},  /* black   */
  {1.0, 0.0, 0.0, 1.0},  /* red     */
  {1.0, 1.0, 0.0, 1.0},  /* yellow  */
  {1.0, 0.0, 1.0, 1.0},  /* magenta */
  {0.0, 1.0, 0.0, 1.0},  /* green   */
  {0.0, 1.0, 1.0, 1.0},  /* cyan    */
  {0.0, 0.0, 1.0, 1.0},  /* blue    */
  {0.5, 0.5, 0.5, 1.0},  /* 50%grey */
  {1.0, 1.0, 1.0, 1.0}   /* white   */
};

GLfloat vertices_sph[50][50][4];

void glut_setup();
void gl_setup();
void my_setup(int argc, char** argv);
void my_display();
void my_reshape(int w, int h);
void my_mouse(int button, int state, int mousex, int mousey);
void my_keyboard(unsigned char key, int x, int y);
void read_data(std::string input_file, int rows, int columns);
void get_data_stats();
void output_data();

void make_sphere(double ray, int rs, int vs);
void draw_axes(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);
void draw_line(double x1, double y1, double z1, double x2, double y2, double z2, int color, double alpha);
void draw_param_quad(GLfloat vertices[][50][4], int line, int col, int ic);
void draw_sphere(int rs, int vs);
void draw_glyph_sphere(double x, double y, double z, double scale_factor);
void draw_data(int x_col, int y_col, int z_col);

void translate(GLfloat verts[][50][4], double x, double y, double z, int rs, int vs);
void scale(GLfloat verts[][50][4], double x, double y, double z, int rs, int vs);