// This demo driver is based on the Program 1.2 DistanceMatrixFit.m
// from the book "Meshfree Approximation Methods with MATLAB, Gregory
// E. Fasshauer", World Scientific Publishing, 2007

// Include general/common includes, utilities and initialisation
#include "../../../src/general/common_includes.h"
#include "../../../src/general/utilities.h"
#include "../../../src/general/initialise.h"

// Matrices
#include "../../../src/matrices/cc_matrix.h"
#include "../../../src/data_structures/cc_node.h"

// The class to solve linear systems using numerical recipes
#include "../../../src/linear_solvers/cc_lu_solver_numerical_recipes.h"

#ifdef CHAPCHOM_USES_ARMADILLO
// Include Armadillo type matrices since the templates may include
// Armadillo type matrices
#include "../../../src/matrices/cc_matrix_armadillo.h"

// The class to solve linear systems using Armadillo's type matrices
#include "../../../src/linear_solvers/cc_solver_armadillo.h"
#endif // #ifdef CHAPCHOM_USES_ARMADILLO

using namespace chapchom;

const Real test_function(CCVector<Real> &x, const unsigned s)
{
 Real prod=1.0;
 for (unsigned i = 0; i < s; i++)
  {
   prod*=x(i)*(1.0-x(i));
  }
 return pow(4, s)*prod;
}

template<class T>
void compute_distance_matrix(CCMatrix<T> &data_sites, CCMatrix<T> &centers,
                             CCMatrix<T> &distance_matrix)
{
 // Get the number of "vector points" on "data_sites"
 // Get the number of "vector points" on "centers"
 const unsigned n_vector_points_data_sites = data_sites.n_columns();
 const unsigned n_vector_points_centers = centers.n_columns();

 // The dimension of input vector points must be the same, otherwise
 // there is an error
 const unsigned dimension = data_sites.n_rows();
 const unsigned tmp_dimension = centers.n_rows();

 if (dimension != tmp_dimension)
  {
   // Error message
   std::ostringstream error_message;
   error_message << "The dimensions of the data sites vector and the\n"
                 << "centers vector are different\n"
                 << "dim(data_site):" << dimension
                 << "\ndim(centers):" << tmp_dimension
                 << std::endl;
   throw ChapchomLibError(error_message.str(),
                          CHAPCHOM_CURRENT_FUNCTION,
                          CHAPCHOM_EXCEPTION_LOCATION);
  }
 
 // Loop over all the data points in the first matrix
 for (unsigned m = 0; m < n_vector_points_data_sites; m++)
  {
   // Loop over all the data points in the second matrix
   for (unsigned n = 0; n < n_vector_points_centers; n++)
    {
     CCVector<T> distance(dimension);
     distance.allocate_memory();
     // Loop over the elements of both vectors
     for (unsigned k = 0; k < dimension; k++)
      {
       distance(k) = data_sites(k, m) - centers(k, n);
      }
     distance_matrix(m,n)=distance.norm_2();
    }
  }
 
}

