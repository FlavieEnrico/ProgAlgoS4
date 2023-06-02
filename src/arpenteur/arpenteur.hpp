#pragma once
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
    float     m_acceleration = 0.001;

    float m_size = 0.1;

    float m_rotate = -(p6::PI / 2);

    float m_verticalSpeed = 0.0005;

public:
    Arpenteur(glm::vec3 position = glm::vec3{0.f}, glm::vec3 direction = glm::vec3{1.f, 0.f, 0.f}, float speed = 0.001f)
        : m_position(position), m_direction(direction), m_speed(speed){};

    // move arpenteur
    void moveForward();
    void moveBackward();
    void moveDown();
    void moveUp();
    void rotateRight();
    void rotateLeft();

    void collision();

    void setDirection();

    // getter
    glm::vec3 getPosition() const { return m_position; };
    glm::vec3 getDirection() const { return m_direction; };
    float     getSpeed() const { return m_speed; };
    float     getRotation() const { return m_rotate; };

    // destructor
    ~Arpenteur() = default;
};