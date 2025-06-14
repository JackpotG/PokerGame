#include "UndoController.h"

void UndoController::undoLastAction ( ) {
	if ( !_undoModel.hasRecords ( ) ) return;

	// 获取最近一条撤销记录
	auto record = _undoModel.popRecord ( );

	// 执行模型层的撤销
	if ( record.executeUndo ) {
		record.executeUndo ( );
	}

	// 位置修改：获取移动的卡牌视图
	auto movedCardView = _gameView->getCardViewById ( record.movedCardId );
	if ( movedCardView ) {
		// 播放回退动画（平移到原始位置）
		movedCardView->playMoveAnimation ( record.originalPos , 0.3f );
	}

	// 位置修改：恢复原顶部牌视图
	if ( record.targetCardId > 0 ) {
		auto targetCardView = _gameView->getCardViewById ( record.targetCardId );
		if ( targetCardView ) {
			// 提升层级并移动到顶部位置
			targetCardView->getParent ( )->reorderChild ( targetCardView , INT_MAX );
			targetCardView->setPosition ( record.stackTopPos );
		}
	}
}