#include "PDJE_interface.hpp"

PDJE::PDJE(const fs::path& rootPath)
{
    DBROOT = std::make_shared<litedb>();
    DBROOT->openDB(rootPath);
    
}


TRACK_VEC
PDJE::SearchTrack(const UNSANITIZED& Title)
{
    trackdata td;
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(Title);
    if(!safeTitle){
        return TRACK_VEC();
    }
    td.trackTitle = safeTitle.value();
    auto dbres = (*DBROOT) << td;
    if(dbres.has_value()){
        return dbres.value();
    }
    else{
        return TRACK_VEC();
    }
}


MUS_VEC
PDJE::SearchMusic(
    const UNSANITIZED& Title, 
    const UNSANITIZED& composer, 
    const double bpm)
{
    musdata md;
    auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(Title);
    auto safeComposer = PDJE_Name_Sanitizer::sanitizeFileName(composer);
    if(!safeTitle || !safeComposer){
        return MUS_VEC();
    }
    md.title = safeTitle.value();
    md.composer = safeComposer.value();
    md.bpm = bpm;
    auto dbres = (*DBROOT) << md;
    if(dbres.has_value()){
        return dbres.value();
    }
    else{
        return MUS_VEC();
    }
}

bool
PDJE::InitPlayer(
    PLAY_MODE mode, 
    trackdata& td, 
    const unsigned int FrameBufferSize)
{
    switch (mode)
    {
    case PLAY_MODE::FULL_PRE_RENDER:
        player =
        std::make_shared<audioPlayer>(
            (*DBROOT),
            td,
            FrameBufferSize,
            false
            );
        break;
    case PLAY_MODE::HYBRID_RENDER:
        player =
        std::make_shared<audioPlayer>(
            (*DBROOT),
            td,
            FrameBufferSize,
            true
        );
        break;
    case PLAY_MODE::FULL_MANUAL_RENDER:
        player =
        std::make_shared<audioPlayer>(
            FrameBufferSize
        );
        break;
    
    default:
        break;
    }

    if(!player){
        return false;
    }
    else{
        if(player->STATUS != "OK"){
            return false;
        }
        else{
            return true;
        }
    }
    
}


bool
PDJE::GetNoteObjects(
        trackdata& td,
        OBJ_SETTER_CALLBACK& ObjectSetCallback)
{
    CapReader<NoteBinaryCapnpData> notereader;
    CapReader<MixBinaryCapnpData> mixreader;
    notereader.open(td.noteBinary);
    mixreader.open(td.mixBinary);
    auto noteTrans = new NoteTranslator();
    auto mixTrans = new MixTranslator();
    if(mixTrans->bpms.has_value()){
        noteTrans->Read(
            notereader, 
            mixTrans->bpms.value().bpmVec, 
            ObjectSetCallback);
    }
    else{
        delete noteTrans;
        delete mixTrans;
        return false;
    }
    delete noteTrans;
    delete mixTrans;
    return true;

}


bool
PDJE::InitEditor(
    const DONT_SANITIZE &auth_name, 
    const DONT_SANITIZE &auth_email,
    const fs::path& projectRoot)
{
    editor = std::make_shared<editorObject>(auth_name, auth_email);
    return editor->Open(projectRoot);
}