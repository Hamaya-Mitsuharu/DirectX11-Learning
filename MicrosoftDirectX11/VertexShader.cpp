#include "VertexShader.h"

#include <d3dcommon.h>
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

namespace {
	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
	{
		if (!srcFile || !entryPoint || !profile || !blob)
			return E_INVALIDARG;

		*blob = nullptr;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		const D3D_SHADER_MACRO defines[] =
		{
			"EXAMPLE_DEFINE", "1",
			NULL, NULL
		};

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, profile,
			flags, 0, &shaderBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}

			if (shaderBlob)
				shaderBlob->Release();

			return hr;
		}

		*blob = shaderBlob;

		return hr;
	}

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

VertexShader::VertexShader(void) : _pVSBlob(nullptr), _pVertexShader(nullptr), _pVertexLayout(nullptr)
{

}

VertexShader::~VertexShader(void) {

}

/// <summary>
/// 頂点シェーダーコードのコンパイル、頂点レイアウトの作成、頂点シェーダーの作成
/// </summary>
/// <param name="dx11"></param>
/// <returns></returns>
bool VertexShader::Create(DirectX11 &dx11, LPCWSTR srcFile, LPCSTR entrypoint) {
	// Compile Shader Code
	HRESULT hr = CompileShader(srcFile, entrypoint, "vs_4_0_level_9_1", &_pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, TEXT("FAILED：頂点シェーダーのコンパイル"), TEXT(""), MB_OK);
		return false;
	}

	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = dx11.GetDevice()->CreateInputLayout(layout, numElements, _pVSBlob->GetBufferPointer(), _pVSBlob->GetBufferSize(), &_pVertexLayout);
	if (FAILED(hr)) {
		MessageBox(nullptr, TEXT("FAILED：インプットレイアウトの生成"), TEXT(""), MB_OK);
		return false;
	}

	// Create  Vertex Shader
	hr = dx11.GetDevice()->CreateVertexShader(_pVSBlob->GetBufferPointer(), _pVSBlob->GetBufferSize(), NULL, &_pVertexShader);
	SafeRelease(_pVSBlob);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：頂点シェーダーの生成"), TEXT(""), MB_OK);
		return false;
	}

	return true;
}

void VertexShader::Finalize(void) {
	// SafeRelease();
	SafeRelease(_pVertexShader);
	SafeRelease(_pVertexLayout);

	// 以下は初期化完了時点でリリース済み
	// SafeRelease(_pVSBlob);
}