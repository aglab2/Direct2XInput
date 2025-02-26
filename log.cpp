#include "log.h"

#include "wind.h"

#include <stdio.h>

#include <mutex>

HWND dllHWND = NULL;

static int vasprintf(char** strp, const char* fmt, va_list ap)
{
    int len = _vscprintf(fmt, ap);
    if (len == -1) {
        return -1;
    }
    size_t size = (size_t)len + 1;
    char* str = (char*) malloc(size);
    if (!str) {
        return -1;
    }

    int r = vsprintf_s(str, len + 1, fmt, ap);
    if (r == -1) {
        free(str);
        return -1;
    }
    *strp = str;
    return r;
}

static FILE* gLogFile;
static void openFileOnce()
{
    std::once_flag flag;
    std::call_once(flag, []() {
        auto dir = dllPath();
        auto logName = dir / "d2x.log";

        gLogFile = fopen(logName.u8string().c_str(), "a+");
    });
}

void WPRINTF(const char* fmt, ...)
{
    openFileOnce();

    va_list argptr;
    va_start(argptr, fmt);

    char* str = NULL;
    int amt = vasprintf(&str, fmt, argptr);
    if (amt > 0)
    {
        fwrite(str, amt, 1, gLogFile);
        fputc('\n', gLogFile);
    }

    free(str);
    va_end(argptr);
}

void IPRINTF(const char* fmt, ...)
{
    openFileOnce();

    va_list argptr;
    va_start(argptr, fmt);

    char* str = NULL;
    int amt = vasprintf(&str, fmt, argptr);
    if (amt > 0)
    {
        fwrite(str, amt, 1, gLogFile);
        fputc('\n', gLogFile);
    }

    free(str);
    va_end(argptr);
}

void logFlush()
{
    if (gLogFile)
        fflush(gLogFile);
}
