#version 410

layout (location = 0) in vec3 vp;
layout (location = 1) in vec2 vt; // per-vertex texture co-ords
uniform vec3 cubeMap_texcoords;

out vec2 texture_coordinates;

void main () {
    texture_coordinates = vt;
    gl_Position = vec4 (vp, 1.0);
}