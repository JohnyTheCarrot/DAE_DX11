#ifndef EFFECT_H
#define EFFECT_H
#include "DX11UniquePtr.h"

class Effect final {
public:
	Effect(ID3D11Device *pDevice, const std::wstring &assetFile);
	Effect()  = default;
	~Effect() = default;

	Effect(const Effect &)					= delete;
	const Effect &operator=(const Effect &) = delete;

	Effect(Effect &&other) noexcept;

	Effect &operator=(Effect &&other) noexcept;

	[[nodiscard]]
	const DX11UniquePtr<ID3DX11Effect> &GetEffectPtr() const;

	[[nodiscard]]
	const DX11UniquePtr<ID3DX11EffectTechnique> &GetTechniquePtr() const;

private:
	DX11UniquePtr<ID3DX11Effect> m_pEffect{nullptr};
	DX11UniquePtr<ID3DX11EffectTechnique> m_pTechnique{nullptr};
};
#endif
