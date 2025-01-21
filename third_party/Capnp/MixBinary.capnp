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
    lfoFilter @9;
    lfoFlanger @10;
    lfoPhaser @11;
    lfoTrance @12;
    lfoPanner @13;
    battleDj @14;
    lfoRoll @15;
}

enum DetailEnum{
    high @0;
    mid @1;
    low @2;
    pause @3;
    trim @4;
    fader @5;
    timeStretch @6;
    spin @7;
    pitch @8;
    rev @9;
    scratch @10;
    bscratch @11;
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