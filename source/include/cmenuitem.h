#ifndef CMENUITEM_H
#define CMENUITEM_H

#include "../include/cdebug.h"
#include <string>
#include <vector>

using namespace std;

class cMenuItem
{
	public:
		cMenuItem();
        string name;
        int actionid;
        virtual ~cMenuItem();
        vector<cMenuItem> menuitems;
	protected:
};

#endif
