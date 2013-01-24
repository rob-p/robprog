/*
Copyright (C) 2011,2012 Remik Ziemlinski. See MIT-LICENSE.

CHANGELOG

v0.0.0 20110502 rsz Created.
V1.0.0 20110522 rsz Extended to show eta with growing bar.
v2.0.0 20110525 rsz Added time elapsed.
v2.0.1 20111006 rsz Added default constructor value.
v2.0.2 20130123 rob Switched over to C++11 timer facilities 
*/

#ifndef EZ_ETAPROGRESSBAR_H
#define EZ_ETAPROGRESSBAR_H

#include <iostream>
#include <string>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <cassert>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/ioctl.h>
#endif

namespace ez {
// One-line refreshing progress bar inspired by wget that shows ETA (time remaining).
// 90% [##################################################     ] ETA 12d 23h 56s
class ezETAProgressBar {
private:
	typedef std::chrono::system_clock system_clock;
	typedef std::chrono::duration<size_t, system_clock::period> duration;
	typedef std::chrono::duration<double, system_clock::period> partialDuration;
public:
	ezETAProgressBar(unsigned int _n=0) : n(_n), pct(0), cur(0), width(80) {}
	void reset( uint64_t _n ) { n = _n; pct = 0; cur = 0; }
	void start() { 
		startTime = system_clock::now();//osQueryPerfomance();
		setPct(0); 
	}
	
	void operator++() {
		if (cur >= n) return;
		++cur;		
		setPct( static_cast<double>(cur)/n );
	};

	std::string durationString( duration t ) {
		using std::chrono::duration_cast;
		/*
		typedef std::chrono::duration<double, std::ratio<86400>> hours;
		typedef std::chrono::duration<double, std::ratio<3600>> hours;
		typedef std::chrono::duration<double, std::ratio<60>> minutes;
		typedef std::chrono::duration<double, std::ratio<1>> seconds;
		*/
	
		typedef std::chrono::duration<size_t, std::ratio<86400>> days;
		using std::chrono::hours;
		using std::chrono::minutes;
		using std::chrono::seconds;

		char tmp[8];
		std::string out;
		
		if ( t >= days(1) ) {
			auto numDays = duration_cast<days>(t);
			sprintf(tmp, "%dd ", numDays.count());
			out += tmp;
			t -= numDays;
		}
		
		if ( t >= hours(1) ) {
			auto numHours = duration_cast<hours>(t);
			sprintf(tmp, "%dh ", numHours.count());
			out += tmp;
			t -= numHours;
		}

		if ( t >= minutes(1) ) { 
			auto numMins = duration_cast<minutes>(t);
			sprintf(tmp, "%dm ", numMins.count());
			out += tmp;
			t -= numMins;
		}
		
		if ( t >= seconds(1) ) {
			auto numSecs = duration_cast<seconds>(t);
			sprintf(tmp, "%ds", numSecs);
			out += tmp;
		}
		
		if (out.empty()) { out = "0s"; }
			
		return out;
	}
	
	// Set 0.0-1.0, where 1.0 equals 100%.
	void setPct(double Pct) {
		using std::chrono::duration_cast;
		using std::chrono::seconds;
		typedef std::chrono::duration<size_t, std::ratio<604800>> weeks;

		endTime = system_clock::now();
		char pctstr[5];
		sprintf(pctstr, "%3d%%", (int)(100*Pct));

		// Compute how many tics we can display.
		int nticsMax = (width-27);
		int ntics = (int)(nticsMax*Pct);
		std::string out(pctstr);
		out.append(" [");
		out.append(ntics,'#');
		out.append(nticsMax-ntics,' ');
		out.append("] ");
		out.append((Pct<1.0) ? "ETA " : "in ");

		// Seconds.
		auto dt = endTime-startTime;

		std::string tstr;
		if (Pct >= 1.0) {
			// Print overall time and newline.
			tstr = durationString(dt);
			out.append(tstr);
			if (out.size() < width)
				out.append(width-out.size(),' ');

			out.append("\n");
			std::cout << out;
			return;
		} else {
			duration eta = std::chrono::seconds::max();

			if (Pct > 0.0) {
				duration esecs = duration_cast<seconds>(dt);
				eta = duration_cast<duration>( ((esecs * n) / cur) - esecs );
			}

			if ( eta > weeks(1) ) {
				out.append("> 1 week");
			} else {
				tstr = durationString(eta);
				out.append(tstr);
			}
		}

		// Pad end with spaces to overwrite previous string that may have been longer.
		if (out.size() < width)
			out.append(width-out.size(),' ');
			
		out.append("\r");
		std::cout << out;
		std::cout.flush();
	}

	private:
		uint64_t n;
		uint64_t cur;
	    unsigned short pct; // Stored as 0-1000, so 2.5% is encoded as 25.
	    unsigned char width; // How many chars the entire line can be.
	    std::chrono::system_clock::time_point startTime, endTime;
};
}
#endif // EZ_ETAPROGRESSBAR_H
