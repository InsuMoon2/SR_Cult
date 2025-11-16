#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CSpriteAnimation : public CBase
{
private:
    explicit CSpriteAnimation();
    explicit CSpriteAnimation(const CSpriteAnimation& rhs);
    ~CSpriteAnimation() override;

public:
    void Init(_uint     maxX,
              _uint     maxY,
              _int      startX,
              _int      startY,
              _float    interval,
              _bool     xAxis,
              ANIMSTATE state);

    void Reset();
    void Update(const _float& timeDelta);

    _int Get_FrameX() const { return m_FrameX; }
    _int Get_FrameY() const { return m_FrameY; }
    _int Get_MaxFrameX() const { return m_MaxFrameX; }
    _int Get_MaxFrameY() const { return m_MaxFrameY; }

    // 현재 UV 사각형
    void Get_UV(_float& u0, _float& v0, _float& u1, _float& v1) const;

    _bool IsFinished() const { return m_State == ANIMSTATE::STOP; }

    static CSpriteAnimation* Create();

private:
    void Free() override;

private:
    // 스프라이트 시트 전체 개수 (가로 X 세로)
    _int m_MaxFrameX;
    _int m_MaxFrameY;

    // 현재 프레임 좌표 (0 ~ max-1 까지가 범위임)
    _int m_FrameX;
    _int m_FrameY;

    ANIMSTATE m_State;
    _bool     m_XAxis;		    // true면 X축으로 재생, false면 y축으로 재생
    _float    m_Interval;		// 다음 프레임까지 간격
    _float    m_AccTime;		// 누적 시간
};

END;
