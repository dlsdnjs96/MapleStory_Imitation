#pragma once



template <typename T>
class Singleton
{
	//static
private:
	static T* instance;
public:
	static T* GetInstance()
	{
		if (!instance)
			instance = new T;

		return instance;
	};
	//member
public:
	virtual ~Singleton() {};
	void DeleteSingleton()
	{
		delete instance;
		instance = nullptr;
	};
};
template <typename T>
T* Singleton<T>::instance = nullptr;

class Gui :public Singleton<Gui>
{
	friend Singleton;
public:
	static LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	class GameObject* target;
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