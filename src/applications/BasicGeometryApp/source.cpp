#include "ApplicationDX11.h"
#include "BufferConfigDX11.h"
#include "TriangleIndices.h"

using namespace forward;

//--------------------------------------------------------------------------------
// Structure for Vertex Buffer
struct Vertex
{
	Vector3f Pos;
	Vector4f Color;
};

// structure for constant buffer
struct CBufferType
{
	Matrix4f mat;
};

class BasicGeometryApp : public Application
{
public:
	BasicGeometryApp(HINSTANCE hInstance, i32 width, i32 height)
		: Application(hInstance, width, height)
		, m_vsID(-1)
		, m_psID(-1)
	{
		mMainWndCaption = L"BasicGeometryApp";
	}

	~BasicGeometryApp()
	{
		Log::Get().Close();
	}

	virtual bool Init();
	virtual void OnResize();

protected:
	virtual void UpdateScene(f32 dt);
	virtual void DrawScene();
	virtual void OnSpace();

private:
	void BuildShaders();
	void BuildGeometry();
	void SetupPipeline();

	i32 m_vsID;
	i32 m_psID;

	GeometryPtr m_pGeometry;
	Matrix4f m_worldMat;
	Matrix4f m_viewMat;
	Matrix4f m_projMat;
	ResourcePtr m_constantBuffer;
};

i32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*prevInstance*/,
	PSTR /*cmdLine*/, i32 /*showCmd*/)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	BasicGeometryApp theApp(hInstance, 1200, 800);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}

void BasicGeometryApp::UpdateScene(f32 /*dt*/)
{
	auto frames = (f32)mTimer.FrameCount() / 1000;
	m_worldMat = Matrix4f::RotationMatrixY(frames) * Matrix4f::RotationMatrixX(frames);
}

void BasicGeometryApp::DrawScene()
{
	m_pRender->pImmPipeline->ClearBuffers(Colors::LightSteelBlue);


	auto pData = m_pRender->pImmPipeline->MapResource(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0);
	auto pBuffer = (CBufferType*)pData.pData;
	pBuffer->mat = m_worldMat * m_viewMat * m_projMat;
	m_pRender->pImmPipeline->UnMapResource(m_constantBuffer, 0);

	m_pRender->pImmPipeline->ApplyPipelineResources();
	m_pGeometry->Execute(m_pRender->pImmPipeline);

	m_pRender->Present(MainWnd(), 0);
}

bool BasicGeometryApp::Init()
{
	Log::Get().Open();
	if (!Application::Init())
		return false;

	// Init the world matrix
	m_worldMat = Matrix4f::Identity();
	// Build the view matrix.
	Vector3f pos = Vector3f(0.0f, 1.0f, -5.0f);
	Vector3f target; target.MakeZero();
	Vector3f up = Vector3f(0.0f, 1.0f, 0.0f);
	m_viewMat = Matrix4f::LookAtLHMatrix(pos, target, up);
	// Build the projection matrix
	m_projMat = Matrix4f::PerspectiveFovLHMatrix(0.5f * Pi, AspectRatio(), 0.01f, 100.0f);

	BufferConfigDX11 cbConfig;
	cbConfig.SetDefaultConstantBuffer(sizeof(CBufferType), true);
	m_constantBuffer = m_pRender->CreateConstantBuffer(&cbConfig, 0);

	BuildShaders();
	BuildGeometry();

	return true;
}

void BasicGeometryApp::BuildShaders()
{
	const std::wstring shaderfile = L"BasicShader.hlsl";
	const std::wstring VSMain = L"VSMain";
	const std::wstring PSMain = L"PSMain";
	m_vsID = m_pRender->LoadShader(ShaderType::VERTEX_SHADER, shaderfile, VSMain, std::wstring(L"vs_5_0"));
	m_psID = m_pRender->LoadShader(ShaderType::PIXEL_SHADER, shaderfile, PSMain, std::wstring(L"ps_5_0"));
}

void BasicGeometryApp::OnResize()
{
	Application::OnResize();
	SetupPipeline();
}

