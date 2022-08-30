#include <Windows.h>

#define _USE_MATH_DEFINES

#include <map>
#include <string>

#include "FreeImage.h"
#include "Rendering.h"
#include "Pipeline.h"

#include "Time.h"
#include "Resource.h"

namespace Rendering
{
	namespace
	{
		inline matrix<4, 4> Scale(vector<2> length) 
		{
			return matrix<4, 4>
			{
				length[0], 0, 0, 0,
				0, length[1], 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		inline matrix<4, 4> Rotation(float angle)
		{
			float const radian = static_cast<float>(M_PI) / 180.0f * angle; // 각도 단위를 degree에서 radian으로 변환

			return matrix<4, 4>
			{
				cos(radian), -sin(radian), 0, 0,
				sin(radian), cos(radian), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		inline matrix<4, 4> Location(vector<2> location)
		{
			return matrix<4, 4>
			{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		inline matrix<4, 4> Translation(vector<2> location)
		{
			return matrix<4, 4>
			{
				1, 0, 0, location[0],
				0, 1, 0, location[1],
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
	}

	// Content = "FreeImage"
	// Location = { 10, 20 }
	// Angle = 80.0f;
	// Length = { 200, 200 }

	// if(Press("W")) Location.y += 1;

	namespace Text
	{
		void Import(std::string const& file)
		{
			AddFontResourceEx(file.data(), FR_PRIVATE | FR_NOT_ENUM, nullptr);
		}

		void Component::Draw()
		{
			LOGFONT descriptor = LOGFONT();

			descriptor.lfHeight = Font.Size;
			descriptor.lfWeight = Font.Bold ? FW_BOLD : FW_NORMAL;
			descriptor.lfItalic = Font.Italic;
			descriptor.lfUnderline = Font.Underlined;
			descriptor.lfStrikeOut = Font.StrikeThrough;
			descriptor.lfCharSet = DEFAULT_CHARSET;

			strcpy_s(descriptor.lfFaceName, LF_FACESIZE, Font.Name);

			HFONT const font = CreateFontIndirect(&descriptor);

			SIZE  const area   = { static_cast<LONG>(Length[0]),   static_cast<LONG>(Length[1])   };
			POINT const center = { static_cast<LONG>(Location[0]), static_cast<LONG>(Location[1]) };

			//matrix<4, 4> const world = Translation(Location) * Rotation(0.0f) * Scale(Length);
			//Pipeline::Transform::Update<Pipeline::Transform::Type::Former>(reinterpret_cast<Pipeline::Transform::matrix const&>(world));


			//matrix<4, 4> const projection = Scale(vector<2>(2.0f / 1280.0f, 2.0f / 720.0f));
			//matrix<4, 4> const view = Rotation(0) * Translation(view_port);
			//matrix<4, 4> const latter = projection * view;
			//Pipeline::Transform::Update<Pipeline::Transform::Type::Latter>(reinterpret_cast<Pipeline::Transform::matrix const&>(latter));

			Pipeline::String::Render(font, str.c_str(), RGB(Color.Red, Color.Green, Color.Blue), area, center);

			DeleteObject(font);
		}

	}

	namespace Image
	{
		struct Descriptor final
		{
		public :
			Pipeline::Texture::Handle* Handle = nullptr;

		public :
			SIZE Size = SIZE();
		};

		std::map<std::string, Descriptor> Storage;

		// Shader/HLSL/Vertex.hlsl
		// Free




		void Import(std::string const& file)
		{
			{
				FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(file.data()), file.data());

				FreeImage_FlipVertical(bitmap);

				if (FreeImage_GetBPP(bitmap) != 32)
				{
					FIBITMAP* const previous = bitmap;
					bitmap = FreeImage_ConvertTo32Bits(bitmap);
					FreeImage_Unload(previous);
				}

				Image::Descriptor descriptor = Image::Descriptor();
				{
					descriptor.Size.cx = FreeImage_GetWidth(bitmap);
					descriptor.Size.cy = FreeImage_GetHeight(bitmap);


					Pipeline::Texture::Create(descriptor.Handle, descriptor.Size, FreeImage_GetBits(bitmap));
				}
				{
					size_t const x = file.find_first_of('/') + sizeof(char);
					size_t const y = file.find_last_of('.');

					Image::Storage.try_emplace(file.substr(x, y - x), descriptor);
					//printf("%s\r\n", file.substr(x, y - x).c_str());
					//printf("Import : %s = %s\r\n", file.c_str(), file.substr(x, y - x).c_str());
					//printf("Size : %d = %d\r\n", descriptor.Size.cx, descriptor.Size.cy);
				}
				FreeImage_Unload(bitmap);
			}
		}

		vector<2> Component::GetLength() 
		{
			Descriptor const& image = Storage.at(Content);
			return { image.Size.cx, image.Size.cy };
		}

		bool Component::IsExist()
		{
			return Storage.find(Content) != Storage.end();
		}

		void Component::Draw(bool const flip)
		{
			using namespace Pipeline;
			{
				matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);
				Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));

				//matrix<4, 4> const projection = Scale(vector<2>(2.0f / 1280.0f, 2.0f / 720.0f));
				//matrix<4, 4> const view = Rotation(0) * Translation(view_port);
				//matrix<4, 4> const latter = projection * view;
				//Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::matrix const&>(latter));

				//matrix<4, 4> const world = Translation(Location) * Scale(Length);
				//Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));

				//matrix<4, 4> const latter = Scale(vector<2>(2.0f / 1280.0f, 2.0f / 720.0f)) * Translation(view_port);
				//Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::matrix const&>(latter));
			}
			{
				Descriptor const& image = Storage.at(Content);

				//Pipeline::Texture::Render(image.Handle, { 0, 0, static_cast<LONG>(Length[0]), static_cast<LONG>(Length[1]) }, flip);
				Pipeline::Texture::Render(image.Handle, { 0, 0, image.Size.cx, image.Size.cy }, flip);
			}
		}


		void Component::Draw(bool const flip, int index, vector<2> size)
		{
			using namespace Pipeline;
			{
				matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);
				Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));


				//matrix<4, 4> const world = Translation(Location) * Scale(Length);
				//Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));

				//matrix<4, 4> const latter = Scale(vector<2>(2.0f / 1280.0f, 2.0f / 720.0f)) * Translation(view_port);
				//Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::matrix const&>(latter));
			}
			{
				Descriptor const& image = Storage.at(Content);
				printf("Render %d %d %d %d\r\n", static_cast<LONG>(size[0] * (index % 6)),
					static_cast<LONG>(size[1] * (int)(index / 6)),
					static_cast<LONG>(size[0] * ((index % 6) + 1)),
					static_cast<LONG>(size[1] * (int)((index / 6) + 1)));
				Pipeline::Texture::Render(image.Handle, 
					{ 
					static_cast<LONG>(size[0] * (index%5)),
					static_cast<LONG>(size[1] * (int)(index / 5)),
					static_cast<LONG>(size[0] * ((index % 5) + 1)),
					static_cast<LONG>(size[1] * (int)((index / 5) + 1))
					}, flip);
				//Pipeline::Texture::Render(image.Handle, { 0, 0, image.Size.cx, image.Size.cy }, flip);
			}
		}

	}


