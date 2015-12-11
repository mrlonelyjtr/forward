//-----------------------------------------------------------------------------
cbuffer Transforms
{
	matrix WorldViewProjMatrix;	
	matrix WorldViewProjMatrixLight;
	float4 flags;
	matrix CSMViewProjMatrix[3];
	float4 toCascadeOffsetX;
	float4 toCascadeOffsetY;
	float4 toCascadeScale;
	float ShadowMapPixelSize;
	float LightSize;
	float BlurFactorX;
	float BlurFactorY;
};

Texture2D<float> tex0: register( t0 );
Texture2DArray<float> tex1 : register( t1 );
Texture2D<float2> vsmTex: register( t2 );
SamplerState s0: register( s0 );
SamplerComparisonState PCFSampler : register( s1 );

//-----------------------------------------------------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float4 positionLight : PositionLightSpace;
	float4 positionWorld : PositionWorldSpace;
};

struct Quad_VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

struct GS_OUTPUT
{
	float4 Pos		: SV_POSITION;
	float4 Color 	: COLOR;
	float4 positionLight : PositionLightSpace;
	float4 positionWorld : PositionWorldSpace;	
	uint RTIndex	: SV_RenderTargetArrayIndex;
};

//-----------------------------------------------------------------------------
VS_OUTPUT VSMain( in VS_INPUT v )
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	o.position = mul(v.position, WorldViewProjMatrix);
	o.color = v.color;
	o.positionLight = mul(v.position, WorldViewProjMatrixLight);

	return o;
}

float4 spotShadowBasic(float3 pos, float2 uv, float4 defaultColor)
{
	float4 ambient = float4(0.01, 0.01, 0.01, 1);
	float bias = 0.0001f;

	float d = pos.z -bias;
	float depthInShadowMap = tex0.Sample(s0, uv).r;

	if(d >= depthInShadowMap)
		return ambient * defaultColor;

	return defaultColor;
}

float4 spotShadowPCF(float3 pos, float2 uv, float4 defaultColor)
{
	float bias = 0.002f;

	float d = pos.z - bias;	
	float depthInShadowMapPCF = tex0.SampleCmpLevelZero(PCFSampler, uv, d).r;

	return depthInShadowMapPCF * defaultColor;
}

// Poisson smapling
static const float2 poissonDisk[16] = 
{
	float2( -0.94201624, -0.39906216 ),
	float2( 0.94558609, -0.76890725 ),
	float2( -0.094184101, -0.92938870 ),
	float2( 0.34495938, 0.29387760 ),
	float2( -0.91588581, 0.45771432 ),
	float2( -0.81544232, -0.87912464 ),
	float2( -0.38277543, 0.27676845 ),
	float2( 0.97484398, 0.75648379 ),
	float2( 0.44323325, -0.97511554 ),
	float2( 0.53742981, -0.47373420 ),
	float2( -0.26496911, -0.41893023 ),
	float2( 0.79197514, 0.19090188 ),
	float2( -0.24188840, 0.99706507 ),
	float2( -0.81409955, 0.91437590 ),
	float2( 0.19984126, 0.78641367 ),
	float2( 0.14383161, -0.14100790 )
};

// Shadow PCSS calculation helper function
float spotShadowPCSS( float3 pos )
{
	float bias = 0.002f;

	// Transform the position to shadow clip space
	float3 UVD = pos;

	// Convert to shadow map UV values
	UVD.xy = 0.5 * UVD.xy + 0.5;
	UVD.y = 1.0 - UVD.y;

	UVD.z = UVD.z - bias;

	// Search for blockers
	float avgBlockerDepth = 0;
	float blockerCount = 0;
	
	[unroll]
	for(int i = -2; i <= 2; i += 2)
	{
		[unroll]
		for(int j = -2; j <= 2; j += 2)
		{
			float4 d4 = tex0.GatherRed(s0, UVD.xy, int2(i, j));
			float4 b4 = (UVD.z <= d4) ? 0.0: 1.0;   

			blockerCount += dot(b4, 1.0);
			avgBlockerDepth += dot(d4, b4);
		}
	}

	// Check if we can early out
	if(blockerCount <= 0.0)
	{
		return 1.0;
	}

	// Penumbra width calculation
	avgBlockerDepth /= blockerCount;
	float fRatio = ((UVD.z - avgBlockerDepth) * LightSize) / avgBlockerDepth;
	fRatio *= fRatio;

	// Apply the filter
	float att = 0;

	[unroll]
	for(i = 0; i < 16; i++)
	{
		float2 offset = fRatio * ShadowMapPixelSize.xx * poissonDisk[i];
		att += tex0.SampleCmpLevelZero(PCFSampler, UVD.xy + offset, UVD.z);
	}

	// Devide by 16 to normalize
	return att * 0.0625;
}


