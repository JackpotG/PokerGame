#include "StackController.h"

void StackController::onCardClicked ( int cardId ) {
	//被点击的cardmodel
	auto clickedCard = _gameModel.getCardById ( cardId );
	cocos2d::log ( "stackcontroller" );
	if ( !clickedCard ) return;
	// 非顶部牌跳过
	if ( !clickedCard->isInStack ( ) || clickedCard->isTopCard ( ) ) {
		return;
	}
	auto topCard = _gameModel.getTopStackCard ( );
	cocos2d::log ( "topCard  _gameModel.getTopStackCard ( ); : tocard getPosition + %d" , topCard->getPosition ( ).x );

	if ( !topCard ) return;


	// 动画目标位置
	cocos2d::Vec2 targetPosition = topCard->getPosition ( );
	
	// 被点击的移动到顶部 打开回退记录开关
	_gameModel.moveCardToStackTop ( cardId ,true);

	// 更新视图
	CardView* cardView = _gameView->getCardViewById ( cardId );
	if ( cardView ) {

		if ( cardView->getParent ( ) ) {
			cardView->getParent ( )->reorderChild ( cardView , INT_MAX );
		}
		cardView->playMoveAnimation ( targetPosition , 0.5f , [=]( ) {
			// 动画完成后更新位置
			cardView->setPosition ( targetPosition );
			} );
		//cardView->playMoveAnimation ( topCard->getPosition ( ) , 0.5f );
	}
}