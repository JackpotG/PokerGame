#pragma once
#include <vector>
#include <unordered_map>
#include "CardModel.h"
#include <cocos2d.h>
#include "UndoModel.h"

class GameModel {
public:
	// 初始化游戏模型，加载主牌区卡牌和堆牌区卡牌
	void initialize (
		const std::vector<CardModel>& playfieldCards ,
		const std::vector<CardModel>& stackCards
	);
	const std::unordered_map<int , CardModel>& getAllCards ( ) const { return _allCards; }
	const std::vector<int>& getPlayfieldCardIds ( ) const { return _playfieldCardIds; }
	const std::vector<int>& getStackCardIds ( ) const { return _stackCardIds; }
	
	// 获取卡牌指针
	// 有报错，增加const方法
	const CardModel* getCardById ( int id ) const {
		auto it = _allCards.find ( id );
		return (it != _allCards.end ( )) ? &it->second : nullptr;
	}

	
    #include <string> // Include for std::to_string

    CardModel* getCardById ( int id ) {
		//cocos2d::log ( "after initialize: this=%p, &_allCards=%p, _allCards.size()=%zu" , this , &_allCards , this->_allCards.size ( ) );
		auto it = _allCards.find ( id );
		return (it != _allCards.end ( )) ? &it->second : nullptr;
	}
	// 控制卡牌
	CardModel* getTopStackCard ( );
	std::vector<CardModel*> getPlayfieldCards ( ) const;

	void moveCardToStackTop ( int cardId ,bool recordUndo);
	void removeCardFromPlayfield ( int cardId );
	void setStackTopPosition ( const cocos2d::Vec2& position );


	std::unordered_map<int , CardModel> _allCards;

	// 回退操作函数
	// 记录回退信息
	void setUndoModel ( UndoModel* undoModel ) { _undoModel = undoModel; }
	void undoMove ( int movedCardId , int targetCardId , const cocos2d::Vec2& originalPos );

private:
	std::vector<int> _playfieldCardIds;
	std::vector<int> _stackCardIds;
	cocos2d::Vec2 _stackTopPosition;
	//回退操作变量
	UndoModel* _undoModel = nullptr;
};