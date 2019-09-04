#include "..\Header\Light.h"
#include "..\Header\Player.h"

Light light({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });

float light_color = 0.3f;

Light player_light({ player.pos.x,player.pos.y,player.pos.z }, { light_color,light_color,light_color,1.0f }, { 0.8f, 0.0f, 0.2f });

Light lanthanum[6];