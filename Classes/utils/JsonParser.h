#pragma once
#include "cocos2d.h"
#include "json/document.h"
#include "json/rapidjson.h"

class JsonParser {
public:
	static rapidjson::Document loadJsonFromFile ( const std::string& filePath ) {
		// 获取文件数据
		cocos2d::Data data = cocos2d::FileUtils::getInstance( )->getDataFromFile ( filePath );

		if ( data.isNull ( ) ) {
			cocos2d::log ( "failed load" , filePath );
			return rapidjson::Document ( );
		}

		// 解析JSON
		rapidjson::Document doc;
		doc.Parse ( reinterpret_cast<const char*>(data.getBytes ( )) , data.getSize ( ) );

		if ( doc.HasParseError ( ) ) {
			cocos2d::log ( "JSON parse error: %s (offset: %zu)" , doc.GetParseError ( ) );
			return rapidjson::Document ( );
		}

		return doc;
	}
	// get Vec2 as postion
	static cocos2d::Vec2 getVec2FromJson ( const rapidjson::Value& jsonObj ) {
		if ( !jsonObj.IsObject ( ) || !jsonObj.HasMember ( "x" ) || !jsonObj.HasMember ( "y" ) ) {
			return cocos2d::Vec2::ZERO;
		}
		return cocos2d::Vec2 (
			jsonObj["x"].GetFloat ( ) ,
			jsonObj["y"].GetFloat ( )
		);
	}
};