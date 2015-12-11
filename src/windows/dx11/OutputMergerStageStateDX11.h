//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// OutputMergerStageStateDX11
//
//--------------------------------------------------------------------------------
#ifndef OutputMergerStageStateDX11_h
#define OutputMergerStageStateDX11_h
//--------------------------------------------------------------------------------
#include <d3d11_2.h>
#include "TStateMonitor.h"
#include "TStateArrayMonitor.h"
//--------------------------------------------------------------------------------
namespace forward
{
	class OutputMergerStageStateDX11
	{
	public:
		OutputMergerStageStateDX11();
		virtual ~OutputMergerStageStateDX11();

		void SetFeautureLevel( D3D_FEATURE_LEVEL level );
		void ClearState( );
		void SetSisterState( OutputMergerStageStateDX11* pState );
		void ResetUpdateFlags( );

		i32 GetRenderTargetCount() const;

		TStateMonitor< i32 > BlendState;
		TStateMonitor< i32 > DepthStencilState;
		TStateMonitor< u32 > StencilRef;
		TStateArrayMonitor< i32, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT > RenderTargetViews;
		TStateMonitor< i32 > DepthTargetViews;
		TStateArrayMonitor< i32, D3D11_PS_CS_UAV_REGISTER_COUNT > UnorderedAccessViews;
		TStateArrayMonitor< u32, D3D11_PS_CS_UAV_REGISTER_COUNT > UAVInitialCounts;

	protected:

		D3D_FEATURE_LEVEL				m_FeatureLevel;

		OutputMergerStageStateDX11*		m_pSisterState;
	};
};
//--------------------------------------------------------------------------------
#endif // OutputMergerStageStateDX11_h
//--------------------------------------------------------------------------------

