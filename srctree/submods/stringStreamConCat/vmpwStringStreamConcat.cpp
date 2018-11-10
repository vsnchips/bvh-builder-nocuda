#include "vmpwStringStreamConcat.hpp"

//This thing can be the prototypical vmpw type


using namespace std;


int vmpwStringStreamConcat::checkDates(){

  static struct stat ts;
#include <ctime>

  if (filenames->size()!=file_dates.size()) return 1;
  for (int i = 0 ; i< filenames->size(); i++){
   // printf("checking file %s", (*filenames)[0].c_str());
    int r = stat(filenames->at(i).c_str(), &ts);
    if (r > 0) status = "broken stat"; 
    else if (ts.st_mtime != file_dates.at(i)) {

      return 1;
    }
  }
  return 0;

}
int vmpwStringStreamConcat::update(clock_t nt){

  int incoming = 0;
  clock_t dt = nt-lt; 

  if (dt > update_interval){
    if ( checkDates() ){
    
    incoming = 1;
    makeString();
    printf("%s", val);
    }
    lt = nt;
  }
  return incoming;
}

int vmpwStringStreamConcat::makeString(){

  val = "";
  std::string Line="";

  file_dates.clear();

  struct stat tfs;

  for (string fn : *filenames){
    ifstream fs(fn.c_str(), std::ios::in);
    if(fs.is_open()){
      while(getline(fs,Line)){
        val+=Line;
      }
      int r = stat(fn.c_str(), &tfs);
      if (r > 0) { status =
        "broken stat"; 
        return 1;
      }
      else{
        file_dates.push_back( tfs.st_mtime );
      }
      fs.close();
    }
    else{
      status = "filestream not open!";
    }
  }
  status = "ok";
  return 0;
}
