
#include <string>
#include <Windows.h>

#include "Gui.h"
#include "Engine.h"
#include "Pipeline.h"


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


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Gui::MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
}
Gui::Gui()
{
	fileID = 0;

	target = nullptr;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsResizeFromEdges = true;
	io.FontAllowUserScaling = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	io.ConfigViewportsNoDefaultParent = true;
	io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(*Engine::GetHwnd());
	ImGui_ImplDX11_Init(Rendering::Pipeline::DirectX::GetD3D(), Rendering::Pipeline::DirectX::GetDC());


	ImGui::GetIO().ConfigWindowsResizeFromEdges = true;
	ImGui::StyleColorsDark();
	//ImGuiStyle& style = ImGui::GetStyle();
}


Gui::~Gui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Gui::ResizeScreen()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}


void Gui::Update()
{
	fileID = 0;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();

}

void Gui::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

bool Gui::FileImGui(std::string button, std::string name, std::string fiter, std::string minPath)
{
	std::string strId = std::to_string(fileID);
	fileID++;

	// open Dialog Simple
	if (ImGui::Button(button.c_str()))
		ImGuiFileDialog::Instance()->OpenDialog(strId, name.c_str(), fiter.c_str(), minPath, ".");

	// display
	if (ImGuiFileDialog::Instance()->Display(strId))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			ImGuiFileDialog::Instance()->Close();
			return true;
			// action
		}
		// close
		ImGuiFileDialog::Instance()->Close();
	}
	return false;
}

