#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#   define EXPORT_LIBRARY __declspec(dllexport)
#   define IMPORT_LIBRARY __declspec(dllimport)
#else
#   define EXPORT_LIBRARY
#   define IMPORT_LIBRARY
#endif

#define GIROPOLYFIT_EXPORTS
#ifdef GIROPOLYFIT_EXPORTS
#   define GIROPOLYFIT_API EXPORT_LIBRARY
#else
#   define GIROPOLYFIT_API IMPORT_LIBRARY
#endif
