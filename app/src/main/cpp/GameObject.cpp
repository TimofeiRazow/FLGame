//
// Created by User on 22.02.2024.
//

#include "GameObject.h"
void GameObject::readObjectFile(AAssetManager *assetManager,const std::string &part) {

    auto obj= AAssetManager_open(assetManager,part.c_str(),AASSET_MODE_BUFFER);
    if(!obj){
        return;
    }



    const char *buf=(const char *) AAsset_getBuffer(obj);

    std::string line;
    GLint lint;
    std::istringstream iss(buf);

    while(std::getline(iss,line)){

        std::string token;
        std::istringstream ss(line);
        ss>>token;

        if(token=="v"){
            float x;
            float y;
            float z;
            ss>>x>>y>>z;
            aout<<x<<" "<<y<<" "<<z;
            pos.emplace_back(Vector3{x,y,z});
        }
        else if(token=="vt"){
            float x;
            float y;
            ss>>x>>y;
            tex.emplace_back(Vector2{x,y});
        }
        else if(token=="vn"){
            float x;
            float y;
            float z;
            ss>>x>>y>>z;
            aout<<x<<" "<<y<<" "<<z;
            normals.emplace_back(Vector3{x,y,z});
        }
        else if(token=="f"){
            int counter=0;
            int num=0;
            while(ss>>lint){
                if(counter==3)
                {
                    const int sp=i_pos.size();
                    const int st=i_tex.size();
                    const int sn=i_norm.size();

                    i_pos.emplace_back(lint - 1);
                    ss.ignore(1,'/');
                    ss>>lint;

                    i_tex.emplace_back(lint - 1);
                    ss.ignore(1,'/');
                    ss>>lint;

                    i_norm.emplace_back(lint - 1);

                    i_pos.emplace_back(i_pos[sp-1]);
                    i_tex.emplace_back(i_tex[st-1]);
                    i_norm.emplace_back(i_norm[sn-1]);

                    i_pos.emplace_back(i_pos[sp-2]);
                    i_tex.emplace_back(i_tex[st-2]);
                    i_norm.emplace_back(i_norm[sn-2]);
                    break;
                }
                if (num == 0) {
                    i_pos.emplace_back(lint - 1);
                } else if (num == 1) {
                    i_tex.emplace_back(lint - 1);
                } else if (num == 2) {
                    i_norm.emplace_back(lint - 1);
                }
                if(ss.peek()=='/'){
                    ++num;
                    ss.ignore(1,'/');
                }else if(ss.peek()==' '){
                    ++counter;
                    aout<<counter;
                    num=0;
                    ss.ignore(1,' ');
                }
            }

        }
    }
    AAsset_close(obj);
}

