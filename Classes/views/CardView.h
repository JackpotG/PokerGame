#pragma once
#include "cocos2d.h"
#include "../models/CardModel.h"

class CardView : public cocos2d::Sprite {
public:
	static CardView* create ( const  CardModel& model );
	bool init ( const  CardModel& model );

	int getCardId ( ) const { return _cardId; }

	//卡牌控制
	void setClickCallback ( std::function<void ( int )> callback ) {
		_clickCallback = callback;
		setupTouchEvents ( );
	}

	// 播放移动动画
	void playMoveAnimation ( const cocos2d::Vec2& target , float duration, std::function<void ( )> onComplete = nullptr );


private:
	void createCardBackground ( ); 
	void createCardContent ( );
	int _cardId;
	CardSuitType _suit;
	CardFaceType _face;
	// 控制移动
	void setupTouchEvents ( );
	std::function<void ( int )> _clickCallback;
};