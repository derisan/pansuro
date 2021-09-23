#pragma once

#include "pch.h"

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};

#define SAFE_RELEASE(p) if (p) (p)->Release()

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw HrException(hr);
	}
}

inline UINT CalculateConstantBufferByteSize(UINT byteSize)
{
	return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
}

inline Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
	const std::wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const std::string& entrypoint,
	const std::string& target)
{
	UINT compileFlags = 0;
#if defined(_DEBUG) || defined(DBG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
	{
		OutputDebugStringA((char*)errors->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	return byteCode;
}

template<typename T>
class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
		mIsConstantBuffer(isConstantBuffer)
	{
		mElementByteSize = sizeof(T);

		if (isConstantBuffer)
			mElementByteSize = CalculateConstantBufferByteSize(sizeof(T));

		const CD3DX12_HEAP_PROPERTIES uploadBufferHeapProps(D3D12_HEAP_TYPE_UPLOAD);
		const CD3DX12_RESOURCE_DESC uploadbufferDesc = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);
		ThrowIfFailed(device->CreateCommittedResource(
			&uploadBufferHeapProps,
			D3D12_HEAP_FLAG_NONE,
			&uploadbufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer)));

		ThrowIfFailed(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
	}

	~UploadBuffer()
	{
		if (mUploadBuffer != nullptr)
			mUploadBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource* Resource() const
	{
		return mUploadBuffer.Get();
	}

	D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress() const
	{
		return mUploadBuffer->GetGPUVirtualAddress();
	}

	void CopyData(int elementIndex, const T& data)
	{
		memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE* mMappedData = nullptr;

	UINT mElementByteSize = 0;
	bool mIsConstantBuffer = false;
};