#include "CardView.h"

bool CardView::init ( const CardModel& model ) {
	if ( !Node::init ( ) ) {
		return false;
	}
	_cardId = model.getId ( );
	_suit = model.getSuit ( );
	_face = model.getFace ( );


	this->setPosition ( model.getPosition ( ) );

	createCardBackground ( );
	createCardContent ( );

	this->setScale (1); 
	///////////////

	// 添加调试标记
	//auto debugLabel = cocos2d::Label::createWithSystemFont (
	//	cocos2d::StringUtils::format ( "%d" , model.getId ( ) ) ,
	//	"Arial" ,
	//	20
	//);
	//debugLabel->setPosition ( cocos2d::Vec2 ( 0 , 0 ) );
	//debugLabel->setTextColor ( cocos2d::Color4B::RED );
	//this->addChild ( debugLabel , 100 );

	//// 添加边界框
	//auto drawNode = cocos2d::DrawNode::create ( );
	//cocos2d::Size size = this->getContentSize ( );
	//cocos2d::Vec2 vertices[4] = {
	//	cocos2d::Vec2 ( -size.width / 2, -size.height / 2 ),
	//	cocos2d::Vec2 ( size.width / 2, -size.height / 2 ),
	//	cocos2d::Vec2 ( size.width / 2, size.height / 2 ),
	//	cocos2d::Vec2 ( -size.width / 2, size.height / 2 )
	//};
	//drawNode->drawPolygon ( vertices , 4 , cocos2d::Color4F ( 1 , 0 , 0 , 0.3 ) , 1 , cocos2d::Color4F ( 1 , 0 , 0 , 1 ) );
	//this->addChild ( drawNode , 99 );
	////////////////
	return true;
}
void CardView::createCardBackground ( ) {

	auto bg = cocos2d::Sprite::create ( "card_general.png" );
	if ( bg ) {
		bg->setAnchorPoint ( cocos2d::Vec2::ANCHOR_MIDDLE );
		this->addChild ( bg , 0 );
		this->setContentSize ( bg->getContentSize ( ) );
	}
}

void CardView::createCardContent ( ) {
	const bool isRed = (_suit == CST_HEARTS || _suit == CST_DIAMONDS);
	// 拼接资源文件名 - 颜色
	std::string colorPrefix = isRed ? "red" : "black";

	static const std::string suitNames[] = {
		"club", "diamond", "heart", "spade"
	};
	// 拼接资源文件名 - 花色
	std::string suitName = suitNames[_suit];

	// 转化配置文件到资源文件名
	static const std::string faceNames[] = {
		"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
	};
	std::string faceName = faceNames[_face];

	// 左上角数字
	auto smallNum = cocos2d::Sprite::create ( "number/small_" + colorPrefix + "_" + faceName + ".png" );
	if ( smallNum ) {
		smallNum->setAnchorPoint ( cocos2d::Vec2::ANCHOR_TOP_LEFT );
		smallNum->setPosition ( -this->getContentSize ( ).width / 2 + 10 ,
			this->getContentSize ( ).height / 2 - 10 );
		this->addChild ( smallNum , 1 );
	}

	// 左上角花色
	auto smallSuit = cocos2d::Sprite::create ( "suits/" + suitName + ".png" );
	if ( smallSuit ) {
		smallSuit->setAnchorPoint ( cocos2d::Vec2::ANCHOR_TOP_LEFT );
		smallSuit->setPosition ( -this->getContentSize ( ).width / 2 + 10 ,
			this->getContentSize ( ).height / 2 - 60 );
		smallSuit->setScale ( 0.7f );
		this->addChild ( smallSuit , 1 );
	}

	// 中间数字
	auto bigNum = cocos2d::Sprite::create ( "number/big_" + colorPrefix + "_" + faceName + ".png" );
	if ( bigNum ) {
		bigNum->setPosition ( 0 , 0 );
		this->addChild ( bigNum , 1 );
	}

	// 右下角数字
	auto smallNumBottom = cocos2d::Sprite::create ( "number/small_" + colorPrefix + "_" + faceName + ".png" );
	if ( smallNumBottom ) {
		smallNumBottom->setAnchorPoint ( cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT );
		smallNumBottom->setPosition ( this->getContentSize ( ).width / 2 - 40 ,
			-this->getContentSize ( ).height / 2 + 55 );
		smallNumBottom->setRotation ( 180 );
		this->addChild ( smallNumBottom , 1 );
	}

	// 右下角花色
	auto bigSuitBottom = cocos2d::Sprite::create ( "suits/" + suitName + ".png" );
	if ( bigSuitBottom ) {
		bigSuitBottom->setAnchorPoint ( cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT );
		bigSuitBottom->setPosition ( this->getContentSize ( ).width / 2 - 40 ,
			-this->getContentSize ( ).height / 2 + 85 );
		bigSuitBottom->setRotation ( 180 );
		bigSuitBottom->setScale ( 0.7f );
		this->addChild ( bigSuitBottom , 1 );
	}
}

void CardView::setupTouchEvents ( ) {
	auto listener = cocos2d::EventListenerTouchOneByOne::create ( );
	listener->setSwallowTouches ( true );

	listener->onTouchBegan = []( cocos2d::Touch* touch , cocos2d::Event* event ) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget ( ));
		cocos2d::Vec2 locationInNode = target->convertToNodeSpace ( touch->getLocation ( ) );
		cocos2d::Size size = target->getContentSize ( );
		cocos2d::Rect rect = cocos2d::Rect ( -size.width/2 , -size.height/2 , size.width , size.height );
		cocos2d::log ( "test setupTouchEvents" );
		if ( rect.containsPoint ( locationInNode ) ) {
			target->setScale ( 0.98f ); 
			return true;
		}
		return false;
		};

	listener->onTouchEnded = [this]( cocos2d::Touch* touch , cocos2d::Event* event ) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget ( ));
		target->setScale ( 1 ); 

		if ( _clickCallback ) {
			_clickCallback ( _cardId );
		}
		};

	cocos2d::Director::getInstance ( )->getEventDispatcher ( )->addEventListenerWithSceneGraphPriority ( listener , this );
}

void CardView::playMoveAnimation ( const cocos2d::Vec2& target , float duration ,
	std::function<void ( )> onComplete ) {
	// 发现有顶部牌遮挡，需要重新排序
	if ( this->getParent ( ) ) {
		this->getParent ( )->reorderChild ( this , INT_MAX );
	}
	auto move = cocos2d::MoveTo::create ( duration , target );
	auto sequence = cocos2d::Sequence::create ( 
		move,
		cocos2d::CallFunc::create ( [onComplete]( ) {
			if ( onComplete ) onComplete ( );
			} ) ,
		nullptr
	);

	this->runAction ( sequence );
}

CardView* CardView::create ( const  CardModel& model ) {
	CardView* view = new CardView ();
	if ( view && view->init ( model ) ) {
		view->autorelease ( );
		return view;
	}
	CC_SAFE_DELETE ( view );
	return nullptr;
}