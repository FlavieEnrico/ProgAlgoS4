#pragma once
#include <stdlib.h>
#include <vector>
#include "../src-common/glimac/FreeflyCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/cone_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
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
    void      draw(const Boid& my_boid, p6::Shader& Shader, FreeflyCamera viewMatrix, glm::mat4 MVMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, const std::vector<glimac::ShapeVertex>& my_cone);
    void      update_position(std::vector<Boid>& flock, float& size_boid, float& separation_force, float& alignment_force, float& cohesion_force);
    glm::vec3 separation(std::vector<Boid>& flock, float perception_radius);
    glm::vec3 cohesion(std::vector<Boid>& flock, float perception_radius);
    glm::vec3 alignment(std::vector<Boid>& flock, float perception_radius);
    void      collision();
    glm::vec3 change_turning_rate();
};