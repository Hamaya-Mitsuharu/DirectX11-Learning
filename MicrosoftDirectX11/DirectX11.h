#pragma once
#include "SimpleWindow.h"
#include <d3d11_1.h>

class DirectX11
{
public:
	DirectX11(void);
	~DirectX11(void);

	bool Initialize(SimpleWindow& window);
	bool SetRenderTargetView(SimpleWindow& window);
	void Finalize(void);

	ID3D11Device* GetDevice(void) { return _pDevice; };
	ID3D11DeviceContext* GetImmediateContext(void) { return _pImmediateContext; };
	IDXGISwapChain* GetSwapChain(void) { return _pSwapChain; };
	ID3D11RenderTargetView* GetRenderTargetView(void) { return _pRenderTargetView; };
	ID3D11DepthStencilView* GetDepthStencilView(void) { return _pDepthStencilView; };
private:
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pImmediateContext;
	IDXGISwapChain* _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11Texture2D* _pDepthStencil;
	ID3D11DepthStencilView* _pDepthStencilView;
};