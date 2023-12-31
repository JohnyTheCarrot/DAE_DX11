#ifndef RENDERER_H
#define RENDERER_H
#include "pch.h"

#include <array>

#include "DX11UniquePtr.h"
#include "Mesh.h"

struct SDL_Window;
struct SDL_Surface;

namespace dae {
	class Timer;

	class Renderer final {
	public:
		Renderer(SDL_Window *pWindow);
		~Renderer();

		Renderer(const Renderer &)				  = delete;
		Renderer(Renderer &&) noexcept			  = delete;
		Renderer &operator=(const Renderer &)	  = delete;
		Renderer &operator=(Renderer &&) noexcept = delete;

		void Update(const Timer *pTimer);
		void Render() const;

	private:
		SDL_Window *m_pWindow{};
		DX11UniquePtr<ID3D11DeviceContext, DX11DeviceContextDeleter> m_pDeviceContext{};
		DX11UniquePtr<ID3D11Device> m_pDevice{};
		DX11UniquePtr<IDXGISwapChain> m_pSwapChain{};
		DX11UniquePtr<ID3D11Texture2D> m_pDepthStencilBuffer{};
		DX11UniquePtr<ID3D11DepthStencilView> m_pDepthStencilView{};
		DX11UniquePtr<ID3D11Resource> m_pRenderTargetBuffer{};
		DX11UniquePtr<ID3D11RenderTargetView> m_pRenderTargetView{};

		std::array<Mesh, 1> m_Meshes{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{false};

		//DIRECTX
		HRESULT InitializeDirectX();
		//...
	};
}// namespace dae
#endif
