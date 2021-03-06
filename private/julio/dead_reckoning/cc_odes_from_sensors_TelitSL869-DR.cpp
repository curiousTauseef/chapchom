/** \file This file implements the CCODEsFromSensorsTelitSL869DR class
 */
#include "cc_odes_from_sensors_TelitSL869-DR.h"

namespace chapchom
{

 // ===================================================================
 // In charge of mapping the input value to the new scale (used by the
 // class CCODEsFromSensorsTelitSL869DR to transform the data read from
 // the NMEA strings to 'real')
 // ===================================================================
 double scale(double x_min, double x_max, double fx_min, double fx_max, double x)
 {
  const double s = (fx_max - fx_min) / (x_max - x_min);
  return ((x - x_min) * s) + fx_min;
 }
 
 // ===================================================================
 // Constructor, sets the number of odes. We currently have two odes,
 // one for the velocity and the other for the acceleration
 // ===================================================================
 CCODEsFromSensorsTelitSL869DR::
 CCODEsFromSensorsTelitSL869DR(const char *input_filename)
  : ACODEs(10), North_velocity(0.0), East_velocity(0.0)
 {
  // Open the file
  Input_file.open(input_filename, std::ios::in);
  if (Input_file.fail())
   {
    // Error message
   std::ostringstream error_message;
   error_message << "We could not open the NMEA strings - sensors file:["
                 << input_filename << "]" << std::endl;
   throw ChapchomLibError(error_message.str(),
                          CHAPCHOM_CURRENT_FUNCTION,
                          CHAPCHOM_EXCEPTION_LOCATION);
  }  
  
  // Initialise nmea parser
  nmea_decoder = new CCNMEADecoder(NFIELDS_NMEA_DECODER);
  
 }

 // ===================================================================
 // Destructor
 // ===================================================================
 CCODEsFromSensorsTelitSL869DR::~CCODEsFromSensorsTelitSL869DR()
 { 
  // Close the file
  Input_file.close();

  // Free memory for nmea
  delete nmea_decoder;
 }
 
 // ======================================================================
 /// Get the values of the sensors at specific time (computed from table)
 // ======================================================================
 bool CCODEsFromSensorsTelitSL869DR::get_sensors_lectures()
 {
  // Read data from the file/sensors. We proceed as follow:
  // -------------------------------------------------------------------
  // - State 1: Read a block of SIZE_BLOCK_DATA(15) PSTM3DACC data
  // -------------------------------------------------------------------
  // - State 2: Read a block of SIZE_BLOCK_DATA(15) PSTM3DGYRO data
  // -------------------------------------------------------------------
  
  // Extracted character
  char character;
  unsigned n_PSTM3DACC = 0;
  unsigned n_PSTM3DGYRO = 0;
  unsigned state = 0;
  // Flag to indicate whether GPRMC data has been read
  bool read_GPRMC_data = false;
  // Flag to indicate whether Euler angles data has been read
  bool read_Euler_angles_data = false;
  // Flag to continue looping
  bool LOOP = true;
  // Clear the input vectors
  Acceleration_data.clear();
  Gyro_data.clear();
  // Loop until eof
  while(LOOP && !Input_file.eof())
   {
    Input_file.get(character);
    //std::cerr << character;
    nmea_decoder->parse(character);
    // Check whether any of the data structures has new information
    if (nmea_decoder->is_accelerometer_data_ready())
     {
      // Is this the first time we are reading data from the accelerometer?
      if (state == 0)
       {
        state = 1;
       }
      else if (state == 3) // We got another acceleration data but no
                           // GPRMC data
       {
        // Get out of here
        LOOP = false;
        break;
       }
      
      // Get the data structure
      struct PSTM3DACC pstm3dacc = nmea_decoder->get_pstm3dacc();
      std::vector<double> read_acc(DIM+1);
      read_acc[0] = pstm3dacc.time;
      read_acc[1] = pstm3dacc.acc_x;
      read_acc[2] = pstm3dacc.acc_y;
      read_acc[3] = pstm3dacc.acc_z;      
      // Consume accelerometer data
      nmea_decoder->consume_accelerometer_data();
      // Add the reading to the acceleration data
      Acceleration_data.push_back(read_acc);
      // Increase the counter of the number of read acceleration data
      n_PSTM3DACC++;
      //      std::cout << "Time:" << read_acc[0]
      //                << " Accelerometer:(" << read_acc[1] << ", " << read_acc[2] << ", " <<  read_acc[3] << ")"
      //                << std::endl;
     }
    
    if (nmea_decoder->is_gyro_data_ready())
     {
      // Is this the first time we are reading data from the gyro?
      if (state == 1)
       {
        state = 2;
       }
      
      // Get the data structure
      struct PSTM3DGYRO pstm3dgyro = nmea_decoder->get_pstm3dgyro();
      std::vector<double> read_gyro(DIM+1);
      read_gyro[0] = pstm3dgyro.time;
      read_gyro[1] = pstm3dgyro.raw_x;
      read_gyro[2] = pstm3dgyro.raw_y;
      read_gyro[3] = pstm3dgyro.raw_z;
      // Consume gyro data
      nmea_decoder->consume_gyro_data();
      // Add the reading to the gyro data
      Gyro_data.push_back(read_gyro);
      // Increase the counter of the number of read acceleration data
      n_PSTM3DGYRO++;
      //      std::cout << "Time:" << read_gyro[0]
      //                << " Gyro:(" << read_gyro[1] << ", " << read_gyro[2] << ", " <<  read_gyro[3] << ")"
      //                << std::endl;
     }
    
    // TODO: GPRMC data
    read_GPRMC_data = false;
    
    if (nmea_decoder->is_GPRMC_data_ready())
     {
      // Is this the first time we are reading GPRMC data?
      if (state == 2)
       {
        state = 3;
       }
      
      // Get the data structure
      struct GPRMC gprmc = nmea_decoder->get_gprmc();
      // Get longitude
      Longitude = gprmc.longitude;
      // Get latitude
      Latitude = gprmc.latitude;
      // Consume GPRMC data
      nmea_decoder->consume_GPRMC_data();
      // Set the true course variable
      True_course_in_degrees = gprmc.course_degrees;
      // Speed in knots
      Speed_in_knots = gprmc.speed_knots;
      // Indicate GPRMC data has been read
      read_GPRMC_data = true;
     }
    
    // TODO: Read Euler angles data
    read_Euler_angles_data = true;
    
    // If we have read the corresponding accelerometer, gyro, GPRMC
    // and Euler angles data then break the cycle
    if (state == 3 && read_GPRMC_data && read_Euler_angles_data)
     {
      LOOP = false;
     }
    
   }
  
  // If we finished the loop because there is no more data return
  // false to stop execution
  if (LOOP)
   {
    return false;
   }
  
  // Align/pair and scale lectures such that accelerometers and gyro
  // data are given at the same time
  //pair_and_scale_lectures();

  // -----------------------------------------------------------------
  // Scale the raw sensor data based on the values provided in the
  // documentation
  // -----------------------------------------------------------------
  scale_raw_sensor_data();
  
  return true;
  
 }
 
