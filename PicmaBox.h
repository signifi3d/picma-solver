#pragma once

enum boxState { OPEN, FILLED, CLOSED };

struct PicmaBox {
	boxState currentState = OPEN;	
};
