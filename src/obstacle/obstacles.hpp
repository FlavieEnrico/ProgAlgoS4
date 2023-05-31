#pragma once
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "../src-common/glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

// Classe Obstacle représentant un obstacle cubique
class Obstacle {
private:
    glm::vec3 position;
    float     size;

public:
    Obstacle(float size)
        : size(size)
    {
        // Générer une position aléatoire pour l'obstacle
        std::srand(std::time(nullptr));
        position.x = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
        position.y = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
        position.z = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f;
    }

    void drawObstacle() const;
};

// Classe Obstacles contenant une liste d'obstacles cubiques
class Obstacles {
private:
    std::vector<Obstacle> obstacles;

public:
    Obstacles(int count, float size)
    {
        // Générer les obstacles
        for (int i = 0; i < count; ++i)
        {
            Obstacle obstacle(size);
            obstacles.push_back(obstacle);
        }
    }

    void drawObstacles() const
    {
        // Dessiner tous les obstacles
        for (const Obstacle& obstacle : obstacles)
        {
            obstacle.drawObstacle();
        }
    }
};