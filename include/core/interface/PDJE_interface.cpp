#include "PDJE_interface.hpp"
#include "PDJE_LOG_SETTER.hpp"
PDJE::PDJE(const DONT_SANITIZE &rootPath)
{
    startlog();
    DBROOT = std::make_shared<litedb>();
    DBROOT->openDB(rootPath);
}

TRACK_VEC
PDJE::SearchTrack(const UNSANITIZED &Title)
{
    trackdata td;
    auto      safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(Title);
    if (!safeTitle) {
        critlog("failed to make sanitized filename in PDJE SearchTrack. "
                "Errtitle: ");
        critlog(Title);
        return TRACK_VEC();
    }
    td.trackTitle = safeTitle.value();
    auto dbres    = (*DBROOT) << td;
    if (dbres.has_value()) {
        return dbres.value();
    } else {
        warnlog("failed to find trackdata from PDJE database. Errtitle: ");
        warnlog(Title);
        return TRACK_VEC();
    }
}

MUS_VEC
PDJE::SearchMusic(const UNSANITIZED &Title,
                  const UNSANITIZED &composer,
                  const double       bpm)
{
    musdata md;
    auto    safeTitle    = PDJE_Name_Sanitizer::sanitizeFileName(Title);
    auto    safeComposer = PDJE_Name_Sanitizer::sanitizeFileName(composer);
    if (!safeTitle || !safeComposer) {
        critlog("failed to sanitize filename in PDJE SearchMusic. Errs: ");
        critlog(Title);
        critlog(composer);
        return MUS_VEC();
    }
    md.title    = safeTitle.value();
    md.composer = safeComposer.value();
    md.bpm      = bpm;
    auto dbres  = (*DBROOT) << md;
    if (dbres.has_value()) {
        return dbres.value();
    } else {
        warnlog("failed to find music from PDJE database. ErrTitle: ");
        warnlog(Title);
        warnlog(composer);
        return MUS_VEC();
    }
}

bool
PDJE::InitPlayer(PLAY_MODE          mode,
                 trackdata         &td,
                 const unsigned int FrameBufferSize)
{
    switch (mode) {
    case PLAY_MODE::FULL_PRE_RENDER:
        player = std::make_shared<audioPlayer>(
            (*DBROOT), td, FrameBufferSize, false);
        break;
    case PLAY_MODE::HYBRID_RENDER:
        player =
            std::make_shared<audioPlayer>((*DBROOT), td, FrameBufferSize, true);
        break;
    case PLAY_MODE::FULL_MANUAL_RENDER:
        player = std::make_shared<audioPlayer>(FrameBufferSize);
        break;

    default:
        break;
    }

    if (!player) {
        critlog("failed to init player on PDJE initPlayer.");
        return false;
    } else {
        if (player->STATUS != "OK") {
            critlog("PDJE initPlayer failed. STATUS not OK. ErrStatus: ");
            critlog(player->STATUS);
            return false;
        } else {
            return true;
        }
    }
}

bool
PDJE::GetNoteObjects(trackdata &td, OBJ_SETTER_CALLBACK &ObjectSetCallback)
{
    CapReader<NoteBinaryCapnpData> notereader;
    CapReader<MixBinaryCapnpData>  mixreader;

    if (!notereader.open(td.noteBinary)) {
        warnlog("failed to read notebinary from trackdata from PDJE "
                "GetNoteObjects");
        return false;
    }
    if (!mixreader.open(td.mixBinary)) {
        warnlog(
            "failed to read mixBinary from trackdata from PDJE GetNoteObjects");
        return false;
    }

    auto noteTrans = new NoteTranslator();
    auto mixTrans  = new MixTranslator();

    mixTrans->Read(mixreader);

    if (mixTrans->bpms.has_value()) {
        noteTrans->Read(
            notereader, mixTrans->bpms.value().bpmVec, ObjectSetCallback);
    } else {
        critlog("failed to emplace optional object from PDJE GetNoteObjects");
        delete noteTrans;
        delete mixTrans;
        return false;
    }
    delete noteTrans;
    delete mixTrans;
    return true;
}

bool
PDJE::InitEditor(const DONT_SANITIZE &auth_name,
                 const DONT_SANITIZE &auth_email,
                 const DONT_SANITIZE &projectRoot)
{
    editor = std::make_shared<editorObject>(auth_name, auth_email);
    return editor->Open(projectRoot);
}

PDJE_CORE_DATA_LINE
PDJE::PullOutDataLine()
{
    if (player) {
        return player->PullOutDataLine();
    } else {
        PDJE_CORE_DATA_LINE errline;
        return errline;
    }
}