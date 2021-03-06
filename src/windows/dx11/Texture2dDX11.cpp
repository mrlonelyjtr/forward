//--------------------------------------------------------------------------------
#include "Texture2dDX11.h"
//--------------------------------------------------------------------------------
using namespace forward;
//--------------------------------------------------------------------------------
Texture2dDX11::Texture2dDX11( Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex )
{
	m_pTexture = pTex;

	ZeroMemory( &m_DesiredDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	ZeroMemory( &m_ActualDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
}
//--------------------------------------------------------------------------------
Texture2dDX11::~Texture2dDX11()
{
}
//--------------------------------------------------------------------------------
ResourceType Texture2dDX11::GetType()
{
	return( RT_TEXTURE2D );
}
//--------------------------------------------------------------------------------
D3D11_TEXTURE2D_DESC Texture2dDX11::GetActualDescription()
{
	ZeroMemory( &m_ActualDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	if ( m_pTexture )
		m_pTexture->GetDesc( &m_ActualDesc );

	return( m_ActualDesc );
}
//--------------------------------------------------------------------------------
D3D11_TEXTURE2D_DESC Texture2dDX11::GetDesiredDescription()
{
	return( m_DesiredDesc );
}
//--------------------------------------------------------------------------------
void Texture2dDX11::SetDesiredDescription( D3D11_TEXTURE2D_DESC description )
{
	m_DesiredDesc = description;
}
//--------------------------------------------------------------------------------
ID3D11Resource* Texture2dDX11::GetResource()
{
	return( m_pTexture.Get() );
}
//--------------------------------------------------------------------------------
u32 Texture2dDX11::GetEvictionPriority()
{
	u32 priority = 0;

	if ( m_pTexture )
		priority = m_pTexture->GetEvictionPriority();

	return( priority );
}
//--------------------------------------------------------------------------------
void Texture2dDX11::SetEvictionPriority( u32 EvictionPriority )
{
	if ( m_pTexture )
		m_pTexture->SetEvictionPriority( EvictionPriority );
}
//--------------------------------------------------------------------------------