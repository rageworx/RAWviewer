#include "winMain.h"

#define START_WIN_WIDTH     600
#define START_WIN_HEIGHT    400

static winMain* wMain = NULL;

int main (int argc, char ** argv)
{
    wMain = new winMain( START_WIN_WIDTH, START_WIN_HEIGHT, argc, argv );

    if ( wMain != NULL )
    {
        wMain->title( "RAW viewer" );
        return wMain->run();
    }
}
