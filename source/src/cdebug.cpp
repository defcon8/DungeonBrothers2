#include "..\include\cdebug.h"
#include <stdio.h>
#include <iostream>

cDebug* cDebug::instance = NULL;

cDebug::cDebug()
{
    command[0] = 0;
    command[1] = 1;
    command[2] = 2;
    command[3] = 3;
    command[4] = 4;
    connectToServer();
}

cDebug::~cDebug()
{}

cDebug* cDebug::getInstance()
{
    if(!instance) {
        instance = new cDebug();
    }
    return instance;
}

int cDebug::connectToServer()
{
    WSADATA data;
    int result = WSAStartup(MAKEWORD(2,2), &data);

    debugsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (debugsocket == INVALID_SOCKET) {
        WSACleanup();
        return 0;
    }

    sockaddr_in clientservice;

    clientservice.sin_family = AF_INET;
    clientservice.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientservice.sin_port = htons(55555);

    if (connect(debugsocket, (SOCKADDR*)&clientservice, sizeof(clientservice)) == SOCKET_ERROR) {
        WSACleanup();
        return 0;
    }

    sendTraceItems();
    prepareTrace("Init","Client connected.\n");

    return 0;
}


void cDebug::sendTraceItems()
{
    string traceitems("Audio,Bullit,checkDirectionCollision,Config,Construct,Deconstruct,Disk,Events,getHorScanPos,Gravity,Init,Jump,Menu,Mode,Objects,Options,pixelIsTransparant,Render,Slopes,Sprite,Texture,TTF");
    send(debugsocket,(char*)&command[3],1, 0);
    send(debugsocket,traceitems.c_str(),traceitems.size(), 0);
    send(debugsocket,(char*)&command[4],1, 0);
}
void cDebug::prepareTrace(string trace, string text)
{
    send(debugsocket,(char*)&command[0],1, 0);
    send(debugsocket,trace.c_str(),trace.length(), 0);
    send(debugsocket,(char*)&command[1],1, 0);
    send(debugsocket,text.c_str(),text.length(), 0);
    send(debugsocket,(char*)&command[2],1, 0);
}
