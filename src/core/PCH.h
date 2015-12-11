#pragma once

#ifdef _WINDOWS

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include <thread>
#include <chrono>

#include "Types.h"
#include "d3dUtil.h"

// DirectX includes
#include <d3d11_2.h>
#include <d3dCompiler.h>

#include <wrl.h>
#define NOMINMAX

typedef Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContextComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11Query> QueryComPtr;

typedef Microsoft::WRL::ComPtr<ID3D11Buffer> BufferComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11Texture1D> Texture1DComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2DComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11Texture3D> Texture3DComPtr;

typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceViewComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilViewComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetViewComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> UnorderedAccessViewComPtr;

typedef Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStateComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStateComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStateComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerStateComPtr;
typedef Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayoutComPtr;

#define SAFE_RELEASE( x ) {if(x){(x)->Release();(x)=NULL;}}
#define SAFE_DELETE( x ) {if(x){delete (x);(x)=NULL;}}
#define SAFE_DELETE_ARRAY( x ) {if(x){delete[] (x);(x)=NULL;}}

const forward::i32 NUM_THREADS = 1;
const forward::f32 Infinity = FLT_MAX;
const forward::f32 Pi = 3.1415926535f;

#endif