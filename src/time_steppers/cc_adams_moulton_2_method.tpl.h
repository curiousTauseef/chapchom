#ifndef CCADAMSMOULTON2METHOD_TPL_H
#define CCADAMSMOULTON2METHOD_TPL_H

#include "ac_time_stepper.h"

// Newton's method
#include "cc_newtons_method_for_adams_moulton_2.h"

// Time stepper to compute the initial guess for Newton's method
#include "cc_euler_method.h"

namespace chapchom
{
 
 /// @class CCAdamsMoulton2Method cc_adams_moulton_2_method.h This
 /// class implements Adams-Moulton or Trapezoidal Rule method to
 /// integrate ODE's
 template<class MAT_TYPE, class VEC_TYPE>
  class CCAdamsMoulton2Method : public virtual ACTimeStepper
 {
  
 public:
  
  /// Constructor
  CCAdamsMoulton2Method();
  
  /// Empty destructor
  virtual ~CCAdamsMoulton2Method();
  
  /// Applies Adams-Moulton 2 or Trapezoidal Rule method to the given
  /// odes from the current time "t" to the time "t+h".
  void time_step(ACODEs &odes, const Real h, const Real t,
                 CCData<Real> &u);
  
 protected:
  
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCAdamsMoulton2Method(const CCAdamsMoulton2Method<MAT_TYPE, VEC_TYPE> &copy)
  : ACTimeStepper()
   {
    BrokenCopy::broken_copy("CCAdamsMoulton2Method");
   }
 
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCAdamsMoulton2Method<MAT_TYPE, VEC_TYPE> &copy)
   {
    BrokenCopy::broken_assign("CCAdamsMoulton2Method");
   }
  
  // Newton's method for Adams-Moulton 2
  CCNewtonsMethodForAdamsMoulton2<MAT_TYPE, VEC_TYPE> Newtons_method;
  
  // The time stepper used to compute the initial guess
  CCEulerMethod Time_stepper_initial_guess;
  
 };
 
}

#endif // #ifndef CCADAMSMOULTON2METHOD_TPL_H