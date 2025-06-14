#pragma once
#include "models/GameModel.h"
#include "models/UndoModel.h"
#include "views/GameView.h"

class UndoController {
public:
	UndoController ( GameModel& gameModel , UndoModel& undoModel , GameView* gameView )
		: _gameModel ( gameModel ) , _undoModel ( undoModel ) , _gameView ( gameView ) {
	}

	// Ö´ÐÐ³·Ïú²Ù×÷
	void undoLastAction ( );

private:
	GameModel& _gameModel;
	UndoModel& _undoModel;
	GameView* _gameView;
};