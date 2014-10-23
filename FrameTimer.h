/*
 *  SgMoveTimer.h
 *  FuegitoProject
 *
 *  Created by Colin Hunt on 12-05-24.
 *  Fuegito Project 2012 
 */

#include <time.h>
#include <sys/time.h>
#include <iostream>

#ifndef FRAME_TIMER_H
#define FRAME_TIMER_H

typedef double TimeVal;

class FrameTimer {
public:

  FrameTimer(TimeVal time = 0)
    : time(time),
      endTime(0),
      isRunning(false) {}
  
  void Set(TimeVal time) {
    this->time = time;
  }
  
  void Start() {
    endTime = CurrentTime() + time;
    isRunning = true;
  }
  
  void Stop() {
    time = TimeLeft();
    isRunning = false;
  }
  
  TimeVal TimeLeft() const {
    if (isRunning)
      return endTime - CurrentTime();
    else
      return time;
  }
  
private:
  
  TimeVal time;
  
  TimeVal endTime;
  
  bool isRunning;
    
  TimeVal CurrentTime() const {
      timeval tv;
      gettimeofday(&tv, NULL);
      TimeVal usecs = static_cast<TimeVal>(tv.tv_usec) / 1000000;
      TimeVal secs = static_cast<TimeVal>(tv.tv_sec);
//      cout << "curent time: " << secs + usecs << endl;
    return secs + usecs;
  }

};

#endif // FRAME_TIMER_H
