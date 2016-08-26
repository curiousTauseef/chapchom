#include <iostream>
#include <cmath>

// The class to create a matrix
#include "../../../src/matrices/cc_matrix.h"

int main(int argc, char *argv[])
{
 // Show matrix manipulation for addition, multiplication and
 // transpose
 
 // Size of the matrix (number of rows and columns)
 const unsigned long n_rows = 10;
 const unsigned long n_columns = 10;

 // ----------------------------------------------------------
 // Identity matrix
 // ----------------------------------------------------------
 CCMatrix I(n_rows, n_columns);
 
 // Feed some data to the matrix (the identity)
 for (unsigned i = 0; i < n_rows; i++)
  {
   I(i,i) = 1;
  }
 
 std::cout << std::endl << "The identity matrix (I)" << std::endl << std::endl;
 I.output();
 
 // ----------------------------------------------------------
 // Matrix from vector
 // ----------------------------------------------------------
 // Create an array to initialise another matrix with it
 double *matrix_pt = new double[n_rows*n_columns];
 
 // Add some data to the array
 for (unsigned i = 0; i < n_rows; i++)
  {
   for (unsigned j = 0; j < n_columns; j++)
    {
     if (i==j)
      {
       matrix_pt[i*n_columns+j] = i;
      }
     else 
      {
       // Get the distance to the diagonal
       matrix_pt[i*n_columns+j] = i;
      }
    }
  }

 // ---------------------------------------
 // Create the matrix from the vector data
 // ---------------------------------------
 CCMatrix B(matrix_pt, n_rows, n_columns);
 
 std::cout << std::endl << "Matrix created from vector" << std::endl << std::endl;
 B.output();

 // ----------------------------------------------------------
 // Create a matrix where to store results
 // ----------------------------------------------------------
 // Solution matrix
 CCMatrix C(n_rows, n_columns);
 
 std::cout << std::endl
           << "------------------------------------------------------------------------------"
           << std::endl << "Matrix operations\n"
           << "------------------------------------------------------------------------------"
           << std::endl;
 
 // --------------------------------------
 // Sum of matrices C = I + B
 // --------------------------------------
 for (unsigned i = 0; i < n_rows; i++)
  {
   for (unsigned j = 0; j < n_columns; j++)
    {
     C(i,j) = I(i,j) + B(i,j);
    }
  }
 
 std::cout << std::endl << "The sum of the matrices is:"
           << std::endl << std::endl;
 C.output();

 // --------------------------------------
 // Matrix multiplication C = I * B
 // -------------------------------------- 
 // Zeroe the result matrix
 for (unsigned i = 0; i < n_rows; i++)
  {
   for (unsigned j = 0; j < n_columns; j++)
    {
     C(i,j) = 0;
    }
  }
 
 for (unsigned i = 0; i < n_rows; i++)
  {
   for (unsigned j = 0; j < n_columns; j++)
    {
     for (unsigned k = 0; k < n_columns; k++)
      {
       C(i,j)+= I(i,k) * B(k,j);
      }
    }
  }
 
 std::cout << std::endl << "The multiplication of the matrices is:"
           << std::endl << std::endl;
 C.output();
 
 // ##############################################################################
 // Now do the same operations but using operator overloading
 // ##############################################################################
 std::cout << std::endl << ""
           << "##############################################################################\n"
           << "Now do the same operations but using operator overloading\n"
           << "##############################################################################"
           << std::endl;
 // --------------------------------------
 // Sum of matrices C = I + B
 // --------------------------------------
 C = I + B;
 
 std::cout << std::endl << "The sum of the matrices is:"
           << std::endl << std::endl;
 C.output();

 // --------------------------------------
 // Matrix multiplication C = I * B
 // --------------------------------------
 C = I * B;
 
 std::cout << std::endl << "The multiplication of the matrices is:"
           << std::endl << std::endl;
 C.output();
 
 // -----------------------------------------------------------------------------
 // Create a non square matrix
 // -----------------------------------------------------------------------------
 const unsigned long n_rows_A = 5;
 const unsigned long n_columns_A = 10;
 double *matrix_A_pt = new double[n_rows_A*n_columns_A];
 
 // Add some data to the array
 for (unsigned i = 0; i < n_rows_A; i++)
  {
   for (unsigned j = 0; j < n_columns_A; j++)
    {
     if (i==j)
      {
       matrix_A_pt[i*n_columns_A+j] = i;
      }
     else 
      {
       // Get the distance to the diagonal
       matrix_A_pt[i*n_columns_A+j] = i;
      }
    }
  }
 // Create the non square matrix
 CCMatrix A(matrix_A_pt, n_rows_A, n_columns_A);
 std::cout << std::endl << "Non square matrix"
           << std::endl << std::endl;
 A.output();
 
 // -----------------------------------------------------------------------------
 // Create a vector to multiply with the non square matrix
 // -----------------------------------------------------------------------------
 const unsigned long n_rows_x = 10;
 const unsigned long n_columns_x = 1;
 double *matrix_x_pt = new double[n_rows_x*n_columns_x];
 
 // Add some data to the array
 for (unsigned i = 0; i < n_rows_x; i++)
  {
   for (unsigned j = 0; j < n_columns_x; j++)
    {
     if (i==j)
      {
       matrix_x_pt[i*n_columns_x+j] = i;
      }
     else 
      {
       // Get the distance to the diagonal
       matrix_x_pt[i*n_columns_x+j] = i;
      }
    }
  }
 
 // Create the vector (matrix)
 CCMatrix x(matrix_x_pt, n_rows_x, n_columns_x);
 std::cout << std::endl << "Vector"
           << std::endl << std::endl;
 x.output();
 
 // --------------------------------------
 // Matrix multiplication A * x = b
 // --------------------------------------
 CCMatrix b(n_rows_A, n_columns_x);
 b = A * x;
 
 std::cout << std::endl << "The multiplication of the matrices is:"
           << std::endl << std::endl;
 b.output();

 // --------------------------------------------------
 // Apply transpose
 // --------------------------------------------------
 CCMatrix b_t(b.ncolumns(), b.nrows());
 b.transpose(b_t);
 std::cout << std::endl << "The transposed matrix:"
           << std::endl << std::endl;
 b_t.output();
 
 // Free memory 
 delete [] matrix_pt;
 matrix_pt = 0;
 delete [] matrix_A_pt;
 matrix_A_pt = 0;
 delete [] matrix_x_pt;
 matrix_x_pt = 0;
 
 return 0;
 
}