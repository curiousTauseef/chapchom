// IN THIS FILE: The definition of a class to represent nodes

// Check whether the class has been already defined
#ifndef CCNODE_TPL_H
#define CCNODE_TPL_H

#include "../general/common_includes.h"
#include "../general/utilities.h"
#include "./cc_data.h"

namespace chapchom
{
 
 /// @class CCNode cc_node.h
 
 // Abstract class to represent nodes
 template<class T>
  class CCNode
 {
  
 public:
  
  // Empty constructor
  CCNode(const unsigned dimension, const unsigned n_variables, const unsigned n_history_values); 
  
  // Empty destructor
  virtual ~CCNode();
  
  // Pin al positions
  inline void pin_all_positions()
  {X.pin_all();}
  
  // Pin all variables
  inline void pin_all_variables()
  {U.pin_all();}
  
  // Pin i-th position
  inline void pin_position(const unsigned &i)
  {X.pin(i);}
  
  // Unpin i-th position
  inline void unpin_position(const unsigned &i)
  {X.unpin(i);}
  
  // Pin i-th variable
  inline void pin_u(const unsigned &i)
  {U.pin(i);}
  
  // Unpin i-th variable
  inline void unpin_u(const unsigned &i)
  {U.unpin(i);}
  
  // Get access to the spatial position of the node
  inline CCData<T> &x() {return X;}
  
  // Get the i-th spatial position of the node at time t
  inline T get_position(const unsigned &i, const unsigned t=0)
  {return X.value(i,t);}
  
  // Set the i-th spatial position of the node at time t
  inline void set_position(const T i_position,
                           const unsigned &i, const unsigned t=0)
  {X.value(i,t)=i_position;}
  
  // Get access to variables stored in the node
  inline CCData<T> &u() {return U;}
  
  // Get the i-th variable value at time t
  inline T get_variable(const unsigned &i, const unsigned t=0)
  {return U.value(i,t);}
  
  // Set the i-th variable value at time t
  inline void set_variable(const T i_variable,
                           const unsigned &i, const unsigned t=0)
  {U.value(i,t)=i_variable;}
  
  inline unsigned dimension() const {return Dimension;}
  
  inline unsigned n_variables() const {return N_variables;}
  
  inline unsigned n_history_values() const {return N_history_values;}
  
  // Output the data stored at the node (output horizontally without
  // position by default, otherwise output vertically with position)
  virtual void output(bool output_position = false,
                      const unsigned t = 0) const;
  // Output the data stored at the node to a file (output horizontally
  // without position by default, otherwise output vertically with
  // position)
  virtual void output(std::ofstream &outfile,
                      bool output_position = false,
                      const unsigned t = 0) const;
  
  // Output the node
  inline void print(bool output_position = false,
                    const unsigned t = 0) const
  {output(output_position, t);}
  
  // Output to file
  inline void print(std::ofstream &outfile, 
                    bool output_position = false,
                    const unsigned t = 0) const
  {output(outfile, output_position, t);}
  
 protected:
  
  // The spatial dimension of the node
  const unsigned Dimension;
  
  // The number of variables stored in the node
  const unsigned N_variables;
  
  // The number of history values of the variable stored in the node
  const unsigned N_history_values;
  
  // Store the spatial position of the node
  CCData<T> X;
  
  // Store the values of the variables stored in the node
  CCData<T> U;
  
 };
 
}

#endif // #ifndef CCNODE_TPL_H
