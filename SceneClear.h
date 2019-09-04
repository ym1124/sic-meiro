#pragma once

#include "..\Header\Scene.h"
#include "..\Header\player.h"

class SceneClear :public Scene
{
private:
	MyMesh mesh;
	MyMesh push;

public:
	void init();
	void update();
	void render();
	void release();

	int render_timer;

	Player players[30];

	~SceneClear() { release(); }
};
