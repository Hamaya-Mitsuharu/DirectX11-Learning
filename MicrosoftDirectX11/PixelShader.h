#pragma once

#include "DirectX11.h"
#include <d3d11.h>

class PixelShader {
public:
	PixelShader(void);
	~PixelShader(void);

	bool Create(DirectX11& dx11, LPCWSTR srcFile, LPCSTR entrypoint);
	void Finalize(void);

	ID3D10Blob* GetPSBlob(void) { return _pPSBlob; };
	ID3D11PixelShader* Get(void) { return _pPixelShader; };
private:
	ID3D10Blob* _pPSBlob;
	ID3D11PixelShader* _pPixelShader;
};