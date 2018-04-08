// IN THIS FILE: The definition of the concrete class
// CCSolverArmadillo to solve systems of equations. This class calls
// the methods solve() or spsolve() from Armadillo to perform the
// solution of the system of equations.

// Check whether the class has been already defined
#ifndef CCSOLVERARMADILLO_TPL_H
#define CCSOLVERARMADILLO_TPL_H

// Include the header from inherited class
#include "ac_linear_solver.h"

// Support for Armadillo's wrappers
#include "../../src/matrices/cc_matrix_armadillo.h"

namespace chapchom
{

 // A concrete class for solving a linear system of equations. This
 // class uses the methods solve() or spsolve() from Armadillo to
 // perform the solution of the system of equations.
 template<class T>
  class CCSolverArmadillo : public virtual ACLinearSolver<CCMatrixArmadillo<T>, CCMatrixArmadillo<T> >
  {
 
  public:
   
   // Empty constructor
   CCSolverArmadillo();
   
   // Constructor where we specify the matrix A
   CCSolverArmadillo(const CCMatrixArmadillo<T> &A);
   
   // Empty destructor
   ~CCSolverArmadillo();
   
   // Solve a system of equations with input A. We specify the
   // right-hand side B and the X matrices where the results are
   // returned, B and X may be 1-column matrices (vectors). We assume
   // that the input/output matrices have the correct dimensions.
   void solve(const CCMatrixArmadillo<T> &A, const CCMatrixArmadillo<T> &B, CCMatrixArmadillo<T> &X);
      
   // Solve a system of equations with the already stored matrix A. We
   // specify the right-hand side B and the X matrices where the
   // results are returned, B and X may be 1-column matrices
   // (vectors). We assume that the input/output matrices have the
   // correct dimensions.
   void solve(const CCMatrixArmadillo<T> &B, CCMatrixArmadillo<T> &X);
   
  private:
 
   // Copy constructor (we do not want this class to be copiable because
   // it contains dynamically allocated variables, A in this
   // case). Check
   // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
   CCSolverArmadillo(const CCSolverArmadillo<T> &copy)
    {
     BrokenCopy::broken_copy("CCSolverArmadillo");
    }
 
   // Copy constructor (we do not want this class to be copiable because
   // it contains dynamically allocated variables, A in this
   // case). Check
   // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
   void operator=(const CCSolverArmadillo<T> &copy)
    {
     BrokenCopy::broken_assign("CCSolverArmadillo");
    }
   
  };
 
}

#endif // #ifndef CCSOLVERARMADILLO_TPL_H