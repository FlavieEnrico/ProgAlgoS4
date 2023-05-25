#include "TrackballCamera.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

TrackballCamera::TrackballCamera(float Distance, float AngleX, float AngleY, glm::vec3 Coordinates)
    : m_fDistance(Distance), m_fAngleX(AngleX), m_fAngleY(AngleY), m_coordinates(Coordinates) {}

void TrackballCamera::moveFront(float delta)
{
    m_fDistance -= delta;
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_fAngleY += degrees;
}

void TrackballCamera::rotateUp(float degrees)
{
    m_fAngleX += degrees;
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 matrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -m_fDistance));
    matrix           = glm::rotate(matrix, glm::radians(m_fAngleY), glm::vec3(0, 1, 0));
    matrix           = glm::rotate(matrix, glm::radians(m_fAngleX), glm::vec3(1, 0, 0));

    return matrix;
}

void TrackballCamera::setCoordinates(Arpenteur& arpenteur)
{
    float dist_arpenteur = 0.5f;

    m_coordinates.y = arpenteur.getPosition().y + m_height;
    m_coordinates.x = arpenteur.getPosition().x + dist_arpenteur * glm::cos(-p6::PI);
    m_coordinates.z = arpenteur.getPosition().z + dist_arpenteur * glm::sin(-p6::PI);
}

glm::vec3 TrackballCamera::getCoordinates()
{
    return m_coordinates;
}