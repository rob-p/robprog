#include "ezETAProgressBar.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
	int i;
	int n = 5;
	ez::ezETAProgressBar eta(n);

	eta.start();
	for(i=0; i <= n; ++i, ++eta) {
	#ifdef WIN32
		Sleep(1000);
	#else
		usleep(500000);
	#endif
	}

	n = 99999;
	//eta.n = n;
	eta.reset(n);
	eta.start();
	for(i=0; i <= 15; ++i, ++eta) {
	#ifdef WIN32
		Sleep(1000);
	#else
		sleep(1);
	#endif
	}

	return 0;
}
