#version 430 
in vec4 vPosition;
in vec4 vColor;
out vec4 fColor;
void main() // Main function
{
	gl_Position = vPosition;
	fColor = vColor;
}
