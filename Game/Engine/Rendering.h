#pragma once
#include "vector.h"
#include <Windows.h>
#include <string>

namespace Rendering
{
	class Camera final
	{
	public:
		void Set() const;

		vector<2>	Length	 = { 1280, 720 };
		float		Angle	 = { 0 };
		vector<2>	Location = { 0, 0 };

	};

	namespace Text
	{
		class Component final
		{
		public:
			void Draw();

			std::string str = "";

			struct
			{
				char const* Name          = nullptr;
				unsigned    Size          = 0;
				bool        Bold          = false;
				bool        Italic        = false;
				bool        Underlined    = false;
				bool        StrikeThrough = false;
			}Font;

			struct
			{
				unsigned char Red   = 0;
				unsigned char Green = 0;
				unsigned char Blue  = 0;
			}Color;

			vector<2> Length = { 0.0f, 0.0f };
			vector<2> Location = { 0.0f, 0.0f };
		};
	}

	namespace Texture
	{
		void Create(struct Handle*& handle, SIZE const size, BYTE const* const data);
	};

	namespace Image
	{
		class Component final
		{
		public:
			//char const* Content = nullptr;
			std::string Content;

		public:
			vector<2> Length = { 0, 0 };
			float Angle = { 0 };
			vector<2> Location = { 0, 0 };
			vector<2> Position = { 0, 0 };

			void Draw(bool const flip);
			void Draw(bool const flip, int index, vector<2> size);
			bool IsExist();
			vector<2> GetLength();
		};
	}

	namespace Animation
	{
		class Component final
		{
		public:
			char const* Content = nullptr;

			float Playback = 0;
			float Duration = 0;
			bool Repeatable = false;

		public:
			vector<2> Length = { 0, 0 };
			float Angle = { 0 };
			vector<2> Location = { 0, 0 };

			void Draw(bool const flip);
		};
	}
}

