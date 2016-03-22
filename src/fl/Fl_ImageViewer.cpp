#include <FL/fl_draw.H>
#include "Fl_ImageViewer.h"

Fl_ImageViewer::Fl_ImageViewer(int x,int y,int w,int h)
 : Fl_Scroll( x, y, w, h ),
   multiplier( 1.0f ),
   dispbox( NULL ),
   imgsrc( NULL ),
   imgcached( NULL )
{
    box( FL_FLAT_BOX );
    type( Fl_Scroll::BOTH );

    this->begin();

        dispbox = new Fl_Box( x, y, x + 10, y + 10 );
        if ( dispbox != NULL )
        {
            dispbox->box( FL_FLAT_BOX );
            dispbox->hide();
        }

    this->end();
}

Fl_ImageViewer::~Fl_ImageViewer()
{
    unloadimage();

    if ( dispbox != NULL )
    {
        delete dispbox;
        dispbox = NULL;
    }
}

void Fl_ImageViewer::image(Fl_Image* aimg)
{
    if( aimg == NULL )
        return;

    unloadimage();

    imgsrc = aimg->copy( aimg->w(), aimg->h() );

    if ( imgsrc != NULL )
    {
        dispbox->resize( x(),
                         y(),
                         x() + imgsrc->w(),
                         y() + imgsrc->h() );
        dispbox->image( imgsrc );
        dispbox->show();
        this->redraw();
    }
}

void Fl_ImageViewer::unloadimage()
{
    if ( dispbox != NULL )
    {
        dispbox->image(NULL);
        dispbox->hide();
    }

    if( imgcached != NULL )
    {
        delete imgcached;
        imgcached = NULL;
    }

    if ( imgsrc != NULL )
    {
        delete imgsrc;
        imgsrc = NULL;
    }
}

void Fl_ImageViewer::multiplyratio( float rf )
{
    if ( dispbox == NULL )
        return;

    if ( imgsrc == NULL )
        return;

    if ( imgcached != NULL )
    {
        if ( dispbox->image() == imgcached )
        {
            dispbox->image( NULL );
        }

        delete imgcached;
        imgcached = NULL;
    }

    multiplier = rf;
    float f_w = imgsrc->w() * multiplier;
    float f_h = imgsrc->h() * multiplier;

    imgcached = imgsrc->copy( f_w, f_h );

    dispbox->resize( x(),
                     y(),
                     x() + int(f_w),
                     y() + int(f_h) );
    dispbox->image( imgcached );
    dispbox->show();
    this->redraw();

}

void Fl_ImageViewer::fitwidth()
{
    if ( imgsrc == NULL )
        return;

    float f_w = float( w() ) / float( imgsrc->w() );

    multiplyratio( f_w );
}

void Fl_ImageViewer::fitheight()
{
    if ( imgsrc == NULL )
        return;

    float f_h = float( h() ) / float( imgsrc->h() );

    multiplyratio( f_h );

}

int Fl_ImageViewer::imgw()
{
    if ( dispbox != NULL )
    {
        return dispbox->w();
    }

    return 0;
}

int Fl_ImageViewer::imgh()
{
    if ( dispbox != NULL )
    {
        return dispbox->h();
    }

    return 0;
}
