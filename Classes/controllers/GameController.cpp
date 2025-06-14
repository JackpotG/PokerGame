#include "GameController.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "UndoController.h"
#include "../utils/LevelConfigLoader.h"
#include "../views/GameView.h"

USING_NS_CC;

bool GameController::init ( ) {
	_gameView = GameView::create ( );
	if ( !_gameView ) return false;

	_gameView->retain ( );
	//添加回退label
	_gameView->setupUndoButton ( );
	//绑定回退callback
	_gameView->setUndoCallback ( [this]( ) {
		this->undoLastAction ( );
		} );

	//initializeControllers ( );

	return true;
}

void GameController::initializeControllers ( ) {

	_playFieldController = new PlayFieldController ( _gameModel , _gameView );
	_stackController = new StackController ( _gameModel , _gameView );

	// 回退控制器
	_undoController = new UndoController ( _gameModel , _undoModel , _gameView );
}
void GameController::loadLevelAndCreateModel ( int levelId ) {

	LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig ( levelId );
	std::vector<CardModel> playfieldCards;
	for ( const auto& cardData : levelConfig.playfieldCards ) {
		playfieldCards.push_back ( CardModel (
			cardData.id ,
			cardData.face ,
			cardData.suit ,
			cardData.position,
			false
		) );
	}
	std::vector<CardModel> stackCards;
	for ( const auto& cardData : levelConfig.stackCards ) {
		stackCards.push_back ( CardModel (
			cardData.id ,
			cardData.face ,
			cardData.suit ,
			cardData.position,
			true
		) );
	}

	_gameModel.initialize ( playfieldCards , stackCards );
	initializeControllers ( );
}

void GameController::startGame ( int levelId ) {

	loadLevelAndCreateModel ( levelId );


	_gameView->setupLayout ( cocos2d::Size ( 1080 , 1500 ) , cocos2d::Size ( 1080 , 580 ) );
	// 设置排队顶部位置 用于移动

	cocos2d::Vec2 topPos = _gameView->getStackTopPosition ( );
	_gameModel.setStackTopPosition ( topPos );

	//卡牌控制 
	_gameModel.setStackTopPosition ( _gameView->getStackTopPosition ( ) );
	// 绑定回调
	_gameView->setCardClickCallback ( [this]( int cardId ) {
		this->onCardClicked ( cardId );
	} );
	_gameView->createCardViews ( _gameModel );
	// 设置回退
	_gameModel.setUndoModel ( &_undoModel );

}
void GameController::onCardClicked ( int cardId ) {
	cocos2d::log ( "after initialize: this=%p, &_allCards=%p, _allCards.size()=%zu" , _gameModel , &_gameModel._allCards , _gameModel._allCards.size ( ) );
	const auto card = _gameModel.getCardById ( cardId );
	if ( !card ) return;

	if ( card->isInStack ( ) ) {
		// 堆牌区
		if ( _stackController ) {
			_stackController->onCardClicked ( cardId );
		}
	}
	else {
		// 主牌区
		if ( _playFieldController ) {
			_playFieldController->onCardClicked ( cardId );
		}
	}
}
void GameController::undoLastAction ( ) {
	if ( _undoController ) {
		_undoController->undoLastAction ( );
	}
}