#pragma once
#include "PDJE_EXPORT_SETTER.hpp"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct PDJE_C           PDJE_C_HANDLE;
typedef struct PDJE_TRACKDATA   C_TRACKDATA;
typedef struct PDJE_COREDL      C_PDJE_CORE_DATA_LINE;
typedef struct PDJE_MUSVEC      C_MUSVEC;
typedef struct PDJE_TRACKVEC    C_TRACKVEC;
typedef struct PDJE_AUDIOPLAYER C_audioPlayer;
typedef struct PDJE_EDITOR_OBJ  C_editorObject;
typedef void(PDJE_CALL *PDJE_NoteCallback)(const char        *noteType,
                                           uint16_t           noteDetail,
                                           const char        *firstArg,
                                           const char        *secondArg,
                                           const char        *thirdArg,
                                           unsigned long long yPos,
                                           unsigned long long yPos2,
                                           uint64_t           railId);

enum {
    PDJE_OK          = 0,
    PDJE_INVALID_ARG = -1,
    PDJE_NO_MEMORY   = -2,
    PDJE_INTERNAL    = -3
};

PDJE_API int PDJE_CALL
PDJE_CREATE(const char *dbRoot, PDJE_C_HANDLE **out);
PDJE_API int PDJE_CALL
PDJE_InitPlayer(const PDJE_C_HANDLE *h,
                const int            mode,
                C_TRACKDATA        **td,
                const unsigned int   FrameBufferSize);

PDJE_API void PDJE_CALL
PDJE_DESTROY(PDJE_C_HANDLE *h);
PDJE_API void PDJE_CALL
PDJE_ResetPlayer(PDJE_C_HANDLE *h);

PDJE_API void PDJE_CALL
PDJE_CloseEditor(PDJE_C_HANDLE *h);

PDJE_API C_PDJE_CORE_DATA_LINE PDJE_CALL
PDJE_PullOutDataLine(PDJE_C_HANDLE *h);

PDJE_API int PDJE_CALL
PDJE_InitEditor(PDJE_C_HANDLE *h,
                const char    *auth_name,
                const char    *auth_email,
                const char    *projectRoot);

PDJE_API int PDJE_CALL
PDJE_GetNoteObjects(PDJE_C_HANDLE    *h,
                    C_TRACKDATA      *td,
                    PDJE_NoteCallback object_set_callback);

PDJE_API C_MUSVEC PDJE_CALL
PDJE_SearchMusic(PDJE_C_HANDLE *h,
                 const char    *Title,
                 const char    *composer,
                 const double   bpm);

PDJE_API C_TRACKVEC PDJE_CALL
PDJE_SearchTrack(PDJE_C_HANDLE *h, const char *Title);

PDJE_API C_audioPlayer PDJE_CALL
PDJE_GetPlayerObject(PDJE_C_HANDLE *h);

PDJE_API C_editorObject PDJE_CALL
PDJE_GetEditorObject(PDJE_C_HANDLE *h);

#ifdef __cplusplus
}
#endif