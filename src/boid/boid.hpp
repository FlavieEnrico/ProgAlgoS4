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
    void static draw(Boid& my_boid, p6::Context& context);
    void      update_position(Boid& my_boid);
    float     distance(const Boid& other_boid);
    glm::vec2 separation(Boid& my_boid);
};