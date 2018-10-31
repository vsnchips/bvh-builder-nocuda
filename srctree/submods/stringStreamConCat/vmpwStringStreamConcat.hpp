#pragma once

#include "includes-l1.hpp"

class vmpwStringStreamConcat{

  public:
    //inputs
    //
  vmpwStringStreamConcat(std::vector<std::string> * f) : filenames(f){}

  std::vector<std::string> * filenames;
  //  std::vector<time_t> intimes; //last time input was checked for changes. TODO: Whose responsibility is it to check date of data? Timestamping neccessitates struct composition

  //outputs
  std::string val;

  //fields - read only from outside
  std::vector<FILE *> filestreams;
  std::vector<int> fds; //file descriptors
  std::vector<clock_t> file_dates;

  //internal variables only visible for debugging

  //Settings -> keep these accessible and generalised !
  clock_t update_interval = 10;//0.05*CLOCKS_PER_SEC;

  //Side window
  std::string status = "ok";
  clock_t lt = 0; //how fresh is it
  clock_t et = 0; //exception time
  
    //internal: generally to be called by the update function.
  vmpwStringStreamConcat(); // Module constructors will probably take no arguments.

  std::vector<time_t> modTimes; // Files update times
  
  int checkDates();
  int makeString();
  int update( clock_t );
};
