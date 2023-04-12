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
    float             size_boids = 0.1;

    flock.resize(10);

    // Declare your infinite update loop.
    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        {
            int nb_boid = flock.size();
            ImGui::SliderFloat("Size", &size_boids, 0.01, 0.7);
            if (ImGui::SliderInt("Number", &nb_boid, 1, 150))
            {
                flock.resize(nb_boid);
            }
        }
        ImGui::End();
        ImGui::ShowDemoWindow();
    };
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::ChartreuseWeb);
        for (auto& boid : flock)
        {
            boid.update_position(flock, size_boids);
            Boid::draw(boid, ctx, size_boids);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}