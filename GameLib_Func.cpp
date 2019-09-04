#include "../Header/Sprite.h"
#include "../Header/framework.h"

#include <Shlwapi.h>
#pragma comment( lib, "Shlwapi.lib" )

template <typename T>
T Sign(T &value)
{
    //0��菬�����@���@�}�C�i�X
    if (value < static_cast<T>(0))
    {
        return static_cast<T>(-1);
    }
    //0���傫���@���@�v���X
    else if (value > static_cast<T>(0))
    {
        return static_cast<T>(1);
    }

    return static_cast<T>(0);
}



void Sprite_Load(std::unique_ptr<Sprite> *pSprite, const wchar_t *file_name)
{
    if (pSprite->get())return;

    //null�`�F�b�N
    if (!PathFileExistsW(file_name)) return assert(0 && "Sprite_Load �̈��� file_name �Ŏw�肵���t�@�C����������܂���I");
 /*   assert(!PathFileExists(file_name) && "Sprite_Load �̈��� file_name �Ŏw�肵���t�@�C����������܂���I");*/

    *pSprite = std::make_unique<Sprite>(FRAMEWORK.getDevice(), file_name);
}



void Sprite_Render(std::unique_ptr<Sprite> &pSprite,
    const float &pos_x, const float &pos_y,
    const float &scale_x, const float &scale_y,
    const float &tex_x, const float &tex_y,
    const float &tex_w, const float &tex_h,
    const bool &center_flg, const float &angle,
    const float &red, const float &green, const float &blue, const float &alpha)
{
    //null�`�F�b�N
    assert(pSprite != nullptr && "Sprite_Render�̈��� pSprite �� nullptr �������Ă��܂��I");


    #pragma region size_x, size_y �̒���




    //!? scale�l���Đݒ�
    //�摜��\��g�̃T�C�Y�����߂�
    float size_x = tex_w  * fabsf(scale_x);
    //�؂���T�C�Y���O�ȉ�(�Ӗ��s��)�Ȃ�A�摜�T�C�Y���̂܂�
    if (size_x <= 0.0f)size_x = pSprite->Get_TextureWidth();

    //!? scale�l���Đݒ�
    //�摜��\��g�̃T�C�Y�����߂�
    float size_y = tex_h  * fabsf(scale_y);
    //�؂���T�C�Y���O�ȉ�(�Ӗ��s��)�Ȃ�A�摜�T�C�Y���̂܂�
    if (size_y <= 0.0f)size_y = pSprite->Get_TextureHeight();

#pragma endregion


    #pragma region tex_width, tex_height �̒���




    //!? �؂���摜�T�C�Y���Ē���
    //�摜��\��g�ɓ����摜(�g�ɑS�Ď��܂�悤�ɉ摜�̑傫�����ς��)
    float tex_width = tex_w;
    //�؂���T�C�Y���O�ȉ�(�Ӗ��s��)�Ȃ�A�摜�T�C�Y���̂܂�
    if (tex_width <= 0.0f)tex_width = pSprite->Get_TextureWidth();
    //? �g�Ɏ��߂�Ƃ��ɁA�摜���Ӗ��s���ɂȂ�Ȃ��悤�ɒ���
    tex_width = tex_width * Sign(scale_x);


    //!? �؂���摜�T�C�Y���Ē���
    float tex_height = tex_h;
    //�؂���T�C�Y���O�ȉ�(�Ӗ��s��)�Ȃ�A�摜�T�C�Y���̂܂�
    if (tex_height <= 0.0f)tex_height = pSprite->Get_TextureHeight();
    //? �g�Ɏ��߂�Ƃ��ɁA�摜���Ӗ��s���ɂȂ�Ȃ��悤�ɒ���
    tex_height = tex_height * Sign(scale_y);


#pragma endregion


    #pragma region tex.x, tex.y �̒���



    //!? �؂���J�n�n�_���Ē���
    DirectX::XMFLOAT3 tex;

    tex.x = tex_x;
    //�X�P�[���l���}�C�i�X�̏ꍇ
    if (scale_x < 0.0f)
    {
        //�摜�̉E������X�^�[�g���邽�߁A��������
        tex.x = tex.x + tex_w;
    }

    tex.y = tex_y;
    //�X�P�[���l���}�C�i�X�̏ꍇ
    if (scale_y < 0.0f)
    {
        //�摜�̉E������X�^�[�g���邽�߁A��������
        tex.y = tex.y + tex_h;
    }


#pragma endregion


    #pragma region pos.x, pos.y �̒���




    //!? �|�W�V�������Ē���
    DirectX::XMFLOAT3 pos;
    //���͂������W�𒆐S�ƍl����Ȃ�
    if (center_flg)
    {
        pos.x = pos_x - (size_x * 0.5f);
        pos.y = pos_y - (size_y * 0.5f);
    }
    //���͂������W�𒆐S�ƍl���Ȃ��Ȃ�
    else
    {
        pos.x = pos_x;
        pos.y = pos_y;
    }



#pragma endregion



    pSprite->Render(FRAMEWORK.getContext(),
        pos.x, pos.y,
        size_x, size_y,
        tex.x, tex.y,
        tex_width, tex_height,
        angle,
        red, green, blue, alpha);


}