// ==================================================================
// ==================================================================
// ==================================================================
// Main function
// ==================================================================
// ==================================================================
// ==================================================================
int main(int argc, char *argv[])
{
 // Initialise chapchom
 initialise_chapchom();
 
 // --------------------------------------------------------------
 // Domain specification
 // --------------------------------------------------------------
 // TODO: Create a DOMAIN (mesh?) type class
 
 // Dimension of the problem
 const unsigned dim = 1;
 // Interpolant degree
 const unsigned degree = 3;
 
 // Specify the one-dimensional lenght of the domain
 const unsigned L = 1.0;
 
 // --------------------------------------------------------------
 // Create and give position to nodes
 // --------------------------------------------------------------
 // Nodes per dimension
 const unsigned n_nodes_per_dim = pow(2, degree+1);
 // The number of nodes
 const unsigned n_nodes = pow(n_nodes_per_dim, dim);
 // A vector of nodes
 std::vector<CCNode<Real> *> nodes_pt(n_nodes);
 
 // Number of variables stored in the node
 const unsigned n_variables = 1;
 // Number of history values
 const unsigned n_history_values = 1;
 
 // Distance between a pair of nodes
 const Real h = L / (Real)(n_nodes_per_dim - 1);
 std::vector<Real> x(dim, 0.0);
 // Create the nodes
 for (unsigned i = 0; i < n_nodes; i++)
  {
   nodes_pt[i] = new CCNode<Real>(dim, n_variables, n_history_values);
  }
 
 // Assign positions
 for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     const Real r = rand();
     const Real position = static_cast<Real>(r / RAND_MAX) * L;
     // Generate position and assign it
     //const Real position = x[k];
     nodes_pt[i]->set_position(position, k); 
     //x[k]+=h;
    }
  }
 
 // --------------------------------------------------------------
 // Set initial conditions
 // --------------------------------------------------------------
 for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned j = 0; j < n_variables; j++)
    {
     const Real u = 0.0;
     nodes_pt[i]->set_variable(u, j);
    }
  }
 
 // --------------------------------------------------------------
 // Set boundary conditions
 // --------------------------------------------------------------
 
 // Move the first and the last node to the boundary of the domain
 //nodes_pt[0]->set_position(0.0, 0);
 //nodes_pt[0]->set_variable(0.0, 0);
 //nodes_pt[n_nodes-1]->set_position(1.0, 0);
 //nodes_pt[n_nodes-1]->set_variable(1.0, 0);
 
 // --------------------------------------------------------------
 // Set the problem and solve it
 // --------------------------------------------------------------
 
 // TODO: The distance matrix may be formed while we loop over the
 // nodes to extract their position
 
 // --------------------------------------------------------------
 // Loop over the nodes and extract their position and store them in a
 // matrix
 // --------------------------------------------------------------
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloMatrix<Real> nodes_position(dim, n_nodes);
#else
 CCMatrix<Real> nodes_position(dim, n_nodes);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 // Each column stores the vector position of a node
 nodes_position.allocate_memory();
  for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned j = 0; j < dim; j++)
    {
     nodes_position(j, i) = nodes_pt[i]->get_position(j);
    }
  }
 
 // -------------------------------------------------------------- 
 // Create the distance matrix
 // --------------------------------------------------------------
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloMatrix<Real> distance_matrix(n_nodes, n_nodes);
#else
 CCMatrix<Real> distance_matrix(n_nodes, n_nodes);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 // --------------------------------------------------------------
 // Generate the distance matrix using the nodes position centers
 // shifted by the same nodes position
 // --------------------------------------------------------------
 distance_matrix.allocate_memory();
 compute_distance_matrix(nodes_position, nodes_position, distance_matrix);
 
 // --------------------------------------------------------------
 // Set right-hand side
 // --------------------------------------------------------------
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloVector<Real> rhs(n_nodes);
#else 
 CCVector<Real> rhs(n_nodes);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO 
 rhs.allocate_memory();
 for (unsigned i = 0; i < n_nodes; i++)
  {
#ifdef CHAPCHOM_USES_ARMADILLO
   CCArmadilloVector<Real> tmp_v(dim);
#else
   CCVector<Real> tmp_v(dim);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
   tmp_v.allocate_memory();
   for (unsigned j = 0; j < dim; j++)
    {
     tmp_v(j) = nodes_pt[i]->get_position(j);
    }
   // --------------------------------------------------------------
   // Evaluate the KNOWN function at the centers positions
   // --------------------------------------------------------------
   rhs(i) = test_function(tmp_v, dim);
  }
 
 // The solution vector (with the respective number of rows) stores
 // the coefficients for the interpolant polynomials
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloVector<Real> sol(n_nodes);
#else
 CCVector<Real> sol(n_nodes);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 
 // --------------------------------------------------------------
 // Solve
 // --------------------------------------------------------------
#ifdef CHAPCHOM_USES_ARMADILLO
  // Create an Armadillo linear solver
  CCSolverArmadillo<Real> linear_solver;