 // ===================================================================
 /// Computes north-east velocities
 // ===================================================================
 void CCODEsFromSensorsTelitSL869DR::
 compute_north_east_velocities(const double x_vel, const double y_vel)
 {
  // Get the direction angle (True_course_in_degrees) into radians
  const double theta = True_course_in_degrees * TO_RADIANS;
  // Based on the value of the angle proceed as follows
  
  if (theta >= 0) // Positive or equal to zero angle
   {
    if (theta >= M_PI) // Second quadrant
     {      
      East_velocity =
       x_vel * sin(M_PI - theta) + y_vel * sin(theta - M_PI * 0.5);
      
      North_velocity =
       -x_vel * cos(M_PI - theta) + y_vel * cos(theta - M_PI * 0.5);
     }
    else if (theta <= 2*M_PI) // First quadrant
     {
      East_velocity = x_vel * sin(theta) - y_vel * cos(theta);
      North_velocity = x_vel * cos(theta) + y_vel * sin(theta);
     }
    else // The value of the angle is in the third or forth quadrant
         // but it should have been represented as a negative angle
     {
      // Error message
      std::ostringstream error_message;
      error_message << "The angle is positive but larger than 2pi, we can\n"
                    << "not handle that case since it should have been\n"
                    << "represented as a negative angle\n"
                    << "theta: " << theta
                    << std::endl;
      
      throw ChapchomLibError(error_message.str(),
                             CHAPCHOM_CURRENT_FUNCTION,
                             CHAPCHOM_EXCEPTION_LOCATION);
     }
    
   }
  else  // Negative angle
   {
    if (theta >= -M_PI) // Third quadrant
     {
      East_velocity = x_vel * sin(theta) - y_vel * cos(theta);
      North_velocity = x_vel * cos(theta) + y_vel * sin(theta);
     }
    else if (theta >= -2*M_PI) // Fourth quadrant
     {
      East_velocity =
       -x_vel * sin(M_PI + theta) + y_vel * cos(M_PI + theta);
      North_velocity = -x_vel * cos(M_PI + theta) - y_vel * sin(M_PI + theta);
     }
    else // The value of the angle is in the first or second quadrant
         // but it should have been represented as a positive angle
     {
      // Error message
      std::ostringstream error_message;
      error_message << "The angle is negative but smaller than -2pi, we can\n"
                    << "not handle that case since it should have been\n"
                    << "represented as cva positive angle\n"
                    << "theta: " << theta
                    << std::endl;
      
      throw ChapchomLibError(error_message.str(),
                             CHAPCHOM_CURRENT_FUNCTION,
                             CHAPCHOM_EXCEPTION_LOCATION);
     }
    
   }
  
 }
 
