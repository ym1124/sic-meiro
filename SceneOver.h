#pragma once

#include "..\Header\Scene.h"
#include "..\Header\player.h"

class SceneOver :public Scene
{
private:
	MyMesh over;
	MyMesh push;

public:
	void init();
	void update();
	void render();
	void release();

	int over_bgm;

	~SceneOver() { release(); }
};
