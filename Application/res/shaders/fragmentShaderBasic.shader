#version 460 core

precision highp float;

out vec4 color;

//uniform float uTime

void main() {
    color = vec4(0.2, 0.3, 0.8, 1.0);
    //color = vec4(abs(sin(u_time)), 0.0, 0.0, 1.0);;
}

