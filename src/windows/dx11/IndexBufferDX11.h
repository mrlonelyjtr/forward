//--------------------------------------------------------------------------------
// IndexBufferDX11 
//
//--------------------------------------------------------------------------------
#ifndef IndexBufferDX11_h
#define IndexBufferDX11_h
//--------------------------------------------------------------------------------
#include "BufferDX11.h"
//--------------------------------------------------------------------------------
namespace forward
{
	class IndexBufferDX11 : public BufferDX11
	{
	public:
		IndexBufferDX11( Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer );
		virtual ~IndexBufferDX11();

		virtual ResourceType				GetType();

		void						SetIndexSize( i32 size );
		void						SetIndexCount( i32 count );

	protected:
		i32							m_iIndexSize;
		i32							m_iIndexCount;

		friend class RendererDX11;
	};
};
//--------------------------------------------------------------------------------
#endif // IndexBufferDX11_h
