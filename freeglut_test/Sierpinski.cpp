// Two-Dimensional Sierpinski Gasket
// Generated using randomly selected vertices and bisection
#include "Angel.h"
const int NumPoints = 5000;
const int NumLines = 5000;
int showGasket = 1;
float Color = 0;
float Theta = 0;

GLuint color; // the location of the "color" fragment variable
GLuint theta; // the location of the "theta" shader uniform variable


void init()
{
	vec2 points[NumPoints];
	// Specify the vertices for a triangle
	vec2 vertices[3] = {
	vec2( -0.5, -0.5 ), vec2( 0.0, 0.5 ), vec2( 0.5, -0.5 )
	};
	// Select an arbitrary initial point inside of the triangle
	points[0] = vec2( 0.25, 0.50 );
	// compute and store N-1 new points
	for ( int i = 1; i < NumPoints; ++i ) {
		int j = rand() % 3; // pick a vertex at random
		// Compute the point halfway between the selected vertex
		// and the previous point
		points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
	}

	// Added fractal tree
	vec2 lines[NumLines];
	lines[0] = vec2(0,-1);
	lines[1] = vec2(0,-0.5);
	// lines[2] = vec2(0,-0.4);
	// lines[3] = vec2(0.25, -0.25);
	float branchHeight = 0.4;
	int linesGenerated = 1;
	for(int i=1; i < NumLines/4; i+=2) {
		double j = rand() % 45;
		j += 15.0;
		float angle = j * 0.0174532925;
		float xdiff = branchHeight * tan(angle);
		lines[linesGenerated+1] = lines[i];
		lines[linesGenerated+2] = vec2(lines[i].x - xdiff, lines[i].y + branchHeight);
		lines[linesGenerated+3] = lines[i];
		lines[linesGenerated+4] = vec2(lines[i].x + xdiff, lines[i].y + branchHeight);
		branchHeight /= 1.2;
		linesGenerated += 4;
	}

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
	glUseProgram( program );

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Create and initialize a buffer object
	GLuint buffer;
	if(showGasket == 1)
	{
		glGenBuffers( 1, &buffer );
		glBindBuffer( GL_ARRAY_BUFFER, buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
	}
	else
	{
		glGenBuffers( 1, &buffer );
		glBindBuffer( GL_ARRAY_BUFFER, buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW );
	}

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc );
	glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
	BUFFER_OFFSET(0) );

	theta = glGetUniformLocation(program, "theta");
	color = glGetUniformLocation(program, "color");
	glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}


void display()
{
	glClear( GL_COLOR_BUFFER_BIT ); // clear the window
	
	glUniform1f(theta, Theta);
	glUniform1f(color, Color);
	if(showGasket == 1)
		glDrawArrays( GL_POINTS, 0, NumPoints ); // draw the points
	else
		glDrawArrays( GL_LINES, 0, NumLines );
	glFlush();
}

// Called when the window needs to be redrawn.
// TODO: Don't forget to add glutPostRedisplay().
void callbackDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: render code goes here
	/*if(showGasket == 1)
		glDrawArrays(GL_TRIANGLES, 0, NumTriangles);
	else
		glDrawArrays(GL_LINES, 0, NumLines);*/

	glutSwapBuffers();
}

// Called when the window is resized.
void callbackReshape(int width, int height)
{

}

// Called when a key is pressed. x, y is the current mouse position.
void callbackKeyboard(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q':
		case 'Q':
		case 27:
			exit(0);
			break;
		case '1':
			showGasket = 1;
			Color = 0;
			init();
			break;
		case '2':
			showGasket = 1;
			Color = 1;
			init();
			break;
		case '3':
			showGasket = 0;
			init();
			break;
		case 'r':
		case 'R':
			Theta += 30;
			break;
	}
}

// Called when a mouse button is pressed or released
void callbackMouse(int button, int state, int x, int y)
{

}

// Called when the mouse is moved with a button pressed
void callbackMotion(int x, int y)
{

}

// Called when the mouse is moved with no buttons pressed
void callbackPassiveMotion(int x, int y)
{

}

// Called when the system is idle. Can be called many times per frame.
void callbackIdle()
{

}

// Called when the timer expires
void callbackTimer(int)
{
	glutTimerFunc(1000/30, callbackTimer, 0);
	glutPostRedisplay();
}

void initCallbacks()
{
	// glutDisplayFunc(callbackDisplay);
	glutReshapeFunc(callbackReshape);
	glutKeyboardFunc(callbackKeyboard);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
	glutPassiveMotionFunc(callbackPassiveMotion);
	glutIdleFunc(callbackIdle);
	glutTimerFunc(1000/30, callbackTimer, 0);
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA );
	glutInitWindowSize( 512, 512 );
	// If you are using freeglut, the next two lines will check if
	// the code is truly 3.2. Otherwise, comment them out
	// glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "Sierpinski Gasket" );
	glewInit();
	init();
	initCallbacks();
	glutDisplayFunc( display );
	glutMainLoop();
	return 0;
}