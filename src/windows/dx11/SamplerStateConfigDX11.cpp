
//--------------------------------------------------------------------------------
#include "SamplerStateConfigDX11.h"
//--------------------------------------------------------------------------------
using namespace forward;
//--------------------------------------------------------------------------------
SamplerStateConfigDX11::SamplerStateConfigDX11()
{
	SetDefaults();
}
//--------------------------------------------------------------------------------
SamplerStateConfigDX11::~SamplerStateConfigDX11()
{
}
//--------------------------------------------------------------------------------
void SamplerStateConfigDX11::SetDefaults()
{
	// Set the state to the default configuration.  These are the D3D11 default
	// values as well.

	Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	MipLODBias = 0.0f;
	MaxAnisotropy = 1;
	ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	BorderColor[0] = 0.0f; BorderColor[1] = 0.0f; BorderColor[2] = 0.0f; BorderColor[3] = 0.0f;
	MinLOD = 0;
	MaxLOD = D3D11_FLOAT32_MAX;
}
//--------------------------------------------------------------------------------
