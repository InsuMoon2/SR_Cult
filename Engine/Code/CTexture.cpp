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

    // 깊은 복사
    for (const auto& keyValue : rhs.m_AnimTextures)
    {
        const wstring& key = keyValue.first;
        const vector<IDirect3DBaseTexture9*>& textures = keyValue.second;

        vector<IDirect3DBaseTexture9*>& dstFrames = m_AnimTextures[key];
        dstFrames.reserve(textures.size());

        for (auto* tex : textures)
        {
            dstFrames.push_back(tex);
            if (tex)
                tex->AddRef();
        }
    }
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

        swprintf_s(fileName, MAX_PATH, filePath.c_str(), i + 1);

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

HRESULT CTexture::Add_Texture(const wstring& animKey, TEXTUREID texType, const wstring& filePathPattern, _uint count)
{
    vector<IDirect3DBaseTexture9*>& frames = m_AnimTextures[animKey];
    frames.reserve(count);

    for (_uint i = 0; i < count; ++i)
    {
        wchar_t fileName[MAX_PATH] = L"";

        // 파일이 1~N 이면 (i+1), 0~N-1 이면 (i)로 맞춰줘
        swprintf_s(fileName, MAX_PATH, filePathPattern.c_str(), i + 1);

        IDirect3DBaseTexture9* texture = nullptr;

        switch (texType)
        {
        case TEX_NORMAL:
            if (FAILED(D3DXCreateTextureFromFile(
                m_GraphicDev, fileName, (LPDIRECT3DTEXTURE9*)&texture)))
                return E_FAIL;
            break;

        case TEX_CUBE:
        {
            if (FAILED(D3DXCreateCubeTextureFromFile(
                m_GraphicDev, fileName, (LPDIRECT3DCUBETEXTURE9*)&texture)))
                return E_FAIL;
        }
        break;

        }

        frames.push_back(texture);
    }

    return S_OK;
}

void CTexture::Set_Texture(const wstring& animKey, _uint frameIndex)
{
    auto iter = m_AnimTextures.find(animKey);

    if (iter == m_AnimTextures.end())
        return;

    const vector<IDirect3DBaseTexture9*>& frames = iter->second;

    if (frames.empty() || frameIndex >= frames.size())
        return;

    m_GraphicDev->SetTexture(0, frames[frameIndex]);
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

    for_each(m_AnimTextures.begin(), m_AnimTextures.end(),
        [](auto& keyValue)
        {
            for (auto*& tex : keyValue.second)
            {
                Safe_Release(tex);
            }
        });
    m_AnimTextures.clear();
}
