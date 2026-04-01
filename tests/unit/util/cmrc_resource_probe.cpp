#include <cmrc/cmrc.hpp>

#include <string_view>

CMRC_DECLARE(pdje_okl);

int
main()
{
    auto fs = cmrc::pdje_okl::get_filesystem();
    auto file = fs.open("tempfile.txt");
    std::string_view contents(
        file.begin(), static_cast<size_t>(file.end() - file.begin()));
    return contents.find("placeholder") == std::string_view::npos;
}
