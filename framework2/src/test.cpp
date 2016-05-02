#include <iostream>

#include "GatewayInterface.h"

int main() {

	GatewayInterface gi;
	gi.sendSetState(1076990003909923, 13571638, 0, 1);
    
	return 0;
}