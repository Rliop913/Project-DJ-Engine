#include "musicDB.hpp"
// #include "errorTable.hpp"


#define CHK_BIND(res)\
if(res != SQLITE_OK){\
return false;\
}
// errpdje::ereport("sql bind errno: " + std::to_string(SQLITE_LAST_ERRNO), errpdje::ERR_TYPE::SQL_ERROR, ("musicDB bind " + std::string(error_type)));}



musdata::musdata(stmt* dbstate)
{
    auto tempTitle = dbstate->colGet<COL_TYPE::TEXT, std::string>(0);
    auto tempComposer = dbstate->colGet<COL_TYPE::TEXT, std::string>(1);
    auto tempMusicPath= dbstate->colGet<COL_TYPE::TEXT, std::string>(2);
    title = std::u8string(tempTitle.begin(), tempTitle.end());
    composer = std::u8string(tempComposer.begin(), tempComposer.end());
    musicPath =std::u8string(tempMusicPath.begin(), tempMusicPath.end());
    bpm = dbstate->colGet<COL_TYPE::DOUBLE, double>(3);
    bpmBinary = dbstate->colGet<COL_TYPE::BLOB, BIN>(4);
    firstBar = dbstate->colGet<COL_TYPE::TEXT, std::string>(5);
}

musdata::musdata(
    const std::u8string& title__,
    const std::u8string& composer__,
    const std::u8string& musicPath__,
    const double bpm__
):
title(title__),
composer(composer__),
musicPath(musicPath__),
bpm(bpm__)
{}


bool
musdata::GenSearchSTMT(stmt& dbstate, sqlite3* db) 
{
    dbstate.placeHold
    =
    "SELECT * FROM MUSIC"
    " WHERE (? = -1 OR Title = ?)"
    " AND (? = -1 OR Composer = ?)"
    " AND (? = -1 OR MusicPath = ?)"
    " AND (? = -1 OR Bpm = ?)"
    ;
    if(!dbstate.activate(db)){
        return false;
    }
    if(title == u8""){
        CHK_BIND(
        dbstate.bind_int(1, -1)
        )
    }
    if(composer == u8""){
        CHK_BIND(
        dbstate.bind_int(3, -1)
        )
    }
    if(musicPath == u8""){
        CHK_BIND(
        dbstate.bind_int(5, -1)
        )
    }
    if(bpm < 0){
        CHK_BIND(
        dbstate.bind_int(7, -1)
        )
    }
    CHK_BIND( dbstate.bind_u8text(2, title))
    CHK_BIND( dbstate.bind_u8text(4, composer))
    CHK_BIND( dbstate.bind_u8text(6, musicPath))
    CHK_BIND( dbstate.bind_double(8, bpm))
    
    return true;
}

bool
musdata::GenInsertSTMT(stmt& dbstate, sqlite3* db)
{
    dbstate.placeHold
    =
    "INSERT INTO MUSIC "
    "( Title, Composer, MusicPath, Bpm, BpmBinary, FirstBar ) "
    "VALUES "
    "( ?, ?, ?, ?, ?, ?); ";

    if(!dbstate.activate(db)){
        return false;
    }
    CHK_BIND( dbstate.bind_u8text(1, title))
    CHK_BIND( dbstate.bind_u8text(2, composer))
    CHK_BIND( dbstate.bind_u8text(3, musicPath))
    CHK_BIND( dbstate.bind_double(4, bpm))
    CHK_BIND( dbstate.bind_blob(5, bpmBinary))
    CHK_BIND( dbstate.bind_text(6, firstBar))

    return true;

}

bool
musdata::GenEditSTMT(stmt& dbstate, sqlite3* db, musdata& toEdit)
{
    dbstate.placeHold
    =
    "UPDATE MUSIC "
    "SET Title = ?, Composer = ?, MusicPath = ?, Bpm = ?, BpmBinary = ?, FirstBar = ? "
    "WHERE Title = ? AND Composer = ? AND MusicPath = ? AND Bpm = ?; ";

    if(!dbstate.activate(db)) return false;
    
    CHK_BIND(dbstate.bind_u8text   (1, toEdit.title    ))
    CHK_BIND(dbstate.bind_u8text   (2, toEdit.composer ))
    CHK_BIND(dbstate.bind_u8text   (3, toEdit.musicPath))
    CHK_BIND(dbstate.bind_double (4, toEdit.bpm      ))
    CHK_BIND(dbstate.bind_blob   (5, toEdit.bpmBinary))
    CHK_BIND(dbstate.bind_text   (6, toEdit.firstBar ))
    CHK_BIND(dbstate.bind_u8text   (7, title           ))
    CHK_BIND(dbstate.bind_u8text   (8, composer        ))
    CHK_BIND(dbstate.bind_u8text   (9, musicPath       ))
    CHK_BIND(dbstate.bind_double (10,bpm             ))
    
    return true;

}


bool 
musdata::GenDeleteSTMT(stmt& dbstate, sqlite3* db)
{
    dbstate.placeHold
    =
    "DELETE FROM MUSIC "
    "WHERE Title = ? AND Composer = ? AND MusicPath = ? AND Bpm = ?; ";

    if(!dbstate.activate(db)) return false;

    CHK_BIND(dbstate.bind_u8text   (1, title       ))
    CHK_BIND(dbstate.bind_u8text   (2, composer    ))
    CHK_BIND(dbstate.bind_u8text   (3, musicPath   ))
    CHK_BIND(dbstate.bind_double (4, bpm         ))
    
    return true;
}


#undef CHK_BIND