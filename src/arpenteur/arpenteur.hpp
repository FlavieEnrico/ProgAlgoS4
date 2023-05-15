#pragma once
#include <stdlib.h>
#include <vector>
#include "../src-common/glimac/FreeflyCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "p6/p6.h"

class Arpenteur {
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    float     m_speed;

public:
    Arpenteur(glm::vec3 position, glm::vec3 direction, float speed);
    Arpenteur();
    ~Arpenteur() = default;
};