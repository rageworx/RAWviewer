#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <tchar.h>
#else
#include "tchar.h"
#endif

#include "dlgColors.h"
#include "dlgCoords.h"
#include "dlgMain.h"

#include <FL/Fl_Native_File_Chooser.h>
#include <FL/Fl_Ask.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Draw.H>

#include "stdunicode.h"

#ifdef _WIN32
    #include "resource.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////

#define STR_APP_TITLE       "RAWview & Phantom & Pattern Detector Test w/ FLTK"
#define DEF_MAX_PATH        256
#define DEF_FONT_SIZE       11

#ifdef UNICODE
    #define _TSTRING        wstring
    #define _TCM2W( _x_ )   convertM2W( (const char*)_x_ )
    #define _TCW2M( _x_ )   convertW2M( (const wchar_t*)_x_ )
#else
    #define _TSTRING        string
    #define _TCM2W( _x_ )   _x_
    #define _TCW2M( _x_ )   _x_
#endif

////////////////////////////////////////////////////////////////////////////////

char                currentPath[DEF_MAX_PATH] = {0};
extern HINSTANCE    fl_display;

////////////////////////////////////////////////////////////////////////////////

static void fl_widget_cb(Fl_Widget* w, void* p);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DialogMain::DialogMain(int argc, char** argv)
 : _argc( argc ),
   _argv( argv ),
   window(NULL),
   dentalRAW(NULL),
   downscaled_raw_buffer(NULL),
   userzoom( false )
{
    string argvextractor = argv[0];

    if ( argvextractor.size() > 0 )
    {
#ifdef _WIN32
        char splitter[] = "\\";
#else
        char splitter[] = "/";
#endif
        string::size_type lastSplitPos = argvextractor.rfind( splitter );

        string extracted = argvextractor.substr(0, lastSplitPos - 1 );

        strcpy( currentPath, extracted.c_str() );
    }

    window = new Fl_Double_Window( COORD_WINDOW );

    if ( window != NULL )
    {
        //Fl::scheme("gtk+");

        window->label( STR_APP_TITLE );
        window->color( COL_WINDOW );

        window->begin();
        createComponents();
        window->end();

        window->resizable( imgDspBox );
        window->size_range( window->w(), window->h() );
        window->icon((char *)LoadIcon(fl_display, MAKEINTRESOURCE(IDC_ICON_A)));
        window->show();
    }

}

DialogMain::~DialogMain()
{
    if ( downscaled_raw_buffer != NULL )
    {
        delete[] downscaled_raw_buffer;
        downscaled_raw_buffer = NULL;
    }

    if ( dentalRAW != NULL )
    {
        delete dentalRAW;
    }
}

int DialogMain::Run()
{
    return Fl::run();
}

void DialogMain::ProcWidgetCB( Fl_Widget* w )
{
    if ( w == NULL )
        return;

    if ( w == btnLoadImage )
    {
        loadImage();
    }
    else
    if ( w == cbUseThreshold )
    {
        thresholdControl();
    }
    else
    if ( ( w == vsThresholdMin ) || ( w == vsThresholdMax ) )
    {
        thresholdControlModify();
    }
    else
    if ( w == vsImageZoom )
    {
        zoomProc();
    }
}

void DialogMain::drawuserimage(int w, int h)
{
    // draw something to display on !
}

void DialogMain::drawHistogram()
{
    if ( dentalRAW == NULL )
        return;

    if ( dentalRAW->getWeightsCount() == 0 )
        return;

    if ( boxHistogram == NULL )
        return;

    vector<int> histograph;

    dentalRAW->GetWeights( histograph );

    if( boxHistogram != NULL )
    {
        boxHistogram->data( histograph );
        if ( weight_report.timestamp > 0 )
        {
            boxHistogram->emphase( true );
            boxHistogram->emphase_value( weight_report.threshold_wide_min,
                                         weight_report.threshold_wide_max );
        }
        else
        {
            boxHistogram->emphase( false );
        }

        boxHistogram->limit( dentalRAW->getMaximumLevel() );
        boxHistogram->redraw();
    }

    histograph.clear();
}

void DialogMain::zoomProc()
{
    vsImageZoom->deactivate();

    float f_v = vsImageZoom->value();

    imgDspBox->multiplyratio( f_v );

    vsImageZoom->activate();

    userzoom = true;
}

