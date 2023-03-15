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

void Boid::draw(Boid& my_boid, p6::Context& context)
{
    context.equilateral_triangle(
        p6::Center(my_boid.m_position.x, my_boid.m_position.y),
        p6::Radius{my_boid.m_radius},
        p6::Rotation{my_boid.m_direction}
    );
}

void Boid::update(Boid& my_boid)
{
    my_boid.m_position = my_boid.m_position + my_boid.m_direction * my_boid.m_speed;
}

Boid Boid::create_new(float radius, float speed)
{
    glm::vec2 position;
    position.x = p6::random::number(-1, 1);
    position.y = p6::random::number(-1, 1);

    glm::vec2 direction;
    direction.x = p6::random::number(-1, 1);
    direction.y = p6::random::number(-1, 1);

    while (direction.x == 0 && direction.y == 0)
    {
        direction.x = p6::random::number(-1, 1);
        direction.y = p6::random::number(-1, 1);
    }

    return Boid(position, direction, radius, speed);
}