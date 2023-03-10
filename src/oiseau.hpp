#pragma once
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Oiseau {
private:
    int          m_id_oiseau;
    p6::Point2D  m_position;
    p6::Rotation m_angle_rotation;
    p6::Radius   m_rayon;
    glm::vec2    m_deplacement_direction;
    bool         m_in_murmuration = false;

public:
    Oiseau(p6::Point2D position, p6::Rotation angle_rotation, p6::Radius rayon);
    ~Oiseau() = default;
};

class Murmuration {
private:
    glm::vec2           m_centre_coordonnées;
    int                 m_nb_oiseau;
    std::vector<Oiseau> m_membres_murmurations;

public:
    ~Murmuration() = default;
};