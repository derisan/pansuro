#include "pch.h"
#include "PipelineState.h"

#include "Engine.h"

PipelineState::PipelineState(const std::wstring& shaderFilename, 
	const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDescs, 
	ComPtr<ID3D12RootSignature> rootSignature, 
	D3D12_FILL_MODE fillMode,
	D3D12_CULL_MODE cullMode,
	CD3DX12_RASTERIZER_DESC rasterizerState, 
	CD3DX12_BLEND_DESC blendState,
	CD3DX12_DEPTH_STENCIL_DESC depthStencilState)
{
	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;

#ifdef MK_DEBUG
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	ThrowIfFailed(D3DCompileFromFile(shaderFilename.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
	ThrowIfFailed(D3DCompileFromFile(shaderFilename.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs.data(), static_cast<UINT>(inputElementDescs.size()) };
	psoDesc.pRootSignature = rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
	psoDesc.RasterizerState = rasterizerState;
	psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
	psoDesc.RasterizerState.FillMode = fillMode;
	psoDesc.RasterizerState.CullMode = cullMode;
	psoDesc.BlendState = blendState;
	psoDesc.DepthStencilState = depthStencilState;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	ThrowIfFailed(DEVICE->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO)));
}

PipelineState::~PipelineState()
{

}
