#ifndef CLOUDFLARESCRAPER_EXPORT_H
#define CLOUDFLARESCRAPER_EXPORT_H

#ifndef NO_DLL
    #ifdef DLL_IMPORT
        #define DLL_API Q_DECL_IMPORT
    #else
        #define DLL_API Q_DECL_EXPORT
    #endif
#endif

#endif // CLOUDFLARESCRAPER_EXPORT_H
