#include "CSpriteAnimation.h"

CSpriteAnimation::CSpriteAnimation()
    : m_MaxFrameX(1), m_MaxFrameY(1),
    m_FrameX(0), m_FrameY(0),
    m_State(ANIMSTATE::Stop),
    m_XAxis(true),
    m_Interval(0.f),
    m_AccTime(0.f)
{

}

CSpriteAnimation::CSpriteAnimation(const CSpriteAnimation& rhs)
    : CBase(rhs)
{
    m_MaxFrameX = rhs.m_MaxFrameX;
    m_MaxFrameY = rhs.m_MaxFrameY;
    m_FrameX    = rhs.m_FrameX;
    m_FrameY    = rhs.m_FrameY;
    m_State     = rhs.m_State;
    m_XAxis     = rhs.m_XAxis;
    m_Interval  = rhs.m_Interval;
    m_AccTime   = 0.f; 
}

CSpriteAnimation::~CSpriteAnimation()
{
	Free();
}

void CSpriteAnimation::Init(_uint MaxX, _uint MaxY,
    _int StartX, _int StartY, _float Interval, bool xAxis,ANIMSTATE State)
{
    m_MaxFrameX = (MaxX == 0) ? 1 : MaxX;
    m_MaxFrameY = (MaxY == 0) ? 1 : MaxY;

    m_FrameX = StartX;
    m_FrameY = StartY;

    m_Interval = Interval;
    m_XAxis = xAxis;
    m_State = State;

    m_AccTime = 0.f;
}

void CSpriteAnimation::Reset()
{
    m_AccTime = 0.f;

    // 
}

void CSpriteAnimation::Update(const _float& TimeDelta)
{
    if (m_State == ANIMSTATE::Stop) // Stop이면 스프라이트 재생 X
        return;

    m_AccTime += TimeDelta;

    // 누적 시간이 Interval을 넘기면, 스프라이트의 다음 프레임으로 이동
    while (m_AccTime >= m_Interval)
    {
        m_AccTime -= m_Interval;

        if (m_XAxis) // 가로 재생
        {
            if (m_State == ANIMSTATE::Loop) // 계속 반복
            {
                m_FrameX++;

                if (m_FrameX >= m_MaxFrameX)
                    m_FrameX = 0;

            }

            else if (m_State == ANIMSTATE::Once) // 한번만 재생 후 Stop
            {
                m_FrameX++;

                if (m_FrameX >= m_MaxFrameX)
                {
                    m_FrameX = m_MaxFrameX - 1;
                    m_State = ANIMSTATE::Stop;
                }
            }

            else if (m_State == ANIMSTATE::Reverse_Loop)    // 반대로 반복재생
            {
                m_FrameX--;

                if (m_FrameX < 0)
                    m_FrameX = m_MaxFrameX - 1;
            }

            else if (m_State == ANIMSTATE::Reverse_Once)    // 반대로 한번만 재생
            {
                m_FrameX--;

                if (m_FrameX < 0)
                {
                    m_FrameX = 0;
                    m_State = ANIMSTATE::Stop;
                }
            }
        }

        else // 세로 재생
        {
            if (m_State == ANIMSTATE::Loop)
            {
                m_FrameY++;

                if (m_FrameY >= m_MaxFrameY)
                    m_FrameY = 0;
            }
            else if (m_State == ANIMSTATE::Once)
            {
                m_FrameY++;

                if (m_FrameY >= m_MaxFrameY)
                {
                    m_FrameY = m_MaxFrameY - 1;
                    m_State = ANIMSTATE::Stop;
                }
            }
            else if (m_State == ANIMSTATE::Reverse_Loop)
            {
                m_FrameY--;

                if (m_FrameY < 0)
                    m_FrameY = m_MaxFrameY - 1;
            }
            else if (m_State == ANIMSTATE::Reverse_Once)
            {
                m_FrameY--;

                if (m_FrameY < 0)
                {
                    m_FrameY = 0;
                    m_State = ANIMSTATE::Stop;
                }
            }
        }
    }

}

void CSpriteAnimation::Get_UV(_float& u0, _float& v0, _float& u1, _float& v1) const
{
    if (m_MaxFrameX > 1)
    {
        u0 = (m_FrameX) / (float)m_MaxFrameX;
        u1 = (m_FrameX + 1.f) / (float)m_MaxFrameX;
    }

    else
    {
        u0 = 0.f;
        u1 = 1.f;
    }

    if (m_MaxFrameY > 1)
    {
        v0 = (m_FrameY) / (float)m_MaxFrameY;
        v1 = (m_FrameY + 1.f) / (float)m_MaxFrameY;
    }
    else
    {
        v0 = 0.f;
        v1 = 1.f;
    }
}

CSpriteAnimation* CSpriteAnimation::Create()
{
    CSpriteAnimation* sprite = new CSpriteAnimation();

    return sprite;
}

void CSpriteAnimation::Free()
{

}

