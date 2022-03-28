#pragma once
#include "SimpleWindow.h"
#include "DirectX11.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexAndIndexBuffer.h"
#include "ConstantBuffer.h"

#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <directxcolors.h>

#pragma comment(lib, "d3d11.lib")
using namespace DirectX;

bool Dx11Setting(SimpleWindow& window);
bool ShaderSetting(void);
bool BufferSetting(void);
void AllFinalize(void);
void Render();

SimpleWindow window;
DirectX11 dx11;
VertexShader mainVertexShader;
PixelShader mainPixelShader;
PixelShader solidPixelShader;
VertexAndIndexBuffer viBuffer;
ConstantBuffer cBuffer;

struct ConstantBufferData
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
};

XMMATRIX g_World;
XMMATRIX g_View;
XMMATRIX g_Projection;

// デバッグ用コピペ
// MessageBox(NULL, TEXT("FAILED：デバッグメッセージ"), TEXT(""), MB_OK);

// デバッグ用コピペ（書式）
// TCHAR s[128];
// wsprintf(s, TEXT("%d"), 3);
// MessageBox(NULL, s, TEXT(""), MB_OK);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow) {

	if (!window.CreateSimpleWindow(TEXT("初DirectX11"), hInstance)) {
		MessageBox(NULL, TEXT("FAILED：ウィンドウの初期化（Main.cpp）"), TEXT(""), MB_OK);
		return 0;
	}
	if (!Dx11Setting(window)) {
		MessageBox(NULL, TEXT("FAILED：DX11関係の初期化（Main.cpp）"), TEXT(""), MB_OK);
		AllFinalize();
		return 0;
	}
	if (!ShaderSetting()) {
		MessageBox(NULL, TEXT("FAILED：シェーダー関係の初期化（Main.cpp）"), TEXT(""), MB_OK);
		AllFinalize();
		return 0;
	}
	if (!BufferSetting()) {
		MessageBox(NULL, TEXT("FAILED：バッファ関係の初期化（Main.cpp）"), TEXT(""), MB_OK);
		AllFinalize();
		return 0;
	}

	// Calcurate the transform matrix
	// g_World = 単位行列
	// g_View = XMMatrixLookAtLH()
	// g_Projection = XMMatrixPerspectiveFovLH()
	XMVECTOR eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
	XMVECTOR focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	g_World = XMMatrixIdentity();
	g_View = XMMatrixLookAtLH(eye, focus, up);
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, window.GetWidth() / (FLOAT)window.GetHeight(), 0.01f, 100.0f);

	while (!window.IsQuitMessage())
	{
		window.UpdateWindowMessage();
		Render();
	}
	AllFinalize();
	return  window.GetMessage().wParam;
}

void AllFinalize(void) {
	dx11.Finalize();
	mainVertexShader.Finalize();
	mainPixelShader.Finalize();
	solidPixelShader.Finalize();
	viBuffer.Finalize();
	cBuffer.Finalize();
}

bool Dx11Setting(SimpleWindow &window) {
	if (!dx11.Initialize(window)) {
		return false;
	}
	if (!dx11.SetRenderTargetView(window)) {
		return false;
	}
	return true;
}

bool ShaderSetting(void) {
	if (!mainVertexShader.Create(dx11, L"PixelAndVertex.hlsl", "VS")) {
		return false;
	}
	if (!mainPixelShader.Create(dx11, L"PixelAndVertex.hlsl", "PS")) {
		return false;
	}
	if (!solidPixelShader.Create(dx11, L"PixelAndVertex.hlsl", "PSSolid")) {
		return false;
	}
	mainVertexShader.SetInputLayout(dx11);

	return true;
}

bool BufferSetting(void) {
	vertexType::SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};
	UINT verticesLen = sizeof(vertices) / sizeof(vertices[0]);

	if (!viBuffer.CreateVertexBuffer(dx11, vertices, sizeof(vertexType::SimpleVertex) * verticesLen)) {
		return false;
	}

	// Create index buffer
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	UINT indicesLen = sizeof(indices) / sizeof(indices[0]);

	if (!viBuffer.CreateIndexBuffer(dx11, indices, sizeof(WORD) * indicesLen)) {
		return false;
	}
	viBuffer.SetBuffer(dx11, sizeof(vertexType::SimpleVertex));
	dx11.GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!cBuffer.Create(dx11, sizeof(ConstantBufferData))) {
		return false;
	}

	return true;
}

void Render() {
	//
	// Clear the bufferes
	//
	dx11.GetImmediateContext()->ClearRenderTargetView(dx11.GetRenderTargetView(), Colors::MidnightBlue);
	dx11.GetImmediateContext()->ClearDepthStencilView(dx11.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	//
	// Update our time
	//
	static float t = 0.0f;
	t += (float)XM_PI * 0.001f;

	//
	// Setup our lighting parameters
	//
	XMFLOAT4 vLightDirs[2] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 vLightColors[2] =
	{
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f)
	};

	//
	// Animate the lights
	//
	XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
	XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
	vLightDir = XMVector3Transform(vLightDir, mRotate);
	XMStoreFloat4(&vLightDirs[1], vLightDir);

	//
	// Update Constant Buffer Data
	//
	ConstantBufferData cbData;
	cbData.mWorld = XMMatrixTranspose(g_World);
	cbData.mView = XMMatrixTranspose(g_View);
	cbData.mProjection = XMMatrixTranspose(g_Projection);
	cbData.vLightDir[0] = vLightDirs[0];
	cbData.vLightDir[1] = vLightDirs[1];
	cbData.vLightColor[0] = vLightColors[0];
	cbData.vLightColor[1] = vLightColors[1];
	cbData.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	dx11.GetImmediateContext()->UpdateSubresource(cBuffer.Get(), 0, NULL, &cbData, 0, 0);

	//
	// Animate center cube
	//
	g_World = XMMatrixRotationY(t);

	//
	// Draw center cube
	//
	dx11.GetImmediateContext()->VSSetShader(mainVertexShader.Get(), nullptr, 0);
	dx11.GetImmediateContext()->PSSetShader(mainPixelShader.Get(), nullptr, 0);
	cBuffer.PSSet(dx11);
	cBuffer.VSSet(dx11);
	dx11.GetImmediateContext()->DrawIndexed(36, 0, 0);

	//
	// Draw light cube (with solid pixel shader)
	//
	for (int m = 0; m < 2; m++)
	{
		// 小さくなって、光線ベクトル×５だけ移動する
		XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&vLightDirs[m]));
		XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		mLight = mLightScale * mLight;

		// Update the world variable to reflect the current light
		// 転置行列を取得し、ワールド変換行列にする
		cbData.mWorld = XMMatrixTranspose(mLight);
		// vLightColors[0]...白　vLightColors[1]...赤
		cbData.vOutputColor = vLightColors[m];
		dx11.GetImmediateContext()->UpdateSubresource(cBuffer.Get(), 0, nullptr, &cbData, 0, 0);
		dx11.GetImmediateContext()->PSSetShader(solidPixelShader.Get(), nullptr, 0);
		dx11.GetImmediateContext()->DrawIndexed(36, 0, 0);
	}

	dx11.GetSwapChain()->Present(0, 0);
}