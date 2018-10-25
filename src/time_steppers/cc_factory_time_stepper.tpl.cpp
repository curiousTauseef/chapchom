#include "cc_factory_time_stepper.tpl.h"

namespace chapchom
{

 // ===================================================================
 // Empty constructor
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 CCFactoryTimeStepper<MAT_TYPE,VEC_TYPE>::CCFactoryTimeStepper()
 { 

 }

 // ===================================================================
 // Empty destructor
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 CCFactoryTimeStepper<MAT_TYPE,VEC_TYPE>::~CCFactoryTimeStepper()
 { 

 }

 // ===================================================================
 // Returns the specified time stepper (integration method)
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 ACTimeStepper* CCFactoryTimeStepper<MAT_TYPE,VEC_TYPE>::
 create_time_stepper(std::string time_stepper_name)
 {
  // Get the string and change it to lower case 
  std::transform(time_stepper_name.begin(), time_stepper_name.end(),
                 time_stepper_name.begin(), ::tolower);
  
  // ------------------------------------------------------
  // Check what time stepper method we need to create
  // ------------------------------------------------------
  // Euler method
  if (time_stepper_name.compare("euler")==0)
   {
    return new CCEulerMethod();
   }
  // Runge-Kutta 4 method
  else if (time_stepper_name.compare("rk4")==0)
   {
    return new CCRK4Method();
   }
  // Adams-Moulton 2 as Predictor-Corrector method
  else if (time_stepper_name.compare("am2pc")==0)
   {
    return new CCAdamsMoulton2PCMethod();
   }
  // Backward Euler method
  else if (time_stepper_name.compare("bdf1")==0)
   {
    return new CCBackwardEulerMethod<MAT_TYPE,VEC_TYPE>();
   }
  // Adams-Moulton 2 or Trapezoidal Rule method
  else if (time_stepper_name.compare("am2")==0)
   {
    return new CCAdamsMoulton2Method<MAT_TYPE,VEC_TYPE>();
   }
  else
   {
    std::ostringstream error_message;
    error_message << "The time stepper (integration method) you want to use is not "
                  << "implemented yet. Please implement it yourself or select"
                  << "another one\n\n"
                  << "Availables ones\n"
                  << "- Euler (euler)\n"
                  << "- Runge-Kutta 4 (rk4)\n"
                  << "- Adams-Moulton 2 - Predictor-Corrector (am2pc)\n"
                  << "- Backward Euler - Fully Implicit (bdf1)\n"
                  << "- Adams-Moulton 2 - Fully Implicit (am2)\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
 }

}
