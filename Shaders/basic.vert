#version 120

attribute vec3 vertex;
attribute vec4 color;

uniform mat4 MVP;

varying vec4 v_color;

void main() {
    v_color = color;
    gl_Position = MVP * vec4(vertex, 1.0);
}