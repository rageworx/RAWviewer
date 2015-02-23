#include "winMain.h"

winMain::winMain( int W, int H, int argc, char** argv  )
 : _argc( argc ),
   _argv( argv ),
   _window( NULL )
{
    _window = new Fl_Window( W, H );
    if ( _window != NULL )
    {
        _window->begin();
        createCompos();
        _window->end();
    }
}

int winMain::run()
{
    if ( _window != NULL )
    {
        _window->show();
        return Fl::run();
    }

    return -1;
}

void winMain::title( const char* t )
{
    if ( _window != NULL )
    {
        _window->label( t );
    }
}

const char* winMain::title()
{
    if ( _window != NULL )
    {
        return _window->label();
    }

    return NULL;
}

void winMain::createCompos()
{
    if ( _window != NULL )
    {

    }
}

winMain::~winMain()
{

}
