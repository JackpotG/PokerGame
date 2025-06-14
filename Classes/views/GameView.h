#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"

class GameView : public cocos2d::Node {
public:
	CREATE_FUNC ( GameView );
	virtual bool init ( ) override;
	// 设置游戏区域
	void setupLayout ( const cocos2d::Size& playfieldSize , const cocos2d::Size& stackSize );
	// 创建卡牌在游戏视图中
	void createCardViews ( GameModel& model );

	// 卡牌控制
	void updateCardPosition ( int cardId , const cocos2d::Vec2& position );
	CardView* getCardViewById ( int cardId );

	void setCardClickCallback ( const std::function<void ( int )>& callback ) {
		_cardClickCallback = callback;
	}

	cocos2d::Vec2 getStackTopPosition ( ) const {
		return _stackArea.origin + cocos2d::Vec2 ( 650 , 300 );
	}
	// 用于回退的函数

	void setupUndoButton ( );
	//回调
	void setUndoCallback ( std::function<void ( )> callback ) {
		_undoCallback = callback;
	}

private:
	//void setupBackground ( );

	cocos2d::Rect _playfieldArea;
	cocos2d::Rect _stackArea;
	std::unordered_map<int , CardView*> _cardViews;
	std::function<void ( int )> _cardClickCallback;
	// 回退回调函数
	std::function<void ( )> _undoCallback;
};