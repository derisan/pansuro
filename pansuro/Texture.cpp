#include "pch.h"
#include "Texture.h"

#include "Engine.h"
#include "TextureDescriptorHeap.h"

UINT Texture::s_NumTextures = 0;

Texture* Texture::CreateTexture(const std::wstring& path)
{
	auto tex = new Texture();
	bool res = tex->LoadTexture(path);

	if (!res) return nullptr;
	else return tex;
}

Texture::Texture()
{
	m_MyIndex = s_NumTextures++;
}

bool Texture::LoadTexture(const std::wstring& path)
{
	std::wstring ext = std::filesystem::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, m_RawImage);
	else if (ext == L".tga" || ext == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, m_RawImage);
	else
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, m_RawImage);

	HRESULT hr = ::CreateTexture(DEVICE.Get(), m_RawImage.GetMetadata(), &m_Texture);
	if (FAILED(hr))
	{
		MK_ERROR("Failed to load Texture.");
		return false;
	}

	std::vector<D3D12_SUBRESOURCE_DATA> subResources;

	hr = ::PrepareUpload(DEVICE.Get(),
		m_RawImage.GetImages(),
		m_RawImage.GetImageCount(),
		m_RawImage.GetMetadata(),
		subResources);

	if (FAILED(hr))
	{
		MK_ERROR("Failed to load Texture.");
		return false;
	}

	const UINT64 bufferSize = ::GetRequiredIntermediateSize(m_Texture.Get(), 0, static_cast<UINT>(subResources.size()));

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_TextureUploadBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		MK_ERROR("Failed to load Texture.");
		return false;
	}

	::UpdateSubresources(CMD_LIST.Get(),
		m_Texture.Get(),
		m_TextureUploadBuffer.Get(),
		0, 0,
		static_cast<unsigned int>(subResources.size()),
		subResources.data());

	RELEASE_UPLOAD_BUFFER(m_TextureUploadBuffer);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_RawImage.GetMetadata().format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	DEVICE->CreateShaderResourceView(m_Texture.Get(), &srvDesc, TEXHEAP->GetCpuHandle(m_MyIndex));

	m_SrvGpuHandle = TEXHEAP->GetGpuHandle(m_MyIndex);

	return true;
}


