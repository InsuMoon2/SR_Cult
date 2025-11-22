#pragma once

#include "CBase.h"
#include "CMapObjectBase.h"
#include "Engine_Enum.h"

class CMapObjectBase;
class CMapObjectGrass;

BEGIN(Engine)
class CEditContext;
class CGameObject;
END

class CMainEditorMgr : public CBase
{
    DECLARE_SINGLETON(CMainEditorMgr)

private:
    explicit CMainEditorMgr();
    virtual ~CMainEditorMgr();

public:
    void Ready_MainEditor(DEVICE graphicDev, CEditContext* ctx);

    // 매 프레임 ImGui로 메인 에디터 창 그리는 부분
    void Render_MainEditor();

    // MapObject
    void Update_MapObjectPlacement(const _float& timeDelta);
    void Render_MapObjectWindow();
    bool Get_MouseWorldPosOnTerrain(_vec3& outPos);

private:
    void Set_EditContext(CEditContext* ctx);

    void Render_Hierarchy();
    void Render_Inspector();

    void Render_DynamicComponents(CGameObject* obj);
    void Render_StaticComponents(CGameObject* obj);

    void Render_SelectComponent();


protected:
    virtual void Free() override;

private:
    DEVICE m_GraphicDev = nullptr;

    CEditContext* m_Context = nullptr;

    // MapObject 배치
    MAPOBJTYPE      m_SelectedMapObjType = MAPOBJTYPE::MAPOBJEND;
    CMapObjectBase* m_PreviewMapObject = nullptr;
    bool            m_PlacingMapObject = false;

};

