#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_PVM;

void main() {

    gl_Position = u_PVM * position;
}
