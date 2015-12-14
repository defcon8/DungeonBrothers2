#ifndef CCONFIG_H
#define CCONFIG_H

#include <map>
#include <string>

using namespace std;

class cConfig
{
	private:
		void init();
		bool load();
		map<string,string> parameters;
		void optionSetDispatcher(const string name, const string value);
		string section;
	public:
		cConfig();
		~cConfig();
		
		int displaywidth, displayheight;
		bool fullscreen;
	protected:
};

#endif
