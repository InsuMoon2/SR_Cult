#ifndef Engine_Define_h__
#define Engine_Define_h__

// DirectX 9
#include <d3d9.h>
#include <d3dx9.h>

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <ctime>

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

// DInput
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

//DirectXTk (SimpleMath)
#include "../../Libs/DirectXTK/SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

//Fmod
#include "../../Libs/Fmod/fmod.hpp"
#pragma comment(lib, "../../Libs/Fmod/fmodL_vc.lib")

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Engine;

#endif // Engine_Define_h__
