#include <cstdio>
#include <cstdlib>
#include "dentaldetector.h"

DentalDetector::DentalDetector()
 : refpixels(NULL),
   refpixelsize(0),
   val_centerpin(128),
   val_sidepin(250)
{

}

DentalDetector::~DentalDetector()
{

}

void DentalDetector::data( const unsigned char* normalizedpixels, int size )
{

}

bool DentalDetector::DetectCenterpin( VatechDectectObjectPos &pos )
{
    return false;
}

bool DentalDetector::DetectSidepin( VatechDectectObjectPos &pos )
{
    return false;
}

bool DentalDetector::DetectLeftCenterBall( VatechDectectObjectPos &pos )
{
    return false;
}

bool DentalDetector::DetectRightCenterBall( VatechDectectObjectPos &pos )
{
    return false;
}
