#include <stdio.h>
#include <GL/glew.h>					//include the glew
#include <GL/glut.h>					//include the glut
#include "LoadShaders.h"
GLuint Buffers[1], VertexArrays[1];		//make a buffer
const GLsizei NumVertices = 6;			//number of vertices
GLfloat vertices[NumVertices][2] = {	
{-0.90f, -0.90f}, {0.85f, -0.90f},
{-0.90f, 0.85f}, {0.90f, -0.85f},
{0.90f, 0.90f}, {-0.85f, 0.90f} };			//set the location of vertices

void init();
void display();
void main(int argc, char** argv)
{
	glutInit(&argc, argv);						
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);			//set window size
	glutCreateWindow(argv[0]);				//initialize the glut
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}										//initiazlize the glew to load openGL extensions
	init();									//set the initial
	glutDisplayFunc(display);				//Register a ¡°display¡± callback function
	glutMainLoop();							//enter the GLUT event processing loop
}

void init()
{
	glGenVertexArrays(1, VertexArrays);				//generate vertex the array
	glBindVertexArray(VertexArrays[0]);				//and bind to use VAO
	glGenBuffers(1, Buffers);						//generate buffers
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);		//and bind it with GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//set the vertices inside buffer
	ShaderInfo shaders[] = {
	{GL_VERTEX_SHADER, "triangles.vert"},
	{GL_FRAGMENT_SHADER, "triangles.frag"},
	{GL_NONE, NULL}
	};															//set the vertex shader and fragment shader
	GLuint program = LoadShaders(shaders);						//load the shader to program
	glUseProgram(program);										//and use it
	GLint location = glGetAttribLocation(program, "vPosition");		//Finds the location of a specified vertex attribute
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);	//Specifies how to read the buffer data through the attribute
	glEnableVertexAttribArray(location);							//Enable the attribute
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);									//Clear the buffers for color writing
	glBindVertexArray(VertexArrays[0]);								//Specifies the current active VAO
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);						//Issues a drawing command
	glFlush();														//Forces the execution of OpenGL commands in finite time
}