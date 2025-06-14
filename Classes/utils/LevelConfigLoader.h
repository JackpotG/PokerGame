#pragma once
#pragma once
#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../utils/JsonParser.h"
#include <vector>

// πÿø®≈‰÷√Ω·ππ
struct LevelConfig {
	struct CardData {
		int id;
		CardFaceType face;
		CardSuitType suit;
		cocos2d::Vec2 position;
	};

	std::vector<CardData> playfieldCards;
	std::vector<CardData> stackCards;
};

class LevelConfigLoader {
public:
	static LevelConfig loadLevelConfig ( int levelId );
	static std::vector<LevelConfig::CardData> parseCardArray ( const rapidjson::Value& array , int startId = 1 );
};