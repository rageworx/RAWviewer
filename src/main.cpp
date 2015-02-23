#include "winMain.h"

static winMain* wMain = NULL;

int main (int argc, char ** argv)
{
    wMain = new winMain( 600, 400, argc, argv );

    if ( wMain != NULL )
    {
        wMain->title( "RAW viewer" );
        return wMain->run();
    }
}
