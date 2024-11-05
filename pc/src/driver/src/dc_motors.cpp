
#include "dc_motor_wiring_pi.hpp"
#include <std_msgs/Float64.h>

#define MOTOR_1_PIN_D 16
#define MOTOR_1_PIN_E 19
#define MOTOR_1_PIN_S 26
#define MOTOR_2_PIN_D 12
#define MOTOR_2_PIN_E 13
#define MOTOR_2_PIN_S 6

Motor left_motor(MOTOR_1_PIN_D, MOTOR_1_PIN_E, MOTOR_1_PIN_S, false);
Motor right_motor(MOTOR_2_PIN_D, MOTOR_2_PIN_E, MOTOR_1_PIN_S, false);

void leftMotorCallback(const std_msgs::Float64& msg) {
	left_motor.set(msg.data * 100);
}

void rightMotorCallback(const std_msgs::Float64& msg) {
	right_motor.set(msg.data * 100);
	
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "motors");
	ros::NodeHandle node;
	ros::Subscriber left_motor_target_vel_sub = node.subscribe("/left_wheel/pwm", 1, &leftMotorCallback);
	ros::Subscriber right_motor_target_vel_sub = node.subscribe("/right_wheel/pwm", 1, &rightMotorCallback);
	ros::spin();
	return 0;
}
