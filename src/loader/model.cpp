#include "model.hpp"

Model::Model(std::string filename, std::string mtl_path)
    : m_filename(filename), m_mtl_path(mtl_path)
{
    tinyobj::LoadObj(&m_attrib, &m_shapes, &m_materials, &m_warning_message, &m_error_message, m_filename.c_str(), m_mtl_path.c_str());
}