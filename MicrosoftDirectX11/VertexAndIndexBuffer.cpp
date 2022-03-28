#include "VertexAndIndexBuffer.h"


namespace {
	template<class T>
	void SafeRelease(T*& p)
	{
		if (p)
		{
			p->Release();
			p = nullptr;
		}
	}
}

VertexAndIndexBuffer::VertexAndIndexBuffer(void) : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr) {

}

VertexAndIndexBuffer::~VertexAndIndexBuffer(void) {

}

/// <summary>
/// 
/// </summary>
/// <param name="dx11"></param>
/// <param name="vertices"></param>
/// <param name="vertexSize">例：sizeof(vertexType::SimpleVertex) * verticesLen</param>
/// <returns></returns>
bool VertexAndIndexBuffer::CreateVertexBuffer(DirectX11 &dx11, void *vertices, UINT verticesSize) {
	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = verticesSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;


	HRESULT hr = dx11.GetDevice()->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：頂点バッファの生成"), TEXT(""), MB_OK);
		return false;
	}
	return true;
}

/// <summary>
/// 
/// </summary>
/// <param name="dx11"></param>
/// <param name="indices"></param>
/// <param name="indicesSize">example ... sizeof(indices) * indicesLen</param>
/// <returns></returns>
bool VertexAndIndexBuffer::CreateIndexBuffer(DirectX11& dx11, void* indices, UINT indicesSize) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = indicesSize; // sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	HRESULT hr = dx11.GetDevice()->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：インデックスバッファの生成"), TEXT(""), MB_OK);
		return false;
	}
	return true;
}

/// <summary>
/// 
/// </summary>
/// <param name="dx11"></param>
/// <param name="verticesSize">example ... sizeof(vertexType::SimpleVertex)</param>
void VertexAndIndexBuffer::SetBuffer(DirectX11& dx11, UINT vertexSize) {
	UINT stride = vertexSize;
	UINT offset = 0;
	dx11.GetImmediateContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	dx11.GetImmediateContext()->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void VertexAndIndexBuffer::Finalize(void) {
	// SafeRelease();
	SafeRelease(_pVertexBuffer);
	SafeRelease(_pIndexBuffer);
}