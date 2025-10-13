#version 330 core
out vec4 FragColor;

uniform vec3 color;  // color del marcador (lo defines desde C++)

void main()
{
    FragColor = vec4(color, 1.0);
}