 // ===================================================================
 // Get yaw correction as a function of time and the number of steps
 // per second
 // ===================================================================
 const double CCODEsFromSensorsTelitSL869DR::get_yaw_correction(const double t,
                                                                const double n_steps_per_second)
 {
  // Error message
  std::ostringstream error_message;
  error_message << "Implement this" << std::endl;
  throw ChapchomLibError(error_message.str(),
                         CHAPCHOM_CURRENT_FUNCTION,
                         CHAPCHOM_EXCEPTION_LOCATION);
  
  // Check in which interval is "t"
  if (t >= 0 && t < 30.0)
   {
    return 2.0 * 0.006544985/n_steps_per_second; // 0.75 degreess per second
    //0.013089969/n_steps_per_second;
   }
  else if (t >= 30.0 && t < 60.0)
   {
    return 2.2 * 0.006544985/n_steps_per_second; // 0.75 degreess per second
    //0.013089969/n_steps_per_second;
   }
  else
   {
    return 2.5 * 0.006544985/n_steps_per_second; // 0.75 degreess per second
   }
  
 }
 
 // ===================================================================
 // Evaluates the system of odes at time "t". The values of the i-th
 // function at previous times are accessible via y[i][1], y[i][2]
 // and so on. The evaluation produces results in the vector dy.
 // ===================================================================
 void CCODEsFromSensorsTelitSL869DR::evaluate(const double t,
                                              const std::vector<std::vector<double> > &y,
                                              std::vector<double> &dy)
 {
  // -----------------
  // y[0][0] Current x-position
  // y[1][0] Current x-velocity
  // y[2][0] Current y-position
  // y[3][0] Current y-velocity
  // y[4][0] Current z-position
  // y[5][0] Current z-velocity
  // y[6][0] Current roll
  // y[7][0] Current pitch
  // y[8][0] Current yaw
  // y[9][0] Current yaw with threshold
  // -----------------
  // dy[0] x-velocity
  // dy[1] x-acceleration
  // dy[2] y-velocity
  // dy[3] y-acceleration
  // dy[4] z-velocity
  // dy[5] z-acceleration
  // dy[6] droll
  // dy[7] dpitch
  // dy[8] dyaw
  // dy[9] dyaw with threshold
  
  dy[0] = y[1][0];
  //dy[0] = North_velocity;
  dy[1] = Linear_acceleration[0];
  dy[2] = y[3][0];
  //dy[2] = East_velocity;
  dy[3] = Linear_acceleration[1];
  dy[4] = y[5][0];
  dy[5] = Linear_acceleration[2];
  dy[6] = Euler_angles_rates[0];
  dy[7] = Euler_angles_rates[1];
  dy[8] = Euler_angles_rates[2];
  dy[9] = Yaw_change_rate_with_threshold;
  
 }
 
 // ===================================================================
 // Evaluates the i-th ode at time "t". The values of the function at
 // previous times are stores at y[1], y[2] and so on. The evaluation
 // stores the result in dy.
 // ===================================================================
 void CCODEsFromSensorsTelitSL869DR::evaluate(const unsigned i,
                                              const double t,
                                              const std::vector<double> &y,
                                              double &dy)
 {
  // Error message
  std::ostringstream error_message;
  error_message << "This method is not implemented for this class"
                << std::endl;
  throw ChapchomLibError(error_message.str(),
                         CHAPCHOM_CURRENT_FUNCTION,
                         CHAPCHOM_EXCEPTION_LOCATION);
 }
 
 // ===================================================================
 /// Scales the raw data obtained from the sensores (gyros and
 /// accelerometers) based on the scaling provided in the
 /// documentation for Telit-Jupiter-SL869-DR (SL869-DR SW User Guide
 /// 1VV0301119 Rev. 1 2015-03-03 pages 17-18)
 // ===================================================================
 void CCODEsFromSensorsTelitSL869DR::scale_raw_sensor_data()
 {
  // Get the number of lectures for gyro and accelerometer
  const unsigned n_gyro_data = Gyro_data.size();  
  const unsigned n_acc_data = Acceleration_data.size();
  
  // Scale gyros data
  for (unsigned i = 0; i < n_gyro_data; i++)
   {
    // Transform to seconds
    Gyro_data[i][0]*= 1.0e-6;
    for (unsigned j = 1; j < DIM+1; j++) // j=0 is the time stamp
     {
      // Transform to radians since they are given in degrees
      Gyro_data[i][j] = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, Gyro_data[i][j]) * TO_RADIANS;
     }  
   }
  
