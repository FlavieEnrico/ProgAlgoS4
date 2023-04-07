#pragma once
#include "boid.hpp"
#include <cstdio>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
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

    m_radius    = 0.1f;
    m_speed     = 0.001f;
    m_direction = p6::random::direction();
}

void Boid::draw(Boid& my_boid, p6::Context& context)
{
    context.equilateral_triangle(
        p6::Center(my_boid.m_position),
        p6::Radius{my_boid.m_radius},
        p6::Rotation{my_boid.m_direction}
    );
}

glm::vec2 Boid::alignment(std::vector<Boid>& flock)
{
    float perception_radius = m_radius * 5.0f;
    int   nb_near_boids     = 0;

    glm::vec2 steering(0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this != &other_boid)
        {
            float current_dist = glm::distance(this->m_position, other_boid.m_position);
            if (current_dist < perception_radius)
            {
                steering += glm::normalize(other_boid.m_direction);
                nb_near_boids++;
            }
        }
    }

    if (nb_near_boids > 0)
    {
        steering /= static_cast<float>(nb_near_boids);
    }

    return steering;
}

glm::vec2 Boid::cohesion(std::vector<Boid>& flock)
{
    float perception_radius = m_radius * 5.0f;
    int   nb_near_boids     = 0;

    glm::vec2 steering(0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this != &other_boid)
        {
            float current_dist = glm::distance(this->m_position, other_boid.m_position);
            if (current_dist < perception_radius)
            {
                steering += other_boid.m_position;
                nb_near_boids++;
            }
        }
    }
    // if there are one or several boids around my_boid then change his behavior
    if (nb_near_boids > 0)
    {
        // average the vector according to the number of near boids
        steering /= static_cast<float>(nb_near_boids);
        steering -= this->m_position;
    }
    // steering = glm::normalize(steering);
    return steering;
}

glm::vec2 Boid::separation(std::vector<Boid>& flock)
{
    float max_dist = m_radius * 2.0f;

    int       nb_near_boids = 0;
    glm::vec2 difference(0.f, 0.f);
    glm::vec2 steering(0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this != &other_boid)
        {
            float current_dist = glm::distance(this->m_position, other_boid.m_position);
            if (current_dist < max_dist)
            {
                difference = this->m_position - other_boid.m_position;
                // The difference is inversely proportional to the distance
                if (current_dist != 0)
                {
                    difference /= current_dist;
                }
                steering += difference;
                nb_near_boids++;
            }
        }
    }
    // if there are one or several boids around my_boid then change his behavior
    if (nb_near_boids > 0)
    {
        // average the vector according to the number of near boids
        steering /= nb_near_boids;
    }
    // steering = glm::normalize(steering);

    return steering;
}

void Boid::collision()
{
    float     wall_dist = 0.5f; // set the distance from the wall to start avoiding
    glm::vec2 avoidance(0.f, 0.f);

    if (this->m_position.x >= 1 - wall_dist)
    {
        avoidance.x = -1;
    }
    if (this->m_position.x <= -1 + wall_dist)
    {
        avoidance.x = 1;
    }
    if (this->m_position.y >= 1 - wall_dist)
    {
        avoidance.y = -1;
    }
    if (this->m_position.y <= -1 + wall_dist)
    {
        avoidance.y = 1;
    }

    if (avoidance != glm::vec2(0.f, 0.f))
    {
        this->m_direction += avoidance;
    }

    if (this->m_position.x >= 1)
    {
        this->m_position.x = 1;
    }
    if (this->m_position.x <= -1)
    {
        this->m_position.x = -1;
    }
    if (this->m_position.y >= 1)
    {
        this->m_position.y = 1;
    }
    if (this->m_position.y <= -1)
    {
        this->m_position.y = -1;
    }
}

void Boid::update_position(std::vector<Boid>& flock)
{
    this->m_direction += this->cohesion(flock);
    this->m_direction += this->separation(flock);
    this->m_direction += this->alignment(flock);

    float turning_rate = glm::mix(0.1f, 0.5f, m_speed);

    // interpolate between the current direction and the desired direction based on the turning rate
    m_direction = glm::mix(m_direction, glm::normalize(m_direction), turning_rate);

    // update the position based on the new direction and speed
    m_position += m_direction * m_speed;
    collision();
}