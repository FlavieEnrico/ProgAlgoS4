#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/TrackballCamera.hpp"

/*
 glm::mat4 ProjMatrix= glm::perspective(glm::radians(70.f),((float)width/(float)height),0.1f, 100.f);
    glm::mat4 MVMatrix(1.0f); 
    MVMatrix= glm::translate(MVMatrix, glm::vec3(0.f,0.f,-5.f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));*/

void TrackballCamera::moveFront(float delta){
    this->m_fDistance += delta;
};
void TrackballCamera::rotateLeft(float degrees){
    this->m_fAngleX += degrees;
};
void TrackballCamera::rotateUp(float degrees){
    this->m_fAngleY += degrees;
};

glm::mat4 TrackballCamera::getViewMatrix() const {

    glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f,0.f,-this->m_fDistance));
    ViewMatrix = glm::rotate(ViewMatrix, glm::radians(this->m_fAngleX),glm::vec3(1.f, 0.f, 0.f));
    ViewMatrix = glm::rotate(ViewMatrix, glm::radians(this->m_fAngleY),glm::vec3(0.f, 1.f, 0.f));
    return ViewMatrix;

}