// STL
#include <iostream>
#include <fstream>
// GNU Plot
#define INCLUDE_PNG_SUPPORT
#include <plotter.h>

// /////////////////////////////////////////////////////////////////
void draw_c_curve_rec (Plotter& plotter, double dx, double dy, int order) {
  if (order >= 12) {
    plotter.fcontrel (dx, dy);	// continue path along (dx, dy)

  } else {
    draw_c_curve_rec (plotter, 0.5 * (dx - dy), 0.5 * (dx + dy), order + 1);
    draw_c_curve_rec (plotter, 0.5 * (dx + dy), 0.5 * (dy - dx), order + 1);
  }
}

// /////////////////////////////////////////////////////////////////
void draw_c_curve_rec (Plotter& plotter) {
  // Specify user coor system
  plotter.fspace (0.0, 0.0, 1000.0, 1000.0);

  // Line thickness in user coordinates
  plotter.flinewidth (0.25);

  // Path will be drawn in red
  plotter.pencolorname ("red");

  // Erase Plotter's graphics display
  plotter.erase();

  // Position the graphics cursor
  plotter.fmove (600.0, 300.0);

  //
  draw_c_curve_rec (plotter, 0.0, 400.0, 0);
}

// ///////////////// M A I N ////////////////
int main (int argc, char* argv[]) {

  // Image file
  const std::string lImageFilename ("plotutils.png");

  // Output log file
  const std::string lLogFilename ("plotutils.log");
    
  // Set the log parameters
  std::ofstream logOutputFile;
    
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Set the Plotter image parameters
  std::ofstream plotterOutputFile;
  plotterOutputFile.open (lImageFilename.c_str());
  
  const std::string kParamType ("PAGESIZE");
  char kParamValueStr[3] = "a4";
  void* kParamValue = static_cast<void*> (kParamValueStr);
  PlotterParams lPlotterParams;
  lPlotterParams.setplparam (kParamType.c_str(), kParamValue);
  PNGPlotter plotter (plotterOutputFile, lPlotterParams);

  // Open Plotter
  if (plotter.openpl () < 0) {
    std::cerr << "Couldn't open Plotter" << std::endl;
    return -1;
  }

  //
  draw_c_curve_rec (plotter);
  
  // Close Plotter
  if (plotter.closepl () < 0) {
    std::cerr << "Couldn't close Plotter" << std::endl;
    return -1;
  }

  //
  std::cout << std::endl << "A PNG image has been generated: open the '"
            << lImageFilename << "' file to see it (e.g., `eog "
            << lImageFilename << "`)." << std::endl << std::endl;

  // Close the files
  logOutputFile.close();
  plotterOutputFile.close();
}
