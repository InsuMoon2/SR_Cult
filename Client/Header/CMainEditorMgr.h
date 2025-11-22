#pragma once

#include "CBase.h"

BEGIN(Engine)
class CEditContext;
END

class CMainEditorMgr : public CBase
{
    DECLARE_SINGLETON(CMainEditorMgr)

private:
    explicit CMainEditorMgr();
    virtual ~CMainEditorMgr();

public:
    void Set_EditContext(CEditContext* ctx);

    // 매 프레임 ImGui로 메인 에디터 창 그리는 부분
    void Render(); 

private:
    void Render_Hierarchy();
    void Render_Inspector();

    virtual void Free() override;

private:
    CEditContext* m_Context;

};

