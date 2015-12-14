#ifndef CFUNCTIONS_H
#define CFUNCTIONS_H

#include <string>
#include <vector>

using namespace std; 

class cFunctions
{
	public:
		cFunctions();
		~cFunctions();
		
		void split(const string& s, char c, vector<string>& v);
		void trim(string& str);
	protected:
};

#endif
