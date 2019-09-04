#pragma once

#include <Windows.h>
#include <Xinput.h>

#define TO_RADIAN	0.017453292519f
#define TO_DEGREE	52.29577951308f

//�}�E�X�֘A
class Mouse
{
public:
	enum Botton
	{
		LEFT,
		RIGHT,
		MIDDLE,
		BOTTON_MAX,
	};

private:
	bool is_push[BOTTON_MAX];
	bool is_release[BOTTON_MAX];

	bool is_push_trigger[BOTTON_MAX];
	bool is_release_trigger[BOTTON_MAX];

	LPPOINT p;
	
public:
	static UINT GetState(UINT msg);

	void SetIsPush(const Botton botton, const bool flg) { is_push[botton] = flg; }
	void SetIsRelease(const Botton botton, const bool flg) { is_release[botton] = flg; }

	void SetIsPushTrigger(const Botton botton, const bool flg) { is_push_trigger[botton] = flg; }
	void SetIsReleaseTrigger(const Botton botton, const bool flg) { is_release_trigger[botton] = flg; }

	void SetBottonDownFlg(const Botton botton);
	void SetBottonUpFlg(const Botton botton);

	bool Push(const Botton botton);				//�}�E�X��������Ă�����		true
	bool PushTrigger(const Botton botton);		//�}�E�X�������ꂽ�u��			true
	bool ReleaseTrigger(const Botton botton);	//�}�E�X�������ꂽ�u��			true
	bool None(const Botton botton);				//�}�E�X��������ĂȂ�������	true
};

extern Mouse mouse;


//�L�[�{�[�h�֘A
class KeyBoard
{
private:
	static const int keyMax = 256;

	BYTE key[keyMax];
	BYTE key_prev[keyMax];

	void GetKeyState();

public:
	KeyBoard() {};
	~KeyBoard() {};

	KeyBoard& operator=(const KeyBoard &org)
	{
		for (int i = 0; i < keyMax; i++) {
			key[i] = org.key[i];
		}

		return *this;
	}

	static void GetState();	//�S���̃L�[�̏�Ԃ��擾���Ă���

	bool Push(const int _key);				//�L�[��������Ă�����		true
	bool PushTrigger(const int _key);		//�L�[�������ꂽ�u��		true
	bool ReleaseTrigger(const int _key);	//�L�[�������ꂽ�u��		true
	bool None(const int _key);				//�L�[��������ĂȂ�������	true
};

extern KeyBoard kb;




//�Q�[���p�b�h�֘A
class GamePad
{
private:
	static const int controllerMax = 4;

	struct ControllerState
	{
		XINPUT_STATE state;			//�p�b�h��id
		XINPUT_VIBRATION vibration;	//�p�b�h�̐U�����x
		bool is_connected;			//�p�b�h���ڑ�����Ă�����	true
	};

	ControllerState gamePad[controllerMax];

	WORD buttons[controllerMax];
	WORD buttons_prev[controllerMax];

	struct bTrigger
	{
		BYTE left;
		BYTE right;
	};

	bTrigger trg[controllerMax];
	bTrigger trg_prev[controllerMax];

	HRESULT UpdateControllerState();	//�p�b�h���ڑ�����Ă��邩���ׂ�

	float GetDegreeAngle(const float x, const float y);
	float GetRadianAngle(const float x, const float y);

	bool CheckRange(const SHORT x, const SHORT y);

public:
	GamePad() {};
	~GamePad() {};

	GamePad& operator=(const GamePad &org)
	{
		for (int i = 0; i < controllerMax; i++) {
			gamePad[i] = org.gamePad[i];
		}

		return *this;
	}

	//�ǂ̃R���g���[���[�����߂邽�߂�id
	enum PlayerID
	{
		_1P,
		_2P,
		_3P,
		_4P,
		PLAYER_MAX
	};

	//�R���g���[���[�� �g���K�[(TRIGGER) �̏�Ԃ𒲂ׂ邽�� 
	enum TriggerType
	{
		PUSH,
		PUSH_TRIGGER,
		RELEASE_TRIGGER,
		NONE,
	};

	static SHORT thres_hold;	//�p�b�h�̔�������͈͂̐ݒ肷�邽�߂̒l

	static void GetState();		//�p�b�h�̏�Ԃ��擾���Ă���


	bool CheckLStickRange(const PlayerID player);		//���̃p�b�h�̔�������͈͂����ׂ�
	bool CheckRStickRange(const PlayerID player);		//�E�̃p�b�h�̔�������͈͂����ׂ�

	float GetLStickDegreeAngle(const PlayerID player);	//�x(degree) �ō��̃p�b�h�̊p�x�𒲂ׂ�
	float GetLStickRadianAngle(const PlayerID player);	//���W�A�� �ō��̃p�b�h�̊p�x�𒲂ׂ�

	float GetRStickDegreeAngle(const PlayerID player);	//�x(degree) �ŉE�̃p�b�h�̊p�x�𒲂ׂ�
	float GetRStickRadianAngle(const PlayerID player);	//���W�A�� �ŉE�̃p�b�h�̊p�x�𒲂ׂ�


	void SetVibration(const float motor1_speed, const float motor2_speed, const int player);	//�p�b�h�̐U���̑��x�̐ݒ�	�ŏ� 0.0f, �ő� 100.0f 
	void ResetVibration(const PlayerID player);													//�p�b�h�̐U���̑��x�� 0.0f �ɂ���


	bool LTrigger(const TriggerType type, const PlayerID player);	//�p�b�h�̍��̃g���K�[(TRIGGER) �̏�Ԃ𒲂ׂ�
	bool RTrigger(const TriggerType type, const PlayerID player);	//�p�b�h�̉E�̃g���K�[(TRIGGER) �̏�Ԃ𒲂ׂ�

	bool Push(const int botton, const PlayerID player);				//�{�^����������Ă�����		true
	bool PushTrigger(const int botton, const PlayerID player);		//�{�^���������ꂽ�u��			true
	bool ReleaseTrigger(const int botton, const PlayerID player);	//�{�^���������ꂽ�u��			true
	bool None(const int botton, const PlayerID player);				//�{�^����������ĂȂ�������	true
};

extern GamePad pad;