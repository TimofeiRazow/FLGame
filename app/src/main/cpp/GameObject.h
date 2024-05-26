//
// Created by User on 22.02.2024.
//

#ifndef TRY9_GAMEOBJECT_H
#define TRY9_GAMEOBJECT_H

#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/imagedecoder.h>

#include <assert.h>
#include "AndroidOut.h"
#include "vector"
#include "TextureAsset.h"

union Vector3{
    struct{
        float x,y,z;
    };
    float ind[3];

};
union Vector2{
    struct{
        float x,y;
    };
    struct{
        float u,v;
    };
    float ind[2];
};
class GameObject {
public:
    inline GameObject(std::vector<Vector3> vectors,
                      std::vector<Vector2> UV,
                      std::vector<Vector3> norms,
                      std::vector<std::uint32_t> faces,std::shared_ptr<TextureAsset> asset)
                      :pos(std::move(vectors)),
                      tex(std::move(UV)),
                      normals(std::move(norms)),
                      i_pos(std::move(faces)),texture(std::move(asset)){}
    inline GameObject(AAssetManager *assetManager,const std::string &assetPath,std::shared_ptr<TextureAsset> asset)
            :texture(std::move(asset)){
        readObjectFile(assetManager,assetPath);
    }
    inline GameObject(){}
    inline const Vector3 * getVertex() const {
        return pos.data();
    }
    inline const uint32_t *getIndexPos() const{
        return i_pos.data();
    }
    inline const uint32_t getIndexPosCount() const{
        return i_pos.size();
    }
    inline const Vector2 * getVertexUV() const{
        return tex.data();
    };
    inline const TextureAsset &getTexture() const {
        return *texture;
    }
    inline const void setTexture(std::shared_ptr<TextureAsset> &asset) {
        texture=std::move(asset);
    }
    void readObjectFile(AAssetManager *assetManager,const std::string &path);
private:
    std::shared_ptr<TextureAsset> texture;
    std::vector<Vector3> pos;
    std::vector<Vector2> tex;
    std::vector<Vector3> normals;
    std::vector<std::uint32_t> i_pos;
    std::vector<std::uint32_t> i_tex;
    std::vector<std::uint32_t> i_norm;
};


#endif
