
#include <glm/glm.hpp>
#include "../src-common/glimac/FreeflyCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "light/light_manager.hpp"
#include "loader/model.hpp"
#include "p6/p6.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "loader/tiny_obj_loader.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "arpenteur/arpenteur.hpp"
#include "boid/boid.hpp"
#include "doctest/doctest.h"

void draw_lod(p6::Shader& Shader, const glm::mat4& ViewMatrix, const glm::mat4& ProjMatrix, FreeflyCamera camera, Boid my_boid, Model key, Model my_cube);

int main(int argc, char* argv[])
{
    auto ctx = p6::Context{{.title = "ProgAlgoS4"}};
    ctx.maximize_window();

    int        width  = ctx.current_canvas_width();
    int        height = ctx.current_canvas_height();
    p6::Shader Shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/PointLight.fs.glsl");

    // Arpenteur and camera
    Arpenteur     arpenteur;
    FreeflyCamera camera = FreeflyCamera(glm::vec3{arpenteur.getPosition().x, arpenteur.getPosition().y, arpenteur.getPosition().z});

    // initialize projMatrix
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ((float)width / (float)height), 0.1f, 100.f);

    // Load all the 3D Models
    Model my_cube("../../assets/models/cube.obj", "../../assets/models/");
    my_cube.create_vbo();

    Model broom_arpenteur("../../assets/models/broom_smoothed_triangle.obj", "../../assets/models/");
    broom_arpenteur.create_vbo();

    Model key("../../assets/models/key.obj", "../../assets/models/");
    key.create_vbo();

    Model cage("../../assets/models/cage.obj", "../../assets/models/");
    cage.create_vbo();

    // our boids

    std::vector<Boid> flock;
    float             size_boids       = 0.1f;
    float             separation_force = 2.0f;
    float             alignment_force  = 5.0f;
    float             cohesion_force   = 2.0f;

    flock.resize(10);

    // Declare your infinite update loop.
    ctx.imgui = [&]() {
        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("Controls"))
            {
                ImGui::Text("Pour se déplacer : Z pour avancer, S pour reculer, Q pour tourner à gauche et D pour tourner à droite. R pour descendre et shift pour monter.");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

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

    // light
    Light pointLight1(arpenteur.getPosition(), 0.6f, glm::vec3(1.0f, 1.f, 0.8f));
    Light pointLight2(glm::vec3(-0.3f, 0.2f, -2.f), 0.3f, glm::vec3(1.f, 0.7f, 0.5f));
    bool  Z     = false;
    bool  Q     = false;
    bool  S     = false;
    bool  D     = false;
    bool  R     = false;
    bool  shift = false;
    camera.rotateUp(-20);

    glEnable(GL_DEPTH_TEST);

    ctx.update = [&]() {
        if (Z)
        {
            arpenteur.moveForward();
        }
        if (Q)
        {
            arpenteur.rotateLeft();
        }
        if (S)
        {
            arpenteur.moveBackward();
        }
        if (D)
        {
            arpenteur.rotateRight();
        }
        if (shift)
        {
            arpenteur.moveUp();
        }
        if (R)
        {
            arpenteur.moveDown();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Shader.use();

        // set lights
        Shader.set("uKa", glm::vec3(0.2f, 1.f, 0.8f));
        Shader.set("uKd", glm::vec3(0.8f, 0.8f, 0.8f));
        Shader.set("uKs", glm::vec3(1.0f, 0.8f, 1.0f));
        Shader.set("uShininess", 100.0f);
        Shader.set("uLightIntensity[1]", pointLight2.getIntensity());
        Shader.set("uLightIntensity[0]", pointLight1.getIntensity());
        Shader.set("uLightColor[1]", pointLight2.getColor());
        Shader.set("uLightColor[0]", pointLight1.getColor());
        Shader.set("uLightPos_vs[0]", pointLight1.getPosition());
        Shader.set("uLightPos_vs[1]", pointLight2.getPosition());
        Shader.set("uAmbientLightColor", glm::vec3(1.f, 1.f, 1.f));
        Shader.set("uAmbientLightIntensity", 0.1f);

        arpenteur.setDirection();

        camera.setPos(glm::vec3{arpenteur.getPosition().x, arpenteur.getPosition().y + 0.2, arpenteur.getPosition().z + 0.2});

        glm::mat4 ViewMatrix = camera.getViewMatrix();

        broom_arpenteur.draw_model(Shader, ViewMatrix, ProjMatrix, 0.1f, -(arpenteur.getDirection()), arpenteur.getPosition());

        cage.draw_model(Shader, ViewMatrix, ProjMatrix, 0.15f, glm::vec3(1.0, 0.f, 0.f), glm::vec3(0.f, -2.f, 0.f));

        for (auto& boid : flock)
        {
            boid.update_position(flock, separation_force, alignment_force, cohesion_force);

            draw_lod(Shader, ViewMatrix, ProjMatrix, camera, boid, key, my_cube);
        }
        my_cube.draw_model(Shader, ViewMatrix, ProjMatrix, 2.f, glm::vec3(1.0, 0.f, 0.f), glm::vec3(0.f));
    };

    ctx.key_pressed = [&Z, &Q, &S, &D, &R, &shift](const p6::Key& key) {
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
        if (key.physical == GLFW_KEY_LEFT_SHIFT)
        {
            shift = true;
        }
        if (key.physical == GLFW_KEY_R)
        {
            R = true;
        }
    };

    ctx.key_released = [&Z, &Q, &S, &D, &R, &shift](const p6::Key& key) {
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
        if (key.physical == GLFW_KEY_LEFT_SHIFT)
        {
            shift = false;
        }
        if (key.physical == GLFW_KEY_R)
        {
            R = false;
        }
    };

    ctx.mouse_dragged = [&camera](const p6::MouseDrag& button) {
        camera.rotateLeft(button.delta.x * 50);
        camera.rotateUp(-button.delta.y * 50);
    };

    ctx.start();

    return 0;
}

void draw_lod(p6::Shader& Shader, const glm::mat4& ViewMatrix, const glm::mat4& ProjMatrix, FreeflyCamera camera, Boid my_boid, Model key, Model my_cube)
{
    if (glm::distance(my_boid.getPosition(), camera.getPos()) < 2)
    {
        key.draw_model(Shader, ViewMatrix, ProjMatrix, 0.1f * my_boid.getSize(), my_boid.getDirection(), my_boid.getPosition());
    }
    else
    {
        my_cube.draw_model(Shader, ViewMatrix, ProjMatrix, 0.1f * my_boid.getSize(), my_boid.getDirection(), my_boid.getPosition());
    }
}