#pragma once

#include "CScene.h"

class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(LAYERTYPE eLayerType);
	HRESULT			Ready_GameLogic_Layer(LAYERTYPE eLayerType) { return S_OK; }
	HRESULT			Ready_UI_Layer(LAYERTYPE eLayerType) { return S_OK; }

	HRESULT			Ready_Prototype();


public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();

};

