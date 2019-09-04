#include "..\Header\framework.h"

#include "..\Header\Scene.h"

Camera cam;
SceneManager sceneManager;

bool framework::initialize()
{
	g_driverType = D3D_DRIVER_TYPE_NULL;
	g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	Device = NULL;
	DeviceContext = NULL;
	SwapChain = NULL;
	RenderTargetView = NULL;
	DepthStencilView = NULL;


	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
	//#ifdef _DEBUG
	//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];

		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &g_featureLevel, &DeviceContext);
		if (SUCCEEDED(hr))
			break;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	//CreateTexture2D���\�b�h�Ő[�x�X�e���V���o�b�t�@�p�̃e�N�X�`�����쐬����//////////

	D3D11_TEXTURE2D_DESC st;	//�쐬����e�N�X�`���̃^�C�v���w�肵�Ă���	
	ZeroMemory(&st, sizeof(st));

	st.Width = width;
	st.Height = height;
	st.MipLevels = 1;
	st.ArraySize = 1;

	//�[�x�X�e���V���Ƃ��Ďg�p�ł������
	/*

	DXGI_FORMAT_D16_UNORM
	DXGI_FORMAT_D24_UNORM_S8_UINT
	DXGI_FORMAT_D32_FLOAT
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT

	*/

	st.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	st.SampleDesc.Count = 1;
	st.SampleDesc.Quality = 0;
	st.Usage = D3D11_USAGE_DEFAULT;

	/*�[�x�X�e���V���^�[�Q�b�g�Ƃ��ăo�C���h������ׁA
	�@D3D11_BIND_DEPTH_STENCIL�@���w�肵�Ă���
	 */
	st.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	st.CPUAccessFlags = 0;
	st.MiscFlags = 0;

	/////////////////////////////////////////////////////////////////////////////////
	ID3D11Texture2D *pDepthStencilTexture = NULL;

	hr = Device->CreateTexture2D(&st, NULL, &pDepthStencilTexture);
	if (FAILED(hr))
		return false;

	//CreateDepthStencilView���\�b�h�Ńe�N�X�`�����\�[�X����r���[���쐬����/////////

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc; //�쐬����r���[�^�C�v

	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = st.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//�}���`�T���v�����O���g���ꍇ��D3D11_DSV_DIMENSION_TEXTURE2DMS���g��
	dsDesc.Texture2D.MipSlice = 0;
	/////////////////////////////////////////////////////////////////////////////////

	hr = Device->CreateDepthStencilView(pDepthStencilTexture, &dsDesc, &DepthStencilView);
	pDepthStencilTexture->Release();
	if (FAILED(hr))
		return false;

	//�u�����h�X�e�[�g�̍쐬

	blender::Init(Device, DeviceContext);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1, &vp);


	//2d�}�`�̐���
	//sprite[0] = std::make_unique<Sprite>(Device, L"logos.jpg");  //1920x1080   
	//static_mesh = std::make_unique<StaticMesh>(Device, L"OBJ/Mr.Incredible/Mr.Incredible.obj", L"OBJ/Mr.Incredible/Mr.Incredible.mtl");

	//skin_mesh = std::make_unique<SkinMesh>(Device, "FBX/turu.fbx");
	//skin_mesh = std::make_unique<SkinMesh>(Device, "FBX/Sphere-Bot Basic/Armature_001-(FBX 7.4 binary mit Animation).fbx");
	//skin_mesh = std::make_unique<SkinMesh>(Device, "FBX/Street environment_V01.FBX");

	//	���e�ϊ��s��
	projection = cam.GetProjectionMatrix();

	//	����(���s��)
	light_direction = DirectX::XMFLOAT4(0, -1, 1, 0);		//	��+�� ���� ��+�O�ւ̃��C�g

	scale = { 1.0f,1.0f,1.0f };
	_angle = { 0.0f,0.0f,0.0f };
	pos = { 0.0f,0.0f,0.0f };

	audio.Init();
	sceneManager.init();

	return true;
}

void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	//�L�[�{�[�h�̏�Ԏ擾

	KeyBoard::GetState();

	//�Q�[���p�b�h�̏�Ԏ擾

	GamePad::GetState();
	//joyGetPosEx()

	sceneManager.update();
	//Move();
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	const FLOAT clear_color[4] = { 0.0f,0.0f,0.0f,1.0f };	//R,G,B,A

	//�����_�[�^�[�Q�b�g�̃N���A
	DeviceContext->ClearRenderTargetView(RenderTargetView, clear_color);

	//�[�x�X�e���V���@���\�[�X�̃N���A
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���@�o�b�t�@�[�̃o�C���h
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	sceneManager.render();

	//�����_�����O���ꂽ�C���[�W�̕\��
	SwapChain->Present(1, 0);
}

void framework::Release()
{
	blender::Release();

	if (DeviceContext) {
		DeviceContext->ClearState();
	}

	if (RenderTargetView) {
		RenderTargetView->Release();
		RenderTargetView = NULL;
	}
	if (DepthStencilView) {
		DepthStencilView->Release();
		DepthStencilView = NULL;
	}
	if (SwapChain) {
		SwapChain->Release();
		SwapChain = NULL;
	}
	if (DeviceContext) {
		DeviceContext->Release();
		DeviceContext = NULL;
	}
	if (Device) {
		Device->Release();
		Device = NULL;
	}

	audio.Release();
}