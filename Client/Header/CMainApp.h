#pragma once

#include "CBase.h"
#include "CGraphicDev.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"

namespace Engine
{
	class CManagement;
}

class CMainApp : public CBase
{
private:	// 持失切, 社瑚切
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_MainApp();
	int		Update_MainApp(const float& fTimeDelta);
	void	LateUpdate_MainApp(const float& fTimeDelta);
	void	Render_MainApp();

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CGraphicDev* m_DeviceClass;
	Engine::CManagement* m_ManagementClass;

	LPDIRECT3DDEVICE9	 m_GraphicDev;

public:	
	static CMainApp*	Create();

private:
	virtual void		Free()override;
};

