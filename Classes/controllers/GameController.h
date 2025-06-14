#pragma once
#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../utils/LevelConfigLoader.h"
#include "UndoController.h" 
// 需要指定路径，后续在调查初始根目录问题
#include "../models/UndoModel.h"     

class PlayFieldController;
class StackController;

class GameController :public cocos2d::Scene {
public:
	CREATE_FUNC ( GameController );
	//static GameController* create ( );
	virtual bool init( ) ;

	void startGame ( int levelId );

	cocos2d::Node* getView ( ) { return _gameView; }

	// 卡牌控制
	// 点击
	void onCardClicked ( int cardId );
	//void onUndoButtonClicked ( );
	GameModel& getGameModel ( ) { return _gameModel; }
	GameView* getGameView ( ) { return _gameView; }

	//回退函数
	void undoLastAction ( );

private:
	GameModel _gameModel;
	GameView* _gameView = nullptr;

	// 卡牌控制
	void initializeControllers ( );
	void loadLevelAndCreateModel ( int levelId );
	PlayFieldController* _playFieldController = nullptr;
	StackController* _stackController = nullptr;

	//回退控制变量

	UndoModel _undoModel;         // 撤销模型
	UndoController* _undoController = nullptr; // 撤销控制器
};