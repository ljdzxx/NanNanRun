#ifndef __DICTIONARY_LOADER_H__
#define __DICTIONARY_LOADER_H__

#include "cocos2d.h"

USING_NS_CC;

class DictionaryLoader{
public:
	static const char * xml_root;
	static std::string getStrByName(const std::string& key);
	static float getFloatByName(const std::string& key);
	static int getIntByName(const std::string& key);
};

#endif