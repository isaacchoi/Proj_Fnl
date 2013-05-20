
// ------------------------
// GLUT harness v. 1.0
// ------------------------

#include "Angel.h"			//added


#if defined(__APPLE__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

const int NumTimesToSubdivide = 5;

const int NumTetrahedrons = 1024;            // 4^5 tetrahedrons
const int NumTrianglesForTetra = 4*NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3*NumTrianglesForTetra;      // 3 vertices / triangle


vec2 points[NumVertices];						//gasket for Sirerpinski
vec3 gasketTetraPoints[NumVertices];		//gaskit for tetra for extra credit 3



//-------------------------//
//Data for colors		   //
//color1  -> red gasket    //
//color2  -> green gasket  //
//color3  -> tetra gasket  //
//-------------------------//
vec3 colors1[NumVertices];
vec3 colors2[NumVertices];
vec3 colors3[NumVertices];



// Create a vertex array and buffer
GLuint vao[3];
GLuint buffer[3];


//make index and function key for showing gasket
int showPoint = 0;		//keyboad keyfind
int Index = 0;			//Index for Gasket
int tetraIndex = 0;		//Index for Tetra



//Rotating OBject

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
const GLfloat DegreeToRadians =  3.14 / 180;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };



void triangle( const vec2& a, const vec2& b, const vec2& c );
void divide_triangle( const vec2& a, const vec2& b, const vec2& c, int count );
void init( void );
void callbackDisplay( void );
void callbackKeyboard( unsigned char key, int x, int y );
void initGlut(int& argc, char** argv);
void callbackReshape(int width, int height);
void callbackMouse(int button, int state, int x, int y);
void callbackMotion(int x, int y);
void callbackPassiveMotion(int x, int y);
void callbackIdle();
void callbackTimer(int);
void initCallbacks();

//////show Tetra fractal for another Sierpinski Gasket and used for Extra Credit number 3/////
void triangleForTetra( const vec3& a, const vec3& b, const vec3& c, const int color );
void tetra( const vec3& a, const vec3& b, const vec3& c, const vec3& d );
void divide_tetra( const vec3& a, const vec3& b, const vec3& c, const vec3& d, int count );
//////////////////////////////////////////////////////////////////////////////////////////////


//THree Vertices 2d vectors for Sierpinski Gasket 
vec2 vertices[3] = { vec2( -0.5, -0.5 ), vec2( 0.5, -0.5 ), vec2( 0.0, 0.5 )};


void
triangleForTetra( const vec3& a, const vec3& b, const vec3& c, const int color )
{
    static vec3  tetra_base_colors[] = {vec3( 0.2, 0.2, 0.0 ),	vec3( 0.0, 0.9, 0.9 ), vec3( 0.2, 0.0, 1.0 ),vec3( 0.0, 0.0, 0.0 )};
    
	gasketTetraPoints[tetraIndex] = a;  colors3[tetraIndex] = tetra_base_colors[color];  tetraIndex++;
    gasketTetraPoints[tetraIndex] = b;  colors3[tetraIndex] = tetra_base_colors[color];  tetraIndex++;
    gasketTetraPoints[tetraIndex] = c;  colors3[tetraIndex] = tetra_base_colors[color];  tetraIndex++;
}

//----------------------------------------------------------------------------

void
tetra( const vec3& a, const vec3& b, const vec3& c, const vec3& d )
{
    triangleForTetra( a, b, c, 0 );
    triangleForTetra( a, c, d, 1 );
    triangleForTetra( a, d, b, 2 );
    triangleForTetra( b, d, c, 3 );
}

//----------------------------------------------------------------------------

