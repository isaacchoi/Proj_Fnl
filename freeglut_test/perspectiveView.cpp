//
// Perspective view of a color cube using LookAt() and Perspective()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//

#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36*8; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.5, 0.5, 0.5, 1.0 ),  // gray
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

// Viewing transformation parameters

GLfloat radius = 10.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

GLfloat scale = 1.0;
int scaleUp = 0;

const GLfloat  dr = DegreesToRadians;

GLuint  model_view;  // model-view matrix uniform shader variable location

// Projection transformation parameters

GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 100.0;

GLfloat yaxis = 0.0;
GLfloat xaxis = 0.0;
GLfloat azimuth = 0.0;


GLuint  projection; // projection matrix uniform shader variable location

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

int colorIndex = 0;

void
quad( int a, int b, int c, int d , point4 cen, float len, int col)
{
    colors[Index] = vertex_colors[col]; points[Index] = vertices[a]*len + cen; Index++;
    colors[Index] = vertex_colors[col]; points[Index] = vertices[b]*len + cen; Index++;
    colors[Index] = vertex_colors[col]; points[Index] = vertices[c]*len + cen; Index++;
    colors[Index] = vertex_colors[col]; points[Index] = vertices[a]*len + cen; Index++;
    colors[Index] = vertex_colors[col]; points[Index] = vertices[c]*len + cen; Index++;
    colors[Index] = vertex_colors[col]; points[Index] = vertices[d]*len + cen; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube(point4 cen, float len, int col)
{
    quad( 1, 0, 3, 2, cen, len, col);
    quad( 2, 3, 7, 6, cen, len, col);
    quad( 3, 0, 4, 7, cen, len, col);
    quad( 6, 5, 1, 2, cen, len, col);
    quad( 4, 5, 6, 7, cen, len, col);
    quad( 5, 4, 0, 1, cen, len, col);
}

//----------------------------------------------------------------------------

/* This function is not used
void redraw()
{
	Index = 0;
    colorcube(point4(10.0, 10.0, 10.0, 0.0), 5.0*scale, (colorIndex) % 8);
	colorcube(point4(10.0, 10.0, -10.0, 0.0), 5.0*scale, (colorIndex+1) % 8);
	colorcube(point4(10.0, -10.0, -10.0, 0.0), 5.0*scale, (colorIndex+2) % 8);
	colorcube(point4(10.0, -10.0, 10.0, 0.0), 5.0*scale, (colorIndex+3) % 8);
	colorcube(point4(-10.0, 10.0, 10.0, 0.0), 5.0*scale, (colorIndex) % 8);
	colorcube(point4(-10.0, 10.0, -10.0, 0.0), 5.0*scale, (colorIndex+1) % 8);
	colorcube(point4(-10.0, -10.0, -10.0, 0.0), 5.0*scale, (colorIndex+2) % 8);
	colorcube(point4(-10.0, -10.0, 10.0, 0.0), 5.0*scale, (colorIndex+3) % 8);
	
	//colorcube(point4(0.0, 0.0, 0.0, 0.0), 5.0, colorIndex+4 % 8);

    // Load shaders and use the resulting shader program
} */


// OpenGL initialization
void
init()
{	
	Index = 0;
	colorcube(point4(10.0, 10.0, 10.0, 0.0), 5.0, (colorIndex) % 8);
	colorcube(point4(10.0, 10.0, -10.0, 0.0), 5.0, (colorIndex+1) % 8);
	colorcube(point4(10.0, -10.0, -10.0, 0.0), 5.0, (colorIndex+2) % 8);
	colorcube(point4(10.0, -10.0, 10.0, 0.0), 5.0, (colorIndex+3) % 8);
	colorcube(point4(-10.0, 10.0, 10.0, 0.0), 5.0, (colorIndex+4) % 8);
	colorcube(point4(-10.0, 10.0, -10.0, 0.0), 5.0, (colorIndex+5) % 8);
	colorcube(point4(-10.0, -10.0, -10.0, 0.0), 5.0, (colorIndex+6) % 8);
	colorcube(point4(-10.0, -10.0, 10.0, 0.0), 5.0, (colorIndex+7) % 8);
	//colorcube(point4(0.0, 0.0, 0.0, 0.0), 5.0, colorIndex+4 % 8);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader42.glsl", "fshader42.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

    model_view = glGetUniformLocation( program, "model_view" );
    projection = glGetUniformLocation( program, "projection" );
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    point4  eye( radius*sin(theta)*cos(phi) + xaxis,
		 radius*sin(theta)*sin(phi) + yaxis,
		 radius*cos(theta),
		 1.0 );
    point4  at( azimuth, 0.0, 0.0, 1.0 );
    vec4    up( 0.0, 1.0, 0.0, 0.0 );

    mat4  mv = LookAt( eye, at, up );
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

    mat4  p = Perspective( fovy, aspect, zNear, zFar );
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );

    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;

	// case 'z': zNear  *= 1.1; zFar *= 1.1; break;
	// case 'Z': zNear *= 0.9; zFar *= 0.9; break;
	case 'm': radius += 0.25; break;
	case 'i': radius -= 0.25; break;
	// case 'o': theta += dr; break;
	// case 'O': theta -= dr; break;

	// case 'p': phi += dr; break;
	// case 'P': phi -= dr; break;

	// BUG: Only works twice, then exits without error
	case 'c': colorIndex++; init(); break;
	
	case 'k': xaxis += 0.25; break;
	case 'j': xaxis -= 0.25; break;

	case 'w': fovy += 5.0; break;
	case 'n': fovy -= 5.0; break;


	case ' ':  // reset values to their defaults
	    zNear = 0.5;
	    zFar = 100.0;

	    radius = 10.0;
	    theta  = 0.0;
	    phi    = 0.0;
		xaxis = 0.0;
		azimuth = 0.0;
		yaxis = 0.0;
		fovy = 45.0;
	    break;
    }

    glutPostRedisplay();
}

void
special( int key, int x, int y )
{
	switch(key) {
	case GLUT_KEY_UP:
		yaxis += 0.25; break;
	case GLUT_KEY_DOWN:
		yaxis -= 0.25; break;
	case GLUT_KEY_LEFT:
		azimuth -= 0.25; break;
	case GLUT_KEY_RIGHT:
		azimuth += 0.25; break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    aspect = GLfloat(width)/height;
	glutPostRedisplay();
}


void callbackIdle()
{
	if(scale >= 1.1)
		scaleUp = 1;
	else if(scale <= 0.9)
		scaleUp = 0;
	// Cycle between a scale of 0.9 and 1.1 by going up and down in 0.01 increments
	if(scaleUp == 0)
		scale += 0.01;
	else
		scale -= 0.01;

	glutPostRedisplay();
}


//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    // glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cubes" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
	glutSpecialFunc( special );
	glutIdleFunc(callbackIdle);
    glutMainLoop();
    return 0;
}