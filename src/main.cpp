#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <tchar.h>
#else
#include "tchar.h"
#endif

using namespace std;

#include "dlgMain.h"

int main (int argc, char ** argv)
{
    DialogMain* dlgMain = new DialogMain( argc, argv );

    int retCode = 0;

    if ( dlgMain != NULL )
    {
        retCode = dlgMain->Run();
    }

    return retCode;
}
