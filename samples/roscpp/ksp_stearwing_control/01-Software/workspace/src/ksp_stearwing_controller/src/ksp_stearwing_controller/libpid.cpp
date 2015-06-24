/*
 * PID Controller
 * Author: Pranav Srinivas Kumar
 * Date: 2015.06.23
 */

#include "ksp_stearwing_controller/libpid.hpp"

libpid::libpid()
{
  kp_ = ki_ = kd_ = derivator_ = integrator_ = integrator_max_ = integrator_min_ = 0;
}

libpid::libpid(double p, 
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

double libpid::update(double current_value) {
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

void libpid::setPoint(double set_point) {
  set_point_ = set_point;
}

void libpid::setIntegrator(double integrator) {
  integrator_ = integrator;
}

void libpid::setKp(double kd) {
  kd_ = kd;
}

void libpid::setKi(double ki) {
  ki_ = ki;
}

void libpid::setKd(double kd) {
  kd_ = kd;
}

double libpid::getPoint() {
  return set_point_;
}

double libpid::getError() {
  return error_; 
}

double libpid::getIntegrator() {
  return integrator_;
}

double libpid::getDerivator() {
  return derivator_;
}
