/*
 * PID Controller
 * Author: Pranav Srinivas Kumar
 * Date: 2015.06.23
 */

#include <iostream>

class PID
{
public:
  PID(double p, 
      double i, 
      double d, 
      double derivator, 
      double integrator,
      double integrator_max,
      double integrator_min);
  double update(double current_value);
  void setPoint(double set_point);
  void setIntegrator(double integrator);
  void setDerivator(double derivator);
  void setKp(double kp);
  void setKi(double ki);
  void setKd(double kd);
  double getPoint();
  double getError();
  double getIntegrator();
  double getDerivator();
private:
  double kp_;
  double ki_;
  double kd_;
  double derivator_;
  double integrator_;
  double integrator_min_;
  double integrator_max_;
  double error_;
  double set_point_;
  double current_value_;
  double p_value_;
  double d_value_;
  double i_value_;
  double pid;
};
