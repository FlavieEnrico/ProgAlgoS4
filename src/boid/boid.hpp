#pragma once
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_direction;
    float     m_radius;
    float     m_speed;

public:
    Boid(glm::vec2 position, glm::vec2 direction, float radius, float speed);
    Boid();
    ~Boid() = default;
    void static draw(Boid& my_boid, p6::Context& context, float& size_boid);
    void      update_position(std::vector<Boid>& flock, float& size_boid, float& separation_force, float& alignment_force, float& cohesion_force);
    glm::vec2 separation(std::vector<Boid>& flock, float perception_radius);
    glm::vec2 cohesion(std::vector<Boid>& flock, float perception_radius);
    glm::vec2 alignment(std::vector<Boid>& flock, float perception_radius);
    void      collision();
    glm::vec2 change_turning_rate();
};