// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sensor_msgs:msg/MultiEchoLaserScan.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "sensor_msgs/msg/multi_echo_laser_scan.h"


#ifndef SENSOR_MSGS__MSG__DETAIL__MULTI_ECHO_LASER_SCAN__STRUCT_H_
#define SENSOR_MSGS__MSG__DETAIL__MULTI_ECHO_LASER_SCAN__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'ranges'
// Member 'intensities'
#include "sensor_msgs/msg/detail/laser_echo__struct.h"

/// Struct defined in msg/MultiEchoLaserScan in the package sensor_msgs.
/**
  * Single scan from a multi-echo planar laser range-finder
  *
  * If you have another ranging device with different behavior (e.g. a sonar
  * array), please find or create a different message, since applications
  * will make fairly laser-specific assumptions about this data
 */
typedef struct sensor_msgs__msg__MultiEchoLaserScan
{
  /// timestamp in the header is the acquisition time of
  /// the first ray in the scan.
  ///
  /// in frame frame_id, angles are measured around
  /// the positive Z axis (counterclockwise, if Z is up)
  /// with zero angle being forward along the x axis
  std_msgs__msg__Header header;
  /// start angle of the scan
  float angle_min;
  /// end angle of the scan
  float angle_max;
  /// angular distance between measurements
  float angle_increment;
  /// time between measurements - if your scanner
  /// is moving, this will be used in interpolating position
  /// of 3d points
  float time_increment;
  /// time between scans
  float scan_time;
  /// minimum range value
  float range_min;
  /// maximum range value
  float range_max;
  /// range data
  /// (Note: NaNs, values < range_min or > range_max should be discarded)
  /// +Inf measurements are out of range
  /// -Inf measurements are too close to determine exact distance.
  sensor_msgs__msg__LaserEcho__Sequence ranges;
  /// intensity data.  If your
  /// device does not provide intensities, please leave
  /// the array empty.
  sensor_msgs__msg__LaserEcho__Sequence intensities;
} sensor_msgs__msg__MultiEchoLaserScan;

// Struct for a sequence of sensor_msgs__msg__MultiEchoLaserScan.
typedef struct sensor_msgs__msg__MultiEchoLaserScan__Sequence
{
  sensor_msgs__msg__MultiEchoLaserScan * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sensor_msgs__msg__MultiEchoLaserScan__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SENSOR_MSGS__MSG__DETAIL__MULTI_ECHO_LASER_SCAN__STRUCT_H_
