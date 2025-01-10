@0x9e19a0c54796948a;


enum TypeEnum{
    filter @0;
    eq @1;
    distortion @2;
    control @3;
    vol @4;
    load @5;
    unload @6;
    beatMatch @7;
    bpmControl @8;
    echo @9;
    lfoFilter @10;
    lfoFlanger @11;
    lfoPhaser @12;
    lfoTrance @13;
    lfoPanner @14;
    battleDj @15;
    lfoRoll @16;
    flags @17;
}

enum DetailEnum{
    high @0;
    mid @1;
    low @2;
    cue @3;
    play @4;
    pause @5;
    trim @6;
    fader @7;
    solaOn @8;
    solaOff @9;
    spin @10;
    bspin @11;
    rev @12;
    scratch @13;
    bscratch @14;
    end @15;
    alive @16;
}

struct MBData{
    type @0 :TypeEnum;
    details @1 :DetailEnum;
    id @2 :Int32;
    first @3 :Text;
    second @4 :Text;
    third @5 :Text;
    bar @6 :Int64;
    beat @7 :Int64;
    separate @8 :Int64;
    ebar @9 :Int64;
    ebeat @10 :Int64;
    eseparate @11 :Int64;
    
}
struct MixBinaryCapnpData{
    datas @0 :List(MBData);
}