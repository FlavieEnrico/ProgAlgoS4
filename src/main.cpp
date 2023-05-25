#include <stdlib.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../src-common/glimac/FreeflyCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/cone_vertices.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "loader/model.hpp"
#include "p6/p6.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "loader/tiny_obj_loader.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "boid/boid.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available =
            argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    auto ctx = p6::Context{{.title = "ProgAlgoS4"}};
    ctx.maximize_window();

    int        width  = ctx.current_canvas_width();
    int        height = ctx.current_canvas_height();
    p6::Shader Shader =
        p6::load_shader("shaders/Boid.vs.glsl", "shaders/Boid.fs.glsl");

    // initialize Matrix

    FreeflyCamera camera = FreeflyCamera();
    camera.moveFront(-5);
    glm::mat4 ProjMatrix = glm::perspective(
        glm::radians(70.f),
        (static_cast<float>(width) / static_cast<float>(height)), 0.1f, 100.f
    );
    glm::mat4 ViewMatrix = camera.getViewMatrix();

    // tinyobj::attrib_t                my_attrib;
    // std::vector<tinyobj::shape_t>    my_shapes;
    // std::vector<tinyobj::material_t> my_materials;
    // std::string                      warning_message, error_message;
    // std::string                      my_filename = "./assets/models/cube.obj";
    // std::string                      mtl_path    = "./assets/models/";

    // bool loadTest = tinyobj::LoadObj(&my_attrib, &my_shapes, &my_materials, &warning_message, &error_message, (my_filename.c_str()), (mtl_path.c_str()));

    //  VBO
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // std::vector<glimac::ShapeVertex> my_cone;
    // cone
    std::vector<glimac::ShapeVertex> my_cone =
        glimac::cone_vertices(1.f, 0.5f, 32, 16);

    // Remplir VBO
    glBufferData(GL_ARRAY_BUFFER, my_cone.size() * sizeof(glimac::ShapeVertex), my_cone.data(), GL_STATIC_DRAW);

    // VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // activate vertex attributes
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    const GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    const GLuint VERTEX_ATTR_UV = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_UV);

    // specification vertex attributes
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
        (const GLvoid*)(offsetof(glimac::ShapeVertex, position))
    );
    glVertexAttribPointer(
        VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
        (const GLvoid*)(offsetof(glimac::ShapeVertex, normal))
    );
    glVertexAttribPointer(
        VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
        (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords))
    );

    // Debinding VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Debinding VAO
    glBindVertexArray(0);

    // our boids

    std::vector<Boid> flock;
    float             size_boids       = 0.1f;
    float             separation_force = 2.0f;
    float             alignment_force  = 5.0f;
    float             cohesion_force   = 2.0f;

    flock.resize(10);

    // Declare your infinite update loop.
    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        {
            int nb_boid = flock.size();
            if (ImGui::SliderFloat("Size", &size_boids, 0.01, 0.7))
            {
                for (auto& boid : flock)
                {
                    boid.updateRadius(size_boids);
                }
            }
            if (ImGui::SliderInt("Number", &nb_boid, 1, 150))
            {
                flock.resize(nb_boid);
            }
            ImGui::SliderFloat("Separation Force", &separation_force, 2.0f, 7.0f);
            ImGui::SliderFloat("Alignment Force", &alignment_force, 2.0f, 7.0f);
            ImGui::SliderFloat("Cohesion Force", &cohesion_force, 0.0f, 7.0f);
        }
        ImGui::End();
    };

    bool Z = false;
    bool Q = false;
    bool S = false;
    bool D = false;

    glEnable(GL_DEPTH_TEST);
    ctx.update = [&]() {
        if (Z)
        {
            camera.moveFront(0.1);
        }
        if (Q)
        {
            camera.moveLeft(0.1);
        }
        if (S)
        {
            camera.moveFront(-0.1);
        }
        if (D)
        {
            camera.moveLeft(-0.1);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Binding VAO

        glBindVertexArray(vao);

        // glDrawArrays(GL_TRIANGLES, 0, my_cone.size());
        /*
        ctx.background(p6::NamedColor::ChartreuseWeb);
        */
        for (auto& boid : flock)
        {
            boid.update_position(flock, separation_force, alignment_force, cohesion_force);

            boid.draw(Shader, ViewMatrix, ProjMatrix, my_cone);
        }
    };
    glBindVertexArray(0);

    ctx.key_pressed = [&Z, &Q, &S, &D](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = true;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = true;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = true;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = true;
        }
    };

    ctx.key_released = [&Z, &Q, &S, &D](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = false;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = false;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = false;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = false;
        }
    };

    ctx.mouse_dragged = [&camera](const p6::MouseDrag& button) {
        camera.rotateLeft(button.delta.x * 50);
        camera.rotateUp(-button.delta.y * 50);
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Clear vbo & vao & texture
    // glDeleteTextures(1, &my_texture);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}