#include "GameView.h"
#include "CardView.h"

bool GameView::init ( ) {
	if ( !Node::init ( ) ) {
		return false;
	}

	this->setContentSize ( cocos2d::Size ( 1080 , 2080 ) );

	auto bgColor = cocos2d::LayerColor::create ( cocos2d::Color4B ( 0 , 100 , 0 , 255 ) , 1080 , 2080 );
	this->addChild ( bgColor , -1 );

	return true;
}

void GameView::setupLayout ( const cocos2d::Size& playfieldSize , const cocos2d::Size& stackSize ) {
	// 主牌区
	auto playfieldBG = cocos2d::LayerColor::create (
		//rgb ( 170 , 121 , 66 )
		cocos2d::Color4B ( 170 , 121 , 66 , 255 ) ,
		playfieldSize.width ,
		playfieldSize.height
	);
	playfieldBG->setPosition ( 0 , stackSize.height );
	this->addChild ( playfieldBG , 0 );

	// 堆牌区
	auto stackBG = cocos2d::LayerColor::create (
		//rgb ( 148 , 33 , 146 )
		cocos2d::Color4B ( 148 , 33 , 146 , 255 ) ,
		stackSize.width ,
		stackSize.height
	);
	stackBG->setPosition ( 0 , 0 );
	this->addChild ( stackBG , 0 );

	// 保存区域信息到当前类
	_playfieldArea = cocos2d::Rect ( 0 , stackSize.height , playfieldSize.width , playfieldSize.height );
	_stackArea = cocos2d::Rect ( 0 , 0 , stackSize.width , stackSize.height );
}

void GameView::createCardViews ( GameModel& model ) {

	const int baseZOrder = 10;

	for ( int id : model.getPlayfieldCardIds ( ) ) {
		const CardModel* card = model.getCardById ( id );
		if ( card ) {
			CardView* cardView = CardView::create ( *card );
			if ( cardView ) {
				cardView->setPosition ( card->getPosition ( ) );
				// 卡牌控制
				cardView->setClickCallback ( _cardClickCallback );
				this->addChild ( cardView , baseZOrder );
				// 保存
				_cardViews[id] = cardView;
			}
		}
	}

	// 堆牌区
	for ( int id : model.getStackCardIds ( ) ) {
		const CardModel* card = model.getCardById ( id );
		if ( card ) {
			CardView* cardView = CardView::create ( *card );
			if ( cardView ) {
				cardView->setPosition ( card->getPosition ( ) );
				cardView->setClickCallback ( _cardClickCallback );
				//移动之后如果是顶层卡牌移动至最高层
				this->addChild ( cardView , baseZOrder );
				_cardViews[id] = cardView;
			}
		}
	}
}

void GameView::updateCardPosition ( int cardId , const cocos2d::Vec2& position ) {
	auto it = _cardViews.find ( cardId );
	if ( it != _cardViews.end ( ) ) {
		it->second->setPosition ( position );
		// 确保顶部牌显示在最前面
		if ( it->second->getParent ( ) ) {
			it->second->getParent ( )->reorderChild ( it->second , INT_MAX );
		}
	}
}

CardView* GameView::getCardViewById ( int cardId ) {
	auto it = _cardViews.find ( cardId );
	return (it != _cardViews.end ( )) ? it->second : nullptr;
}



// 回退函数
void GameView::setupUndoButton ( ) {
	cocos2d::Vec2 stackTop = getStackTopPosition ( );
	cocos2d::Vec2 buttonPos ( stackTop.x + 300 , stackTop.y );

	// 创建"回退"文字
	auto label = cocos2d::Label::createWithSystemFont ( "Undo" , "fonts/arial.ttf" , 72 );
	label->setTextColor ( cocos2d::Color4B::WHITE );

	// 创建菜单项
	auto menuItem = cocos2d::MenuItemLabel::create ( label , [this]( cocos2d::Ref* sender ) {
		if ( _undoCallback ) {
			_undoCallback ( );
		}
		} );

	// 创建菜单
	auto menu = cocos2d::Menu::create ( menuItem , nullptr );
	menu->setPosition ( buttonPos );
	this->addChild ( menu , 100 ); // 确保在顶层显示
}