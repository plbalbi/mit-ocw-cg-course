#include <GLUT/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <vecmath.h>

// libs for generating random colors
#include <stdlib.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

using namespace std;

typedef struct face_s{
    int a,b,c,d,e,f,g,h,i;
    face_s(int a,int b,int c,int d,int e,int f,int g,int h,int i):
        a(a),
        b(b),
        c(c),
        d(d),
        e(e),
        f(f),
        g(g),
        h(h),
        i(i) {}
}face;

// Globals
GLfloat lightPosition[] = {1.0f, 1.0f, 5.0f, 1.0f};

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector< face > vecf;

// GLobal variable which represents current color
GLfloat currentColor[4] = {0.5, 0.5, 0.9, 1.0};


void randomizeCurrentColor() {
    currentColor[0] = float(rand())/RAND_MAX;
    currentColor[1] = float(rand())/RAND_MAX;
    currentColor[2] = float(rand())/RAND_MAX;
    currentColor[3] = float(rand())/RAND_MAX;
}



// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
        randomizeCurrentColor();
		cout << "Changing teapot colors..." << endl; 
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position
		cout << "Adjusting light position up..." << endl;
        lightPosition[1]+=.5;
		break;
    case GLUT_KEY_DOWN:
        // add code to change light position
		cout << "Adjusting light position down..." << endl;
        lightPosition[1]-=.5;
		break;
    case GLUT_KEY_LEFT:
        // add code to change light position
		cout << "Adjusting light position to left..." << endl;
        lightPosition[0]-=.5;
		break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
		cout << "Adjusting light position to right..." << endl;
        lightPosition[0]+=.5;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, currentColor);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	glutSolidTeapot(1.0);
    
    // Dump the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput() {
    char buffer[MAX_BUFFER_SIZE];
    Vector3f v;
    string s;
    while (cin.eof()){
        cin.getline(buffer, MAX_BUFFER_SIZE);
        stringstream ss(buffer);
        ss >> s;
        if (s == "v") {
            // Its a vector
            ss >> v[0] >> v[1] >> v[2];
            vecv.push_back(Vector3f(v[0],v[1],v[2]));
        }else if (s == "vn") {
            // Its a normal
            ss >> v[0] >> v[1] >> v[2];
            vecn.push_back(Vector3f(v[0],v[1],v[2]));
        }else if (s == "f") {
            // Its a face
            // bring first face vector
            ss >> s;
            // its sth like NUM/NUM/NUM

        } // else: do nothing

    }
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{

    // generate time seed for random colors
    srand(time(0));

    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
