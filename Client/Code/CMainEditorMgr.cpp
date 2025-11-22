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
#include "CBoxCollider.h"
#include "CInventory.h"
#include "CWeaponEquip.h"
#include "CClient_CreateHelper.h"
#include "CDInputMgr.h"
#include "CLayer.h"

IMPLEMENT_SINGLETON(CMainEditorMgr)

CMainEditorMgr::CMainEditorMgr()
{ }

CMainEditorMgr::~CMainEditorMgr()
{
    Free();
}

void CMainEditorMgr::Set_EditContext(CEditContext* ctx)
{
    Safe_Release(m_Context);

    m_Context = ctx;
}

void CMainEditorMgr::Render_MainEditor()
{
    if (!m_Context)
        return;

    CScene* scene = CManagement::GetInstance()->Get_Scene();
    if (scene)
        m_Context->Set_ActiveScene(scene);

    else
        MSG_BOX("Scene Is Where ? ");

    // 프리뷰 오브젝트 렌더
    if (m_PlacingMapObject && m_PreviewMapObject)
    {
        m_PreviewMapObject->Render_GameObject();
    }

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

    // MapObject
    Render_MapObjectWindow();
}

void CMainEditorMgr::Ready_MainEditor(DEVICE graphicDev, CEditContext* ctx)
{
    m_GraphicDev = graphicDev;

    Set_EditContext(ctx);
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

    Render_DynamicComponents(obj);
    Render_StaticComponents(obj);

    ImGui::Separator();
    obj->Render_Editor();

    Render_SelectComponent();
}

void CMainEditorMgr::Render_DynamicComponents(CGameObject* obj)
{
    // Dynamic Component
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
}

void CMainEditorMgr::Render_StaticComponents(CGameObject* obj)
{
    // Static Component
    auto inven = obj->Get_Component(ID_STATIC, COMPONENTTYPE::INVENTORY);
    if (inven)
    {
        wstring wTypeName = inven->Get_Name();
        string typeName(wTypeName.begin(), wTypeName.end());

        bool isSelected = (inven == m_Context->Get_SelectedComponent());
        if (ImGui::Selectable(typeName.c_str(), isSelected))
        {
            m_Context->Select_Component(inven);
        }
    }

    auto weapon = obj->Get_Component(ID_STATIC, COMPONENTTYPE::WEAPON_EQUIP);
    if (weapon)
    {
        wstring wTypeName = weapon->Get_Name();
        string typeName(wTypeName.begin(), wTypeName.end());

        bool isSelected = (weapon == m_Context->Get_SelectedComponent());
        if (ImGui::Selectable(typeName.c_str(), isSelected))
        {
            m_Context->Select_Component(weapon);
        }
    }
}

void CMainEditorMgr::Render_SelectComponent()
{
    // 선택된 컴포넌트의 인스펙터
    CComponent* selectComponent = m_Context->Get_SelectedComponent();
    if (selectComponent)
    {
        ImGui::Separator();

        string componentName = WStringToUtf8(selectComponent->Get_Name());

        switch (selectComponent->Get_Type())
        {
            // Dynamic
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

        // Static
        case COMPONENTTYPE::INVENTORY:
        {
            auto inven = dynamic_cast<CInventory*>(selectComponent);
            Inventory_Inspector(inven);
            break;
        }

        case COMPONENTTYPE::WEAPON_EQUIP:
        {
            auto weapon = dynamic_cast<CWeaponEquip*>(selectComponent);
            Weapon_Inspector(weapon);
            break;
        }

        // 다른 컴포넌트 타입 ImGui에 보여줄거 있으면 추가하기

        }
    }
}

void CMainEditorMgr::Update_MapObjectPlacement(const _float& timeDelta)
{
    if (!m_PlacingMapObject || m_PreviewMapObject == nullptr)
        return;

    // 마우스 -> 월드좌표
    _vec3 worldPos;
    if (Get_MouseWorldPosOnTerrain(worldPos))
    {
        m_PreviewMapObject->Set_WorldPos(worldPos);
    }

    m_PreviewMapObject->Update_GameObject(timeDelta);
    m_PreviewMapObject->LateUpdate_GameObject(timeDelta);

    // 좌클릭 하면 배치하기
    auto input = CDInputMgr::GetInstance();

    if (input->KeyDown(VK_LBUTTON))
    {
        CScene* scene = CManagement::GetInstance()->Get_Scene();
        if (scene)
        {
            CLayer* layer = scene->Get_Layer(LAYERTYPE::ENVIRONMENT);
            if (layer)
            {
                CMapObjectBase* newObj = CreateMapObjectType(m_GraphicDev, m_SelectedMapObjType);
                if (newObj)
                {
                    newObj->Set_WorldPos(worldPos);
                    layer->Add_GameObject(OBJTYPE::PLACEMENT, newObj);

                }
            }
        }
    }

    // 우클릭 : 배치 종료
    if (input->KeyDown(VK_LBUTTON) || input->KeyDown(VK_ESCAPE))
    {
        m_PlacingMapObject = false;
        Safe_Release(m_PreviewMapObject);
    }
}

void CMainEditorMgr::Render_MapObjectWindow()
{
    ImGui::Begin("Map Objects");

    // Grass 풀때기만 일단
    if (ImGui::Selectable("Grass", m_SelectedMapObjType == MAPOBJTYPE::GRASS))
    {
        m_SelectedMapObjType = MAPOBJTYPE::GRASS;

        Safe_Release(m_PreviewMapObject);

        // 새 프리뷰 설정. 투명하게도 할지?
        m_PreviewMapObject = CreateMapObjectType(m_GraphicDev, m_SelectedMapObjType);
        m_PlacingMapObject = (m_PreviewMapObject != nullptr); // m_PreviewMapObject이 있으면 true
    }

    // 추가 예정) Rock, Tree 등등

    // 배치 취소
    if (m_PlacingMapObject)
    {
        if (ImGui::Button("Cancel Place"))
        {
            m_PlacingMapObject = false;
            Safe_Release(m_PreviewMapObject);
        }
    }

    ImGui::End();

}

bool CMainEditorMgr::Get_MouseWorldPosOnTerrain(_vec3& outPos)
{
    // TODO : 마우스 월드좌표로 변환하기. 일단은 상수값으로 실험

    outPos = _vec3(0.1f, 3.1f, 3.1f);

    return true;
}

void CMainEditorMgr::Free()
{
    Safe_Release(m_Context);
}