	namespace Animation
	{
		struct Descriptor final
		{
		public:
			Pipeline::Texture::Handle* Handle = nullptr;

		public:
			UINT Motion = UINT();
			SIZE Frame = SIZE();
		};

		std::map<std::string, Descriptor> Storage;

		// Shader/HLSL/Vertex.hlsl
		// Free


		void Import(std::string const& file)
		{
			{
				FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(file.data()), file.data());

				FreeImage_FlipVertical(bitmap);

				if (FreeImage_GetBPP(bitmap) != 32)
				{
					FIBITMAP* const previous = bitmap;
					bitmap = FreeImage_ConvertTo32Bits(bitmap);
					FreeImage_Unload(previous);
				}

				Animation::Descriptor descriptor = Animation::Descriptor();
				{
					descriptor.Frame.cx = FreeImage_GetWidth(bitmap);
					descriptor.Frame.cy = FreeImage_GetHeight(bitmap);

					Pipeline::Texture::Create(descriptor.Handle, descriptor.Frame, FreeImage_GetBits(bitmap));
				}
				{
					size_t const x = file.find_first_of('/') + sizeof(char);
					size_t const y = file.find_last_of('[');
					size_t const z = file.find_last_of(']');

					descriptor.Motion = atoi(file.substr(y + sizeof(char), z - (y + sizeof(char))).c_str());
					descriptor.Frame.cx /= descriptor.Motion;
					Animation::Storage.try_emplace(file.substr(x, y - x), descriptor);
				}
				FreeImage_Unload(bitmap);
			}
		}


		void Component::Draw(bool const flip)
		{
			using namespace Pipeline;
			{
				matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);
				Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));
			}
			{
				Descriptor const& descriptor = Storage.at(Content);

				LONG const progress = static_cast<LONG>((Playback / Duration) * descriptor.Motion);

				RECT const area
				{
					descriptor.Frame.cx * (progress + 0), (descriptor.Frame.cy * 0),
					descriptor.Frame.cx * (progress + 1), (descriptor.Frame.cy * 1),
				};

				Pipeline::Texture::Render(descriptor.Handle, area, flip);

				float const delta = Time::Get::Delta();
				Playback += delta;

				if (Duration <= Playback)
				{
					if (Repeatable) Playback =  fmod(Playback, Duration);
					else			Playback -= 1;
				}
			}
		}
	}


	namespace Pipeline 
	{
		void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParamete);
	}


	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
		case WM_CREATE:
		{

			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

			Resource::Import("Font", Text::Import);

			FreeImage_Initialise();
			//FILE* fp = freopen("image_name.txt", "w", stdout);
			Resource::Import("Image", Image::Import);
			//fclose(fp);
			//Resource::Import("Animation", Animation::Import);
			FreeImage_DeInitialise();


			return;
		}
		case WM_APP:
		{
			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

			return;
		}
		case WM_SIZE:
		{
			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);
			return;
		}
		case WM_DESTROY:
		{
			for (auto const& pair : Image::Storage)
				Pipeline::Texture::Delete(pair.second.Handle);

			//for (auto const& pair : Animation::Storage)
			//	Pipeline::Texture::Delete(pair.second.Handle);

			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);
			return;
		}
		return;
		}
	}
	void Camera::Set() const
	{
		using namespace Pipeline;

		matrix<4, 4> const projection = Scale(vector<2>(2.0f / Length[0], 2.0f / Length[1]));
		matrix<4, 4> const view = Rotation(-Angle) * Translation(-Location);
		matrix<4, 4> const latter = projection * view;

		Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::matrix const&>(latter));
	}

}