void BasicGeometryApp::BuildGeometry()
{
	// create a box with GeometryDX11
	{
		m_pGeometry = GeometryPtr(new GeometryDX11());

		const i32 NumVertexOfBox = 8;
		// create the vertex element streams
		VertexElementDX11* pPositions = new VertexElementDX11(3, NumVertexOfBox);
		pPositions->m_SemanticName = VertexElementDX11::PositionSemantic;
		pPositions->m_uiSemanticIndex = 0;
		pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
		pPositions->m_uiInputSlot = 0;
		pPositions->m_uiAlignedByteOffset = 0;
		pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pPositions->m_uiInstanceDataStepRate = 0;

		VertexElementDX11* pColors = new VertexElementDX11(4, NumVertexOfBox);
		pColors->m_SemanticName = VertexElementDX11::ColorSemantic;
		pColors->m_uiSemanticIndex = 0;
		pColors->m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		pColors->m_uiInputSlot = 0;
		pColors->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		pColors->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pColors->m_uiInstanceDataStepRate = 0;

		m_pGeometry->AddElement(pPositions);
		m_pGeometry->AddElement(pColors);


		*pPositions->Get3f(0) = Vector3f(-1.0f, -1.0f, -1.0f);
		*pPositions->Get3f(1) = Vector3f(-1.0f, +1.0f, -1.0f);
		*pPositions->Get3f(2) = Vector3f(+1.0f, +1.0f, -1.0f);
		*pPositions->Get3f(3) = Vector3f(+1.0f, -1.0f, -1.0f);
		*pPositions->Get3f(4) = Vector3f(-1.0f, -1.0f, +1.0f);
		*pPositions->Get3f(5) = Vector3f(-1.0f, +1.0f, +1.0f);
		*pPositions->Get3f(6) = Vector3f(+1.0f, +1.0f, +1.0f);
		*pPositions->Get3f(7) = Vector3f(+1.0f, -1.0f, +1.0f);

		*pColors->Get4f(0) = Colors::White;
		*pColors->Get4f(1) = Colors::Black;
		*pColors->Get4f(2) = Colors::Red;
		*pColors->Get4f(3) = Colors::Green;
		*pColors->Get4f(4) = Colors::Blue;
		*pColors->Get4f(5) = Colors::Yellow;
		*pColors->Get4f(6) = Colors::Cyan;
		*pColors->Get4f(7) = Colors::Magenta;

		m_pGeometry->AddFace(TriangleIndices(0, 1, 2));
		m_pGeometry->AddFace(TriangleIndices(0, 2, 3));

		m_pGeometry->AddFace(TriangleIndices(4, 6, 5));
		m_pGeometry->AddFace(TriangleIndices(4, 7, 6));

		m_pGeometry->AddFace(TriangleIndices(4, 5, 1));
		m_pGeometry->AddFace(TriangleIndices(4, 1, 0));

		m_pGeometry->AddFace(TriangleIndices(3, 2, 6));
		m_pGeometry->AddFace(TriangleIndices(3, 6, 7));

		m_pGeometry->AddFace(TriangleIndices(1, 5, 6));
		m_pGeometry->AddFace(TriangleIndices(1, 6, 2));

		m_pGeometry->AddFace(TriangleIndices(4, 0, 3));
		m_pGeometry->AddFace(TriangleIndices(4, 3, 7));

		m_pGeometry->LoadToBuffers();
	}

	SetupPipeline();
}

void BasicGeometryApp::SetupPipeline()
{
	auto resource = m_pRender->GetResourceByIndex(m_constantBuffer->m_iResource);

	ShaderStageStateDX11 vsState;
	vsState.ShaderProgram.SetState(m_vsID);
	vsState.ConstantBuffers.SetState(0, (ID3D11Buffer*)resource->GetResource());
	m_pRender->pImmPipeline->VertexShaderStage.DesiredState = vsState;

	ShaderStageStateDX11 psState;
	psState.ShaderProgram.SetState(m_psID);
	psState.ConstantBuffers.SetState(0, (ID3D11Buffer*)resource->GetResource());
	m_pRender->pImmPipeline->PixelShaderStage.DesiredState = psState;
}

void BasicGeometryApp::OnSpace()
{
	mAppPaused = !mAppPaused;
}