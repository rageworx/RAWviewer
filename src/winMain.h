#ifndef __WINMAIN_H__
#define __WINMAIN_H__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>

class winMain
{
    public:
            winMain( int W = 400, int H = 400, int argc = 0 , char** argv = NULL );

    public:
            int run();
            void title( const char* t );

    public:
            const char*    title();
            Fl_Window*      window() { return _window; }

    protected:
            ~winMain();

    protected:
            void createCompos();

    protected:
            int         _argc;
            char**      _argv;

    protected:
            Fl_Window*  _window;
            Fl_Group*   _grpMain;

};

#endif // __WINMAIN_H__
