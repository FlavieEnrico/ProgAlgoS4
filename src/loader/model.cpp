#include "model.hpp"
#include "glm/fwd.hpp"
#include "loader/model.hpp"
#include "loader/tiny_obj_loader.h"

Model::Model(std::string filename, std::string mtl_path)
    : m_filename(filename), m_mtl_path(mtl_path)
{
    tinyobj::LoadObj(&m_attrib, &m_shapes, &m_materials, &m_warning_message, &m_error_message, m_filename.c_str(), m_mtl_path.c_str());
    // fonction qui prend les indices des vertex et construit les faces
    this->order_vertices();
    this->order_normals();
    // this->order_texcoords();
    m_vbo_vertices  = 8;
    m_vbo_normals   = 6;
    m_vbo_texcoords = 7;
    m_vao           = 5;
}

Model::~Model()
{
    glDeleteBuffers(1, &this->m_vbo_vertices);
    glDeleteBuffers(1, &this->m_vbo_normals);
    glDeleteBuffers(1, &this->m_vbo_texcoords);
    glDeleteBuffers(1, &this->m_vao);
}

void Model::order_vertices()
{
    for (const auto& index : this->m_shapes[0].mesh.indices)
    {
        glm::vec3 vertex{};
        vertex.x = this->m_attrib.vertices[3 * index.vertex_index + 0];
        vertex.y = this->m_attrib.vertices[3 * index.vertex_index + 1];
        vertex.z = this->m_attrib.vertices[3 * index.vertex_index + 2];
        this->m_ordered_vertices.push_back(vertex);
    }
}

void Model::order_normals()
{
    for (const auto& index : this->m_shapes[0].mesh.indices)
    {
        glm::vec3 vertex{};
        vertex.x = this->m_attrib.normals[3 * index.normal_index + 0];
        vertex.y = this->m_attrib.normals[3 * index.normal_index + 1];
        vertex.z = this->m_attrib.normals[3 * index.normal_index + 2];
        this->m_ordered_normals.push_back(vertex);
    }
}

void Model::order_texcoords()
{
    for (const auto& index : this->m_shapes[0].mesh.indices)
    {
        glm::vec2 vertex{};
        vertex.x = this->m_attrib.texcoords[2 * index.texcoord_index + 0];
        vertex.y = this->m_attrib.texcoords[2 * index.texcoord_index + 1];
        this->m_ordered_texcoords.push_back(vertex);
    }
}

void Model::create_vbo()
{
    glGenBuffers(1, &this->m_vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo_vertices);

    glBufferData(GL_ARRAY_BUFFER, this->m_ordered_vertices.size() * sizeof(glm::vec3), this->m_ordered_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &this->m_vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo_normals);

    glBufferData(GL_ARRAY_BUFFER, this->m_ordered_normals.size() * sizeof(glm::vec3), this->m_ordered_normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &this->m_vbo_texcoords);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo_texcoords);

    glBufferData(GL_ARRAY_BUFFER, this->m_ordered_texcoords.size() * sizeof(glm::vec2), this->m_ordered_texcoords.data(), GL_STATIC_DRAW);

    this->create_fill_vao();
}

void Model::create_fill_vao()
{
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    const GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    const GLuint VERTEX_ATTR_UV = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_UV);

    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo_vertices);
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
        nullptr
    );

    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo_normals);
    glVertexAttribPointer(
        VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
        nullptr
    );

    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo_texcoords);
    glVertexAttribPointer(
        VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
        nullptr
    );

    // Debinding VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // // Debinding VAO
    // glBindVertexArray(0);
}

void Model::draw_model(p6::Shader& Shader, const glm::mat4& ViewMatrix, const glm::mat4& ProjMatrix)
{
    glm::mat4 ModelMatrix = glm::mat4(1.0f);

    // ModelMatrix = glm::translate(ModelMatrix, this->m_position);

    // glm::vec3 forwardAxis, leftAxis, upAxis;
    // computeDirectionVectors(forwardAxis, leftAxis, upAxis, this->m_direction);
    // // construct matrix from axes
    // glm::mat4 rotationMatrix = glm::mat4(
    //     glm::vec4(leftAxis, 0.0f),
    //     glm::vec4(upAxis, 0.0f),
    //     glm::vec4(forwardAxis, 0.0f),
    //     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    // );
    // ModelMatrix = ModelMatrix * rotationMatrix;
    // ModelMatrix = glm::scale(ModelMatrix, glm::vec3(this->m_radius));

    glm::mat4 MVMatrix = ViewMatrix * ModelMatrix;

    glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    Shader.set("uMVMatrix", MVMatrix);
    Shader.set("uMVPMatrix", MVPMatrix);
    Shader.set("uNormalMatrix", NormalMatrix);

    // Binding VAO
    glBindVertexArray(this->m_vao);

    glDrawArrays(GL_TRIANGLES, 0, this->m_ordered_vertices.size());

    // Debinding VAO
    glBindVertexArray(0);
}