float chebyshevUpperBound( float distance, float2 uv)
{
	float bias = 0.0001f;
	float2 moments = vsmTex.Sample(s0, uv).xy;
	//texture2D(ShadowMap,ShadowCoordPostW.xy).rg;
	
	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance - bias <= moments.x)
		return 1.0 ;

	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.00002);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
}

float spotShadowVSM(float3 pos, float2 uv)
{
	float shadow = chebyshevUpperBound(pos.z, uv);
	return shadow;
}

//-----------------------------------------------------------------------------
float4 PSMain( in VS_OUTPUT input ) : SV_Target
{
	float4 ambient = float4(0.01, 0.01, 0.01, 1);
	float3 pos = input.positionLight.xyz / input.positionLight.w;
	float2 uv = float2(0, 0);
	uv.x = pos.x * 0.5f + 0.5f;
	uv.y = -pos.y * 0.5f + 0.5f;

	bool usePCF = (flags.x >= 1.0f);
	bool useCSM = (flags.y >= 1.0f);
	bool usePCSS= (flags.z >= 1.0f);
	bool useVSM = (flags.w >= 1.0f);

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if((saturate(uv.x) == uv.x) && (saturate(uv.y) == uv.y))
	{
		if(useCSM)
		{
			// Transform the shadow space position into each cascade position
			float4 posCascadeSpaceX = (toCascadeOffsetX + pos.xxxx) * toCascadeScale;
			float4 posCascadeSpaceY = (toCascadeOffsetY + pos.yyyy) * toCascadeScale;

			// Check which cascade we are in
			float4 inCascadeX = abs(posCascadeSpaceX) <= 1.0;
			float4 inCascadeY = abs(posCascadeSpaceY) <= 1.0;
			float4 inCascade = inCascadeX * inCascadeY;

			// Prepare a mask for the highest quality cascade the position is in
			float4 bestCascadeMask = inCascade;
			bestCascadeMask.yzw = (1.0 - bestCascadeMask.x) * bestCascadeMask.yzw;
			bestCascadeMask.zw = (1.0 - bestCascadeMask.y) * bestCascadeMask.zw;
			bestCascadeMask.w = (1.0 - bestCascadeMask.z) * bestCascadeMask.w;
			float bestCascade = dot(bestCascadeMask, float4(0.0, 1.0, 2.0, 3.0));

			// Pick the position in the selected cascade
			float3 UVD;
			UVD.x = dot(posCascadeSpaceX, bestCascadeMask);
			UVD.y = dot(posCascadeSpaceY, bestCascadeMask);
			UVD.z = pos.z;

			// Convert to shadow map UV values
			UVD.xy = 0.5 * UVD.xy + 0.5;
			UVD.y = 1.0 - UVD.y;

			float bias = 0.000002f;
			float d = pos.z - bias;	

			float depthInShadowMap = tex1.Sample(s0, float3(UVD.xy, bestCascade));
			float depthInShadowMapPCF = tex1.SampleCmpLevelZero(PCFSampler, float3(UVD.xy, bestCascade), d);

			if(usePCF)
				return input.color * depthInShadowMapPCF;
			else
			{
				if(d >= depthInShadowMap)
					return ambient * input.color;
				return input.color;
			}
		}
		else if(usePCF)
		{
			return spotShadowPCF(pos, uv, input.color);
		}
		else if(usePCSS)
		{
			return spotShadowPCSS(pos) * input.color;
		}
		else if(useVSM)
		{
			return spotShadowVSM(pos, uv) * input.color;
			//return float4(pos.zzz, 1.0f);
			//return float4(input.position.zzz, 1.0f);
		}
		else
		{
			return spotShadowBasic(pos, uv, input.color);
		}
	}
	else
	{
		return input.color;
	}
}

