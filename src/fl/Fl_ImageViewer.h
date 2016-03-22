#ifndef __FL_IMAGEVIEWER_H__
#define __FL_IMAGEVIEWER_H__

#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Image.H>

class Fl_ImageViewer : public Fl_Scroll
{
    public:
        Fl_ImageViewer(int x,int y,int w,int h);
        virtual ~Fl_ImageViewer();

    public:
        void image(Fl_Image* aimg);
        void unloadimage();
        void multiplyratio( float rf );
        void fitwidth();
        void fitheight();

    public:
        Fl_Image* image()           { return imgsrc; }
        float     multiplyratio()   { return multiplier; }

    public:
        int imgw();
        int imgh();

    private:
        float       multiplier;

    private:
        Fl_Box*     dispbox;
        Fl_Image*   imgsrc;
        Fl_Image*   imgcached;
};


#endif /// of __FL_IMAGEVIEWER_H__
