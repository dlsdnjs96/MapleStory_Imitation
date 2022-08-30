#pragma once
#include "stdafx.h"


class Gui :public Singleton<Gui>
{
	friend Singleton;
public:
	static LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void* target;
	bool active = true;

private:
	UINT fileID;
public:
	Gui();
	~Gui();
	void ResizeScreen();

	void Update();
	void Render();
	bool FileImGui(std::string button, std::string name, std::string fiter, std::string minPath);
};