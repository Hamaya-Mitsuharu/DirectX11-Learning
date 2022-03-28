#include "ConstantBuffer.h"


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

ConstantBuffer::ConstantBuffer(void) : _pConstantBuffer(nullptr) {

}

ConstantBuffer::~ConstantBuffer(void) {

}

bool ConstantBuffer::Create(DirectX11& dx11, UINT bufferSize) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HRESULT hr = (dx11.GetDevice()->CreateBuffer(&bd, nullptr, &_pConstantBuffer));
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：定数バッファの生成"), TEXT(""), MB_OK);
		return false;
	}
	return true;
}

void ConstantBuffer::VSSet(DirectX11& dx11) {
	dx11.GetImmediateContext()->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
}

void ConstantBuffer::PSSet(DirectX11& dx11) {
	dx11.GetImmediateContext()->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
}

void ConstantBuffer::Finalize(void) {
	// SafeRelease();
	SafeRelease(_pConstantBuffer);
}