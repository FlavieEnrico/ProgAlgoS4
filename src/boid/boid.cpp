#pragma once
#include "boid.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

Boid::Boid(glm::vec2 position, glm::vec2 direction, float radius, float speed)
    : m_position(position), m_direction(direction), m_radius(radius), m_speed(speed)
{
}

Boid::Boid()
{
    m_position.x = p6::random::number(-1, 1);
    m_position.y = p6::random::number(-1, 1);

    m_direction.x = p6::random::number(-1, 1);
    m_direction.y = p6::random::number(-1, 1);

    while (m_direction.x == 0 && m_direction.y == 0)
    {
        m_direction.x = p6::random::number(-1, 1);
        m_direction.y = p6::random::number(-1, 1);
    }

    m_radius = 0.1f;
    m_speed  = 0.001f;
}

void Boid::draw(Boid& my_boid, p6::Context& context)
{
    context.equilateral_triangle(
        p6::Center(my_boid.m_position.x, my_boid.m_position.y),
        p6::Radius{my_boid.m_radius},
        p6::Rotation{my_boid.m_direction}
    );
}

void Boid::updatePosition(Boid& my_boid)
{
    my_boid.m_position = my_boid.m_position + my_boid.m_direction * my_boid.m_speed;
}