void
divide_tetra( const vec3& a, const vec3& b,
	      const vec3& c, const vec3& d, int count )
{
    if ( count > 0 ) {
        vec3 v0 = ( a + b ) / 2.0;
        vec3 v1 = ( a + c ) / 2.0;
        vec3 v2 = ( a + d ) / 2.0;
	vec3 v3 = ( b + c ) / 2.0;
	vec3 v4 = ( c + d ) / 2.0;
	vec3 v5 = ( b + d ) / 2.0;
        divide_tetra( a, v0, v1, v2, count - 1 );
        divide_tetra( v0, b, v3, v5, count - 1 );
        divide_tetra( v1, v3, c, v4, count - 1 );
	divide_tetra( v2, v4, v5, d, count - 1 );
    }
    else {
        tetra( a, b, c, d );    // draw tetrahedron at end of recursion
    }
}


void
triangle( const vec2& a, const vec2& b, const vec2& c)
{
  	   static vec3  base_colors[] = {
	vec3( 1.0, 0.0, 0.0 ),     //red
	vec3( 0.0, 1.0, 0.0 ),	   //green
    };
		
	points[Index] = a;  
	colors1[Index] = base_colors[0];	//red
	colors2[Index] = base_colors[1];	//green
	Index++;

	points[Index] = b;  
	colors1[Index] = base_colors[0];
	colors2[Index] = base_colors[1];
	Index++;

	points[Index] = c;  
	colors1[Index] = base_colors[0];
	colors2[Index] = base_colors[1];
	Index++;

}

void
divide_triangle( const vec2& a, const vec2& b, const vec2& c, int count )
{
    if ( count > 0 ) {
        vec2 v0 = ( a + b ) / 2.0;
        vec2 v1 = ( a + c ) / 2.0;
        vec2 v2 = ( b + c ) / 2.0;
        divide_triangle( a, v0, v1, count - 1 );
        divide_triangle( c, v1, v2, count - 1 );
        divide_triangle( b, v2, v0, count - 1 );
    }
    else {
        triangle( a, b, c );    // draw triangle at end of recursion
    }
}

void
init( void )
{


    // Subdivide the original triangle
    divide_triangle( vertices[0], vertices[1], vertices[2],
                     NumTimesToSubdivide );
	// Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader22.glsl", "fshader22.glsl" );
    glUseProgram( program );


    // Create a vertex array object
    glGenVertexArrays( 3, vao );
    glBindVertexArray( vao[0] );

	// Create and initialize a buffer object
   
    glGenBuffers( 3, buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );

    // First, we create an empty buffer of the size we need by passing
    //   a NULL pointer for the data values
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors1), NULL, GL_STATIC_DRAW );

	// Next, we load the real data in parts.  We need to specify the
    //   correct byte offset for placing the color data after the point
    //   data in the buffer.  Conveniently, the byte offset we need is
    //   the same as the size (in bytes) of the points array, which is
    //   returned from "sizeof(points)".
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors1), colors1 );




	
	// Likewise, initialize the vertex color attribute.  Once again, we
    //    need to specify the starting offset (in bytes) for the color
    //    data.  Just like loading the array, we use "sizeof(points)"
    //    to determine the correct value.
	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );


    // Initialize the vertex position attribute from the vertex shader    
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );



	//Setting up for vertex second array of vao
    glBindVertexArray( vao[1] );
	glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors2), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors2), colors2 );
	
	//setup for second color
	vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );


    // Initialize the vertex position attribute from the vertex shader    
    loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	
	//Setting up for vertex third array of vao
	//this is for extra credit number 3
	//it wiil work out with keyboard '3'
	vec3 tetraVertices[4] = {vec3( 0.0, 0.0, -1.0 ),vec3( 0.0, 0.942809, 0.333333 ),vec3( -0.816497, -0.471405, 0.333333 ),vec3( 0.816497, -0.471405, 0.333333 ) };	

	divide_tetra( tetraVertices[0], tetraVertices[1], tetraVertices[2], tetraVertices[3], NumTimesToSubdivide );
    glBindVertexArray( vao[2] );
	glBindBuffer( GL_ARRAY_BUFFER, buffer[2] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(gasketTetraPoints) + sizeof(colors3), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(gasketTetraPoints), gasketTetraPoints );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(gasketTetraPoints), sizeof(colors3), colors3 );

	//setup color
	vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(gasketTetraPoints)) );

	
    // Initialize the vertex position attribute from the vertex shader    
    loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	glEnable( GL_DEPTH_TEST );	//this will have depth in tetra shape


	//Set the initial plot for RED Sierspien gasket
	glBindVertexArray(vao[0]);

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background 

  
}


