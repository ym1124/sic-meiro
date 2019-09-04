#pragma once

#include "..\Header\Scene.h"


class SceneSelect :public Scene
{
private:
	int timer;

public:
	void init();
	void update();
	void render();
	void release();

	~SceneSelect() { release(); }
};
