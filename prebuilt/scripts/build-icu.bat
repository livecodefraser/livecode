@echo off
REM This file requires that the execution environment is set up as follows:
REM	Cygwin is in PATH
REM	MSVC is in PATH
REM	INCLUDE and LIB point to the Windows SDK
REM
REM It is also assumed that the ICU source has been unpacked in the current directory
REM (i.e. ./icu/source/runConfigureICU is the path to the configure script)

mkdir %_ROOT_DIR%\build-icu-%ARCH%
cd %_ROOT_DIR%\build-icu-%ARCH%

REM Tell bash to cope with CRLF line endings without complaining
set SHELLOPTS=igncr

REM --with-data-packaging=archive
bash ../../icu/source/runConfigureICU Cygwin/MSVC --prefix=/ --with-data-packaging=static --enable-static --disable-shared --disable-debug --disable-samples --disable-tests

REM Unfortunately, the following defines do not work properly for Win32 builds but
REM are used by the build scripts for the other platforms. This shouldn't make
REM anything break for Win32 but it makes it a bit less strict for some checks
REM
REM	-DU_USING_ICU_NAMESPACE=0
REM	-DUNISTR_FROM_CHAR_EXPLICIT=explicit
REM	-DUNISTR_FROM_STRING_EXPLICIT=explicit

REM Do the build
bash -c "make -k | tee build.log"

REM Build the minimal ICU data lib required by the installer
REM bash -c "bin/pkgdata.exe --bldopt data/icupkg.inc --quiet --copyright --sourcedir data/out/build/icudt52l --destdir ./lib --entrypoint icudt52 --tempdir data/out/tmp --name icudt52l-minimal --mode static --revision 52 --libname icudt-minimal ../../minimal-data.lst"

REM Install the files to the staging location
FOR /F "usebackq tokens=*" %%x IN (`cygpath.exe -u %_INSTALL_DIR%\win32\%ARCH%`) DO SET install_dir=%%x
bash -c "make DESTDIR=%install_dir% install"
