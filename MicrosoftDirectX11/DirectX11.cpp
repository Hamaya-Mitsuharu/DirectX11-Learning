#include "DirectX11.h"

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

DirectX11::DirectX11(void) : _pDevice(nullptr), _pImmediateContext(nullptr) ,_pSwapChain(nullptr), _pRenderTargetView(nullptr){

}
DirectX11::~DirectX11() {

}

bool DirectX11::Initialize(SimpleWindow& window) {
	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX 11.1
		D3D_FEATURE_LEVEL_11_0,		// DirectX 11.0
		// D3D_FEATURE_LEVEL_10_1,	// DirectX 10.1
		// D3D_FEATURE_LEVEL_10_0,	// DirectX 10.0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = window.GetWidth();
	sd.BufferDesc.Height = window.GetHeight();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window.GetWindowHandle();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL		, 0	  , featureLevels	   , numFeatureLevels, D3D11_SDK_VERSION, 
									   &sd , &_pSwapChain			 , &_pDevice, NULL, &_pImmediateContext										 );
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：スワップチェインの生成"), TEXT(""), MB_OK);
		return false;
	}

	// Create a viewport
	D3D11_VIEWPORT _vp;
	_vp.Width = (FLOAT)window.GetWidth();
	_vp.Height = (FLOAT)window.GetHeight();
	_vp.MinDepth = 0.0f;
	_vp.MaxDepth = 1.0f;
	_vp.TopLeftX = 0;
	_vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &_vp);

	return true;
}

bool DirectX11::SetRenderTargetView(SimpleWindow& window) {
	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	HRESULT hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：バックバッファの取得"), TEXT(""), MB_OK);
		return false;
	}
	hr = _pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	SafeRelease(pBackBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：レンダーターゲットビューの生成"), TEXT(""), MB_OK);
		return false;
	}

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = window.GetWidth();
	descDepth.Height = window.GetHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = _pDevice->CreateTexture2D(&descDepth, NULL, &_pDepthStencil);
	if (FAILED(hr)){
		MessageBox(NULL, TEXT("FAILED：デプスステンシルテクスチャの生成"), TEXT(""), MB_OK);
		return false;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	
	hr = _pDevice->CreateDepthStencilView(_pDepthStencil, &descDSV, &_pDepthStencilView);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：デプスステンシルビューの生成"), TEXT(""), MB_OK);
		return false;
	}

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	return true;
}

/// <summary>
/// DirectX11の全メンバ変数にRelease()を行う
/// DX11関係のポインタは使用後に必ずRelease()で解放する必要がある
/// </summary>
void DirectX11::Finalize() {
	SafeRelease(_pDevice);
	SafeRelease(_pImmediateContext);
	SafeRelease(_pSwapChain);
	SafeRelease(_pRenderTargetView);
	SafeRelease(_pDepthStencil);
	SafeRelease(_pDepthStencilView);
}