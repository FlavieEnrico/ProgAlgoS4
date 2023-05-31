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
    collision();
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
    collision();
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
    collision();
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
    collision();
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

void Arpenteur::collision()
{
    float distance_arp_camera = 0.2f;
    float wall_position       = 2.f;

    if (this->m_position.x >= wall_position)
    {
        this->m_position.x = wall_position;
    }
    if (this->m_position.x <= -wall_position)
    {
        this->m_position.x = -wall_position;
    }
    if (this->m_position.y >= wall_position)
    {
        this->m_position.y = wall_position;
    }
    if (this->m_position.y <= -wall_position)
    {
        this->m_position.y = -wall_position;
    }
    if (this->m_position.z >= wall_position)
    {
        this->m_position.z = wall_position;
    }
    if (this->m_position.z <= -wall_position)
    {
        this->m_position.z = -wall_position;
    }
}