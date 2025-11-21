#include "pch.h"
#include "CMainEditorMgr.h"
#include "CComponent.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CEditContext.h"
#include "CEnumHelper.h"
#include "CManagement.h"
#include "CScene.h"
#include "CState.h"
#include "CCombatStat.h"
#include "CComInspectors.h"
#include "CTerrainRenderer.h"
#include "CTransform.h"
#include "CBOxCollider.h"

IMPLEMENT_SINGLETON(CMainEditorMgr)

CMainEditorMgr::CMainEditorMgr()
    : m_Context(nullptr)
{
}

CMainEditorMgr::~CMainEditorMgr()
{
    Free();
}

void CMainEditorMgr::Set_EditContext(CEditContext* ctx)
{
    Safe_Release(m_Context);

    m_Context = ctx;
}

void CMainEditorMgr::Render()
{
    if (!m_Context)
        return;

    CScene* scene = CManagement::GetInstance()->Get_Scene();
    if (scene)
        m_Context->Set_ActiveScene(scene);

    else
        MSG_BOX("Scene Is Where ? ");

    ImGui::Begin("MainEditor");

    // 왼쪽 Hierarchy (계층 구조)
    ImGui::BeginChild(u8"[ Hierarchy ]", ImVec2(170, 0), true);
    Render_Hierarchy();
    ImGui::EndChild();

    ImGui::SameLine();

    // 오른쪽 Inspector (정보 및 설정)
    ImGui::BeginChild(u8"[ Inspector ]", ImVec2(0, 0), true);
    Render_Inspector();
    ImGui::EndChild();

    ImGui::End();
}

void CMainEditorMgr::Render_Hierarchy()
{
    CScene* scene = m_Context->Get_ActiveScene();
    if (scene == nullptr)
    {
        ImGui::Text("No Active Scene");
        return;
    }

    // TODO 인수 : 일단은 하나의 Scene만 있다고 가정하고 구현
    if (ImGui::TreeNode("Scene"))
    {
        const auto& layers = scene->Get_Layers();

        for (const auto& layerPair : layers)
        {
            LAYERTYPE layerType = layerPair.first;
            CLayer* layer = layerPair.second;

            if (layer == nullptr) continue;

            string layerName = ToString(layerType);

            if (ImGui::TreeNode(layerName.c_str()))
            {
                const auto& objMap = layer->Get_Objects();

                for (const auto& objPair : objMap)
                {
                    OBJTYPE objType = objPair.first;
                    const vector<CGameObject*>& objList = objPair.second;

                    for (CGameObject* obj : objList)
                    {
                        if (!obj) continue;

                        bool isSelected = (obj == m_Context->Get_SelectedObject());

                        const wstring& wName = obj->Get_Name();
                        string name(wName.begin(), wName.end()); // wstring -> string

                        if (ImGui::Selectable(name.c_str(), isSelected))
                        {
                            m_Context->Select_Object(obj);
                        }
                    }
                }

                ImGui::TreePop(); // Layer
            }
        }

        ImGui::TreePop(); // Scene
    }
}

void CMainEditorMgr::Render_Inspector()
{
    CGameObject* obj = m_Context->Get_SelectedObject();

    if (obj == nullptr)
    {
        ImGui::Text("Not Object Selected");
        return;
    }

    // 기본값 Transform으로 시작
    if (m_Context->Get_SelectedComponent() == nullptr)
    {
        CComponent* transformCom = obj->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM);

        if (transformCom)
            m_Context->Select_Component(transformCom);
    }

    // Object 이름 세팅
    {
        wstring wName = obj->Get_Name();
        string name(wName.begin(), wName.end());
        ImGui::Text("Object : %s", name.c_str());
    }

    ImGui::Separator();
    ImGui::Text("[ Components ]");

    // Component 전체 맵 순회
    const auto& dynamicComponents = obj->Get_DynamicComponents();

    for (const auto& typePair : dynamicComponents)
    {
        COMPONENTTYPE   type = typePair.first;
        CComponent* com = typePair.second;

        if (!com) continue;

        wstring wTypeName = com->Get_Name();
        string typeName(wTypeName.begin(), wTypeName.end());

        bool isSelected = (com == m_Context->Get_SelectedComponent());

        if (ImGui::Selectable(typeName.c_str(), isSelected))
        {
            m_Context->Select_Component(com);
        }
    }

    ImGui::Separator();

    obj->Render_Editor();

    // 선택된 컴포넌트의 인스펙터
    CComponent* selectComponent = m_Context->Get_SelectedComponent();
    if (selectComponent)
    {
        ImGui::Separator();

        string componentName = WStringToUtf8(selectComponent->Get_Name());

        switch (selectComponent->Get_Type())
        {
        case COMPONENTTYPE::TRANSFORM:
        {
            auto transform = dynamic_cast<CTransform*>(selectComponent);
            Transform_Inspector(transform);
            break;
        }
        case COMPONENTTYPE::STATE:
        {
            auto state = dynamic_cast<CState*>(selectComponent);
            State_Inspector(state);
            break;
        }
        case COMPONENTTYPE::COMBATSTAT:
        {
            auto stat = dynamic_cast<CCombatStat*>(selectComponent);
            CombatStat_Inspector(stat);
            break;
        }

        case COMPONENTTYPE::CAMERA:
        {
            auto camera = dynamic_cast<CCameraCom*>(selectComponent);
            Camera_Inspector(camera);
            break;
        }

        case COMPONENTTYPE::TERRAIN_RENDER:
        {
            auto terrain = dynamic_cast<CTerrainRenderer*>(selectComponent);
            Terrain_Inspector(terrain);
            break;
        }

        case COMPONENTTYPE::BOX_COLL:
        {
            auto collider = dynamic_cast<CBoxCollider*>(selectComponent);
            BoxColl_Inspector(collider);
            break;
        }

        // 다른 컴포넌트 타입 ImGui에 보여줄거 있으면 추가하기


        }

        // TODO 인수 : Static Component들도 보이게 세팅해야할지?
        // Update 하지 않을 컴포넌트들이라 실시간으로 툴에서 값 변경이 필요한가?
        //const auto& staticComponents = obj->Get_StaticComponents();

    }
}

void CMainEditorMgr::Free()
{
    Safe_Release(m_Context);
}
