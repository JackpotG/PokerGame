#include "UndoModel.h"
void UndoModel::pushRecord ( const UndoRecord& record ) {
	if ( _records.size ( ) >= MAX_RECORDS ) {
		_records.erase ( _records.begin ( ) );
	}
	_records.push_back ( record );
}

UndoRecord UndoModel::popRecord ( ) {
	if ( _records.empty ( ) ) {
		return UndoRecord{};
	}
	UndoRecord record = _records.back ( );
	_records.pop_back ( );
	return record;
}

bool UndoModel::hasRecords ( ) const {
	return !_records.empty ( );
}

void UndoModel::clear ( ) {
	_records.clear ( );
}