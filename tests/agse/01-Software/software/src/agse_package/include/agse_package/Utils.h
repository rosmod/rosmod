/*
 * Utils.h
 *
 *  Created on: Mar 3, 2012
 *      Author: jose
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <unistd.h>

class Utils {
public:
	static void sleepMS(int ms) {
		sleepMS((long)ms);
	}

	static void sleepMS(long ms) {
		usleep(ms*1000);
	}
};

#endif /* UTILS_H_ */
