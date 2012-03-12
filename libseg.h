//
// MATLAB Compiler: 4.15 (R2011a)
// Date: Mon Mar 12 14:17:09 2012
// Arguments: "-B" "macro_default" "-W" "cpplib:libseg" "-T" "link:lib"
// "region_seg.m" "-d" "./lib2" 
//

#ifndef __libseg_h
#define __libseg_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_libseg
#define PUBLIC_libseg_C_API __global
#else
#define PUBLIC_libseg_C_API /* No import statement needed. */
#endif

#define LIB_libseg_C_API PUBLIC_libseg_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libseg
#define PUBLIC_libseg_C_API __declspec(dllexport)
#else
#define PUBLIC_libseg_C_API __declspec(dllimport)
#endif

#define LIB_libseg_C_API PUBLIC_libseg_C_API


#else

#define LIB_libseg_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libseg_C_API 
#define LIB_libseg_C_API /* No special import/export declaration */
#endif

extern LIB_libseg_C_API 
bool MW_CALL_CONV libsegInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libseg_C_API 
bool MW_CALL_CONV libsegInitialize(void);

extern LIB_libseg_C_API 
void MW_CALL_CONV libsegTerminate(void);



extern LIB_libseg_C_API 
void MW_CALL_CONV libsegPrintStackTrace(void);

extern LIB_libseg_C_API 
bool MW_CALL_CONV mlxRegion_seg(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libseg_C_API 
long MW_CALL_CONV libsegGetMcrID();


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libseg
#define PUBLIC_libseg_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libseg_CPP_API __declspec(dllimport)
#endif

#define LIB_libseg_CPP_API PUBLIC_libseg_CPP_API

#else

#if !defined(LIB_libseg_CPP_API)
#if defined(LIB_libseg_C_API)
#define LIB_libseg_CPP_API LIB_libseg_C_API
#else
#define LIB_libseg_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libseg_CPP_API void MW_CALL_CONV region_seg(int nargout, mwArray& seg, const mwArray& I, const mwArray& init_mask, const mwArray& max_its, const mwArray& alpha, const mwArray& display);

#endif
#endif
