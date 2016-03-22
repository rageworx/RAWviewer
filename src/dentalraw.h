#ifndef __DENTAL_RAW_READER_H__
#define __DENTAL_RAW_READER_H__
////////////////////////////////////////////////////////////////////////////////
//
//  VATECH RAW Image Analyser for stdc++ w/ FLTK
//  =========================================================================
//  (C)Copyright 2013, Raphael Kim (rageworx@gmail.com)
//
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

typedef enum
{
    DNSCALE_NORMAL = 0,
    DNSCALE_FULLDOWN,
    DNSCALE_USER,
    DNSCALE_MAX
}DownscaleType;

struct WeightAnalisysReport
{
    unsigned long       timestamp;
    unsigned short      base_threshold_index;
    unsigned short      threshold_wide;
    unsigned short      threshold_wide_min;
    unsigned short      threshold_wide_max;
    unsigned int        htreshold_max_amount;
};

class VatechRAWUserScaleIF
{
    public:
        virtual bool processUserScale( std::vector<unsigned short> &src,
                                       std::vector<unsigned char> &dst ) = 0;
};

class DentalRAWReader
{
    public:
        DentalRAWReader();
        DentalRAWReader( const TCHAR* raw_file );
        virtual~DentalRAWReader();

    public:
        bool isLoaded() { return raw_loaded; }
        int  getPixelCount() { return pixel_arrays.size(); }
        unsigned short getMinimumLevel() { return pixel_min_level; }
        unsigned short getMaximumLevel() { return pixel_max_level; }
        unsigned short getMediumLevel()  { return pixel_med_level; }
        unsigned short* refPixelPointer() { return pixel_arrays.data(); }
        int  getWidth() { return img_width; }
        int  getHeight() { return img_height; }
        int  getWeightsCount() { return pixel_weights.size(); }

    public:
        bool Load( const TCHAR* raw_file );
        bool Reload( const TCHAR* raw_file );
        bool Reload();
        void Unload();
        void SetUserScale( VatechRAWUserScaleIF* ptr );
        bool Get8bitDownscaled( std::vector<unsigned char> &byte_arrays, DownscaleType dntype = DNSCALE_NORMAL );
        bool Get16bitRawImage( std::vector<unsigned short> &word_arrays );
        bool GetWeights( std::vector<int> &weight_arrays );
        bool GetAnalisysReport( WeightAnalisysReport &report );
        bool Get16bitThresholdedImage( WeightAnalisysReport &report, std::vector<unsigned short> &word_arrays );
        bool Get8bitThresholdedImage( WeightAnalisysReport &report, std::vector<unsigned char> &byte_arrays );

    protected:
        void analyse();
        void resetWeights();

    protected:
        bool                        raw_loaded;
        std::vector<unsigned short> pixel_arrays;
        std::vector<int>            pixel_weights;
        unsigned short              pixel_min_level;
        unsigned short              pixel_max_level;
        unsigned short              pixel_med_level;
        unsigned short              index_max_pixel;
#ifdef UNICODE
        std::wstring                raw_file_name;
#else
        std::string                 raw_file_name;
#endif
        int                         img_height;
        int                         img_width;
        VatechRAWUserScaleIF*       userscaler;

};


#endif /// of __VATECH_RAW_READER_H__