// Called when the window needs to be redrawn.
// TODO: Don't forget to add glutPostRedisplay().
void
callbackDisplay( void )
{
	//std::cout <<**DEBUG__"this is Theta" <<Theta[0]<<endl;
	if (Theta[0] > 360 )
		Theta[0] = Theta[0]-360;	//if Theta is over than 360 degree then change value to 0 to 360 degree again
	//std::cout << "**DEBUG__this is Theta" <<Theta[0]<<endl;
	GLfloat angle = Theta[0] * DegreeToRadians;

	glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT  );
	angle = angle/NumVertices;

	for (int i =0 ; i < NumVertices ; i++)
	{
		float x = cos(angle)*gasketTetraPoints[i].x - sin(angle)*gasketTetraPoints[i].y;
		float y = sin(angle)*gasketTetraPoints[i].x - cos(angle)*gasketTetraPoints[i].y;
		gasketTetraPoints[i].x = x;
		gasketTetraPoints[i].y = y;

	}
		//std::cout << "THIS is DEBUG MODE for points 1\n";
		//std::cout << "points[0] is" <<points[0]<<endl;
		//std::cout << "points[1] is" <<points[1]<<endl;
		//std::cout << "this is Theta" <<angle<<endl;
		//std::cout << "THIS is DEBUG MODE 2\n";
		//std::cout << "this is Theta" <<angle<<endl;
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(gasketTetraPoints), gasketTetraPoints );
	
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	showPoint =0;


	glFlush();
}



//----------------------------------------------------------------------------


// Called when a key is pressed. x, y is the current mouse position.
void
callbackKeyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'q':
	case 'Q':
		exit( EXIT_SUCCESS );
		break;

	case '1':
		showPoint =1; 
		glBindVertexArray(vao[0]);
		break;

	case '2':
		showPoint =1;
		glBindVertexArray(vao[1]);
		break;

	case '3':
		showPoint =1;
		glBindVertexArray(vao[2]);
		break;

	case 'R':
	case 'r':
	Theta[0] +=60;
	break;

	case '5':
	Theta[0]=0.0;
	break;

    }

	//std::cout << "THIS is DEBUG MODE\n";
	//std::cout << "this is Theta" <<Theta<<endl;
	
	glutPostRedisplay();
}



void initGlut(int& argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(640, 480);
	glutCreateWindow("GLUT Harness");
}



// Called when the window is resized.
void callbackReshape(int width, int height)
{

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
	glutDisplayFunc(callbackDisplay);
	glutReshapeFunc(callbackReshape);
	glutKeyboardFunc(callbackKeyboard);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
	glutPassiveMotionFunc(callbackPassiveMotion);
	glutIdleFunc(callbackIdle);
	glutTimerFunc(1000/30, callbackTimer, 0);
}



int main(int argc, char** argv)
{

	glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Sierpinski Gasket" );

    glewInit();

    init();
	
	glutDisplayFunc( callbackDisplay );
	glutKeyboardFunc( callbackKeyboard );

	

    glutMainLoop();
    return 0;
	
}



/*   //TEST function for startup
#include <stdio.h>
#include <GL/glew.h>

#include <GL/freeglut.h>

int main (int argcp, char **argv){

	glutInit (&argcp, argv);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(0,0);

	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE );

	glutCreateWindow("Hello world FUXXXXXXXXXXXXXX");
	GLenum err = glewInit();
	if ( GLEW_OK != err){}
	fprintf(stdout, "STATUS: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	glutMainLoop();

	return 0;

}
*/