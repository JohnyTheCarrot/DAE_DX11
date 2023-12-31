#include "pch.h"
#include "Mesh.h"

#include <array>

using namespace dae;

Mesh::Mesh(
		ID3D11Device *rpDevice, const std::vector<Vertex> &vertices,
		const std::vector<uint32_t> &indices
)
	: m_Effect{rpDevice, L"./Resources/PosCol3D.fx"} {
	// Create vertex layout
	std::array<D3D11_INPUT_ELEMENT_DESC, 2> vertexDescriptions{};

	vertexDescriptions[0] = {
			.SemanticName	   = "POSITION",
			.Format			   = DXGI_FORMAT_R32G32B32_FLOAT,
			.AlignedByteOffset = offsetof(Vertex, position),
			.InputSlotClass	   = D3D11_INPUT_PER_VERTEX_DATA
	};
	vertexDescriptions[1] = {
			.SemanticName	   = "COLOR",
			.Format			   = DXGI_FORMAT_R32G32B32_FLOAT,
			.AlignedByteOffset = offsetof(Vertex, color),
			.InputSlotClass	   = D3D11_INPUT_PER_VERTEX_DATA
	};

	// Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{
			.ByteWidth		= static_cast<UINT>(sizeof(Vertex) * vertices.size()),
			.Usage			= D3D11_USAGE_IMMUTABLE,
			.BindFlags		= D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags		= 0
	};

	D3D11_SUBRESOURCE_DATA initData{.pSysMem = vertices.data()};

	ID3D11Buffer *rpVertexBuffer{};
	HRESULT result{rpDevice->CreateBuffer(&bufferDesc, &initData, &rpVertexBuffer)};
	if (FAILED(result)) { throw std::runtime_error{"Failed to create vertex buffer"}; }
	m_pVertexBuffer.reset(rpVertexBuffer);

	// Create input layout
	D3DX11_PASS_DESC passDesc{};
	result = m_Effect.GetTechniquePtr()->GetPassByIndex(0)->GetDesc(&passDesc);
	if (FAILED(result)) { throw std::runtime_error{"Failed to get effect pass description"}; }

	ID3D11InputLayout *rpInputLayout{};
	result = rpDevice->CreateInputLayout(
			vertexDescriptions.data(), static_cast<UINT>(vertexDescriptions.size()),
			passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &rpInputLayout
	);
	if (FAILED(result)) throw std::runtime_error{"Couldn't create input layout"};
	m_pInputLayout.reset(rpInputLayout);

	// Create index buffer
	m_NumIndices = static_cast<UINT>(indices.size());
	bufferDesc	 = D3D11_BUFFER_DESC{
			  .ByteWidth	  = static_cast<UINT>(sizeof(UINT) * m_NumIndices),
			  .Usage		  = D3D11_USAGE_IMMUTABLE,
			  .BindFlags	  = D3D11_BIND_INDEX_BUFFER,
			  .CPUAccessFlags = 0,
			  .MiscFlags	  = 0,
	  };
	initData.pSysMem = indices.data();

	ID3D11Buffer *rpIndexBuffer{};
	result = rpDevice->CreateBuffer(&bufferDesc, &initData, &rpIndexBuffer);
	if (FAILED(result)) throw std::runtime_error{"Failed to create index buffer"};
	m_pIndexBuffer.reset(rpIndexBuffer);
}

void Mesh::Render(ID3D11DeviceContext *rpDeviceContext) const {
	//1. Set primitive topology
	rpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//2. Set input layout
	rpDeviceContext->IASetInputLayout(m_pInputLayout.get());

	//3. Set VertexBuffer
	constexpr UINT stride{sizeof(Vertex)};
	constexpr UINT offset{0};
	const std::array buffers{m_pVertexBuffer.get()};
	rpDeviceContext->IASetVertexBuffers(
			0, static_cast<UINT>(buffers.size()), buffers.data(), &stride, &offset
	);

	//4. Set IndexBuffer
	rpDeviceContext->IASetIndexBuffer(m_pIndexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);

	//5. Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	if (const HRESULT result{m_Effect.GetTechniquePtr()->GetDesc(&techniqueDesc)}; FAILED(result))
		throw std::runtime_error{"Failed to get technique description"};

	for (UINT pass{}; pass < techniqueDesc.Passes; ++pass) {
		std::ignore = m_Effect.GetTechniquePtr()->GetPassByIndex(pass)->Apply(0, rpDeviceContext);
		rpDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}