#else
 // Create a linear solver
 CCLUSolverNumericalRecipes<Real> linear_solver;
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 
 std::cerr << "Distance matrix" << std::endl;
 //distance_matrix.print();
 
 // --------------------------------------------------------------
 // Solve the system of equations
 // --------------------------------------------------------------
 linear_solver.solve(distance_matrix, rhs, sol);
 std::cerr << "Solution vector" << std::endl;
 //sol.print();

 std::cerr << "Nodes positions and values" << std::endl;
 // Show results
 for (unsigned i = 0; i < n_nodes; i++)
  {
   //nodes_pt[i]->print(true);
  }

 // --------------------------------------------------------------
 // --------------------------------------------------------------
 // EVALUATION STAGE
 // --------------------------------------------------------------
 // --------------------------------------------------------------
 std::cerr << "\n\nEVALUATION\n" << std::endl;
 
 // --------------------------------------------------------------
 // Evaluate (compute error RMSE)
 // --------------------------------------------------------------
 const unsigned n_evaluation_points = 10;
 const unsigned n_data_in_evaluation_points = pow(n_evaluation_points, dim);
 // Distance between a pair of nodes
 const Real h_test = L / (Real)(n_evaluation_points - 1);

 // Compute approximated solution at new positions
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloMatrix<Real> approx_solution_position(dim, n_evaluation_points);
#else
 CCMatrix<Real> approx_solution_position(dim, n_evaluation_points);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 approx_solution_position.allocate_memory();
 // --------------------------------------------------------------
 // Assign positions
 // --------------------------------------------------------------
 std::vector<Real> x_eval(dim, 0.0);
 for (unsigned i = 0; i < n_evaluation_points; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     const Real r = rand();
     const Real position = static_cast<Real>(r / RAND_MAX) * L;
     // Generate position and assign it
     //const Real position = x_eval[k];
     approx_solution_position(k, i) = position;
     //x_eval[k]+=h_test;
    }
  }
 
 // Compute distance matrix with new positions
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloMatrix<Real> approx_distance_matrix(n_evaluation_points, n_nodes);
#else
 CCMatrix<Real> approx_distance_matrix(n_evaluation_points, n_nodes);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 // --------------------------------------------------------------
 // Generate the distance matrix using the nodes position centers
 // shifted by the new positions
 // --------------------------------------------------------------
 approx_distance_matrix.allocate_memory();
 compute_distance_matrix(approx_solution_position, nodes_position, approx_distance_matrix);
 //approx_distance_matrix.print();
 
 // Approximated solution
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloVector<Real> approx_sol(n_evaluation_points);
#else
 CCVector<Real> approx_sol(n_evaluation_points);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 // Approximate solutin at given points
 multiply_matrix_times_vector(approx_distance_matrix, sol, approx_sol);

 // --------------------------------------------------------------
 // Output data for plotting
 // --------------------------------------------------------------
 std::ofstream output_file("output.dat");
 for (unsigned i = 0; i < n_evaluation_points; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     output_file << approx_solution_position(k, i) << " ";
    }
   output_file << approx_sol(i) << std::endl;
  }
 
 // Close output file
 output_file.close();
 
 // --------------------------------------------------------------
 // Get real solution at given points and get the error 
 // --------------------------------------------------------------
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloVector<Real> real_sol(n_evaluation_points);
#else 
 CCVector<Real> real_sol(n_evaluation_points);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 real_sol.allocate_memory();
 for (unsigned i = 0; i < n_evaluation_points; i++)
  {
#ifdef CHAPCHOM_USES_ARMADILLO
   CCArmadilloVector<Real> tmp_v(dim);
#else
   CCVector<Real> tmp_v(dim);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
   tmp_v.allocate_memory();
   for (unsigned j = 0; j < dim; j++)
    {
     tmp_v(j) = approx_solution_position(j, i);
    }
   // ------------------------
   // Evaluation at approx_solution_position
   real_sol(i) = test_function(tmp_v, dim);
  }
 
 // --------------------------------------------------------------
 // Compute error
 // --------------------------------------------------------------
#ifdef CHAPCHOM_USES_ARMADILLO
 CCArmadilloVector<Real> error(n_evaluation_points);
#else
 CCVector<Real> error(n_evaluation_points);
#endif // #ifdef CHAPCHOM_USES_ARMADILLO
 error.allocate_memory();
 std::cerr << "ERRORS" << std::endl;
 for (unsigned i = 0; i < n_evaluation_points; i++)
  {
   error(i) = real_sol(i) - approx_sol(i);
   //std::cerr << i << ": " << std::fabs(error(i)) << std::endl;
   //std::cerr << i << ": " << real_sol(i) << ":" << approx_sol(i) << std::endl;
  }
 
 const Real rms_error = error.norm_2() / sqrt(n_data_in_evaluation_points);
 
 // --------------------------------------------------------------
 // Output error
 // --------------------------------------------------------------
 std::ofstream error_file("error.dat");
 for (unsigned i = 0; i < n_evaluation_points; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     error_file << approx_solution_position(k, i) << " ";
    }
   error_file << error(i) << std::endl;
  }
 
 // Close error file
 error_file.close();
 
 // --------------------------------------------------------------
 // Summary
 // --------------------------------------------------------------
 std::cerr << std::endl;
 std::cerr << "Polynomial degree: " << degree << std::endl;
 std::cerr << "N. nodes per dimension: " << n_nodes_per_dim << std::endl;
 std::cerr << "N. total nodes: " << n_nodes << std::endl; 
 std::cerr << "RMS-error: " << rms_error << std::endl;
 
 // ==============================================================
 // ==============================================================
 
 // --------------------------------------------------------------
 // Delete nodes storage
 // --------------------------------------------------------------
 for (unsigned i = 0; i < n_nodes; i++)
  {
   delete nodes_pt[i];
  }
 
 // Finalise chapcom
 finalise_chapchom();
 
 return 0;
 
}

