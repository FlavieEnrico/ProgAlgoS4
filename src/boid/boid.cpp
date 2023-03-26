#pragma once
#include "boid.hpp"
#include <cstdio>
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

glm::vec2 Boid::alignment(std::vector<Boid>& flock)
{
    float perception_radius = m_radius * 5.0f;
    int   nb_near_boids     = 0;

    glm::vec2 steering(0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this != &other_boid)
        {
            float current_dist = this->distance(other_boid);
            if (current_dist < perception_radius)
            {
                steering += other_boid.m_direction;
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
            float current_dist = this->distance(other_boid);
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
            float current_dist = this->distance(other_boid);
            if (current_dist < max_dist)
            {
                difference = this->m_position - other_boid.m_position;
                // The difference is inversely proportional to the distance
                difference /= current_dist;
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

float Boid::distance(const Boid& other_boid)
{
    return glm::length(this->m_position - other_boid.m_position);
}

void Boid::collision()
{
    float     wall_dist = 0.1f; // set the distance from the wall to start avoiding
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
    this->m_position = this->m_position + this->m_direction * this->m_speed;
    collision();
}