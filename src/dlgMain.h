#ifndef __DLGMAIN_H__
#define __DLGMAIN_H__

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Hold_Browser.H>
#include <Fl/Fl_Slider.H>
#include <Fl/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>

//#include "fl/Fl_ImageDisplayBox.h"
#include "fl/Fl_ImageViewer.h"
#include "fl/Fl_HistogramDrawer.h"

#include "dentalraw.h"

class DentalRAWReader;

//class DialogMain : Fl_ImageDisplayBoxUserOverride
class DialogMain
{
    public:
        DialogMain(int argc, char** argv);
        virtual ~DialogMain();

    public:
        int Run();
        void ProcWidgetCB( Fl_Widget* w );

    public: /// embody for inherits Fl_ImageDisplayBoxUserOverride
        void drawuserimage(int w, int h);

    private:
        void createComponents();

    private:
        void loadImage();
        void updateImageInfo();
        void importRAWimage( bool useThreshold = false );
        void thresholdControl();
        void thresholdControlModify( int ccnt = 0 );
        void thresholdApply();
        void drawHistogram();
        void zoomProc();

    private:
        int     _argc;
        char**  _argv;

    protected:
        Fl_Double_Window*   window;
        //Fl_ImageDisplayBox* imgDspBox;
        Fl_ImageViewer*     imgDspBox;
        Fl_Hold_Browser*    hbHistogram;
        Fl_Value_Slider*    vsImageZoom;
        Fl_Slider*          vsThresholdMin;
        Fl_Slider*          vsThresholdMax;
        Fl_Check_Button*    cbUseThreshold;
        Fl_HistogramDrawer* boxHistogram;
        Fl_Box*             boxImageFileName;
        Fl_Button*          btnLoadImage;
        Fl_Box*             boxImageInfo;

    protected:
        DentalRAWReader*        dentalRAW;
        WeightAnalisysReport    weight_report;
        bool                    userzoom;
        int                     prev_scroll_pos_x;
        int                     prev_scroll_pos_y;
        int                     prev_scroll_pos_w;
        int                     prev_scroll_pos_h;

    protected:
        std::string         rawimagefilename;
        unsigned char*      downscaled_raw_buffer;

};

#endif // of __DLGMAIN_H__
