import pdje_POLYGLOT
from pdje_POLYGLOT import TRACK_VEC
from pdje_POLYGLOT import MUS_VEC
from pdje_POLYGLOT import trackdata
from pdje_POLYGLOT import audioPlayer
from pdje_POLYGLOT import FXControlPannel
from pdje_POLYGLOT import ARGSETTER_WRAPPER
from pdje_POLYGLOT import KEY_VEC
engine = pdje_POLYGLOT.PDJE("../SWIG_test/testSampleDatas/tempdb.db")

ret:TRACK_VEC = engine.SearchTrack("")
for mus in ret:
    
    print(mus.trackTitle)
    

musret = engine.SearchMusic("WTC", "")

for mus in musret:
    print(mus.title)

tempdd =trackdata()


# print(ret.front().thisown)
print(engine.InitPlayer(pdje_POLYGLOT.FULL_PRE_RENDER, ret.front(), 48))
ap:audioPlayer = engine.GetPlayer()
print(ap.Activate())
input("waiting for enter: ")
print(engine.GetPlayer().GetStatus())

fxpannelMaster:FXControlPannel = ap.GetFXControlPannel()
fxpannelMaster.FX_ON_OFF(pdje_POLYGLOT.FILTER, True)
fxHandler = ARGSETTER_WRAPPER(fxpannelMaster)
keylist:KEY_VEC = fxHandler.GetFXArgKeys(pdje_POLYGLOT.FILTER)

for i in keylist:
    print("key: ", i)
fxHandler.SetFXArg(pdje_POLYGLOT.FILTER, "HLswitch", 1)
fxHandler.SetFXArg(pdje_POLYGLOT.FILTER, "Filterfreq", 5000)

input("FX Activated")
# engine.player.GetPlayer()