void DialogMain::loadImage()
{
    if ( cbUseThreshold != NULL )
    {
        if ( cbUseThreshold->value() > 0 )
        {
            cbUseThreshold->value( 0 );
            thresholdControl();
        }
    }

    Fl_Native_File_Chooser nFC;

    nFC.title( "Choose a RAW image file to open" );
    nFC.type( Fl_Native_File_Chooser::BROWSE_FILE );
    nFC.filter( " RAW Image\t*.raw\n" );
    nFC.directory( currentPath );
    nFC.preset_file(NULL);

    int retVal = nFC.show();
    if ( retVal == 0 )
    {
        if ( strlen(nFC.filename()) == 0 )
        {
            boxImageFileName->label(" ");
            fl_alert("No image selected.");
            return;
        }

        if ( dentalRAW == NULL )
        {
            dentalRAW = new DentalRAWReader();
        }

        if ( dentalRAW != NULL )
        {
            fl_cursor( FL_CURSOR_WAIT, 0, 0 );

#if( UNICODE && DEBUG )
            if ( sizeof(TCHAR) != 2 )
            {
                fl_alert("What's wrong? why it is not unicode ?");
                return;
            }
#endif

            _TSTRING refPath =  _TCM2W( nFC.filename() );

            if ( dentalRAW->Load( refPath.c_str() ) == true )
            {
                rawimagefilename = nFC.filename();

                boxImageFileName->label( rawimagefilename.c_str() );
                boxImageFileName->redraw();
            }
            else
            {
                fl_alert("Failed to load RAW image:\n%s", nFC.filename() );
            }

            if ( dentalRAW->GetAnalisysReport( weight_report ) == false )
            {
                memset( &weight_report, 0, sizeof (weight_report) );
            }

            if( vsThresholdMin != NULL )
            {
                vsThresholdMin->range( 0.0f, float( dentalRAW->getMaximumLevel() ) - 1.0f );
                vsThresholdMin->value( 0 );
            }

            if( vsThresholdMax != NULL )
            {
                vsThresholdMax->range( 1.0f, float( dentalRAW->getMaximumLevel() ) );
                vsThresholdMax->value( dentalRAW->getMaximumLevel() );
            }

            updateImageInfo();
            drawHistogram();
            importRAWimage();

            fl_cursor( FL_CURSOR_DEFAULT, 0, 0 );
        }
    }
}

void DialogMain::updateImageInfo()
{
    if ( boxImageInfo == NULL )
        return;

    if ( dentalRAW == NULL )
    {
        boxImageInfo->label("Image processor not loaded.");
        return;
    }

    if ( dentalRAW->getPixelCount() == 0 )
    {
        boxImageInfo->label("Image not loaded. size = 0");
        return;
    }

    static char infoText[1024] = {0};

    sprintf( infoText,
             "Pixels count = %d ( %d x %d )\nImage levels, min = %d | med = %d | max = %d",
             dentalRAW->getPixelCount(),
             dentalRAW->getWidth(),
             dentalRAW->getHeight(),
             dentalRAW->getMinimumLevel(),
             dentalRAW->getMediumLevel(),
             dentalRAW->getMaximumLevel());

    boxImageInfo->label( infoText );

    // analyser
    if ( weight_report.timestamp > 0 )
    {
        sprintf( infoText,
                 "%s\nThreshold min = %d, max = %d, wide = %d\n",
                 infoText,
                 weight_report.threshold_wide_min,
                 weight_report.threshold_wide_max,
                 weight_report.threshold_wide );
        boxImageInfo->label( infoText );

        if ( hbHistogram != NULL )
        {
            hbHistogram->clear();

            vector<int> weights;

            dentalRAW->GetWeights( weights );

            hbHistogram->add("weights list", NULL);
            hbHistogram->add("(idx: counts)", NULL);
            hbHistogram->add("-------------", NULL);

            for(int cnt=0; cnt<weights.size(); cnt++ )
            {
                int curW = weights[cnt];
                if ( curW > 0 )
                {
                    char tmpStr[40] = {0};
                    sprintf( tmpStr, "%05d: %d", cnt, curW );
                    hbHistogram->add(tmpStr, NULL);
                }
            }
        }

    }
}

