#include "turretMainLoop.hpp"
#include "clockFix.h"
// C standard includes.
#include <stdio.h>
#include <unistd.h>

// micro-ROS general includes with general functionality.
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rosidl_runtime_c/message_type_support_struct.h>

#include <std_msgs/msg/string.h>
#include <std_msgs/msg/bool.h>
#include <geometry_msgs/msg/twist.h>
#include <pico/time.h>
#include <pico/types.h>



// got a lot of help from https://docs.vulcanexus.org/en/humble/rst/microros_documentation/getting_started/getting_started.html

rcl_publisher_t LeftLimitPublisher;
rcl_publisher_t RightLimitPublisher;
rcl_publisher_t currentAnglesPublisher;
std_msgs__msg__Bool leftMsg, rightMsg;
geometry_msgs__msg__Twist positionMsg;

struct timespec ts;


const char *nodeName = "AutoTurret";
const char *nodeNameSpace = "pico";



const rosidl_message_type_support_t *LimitPublisherSupport =
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool); // this just fetches the definition of the message to help publish later

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{

    if (timer != NULL)
    {

        leftMsg.data = true;
        rcl_publish(&LeftLimitPublisher, &leftMsg, NULL);
    }
}

int main()
{

    // I think this just is the thing that allocates all the memory the node will use but I'm unsure
    rcl_allocator_t allocator = rcl_get_default_allocator();

    // not sure but it seems to initilaze things like the clock.
    rclc_support_t support;
    rclc_support_init(&support, 0, NULL, &allocator);

    rcl_timer_t timer;
    rclc_timer_init_default(&timer, &support, 100000, &timer_callback);

    //the actual node
    rcl_node_t node;
    rclc_node_init_default(&node, nodeName, nodeNameSpace, &support);

    rclc_publisher_init_default(&LeftLimitPublisher, &node, LimitPublisherSupport, nodeName);

    rclc_executor_t executor;
    const size_t numHandles = 3;
    rclc_executor_init(&executor, &support.context,1, &allocator );
    rclc_executor_add_timer(&executor, &timer);

    rclc_executor_spin(&executor);


    return 1;


}