
#include "ModelLoader_FBX.h"

#include <glm/glm.hpp>
#include <vector>
#include <fbxsdk.h>

using namespace std;

bool InitializeFbxObjects(FbxManager **fbxManager_out, FbxScene **fbxScene_out)
{
    assert(fbxManager_out);
    assert(fbxScene_out);
    auto *fbxManager = FbxManager::Create();
    if (!fbxManager)
    {
        return false;
    }
    
    auto *fbxIoSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(fbxIoSettings);

    FbxString path = FbxGetApplicationDirectory();
    fbxManager->LoadPluginsDirectory(path.Buffer());

    auto *fbxScene = FbxScene::Create(fbxManager, "FBX Default Scene");
    if (!fbxScene)
    {
        return false;
    }

    *fbxManager_out = fbxManager;
    *fbxScene_out = fbxScene;
    return true;
}

void DestroyFbxObjects(FbxManager *fbxManager)
{
    fbxManager->Destroy();
}

bool LoadFbxScene(FbxManager *fbxManager, FbxScene *fbxScene, const char *filename)
{
    FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "FBX Default Importer");
    bool importStatus = fbxImporter->Initialize(filename, -1, fbxManager->GetIOSettings());
    if (!importStatus)
    {
        return false;
    }

    if (!fbxImporter->IsFBX())
    {
        return false;
    }

    importStatus = fbxImporter->Import(fbxScene);
    fbxImporter->Destroy();
    return importStatus;
}

vector<glm::vec3> ParseFBX(const char *filename)
{
    vector<glm::vec3> vertices;

    FbxManager *fbxManager = nullptr;
    FbxScene *fbxScene = nullptr;

    InitializeFbxObjects(&fbxManager, &fbxScene);
    LoadFbxScene(fbxManager, fbxScene, filename);

    auto *rootNode = fbxScene->GetRootNode();
    for (int i = 0; i < rootNode->GetChildCount(); ++i)
    {
        auto *childNode = rootNode->GetChild(i);
        auto *nodeAttribute = childNode->GetNodeAttribute();
        if (!nodeAttribute)
        {
            continue;
        }

        auto attributeType = nodeAttribute->GetAttributeType();
        if (attributeType != FbxNodeAttribute::eMesh)
        {
            continue;
        }

        auto *nodeMesh = static_cast<FbxMesh*>(nodeAttribute);
        FbxVector4 *nodeVertices = nodeMesh->GetControlPoints();
        for (int j = 0; j < nodeMesh->GetPolygonCount(); ++j)
        {
            int numVertices = nodeMesh->GetPolygonSize(j);
            assert(numVertices == 3);

            for (int k = 0; k < numVertices; ++k)
            {
                int controlPointIndex = nodeMesh->GetPolygonVertex(j, k);

                vertices.emplace_back(nodeVertices[controlPointIndex].mData[0],
                                       nodeVertices[controlPointIndex].mData[1],
                                       nodeVertices[controlPointIndex].mData[2]);
            }
        }
    }
    DestroyFbxObjects(fbxManager);
    return vertices;
}

Mesh LoadMeshFromFBX(const char *filename)
{
    Mesh mesh;
    mesh.vertices = ParseFBX(filename);
    return mesh;
}
