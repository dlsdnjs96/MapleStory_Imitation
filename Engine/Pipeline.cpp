

#include <d3d11.h>
#include <assert.h>
#include <stdio.h>

#include "Pipeline.h"

#if not defined _DEBUG
#define MUST(Expression) (      (         (Expression)))
#else
#define MUST(Expression) (assert(SUCCEEDED(Expression)))
#endif


namespace Rendering::Pipeline
{
	namespace
	{
		ID3D11Device*			Device;
		ID3D11DeviceContext*	DeviceContext;

		IDXGISwapChain*			SwapChain;

		namespace Buffer
		{
			ID3D11Buffer* Vertex;
			ID3D11Buffer* Constant[2];

			template<typename Data>
			void Update(ID3D11Buffer* buffer, Data const& data)
			{
				D3D11_MAPPED_SUBRESOURCE Subresource = D3D11_MAPPED_SUBRESOURCE();

				MUST(DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Subresource));
				{ memcpy_s(Subresource.pData, Subresource.RowPitch, data, sizeof(data)); }
				DeviceContext->Unmap(buffer, 0);
			};
		}

		ID3D11RenderTargetView* RenderTargetView;

	}

	namespace DirectX {
		ID3D11Device* GetD3D()
		{
			return Device;
		}
		ID3D11DeviceContext* GetDC()
		{
			return DeviceContext;
		}
	}

	namespace String
	{
		void Render(HFONT const hFont, LPCSTR const string, COLORREF const color, SIZE const size, POINT const center)
		{
			IDXGISurface1* Surface = nullptr;

			MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&Surface)));
			{
				HDC hdc = HDC();

				MUST(Surface->GetDC(false, &hdc));
				{
					if (hFont != HFONT())
						SelectObject(hdc, hFont);

					SetTextColor(hdc, color);

					RECT area
					{
						center.x - size.cx / 2,
						center.y - size.cy / 2,
						center.x + size.cx / 2,
						center.y + size.cy / 2,
					};

					DrawText(hdc, string, ~'\0', &area, 0);
				}
				MUST(Surface->ReleaseDC(nullptr));
			}
			Surface->Release();

			DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
		}
	}

	namespace Texture
	{
		struct Handle final
		{ ID3D11ShaderResourceView* ShaderResourceView = nullptr; };

		void Create(Handle*& handle, SIZE const size, BYTE const* const data)
		{
			D3D11_TEXTURE2D_DESC const Descriptor
			{
				static_cast<UINT>(size.cx),
				static_cast<UINT>(size.cy),
				1,
				1,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				1,
				0,
				D3D11_USAGE_IMMUTABLE,
				D3D11_BIND_SHADER_RESOURCE
			};

			UINT constexpr BPP = 32;

			D3D11_SUBRESOURCE_DATA const SubResource
			{
				data,
				size.cx * BPP / 8
			};

			ID3D11Texture2D* Texture2D = nullptr;

			MUST(Device->CreateTexture2D(&Descriptor, &SubResource, &Texture2D));
			{ MUST(Device->CreateShaderResourceView(Texture2D, nullptr, &(handle = new Handle())->ShaderResourceView)); }
			Texture2D->Release();
		}

		void Render(Handle const* const& handle, RECT const area, bool const flip)
		{
			DeviceContext->PSSetShaderResources(0, 1, &handle->ShaderResourceView);
			{
				float const Coordinates[4][2]
				{
					{ static_cast<float>(area.left),  static_cast<float>(area.top)    },
					{ static_cast<float>(area.right), static_cast<float>(area.top)    },
					{ static_cast<float>(area.left),  static_cast<float>(area.bottom) },
					{ static_cast<float>(area.right), static_cast<float>(area.bottom) }
				};
				float const Coordinates_flipped[4][2]
				{
					{ static_cast<float>(area.right), static_cast<float>(area.top)    },
					{ static_cast<float>(area.left),  static_cast<float>(area.top)    },
					{ static_cast<float>(area.right), static_cast<float>(area.bottom) },
					{ static_cast<float>(area.left),  static_cast<float>(area.bottom) }
				};

				if (flip)
					Buffer::Update(Buffer::Vertex, Coordinates_flipped);
				else
					Buffer::Update(Buffer::Vertex, Coordinates);
			}
			DeviceContext->Draw(4, 0);
		}

		void Delete(Handle const* const& handle)
		{
			handle->ShaderResourceView->Release();

			delete handle;
		}
	}

	namespace Transform
	{
		template<Type type>
		void Update(matrix const& matrix)
		{ Buffer::Update(Buffer::Constant[static_cast<UINT>(type)], matrix); }

		template void Update<Type::Former>(matrix const& matrix);
		template void Update<Type::Latter>(matrix const& matrix);
	}

	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParam, LPARAM const lParam)
	{
		switch (uMessage)
		{
		case WM_CREATE:
		{
			{
				DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();

				Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				Descriptor.SampleDesc.Count = 1;
				Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				Descriptor.BufferCount = 1;
				Descriptor.OutputWindow = hWindow;
				Descriptor.Windowed = true;
				Descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

				MUST(D3D11CreateDeviceAndSwapChain
				(
					nullptr,
					D3D_DRIVER_TYPE_HARDWARE,
					nullptr,
					0,
					nullptr,
					0,
					D3D11_SDK_VERSION,
					&Descriptor,
					&SwapChain,
					&Device,
					nullptr,
					&DeviceContext
				));
			}
			{
#include "Shader/Bytecode/Vertex.h"
				{
					D3D11_INPUT_ELEMENT_DESC const Descriptor[2]
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1 }
					};

					ID3D11InputLayout* InputLayout = nullptr;

					MUST(Device->CreateInputLayout(Descriptor, 2, Bytecode, sizeof(Bytecode) ,&InputLayout));

					DeviceContext->IASetInputLayout(InputLayout);

					InputLayout->Release();
				}
				{
					ID3D11VertexShader* VertexShader = nullptr;

					MUST(Device->CreateVertexShader(Bytecode, sizeof(Bytecode), nullptr, &VertexShader));

					DeviceContext->VSSetShader(VertexShader, nullptr, 0);

					VertexShader->Release();
				}
			}
			{
#include "Shader/Bytecode/Pixel.h"
				{
					ID3D11PixelShader* PixelShader = nullptr;

					MUST(Device->CreatePixelShader( Bytecode, sizeof(Bytecode), nullptr, &PixelShader ));

					DeviceContext->PSSetShader(PixelShader, nullptr, 0);

					PixelShader->Release();
				}
			}
			{
				float const Coordinates[4][2] // 정점 정보
				{
					{ -0.5f, +0.5f },
					{ +0.5f, +0.5f },
					{ -0.5f, -0.5f },
					{ +0.5f, -0.5f }
				};

				D3D11_BUFFER_DESC const Descriptor	// vertex buffer의 Descriptor
				{
					sizeof(Coordinates),			// 정점 정보의 사이즈
					D3D11_USAGE_IMMUTABLE,			// GPU에서만 접근&수정 가능
					D3D11_BIND_VERTEX_BUFFER,		// Vertex Buffer로 바인드
					0								// CPU접근 권한 X
				};

				D3D11_SUBRESOURCE_DATA const Subresource{ Coordinates };	// Subresource에 정점을 저장
				ID3D11Buffer* Buffer = nullptr;								// Buffer 선언

				MUST(Device->CreateBuffer(&Descriptor, &Subresource, &Buffer)); // Vertex Buffer 생성

				UINT const Stride = sizeof(*Coordinates);	// 정점정보 단위(어디까지가 한개의 정점정보인지)
				UINT const Offset = 0;						// 시작 위치

				DeviceContext->IASetVertexBuffers(0, 1, &Buffer, &Stride, &Offset); // Vertex Buffer 등록

				Buffer->Release(); // Buffer 할당해제
			}
			{
				D3D11_BUFFER_DESC const Descriptor
				{
					sizeof(float[4][2]),
					D3D11_USAGE_DYNAMIC,
					D3D11_BIND_VERTEX_BUFFER,
					D3D11_CPU_ACCESS_WRITE
				};

				MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Vertex));

				UINT const Stride = sizeof(float[2]);
				UINT const Offset = 0;

				DeviceContext->IASetVertexBuffers(1, 1, &Buffer::Vertex, &Stride, &Offset);
			}
			{ 
				DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); 
			}
			{
				D3D11_BUFFER_DESC Descriptor
				{
					sizeof(float[4][4]),
					D3D11_USAGE_DYNAMIC,
					D3D11_BIND_CONSTANT_BUFFER,
					D3D11_CPU_ACCESS_WRITE
				};

				for (UINT i = 0; i < 2; ++i)
					MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Constant[i]));

				DeviceContext->VSSetConstantBuffers(0, 2, Buffer::Constant);
			}
			{
				D3D11_BLEND_DESC Descriptor = D3D11_BLEND_DESC();

				Descriptor.AlphaToCoverageEnable = 0; // 멀티샘플링 유무
				Descriptor.IndependentBlendEnable = 0; // 
				Descriptor.RenderTarget->BlendEnable   = true;
				Descriptor.RenderTarget->SrcBlend      = D3D11_BLEND_SRC_ALPHA;
				Descriptor.RenderTarget->DestBlend     = D3D11_BLEND_INV_SRC_ALPHA;
				Descriptor.RenderTarget->BlendOp       = D3D11_BLEND_OP_ADD;
				Descriptor.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ZERO;
				Descriptor.RenderTarget->DestBlendAlpha = D3D11_BLEND_ONE;
				Descriptor.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
				Descriptor.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


				ID3D11BlendState* BlendState = nullptr;
				MUST(Device->CreateBlendState(&Descriptor, &BlendState));

				DeviceContext->OMSetBlendState(BlendState, nullptr, D3D11_DEFAULT_SAMPLE_MASK);
				BlendState->Release();
			}
			return;
		}
		case WM_APP:
		{
			MUST(SwapChain->Present(0, 0));

			float const Color[4]{ 1.0f, 1.0f, 1.0f ,1.0f };

			DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
			return;
		}
		case WM_DESTROY:
		{
			for (UINT i = 0; i < 2; ++i)
				Buffer::Constant[i]->Release();
			Buffer::Vertex->Release();
			SwapChain->Release();
			DeviceContext->Release();
			Device->Release();
			return;
		}
		case WM_SIZE:
		{
			{
				D3D11_VIEWPORT Viewport = D3D11_VIEWPORT();

				Viewport.Width  = LOWORD(lParam);
				Viewport.Height = HIWORD(lParam);

				DeviceContext->RSSetViewports(1, &Viewport);
			}
			{
				if (RenderTargetView != nullptr)
				{
					RenderTargetView->Release();

					MUST(SwapChain->ResizeBuffers
					(
						1,
						LOWORD(lParam),
						HIWORD(lParam),
						DXGI_FORMAT_B8G8R8A8_UNORM,
						DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE
					));
				}
				{
					ID3D11Texture2D* Texture2D = nullptr;

					MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&Texture2D)));
					{
						IDXGISurface1* Surface = nullptr;
						MUST(Texture2D->QueryInterface(IID_PPV_ARGS(&Surface)));
						{
							HDC hdc = HDC();
							MUST(Surface->GetDC(false, &hdc));
							{
								SetBkMode(hdc, TRANSPARENT);
							}
							MUST(Surface->ReleaseDC(nullptr));
						}
						Surface->Release();
					}
					{
						MUST(Device->CreateRenderTargetView(Texture2D, nullptr, &RenderTargetView));

						DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
					}
					Texture2D->Release();
				}

			}
			return;
		}
		}
	}
}