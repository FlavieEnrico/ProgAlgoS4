#include "boid.hpp"

Boid::Boid(glm::vec3 position, glm::vec3 direction, float radius, float speed)
    : m_position(position), m_direction(direction), m_radius(radius), m_speed(speed)
{
}

Boid::Boid()
{
    m_position.x = p6::random::number(-1, 1);
    m_position.y = p6::random::number(-1, 1);
    m_position.z = p6::random::number(-1, 1);

    m_radius = 0.1f;
    m_speed  = 0.01f;
    // m_direction = p6::random::direction();
    m_direction.x = p6::random::number(-1, 1);
    m_direction.y = p6::random::number(-1, 1);
    m_direction.z = p6::random::number(-1, 1);
}

void Boid::updateRadius(float newRadius)
{
    this->m_radius = newRadius;
}

glm::vec3 Boid::alignment(std::vector<Boid>& flock, float perception_radius)
{
    int nb_near_boids = 0;

    glm::vec3 steering(0.f, 0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this == &other_boid)
            continue;
        float current_dist = glm::distance(this->m_position, other_boid.m_position);
        if (current_dist > perception_radius)
            continue;
        steering += glm::normalize(other_boid.m_direction);
        nb_near_boids++;
    }

    if (nb_near_boids > 0)
    {
        steering /= static_cast<float>(nb_near_boids);
    }

    return steering;
}

glm::vec3 Boid::cohesion(std::vector<Boid>& flock, float perception_radius)
{
    int nb_near_boids = 0;

    glm::vec3 steering(0.f, 0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this == &other_boid)
            continue;
        float current_dist = glm::distance(this->m_position, other_boid.m_position);
        if (current_dist > perception_radius)
            continue;
        steering += other_boid.m_position;
        nb_near_boids++;
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

glm::vec3 Boid::separation(std::vector<Boid>& flock, float perception_radius)
{
    int       nb_near_boids = 0;
    glm::vec3 difference(0.f, 0.f, 0.f);
    glm::vec3 steering(0.f, 0.f, 0.f);

    for (auto& other_boid : flock)
    {
        if (this == &other_boid)
            continue;

        float current_dist = glm::distance(this->m_position, other_boid.m_position);
        if (current_dist >= perception_radius)
            continue;

        difference = this->m_position - other_boid.m_position;
        // The difference is inversely proportional to the distance
        if (current_dist == 0.00001f)
            continue;

        // difference = difference / static_cast<float>(difference.length());
        difference /= current_dist;
        steering += difference;
        nb_near_boids++;
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
    glm::vec3 avoidance(0.f, 0.f, 0.f);
    float     wall_position = 2.f;

    if (this->m_position.x >= wall_position - wall_dist)
    {
        avoidance.x = -wall_position;
    }
    if (this->m_position.x <= -wall_position + wall_dist)
    {
        avoidance.x = wall_position;
    }
    if (this->m_position.y >= wall_position - wall_dist)
    {
        avoidance.y = -wall_position;
    }
    if (this->m_position.y <= -wall_position + wall_dist)
    {
        avoidance.y = wall_position;
    }
    if (this->m_position.z >= wall_position - wall_dist)
    {
        avoidance.z = -wall_position;
    }
    if (this->m_position.z <= -wall_position + wall_dist)
    {
        avoidance.z = wall_position;
    }

    if (avoidance != glm::vec3(0.f, 0.f, 0.f))
    {
        this->m_direction += avoidance;
    }
}

void Boid::change_turning_rate()
{
    // Necessary: Otherwise, the boids are turning too slowly and collide with the walls.

    // Modify the turning rate for each dimension separately
    float turning_rate = glm::mix(0.1f, 0.5f, this->m_speed);

    // Create a new direction vector with the modified values
    m_direction = glm::mix(this->m_direction, glm::normalize(this->m_direction), turning_rate);
}

void Boid::update_position(std::vector<Boid>& flock, float& separation_force, float& alignment_force, float& cohesion_force)
{
    this->m_direction += this->cohesion(flock, this->m_radius * cohesion_force);
    this->m_direction += this->separation(flock, this->m_radius * separation_force);
    this->m_direction += this->alignment(flock, this->m_radius * alignment_force);
    collision();

    m_direction = glm::normalize(m_direction);
    //  m_position += this->change_turning_rate() * m_speed;
    change_turning_rate();
    m_position += m_direction * m_speed;
}