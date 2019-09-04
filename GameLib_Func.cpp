#include "../Header/Sprite.h"
#include "../Header/framework.h"

#include <Shlwapi.h>
#pragma comment( lib, "Shlwapi.lib" )

template <typename T>
T Sign(T &value)
{
    //0より小さい　＝　マイナス
    if (value < static_cast<T>(0))
    {
        return static_cast<T>(-1);
    }
    //0より大きい　＝　プラス
    else if (value > static_cast<T>(0))
    {
        return static_cast<T>(1);
    }

    return static_cast<T>(0);
}



void Sprite_Load(std::unique_ptr<Sprite> *pSprite, const wchar_t *file_name)
{
    if (pSprite->get())return;

    //nullチェック
    if (!PathFileExistsW(file_name)) return assert(0 && "Sprite_Load の引数 file_name で指定したファイルが見つかりません！");
 /*   assert(!PathFileExists(file_name) && "Sprite_Load の引数 file_name で指定したファイルが見つかりません！");*/

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
    //nullチェック
    assert(pSprite != nullptr && "Sprite_Renderの引数 pSprite に nullptr が入っています！");


    #pragma region size_x, size_y の調整




    //!? scale値を再設定
    //画像を貼る枠のサイズを決める
    float size_x = tex_w  * fabsf(scale_x);
    //切り取りサイズが０以下(意味不明)なら、画像サイズそのまま
    if (size_x <= 0.0f)size_x = pSprite->Get_TextureWidth();

    //!? scale値を再設定
    //画像を貼る枠のサイズを決める
    float size_y = tex_h  * fabsf(scale_y);
    //切り取りサイズが０以下(意味不明)なら、画像サイズそのまま
    if (size_y <= 0.0f)size_y = pSprite->Get_TextureHeight();

#pragma endregion


    #pragma region tex_width, tex_height の調整




    //!? 切り取る画像サイズを再調整
    //画像を貼る枠に入れる画像(枠に全て収まるように画像の大きさが変わる)
    float tex_width = tex_w;
    //切り取りサイズが０以下(意味不明)なら、画像サイズそのまま
    if (tex_width <= 0.0f)tex_width = pSprite->Get_TextureWidth();
    //? 枠に収めるときに、画像が意味不明にならないように調整
    tex_width = tex_width * Sign(scale_x);


    //!? 切り取る画像サイズを再調整
    float tex_height = tex_h;
    //切り取りサイズが０以下(意味不明)なら、画像サイズそのまま
    if (tex_height <= 0.0f)tex_height = pSprite->Get_TextureHeight();
    //? 枠に収めるときに、画像が意味不明にならないように調整
    tex_height = tex_height * Sign(scale_y);


#pragma endregion


    #pragma region tex.x, tex.y の調整



    //!? 切り取り開始地点を再調整
    DirectX::XMFLOAT3 tex;

    tex.x = tex_x;
    //スケール値がマイナスの場合
    if (scale_x < 0.0f)
    {
        //画像の右側からスタートするため、調整する
        tex.x = tex.x + tex_w;
    }

    tex.y = tex_y;
    //スケール値がマイナスの場合
    if (scale_y < 0.0f)
    {
        //画像の右側からスタートするため、調整する
        tex.y = tex.y + tex_h;
    }


#pragma endregion


    #pragma region pos.x, pos.y の調整




    //!? ポジションを再調整
    DirectX::XMFLOAT3 pos;
    //入力した座標を中心と考えるなら
    if (center_flg)
    {
        pos.x = pos_x - (size_x * 0.5f);
        pos.y = pos_y - (size_y * 0.5f);
    }
    //入力した座標を中心と考えないなら
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


