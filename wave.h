#pragma once

#include	<directxmath.h>
#include	"..\Header\Mesh.h"

/*******************************************************************************
TODO:01 WAVE�t���O���ݓG�ݒ�\���̂��`����
����    �P�Ȃ�G�o�����ł͂Ȃ��AWAVE�̉��Ԗڂɏo�����邩�̏�����������
--------------------------------------------------------------------------------
�菇    �ȉ��̃����o�����\���̂��`����
		�Eint		�ԍ�(��WAVE�ڂɏo�����邩�̊Ǘ��ԍ�)
		�Eint		���f���ԍ�(�ǂ̃��f�����g�p���邩�̊Ǘ��ԍ�)
		�Efloat		���W(X,Z�̂݁A�����Y�l���ꗥ0�Ƃ��ĊǗ�����\��)
		�Efloat		��]�p�x
		�EXMFLOAT4	�F(3�F+�s�����x)
*******************************************************************************/
struct Wave_Enemy
{
    //��WAVE�ڂɏo�����邩�̊Ǘ��ԍ�
    int wave_number;
    //���f���ԍ�
    int model_number;
    //���W
    float x;
    float z;
    //��]�p�x
    float rotation;
    //�F
    DirectX::XMFLOAT4 color;

};











/*******************************************************************************
TODO:02 WAVE�Ǘ��N���X���`����
����    �G�o��(WAVE)�Ǘ��N���X���쐬����
		�G�o���f�[�^�����łȂ��A1�X�e�[�W���Ŏg�p����G���f��(�̃I���W�i��)��
		�S�Ă��̃N���X���Ǘ�����`�Ő݌v����
--------------------------------------------------------------------------------
�菇    �ȉ��̃����o�A���\�b�h�����\���̂��`����
		�Econst int		���f���ő吔(�I���W�i�����f���̍ő吔�A�����l:10)
		�EMyMesh		�I���W�i�����f���z��(�v�f���F���f���ő吔)
		�E�G�ݒ�\����*	�G�o���f�[�^(�|�C���^�̂�)
		�Eint			�J�E���^(�o���f�[�^�̉����܂ŏo�����Ă��邩���L�^����ׂ̕ϐ�)
		�Eint			��WAVE�ԍ�(��WAVE�܂œG���o�Ă��邩���Ǘ�)

		�Evoid	�������֐�(������z��, �G�ݒ�\���̔z��)
		�Evoid	����֐�()
		�Ebool	�����֐�()
		�Evoid	WAVE�i�s�֐�()	{ ��WAVE�ԍ���1���₷; }
		
	���K�v�ɉ����ăV���O���g�����A���̑������o�̒ǉ������Ȃ���
*******************************************************************************/
class Enemy_WaveManager
{
private:
    //���݂̃E�F�[�u�m�F
    int next_stage = 0;
    //���f���ő吔
    static const int model_max = 10;
   //�I���W�i�����f���z��
    MyMesh  model[model_max];
   
    //�G�o���f�[�^�̉����܂ŏo�����Ă��邩���L�^����ׂ̕ϐ�
    int counter;
    //��WAVE�ԍ�
    int now_wave_number;

public:
    //�G�o���f�[�^
    Wave_Enemy *enemy;

    void Initialize(char *data[], Wave_Enemy w_enemy[], const bool &is_init = true);
    void Release();
    
    bool Add();
    void Move_Wave() { now_wave_number++; }

    void NextStage()
    {
        next_stage++;
    }

public:
    static Enemy_WaveManager *instance;

private:
    Enemy_WaveManager(){}
    ~Enemy_WaveManager(){}

public:

    static  Enemy_WaveManager *Get_Instance()
    {
        if (instance == nullptr)
        {
            instance = new Enemy_WaveManager();
        }
        return instance;
    }


    static void Delete_Instance()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }


};

#define enemy_waveManager (Enemy_WaveManager::Get_Instance())


extern char*	modelNames01[];
extern Wave_Enemy waveData[][32];































