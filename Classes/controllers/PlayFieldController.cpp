#include "PlayFieldController.h"

void PlayFieldController::onCardClicked ( int cardId ) {
	auto clickedCard = _gameModel.getCardById ( cardId );
	if ( !clickedCard || clickedCard->isInStack ( ) ) {
		return; 
	}

	auto topCard = _gameModel.getTopStackCard ( );
	cocos2d::log ( "topCard  _gameModel.getTopStackCard ( ) play field; : tocard getPosition %d,%d" , topCard->getPosition ( ).x , topCard->getPosition ( ).y);

	if ( !topCard ) return;

	if ( !canMatchWithTopCard ( cardId ) ) { return; }

	cocos2d::Vec2 targetPosition = topCard->getPosition ( );
	// 回退记录开关打开
	_gameModel.moveCardToStackTop ( cardId , true );

	CardView* cardView = _gameView->getCardViewById ( cardId );
	if ( cardView ) {
		if ( cardView->getParent ( ) ) {
			cardView->getParent ( )->reorderChild ( cardView , INT_MAX );
		}
		cardView->playMoveAnimation ( targetPosition , 0.5f , [=]( ) {
			cardView->setPosition ( targetPosition );
			} );
		
	}
}
bool PlayFieldController::canMatchWithTopCard ( int cardId ) const {
	auto clickedCard = _gameModel.getCardById ( cardId );
	auto topCard = _gameModel.getTopStackCard ( );

	if ( !clickedCard || !topCard ) return false;

	// 获取点数值
	int clickedValue = clickedCard->getFaceValue ( );
	int topValue = topCard->getFaceValue ( );

	// 差为1
	return std::abs ( clickedValue - topValue ) == 1;
}