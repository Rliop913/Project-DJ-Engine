import pdje_POLYGLOT as pyPDJE
from pdje_POLYGLOT import MusicControlPannel
from pdje_POLYGLOT import FXControlPannel
from pdje_POLYGLOT import audioPlayer
from pdje_POLYGLOT import MUS_VEC
from pdje_POLYGLOT import editorObject

engine = pyPDJE.PDJE("./dblocation")

AudioP:audioPlayer = engine.GetPlayerObject()

mus:MUS_VEC = engine.SearchMusic("", "")

muspannel:MusicControlPannel = AudioP.GetMusicControlPannel()

muspannel.LoadMusic(engine.DBROOT, mus[0])

muspannel.CueMusic()


muspannel.GetLoadedMusicList()

fxhandle:FXControlPannel = muspannel.getFXHandle("title")

init_res = engine.InitEditor("", "", "")
editor:editorObject = engine.GetEditorObject()
destroy_res = editor.DESTROY_PROJECT()
engine.CloseEditor()