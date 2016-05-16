/** \file This file implements the Newton form of the interpolation
    polynomial
 */
#include "cc_newton_interpolator.h"

// ===================================================================
// Empty constructor
// ===================================================================
CCNewtonInterpolator::CCNewtonInterpolator()
{ 

}

// ===================================================================
// Empty destructor
// ===================================================================
CCNewtonInterpolator::~CCNewtonInterpolator()
{ 

}

// ===================================================================
// Does 1D interpolation specifying the data points, the order of the
// interpolation and the desired "x" value to interpolate
// ===================================================================
double CCNewtonInterpolator::interpolate_1D(std::vector<double> &x_points,
					    std::vector<double> &fx_points,
					    const double x,
					    const unsigned order)
{
 // We assume the data points are in order, thus we do not check for
 // that
 
 // Get the size of the data set
 const unsigned n_x_points = n_points.size();
 const unsigned n_fx_points = fx_points.size();
 
 if (n_x_points != n_fx_points)
  {
   std::cout << "ERROR in CCNewtonInterpolator::interpolate_1D() - The number of data points do not match" << std::endl;
   std::cout << "Number of x-points: " << n_x_points << std::endl;
   std::cout << "Number of fx-points: " << n_fx_points << std::endl;
   throw(1);
   return 0;
  }
 
 // We check whether we have the corresponding number of data points
 // to perform the given order interpolation
 if (n_x_points - 1 != order)
  {
   std::cout << "ERROR in CCNewtonInterpolator::interpolate_1D() - The number of data points do not match with the requested interpolation order" << std::endl;
  }
 
 // The coefficients vector
 std::vector<double> a(n_order + 1);
 
 // What interpolation to perform
 if (order == 0) // zero interpolation or no interpolation
  {
   a[0] = fx_points[0];
   return a[0];
  }
 else if (order == 1) // linear interpolation
  {
   a[0] = fx_points[0];
   // Using divided differences notation
   const double f01 =
    (fx_points[1] - fx_points[0]) / (x_points[1] - x_points[0]);
   a[1] = f01;
   
   // Do interpolation
   const double interpolated_value = a[0] + a[1] * (x - x_points[0]);
   return interpolated_value;
  }
 else if (order == 2) // quadratic interpolation
  {
   a[0] = fx_points[0];
   // Using divided differences notation
   const double f01 =
    (fx_points[1] - fx_points[0]) / (x_points[1] - x_points[0]);
   const double f12 =
    (fx_points[2] - fx_points[1]) / (x_points[2] - x_points[1]);
   
   a[1] = (f12 - f01) / (x_points[2] - x_points[0]);
   
   // Do interpolation
   const double interpolated_value = a[0] + a[1] * (x - x_points[0]) +
    a[2] * (x - x_points[0]) * (x - x_points[1]);
   
   return interpolated_value;
  }
 else
  {
   std::cout << "ERROR in CCNewtonInterpolator::interpolate_1D() - The requested interpolation order is not implemented" << std::endl;
   std::cout << "We only implement zero, linear and quadratic interpolation" << std::endl;
   std::cout << "Requested interpolation order: " << order << std::endl;
  }
 
}
