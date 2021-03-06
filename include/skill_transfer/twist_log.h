#ifndef TWIST_LOG_H
#define TWIST_LOG_H

#include <deque>
#include <geometry_msgs/Twist.h>

class TwistLog
{
public:
  TwistLog(unsigned int size);
  void push(geometry_msgs::Twist twist);
  void clear();
  bool allFilledAndBelowThreshold(double threshold);

protected:
  std::deque<geometry_msgs::Twist> log_;
  std::deque<geometry_msgs::Twist>::size_type size_;
};

#endif
