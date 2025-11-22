#pragma once
#include "CUI.h"

class CUIInventoryCell : public CUI
{
private:
    explicit CUIInventoryCell(DEVICE pGraphicDev);
    explicit CUIInventoryCell(const CUIInventoryCell &rhs);
    ~CUIInventoryCell() override;

public:
    void    OnBeginOverlap(CCollider* self, CCollider* other) override;
    void    OnEndOverlap(CCollider* self, CCollider* other) override;
    void    Render_Editor() override;
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;
    HRESULT Add_Component();

protected:
    void Free() override;
};


