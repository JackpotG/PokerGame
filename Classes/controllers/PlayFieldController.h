#pragma once
#include "../models/GameModel.h"
#include "../views/GameView.h"

class GameView;
class CardService;

class PlayFieldController {
public:
	PlayFieldController ( GameModel& gameModel , GameView* gameView )
		: _gameModel ( gameModel ) , _gameView ( gameView ) {
	}

	void onCardClicked ( int cardId );

private:
	bool canMatchWithTopCard ( int cardId ) const;

	GameModel& _gameModel;
	GameView* _gameView;
};