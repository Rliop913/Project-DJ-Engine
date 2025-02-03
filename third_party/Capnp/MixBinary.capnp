@0x9e19a0c54796948a;


enum TypeEnum{
    filter @0;
    eq @1;
    distortion @2;
    control @3;
    vol @4;
    load @5;
    unload @6;
    bpmControl @7;
    echo @8;
    oscFilter @9;
    flanger @10;
    phaser @11;
    trance @12;
    panner @13;
    battleDj @14;
    roll @15;
    compressor @16;
}

enum DetailEnum{
    high @0;
    mid @1;
    low @2;
    pause @3;
    cue @4;
    trim @5;
    fader @6;
    timeStretch @7;
    spin @8;
    pitch @9;
    rev @10;
    scratch @11;
    bscratch @12;
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