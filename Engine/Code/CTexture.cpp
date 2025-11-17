#include "CTexture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 graphicDev)
    : CComponent(graphicDev)
{ }

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs)
{
    size_t size = rhs.m_Textures.size();

    m_Textures.reserve(size);

    m_Textures = rhs.m_Textures;

    for (_uint i = 0; i < size; ++i)
    {
        m_Textures[i]->AddRef();
    }

    m_KeyIndexMap = rhs.m_KeyIndexMap;
}

CTexture::~CTexture()
{ }

HRESULT CTexture::Ready_Texture(TEXTUREID texType, const wstring& filePath, const _uint& count)
{
    m_Textures.reserve(count);

    IDirect3DBaseTexture9* texture = nullptr;

    for (_uint i = 0; i < count; ++i)
    {
        wchar_t fileName[MAX_PATH] = L"";

        swprintf_s(fileName, MAX_PATH, filePath.c_str(), i);

        switch (texType)
        {
        case TEX_NORMAL:
        {
            if (FAILED(D3DXCreateTextureFromFile(
                m_GraphicDev, fileName, (LPDIRECT3DTEXTURE9*)&texture)))
                return E_FAIL;
        }
        break;
        case TEX_CUBE:
        {
            if (FAILED(D3DXCreateCubeTextureFromFile(
                m_GraphicDev, fileName, (LPDIRECT3DCUBETEXTURE9*)&texture)))
                return E_FAIL;
        }
        break;
        default:
            return E_FAIL;
        }

        m_Textures.push_back(texture);
    }

    return S_OK;
}

void CTexture::Set_Texture(const _uint& index)
{
    if (m_Textures.empty() || m_Textures.size() <= index)
        return;

    m_GraphicDev->SetTexture(0, m_Textures[index]);
}

HRESULT CTexture::Add_Texture(TEXTUREID texType, const wstring& key, const wstring& filePath)
{
    IDirect3DBaseTexture9* texture = nullptr;

    switch (texType)
    {
    case TEX_NORMAL:
        if (FAILED(D3DXCreateTextureFromFile(
            m_GraphicDev, filePath.c_str(), (LPDIRECT3DTEXTURE9*)&texture)))
            return E_FAIL;
        break;

    case TEX_CUBE:
        if (FAILED(D3DXCreateCubeTextureFromFile(
            m_GraphicDev, filePath.c_str(), (LPDIRECT3DCUBETEXTURE9*)&texture)))
            return E_FAIL;
        break;
    }

    m_Textures.push_back(texture);

    _uint index = static_cast<_uint>(m_Textures.size() - 1);
    m_KeyIndexMap[key] = index;
}

void CTexture::Set_Texture(const wstring& key)
{
    auto iter = m_KeyIndexMap.find(key);

    if (iter == m_KeyIndexMap.end())
        return;

    Set_Texture(iter->second);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 graphicDev,
                           TEXTUREID         texType,
                           const wstring&    filePath,
                           const _uint&      count)
{
    auto texture = new CTexture(graphicDev);

    if (FAILED(texture->Ready_Texture(texType, filePath, count)))
    {
        Safe_Release(texture);
        MSG_BOX("Texture Create Failed");
        return nullptr;
    }

    return texture;
}

CComponent* CTexture::Clone()
{
    return new CTexture(*this);
}

void CTexture::Free()
{
    for_each(m_Textures.begin(), m_Textures.end(), CDeleteObj());
    m_Textures.clear();
}
