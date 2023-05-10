#pragma once
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    float     m_radius;
    float     m_speed;

public:
    Boid(glm::vec3 position, glm::vec3 direction, float radius, float speed);
    Boid();
    ~Boid() = default;
    void static draw(Boid& my_boid, p6::Context& context, float& size_boid);
    void      update_position(std::vector<Boid>& flock, float& size_boid, float& separation_force, float& alignment_force, float& cohesion_force);
    glm::vec3 separation(std::vector<Boid>& flock, float perception_radius);
    glm::vec3 cohesion(std::vector<Boid>& flock, float perception_radius);
    glm::vec3 alignment(std::vector<Boid>& flock, float perception_radius);
    void      collision();
    glm::vec3 change_turning_rate();
};