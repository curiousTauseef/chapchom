#ifndef CCODESFROMSENSORSTELITSL869DR_H
#define CCODESFROMSENSORSTELITSL869DR_H

// Include general/common includes, utilities and initialisation
#include "../../../src/general/common_includes.h"
#include "../../../src/general/utilities.h"
#include "../../../src/general/initialise.h"
// The class implementing the interfaces for the ODEs
#include "../../../src/odes/ac_odes.h"

// The nmea decoder
#include "cc_nmea_decoder.h"

#define X_MIN -32768
#define X_MAX 32767
#define FX_MIN_ACC -2.0
#define FX_MAX_ACC 2.0
#define FX_MIN_GYRO -250.0
#define FX_MAX_GYRO 250.0
#define SIZE_BLOCK_DATA 15
#define DIM 3

namespace chapchom
{
 
 // In charge of mapping the input value to the new scale (used by the
 // class CCODEsFromSensorsTelitSL869DR to transform the data read from
 // the NMEA strings to 'real')
 double scale(double x_min, double x_max, double fx_min, double fx_max, double x);
 
 /// \class CCODEsFromSensorsTelitSL869DR cc_odes_from_sensors_TelitSL869-DR.h
 
 /// This class implements a set of odes from a Table. It inherits the
 /// interface to define ODEs from the ACODEs class
 class CCODEsFromSensorsTelitSL869DR : public virtual ACODEs
 {
  
 public:
  
  /// Constructor, sets the number of odes
  CCODEsFromSensorsTelitSL869DR(const char *input_filename);
  
  /// Destructor
  virtual ~CCODEsFromSensorsTelitSL869DR();
  
  /// Get the values of the sensors at specific time
  bool get_sensors_lectures();
  
  // Get the number of acceleration data
  inline const unsigned nacceleration_data()
  {return Acceleration_data.size();}
  
  // Get acceleration data
  inline std::vector<double> &get_accelerations(const unsigned i)
  {return Acceleration_data[i];}
  
  // Get the number of gyro data
  inline const unsigned ngyro_data() {return Gyro_data.size();}
  
  // Get gyro's data
  inline std::vector<double> &get_angular_rates(const unsigned i)
  {return Gyro_data[i];}
  
  // Get the number of Euler angles data
  inline const unsigned neuler_angles_data()
  {return Euler_angles_data.size();}
  
  /// Get the values of the Euler angles
  inline std::vector<double> &get_euler_angles(const unsigned i)
  {return Euler_angles_data[i];}
  
  // Get access to the current data index
  inline unsigned current_data_index() const {return Current_data_index;}
  
  // Set the value of the current data index
  inline unsigned &current_data_index() {return Current_data_index;}
  
  // Get yaw correction as a function of time and the number of steps
  // per second
  const double get_yaw_correction(const double t,
                                  const double n_steps_per_second);
  
  /// Fills the matrix that performs the transformation from angular
  /// velocities to Euler-rates
  void fill_angular_velocities_to_euler_rates_matrix(std::vector<std::vector<double> > &A,
                                                     std::vector<double> &euler_angles);
  
  /// Multiplies a matrix times a vector
  void multiply_matrix_times_vector(std::vector<std::vector<double> > &A,
                                    std::vector<double> &b,
                                    std::vector<double> &x);
  
  /// Set linear acceleration for current time
  inline std::vector<double> &linear_acceleration() {return Linear_acceleration;}
 
  /// Evaluates the system of odes at the given time "t" and the values
  /// of the function in "y". The evaluation produces results in the dy
  /// vector
  void evaluate(const double t,
                const std::vector<double> &y, std::vector<double> &dy);
 
  /// Evaluates the specified ode by "i" of the system of odes at the
  /// given time "t" and the values of the function in "y". The
  /// evaluation produces results in the dy vector at the dy[i]
  /// position
  void evaluate(const unsigned i, const double t,
                const std::vector<double> &y, std::vector<double> &dy);
 
 protected:
  
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCODEsFromSensorsTelitSL869DR(const CCODEsFromSensorsTelitSL869DR &copy)
  : ACODEs(copy)
   {
    BrokenCopy::broken_copy("CCODEsFromSensorsTelitSL869DR");
   }
 
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCODEsFromSensorsTelitSL869DR &copy)
   {
    BrokenCopy::broken_assign("CCODEsFromSensorsTelitSL869DR");
   }
  
  /// In charge of "pairing/align" the data obtained from the sensors
  /// (accelerometers and gyro) via nearest value or interpolation
  /// such that both lectures correspond to the same time
  void pair_lectures();

  // Indicates the index of the lectures from sensors currently beeing
  // processed
  unsigned Current_data_index;
  
  // An nmea decoder
  CCNMEADecoder *nmea_decoder;
  
  // File handler
  std::ifstream Input_file;
  
  // A transformation matrix from angular velocities to Euler rates
  std::vector<std::vector<double> > A;
  
  // Stores linear acceleration
  std::vector<double> Linear_acceleration;

  // Temporary storage for acceleration data
  std::vector<std::vector<double> > Acceleration_data;
  // Temporary storage for gyroscope data
  std::vector<std::vector<double> > Gyro_data;
  // Temporary storage for Euler angles data
  std::vector<std::vector<double> > Euler_angles_data;
  
 };
 
}

#endif // #ifndef CCODESFROMSENSORSTELITSL869DR_H