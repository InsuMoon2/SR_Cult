#pragma once

#include "CScene.h"

class CStage : public CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(LAYERTYPE eLayerType);
	HRESULT			Ready_GameLogic_Layer(LAYERTYPE eLayerType);
	HRESULT			Ready_UI_Layer(LAYERTYPE eLayerType);

	HRESULT			Ready_Prototype();


public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();

};

