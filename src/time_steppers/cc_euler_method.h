#ifndef CCEULERMETHOD_H
#define CCEULERMETHOD_H

#include "ac_time_stepper.h"

namespace chapchom
{

 /// @class CCEulerMethod cc_euler_method.h
 /// This class implements Euler's method to integrate ODE's
 class CCEulerMethod : public virtual ACTimeStepper
 {
 
 public:

  /// Constructor
  CCEulerMethod();
  
  /// Empty destructor
  virtual ~CCEulerMethod();
  
  /// Applies Eulers method to the given odes from the current time "t"
  /// to the time "t+h".
  void time_step(ACODEs &odes, const double h, const double t,
                 CCData<double> &u);
  
 protected:
 
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  CCEulerMethod(const CCEulerMethod &copy)
   : ACTimeStepper()
   {
    BrokenCopy::broken_copy("CCEulerMethod");
   }
 
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCEulerMethod &copy)
   {
    BrokenCopy::broken_assign("CCEulerMethod");
   }

 };

}
 
#endif // #ifndef CCEULERMETHOD_H