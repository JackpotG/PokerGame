#include "LevelConfigLoader.h"

LevelConfig LevelConfigLoader::loadLevelConfig ( int levelId ) {
	LevelConfig config;
	// 构建文件路径
	// D:\Workspace\Cocos2dx\pokerGame\Classes\configs
	std::string filePath = cocos2d::StringUtils::format ( "configs/level_%d.json" , levelId );
	//std::string filePath = cocos2d::StringUtils::format ( "D:\Workspace\Cocos2dx\pokerGame\Classes\configs" );
	// 加载JSON文档
	rapidjson::Document doc = JsonParser::loadJsonFromFile ( filePath );
	if ( doc.IsNull ( ) || !doc.IsObject ( ) ) {
		cocos2d::log ( "Failed to load level %d" , levelId );
		return config;
	}
	// 解析主牌区卡牌
	if ( doc.HasMember ( "Playfield" ) && doc["Playfield"].IsArray ( ) ) {
		config.playfieldCards = parseCardArray ( doc["Playfield"] , 1 );
	}
	// 解析堆牌区卡牌
	if ( doc.HasMember ( "Stack" ) && doc["Stack"].IsArray ( ) ) {
		config.stackCards = parseCardArray ( doc["Stack"] , 100 ); // 堆牌区ID从100开始
	}
	return config;
}
std::vector<LevelConfig::CardData> LevelConfigLoader::parseCardArray ( const rapidjson::Value& array , int startId ) {
	std::vector<LevelConfig::CardData> cards;

	if ( !array.IsArray ( ) ) {
		return cards;
	}

	int cardId = startId;
	for ( rapidjson::SizeType i = 0; i < array.Size ( ); i++ ) {
		const rapidjson::Value& cardObj = array[i];
		if ( !cardObj.IsObject ( ) ) continue;

		LevelConfig::CardData data;
		data.id = cardId++;

		// 解析牌面
		if ( cardObj.HasMember ( "CardFace" ) && cardObj["CardFace"].IsInt ( ) ) {
			int faceValue = cardObj["CardFace"].GetInt ( );
			data.face = static_cast<CardFaceType> ( faceValue );
		}

		// 解析花色
		if ( cardObj.HasMember ( "CardSuit" ) && cardObj["CardSuit"].IsInt ( ) ) {
			int suitValue = cardObj["CardSuit"].GetInt ( );
			data.suit = static_cast<CardSuitType>(suitValue);
		}

		// 解析位置
		if ( cardObj.HasMember ( "Position" ) && cardObj["Position"].IsObject ( ) ) {
			data.position = JsonParser::getVec2FromJson ( cardObj["Position"] );
		}

		cards.push_back ( data );
	}

	return cards;
}