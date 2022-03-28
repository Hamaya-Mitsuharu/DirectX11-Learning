#pragma once
#include "DirectX11.h"

#include <d3d11.h>
#include <directXMath.h>
using namespace DirectX;

namespace vertexType{
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
	};
}

class VertexAndIndexBuffer {
public:
	VertexAndIndexBuffer(void);
	~VertexAndIndexBuffer(void);

	bool CreateVertexBuffer(DirectX11& dx11, void* vertices, UINT verticesSize);
	bool CreateIndexBuffer(DirectX11& dx11, void* indices, UINT indicesSize);
	void SetBuffer(DirectX11 &dx11, UINT vertexSize);
	void Finalize(void);

	ID3D11Buffer* GetVertexBuffer(void) { return _pVertexBuffer; };
	ID3D11Buffer* GetIndexBuffer(void) { return _pIndexBuffer; };
private:
	ID3D11Buffer* _pVertexBuffer;
	ID3D11Buffer* _pIndexBuffer;
};