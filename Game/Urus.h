#pragma once
#include "Xml.h"


namespace Urus
{

	struct AniFrames
	{

	};

	enum class UrusState
	{
		MOTION,
		IDLE,
		WALK
	};

	struct Frame
	{
		vector<2> transform;
		float rotation;
	};

	struct FrameT
	{
		vector<2> transform;
		float rotation, time;
	};

	struct Animations
	{
		float totalDuration, framePerSec;
		std::map<std::string, Frame*> Frames;

		Animations()
		{
		};
		~Animations()
		{
			for (auto iter = Frames.begin(); iter != Frames.end(); iter++)
				delete iter->second;
		};
	};

	class UrusPart
	{
	protected:
		std::string				name;
		UrusPart*				parent;
		std::vector<UrusPart*>	children;
		bool isActivating = true;

	public:
		vector<2>				position;
		vector<2>				transform;
		float					rotation;

		~UrusPart();

	public:
		class UrusRoot* root;
		void AddPart(std::string partName);
		const char* GetName();
		bool IsActivating() { return isActivating; }
		void SetAllActivating(bool act);

		void Update();
		void Render();
		void Hierarchy();

		void LoadPart(Xml::XMLElement* This);
		void SavePart(Xml::XMLElement* This, Xml::XMLDocument* doc);
		UrusPart* Find(std::string name);
		void AddMotion();
	};

	class UrusRoot : public UrusPart
	{
		friend class UrusPart;
	private:
		std::unordered_map<std::string, UrusPart*> partList;
		std::unordered_map<std::string, Animations*> animations;
		UrusState state = UrusState::MOTION;
		float changedState = 0.f;


	public:
		std::map<std::string, std::vector<Urus::FrameT>> urusMotion;
		float motionT = 0.f;

	protected:
		void AddList(std::string name, UrusPart* ptr);
	public:
		UrusRoot();
		~UrusRoot();

		void Update();
		void Render();
		void Hierarchy();

		void Motion();
		void Idle();
		void Walk();

		void SetAllActivating(bool act);
		void LoadRoot(std::string file);
		void SaveRoot(std::string file);
		UrusPart* Find(std::string name);

		Animations* LoadAni(std::string name);
		void LoadAnimations();
		void AddMotion();
		void SaveMotions();
	};
};

