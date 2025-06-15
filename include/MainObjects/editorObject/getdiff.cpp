#include "editorObject.hpp"

template<>
PDJE_API
DiffResult
editorObject::GetDiff<EDIT_ARG_MIX>(const gitwrap::commit& oldTimeStamp, 
                                    const gitwrap::commit& newTimeStamp)
{
    return std::move(E_obj->mixHandle.first->GetDiff(oldTimeStamp, newTimeStamp));
}

template<>
PDJE_API
DiffResult
editorObject::GetDiff<EDIT_ARG_NOTE>(const gitwrap::commit& oldTimeStamp, 
                                    const gitwrap::commit& newTimeStamp)
{
    return std::move(E_obj->noteHandle.first->GetDiff(oldTimeStamp, newTimeStamp));
}

template<>
PDJE_API
DiffResult
editorObject::GetDiff<EDIT_ARG_KEY_VALUE>(const gitwrap::commit& oldTimeStamp, 
                                    const gitwrap::commit& newTimeStamp)
{
    return std::move(E_obj->KVHandler.first->GetDiff(oldTimeStamp, newTimeStamp));
}

template<>
PDJE_API
DiffResult
editorObject::GetDiff<EDIT_ARG_MUSIC>(const gitwrap::commit& oldTimeStamp, 
                                    const gitwrap::commit& newTimeStamp)
{
    for(auto& i : E_obj->musicHandle){
        auto restemp = i.gith.GetDiff(oldTimeStamp, newTimeStamp);
        if(!restemp.NewLines.empty() || !restemp.OldLines.empty()){
            return std::move(restemp);
        }
    }
    return DiffResult();
}


