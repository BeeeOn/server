#include <iostream>

#include "GatewayInterface.h"

int main()
{  
  	try
  	{
		GatewayInterface gi;
		gi.sendSetState(1076990003909923, 2728173022, 1, 10);
	}
  	catch (std::exception& e) {
    		std::cerr << "Exception: " << e.what() << "\n";
  	}
	return 0;
}
