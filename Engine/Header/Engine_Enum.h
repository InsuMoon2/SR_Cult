#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

    enum class COLLIDERTYPE { Rect, Circle, Box, Sphere };

    enum class ANIMSTATE
    {
        Stop = 0,		// Stop
        Loop,			// 계속 반복
        Once,			// 한번만 재생
        Reverse_Loop,	// 거꾸로 계속 반복
        Reverse_Once,	// 거꾸로 한번만 재생

        END
    };

    enum class LAYERTYPE { Enviroment, GameLogic, UI, END };
    enum class OBJTYPE { Player, Monster, /* 오브젝트 타입 종류 추가 */ END };

	enum class COMPONENTTYPE
	{
		TriCol,
		RcCol,
		RcTex,
		Transform,
		Texture,
		Animator,
		Camera,

        RectCol,
        CircleCol,
        BoxCol,
        SphereCol,

        END
	};

}
#endif // Engine_Enum_h__
