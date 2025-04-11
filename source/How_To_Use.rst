
HOW TO USE
================

.. contents:: how to use

Initialize PDJE Engine
----------------------
.. doxygenclass:: PDJE
    :project: Project_DJ_Engine

we use "PDJE" class as an interface.

.. code-block:: cpp

    PDJE engine = new PDJE("path/to/root/sqlite/database.sql");

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
    engine->InitPlayer(PLAY_MODE::RENDER_MODE, Track_Chosen, Buffer_Size);
    if(!engine->player.has_value()){
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

    engine->Activate();


.. doxygenfunction:: audioPlayer::Deactivate
    :project: Project_DJ_Engine

.. code-block:: cpp

    engine->Deactivate();

.. doxygenfunction:: audioPlayer::GetFXControlPannel
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto FXController = engine->GetFXControlPannel();

.. doxygenfunction:: audioPlayer::GetMusicControlPannel
    :project: Project_DJ_Engine

.. code-block:: cpp

    auto MusicController = engine->GetMusicControlPannel();


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
    argHandler["FX key name"](FXValue);


.. doxygenfunction:: FXControlPannel::checkSomethingOn
    :project: Project_DJ_Engine

.. code-block:: cpp

    if(FXController->checkSomethingOn()){
        std::cout<< "FX is turned on" << std::endl;
    }


Handle MusicControlPannel
--------------------------

need more documentation

.. doxygenclass:: MusicControlPannel
    :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPannel::LoadMusic
    :project: Project_DJ_Engine