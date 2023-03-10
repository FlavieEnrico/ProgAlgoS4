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
    Boid(Boid& other_b);
    ~Boid() = default;
    void static draw(Boid my_boid, p6::Context& context);
    void static update(Boid& my_boid);
};