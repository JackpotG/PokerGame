#include "GameModel.h"

void GameModel::initialize(
    const std::vector<CardModel>& playfieldCards,
    const std::vector<CardModel>& stackCards
) {
   for (const auto& card : playfieldCards) {
        const int id = card.getId();
        _allCards[id] = card;
        _playfieldCardIds.push_back(id);
    }

    for (const auto& card : stackCards) {
        const int id = card.getId();
       _allCards[id] = card;
        _stackCardIds.push_back(id);
        _allCards[id].setIsInStack(true);
    }
    if (!_stackCardIds.empty()) {
        _allCards[_stackCardIds.back()].setIsTopCard(true);
    }
    //cocos2d::log("after initialize: this=%p, &_allCards=%p, _allCards.size()=%zu", this, &_allCards, _allCards.size());
}

CardModel* GameModel::getTopStackCard() {
    //cocos2d::log("this=%p, &_allCards=%p, _allCards.size()=%zu", this, &_allCards, _allCards.size());
    if (!_stackCardIds.empty()) {
        return getCardById(_stackCardIds.back());
    }
    return nullptr;
}

// 将卡牌移动到堆牌区顶部
void GameModel::moveCardToStackTop(int cardId,bool recordUndo ) {


	auto card = getCardById ( cardId );
	if ( !card ) return;
	//回退

	// 保存原始状态用于回退
	cocos2d::Vec2 originalPos = card->getPosition ( );
	bool wasInStack = card->isInStack ( );
	int oldTopCardId = -1;
	
	//回退部分结束

		// 获取当前顶部牌
	if ( auto oldTopCard = getTopStackCard ( ) ) {
		oldTopCardId = oldTopCard->getId ( );
		// 更新原顶部牌状态
		oldTopCard->setIsTopCard ( false );
	}

	// 从原区域移除卡牌
	if ( card->isInStack ( ) ) {
		auto& stackIds = _stackCardIds;
		stackIds.erase ( std::remove ( stackIds.begin ( ) , stackIds.end ( ) , cardId ) , stackIds.end ( ) );
	}
	else {
		auto& playfieldIds = _playfieldCardIds;
		playfieldIds.erase ( std::remove ( playfieldIds.begin ( ) , playfieldIds.end ( ) , cardId ) , playfieldIds.end ( ) );
	}

	// 设置新顶部牌
	card->setIsInStack ( true );
	card->setIsTopCard ( true );
	_stackCardIds.push_back ( cardId );
	card->setPosition ( _stackTopPosition );

	// 记录回退操作开始
	if ( recordUndo && _undoModel ) {
		UndoRecord record;
		if ( wasInStack ) {
			record.actionType = UndoActionType::HAND_REPLACE;
		}
		else {
			record.actionType = UndoActionType::DESKTOP_MATCH;
		}

		record.movedCardId = cardId;
		record.targetCardId = oldTopCardId;
		record.originalPos = originalPos;
		record.stackTopPos = _stackTopPosition; // 记录当前牌堆顶部位置

		// 设置撤销执行函数
		record.executeUndo = [this , cardId , oldTopCardId , originalPos]( ) {
			this->undoMove ( cardId , oldTopCardId , originalPos );
			};

		_undoModel->pushRecord ( record );
	}

}

// 设置堆牌区顶部位置
void GameModel::setStackTopPosition(const cocos2d::Vec2& position) {
	cocos2d::log ( "StackTopPosition: position =%p," , position  );
    _stackTopPosition = position;
}

void GameModel::removeCardFromPlayfield(int cardId) {
    auto& playfieldIds = _playfieldCardIds;
    playfieldIds.erase(std::remove(playfieldIds.begin(), playfieldIds.end(), cardId), playfieldIds.end());
    _allCards.erase(cardId);
}

// 回退操作函数

// 位置修改：撤销移动操作
void GameModel::undoMove ( int movedCardId , int targetCardId , const cocos2d::Vec2& originalPos ) {
	// 1. 移除移动的卡牌
	auto movedCard = getCardById ( movedCardId );
	if ( !movedCard ) return;

	// 从牌堆中移除
	auto& stackIds = _stackCardIds;
	stackIds.erase ( std::remove ( stackIds.begin ( ) , stackIds.end ( ) , movedCardId ) , stackIds.end ( ) );

	// 2. 恢复原始状态
	movedCard->setIsInStack ( false );
	movedCard->setIsTopCard ( false );
	movedCard->setPosition ( originalPos );

	// 3. 恢复原顶部牌
	if ( targetCardId > 0 ) {
		auto targetCard = getCardById ( targetCardId );
		if ( targetCard ) {
			targetCard->setIsTopCard ( true );
			// 位置修改：恢复顶部牌到牌堆顶部位置
			targetCard->setPosition ( _stackTopPosition );
		}
	}

	// 4. 将卡牌放回原区域
	if ( movedCard->isInStack ( ) ) {
		_stackCardIds.push_back ( movedCardId );
	}
	else {
		_playfieldCardIds.push_back ( movedCardId );
	}
}