#pragma once
#include "DirectX11.h"

#include <d3d11.h>

class ConstantBuffer {
public:
	ConstantBuffer(void);
	~ConstantBuffer(void);

	bool Create(DirectX11& dx11, UINT bufferSize);
	void VSSet(DirectX11& dx11);
	void PSSet(DirectX11& dx11);
	void Finalize(void);

	ID3D11Buffer* Get(void) { return _pConstantBuffer; };
private:
	ID3D11Buffer* _pConstantBuffer;
};
