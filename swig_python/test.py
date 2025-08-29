import pdje_POLYGLOT as pypdje

from pdje_POLYGLOT import EDIT_ARG_MIX
from pdje_POLYGLOT import EDIT_ARG_MUSIC
from pdje_POLYGLOT import EDIT_ARG_KEY_VALUE

from pdje_POLYGLOT import EDIT_ARG_NOTE
from pdje_POLYGLOT import editorObject

engine:pypdje.PDJE = pypdje.PDJE("dbroot")
engine.InitEditor("","","")
editor:editorObject = engine.GetEditorObject()

mixs = EDIT_ARG_MIX()

editor.AddLineMix(mixs)

pypdje.TypeEnum_BATTLE_DJ


mixs.type = pypdje.TypeEnum_FILTER #Filter
mixs.details = pypdje.DetailEnum_LOW #Low pass

mixs.ID = 1 #Deck number. access music with this.

mixs.first = pypdje.ITPL_COSINE.ToString() # first arg
mixs.second = "5000,1000,2000,3000,4000,5000,5500,6000" # second arg, eight point values
# mixs.first = pypdje.ITPL_FLAT.ToString() # if no need interpolation
# mixs.second = "5000" # just one value

mixs.third = "NONE" # third arg

mixs.beat = 0
mixs.subBeat = 0
mixs.separate = 0
#"start_position" = beat + (beat / separate) * subBeat

mixs.Ebeat = 16#end beat
mixs.EsubBeat = 2#end subBeat
mixs.Eseparate = 4#end separate
#"end_position" = ebeat + (ebeat / eseparate) * esubBeat

#summation: add low pass filter from "start_position" to "end_position" with interpolation

editor.AddLineMix(mixs)#add mix data
# data = EDIT_ARG_NOTE()
# editor.AddLineNote(data)

# data = EDIT_ARG_KEY_VALUE()
# editor.AddLineKV(data)

# data = EDIT_ARG_MUSIC()
# editor.AddLineMusic(data)

# editor.AddLine("music name", "48000") # this changes the music's first beat position.

class MixCall(pypdje.MixVisitor):
    def __init__(self):
        super().__init__()
        self.mixs = []

    def on_item(self, o:EDIT_ARG_MIX):
        if o.beat > 50:
            self.mixs.append(o)

caller = MixCall()

editor.GetAllMixes(caller)

skip_type = False
skip_detail = True

for i in caller.mixs:
    editor.deleteLine(i, skip_type, skip_detail)
    # editor.deleteLineKV(i)
    # editor.deleteLineMusic(i)
    # editor.deleteLineNote(i)

