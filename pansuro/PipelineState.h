#pragma once

#include "Core.h"

class PipelineState
{
public:
	PipelineState(const std::wstring& shaderFilename,
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDescs,
		ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK,
		CD3DX12_RASTERIZER_DESC rasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
		CD3DX12_BLEND_DESC blendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT),
		CD3DX12_DEPTH_STENCIL_DESC depthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	~PipelineState();

	ComPtr<ID3D12PipelineState> GetPSO() { return m_PSO; }

private:
	ComPtr<ID3D12PipelineState> m_PSO;
};