
#pragma once

#include "GLContext.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "Renderable.h"
#include <vector>

class Camera;
class ShaderManager;
class ResourceLoader;

struct renderer3d_config
{
    uint x = 0;
    uint y = 0;
    uint width = 0;
    uint height = 0;
};

class Renderer3D
{
public:
    Renderer3D(void *GLContext);

    void Init(const renderer3d_config& config);

    void Shutdown();

    void RenderScene(const Viewport* viewport, const Camera* cam, const std::vector<Renderable>& scene);

    ResourceLoader* GetResourceLoader() const;

private:
    GLContext m_glcontext;
    uint m_vao;

    ResourceLoader *m_resourceLoader;

    ShaderManager *m_shader_manager;
    ShaderProgram m_colour_shader;
    ShaderProgram m_texture_shader;

    std::vector<const Renderable*> m_colour_shader_cache;
    std::vector<const Renderable*> m_texture_shader_cache;

    Viewport m_fullview;
};

inline ResourceLoader* Renderer3D::GetResourceLoader() const
{
    return m_resourceLoader;
}
