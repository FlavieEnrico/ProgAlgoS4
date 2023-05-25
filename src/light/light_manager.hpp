#pragma once
#include <stdlib.h>
#include <vector>
#include "../src-common/glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "p6/p6.h"

class Light {
private:
    float     m_intensity;
    glm::vec3 m_color;
    glm::vec3 m_position;

public:
    Light(glm::vec3 position, float intensity, glm::vec3 color)
        : m_position(position), m_intensity(intensity), m_color(color){};
    ;

    // getter
    glm::vec3 getColor() const { return m_color; };
    float     getIntensity() const { return m_intensity; };
    glm::vec3 getPosition() const { return m_position; };

    ~Light() = default;
};