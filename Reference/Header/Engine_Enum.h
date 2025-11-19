#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
    // 윈도우 생성 시 사용할 화면 모드
    enum WINMODE { MODE_FULL, MODE_WIN };

    /// @brief 컴포넌트 업데이트에 따른 인덱스
    ///	@details
    ///	- ID_DYNAMIC : 다이나믹 컴포넌트. 매 프레임마다 갱신할 경우
    /// - ID_STATIC : 스태틱 컴포넌트. 갱신할 필요가 없을 경우
    enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

    // 게임 오브젝트의 방향 및 위치 벡터 인덱스
    enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

    // 회전 축 인덱스
    enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

    // 텍스처 자원의 타입 구분 인덱스
    enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

    // 렌더링 우선순위별 렌더링 그룹 인덱스
    enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

    // DirectInput 마우스 버튼 상태 인덱스
    enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

    // DirectInput 마우스 이동 축 인덱스
    enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

    enum class COLLIDERTYPE { RECT, CIRCLE, BOX, SPHERE };

    enum class ANIMSTATE
    {
        STOP,		    // 정지
        LOOP,			// 계속 반복
        ONCE,			// 한번만 재생
        REVERSE_LOOP,	// 거꾸로 계속 반복
        REVERSE_ONCE,	// 거꾸로 한번만 재생

        ANIM_END
    };

    enum class LAYERTYPE
    {
        ENVIRONMENT,
        GAMELOGIC,
        CAMERA,
        UI,

        LAYER_END
    };

    enum class OBJTYPE
    {
        PLAYER,
        MONSTER,
        HUMANMONSTER,
        CAMERA,
        /* 오브젝트 타입 종류 추가 */
        UI,
        BOSS2,

        OBJ_END
    };

    // 만들어지는 컴포넌트마다 부여하는 고유 타입
    enum class COMPONENTTYPE
    {
        TRI_COLOR,
        RC_COLOR,
        RC_TEX,
        TRANSFORM,
        STATE,

        TEXTURE,
        TEX_PLAYER,
        TEX_MAINBG,
        TEX_MONSTER,
        TEX_HUMANMONSTER,
        // TODO 석호: 현재 텍스처 프로토타입마다 다른 enum 값을 줘야 하기에, 텍스처마다 새로 추가되어야 함

        TEX_UI_HEART,

        ANIMATOR,
        CAMERA,

        RECT_COLL,
        CIRCLE_COLL,
        BOX_COLL,
        SPHERE_COLL,

        COMPONENT_END
    };

    enum class PLAYERSTATE
    {
        IDLE,
        RUN,
        ATTACK,
        HIT,
        DEAD,

        PLAYERSTATE_END
    };

    enum class PLAYERDIR
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,

        PLAYERDIR_END
    };
}
#endif // Engine_Enum_h__
