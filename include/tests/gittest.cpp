#include <iostream>
#include "jsonWrapper.hpp"
// #include "gitWrapper.hpp"
int main()
{
    auto jj = PDJE_JSONHandler();
    jj.load("./tempjson/jjjson.json");
    jj.add(
        TypeEnum::EQ,
        DetailEnum::HIGH,
        0,
        "3",
        "20.0",
        "",
        1, 0, 4,
        4, 0, 4
    );
    // jj.deleteLine(
    //     TypeEnum::EQ,
    //     DetailEnum::HIGH,
    //     0
    // );
    jj.save("./tempjson/jjjson.json");
    jj.save("./tempjson/jjjson_for_delete.json");
    
    jj.getAll([](TypeEnum type, DetailEnum details, int ID, const std::string &first, const std::string &second, const std::string &third, long long bar, long long beat, long long separate, long long Ebar, long long Ebeat, long long Eseparate){
        std::cout
        << "type : " << static_cast<uint16_t>( type) << std::endl 
        << "details : " << static_cast<uint16_t>( details) << std::endl 
        << "ID : " << ID << std::endl 
        << "first : " << first << std::endl 
        << "second : " << second << std::endl 
        << "third : " << third << std::endl 
        << "bar : " << bar << std::endl 
        << "beat : " << beat << std::endl 
        << "separate : " << separate << std::endl 
        << "Ebar : " << Ebar << std::endl 
        << "Ebeat : " << Ebeat << std::endl 
        << "Eseparate : " << Eseparate << std::endl;
        
    });
    auto res = jj.render();
    auto dd = res->Wp->getDatas();
    if(dd[0].getType() == TypeEnum::EQ){
        std::cout << "SAME!!!" << std::endl;
    }
    jj.deleteFile("./tempjson/jjjson_for_delete.json");
    return 0;
}