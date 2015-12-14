#ifndef CBASE64_H
#define CBASE64_H

#include <string>
#include <vector>

class cBase64
{
	public:
		cBase64();
		~cBase64();
		string base64_encode(unsigned char const* , unsigned int len);
		vector<int> base64_decode(std::string const& s);
			
	protected:
};

#endif
