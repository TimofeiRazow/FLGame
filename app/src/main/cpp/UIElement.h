//
// Created by User on 03.03.2024.
//

#ifndef FL_UIELEMENT_H
#define FL_UIELEMENT_H
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <GLES3/gl3.h>
#include "TextureAsset.h"
#include "GameObject.h"

class UIElement {
public:
    inline UIElement(std::vector<Vector3> pos,
                     std::vector<Vector2> uv,
                     std::vector<std::uint32_t> indicses, std::shared_ptr<TextureAsset> asset):

                     pos_(std::move(pos)),
                     tex(std::move(uv)),
                     i_pos(std::move(indicses)),
                     texture(std::move(asset)){};
    inline const Vector3 * getVertexPosition() const{
        return pos_.data();
    };
    inline const Vector2 * getVertexUV() const{
        return tex.data();
    };
    inline const std::uint32_t *getIndexPos() const{
        return i_pos.data();
    }
    inline const GLint getIndexPosCount() const{
        return i_pos.size();
    }
    inline const TextureAsset &getTexture() const {
        return *texture;
    }
private:
std::shared_ptr<TextureAsset> texture;
std::vector<Vector3> pos_;
std::vector<Vector2> tex;
std::vector<std::uint32_t> i_pos;
std::vector<std::uint32_t> i_tex;
};


#endif //FL_UIELEMENT_H
