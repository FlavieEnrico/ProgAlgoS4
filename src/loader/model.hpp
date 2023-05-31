#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "glm/fwd.hpp"
#include "loader/tiny_obj_loader.h"
#include "p6/p6.h"

class Model {
private:
    tinyobj::attrib_t                m_attrib{};
    std::vector<tinyobj::shape_t>    m_shapes{};
    std::vector<tinyobj::material_t> m_materials{};
    std::string                      m_warning_message, m_error_message{};
    std::string                      m_filename;
    std::string                      m_mtl_path;
    GLuint                           m_vbo_vertices;
    GLuint                           m_vbo_normals;
    GLuint                           m_vbo_texcoords;
    GLuint                           m_vao;
    std::vector<glm::vec3>           m_ordered_vertices;
    std::vector<glm::vec3>           m_ordered_normals;
    std::vector<glm::vec2>           m_ordered_texcoords;

public:
    Model() = default;
    Model(std::string m_filename, std::string m_mtl_path);
    ~Model();
    void create_vbo();
    void create_fill_vao();
    void draw_model(p6::Shader& Shader, const glm::mat4& ViewMatrix, const glm::mat4& ProjMatrix, float size, glm::vec3 direction, glm::vec3 position);
    void order_vertices();
    void order_normals();
    void order_texcoords();
};