  // Scale accelometers data
  for (unsigned i = 0; i < n_acc_data; i++)
   {
    // Transform to seconds
    Acceleration_data[i][0]*=1.0e-6;
    for (unsigned j = 1; j < DIM+1; j++) // j=0 is the time stamp
     {
      Acceleration_data[i][j] = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, Acceleration_data[i][j]);
     }
   }
  
 }
 
 // ===================================================================
 /// In charge of "pairing/align" the data obtained from the sensors
 /// (accelerometers and gyro) via nearest value or interpolation
 /// such that both lectures correspond to the same time. This method
 /// is also in charge of scaling the raw values obtined from the
 /// lectures and transform them into 'real' values
 // ===================================================================  
 void CCODEsFromSensorsTelitSL869DR::pair_and_scale_lectures()
 {
  // Get the number of lectures for accelerometer and gyro
  const unsigned n_acc_data = Acceleration_data.size();
  const unsigned n_gyro_data = Gyro_data.size();
  // Most of the times we have the same number of lectures per
  // sensors, additionally, the gyro's lecture comes in between two
  // accelerometers lectures, if that is the case we interpolate the
  // accelerometer's lectures to the gyro's time, otherwise, we only
  // copy the nearest data from the accelerometers
  
  // Get the minimum number of lectures
  double n_min_data = 0.0;
  if (n_acc_data <= n_gyro_data)
   {
    n_min_data = n_acc_data;
   }
  else if (n_acc_data > n_gyro_data)
   {
    n_min_data = n_gyro_data;
   }
  
  // Generate vectors with the same number of data
  std::vector<std::vector<double> > tmp_acc_data;
  std::vector<std::vector<double> > tmp_gyro_data;
  for (unsigned i = 0; i < n_min_data-1; i++)
   {
    const double t = Gyro_data[i][0];
    // Push back gyros data
    tmp_gyro_data.push_back(Gyro_data[i]);
    // Get the times for the acceleration readings
    const double t_acc0 = Acceleration_data[i][0];
    const double t_acc1 = Acceleration_data[i+1][0];
    // Can we interpolate?
    if (t >= t_acc0 && t <= t_acc1)
     {
      std::vector<double> interpolated_data(DIM+1);
      interpolated_data[0] = t;
      const double dt = t_acc1 - t_acc0;
      // Linear interpolation (for each dimension)
      for (unsigned j = 1; j < DIM+1; j++)
       {
        // Using divided differences notation
        const double a0 = Acceleration_data[i][j];
        const double a1 =
         (Acceleration_data[i+1][j] - Acceleration_data[i][j]) / dt;
        // Do interpolation
        interpolated_data[j] = a0 + a1 * (t - t_acc0);
       }
      // Add to the vector
      tmp_acc_data.push_back(interpolated_data);
     }
    else // No iterpolation, copy the nearest
     {
      if (fabs(t - t_acc0) < fabs(t - t_acc1))
       {
        // Change the time to reflect the same as the gyro's data
        Acceleration_data[i][0] = t;
        // Add to the vector
        tmp_acc_data.push_back(Acceleration_data[i]);
       }
      else
       {
        // Change the time to reflect the same as the gyro's data
        Acceleration_data[i+1][0] = t;
        // Add to the vector
        tmp_acc_data.push_back(Acceleration_data[i+1]);
       }
      
     } // if (t >= t_acc0 && t <= t_acc1)
    
   } // for (i < n_min_data-1)
  
  // Copy back the information to the "Acceleration_data" and
  // "Gyro_data" structure. Also perform the scaling of the
  // information
  Acceleration_data.clear();
  Gyro_data.clear();
  for (unsigned i = 0; i < n_min_data-1; i++)
   {
    tmp_acc_data[i][1] = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, tmp_acc_data[i][1]);
    tmp_acc_data[i][2] = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, tmp_acc_data[i][2]);
    tmp_acc_data[i][3] = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, tmp_acc_data[i][3]);    
    Acceleration_data.push_back(tmp_acc_data[i]);
    // And transform to radians since they are given in degrees
    tmp_gyro_data[i][1] = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, tmp_gyro_data[i][1]) * TO_RADIANS;
    tmp_gyro_data[i][2] = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, tmp_gyro_data[i][2]) * TO_RADIANS;
    tmp_gyro_data[i][3] = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, tmp_gyro_data[i][3]) * TO_RADIANS;
    Gyro_data.push_back(tmp_gyro_data[i]);
   }
  
 }
 
}