//-----------------------------------------------------------------------------
VS_OUTPUT VSShadowTargetMain( in VS_INPUT v )
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	o.position = mul(v.position, WorldViewProjMatrixLight);
	o.color = v.color;

	//o.position.xyz = o.position.xyz / o.position.w;
	//o.position.w = 1.0f;
	o.positionWorld = v.position;
	o.positionLight = float4(o.position.xyz / o.position.w, 1.0f);


	return o;
}


//-----------------------------------------------------------------------------
float4 PSShadowTargetMain( in VS_OUTPUT input ) : SV_Target
{
	return input.color;
}

//-----------------------------------------------------------------------------
VS_OUTPUT VSCSMTargetMain(in VS_INPUT v)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	o.position = v.position;
	o.color = v.color;

	return o;
}

///////////////////////////////////////////////////////////////////
// Cascaded shadow maps generation
///////////////////////////////////////////////////////////////////
[maxvertexcount(9)]
void GSCSMTargetMain(triangle float4 InPos[3] : SV_Position, inout TriangleStream<GS_OUTPUT> OutStream)
{
	for(int iFace = 0; iFace < 3; iFace++ )
	{
		GS_OUTPUT output;

		output.RTIndex = iFace;

		for(int v = 0; v < 3; v++ )
		{
			output.Pos = mul(InPos[v], CSMViewProjMatrix[iFace]);
			output.Color = float4(1, 0, 0, 1);
			output.positionLight = output.Pos;
			output.positionWorld = output.Pos;
			OutStream.Append(output);
		}
		OutStream.RestartStrip();
	}
}

/////////////////////////////////////////////////////////////////////
// Variance shadow map generation
/////////////////////////////////////////////////////////////////////
float2 PSVSMDepthGenMain( in VS_OUTPUT input ) : SV_Target
{
	//float depth = input.positionLight.z ;
	float depth = input.position.z;
	//depth = depth * 0.5 + 0.5;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

	float moment1 = depth;
	float moment2 = depth * depth;

	// Adjusting moments (this is sort of bias per pixel) using derivative
	float dx = ddx(depth);
	float dy = ddy(depth);
	moment2 += 0.25*(dx*dx+dy*dy) ;
	

	return float2( moment1, moment2);
}

///////////////////////////////////////////////////////////////////////
/// Pixel Shader for texture blurring
///////////////////////////////////////////////////////////////////////
float2 PSQuadBlurMain( in Quad_VS_OUTPUT input ) : SV_Target
{
	float2 color = 0.0f;

	color += vsmTex.Sample(s0, input.color.xy + float2(-3.0 * BlurFactorX, -3.0 * BlurFactorY) ) * 0.015625;
	color += vsmTex.Sample(s0, input.color.xy + float2(-2.0 * BlurFactorX, -2.0 * BlurFactorY) ) * 0.09375;
	color += vsmTex.Sample(s0, input.color.xy + float2(-1.0 * BlurFactorX, -1.0 * BlurFactorY) ) * 0.234375;
	color += vsmTex.Sample(s0, input.color.xy + float2(-0.0 * BlurFactorX, -0.0 * BlurFactorY) ) * 0.3125;
	color += vsmTex.Sample(s0, input.color.xy + float2(+1.0 * BlurFactorX, +1.0 * BlurFactorY) ) * 0.234375;
	color += vsmTex.Sample(s0, input.color.xy + float2(+2.0 * BlurFactorX, +2.0 * BlurFactorY) ) * 0.09375;
	color += vsmTex.Sample(s0, input.color.xy + float2(+3.0 * BlurFactorX, +3.0 * BlurFactorY) ) * 0.015625;

	return color;
}