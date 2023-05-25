#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;
in vec3 vColor;

uniform vec3 uka;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

const int LIGHTS_NB = 2;
uniform vec3 uLightPos_vs[LIGHTS_NB];
uniform float uLightIntensity[LIGHTS_NB]; 
uniform vec3 uLightColor[LIGHTS_NB]; 

out vec4 fFragColor;

vec3 blinnPhong(int IndexLight){

    vec3 N = normalize(vNormal_vs);
    vec3 W = normalize(uLightPos_vs[IndexLight] - vPosition_vs);
    float d = distance(vPosition_vs, uLightPos_vs[IndexLight]);
    float attenuation = 1.0 / (uLightIntensity[IndexLight] + d * d);
    vec3 L = vec3(attenuation);
    float dotW = max(dot(W, N), 0.0);
    vec3 W0 = normalize(-vPosition_vs);   
    vec3 halfVector = normalize((W0 + W) / 2.0);
    float dotH = max(dot(halfVector, N), 0.0);
    float specular= pow(dotH, uShininess);

    return uka + L * (uLightColor[IndexLight] * uKd * dotW + uKs * specular);
}
void main() {
    vec3 lightTotal = vec3(0.0);
    for(int i = 0; i < LIGHTS_NB; i++){
        lightTotal += blinnPhong(i);
    }
    fFragColor = vec4(lightTotal,1.0);
}
