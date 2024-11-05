
#ifndef DC_MOTOR_WIRING_PI_HPP_
#define DC_MOTOR_WIRING_PI_HPP_

#include <ros/ros.h>
#include <wiringPi.h>

#define RPI_MAX_PWM_VALUE 1023

int16_t constrain(int64_t val, int64_t m1 = -RPI_MAX_PWM_VALUE, int64_t m2 = RPI_MAX_PWM_VALUE){
    if (val < std::min(m1, m2)) return std::min(m1, m2);
    if (val > std::max(m1, m2)) return std::max(m1, m2);
    return val;
}

class Motor {
public:
    Motor(int8_t direction_pin, int8_t enable_pin, int8_t stop_pin, bool inverted = 0);
    void set(int16_t val);
    void stop(bool state);
private:
    int8_t _direction_pin;
    int8_t _enable_pin;
    int8_t _stop_pin;
    bool _inverted;
};

Motor::Motor(int8_t direction_pin, int8_t enable_pin, int8_t stop_pin, bool inverted) {
    _direction_pin = direction_pin;
    _enable_pin = enable_pin;
    _stop_pin = stop_pin;
    _inverted = inverted;
    
    if (wiringPiSetupGpio() < 0) {
        ROS_ERROR("DCMotor wiringPi error: GPIO setup error");
        throw std::runtime_error("");
    }
    ROS_INFO("Motor setup started");
    pinMode(_direction_pin, OUTPUT);
    pinMode(_enable_pin, PWM_OUTPUT);
    pinMode(_stop_pin, OUTPUT);
    stop(0);
    ROS_INFO("Motor setup finished");
}
void Motor::set(int16_t speed) {
    if (speed == 0) {
        stop(true);
    } else {
        stop(false);

        bool dir = speed > 0;
        speed = abs(constrain(speed));

        if (_inverted) {
            dir = !dir;
        }
        
        digitalWrite(_direction_pin, dir);
        pwmWrite(_enable_pin, speed); 
    }
}
void Motor::stop(bool state) {
    if (state) {
        pwmWrite(_enable_pin, 0);
        digitalWrite(_stop_pin, 1);
        digitalWrite(_direction_pin, 0);
    } else {
        digitalWrite(_stop_pin, 0);
    }
}

#endif // DC_MOTOR_WIRING_PI_HPP_