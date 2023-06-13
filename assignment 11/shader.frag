#version 430
in vec4 fColor;
out vec4 FragColor;
layout(location=4) uniform int mode;
void main()
{
	FragColor = fColor;
}