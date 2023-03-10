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

Boid::Boid(Boid& other_b)
    : m_position(other_b.m_position), m_direction(other_b.m_direction), m_radius(other_b.m_radius), m_speed(other_b.m_speed)
{
}

void Boid::draw(Boid my_boid, p6::Context& context)
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