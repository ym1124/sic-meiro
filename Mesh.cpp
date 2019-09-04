#include	"..\Header\Mesh.h"

#include	"..\Header\framework.h"



/*******************************************************************************
メッシュ初期化
*******************************************************************************/
void	MyMesh::Initialize()
{
	//	情報初期値
	pos = DirectX::XMFLOAT3(0, 0, 0);
	angle = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);


	color = DirectX::XMFLOAT4(1, 1, 1, 1);

	anim_timer = 0.0f;
	//	モデル読み込み
	obj = nullptr;
}

/*******************************************************************************
メッシュモデル読み込み
引数
fbx_filename:メッシュファイル名(FBX形式)
*******************************************************************************/
bool MyMesh::Load(const char* fbx_filename)
{
	//	モデル読み込み
	obj = new SkinMesh(FRAMEWORK.getDevice(), fbx_filename);
	bLoad = true;

	return	true;
}

/*******************************************************************************
メッシュモデル設定
引数
primitive:メッシュモデル
*******************************************************************************/
//void	MyMesh::SetPrimitive(GeometricPrimitive* primitive)
//{
//	obj = primitive;
//	bLoad = true;
//}

/*******************************************************************************
メッシュモデル設定(使い回し型)
引数
primitive:メッシュモデル
*******************************************************************************/
void	MyMesh::SetMesh(MyMesh& org)
{
	*this = org;
	bLoad = false;
}

/*******************************************************************************
メッシュ解放
*******************************************************************************/
void	MyMesh::Release()
{
	if (obj && bLoad)
	{
		delete	obj;
	}
	obj = nullptr;
	bLoad = false;
}

/*******************************************************************************
メッシュ描画
引数
view:ビュー変換行列
projection:投影変換行列
light_dir:平行光ベクトル
*******************************************************************************/
void	MyMesh::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, float timer)
{
	//	モデルが無ければ描画処理を一切行わない
	if (!obj)		return;

	//	ワールド変換行列取得
	DirectX::XMMATRIX worldM = GetWorldMatrix();

	//	Matrix -> Float4x4 変換
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world_view_projection, worldM * view * projection);
	DirectX::XMStoreFloat4x4(&world, worldM);

	//	描画
	obj->Render(FRAMEWORK.getContext(), world_view_projection, world, light_dir, color, timer, true);
}

void MyMesh::Render(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& projectionM, const DirectX::XMFLOAT4& light_dir, int renderType, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light, float timer)
{
	//	モデルが無ければ描画処理を一切行わない
	if (!obj)		return;

	//	ワールド変換行列取得
	DirectX::XMMATRIX worldM = GetWorldMatrix();

	//	Matrix -> Float4x4 変換
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, worldM * viewM * projectionM);
	DirectX::XMStoreFloat4x4(&world, worldM);
	DirectX::XMStoreFloat4x4(&view, viewM);
	DirectX::XMStoreFloat4x4(&projection, projectionM);

	//	描画
	switch (renderType)
	{
	case 0:
		obj->Render(FRAMEWORK.getContext(), world_view_projection, world, view, projection, light_dir, color, timer, true, pos, light_num, light);
		break;
	case 1:
		obj->EnergizedRender(FRAMEWORK.getContext(), world_view_projection, world, view, projection, light_dir, color, timer, true, pos, light_num, light);
		break;
	case 2:
		obj->LoadRender(FRAMEWORK.getContext(), world_view_projection, world, view, projection, light_dir, color, timer, true, pos, light_num, light);
		break;
	}
}

/*******************************************************************************
メッシュ情報からワールド変換行列取得
*******************************************************************************/
DirectX::XMMATRIX	MyMesh::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	初期化
	world = DirectX::XMMatrixIdentity();

	//	拡大・縮小
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//	回転
	Rx = DirectX::XMMatrixRotationX(angle.x);				//	X軸を基準とした回転行列
	Ry = DirectX::XMMatrixRotationY(angle.y);				//	Y軸を基準とした回転行列
	Rz = DirectX::XMMatrixRotationZ(angle.z);				//	Z軸を基準とした回転行列
	R = Rz * Ry * Rx;

	//	平行移動
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//	ワールド変換行列
	world = S * R * T;


	return	world;
}










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeometricMesh::Initialize()
{
	//	情報初期値
	pos = DirectX::XMFLOAT3(0, 0, 0);
	angle = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);


	color = DirectX::XMFLOAT4(1, 1, 1, 1);

	//	モデル読み込み
	obj = nullptr;
}

void GeometricMesh::SetPrimitive(GeometricPrimitive* primitive)
{
	obj = primitive;
	bLoad = true;
}

/*******************************************************************************
メッシュモデル設定(使い回し型)
引数
primitive:メッシュモデル
*******************************************************************************/
void GeometricMesh::SetMesh(GeometricMesh& org)
{
	*this = org;
	bLoad = false;
}

/*******************************************************************************
メッシュ解放
*******************************************************************************/
void GeometricMesh::Release()
{
	if (obj && bLoad)
	{
		delete	obj;
	}
	obj = nullptr;
	bLoad = false;
}

/*******************************************************************************
メッシュ描画
引数
view:ビュー変換行列
projection:投影変換行列
light_dir:平行光ベクトル
*******************************************************************************/
void GeometricMesh::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, float timer)
{
	//	モデルが無ければ描画処理を一切行わない
	if (!obj)		return;

	//	ワールド変換行列取得
	DirectX::XMMATRIX worldM = GetWorldMatrix();

	//	Matrix -> Float4x4 変換
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world_view_projection, worldM * view * projection);
	DirectX::XMStoreFloat4x4(&world, worldM);

	//	描画
	obj->Render(FRAMEWORK.getContext(), world_view_projection, world, light_dir, color, true);
}


/*******************************************************************************
メッシュ情報からワールド変換行列取得
*******************************************************************************/
DirectX::XMMATRIX GeometricMesh::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	初期化
	world = DirectX::XMMatrixIdentity();

	//	拡大・縮小
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//	回転
	Rx = DirectX::XMMatrixRotationX(angle.x);				//	X軸を基準とした回転行列
	Ry = DirectX::XMMatrixRotationY(angle.y);				//	Y軸を基準とした回転行列
	Rz = DirectX::XMMatrixRotationZ(angle.z);				//	Z軸を基準とした回転行列
	R = Rz * Ry * Rx;

	//	平行移動
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//	ワールド変換行列
	world = S * R * T;


	return	world;
}
