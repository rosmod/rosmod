/*
 * PID Controller
 * Author: Pranav Srinivas Kumar
 * Date: 2015.06.23
 */

#include "rover_pkg/libpid.hpp"

libpid::libpid()
{
  kp_ = ki_ = kd_ = derivator_ = integrator_ = integrator_max_ = integrator_min_ = set_point_ = 0;
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
  double error = set_point_ - current_value;
  double p_value = kp_ * error;
  double d_value = kd_ * (error - derivator_);
  derivator_ = error;
  integrator_ = integrator_ + error;
  if (integrator_ > integrator_max_)
    integrator_ = integrator_max_;
  else if (integrator_ < integrator_min_)
    integrator_ = integrator_min_;
  double i_value = integrator_ * ki_;
  double pid = p_value + i_value + d_value;
  return pid;  
}

void libpid::setPoint(double set_point) {
  set_point_ = set_point;
}

void libpid::setIntegrator(double integrator) {
  integrator_ = integrator;
}

void libpid::setIntegratorMax(double max) {
  integrator_max_ = max;
}

void libpid::setIntegratorMin(double min) {
  integrator_min_ = min;
}

void libpid::setKp(double kp) {
  kp_ = kp;
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

double libpid::getIntegrator() {
  return integrator_;
}

double libpid::getDerivator() {
  return derivator_;
}
