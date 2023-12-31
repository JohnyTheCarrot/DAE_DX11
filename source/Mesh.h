#ifndef MESH_H
#define MESH_H
#include "Effect.h"
#include "Vertex.h"

namespace dae {
	class Mesh final {
	public:
		Mesh() = default;
		Mesh(ID3D11Device *rpDevice, const std::vector<Vertex> &vertices,
			 const std::vector<uint32_t> &indices);

		void Render(ID3D11DeviceContext *rpDeviceContext) const;

	private:
		Effect m_Effect{};
		DX11UniquePtr<ID3D11Buffer> m_pVertexBuffer{};
		DX11UniquePtr<ID3D11InputLayout> m_pInputLayout{};
		DX11UniquePtr<ID3D11Buffer> m_pIndexBuffer{};
		UINT m_NumIndices{};
	};
}// namespace dae

#endif
