//--------------------------------------------------------------------------------

#include "DepthStencilViewDX11.h"
//--------------------------------------------------------------------------------
using namespace forward;
//--------------------------------------------------------------------------------
DepthStencilViewDX11::DepthStencilViewDX11( DepthStencilViewComPtr pView )
{
	m_pDepthStencilView = pView;
}
//--------------------------------------------------------------------------------
DepthStencilViewDX11::~DepthStencilViewDX11()
{
}
//--------------------------------------------------------------------------------