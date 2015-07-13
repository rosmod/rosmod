/*
 * PID Controller
 * Author: Pranav Srinivas Kumar
 * Date: 2015.06.23
 */

#include "pid.hpp"

PID::PID(double p, 
	 double i, 
	 double d, 
	 double derivator, 
	 double integrator,
	 double integrator_max,
	 double integrator_min) {
  kp_ = p;
  ki_ = i;
  kd_ = d;
  derivator_ = derivator;
  integrator_ = integrator;
  integrator_max_ = integrator_max;
  integrator_min_ = integrator_min;
}

double PID::update(double current_value) {
  error_ = set_point_ - current_value_;
  p_value_ = kp_ * error_;
  d_value_ = kd_ * (error_ - derivator_);
  derivator_ = error_;
  integrator_ = integrator_ + error_;
  if (integrator_ > integrator_max_)
    integrator_ = integrator_max_;
  else if (integrator_ < integrator_min_)
    integrator_ = integrator_min_;
  i_value_ = integrator_ * ki_;
  pid = p_value_ + i_value_ + d_value_;
  return pid;  
}

void PID::setPoint(double set_point) {
  set_point_ = set_point;
}

void PID::setIntegrator(double integrator) {
  integrator_ = integrator;
}

void PID::setKp(double kd) {
  kd_ = kd;
}

void PID::setKi(double ki) {
  ki_ = ki;
}

void PID::setKd(double kd) {
  kd_ = kd;
}

double PID::getPoint() {
  return set_point_;
}

double PID::getError() {
  return error_; 
}

double PID::getIntegrator() {
  return integrator_;
}

double PID::getDerivator() {
  return derivator_;
}