void DialogMain::importRAWimage( bool useThreshold )
{
    if ( dentalRAW != NULL )
    {
        vector<unsigned char> downscaled;

        if ( useThreshold == true )
        {
            dentalRAW->Get8bitThresholdedImage( weight_report, downscaled );
        }
        else
        {
            dentalRAW->Get8bitDownscaled( downscaled , DNSCALE_FULLDOWN);
        }

        imgDspBox->image(NULL);
        imgDspBox->clear_damage();

        if ( downscaled_raw_buffer != NULL )
        {
            delete[] downscaled_raw_buffer;
            downscaled_raw_buffer = NULL;
        }

        int imgSize = downscaled.size();
        if ( imgSize > 0 )
        {
            downscaled_raw_buffer = new unsigned char[ imgSize * 3 ];

            for( int cnt=0; cnt<imgSize; cnt++ )
            {
                unsigned char* refPixel = &downscaled_raw_buffer[cnt * 3];

                refPixel[0] = downscaled[cnt];
                refPixel[1] = downscaled[cnt];
                refPixel[2] = downscaled[cnt];
            }

            Fl_RGB_Image* newImg = new Fl_RGB_Image( downscaled_raw_buffer, dentalRAW->getWidth(), dentalRAW->getHeight(), 3);

            imgDspBox->image( newImg );
            if ( userzoom == true )
            {
                zoomProc();
            }
            else
            {
                imgDspBox->fitwidth();
                vsImageZoom->value( imgDspBox->multiplyratio() );

                prev_scroll_pos_w = imgDspBox->hscrollbar.maximum();
                prev_scroll_pos_x = imgDspBox->hscrollbar.value();
                prev_scroll_pos_h = imgDspBox->scrollbar.maximum();
                prev_scroll_pos_y = imgDspBox->scrollbar.value();
            }
        }

        downscaled.clear();
    }
}

void DialogMain::thresholdControl()
{
    int checkedstate = cbUseThreshold->value();

    cbUseThreshold->deactivate();

    if ( checkedstate > 0 )
    {
        vsThresholdMin->activate();
        vsThresholdMax->activate();

        if ( weight_report.timestamp > 0 )
        {
            vsThresholdMin->value( weight_report.threshold_wide_min );
            vsThresholdMax->value( weight_report.threshold_wide_max );

            thresholdControlModify(0);
        }
    }
    else
    {
        vsThresholdMin->deactivate();
        vsThresholdMax->deactivate();

        fl_cursor( FL_CURSOR_WAIT, 0, 0 );

        updateImageInfo();
        importRAWimage();

        fl_cursor( FL_CURSOR_DEFAULT, 0, 0 );
    }

    cbUseThreshold->activate();
}

void DialogMain::thresholdControlModify( int ccnt )
{
    int checkedstate = cbUseThreshold->value();

    if ( checkedstate > 0 )
    {
        if ( weight_report.timestamp > 0 )
        {
            int v_min = vsThresholdMin->value();
            int v_max = vsThresholdMax->value();

            if ( ccnt == 0 )
            {
                if ( v_min > v_max )
                {
                    v_max = v_min + 1;

                    vsThresholdMax->value( v_max );
                }
            }
            else
            {
                if ( v_min > v_max )
                {
                    v_min = v_max - 1;

                    vsThresholdMin->value( v_min );
                }
            }

            static char v_min_str[10] = {0};
            sprintf( v_min_str, " %d ", v_min );
            vsThresholdMin->label( "               " );
            vsThresholdMin->label( v_min_str );

            static char v_max_str[10] = {0};
            sprintf( v_max_str, " %d ", v_max );
            vsThresholdMax->label( "               " );
            vsThresholdMax->label( v_max_str );

            weight_report.threshold_wide_min = v_min;
            weight_report.threshold_wide_max = v_max;
            weight_report.threshold_wide = weight_report.threshold_wide_max - weight_report.threshold_wide_min;

            updateImageInfo();
            drawHistogram();
            thresholdApply();
        }
    }
}

void DialogMain::thresholdApply()
{
    fl_cursor( FL_CURSOR_WAIT, 0, 0 );

    updateImageInfo();
    importRAWimage( true );

    fl_cursor( FL_CURSOR_DEFAULT, 0, 0 );
}

