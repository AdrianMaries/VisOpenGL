#include <fstream>
#include <sstream>
#include "MultiDimScatterPlot.h"

double data[600][25];
double max_data[25];
double min_data[25];

int rows, cols;

// Displacement of the camera as a result of user interaction
int dir_x;
int dir_y;
int dir_z;

// Rotation around the axes of the camera as a result of user interaction
int theta_x;
int theta_y;
int theta_z;

// Display shere or just wire frame of the sphere
int crt_render_mode;

// Approximation of the sphere (numbers of circles in the vertical, horizontal planes)
int crt_rs, crt_vs;

// Stores the color of the sphere
int color;

double sph_ray;

int main(int argc, char **argv)
{
	/* General initialization for GLUT and OpenGL
	Must be called first */
	glutInit(&argc, argv);

	/* we define these setup procedures */
	glut_setup();
	gl_setup();
	my_setup(argc, argv);

	/* go into the main event loop */
	glutMainLoop();

	return(0);
}

/* This function sets up the windowing related glut calls */
void glut_setup()
{
	/* specify display mode -- here we ask for a double buffer and RGB coloring */
	glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB);// |GLUT_DEPTH);

	/* make a 400x300 window with the title of "GLUT Skeleton" placed at the top left corner */
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("GLUT Skeleton");

	/*initialize callback functions */
	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	glutMouseFunc(my_mouse);
	glutKeyboardFunc(my_keyboard);

	return;
}

/* This function sets up the initial states of OpenGL related enivornment */
void gl_setup()
{
	/* specifies a background color: black in this case */
	glClearColor(0,0,0,0);

	/* NEW: now we have to enable depth handling (z-buffer) */
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND); //Enable alpha blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function

	/* NEW: setup for 3d projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// perspective view
	gluPerspective(30.0, 1.0, 1.0, 1000.0);

	return;
}

void my_setup(int argc, char** argv)
{
	theta_x = 0;
	theta_y = 90;
	theta_z = 0;
	dir_x = 0;
	dir_y = 0;
	dir_z = 0;
	crt_rs = 10;
	crt_vs = 5;
	crt_render_mode = GL_POLYGON;
	sph_ray = 1;
	color  = BLUE;

	// Reads the specified number of rows and columns from the data file
	read_data(argv[1], 507, 25);

	// Find min and max for each column
	get_data_stats();
	//output_data();

	return;
}

void my_reshape(int w, int h)
{
	/* define viewport -- x, y, (origin is at lower left corner) width, height */
	glViewport (0, 0, min(w,h), min(w,h));

	return;
}

void my_display() {
	/* clear the buffer */
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 200.0,  // x,y,z coord of the camera 
		5.0, 15.0, 0.0,  // x,y,z coord of the origin
		0.0, 1.0, 0.0); // the direction of up (default is y-axis)
	glTranslatef(dir_x, 0, 0);
	glTranslatef(0, dir_y, 0);
	glTranslatef(0, 0, dir_z);
	glRotatef(theta_x, 1, 0, 0);
	glRotatef(theta_y, 0, 1, 0);
	glRotatef(theta_z, 0, 0, 1);

	int col_x = 1, col_y = 2, col_z = 3;
	draw_axes(0, 0, 0, max_data[col_x], max_data[col_y], max_data[col_z]);
	draw_data(col_x, col_y, col_z);

	/* buffer is ready */
	glutSwapBuffers();
	
	return;
}

void my_mouse(int button, int state, int mousex, int mousey)
{
	// Clicking toggles wire frame / full display of spheres
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(crt_render_mode != GL_LINE_LOOP)
			// Display wire frame of the spheres
			crt_render_mode = GL_LINE_LOOP;
		else
			// Display full spheres
			crt_render_mode = GL_POLYGON;
		glutPostRedisplay();
	}
}

void draw_line(double x1, double y1, double z1, double x2, double y2, double z2, int color, double alpha)
{
	colors[color][3] = alpha;

	glBegin(GL_LINES);
		glColor4fv(colors[color]);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}

void draw_axes(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z)
{
	draw_line(min_x, 0, 0, max_x, 0, 0, YELLOW, 1.0);//x-axis
	draw_line(0, min_y, 0, 0, max_y, 0, RED, 1.0);//y-axis
	draw_line(0, 0, min_z, 0, 0, max_z, BLUE, 1.0);//z-axis
}

void make_sphere(double ray, int rs, int vs)
{
  /*hint: work out the math first*/
	double theta = M_PI / (vs-1);
	double phi = (2 * M_PI) / rs;
	int r, v;
	//large 3 and large 0 or 1 or 2 give rods
	//small 3 and small 0 or 1 or 2 give discs
	for (r = 0; r < (rs+1); r++)
	{
		// Again with the (r+1) dirty dirty hack
		for (v = 0; v < vs; v++)
		{
			vertices_sph[r][v][0] = 1 * ray * cos((v * theta) - M_PI_2) * sin(phi * r);
			vertices_sph[r][v][1] = 1 * ray * sin((v * theta) - M_PI_2);
			vertices_sph[r][v][2] = 1 * -ray * cos((v * theta) - M_PI_2) * cos(phi * r);
			vertices_sph[r][v][3] = 1;
		}
	}
}

