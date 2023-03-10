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

void Boid::draw(std::vector<Boid>& flock, p6::Context context)
{
    for (int i = 0; i < flock.size(); i++)
    {
        context.equilateral_triangle(
            p6::Center(flock[i].m_position.x, flock[i].m_position.y),
            p6::Radius{flock[i].m_radius},
            p6::Rotation{flock[i].m_direction}
        );
    }
}

std::vector<Boid> Boid::update(std::vector<Boid>& flock)
{
    std::vector<Boid> updated_flock;
    for (int i = 0; i < flock.size(); i++)
    {
        updated_flock[i].m_position = flock[i].m_position + flock[i].m_direction * flock[i].m_speed;
    }
    return updated_flock;
}