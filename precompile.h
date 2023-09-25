#ifndef PRECOMPILE_H_
#define PRECOMPILE_H_

#include <Windows.h>
#include <XInput.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	//ビルド時の警告対処
#include "dinput.h"
#include "xaudio2.h"
#define IMGUI_DEFINE_MATH_OPERATORS

//ImGUI
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_win32.h"
#include "ImGUI\imgui_impl_dx9.h"
#include <d3d9.h>
#include <tchar.h>
#include <vector>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

#endif // !PRECOMPILE_H_
