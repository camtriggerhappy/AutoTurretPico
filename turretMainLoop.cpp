#include "turretMainLoop.hpp"
#include "clockFix.h"
// C standard includes.
#include <stdio.h>
#include <unistd.h>
#include <pico/stdlib.h>

// micro-ROS general includes with general functionality.
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rosidl_runtime_c/message_type_support_struct.h>

#include <std_msgs/msg/string.h>
#include <std_msgs/msg/bool.h>
#include <rmw_microros/rmw_microros.h>
#include <geometry_msgs/msg/twist.h>

#include <pico_uart_transport.h>
#include <uxr/client/profile/transport/custom/custom_transport.h>



bool pico_serial_transport_open(struct uxrCustomTransport * transport)
{
    // Ensure that stdio_init_all is only called once on the runtime
    static bool require_init = true;
    if(require_init)
    {
        stdio_init_all();
        require_init = false;
    }

    return true;
}

bool pico_serial_transport_close(struct uxrCustomTransport * transport)
{
    return true;
}

size_t pico_serial_transport_write(struct uxrCustomTransport * transport, unsigned char const *buf, unsigned int len, unsigned char *errcode)
{
    for (size_t i = 0; i < len; i++)
    {
        if (buf[i] != putchar(buf[i]))
        {
            *errcode = 1;
            return i;
        }
    }
    return len;
}

size_t pico_serial_transport_read(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode)
{
    uint64_t start_time_us = time_us_64();
    for (size_t i = 0; i < len; i++)
    {
        int64_t elapsed_time_us = timeout * 1000 - (time_us_64() - start_time_us);
        if (elapsed_time_us < 0)
        {
            *errcode = 1;
            return i;
        }

        int character = getchar_timeout_us(elapsed_time_us);
        if (character == PICO_ERROR_TIMEOUT)
        {
            *errcode = 1;
            return i;
        }
        buf[i] = character;
    }
    return len;
}


// got a lot of help from https://docs.vulcanexus.org/en/humble/rst/microros_documentation/getting_started/getting_started.html

rcl_publisher_t LeftLimitPublisher;
rcl_publisher_t RightLimitPublisher;
rcl_publisher_t currentAnglesPublisher;
std_msgs__msg__Bool leftMsg, rightMsg;
geometry_msgs__msg__Twist positionMsg;

struct timespec ts;
const uint LED_PIN = 25;


const char *nodeName = "AutoTurret";
const char *nodeNameSpace = "";



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
        rmw_uros_set_custom_transport(
		true,
		NULL,
		pico_serial_transport_open,
		pico_serial_transport_close,
		pico_serial_transport_write,
		pico_serial_transport_read
	);

        gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    const int timeout_ms = 1000; 
    const uint8_t attempts = 120;

    rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);


    // I think this just is the thing that allocates all the memory the node will use but I'm unsure
    rcl_allocator_t allocator = rcl_get_default_allocator();

    // not sure but it seems to initilaze things like the clock.
    rclc_support_t support;
    rclc_support_init(&support, 0, NULL, &allocator);

    rcl_timer_t timer;
    rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(1000), &timer_callback);

    //the actual node
    rcl_node_t node;
    rclc_node_init_default(&node, nodeName, nodeNameSpace, &support);
    //don't put a space in the name
    rclc_publisher_init_default(&LeftLimitPublisher, &node, LimitPublisherSupport, "leftLimit");

    rclc_executor_t executor;
    const size_t numHandles = 3;
    rclc_executor_init(&executor, &support.context,1, &allocator );
    rclc_executor_add_timer(&executor, &timer);

    rclc_executor_spin(&executor);


    return 1;


}