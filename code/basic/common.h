#pragma once

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define EXPORT_LIBRARY __declspec(dllexport)
#define IMPORT_LIBRARY __declspec(dllimport)
#else
#define EXPORT_LIBRARY
#define IMPORT_LIBRARY
#endif

#define GIROPOLYFIT_EXPORTS
#ifdef GIROPOLYFIT_EXPORTS
#define GIROPOLYFIT_API EXPORT_LIBRARY
#else
#define GIROPOLYFIT_API IMPORT_LIBRARY
#endif

// Windows DLL export macros
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
// Disable a warning message about dll. This is a temporary solution
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;168958
#pragma warning(disable : 4251)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)
#pragma warning(disable : 4091)
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4101)
#endif
