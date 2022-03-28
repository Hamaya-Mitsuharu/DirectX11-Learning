#include "PixelShader.h"

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

PixelShader::PixelShader(void) : _pPSBlob(nullptr), _pPixelShader(nullptr)
{

}

PixelShader::~PixelShader(void) {

}

bool PixelShader::Create(DirectX11& dx11, LPCWSTR srcFile, LPCSTR entrypoint) {
	// Compile Shader Code
	HRESULT hr = CompileShader(srcFile, entrypoint, "ps_4_0_level_9_1", &_pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("FAILED：ピクセルシェーダーのコンパイル"), TEXT(""), MB_OK);
		return false;
	}

	// Create  Pixel Shader
	hr = dx11.GetDevice()->CreatePixelShader(_pPSBlob->GetBufferPointer(), _pPSBlob->GetBufferSize(), NULL, &_pPixelShader);
	SafeRelease(_pPSBlob);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("FAILED：ピクセルシェーダーの生成"), TEXT(""), MB_OK);
		return false;
	}

	return true;
}

void PixelShader::Finalize(void) {
	// SafeRelease();
	SafeRelease(_pPixelShader);

	// 以下は初期化完了時点でリリース済み
	// SafeRelease(_pPSBlob);
}