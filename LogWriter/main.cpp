#include <wiringPi.h>
#include <string.h>
#include <signal.h>
#include "LogWriter/LogWriter.h"

static LogWriter* g_LogWriter = NULL;

#define NAME_MAXLEN     (16)

void signalHandler(int signum)
{
    if (g_LogWriter != NULL)
    {
        g_LogWriter->Stop(1000);
    }
}

int main(int argc, char* argv[])
{
    char controllerName[NAME_MAXLEN] = {0};

    if (argc <= 1)
    {
        strncpy(&controllerName[0], "Unknown", NAME_MAXLEN);
    }
    else
    {
        strncpy(&controllerName[0], &argv[1][0], NAME_MAXLEN);
    }

    signal(SIGKILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    g_LogWriter = new LogWriter(&controllerName[0]);
    if (g_LogWriter == NULL)
    {
        goto FINISH;
    }

    g_LogWriter->Run();

    while (1)
    {
        delay(100);
    }


FINISH :
	return 0;
}
