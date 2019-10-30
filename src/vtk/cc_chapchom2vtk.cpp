#include "cc_chapchom2vtk.h"

namespace chapchom
{

 // ==================================================================
 // Functions for VTK output
 // ==================================================================

 // ==================================================================
 // Transfer the data from the input variable time, to the
 // data_set. Inserts the associated TIME to the set of
 // particles. Called from method output_partciles()
 // ==================================================================
 void CCChapchom2VTK::add_time_to_vtk_data_set_helper(Real time,
                                                      vtkSmartPointer<vtkUnstructuredGrid> &data_set)
 {
  // Add time stamp for the current file
  vtkSmartPointer<vtkDoubleArray> array = vtkSmartPointer<vtkDoubleArray>::New();
  array->SetName("TIME");
  array->SetNumberOfTuples(1);
  array->SetTuple1(0, time);
  data_set->GetFieldData()->AddArray(array);
 }

 // ==================================================================
 // Transfer the data from particles_data to data_points and
 // data_set. Called from method output_partciles()
 // ==================================================================
 void CCChapchom2VTK::add_particles_to_vtk_data_set_helper(CCData<Real> &particles_data,
                                                           vtkSmartPointer<vtkPoints> &data_points,
                                                           vtkSmartPointer<vtkUnstructuredGrid> &data_set,
                                                           const unsigned n_data_per_particle)
 {
  const int n_particles = particles_data.n_values()/n_data_per_particle;
 
  // An array to store the particles IDs
  vtkSmartPointer<vtkDoubleArray> ids = vtkSmartPointer<vtkDoubleArray>::New();
  ids->SetNumberOfComponents(1);
  ids->SetNumberOfTuples(n_particles);
  ids->SetName("ID");
#if 0
  // An array to store the particles radius
  vtkSmartPointer<vtkDoubleArray> radius = vtkSmartPointer<vtkDoubleArray>::New();
  radius->SetNumberOfComponents(3);
  radius->SetNumberOfTuples(n_particles);
  radius->SetName("Radius");
#endif // #if 0
#if 0
  // An array to store the particles positions
  vtkSmartPointer<vtkDoubleArray> position = vtkSmartPointer<vtkDoubleArray>::New();
  position->SetNumberOfComponents(3);
  position->SetNumberOfTuples(n_particles);
  position->SetName("Position");
#endif // #if 0
  // An array to store the particles velocity
  vtkSmartPointer<vtkDoubleArray> velocity = vtkSmartPointer<vtkDoubleArray>::New();
  velocity->SetNumberOfComponents(3);
  velocity->SetNumberOfTuples(n_particles);
  velocity->SetName("Velocity");
  // An array to store the particles masses
  vtkSmartPointer<vtkDoubleArray> masses = vtkSmartPointer<vtkDoubleArray>::New();
  masses->SetNumberOfComponents(1);
  masses->SetNumberOfTuples(n_particles);
  masses->SetName("Masses");

  // Temporal vector to extract data
  Real pos[3];
  Real vel[3];
  Real mass[1];
  int global_id = 0;
  // Loop through particles data
  for (unsigned long i = 0; i < n_particles*n_data_per_particle; i+=n_data_per_particle)
   {
    // Position
    pos[0] = particles_data(i,0);
    pos[1] = particles_data(i+2,0);
    pos[2] = particles_data(i+4,0);
    data_points->SetPoint(global_id, pos);
   
    // IDs
    ids->InsertValue(global_id, global_id);
   
    // Velocity
    vel[0] = particles_data(i+1,0);
    vel[1] = particles_data(i+3,0);
    vel[2] = particles_data(i+5,0);
    velocity->InsertTuple(global_id, vel);

    // Mass
    mass[0] = global_id;
    masses->InsertTuple(global_id, mass);
   
    global_id++;
   }
 
  // Add particles data to data set
  data_set->GetPointData()->AddArray(ids);
  //data_set->GetPointData()->AddArray(radius);
  //data_set->GetPointData()->AddArray(position);
  data_set->GetPointData()->AddArray(velocity);
  data_set->GetPointData()->AddArray(masses);
 }

 //========================================================================
 // In charge of output a set of particles at current time into a
 // file. The particles are stored at particles_data parameter.
 // 
 // If having three dimensions data then it should be stored as
 // follows and set n_data_per_particle = 6
 //
 // For particle i
 // x-position of particle i, particles_data(i*6+0, 0)
 // x-velocity of particle i, particles_data(i*6+1, 0)
 // y-position of particle i, particles_data(i*6+2, 0)
 // y-velocity of particle i, particles_data(i*6+3, 0)
 // z-position of particle i, particles_data(i*6+4, 0)
 // z-velocity of particle i, particles_data(i*6+5, 0)
 //
 //
 // If having two dimensions data then it should be stored as
 // follows and set n_data_per_particle = 4
 //
 // For particle i
 // x-position of particle i, particles_data(i*4+0, 0)
 // x-velocity of particle i, particles_data(i*4+1, 0)
 // y-position of particle i, particles_data(i*4+2, 0)
 // y-velocity of particle i, particles_data(i*4+3, 0)
 // 
 // where k is the number of data per particle
 //========================================================================
 void CCChapchom2VTK::output_particles(Real time,
                                       CCData<Real> &particles_data,
                                       std::ostringstream &file_name,
                                       const unsigned n_data_per_particle)
 {
  // Create a VTK writer
  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
   vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
 
  // Generate the filename with the proper extension
  file_name << "." << writer->GetDefaultFileExtension();
  writer->SetFileName((file_name.str()).c_str());
 
  // Create a pointer to a VTK Unstructured Grid data set
  vtkSmartPointer<vtkUnstructuredGrid> data_set =
   vtkSmartPointer<vtkUnstructuredGrid>::New();
 
  // Set up pointer to data point
  vtkSmartPointer<vtkPoints> data_points =
   vtkSmartPointer<vtkPoints>::New();
 
  // Get the total number of particles, divide data by n_data_per_particle
  const int n_points = particles_data.n_values()/n_data_per_particle;
  data_points->SetNumberOfPoints(n_points);
 
  // Add time
  add_time_to_vtk_data_set_helper(time, data_set);
 
  // Add the particle data to the unstructured grid
  add_particles_to_vtk_data_set_helper(particles_data, data_points, data_set, n_data_per_particle);
 
  // Set the points
  data_set->SetPoints(data_points);
  // Remove unused memory
  data_set->Squeeze();
 
  // Set the writer's input data set
  writer->SetInputData(data_set);
  //writer->SetDataModelToBinary();
  writer->SetDataModeToAscii();
  writer->Write();
 
 }

}