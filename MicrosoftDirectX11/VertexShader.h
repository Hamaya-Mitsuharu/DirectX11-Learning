#pragma once
#include "DirectX11.h"
#include <d3d11.h>

class VertexShader {
public:
	VertexShader(void);
	~VertexShader(void);
	
	bool Create(DirectX11 &dx11, LPCWSTR srcFile, LPCSTR entrypoint);
	void Finalize(void);

	void SetInputLayout(DirectX11 &dx11) { dx11.GetImmediateContext()->IASetInputLayout(_pVertexLayout); };
	ID3D10Blob* GetVSBlob(void) { return _pVSBlob; };
	ID3D11VertexShader* Get(void) { return _pVertexShader; };
	ID3D11InputLayout* GetVertexLayout(void) { return _pVertexLayout; };
private:
	ID3D10Blob* _pVSBlob;
	ID3D11VertexShader* _pVertexShader;
	ID3D11InputLayout* _pVertexLayout;
};