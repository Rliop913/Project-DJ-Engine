#include "PDJE_C_EXPORT.hpp"
#include "PDJE_interface.hpp"
#include <functional>
struct PDJE_C {
    PDJE *core = nullptr;
};

struct PDJE_TRACKDATA {
    trackdata *data;
};

struct PDJE_COREDL {
    PDJE_CORE_DATA_LINE *line;
};

struct PDJE_MUSVEC {
    MUS_VEC *vec;
};

struct PDJE_TRACKVEC {
    TRACK_VEC *vec;
};

struct PDJE_AUDIOPLAYER {
    std::shared_ptr<audioPlayer> ap;
};

struct PDJE_EDITOR_OBJ {
    std::shared_ptr<editorObject> ep;
};

static int
map_exception_to_code() noexcept
{
    try {
        throw;
    } catch (const std::bad_alloc &) {
        return PDJE_NO_MEMORY;
    } catch (const std::exception &e) {
        critlog(e.what());
        return PDJE_INTERNAL;
    }
}

extern "C" {

PDJE_API int PDJE_CALL
PDJE_CREATE(const char *dbRoot, PDJE_C_HANDLE **out)
{
    startlog();
    if (!out)
        return PDJE_INVALID_ARG;
    *out = nullptr;

    try {
        PDJE_C *h = new (std::nothrow) PDJE_C{};
        if (!h)
            return PDJE_NO_MEMORY;

        h->core = new (std::nothrow) PDJE(std::string(dbRoot));
        if (!h->core) {
            delete h;
            return PDJE_NO_MEMORY;
        }
        *out = h;
        return PDJE_OK;
    } catch (...) {
        return map_exception_to_code();
    }
}

PDJE_API void PDJE_CALL
PDJE_DESTROY(PDJE_C_HANDLE *h)
{
    if (!h)
        return;
    try {
        delete h->core;
        h->core = nullptr;
        delete h;
    } catch (...) {
        ;
    }
}

PDJE_API void PDJE_CALL
PDJE_ResetPlayer(PDJE_C_HANDLE *h)
{
    if (!h)
        return;
    try {
        h->core->player.reset();
    } catch (...) {
        map_exception_to_code();
        return;
    }
}
PDJE_API void PDJE_CALL
PDJE_CloseEditor(PDJE_C_HANDLE *h)
{
    if (!h)
        return;
    try {
        h->core->editor.reset();
    } catch (...) {
        map_exception_to_code();
        return;
    }
}

PDJE_API C_PDJE_CORE_DATA_LINE PDJE_CALL
PDJE_PullOutDataLine(PDJE_C_HANDLE *h)
{
    if (!h)
        return C_PDJE_CORE_DATA_LINE{};
    try {
        C_PDJE_CORE_DATA_LINE dl;
        dl.line = &h->core->PullOutDataLine();
        return dl;
    } catch (...) {
        map_exception_to_code();
        return C_PDJE_CORE_DATA_LINE{};
    }
}

PDJE_API int PDJE_CALL
PDJE_InitEditor(PDJE_C_HANDLE *h,
                const char    *auth_name,
                const char    *auth_email,
                const char    *projectRoot)
{
    if (!h)
        return PDJE_INVALID_ARG;
    try {
        bool res = h->core->InitEditor(std::string(auth_name),
                                       std::string(auth_email),
                                       std::string(projectRoot));
        return res ? PDJE_OK : PDJE_INVALID_ARG;
    } catch (...) {
        return map_exception_to_code();
    }
}

PDJE_API int PDJE_CALL
PDJE_GetNoteObjects(PDJE_C_HANDLE    *h,
                    C_TRACKDATA      *td,
                    PDJE_NoteCallback object_set_callback)
{
    if (!h)
        return PDJE_INVALID_ARG;
    try {
        OBJ_SETTER_CALLBACK lambda = [&](const std::string        noteType,
                                         const uint16_t           note_Detail,
                                         const std::string        firstArg,
                                         const std::string        secondArg,
                                         const std::string        thirdArg,
                                         const unsigned long long Y_axis,
                                         const unsigned long long Y_axis2,
                                         const uint64_t           rail_id) {
            object_set_callback(noteType.c_str(),
                                note_Detail,
                                firstArg.c_str(),
                                secondArg.c_str(),
                                thirdArg.c_str(),
                                Y_axis,
                                Y_axis2,
                                rail_id);
        };

        bool res = h->core->GetNoteObjects(*(td->data), lambda);
        return res ? PDJE_OK : PDJE_INVALID_ARG;
    } catch (...) {
        return map_exception_to_code();
    }
}

PDJE_API C_MUSVEC PDJE_CALL
PDJE_SearchMusic(PDJE_C_HANDLE *h,
                 const char    *Title,
                 const char    *composer,
                 const double   bpm)
{
    if (!h)
        return C_MUSVEC{};
    try {
        C_MUSVEC res;

        res.vec = &h->core->SearchMusic(
            std::string(Title), std::string(composer), bpm);
        return res;
    } catch (...) {
        map_exception_to_code();
        return C_MUSVEC{};
    }
}

PDJE_API C_TRACKVEC PDJE_CALL
PDJE_SearchTrack(PDJE_C_HANDLE *h, const char *Title)
{
    if (!h)
        return C_TRACKVEC{};
    try {
        C_TRACKVEC res;
        res.vec = &h->core->SearchTrack(std::string(Title));
        return res;
    } catch (...) {
        map_exception_to_code();
        return C_TRACKVEC{};
    }
}

PDJE_API C_audioPlayer PDJE_CALL
PDJE_GetPlayerObject(PDJE_C_HANDLE *h)
{
    if (!h)
        return C_audioPlayer{};
    try {
        C_audioPlayer res;
        res.ap = h->core->GetPlayerObject();
        return res;
    } catch (...) {
        map_exception_to_code();
        return C_audioPlayer{};
    }
}

PDJE_API C_editorObject PDJE_CALL
PDJE_GetEditorObject(PDJE_C_HANDLE *h)
{
    if (!h)
        return C_editorObject{};
    try {
        C_editorObject res;
        res.ep = h->core->GetEditorObject();
        return res;
    } catch (...) {
        map_exception_to_code();
        return C_editorObject{};
    }
}
};