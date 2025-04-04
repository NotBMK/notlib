#include <iostream>
#include <cmd.h>

using namespace std;

int main(int argc, char const *argv[])
{
    Module mov("mov");

    mov.addOption("test", [&]()
    {
        double num;
        while (mov.utils().hasNextArg())
        {
            mov.utils().dumpNext(&num);
            mov.utils().report("got number " + std::to_string(num));
        }
    });

    mov.addOption("input", [&]()
    {
        std::string file =  mov.utils().next();
        mov.utils().report("got file '" + file + "'");
    });

    mov.addOption("execute", [&]()
    {
        std::string file = mov.utils().next();
        system(file.c_str());
    });

    mov.noReport();

    mov.parse(argc, argv);
}