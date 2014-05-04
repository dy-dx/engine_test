
#pragma once

#include "../Renderer/3DRenderer/Camera.h"
#include "../Renderer/3DRenderer/Renderable.h"
#include "DataTable.h"

class Renderer3D;
class ResourceLoader;
class Viewport;

template<typename T> class ComponentTable;
struct PhysicalComponent;
struct GraphicalComponent;

class RenderProxy
{
public:
    RenderProxy(Renderer3D* renderer);

    void SetComponentTables(ComponentTable<PhysicalComponent>* physical,
                            ComponentTable<GraphicalComponent>* graphical);

    void RenderScene(const Viewport* viewport, uint camera);

    void Update();

private:
    void SetActiveCamera(uint camera);

    void CheckEntChanges();

    void LoadRenderable(Renderable *r, const PhysicalComponent* physical);

    void LoadRenderable(Renderable *r, const GraphicalComponent* graphical);

private:
    Renderer3D *m_renderer;
    ResourceLoader *m_resource_loader;
    ComponentTable<PhysicalComponent> *m_physical_components;
    ComponentTable<GraphicalComponent> *m_graphical_components;

    Camera m_camera;
    DataTable<Renderable> m_scene;
};

inline void RenderProxy::SetComponentTables(ComponentTable<PhysicalComponent>* physical,
                                            ComponentTable<GraphicalComponent>* graphical)
{
    m_physical_components = physical;
    m_graphical_components = graphical;
}
