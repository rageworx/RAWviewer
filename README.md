#RAWviewer

RAWviewer is an open source project to read and viewing gray scale raw image includes DCM(DICOM).
Supports under 16bit grayscale to 8bit downscale, crop, threshold(cut-off) and write it gain.

## What will be possible to do ?
 1. View RAW or DCM in 8bit base video system (Your 24/32bit RGB is actually ARGB 8/8/8/8 bits).
 2. View DICOM tags (Edit tag is not be supported ...)
 3. Applying threshold and make it to cut-off filtered image.
 4. Crop and resize.
 5. Save it into RAW(with info), BMP, JPG and PNG image formats.

## Required libraries
 1. FLTK 1.3.2
 2. libtinydicom (in my repo.)
 3. librawprocessor (in my repo.)

## Required additional components
 1. customfltkcomponents (in my repo.)

## Build targets
 1. Windows32 (not 64bit, FLTK can not built in 64bit OS.)
 2. Linux (Desktop version, 32bit)
 3. MacOS (Maybe it works if successfully compiled in Linux.)
