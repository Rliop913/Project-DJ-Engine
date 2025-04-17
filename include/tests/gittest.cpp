#include <iostream>
#include "jsonWrapper.hpp"
// #include "gitWrapper.hpp"
int main()
{
    auto jj = PDJE_JSONHandler<MIX_W>();
    auto nn = PDJE_JSONHandler<NOTE_W>();
    jj.load("./tempjson/jjjson.json");
    nn.load("./tempjson/jjnote.json");
    jj.add(
        MixArgs{
            TypeEnum::EQ,
            DetailEnum::HIGH,
            0,
            "3",
            "20.0",
            "",
            1, 0, 4,
            4, 0, 4
        }
    );
    nn.add(
        NoteArgs{
            "TypeEnum::EQ",
            "DetailEnum::HIGH",
            "0",
            "3",
            "20.0",
            1, 0, 4,
            4, 0, 4
        }
    );
    // jj.deleteLine(
    //     TypeEnum::EQ,
    //     DetailEnum::HIGH,
    //     0
    // );
    jj.save("./tempjson/jjjson.json");
    jj.save("./tempjson/jjjson_for_delete.json");
    nn.save("./tempjson/jjnote.json");
    jj.getAll<MixArgs>([](const MixArgs& args){
        std::cout
        << "type : " << static_cast<uint16_t>( args.type) << std::endl 
        << "details : " << static_cast<uint16_t>( args.details) << std::endl 
        << "ID : " << args.ID << std::endl 
        << "first : " << args.first << std::endl 
        << "second : " << args.second << std::endl 
        << "third : " << args.third << std::endl 
        << "bar : " << args.bar << std::endl 
        << "beat : " << args.beat << std::endl 
        << "separate : " << args.separate << std::endl 
        << "Ebar : " << args.Ebar << std::endl 
        << "Ebeat : " << args.Ebeat << std::endl 
        << "Eseparate : " << args.Eseparate << std::endl;
    });
    auto res = jj.render();
    auto dd = res->Wp->getDatas();
    if(dd[0].getType() == TypeEnum::EQ){
        std::cout << "SAME!!!" << std::endl;
    }
    jj.deleteFile("./tempjson/jjjson_for_delete.json");
    return 0;
}