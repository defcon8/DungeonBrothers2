#ifndef CDEBUG_H
#define CDEBUG_H

#include <winsock2.h>
#include <string>

using namespace std;

class cDebug
{
	public:
		~cDebug();
		static cDebug* getInstance();
		void sendTraceItems();
		byte command[5];
		void prepareTrace(string trace, string text);
	private:
		cDebug();
		int connectToServer();
    	static cDebug* instance;
    	SOCKET debugsocket;
	protected:
};

//! Debug message creation macro
#define TRACE(traceitem, ...)                                           \
    do {                                                                \
        char __message[512];                                            \
        snprintf(__message, 512, __VA_ARGS__);                          \
        string output(__message);                                       \
        cDebug::getInstance()->prepareTrace(traceitem, output);   \
    } while(0)


#endif
