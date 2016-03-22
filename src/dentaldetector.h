#ifndef __DentalDetector_H__
#define __DentalDetector_H__

typedef struct VatechDectectObjectPos
{
    int x;
    int y;
    int w;
    int h;
};

class DentalDetector
{
    public:
        DentalDetector();
        virtual ~DentalDetector();

    public:
        void data( const unsigned char* normalizedpixels, int size );

    public:
        unsigned char* data()                   { return refpixels; }
        void centerpinlevel( unsigned char lv ) { val_centerpin = lv; }
        unsigned char centerpinlevel()          { return val_centerpin; }
        void sidepinlevel( unsigned char lv )   { val_sidepin = lv; }

    public:
        bool DetectCenterpin( VatechDectectObjectPos &pos );
        bool DetectSidepin( VatechDectectObjectPos &pos );
        bool DetectLeftCenterBall( VatechDectectObjectPos &pos );
        bool DetectRightCenterBall( VatechDectectObjectPos &pos );

    protected:
        unsigned char*          refpixels;
        int                     refpixelsize;

    protected:
        unsigned char           val_centerpin;
        unsigned char           val_sidepin;
};

#endif /// of __DentalDetector_H__
