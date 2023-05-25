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

void Arpenteur::drawArpenteur(const p6::Shader& Shader, const glm::mat4& ViewMatrix, const glm::mat4& ProjMatrix, const std::vector<glimac::ShapeVertex>& my_cone)
{
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    ModelMatrix           = glm::translate(ModelMatrix, this->m_position);
    // ModelMatrix = glm::translate(ModelMatrix, glm::vec3(m_position.x, m_position.y, m_position.z));
    //  construct matrix from axes
    glm::mat4 rotationMatrix = glm::rotate(ModelMatrix, m_rotate, glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix              = ModelMatrix * rotationMatrix;
    ModelMatrix              = glm::scale(ModelMatrix, glm::vec3(this->m_size));

    glm::mat4 MVMatrix = ViewMatrix * ModelMatrix;

    glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    Shader.set("uMVMatrix", MVMatrix);
    Shader.set("uMVPMatrix", MVPMatrix);
    Shader.set("uNormalMatrix", NormalMatrix);

    glDrawArrays(GL_TRIANGLES, 0, my_cone.size());
}