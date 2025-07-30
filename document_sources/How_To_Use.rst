
HOW TO USE
================

.. contents:: how to use

Initialize PDJE Engine
----------------------
.. doxygenclass:: PDJE
    :project: Project_DJ_Engine

we use "PDJE" class as an interface.

.. code-block:: cpp

    auto engine = PDJE("path/to/root/sqlite/database.sql");

this code Initialize engine and interfaces.

Search Track and Music
-----------------------
.. doxygenfunction:: PDJE::SearchMusic
    :project: Project_DJ_Engine

.. code-block:: cpp

    double bpm = 140.0;
    auto music_searched = engine->SearchMusic("Music Title", "Composer name", bpm);
    if(music_searched.empty()){
        std::cout << "can't find anything" << std::endl;
        return -1;
    }
    for(auto i : music_searched){
        std::cout 
        << "title: " 
        << i.title 
        << "path: "
        << i.musicPath 
        << std::endl;
    }

.. doxygenfunction:: PDJE::SearchTrack
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto trackSearch = engine->SearchTrack("Track Title");
    if(trackSearch.empty()){
        std::cout << "can't find anything" << std::endl;
        return -1;
    }
    for(auto i: trackSearch){
        std::cout 
        << " track title: "
        << i.trackTitle
        << " note binary size: "
        << i.noteBinary.size()
        << " mix binary size: "
        << i.mixBinary.size()
        << std::endl;
    }


Initialize Player
-------------------

.. doxygenfunction:: PDJE::InitPlayer
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto Track_Chosen = trackSearch[0];
    int Buffer_Size = 48;
    engine->InitPlayer(PLAY_MODE::FULL_PRE_RENDER, Track_Chosen, Buffer_Size);
    if(!engine->player){
        std::cout << "can't use track" << std::endl;
        return -1;
    }


Handle Player
---------------

.. doxygenclass:: audioPlayer
    :project: Project_DJ_Engine

this is the handler of musics.

.. doxygenfunction:: audioPlayer::Activate
    :project: Project_DJ_Engine

.. code-block:: cpp

    engine->player->Activate();


.. doxygenfunction:: audioPlayer::Deactivate
    :project: Project_DJ_Engine

.. code-block:: cpp

    engine->player->Deactivate();

.. doxygenfunction:: audioPlayer::GetFXControlPannel
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto FXController = engine->player->GetFXControlPannel();

.. doxygenfunction:: audioPlayer::GetMusicControlPannel
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto MusicController = engine->player->GetMusicControlPannel();


Handle FXControlPannel
-----------------------

.. doxygenfunction:: FXControlPannel::FX_ON_OFF
    :project: Project_DJ_Engine

.. code-block:: cpp

    bool TurnON = true;
    FXController->FX_ON_OFF(FXList::ChooseFX, TurnON);

.. doxygenfunction:: FXControlPannel::GetArgSetter
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto argHandler = FXController->GetArgSetter(FXList::ChooseFX);
    for(auto i : argHandler){
        std::cout
        << "FX key: "
        << i.first
        << std::endl;
    }
    double FXValue = 2.0;
    argHandler["FX key name"](FXValue);ß


.. doxygenfunction:: FXControlPannel::checkSomethingOn
    :project: Project_DJ_Engine

.. code-block:: cpp

    if(FXController->checkSomethingOn()){
        std::cout<< "FX is turned on" << std::endl;
    }


Handle MusicControlPannel
--------------------------

.. doxygenclass:: MusicControlPannel
    :project: Project_DJ_Engine


.. doxygenfunction:: MusicControlPannel::LoadMusic
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto MUSIC_TO_LOAD = music_searched[0];
    MusicController->LoadMusic(*engine->DBROOT, MUSIC_TO_LOAD);



.. doxygenfunction:: MusicControlPannel::CueMusic
    :project: Project_DJ_Engine

.. code-block:: cpp

    unsigned long long music_position = 0;
    MusicController->CueMusic("MUSIC_TITLE", music_position);


.. doxygenfunction:: MusicControlPannel::SetMusic
    :project: Project_DJ_Engine

.. code-block:: cpp

    bool ON = true;
    MusicController->SetMusic("MUSIC_TITLE", ON);

.. doxygenfunction:: MusicControlPannel::GetLoadedMusicList
    :project: Project_DJ_Engine

.. code-block:: cpp

    MusicController->GetLoadedMusicList();

.. doxygenfunction:: MusicControlPannel::UnloadMusic
    :project: Project_DJ_Engine

.. code-block:: cpp

    MusicController->UnloadMusic("MUSIC_TITLE");


.. doxygenfunction:: MusicControlPannel::getFXHandle
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto fxHandler = MusicController->getFXHandle("MUSIC_TITLE");

Handle Editor
--------------------------

.. doxygenfunction:: PDJE::InitEditor
    :project: Project_DJ_Engine

.. code-block:: cpp

    engine->InitEditor("editor's name", "editor's email. no need to fill", "Editor Sandbox Directory");


.. doxygenfunction:: editorObject::Open
    :project: Project_DJ_Engine

.. code-block:: cpp

    engine->editor->Open("Editor Sandbox Directory");

This code opens the editor project.
if the path is empty, generates new one.

this will be called from PDJE::InitEditor. no need to call twice.

.. doxygenfunction:: editorObject::DESTROY_PROJECT
    :project: Project_DJ_Engine

.. code-block:: cpp

    engine->editor->DESTROY_PROJECT();

This removes the opened project.
use careful.

template<typename EDIT_ARG_TYPE>
AddLine<EDIT_ARG_TYPE>(const EDIT_ARG_TYPE& obj)

and

AddLine(const std::string& musicName, const std::string& firstBar)

.. code-block:: cpp

    engine->editor->AddLine<EDIT_ARG_NOTE>(noteObj);
    engine->editor->AddLine<EDIT_ARG_KEY_VALUE>(key_value_Obj);
    engine->editor->AddLine<EDIT_ARG_MIX>(mixObj);
    engine->editor->AddLine<EDIT_ARG_MUSIC>(musicObj);
    engine->editor->AddLine("music name", "48000");
    

this code adds data into the editor project.
every add command calls save function, and will be recorded by git.

to see what is that datas, go to :doc:`pdje format page <PDJE_Format>`

