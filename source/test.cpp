#include <iostream>
#include <cmd.h>
#include <objh.h>

using namespace std;

int main(int argc, char const *argv[])
{
    Module test("test");
    test.addOption("test", [](Module& mod)
    {
        double num;
        while (mod.utils().nextIsArg())
        {
            mod.utils().dumpNext(&num);
            mod.utils().report("'-t' got number " + std::to_string(num));
        }
    });

    test.report(true);

    test.parse(argc, argv);
}