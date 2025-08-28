@0xbbc6835e006f9146;

struct MMeta{
    bpm @0 :Text;
    beat @1 :Int64;
    subBeat @2 :Int64;
    separate @3 :Int64;
}

struct MusicBinaryCapnpData{
    datas @0 :List(MMeta);
}