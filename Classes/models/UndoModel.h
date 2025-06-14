#pragma once  
#include <functional>  
#include <vector>  
#include "cocos2d.h" // 添加此行以确保正确包含cocos2d命名空间

enum class UndoActionType {  
	HAND_REPLACE ,   // 手牌替换  
	DESKTOP_MATCH   // 桌面匹配  
};  

struct UndoRecord {  
	UndoActionType actionType;  
	int movedCardId;           
	int targetCardId;           
	cocos2d::Vec2 originalPos;  // 原始位置  
	cocos2d::Vec2 stackTopPos;  // 回退定位  

	std::function<void ( )> executeUndo;  
};  

class UndoModel {  
public:  
	void pushRecord ( const UndoRecord& record );  
	UndoRecord popRecord ( );  
	bool hasRecords ( ) const;  
	void clear ( );  

private:  
	std::vector<UndoRecord> _records;  
	const size_t MAX_RECORDS = 100;   
};