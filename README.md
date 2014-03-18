#RAWviewer

RAWviewer is an open source project to read and viewing gray scale raw image includes DCM(DICOM).
Supports under 16bit grayscale to 8bit downscale, crop, threshold(cut-out) and write it gain.

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
