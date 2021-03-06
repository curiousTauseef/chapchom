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

#define NFIELDS_NMEA_DECODER 50

#define X_MIN -32768
#define X_MAX 32767
#define FX_MIN_ACC -2.0
#define FX_MAX_ACC 2.0
#define FX_MIN_GYRO -250.0
#define FX_MAX_GYRO 250.0
//#define SIZE_BLOCK_DATA 15
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
  
  /// Computes north-east velocities
  void compute_north_east_velocities(const double x_vel,
                                     const double y_vel);
  
  // Get the number of acceleration data
  inline const unsigned nacceleration_data()
  {return Acceleration_data.size();}
  
  // Get acceleration data
  inline std::vector<double> &get_accelerations(const unsigned i)
  {return Acceleration_data[i];}
  
  // Get acceleration data
  inline std::vector<std::vector<double> > &get_accelerations()
  {return Acceleration_data;}
  
  // Get the number of gyro data
  inline const unsigned ngyro_data() {return Gyro_data.size();}
  
  // Get gyro's data
  inline std::vector<double> &get_angular_velocities(const unsigned i)
  {return Gyro_data[i];}
  
  // Get gyro's data
  inline std::vector<std::vector<double> > &get_angular_velocities()
  {return Gyro_data;}
  
  /// Get the value of the true course (in degrees)
  inline double true_course_in_degrees(){return True_course_in_degrees;}
  
  // Get the value of the speed (in knots)
  inline double speed_in_knots(){return Speed_in_knots;}
  
  // Get longitude
  inline double longitude(){return Longitude;}
  
  // Get latitude
  inline double latitude(){return Latitude;}
  
  // Get yaw correction as a function of time and the number of steps
  // per second
  const double get_yaw_correction(const double t,
                                  const double n_steps_per_second);
  
  /// Get north velocity
  inline double north_velocity() const {return North_velocity;}
  
  /// Get east velocity
  inline double east_velocity() const {return East_velocity;}
  
  /// Set linear acceleration for current time
  inline double *&linear_acceleration() {return Linear_acceleration;}
  
  /// Set Euler angles rates for current time
  inline double *&euler_angles_rates() {return Euler_angles_rates;}
  
  // Set the rate of change of yaw at the current time obtained after
  // applying a threshold to the gyro-z lecture
  inline double &yaw_change_rate_with_threshold()
  {return Yaw_change_rate_with_threshold;}
  
  /// Evaluates the system of odes at time "t". The values of the i-th
  /// function at previous times are accessible via y[i][1], y[i][2]
  /// and so on. The evaluation produces results in the vector dy.
  void evaluate(const double t,
                const std::vector<std::vector<double> > &y,
                std::vector<double> &dy);

  /// Evaluates the i-th ode at time "t". The values of the function
  /// at previous times are stores at y[1], y[2] and so on. The
  /// evaluation stores the result in dy.
  void evaluate(const unsigned i, const double t,
                const std::vector<double> &y, double &dy);
  
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
  /// such that both lectures correspond to the same time. This method
  /// is also in charge of scaling the raw values obtined from the
  /// lectures and transform them into 'real' values
  void pair_and_scale_lectures();
  
  /// Scales the raw data obtained from the sensores (gyros and
  /// accelerometers) based on the scaling provided in the
  /// documentation for Telit-Jupiter-SL869-DR (SL869-DR SW User Guide
  /// 1VV0301119 Rev. 1 2015-03-03 pages 17-18)
  void scale_raw_sensor_data();
  
  // An nmea decoder
  CCNMEADecoder *nmea_decoder;
  
  // File handler
  std::ifstream Input_file;
  
  // North-east velocities
  double North_velocity;
  double East_velocity;
  
  // Stores linear acceleration (to integrate)
  double *Linear_acceleration;
  
  // Stores Euler angles rates (to integrate)
  double *Euler_angles_rates;
  
  // Stores the rate of change of yaw obtained after applying a
  // threshold to the gyro-z lecture
  double Yaw_change_rate_with_threshold;
  
  // Temporary storage for acceleration data
  std::vector<std::vector<double> > Acceleration_data;
  // Temporary storage for gyroscope data
  std::vector<std::vector<double> > Gyro_data;
  // Temporary storage for true course (angle in degrees)
  double True_course_in_degrees;
  // Temporary storage for speed in knots
  double Speed_in_knots;
  // Store longitude from GPRMC NMEA string
  double Longitude;
  // Store latitude from GPRMC NMEA string
  double Latitude;
  
 };
 
}

#endif // #ifndef CCODESFROMSENSORSTELITSL869DR_H
