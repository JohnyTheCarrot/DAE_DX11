#ifndef DX11_PTR_H
#define DX11_PTR_H
#include "pch.h"

template<typename TDX11>
concept DX11Superclass = std::is_base_of_v<IUnknown, TDX11>;

template<DX11Superclass TDX11>
struct DX11Deleter final {
	void operator()(TDX11 *resource) const noexcept {
#ifdef DEBUG_BUILD
		std::cout << "Releasing " << typeid(TDX11).name() << '\n';
#endif
		resource->Release();
	}
};

struct DX11DeviceContextDeleter final {
	void operator()(ID3D11DeviceContext *deviceContext) const noexcept {
#ifdef DEBUG_BUILD
		std::cout << "Releasing ID3D11DeviceContext" << std::endl;
#endif
		deviceContext->ClearState();
		deviceContext->Flush();
		deviceContext->Release();
	}
};

template<DX11Superclass T, typename TDeleter = DX11Deleter<T>>
using DX11UniquePtr = std::unique_ptr<T, TDeleter>;

#endif
