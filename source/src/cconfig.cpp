#include <iostream>
#include <fstream>
#include <string>
#include "../include/cdebug.h"
#include "../include/cconfig.h"
#include "../include/cworld.h"
#include "../include/cfunctions.h"


cConfig::cConfig()
{
	TRACE("Construct","cConfig()");
	init();
	load();
}

cConfig::~cConfig()
{
	TRACE("Deconstruct","cConfig()");
}

void cConfig::init(){
	displaywidth = 640;
	displayheight = 480; 
	fullscreen = false;
}

void cConfig::optionSetDispatcher(const string name, const string value){
	TRACE("Options","Set ->   Name: %s   Value: %s", name.c_str(), value.c_str());
	if(name == "displaywidth") displaywidth = atoi(value.c_str());
	if(name == "displayheight") displayheight = atoi(value.c_str());
	if(name == "fullscreen") fullscreen = (value == "1" ? true : false);
}

bool cConfig::load()
{
	/* Reset section */
	section = "";
	
	ifstream in_stream;
	string line;
	
	in_stream.open("settings.cfg");

	while(!in_stream.eof())
	{
	    in_stream >> line;
	    TRACE("Disk","Read config line: %s", line.c_str());
	    cWorld::getInstance()->functions->trim(line);
	    vector<string> parameter;
	    cWorld::getInstance()->functions->split(line,'=',parameter);
	    TRACE("Disk","Split size: %d", parameter.size());
		if(parameter.size() == 2){
			optionSetDispatcher(parameter.at(0), parameter.at(1));
		}
	}
	
	in_stream.close();
}
