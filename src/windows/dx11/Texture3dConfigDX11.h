//--------------------------------------------------------------------------------
// Texture3dConfigDX11
//
//--------------------------------------------------------------------------------
#ifndef Texture3dConfigDX11_h
#define Texture3dConfigDX11_h
//--------------------------------------------------------------------------------
#include "Types.h"
#include <d3d11_2.h>
//--------------------------------------------------------------------------------
namespace forward
{
	class Texture3dConfigDX11
	{
	public:
		Texture3dConfigDX11();
		virtual ~Texture3dConfigDX11();

		void SetDefaults();

		void SetWidth( u32 state );
		void SetHeight( u32 state );
		void SetDepth( u32 state );
		void SetMipLevels( u32 state );
		void SetFormat( DXGI_FORMAT state );
		void SetUsage( D3D11_USAGE state ); 
		void SetBindFlags( u32 state );
		void SetCPUAccessFlags( u32 state );
		void SetMiscFlags( u32 state );

		D3D11_TEXTURE3D_DESC GetTextureDesc();

	protected:
		D3D11_TEXTURE3D_DESC 		m_State;

		friend class RendererDX11;
	};
};
//--------------------------------------------------------------------------------
#endif // Texture3dConfigDX11_h
//--------------------------------------------------------------------------------