void draw_param_quad(GLfloat vertices[][50][4], int line, int col, int ic)
{
	glBegin(crt_render_mode);
	glColor3fv(colors[ic]);
		glVertex4fv(vertices[line][col]);
		glVertex4fv(vertices[line+1][col]);
		glVertex4fv(vertices[line+1][col-1]);
		glVertex4fv(vertices[line][col-1]);
	glEnd();
}

void draw_sphere(int rs, int vs)
{
	int r, v;

	for (v = vs; v > 0; v--)
	{
		for (r = 0; r < rs; r++)
		{
			draw_param_quad(vertices_sph,  r , v, color);
		}
	}
}

void draw_glyph_sphere(double x, double y, double z, double scale_factor)
{
	make_sphere(sph_ray, crt_rs, crt_vs);
	scale(vertices_sph, scale_factor, scale_factor, scale_factor, crt_rs, crt_vs);
	translate(vertices_sph, x, y, z, crt_rs, crt_vs);
	draw_sphere(crt_rs, crt_vs);
}

void draw_data(int x_col, int y_col, int z_col)
{
	for(int i = 0; i < rows; i++)
		draw_glyph_sphere(data[i][x_col], data[i][y_col], data[i][z_col], 1);
}

void my_keyboard(unsigned char key, int x, int y)
/*
l/r//u/d//f/b moves the camera lefft / right // up / down // forward / backward
x/X//y/Y//z/Z rotates the camera around the x // y // z axis
-/= changes the numebr of vertical circles used to approximate the sphere
,/. changes the number of horizontal circles used to approximate the sphere
*/

{
	switch(key)
	{
	case 'r':
		dir_x -= 2;
		glutPostRedisplay(); 
		break;
	case 'l':
		dir_x += 2;
		glutPostRedisplay(); 
		break;
	case 'u':
		dir_y -= 2;
		glutPostRedisplay(); 
		break;
	case 'd':
		dir_y += 2;
		glutPostRedisplay(); 
		break;
	case 'b': 
		dir_z -= 2;
		glutPostRedisplay(); 
		break;
	case 'f':
		dir_z += 2;
		glutPostRedisplay(); 
		break;
	case 'x':
		theta_x = (theta_x+2) %360;
		glutPostRedisplay(); 
		break;
	case 'X': 
		theta_x = (theta_x-2) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		theta_y = (theta_y+2) %360;
		glutPostRedisplay(); 
		break;
	case 'Y':
		theta_y = (theta_y-2) % 360;
		glutPostRedisplay();
		break;
	case 'z':
		theta_z = (theta_z+2) %360;
		glutPostRedisplay(); 
		break;
	case 'Z': 
		theta_z = (theta_z-2) % 360;
		glutPostRedisplay();
		break;
	case '=':
		crt_rs++;
		if (crt_rs > 50)
			crt_rs = 50;
		glutPostRedisplay();
		break;
	case '-':
		crt_rs--;
		if (crt_rs < 2)
			crt_rs = 2;
		glutPostRedisplay();
		break;
	case'.':
		crt_vs++;
		if (crt_vs > 50)
			crt_vs = 50;
		glutPostRedisplay();
		break;
	case ',':
		crt_vs--;
		if (crt_vs < 2)
			crt_vs = 2;
		glutPostRedisplay();
		break;
	case 'q': 
	case 'Q':
		exit(0);
	default: break;
	}
	return;
}

void translate(GLfloat verts[][50][4], double x, double y, double z, int rs, int vs)
{
	int r, v;
	for (r = 0; r < rs+1; r++)
	{
		for (v = 0; v < vs; v++)
		{
			verts[r][v][0] += x;
			verts[r][v][1] += y;
			verts[r][v][2] += z;
		}
	}
}

void scale(GLfloat verts[][50][4], double x, double y, double z, int rs, int vs)
{
	int r, v;
	for (v = 0; v < vs; v++)
	{
		for (r = 0; r < rs+1; r++)
		{
			verts[r][v][0] *= x;
			verts[r][v][1] *= y;
			verts[r][v][2] *= z;
		}
	}
}

void output_data()
{
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
			std::cout << data[i][j] << "\t";
		std::cout << std::endl;
	}
}

void get_data_stats()
{
	for(int j = 0; j < cols; j++)
	{
		min_data[j] = max_data[j] = data[0][j];
		for(int i = 0; i < rows; i++)
		{
			if(data[i][j] < min_data[j])
				min_data[j] = data[i][j];
			if(data[i][j] > max_data[j])
				max_data[j] = data[i][j];
		}
	}
}

void read_data(std::string input_file, int read_rows, int read_cols)
{
	std::ifstream file(input_file.c_str());
	std::string value;
	int row_count = 0, col_count = 0;

	// Get rid of the first line
	getline(file, value);
	while(file.good() && row_count < read_rows)
	{
		// Read line by line
		getline(file, value);
		std::istringstream line(value);
		col_count = 0;
		while(line.good() && col_count < read_cols)
		{
			// Parse each line using "," as a separator
			getline(line, value, ',');
			data[row_count][col_count] = atof(value.c_str());
			col_count++;
		}
		row_count++;
	}
	rows = row_count;
	cols = col_count;
}