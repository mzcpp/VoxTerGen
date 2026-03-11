#ifndef RENDER_MESH_HPP
#define RENDER_MESH_HPP

#include <mesh/Mesh.hpp>

#include <glad/glad/glad.h>

class RenderMesh
{
private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

public:
    RenderMesh();

    RenderMesh(const RenderMesh&) = delete;
	RenderMesh& operator=(const RenderMesh&) = delete;

    ~RenderMesh();    
};

#endif // RENDER_MESH_HPP

