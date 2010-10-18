#include <fstream>
#include <sstream>
#include "MultiDimScatterPlot.h"

double data[600][25];

int main(int argc, char **argv)
{
	/* General initialization for GLUT and OpenGL
	Must be called first */
	glutInit(&argc, argv);

	/* we define these setup procedures */
	glut_setup();
	gl_setup();
	my_setup();
	int rows = 5, columns = 25;

	// Reads the specified number of rows and columns from the data file
	read_data(argv[1], rows, columns);
	output_data(rows, columns);

	/* go into the main event loop */
	glutMainLoop();

	return(0);
}

/* This function sets up the windowing related glut calls */
void glut_setup()
{
	/* specify display mode -- here we ask for a double buffer and RGB coloring */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	/* make a 400x300 window with the title of "GLUT Skeleton" placed at the top left corner */
	glutInitWindowSize(400,100);
	glutInitWindowPosition(0,0);
	glutCreateWindow("GLUT Skeleton");

	/*initialize callback functions */
	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape); 

	return;
}

/* This function sets up the initial states of OpenGL related enivornment */
void gl_setup()
{
	/* specifies a background color: black in this case */
	glClearColor(0,0,0,0);

	/* setup for simple 2d projection */
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, 0, 1.0); 

	return;
}

void my_setup() {
	return;
}

void my_reshape(int w, int h)
{
	/* define viewport -- x, y, (origin is at lower left corner) width, height */
	glViewport (0, (int)(h*0.25), w/2, h/2); 

	return;
}

void my_display() {
	/* clear the buffer */
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0,1,0);

	/* draw something */
	glBegin(GL_POLYGON);
	glVertex2f(-1.0,1.0);
	glVertex2f(1.0,1.0);
	glVertex2f(1.0,-1.0);
	glVertex2f(-1.0,-1.0);
	glEnd();

	glColor3f(0,0,1) ;
	glBegin(GL_POLYGON);
		glVertex2f(-0.1, 0.7);
		glVertex2f(0.9, 0.7);
		glColor3f(1,0,0);
		glVertex2f(0.9, -0.3);
		glVertex2f(-0.1, -0.3);
	glEnd();

	/* buffer is ready */
	glutSwapBuffers();
	
	return;
}

void output_data(int rows, int columns)
{
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
			std::cout << data[i][j] << "\t";
		std::cout << std::endl;
	}
}

void read_data(std::string input_file, int rows, int columns)
{
	std::ifstream file(input_file.c_str());
	std::string value;
	int row_count = 0, column_count = 0;

	// Get rid of the first line
	getline(file, value);
	while(file.good() && row_count++ < rows)
	{
		// Read line by line
		getline(file, value);
		std::istringstream line(value);
		column_count = 0;
		while(line.good() && column_count++ < columns)
		{
			// Parse each line using "," as a separator
			getline(line, value, ',');
			data[row_count - 1][column_count - 1] = atof(value.c_str());
		}
	}
}