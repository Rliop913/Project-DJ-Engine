@0xda9d655f7a1e8315;


struct NoteBinaryData{
    noteType @0 :Text;
    noteDetail @1 :Text;
    first @2 :Text;
    second @3 :Text;
    third @4 :Text;
    bar @5 :Int64;
    beat @6 :Int64;
    separate @7 :Int64;
    eBar @8 :Int64;
    eBeat @9 :Int64;
    eSeparate @10 :Int64;
}

struct NoteBinaryCapnpData{
    datas @0 :List(NoteBinaryData);
}