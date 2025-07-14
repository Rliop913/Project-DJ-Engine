#include "editorObject.hpp"

int 
editorObject::deleteLine(   const EDIT_ARG_MIX& obj,
                            bool skipType, 
                            bool skipDetail)
{
    const int deleted_lines =
    E_obj->mixHandle.second.deleteLine(
        obj,
        skipType,
        skipDetail
    );
    if(DefaultSaveFuntion<EDIT_ARG_MIX>()){
        return deleted_lines;
    }
    else{
        infolog("nothing to remove. from note deleteLine");
        return 0;
    }
}


template<>
PDJE_API
int
editorObject::deleteLine(const EDIT_ARG_NOTE& obj)
{
    const int deleted_lines = 
    E_obj->noteHandle.second.deleteLine(obj);
    if(DefaultSaveFuntion<EDIT_ARG_NOTE>()){
        return deleted_lines;
    }
    else{
        infolog("nothing to remove. from note deleteLine");
        return 0;
    }
}

template<>
PDJE_API
int
editorObject::deleteLine(const EDIT_ARG_KEY_VALUE& obj)
{
    const int deleted_lines =
    E_obj->KVHandler.second.deleteLine(obj.second);
    if(DefaultSaveFuntion<EDIT_ARG_KEY_VALUE>()){
        return deleted_lines;
    }
    else{
        infolog("nothing to remove. from KV deleteLine");
        return 0;
    }
}

template<>
PDJE_API
int
editorObject::deleteLine(const EDIT_ARG_MUSIC& obj)
{
    for(auto& i : E_obj->musicHandle){
        if(i.musicName == obj.musicName){
            const int deleted_lines = i.jsonh.deleteLine(obj.arg);
            if(DefaultSaveFuntion<EDIT_ARG_MUSIC>(i, obj)){
                return deleted_lines;
            }
            else{
                infolog("nothing to remove. from music deleteLine");
                return 0;
            }
        }
    }
    warnlog("music is not exists. from editorObject deleteLine(Music obj)");
    return 0;
}
