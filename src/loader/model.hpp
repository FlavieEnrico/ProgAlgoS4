#pragma once
#include <string>
#include <vector>
#include "loader/tiny_obj_loader.h"

class Model {
private:
    tinyobj::attrib_t                m_attrib{};
    std::vector<tinyobj::shape_t>    m_shapes{};
    std::vector<tinyobj::material_t> m_materials{};
    std::string                      m_warning_message, m_error_message{};
    std::string                      m_filename;
    std::string                      m_mtl_path;

public:
    Model() = default;
    Model(std::string m_filename, std::string m_mtl_path);
    ~Model() = default;

    // faire une méthode qui initialise tous les vbo et les vao
};