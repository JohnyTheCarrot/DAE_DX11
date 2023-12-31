#include "pch.h"
#include "Effect.h"

Effect::Effect(ID3D11Device *pDevice, const std::wstring &assetFile) {
	HRESULT result;
	ID3D10Blob *rpErrorBlob{nullptr};
	ID3DX11Effect *rpEffect{};

	DWORD shaderFlags{0};
#ifdef DEBUG_BUILD
	shaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(
			assetFile.c_str(), nullptr, nullptr, shaderFlags, 0, pDevice, &rpEffect, &rpErrorBlob
	);
	DX11UniquePtr<ID3D10Blob> pErrorBlob{rpErrorBlob};

	if (FAILED(result)) {
		if (rpErrorBlob != nullptr) {
			const char *pErrors{static_cast<char *>(rpErrorBlob->GetBufferPointer())};
			std::stringstream ss;

			for (SIZE_T i{}; i < rpErrorBlob->GetBufferSize(); ++i) { ss << pErrors[i]; }

			throw std::runtime_error{ss.str()};
		}

		throw std::runtime_error("Failed to load CreateEffectFromFile!");
	}

	m_pEffect = DX11UniquePtr<ID3DX11Effect>(rpEffect);
	m_pTechnique =
			DX11UniquePtr<ID3DX11EffectTechnique>(m_pEffect->GetTechniqueByName("DefaultTechnique")
			);
	if (!m_pTechnique->IsValid()) { throw std::runtime_error("Technique isn't valid!"); }
}

Effect::Effect(Effect &&other) noexcept
	: m_pEffect{std::move(other.m_pEffect)}
	, m_pTechnique{std::move(other.m_pTechnique)} {
	other.m_pEffect	   = nullptr;
	other.m_pTechnique = nullptr;
}

Effect &Effect::operator=(Effect &&other) noexcept {
	m_pEffect		   = std::move(other.m_pEffect);
	other.m_pEffect	   = nullptr;
	m_pTechnique	   = std::move(other.m_pTechnique);
	other.m_pTechnique = nullptr;

	return *this;
}

const DX11UniquePtr<ID3DX11Effect> &Effect::GetEffectPtr() const { return m_pEffect; }

const DX11UniquePtr<ID3DX11EffectTechnique> &Effect::GetTechniquePtr() const {
	return m_pTechnique;
}
