
#include "3DRenderer.h"

#include <iostream>
#include "ShaderManager.h"
#include "Camera.h"
#include "utils.h"

static void STDCALL GLErrorCallback(
    GLenum source, GLenum type,
    GLuint id, GLenum severity,
    GLsizei length, const GLchar *msg,
    GLvoid *userdata )
{
    std::cout << msg << std::endl;
}

Renderer3D::Renderer3D(void *GLContext):
    m_glcontext(GLContext),
    m_shader_manager(nullptr),
    m_colour_shader(0),
    m_texture_shader(0)
{
    assert(GLContext);
    // glew needs to be initialized after 
    // the gl_createcontext call
    static RunOnce glew([]
    {
        glewExperimental = true;
        glewInit();
    });

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void Renderer3D::Init(const renderer3d_config& config)
{
    GLContext::SetDebugMessageCallback(&GLErrorCallback);
    GLContext::EnableDepthTest(GL_LESS);

    m_fullview.SetView(config.x, config.y, config.width, config.height);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    m_shader_manager = m_glcontext.CreateShaderManager();

    std::vector<uint> vertexShaders;
    std::vector<uint> fragmentShaders;

    m_shader_manager->CompileShaders(
        {"../src/shaders/simplevertex.vert", "../src/shaders/texturevertex.vert"},
        {"../src/shaders/simplefragment.frag", "../src/shaders/texturefragment.frag"},
    vertexShaders, fragmentShaders);

    m_colour_shader = m_shader_manager->CreateProgram();
    m_colour_shader.AttachShader(vertexShaders[0]);
    m_colour_shader.AttachShader(fragmentShaders[0]);
    bool linked = m_colour_shader.Link();
    assert(linked);

    /*m_texture_shader = m_shader_manager->CreateProgram();
    glAttachShader(m_texture_shader, vertexShaders[1]);
    glAttachShader(m_texture_shader, fragmentShaders[1]);
    glLinkProgram(m_texture_shader);
    {
        int res = GL_FALSE;
        glGetProgramiv(m_texture_shader, GL_LINK_STATUS, &res);
        bool linked = res == GL_TRUE;
        assert(linked);
    }
    m_tex_id = glGetUniformLocation(m_texture_shader, "myTextureSampler");*/
}

void Renderer3D::Shutdown()
{
}

void Renderer3D::RenderScene(const Viewport* viewport, const Camera* cam, const std::vector<Renderable>& scene)
{
    bool do_scissor = false;
    if (viewport)
    {
        do_scissor = (m_fullview != *viewport);
    }
    else
    {
        viewport = &m_fullview;
    }
    
    if (do_scissor)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    viewport->SetActive();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto aspect = viewport->GetAspectRatio();
    auto pv = glm::perspective(glm::quarter_pi<float>(), aspect, 0.1f, 1000.0f) * cam->GetView();

    m_colour_shader.Bind();
    for (auto obj : scene)
    {
        auto mvp = pv * obj.transform;
        m_colour_shader.SetUniform("MVP", &mvp[0][0]);

        GLuint buffer_id = obj.mesh.GetVertexBufferId();
        GLsizei num_verticies = static_cast<GLsizei>(obj.mesh.GetNumVerticies());

        glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, num_verticies);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    m_colour_shader.Unbind();
    if (do_scissor)
    {
        glDisable(GL_SCISSOR_TEST);
    }
}