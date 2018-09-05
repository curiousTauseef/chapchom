#ifndef CCFACTORYTIMESTEPPER_H
#define CCFACTORYTIMESTEPPER_H

// Include the integration methods (time steppers)
#include "ac_time_stepper.h"
#include "cc_euler_method.h"
#include "cc_RK4_method.h"

namespace chapchom
{

 /// @class CCFactoryTimeStepper cc_factory_time_stepper.h

 /// This class implements a factory for the integration methods (time
 /// steppers)

 class CCFactoryTimeStepper
 {
 
 public:
 
  /// Empty constructor
  CCFactoryTimeStepper();
 
  /// Empty destructor
  virtual ~CCFactoryTimeStepper();
 
  /// Returns the specified time stepper (integration method)
  ACTimeStepper* create_time_stepper(std::string time_stepper_name);
 
 protected:
 
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  CCFactoryTimeStepper(const CCFactoryTimeStepper &copy)
   {
    BrokenCopy::broken_copy("CCFactoryTimeStepper");
   }
 
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCFactoryTimeStepper &copy)
   {
    BrokenCopy::broken_assign("CCFactoryTimeStepper");
   }
 
 };

}
 
#endif // #ifndef CCFACTORYTIMESTEPPER_H