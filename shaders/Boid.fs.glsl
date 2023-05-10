#version 330

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vFragTexCoords;

out vec4 fFragColor;


void main() {
    fFragColor = vec4(normalize(vFragNormal), 1.0);
}
