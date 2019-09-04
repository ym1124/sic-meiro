#pragma once

#include "..\Header\Scene.h"
#include "..\Header\player.h"
#include "..\Header\Sound.h"

class SceneTitle :public Scene
{
private:
	static const int mesh_max = 10;

	MyMesh title;
	MyMesh mesh;
	MyMesh back;

	MyMesh push;
	MyMesh world_box;

	GeometricMesh geometric;

	int font_angle;
public:
	bool iskey;
	bool is_move_camera;

	int title_bgm;
	int warp_se;

	DirectX::XMFLOAT4 p_light;
	int holler_state;
	int holler_timer;

	float timer;

	void init();
	void update();
	void render();
	void release();
	void holler();

	void move();

	~SceneTitle() { release(); }
};
