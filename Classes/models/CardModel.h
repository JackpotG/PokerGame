#pragma once
#include "cocos2d.h"

// 花色类型
enum CardSuitType {
	CST_NONE = -1 ,
	CST_CLUBS ,      // 梅花
	CST_DIAMONDS ,   // 方块
	CST_HEARTS ,     // 红桃
	CST_SPADES ,     // 黑桃
	CST_NUM_CARD_SUIT_TYPES
};
// 正面类型
enum CardFaceType {
	CFT_NONE = -1 ,
	CFT_ACE ,
	CFT_TWO ,
	CFT_THREE ,
	CFT_FOUR ,
	CFT_FIVE ,
	CFT_SIX ,
	CFT_SEVEN ,
	CFT_EIGHT ,
	CFT_NINE ,
	CFT_TEN ,
	CFT_JACK ,
	CFT_QUEEN ,
	CFT_KING ,
	CFT_NUM_CARD_FACE_TYPES
};

class CardModel {
public:    
	// 默认构造函数
	CardModel ( ) : _id ( 0 ) , _face ( CFT_NONE ) , _suit ( CST_NONE ) ,
		_position ( cocos2d::Vec2::ZERO ) , _isInStack ( false ) ,
		_isTopCard ( false ) {
	}
	// 带参数构造
	CardModel ( int id , CardFaceType face , CardSuitType suit ,
		const cocos2d::Vec2& position , bool isInStack = false )
		: _id ( id ) , _face ( face ) , _suit ( suit ) , _position ( position ) ,
		_isInStack ( isInStack ) , _isTopCard ( false ) {
	}

	int getId ( ) const { return _id; }
	CardFaceType getFace ( ) const { return _face; }
	CardSuitType getSuit ( ) const { return _suit; }
	cocos2d::Vec2 getPosition ( ) const { return _position; }

	// 移动控制
	void setPosition ( const cocos2d::Vec2& position ) { _position = position; }
	bool isInStack ( ) const { return _isInStack; }
	bool isTopCard ( ) const { return _isTopCard; }
	void setIsInStack ( bool value ) { _isInStack = value; }
	void setIsTopCard ( bool value ) { _isTopCard = value; }

	// 逻辑判断
	int getFaceValue ( ) const {
		return static_cast<int>(_face) + 1;
	}

private:
	int _id; // 卡牌id
	CardFaceType _face; // 牌面枚举值
	CardSuitType _suit; // 花色
	cocos2d::Vec2 _position; //位置
	bool _isInStack = false;
	bool _isTopCard = false;
};