void DialogMain::createComponents()
{
    //imgDspBox = new Fl_ImageDisplayBox( COORD_IMGDSPBOX );
    imgDspBox = new Fl_ImageViewer( COORD_IMGDSPBOX );
    if ( imgDspBox != NULL )
    {
        //imgDspBox->transparent( false );
        //imgDspBox->userdrawoverride(this);
    }

    boxImageFileName = new Fl_Box( COORD_IMGFILEDSP );
    if ( boxImageFileName != NULL )
    {
        boxImageFileName->box( FL_DOWN_BOX );
        boxImageFileName->align( FL_ALIGN_LEFT | FL_ALIGN_INSIDE );
        boxImageFileName->color( COL_WINDOW );
        boxImageFileName->labelcolor( COL_TEXT );
        boxImageFileName->labelsize( DEF_FONT_SIZE );
    }

    hbHistogram = new Fl_Hold_Browser( COORD_TABLEHISTO );
    if ( hbHistogram != NULL )
    {
        //hbHistogram->scrollbar.size()
        //hbHistogram->hscrollbar.size(4);
        hbHistogram->scrollbar_width(6);
        hbHistogram->textsize(10);
    }

    vsThresholdMin = new Fl_Slider( COORD_THLD_MIN, "0" );
    if ( vsThresholdMin != NULL )
    {
        vsThresholdMin->align( FL_ALIGN_LEFT );
        vsThresholdMin->type( FL_HOR_NICE_SLIDER );
        vsThresholdMin->box( FL_THIN_DOWN_BOX );
        vsThresholdMin->precision( 0 );
        vsThresholdMin->labelsize( DEF_FONT_SIZE );
        vsThresholdMin->range( 0.0f, 65534.0f );
        vsThresholdMin->value( 0 );
        vsThresholdMin->when( FL_WHEN_RELEASE );
        vsThresholdMin->callback( fl_widget_cb, this );
        vsThresholdMin->deactivate();
    }

    vsThresholdMax = new Fl_Slider( COORD_THLD_MAX, "65535" );
    if ( vsThresholdMax != NULL )
    {
        vsThresholdMax->align( FL_ALIGN_LEFT );
        vsThresholdMax->type( FL_HOR_NICE_SLIDER );
        vsThresholdMax->box( FL_THIN_DOWN_BOX );
        vsThresholdMax->precision( 0 );
        vsThresholdMax->labelsize( DEF_FONT_SIZE );
        vsThresholdMax->range( 1.0f, 65535.0f );
        vsThresholdMax->value( 65535 );
        vsThresholdMax->when( FL_WHEN_RELEASE );
        vsThresholdMax->callback( fl_widget_cb, this );
        vsThresholdMax->deactivate();
    }

    cbUseThreshold = new Fl_Check_Button( COORD_CHKBX_USERS, "Use threshold rendering");
    if ( cbUseThreshold != NULL )
    {
        cbUseThreshold->labelsize( DEF_FONT_SIZE );
        cbUseThreshold->callback( fl_widget_cb, this );
    }

    vsImageZoom = new Fl_Value_Slider( COORD_IMGZOOM, "Image Zoom" );
    if ( vsImageZoom != NULL )
    {
        vsImageZoom->align( FL_ALIGN_LEFT );
        vsImageZoom->type( FL_HOR_NICE_SLIDER );
        vsImageZoom->box( FL_THIN_DOWN_BOX );
        vsImageZoom->labelsize( DEF_FONT_SIZE );
        vsImageZoom->range( 0.2f, 3.0f );
        vsImageZoom->value( 1.0f );
        vsImageZoom->when( FL_WHEN_RELEASE );
        vsImageZoom->callback( fl_widget_cb, this );
    }

    boxHistogram = new Fl_HistogramDrawer( COORD_BOX_HISTO );
    if ( boxHistogram != NULL )
    {
        //boxHistogram->emphase( true );
    }

    btnLoadImage = new Fl_Button( COORD_BTNLOADIMG, "Load RAW" );
    if ( btnLoadImage != NULL )
    {
        btnLoadImage->labelsize( DEF_FONT_SIZE );
        btnLoadImage->callback( fl_widget_cb, this );
    }

    boxImageInfo = new Fl_Box( COORD_IMGINFO );
    if ( boxImageInfo != NULL )
    {
        boxImageInfo->box( FL_DOWN_BOX );
        boxImageInfo->align( FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE );
        boxImageInfo->color( COL_WINDOW );
        boxImageInfo->labelfont( FL_COURIER );
        boxImageInfo->labelcolor( COL_TEXT );
        boxImageInfo->labelsize( DEF_FONT_SIZE );
        boxImageInfo->label("Rageworx's Dental RAW image view & Phantom/Pattern Detector Engineering Version,\n"
                            "(C)Copyright 2013 rageworx open source softwares, Programmed by Raphael Kim (rage.kim@@gmail.com)");
    }
}

////////////////////////////////////////////////////////////////////////////////

void fl_widget_cb(Fl_Widget* w, void* p)
{
    if ( p!= NULL )
    {
        DialogMain* pDM = (DialogMain*)p;
        pDM->ProcWidgetCB( w );
    }
}
