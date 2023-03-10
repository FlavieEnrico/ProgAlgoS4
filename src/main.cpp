#include <stdlib.h>
#include <vector>
#include "imgui.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "boid/boid.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "ProgAlgoS4"}};
    ctx.maximize_window();
    std::vector<Boid> flock;
    float             speed        = 0.001f;
    float             radius       = 0.1f;
    int               number_boids = 100;

    for (int i = 0; i < number_boids; i++)
    {
        glm::vec2 position;
        position.x = p6::random::number(-1, 1);
        position.y = p6::random::number(-1, 1);

        glm::vec2 direction;
        direction.x = p6::random::number(-1, 1);
        direction.y = p6::random::number(-1, 1);

        while (direction.x == 0 && direction.y == 0)
        {
            direction.x = p6::random::number(-1, 1);
            direction.y = p6::random::number(-1, 1);
        }

        flock.emplace_back(position, direction, radius, speed);
    }

    // Declare your infinite update loop.
    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        ImGui::SliderInt("Number", &number_boids, 1, 150);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::ChartreuseWeb);
        for (auto& i : flock)
        {
            Boid::update(i);
            Boid::draw(i, ctx);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}