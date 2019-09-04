#pragma once

#include <directxmath.h>
#include "..\Header\Mesh.h"
#include "..\Header\Input.h"
#include "..\Header\camera.h"
#include "..\Header\blender.h"
#include "..\Header\Sound.h"

enum
{
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SELECT_STAGE,
	SCENE_GAME,
	SCENE_OVER,
	SCENE_CLEAR,
	SCENE_NAME,
};

class SceneManager
{
public:
	int next_scene;

	void init();
	void update();
	void render();
	void final();

	void setScene(int const scene);
};

class Scene
{
public:
	Camera camera;

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 light_direction;

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void release() = 0;

	virtual ~Scene(){}

	virtual void cameraInit(DirectX::XMFLOAT4 ilght_direct);
	virtual void cameraMove();
};

extern int scene;
extern int now_stage;