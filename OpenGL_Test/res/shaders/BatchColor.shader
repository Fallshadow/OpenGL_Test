#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 vertex_color;

void main()
{
    gl_Position = position;
    vertex_color = color;
};

#shader fragment
#version 330 core

in vec4 vertex_color;

out vec4 color;

void main()
{
    color = vertex_color;
};