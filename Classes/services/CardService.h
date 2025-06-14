#pragma once
#include "models/CardModel.h"

class CardService {
public:
	static bool canMatchCards ( CardFaceType topStackFace , CardFaceType clickedFace );
	static std::string getCardTextureName ( CardFaceType face , CardSuitType suit );
};