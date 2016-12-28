#include "DictionaryLoader.h"

const char * DictionaryLoader::xml_root="strings.xml";

std::string DictionaryLoader::getStrByName(const std::string& key){
		//����CCDictionary����ȡxml  
		__Dictionary *strings = __Dictionary::createWithContentsOfFile(xml_root);
		//const char *str = ((__String*)strings->objectForKey(key))->getCString();
		return ((__String*)strings->objectForKey(key))->_string;

		//label->setString(StringUtils::format(game_cards_count, cardOK));//"�ѷ�: %d/4"
	}

float DictionaryLoader::getFloatByName(const std::string& key){
	std::string temp=getStrByName(key);
	return atof(temp.c_str());
}

int DictionaryLoader::getIntByName(const std::string& key){
	std::string temp=getStrByName(key);
	return atoi(temp.c_str());
}