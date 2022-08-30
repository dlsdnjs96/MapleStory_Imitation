#pragma once
//
//template<class T>using uPtr = std::unique_ptr<T>;
//template<class T>using sPtr = std::shared_ptr<T>;
//template<class T>using wPtr = std::weak_ptr<T>;



#include "Engine/Collision.h"
#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/input.h"
#include "Engine/matrix.h"
#include "Engine/Pipeline.h"
#include "Engine/Rendering.h"
#include "Engine/Sound.h"
#include "Engine/Time.h"
#include "Engine/vector.h"


#include <vector>
#include <map>
#include <fstream>



//Imgui
#include "../.lib/ImGui/imgui.h"
#include "../.lib/ImGui/imgui_internal.h"
#include "../.lib/ImGui/imgui_impl_dx11.h"
#include "../.lib/ImGui/imgui_impl_win32.h"
#pragma comment(lib, "../.lib/ImGui/example_win32_directx11.lib")

//Dialog
#include "../.lib/ImGui/dirent.h"
#include "../.lib/ImGui/ImGuiFileDialog.h"
#include "../.lib/ImGui/ImGuiFileDialogConfig.h"