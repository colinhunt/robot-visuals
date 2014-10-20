/*
 *  SgMoveTimer.h
 *  FuegitoProject
 *
 *  Created by Colin Hunt on 12-05-24.
 *  Fuegito Project 2012 
 */

#include <time.h>

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
    return static_cast<TimeVal>(clock()) / CLOCKS_PER_SEC;
  }

};

#endif // FRAME_TIMER_H
