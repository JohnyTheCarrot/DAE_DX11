#ifndef RENDERER_H
#define RENDERER_H
#include "pch.h"

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Timer;

	struct DX11Deleter final {
		void operator()(IUnknown *resource) const noexcept {
			std::cout << "Releasing resource" << std::endl;
			resource->Release();
		}
	};

	struct DX11DeviceContextDeleter final {
		void operator()(ID3D11DeviceContext *deviceContext) const noexcept {
			std::cout << "Releasing ID3D11DeviceContext" << std::endl;
			deviceContext->ClearState();
			deviceContext->Flush();
			deviceContext->Release();
		}
	};

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;

	private:
		SDL_Window* m_pWindow{};
		std::unique_ptr<ID3D11DeviceContext, DX11DeviceContextDeleter> m_pDeviceContext{};
		std::unique_ptr<ID3D11Device, DX11Deleter> m_pDevice{};
		std::unique_ptr<IDXGISwapChain, DX11Deleter> m_pSwapChain{};
		std::unique_ptr<ID3D11Texture2D, DX11Deleter> m_pDepthStencilBuffer{};
		std::unique_ptr<ID3D11DepthStencilView, DX11Deleter> m_pDepthStencilView{};
		std::unique_ptr<ID3D11Resource, DX11Deleter> m_pRenderTargetBuffer{};
		std::unique_ptr<ID3D11RenderTargetView, DX11Deleter> m_pRenderTargetView{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		//DIRECTX
		HRESULT InitializeDirectX();
		//...
	};
}
#endif