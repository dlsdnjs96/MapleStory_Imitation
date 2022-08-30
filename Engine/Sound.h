#pragma once

namespace Sound
{
	class Sound final
	{
	public:
		void Play();
		void Stop();
		void Pause();
		void Reset();

	public:
		std::string Content = "";

	public:
		float volume = 0.0f;
		bool  loop   = false;
		bool  pause  = false;
		bool onlyonce = false;
	};
}