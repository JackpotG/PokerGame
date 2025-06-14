#pragma once
#include "models/UndoModel.h"

class UndoService {
public:
	static void addHandReplaceRecord ( UndoModel& model , int sourceCardId , int targetCardId );
	static void addDesktopMatchRecord ( UndoModel& model , int sourceCardId , int targetCardId );
};