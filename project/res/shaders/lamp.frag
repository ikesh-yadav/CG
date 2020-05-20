#version 330 core
out vec4 color;

uniform vec3 lightcolor;
uniform float lightbrightness;

void main()
{
    color = vec4(1.0f, 1.0f, 0.9f, 0.5f); // Set alle 4 vector values to 1.0f
}