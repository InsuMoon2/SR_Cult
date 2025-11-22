#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CManagement.h"
class CSceneMgr :   public CBase
{

    DECLARE_SINGLETON(CSceneMgr)

private:
    explicit CSceneMgr();
    virtual ~CSceneMgr();

public:
    virtual HRESULT Ready_SceneMgr(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual void    Render_SceneMgr();
    virtual void    Change_SceneMgr(SCENETYPE typeInput);

private:
    LPDIRECT3DDEVICE9 m_pGraphicDev;

private:
    virtual void    Free();


};

