#pragma once
#include <stdlib.h>
#include <cstdio>
#include <filesystem>
#include <vector>
#include "../src-common/glimac/FreeflyCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/cone_vertices.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"
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
    // void draw(Boid& my_boid, p6::Context& context, float& size_boid)
    void      draw(p6::Shader& Shader, const glm::mat4& ViewMatrix, const glm::mat4& ProjMatrix, const std::vector<glimac::ShapeVertex>& my_cone);
    void      update_position(std::vector<Boid>& flock, float& separation_force, float& alignment_force, float& cohesion_force);
    void      updateRadius(float newRadius);
    glm::vec3 separation(std::vector<Boid>& flock, float perception_radius);
    glm::vec3 cohesion(std::vector<Boid>& flock, float perception_radius);
    glm::vec3 alignment(std::vector<Boid>& flock, float perception_radius);
    void      collision();
    void      change_turning_rate();
};