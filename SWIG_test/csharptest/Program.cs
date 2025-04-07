// See https://aka.ms/new-console-template for more information


Console.WriteLine("Hello, World!");
Console.WriteLine(Environment.CurrentDirectory);
PDJE engine = new PDJE("../testSampleDatas/tempdb.db");

TRACK_VEC ret = engine.SearchTrack("");

foreach(var track in ret){
    Console.WriteLine(track.trackTitle);
}

MUS_VEC musret = engine.SearchMusic("WTC", "");
foreach(var mus in musret){
    Console.WriteLine(mus.title);
}

Console.WriteLine(
engine.InitPlayer(PLAY_MODE.FULL_PRE_RENDER, ret[0], 48)
);

var ap = engine.GetPlayer();
Console.WriteLine(ap.Activate());
Console.WriteLine(ap.GetStatus());
FXControlPannel fxpannel = ap.GetFXControlPannel();
fxpannel.FX_ON_OFF(FXList.FILTER, true);
ARGSETTER_WRAPPER fxHandler = new ARGSETTER_WRAPPER(fxpannel);
KEY_VEC keylist = fxHandler.GetFXArgKeys(FXList.FILTER);
foreach(var keys in keylist){
    Console.WriteLine(keys);
}

fxHandler.SetFXArg(FXList.FILTER, "HLswitch", 1);
fxHandler.SetFXArg(FXList.FILTER, "Filterfreq", 5000);
