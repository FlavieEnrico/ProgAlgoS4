#pragma once

#include <glm/glm.hpp>
#include "../src/arpenteur/arpenteur.hpp"

class TrackballCamera {
private:
    float     m_fDistance;
    float     m_fAngleX;
    float     m_fAngleY;
    float     m_height = 0.5;
    glm::vec3 m_coordinates;

public:
    explicit TrackballCamera(float Distance = 0.f, float AngleX = 0, float AngleY = 0, glm::vec3 m_coordinates = {0.f, 0.1f, 0.f});

    void      moveFront(float delta);
    void      rotateLeft(float degrees);
    void      rotateUp(float degrees);
    void      setCoordinates(Arpenteur& arpenteur);
    glm::vec3 getCoordinates();
    glm::mat4 getViewMatrix() const;
};
