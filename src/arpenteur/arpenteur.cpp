#include "arpenteur.hpp"

float max_speed = 0.05;
float min_speed = 0.02;

void Arpenteur::moveForward()
{
    if (m_speed + m_acceleration < max_speed)
    {
        m_speed += m_acceleration;
    }
    else
    {
        m_speed = max_speed;
    }
    m_position.x += m_speed * m_direction.x;
    m_position.z += m_speed * m_direction.z;
}

void Arpenteur::moveBackward()
{
    if (min_speed < m_speed - m_acceleration)
    {
        m_speed -= m_acceleration;
    }
    else
    {
        m_speed = min_speed;
    }

    m_position.x -= m_speed * m_direction.x;
    m_position.z -= m_speed * m_direction.z;
}

void Arpenteur::moveUp()
{
    if (m_verticalSpeed + m_acceleration < max_speed)
    {
        m_verticalSpeed += m_acceleration;
    }
    else
    {
        m_verticalSpeed = max_speed;
    }
    m_position.y += m_verticalSpeed;
}

void Arpenteur::moveDown()
{
    if (min_speed < m_verticalSpeed - m_acceleration)
    {
        m_verticalSpeed -= m_acceleration;
    }
    else
    {
        m_verticalSpeed = min_speed;
    }
    m_position.y -= m_verticalSpeed;
}

void Arpenteur::rotateLeft()
{
    m_rotate -= 0.1 * (p6::PI / 6);
}

void Arpenteur::rotateRight()
{
    m_rotate += 0.1 * (p6::PI / 6);
}

void Arpenteur::setDirection()
{
    m_direction.x = glm::cos(m_rotate);
    m_direction.z = glm::sin(m_rotate);
}