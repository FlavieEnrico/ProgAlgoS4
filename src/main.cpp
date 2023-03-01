#include <stdlib.h>
#include <vector>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
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
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();
    std::vector<std::vector<float>> coord;

    for (int i = 0; i < 100; i++)
    {
        std::vector<float> pos;
        pos.push_back(p6::random::number(-1, 1));
        pos.push_back(p6::random::number(-1, 1));

        std::vector<float> depl;
        depl.push_back(ceil(p6::random::number(-2, 1)));
        depl.push_back(ceil(p6::random::number(-2, 1)));

        while (depl[0] == 0 && depl[1] == 0)
        {
            depl[0] = ceil(p6::random::number(-2, 1));
            depl[1] = ceil(p6::random::number(-2, 1));
        }

        coord.push_back(pos);
        coord.push_back(depl);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::ChartreuseWeb);
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );
        for (int i = 0; i < 100; i++)
        {
            ctx.equilateral_triangle(
                p6::Center(coord[i][0], coord[i][1]),
                p6::Radius{0.1f},
                p6::Rotation{p6::Radians{0}}
            );
        }
        for (int i = 0; i < 100; i++)
        {
            coord[i][0] = coord[i][0] + 0.001f * coord[i + 1][0];
            coord[i][1] = coord[i][1] + 0.001f * coord[i + 1][1];
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}