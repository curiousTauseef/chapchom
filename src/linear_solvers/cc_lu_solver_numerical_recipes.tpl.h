// IN THIS FILE: The definition of the concrete class
// CCLUSolverNumericalRecipes to solve systems of equations by LU
// decomposition. This class calls the methods ludcmp() and lubksb()
// from numerical recipes to perform the LU decomposition and
// back-substitution, respectively.

// Check whether the class has been already defined
#ifndef CCLUSOLVERNUMERICALRECIPES_TPL_H
#define CCLUSOLVERNUMERICALRECIPES_TPL_H

// Include the header from inherited class
#include "ac_linear_solver.h"

// The LU factorisation and back-substitution from Numerical Recipes
#include "../../external_src/numerical_recipes/nr.h"

namespace chapchom
{

 // A concrete class for solving a linear system of equations. This
 // class uses the methods ludcmp() and lubksb() from numerical recipes
 // to perform the LU decomposition and back-substitution,
 // respectively.
 template<class T>
  class CCLUSolverNumericalRecipes : public virtual ACLinearSolver<CCMatrix<T>, CCVector<T> >
  {
 
 public:
 
  // Empty constructor
  CCLUSolverNumericalRecipes();
 
  // Constructor where we specify the matrix A
  CCLUSolverNumericalRecipes(const CCMatrix<T> &A);
 
  // Empty destructor
  ~CCLUSolverNumericalRecipes();
  
  // Solves a system of equations with input A_mat. We specify the
  // right-hand side B and the X matrices where the results are
  // returned. We assume that the input/output matrices have the
  // correct dimensions: A_mat.ncolumns() x A_mat.nrows() for B, and
  // A_mat.nrows() x A_mat.ncolumns() for X.
  void solve(const CCMatrix<T> &A_mat, const CCMatrix<T> &B, CCMatrix<T> &X);

  // Solves a system of equations with input A_mat. We specify the
  // right-hand side b and the x vector where the result is
  // returned. We assume that the input/output vectors have the
  // correct dimensions: A_mat.ncolumns() for b, and A_mat.nrows() for
  // x.
  void solve(const CCMatrix<T> &A_mat, const CCVector<T> &b, CCVector<T> &x);
  
  // Solve a system of equations with the already stored matrix A. We
  // specify the right-hand side B and the X matrices where the
  // results are returned. We assume that the input/output matrices
  // have the correct dimensions: A.ncolumns() x A.nrows() for B, and
  // A.nrows() x A.ncolumns() for X.
  void solve(const CCMatrix<T> &B, CCMatrix<T> &X);
  
  // Solve a system of equations with the already stored matrix A. We
  // specify the right-hand side b and the x vectors where the result
  // is returned. We assume that the input/output vectors have the
  // correct dimensions: A.ncolumns() for b, and A.nrows() for x.
  void solve(const CCVector<T> &b, CCVector<T> &x);
  
  // Re-solve a system of equations with the already stored matrix
  // A. Reusing the LU decomposition. We specify the right-hand
  // side B and the X matrices where the results are returned. We
  // assume that the input/output vectors have the correct dimensions:
  // A.ncolumns() x A.nrows() for B, and A.nrows() x A.ncolumns() for
  // X.
  void resolve(const CCMatrix<T> &B, CCMatrix<T> &X);
  
  // Re-solve a system of equations with the already stored matrix A
  // (re-use of the LU decomposition or call the solve method for an
  // iterative solver). BROKEN beacuse iterative solvers may not
  // implement it. We specify the right-hand side b and the x vector
  // where the result is returned. We assume that the input/output
  // vectors have the correct dimensions: A.ncolumns() for b, and
  // A.nrows() for x.
  void resolve(const CCVector<T> &b, CCVector<T> &x);
  
  // Performs LU factorisation of the input matrix, the factorisation
  // is internally stored such that it can be re-used when calling
  // resolve
  void factorise(const CCMatrix<T> &A_mat);
 
  // Performs LU factorisation of already stored matrix A, the
  // factorisation is internally stored such that it can be re-used
  // when calling resolve
  void factorise();
  
  // Performs the back substitution with the LU decomposed matrix
  void back_substitution(const CCMatrix<T> &B, CCMatrix<T> &X);
  
  // Performs the back substitution with the LU decomposed matrix
  void back_substitution(const CCVector<T> &b, CCVector<T> &x);
  
  protected:
 
  // Flag to indicate whether resolve is enabled (only after calling
  // factorise)
  bool Resolve_enabled;
 
  private:
 
  // Copy constructor (we do not want this class to be copiable because
  // it contains dynamically allocated variables, A in this
  // case). Check
  // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  CCLUSolverNumericalRecipes(const CCLUSolverNumericalRecipes<T> &copy)
   : ACLinearSolver<CCMatrix<T>, CCVector<T> >(),
   Resolve_enabled(false)
    {
     BrokenCopy::broken_copy("CCLUSolverNumericalRecipes");
    }
 
  // Copy constructor (we do not want this class to be copiable because
  // it contains dynamically allocated variables, A in this
  // case). Check
  // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCLUSolverNumericalRecipes<T> &copy)
   {
    BrokenCopy::broken_assign("CCLUSolverNumericalRecipes");
   }
  
  // Data used for ludcmp() and lubksb()
  Mat_DP *lu_a;
  Vec_INT *lu_indx;
  
 };
 
}

#endif // #ifndef CCLUSOLVERNUMERICALRECIPES_TPL_H
