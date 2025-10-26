@0xda9d655f7a1e8315;


struct NoteBinaryData{
    noteType @0 :Text;
    noteDetail @1 :UInt16;
    first @2 :Text;
    second @3 :Text;
    third @4 :Text;
    beat @5 :Int64;
    subBeat @6 :Int64;
    separate @7 :Int64;
    ebeat @8 :Int64;
    esubBeat @9 :Int64;
    eSeparate @10 :Int64;
    railID @11 : UInt64;
}

struct NoteBinaryCapnpData{
    datas @0 :List(NoteBinaryData);
}