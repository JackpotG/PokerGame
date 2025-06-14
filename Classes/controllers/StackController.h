#pragma once
#include "../models/GameModel.h"
#include "../views/GameView.h"

class StackController {
public:
	StackController ( GameModel& gameModel , GameView* gameView )
		: _gameModel ( gameModel ) , _gameView ( gameView ) {
	}

	void onCardClicked ( int cardId );

private:
	GameModel& _gameModel;
	GameView* _gameView;
};