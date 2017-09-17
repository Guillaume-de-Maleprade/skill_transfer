#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <map>
#include <vector>
#include <giskard_core/giskard_core.hpp>
#include <kdl_conversions/kdl_msg.h>

template <class T, class U>
inline std::map<T, U> toMap(const std::vector<T> &keys, const std::vector<U> &values)
{
  // FIXME: move move to another package
  if (keys.size() != values.size())
    throw std::runtime_error("Number of keys not equal to numbers of values.");

  std::map<T, U> result;
  for (size_t i = 0; i < keys.size(); ++i)
    result.insert(std::pair<T, U>(keys[i], values[i]));

  return result;
}

inline Eigen::VectorXd msgPoseToEigenVector(const geometry_msgs::Pose &pose)
{
  // FIXME: refactor this into a header file
  KDL::Frame frame;
  tf::poseMsgToKDL(pose, frame);

  Eigen::VectorXd result(6);

  result(0) = pose.position.x;
  result(1) = pose.position.y;
  result(2) = pose.position.z;

  KDL::Rotation::Quaternion(
      pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w)
      .GetEulerZYX(result(3), result(4),
                   result(5));

  return result;
}

inline Eigen::VectorXd kdlFrameToEigenVector(const KDL::Frame &frame)
{
  Eigen::VectorXd result(6);

  result(0) = frame.p.x();
  result(1) = frame.p.y();
  result(2) = frame.p.z();
  frame.M.GetEulerZYX(result(3), result(4), result(5));

  return result;
}

inline std::vector<double> eigenVectorToStdVector(const Eigen::VectorXd &v)
{
  // FIXME: where to put this?
  std::vector<double> result;
  for (Eigen::Index i = 0; i < v.rows(); ++i)
    result.push_back(v(i));

  return result;
}

inline geometry_msgs::Twist eigenVectorToMsgTwist(const Eigen::VectorXd &t)
{
  // FIXME: where to put this?
  if (t.rows() != 6)
    throw std::runtime_error("Did not receive vector representing a twist with 6 values.");

  geometry_msgs::Twist result;

  result.linear.x = t(0);
  result.linear.y = t(1);
  result.linear.z = t(2);
  result.angular.x = t(3);
  result.angular.y = t(4);
  result.angular.z = t(5);

  return result;
}

#endif