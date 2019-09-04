#pragma once

#include "..\Header\Scene.h"

enum DANBO_MODE
{
	ATK,
	FLY,
	TAIKI,
	MAX,
};

class SceneGame :public Scene
{
private:
	int danbo_state;
	int timer;
	int wani_counter;
	int over_timer;
	int over_wani_num;

public:
	void init();
	void update();
	void render();
	void release();
	~SceneGame() { release(); }
};
