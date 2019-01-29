#ifndef WC_FILE
#define WC_FILE FILE
#endif


#ifndef WC_FOPEN
#define WC_FOPEN fopen
#endif

#ifndef WC_FCLOSE
#define WC_FCLOSE fclose

#endif
#ifndef WC_FGETS
#define WC_FGETS fgets
#endif

#ifndef WC_FREAD
#define WC_FREAD(a, b, c) fread((a), (b), 1, (c))
#endif

#ifndef WC_FWRITE
#define WC_FWRITE(a, b, c) fwrite((a), (b), 1, (c))
#endif

#ifndef WC_FSEEK
#define WC_FSEEK fseek
#endif

#ifndef WC_FTELL
#define WC_FTELL ftell
#endif

#ifndef WC_REWIND
#define WC_REWIND rewind
#endif

#ifndef WC_FEOF
#define WC_FEOF feof
#endif

#ifndef WC_FGETC
#define WC_FGETC fgetc
#endif

#ifndef WC_FPUTC
#define WC_FPUTC fputc
#endif

#ifndef WC_FFLUSH
#define WC_FFLUSH fflush
#endif

#ifndef WC_PUTS
#define WC_PUTS puts
#endif

#ifndef WC_FSTAT
#define WC_FSTAT fstat
#endif

#ifndef WC_MALLOC
#define WC_MALLOC malloc
#endif

#ifndef WC_FREE
#define WC_FREE free
#endif

#ifndef WC_REALLOC
#define WC_REALLOC realloc
#endif

#ifndef MEMORY_TABLE_LENGTH
#define MEMORY_TABLE_LENGTH 300000
#endif
