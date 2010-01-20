// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// STL
#include <string>
#include <sstream>
// SOCI
#include <soci/core/soci.h>
#include <soci/backends/mysql/soci-mysql.h>
// Trademgen
#include <trademgen/DBParams.hpp>
#include <trademgen/command/SociSessionManager.hpp>
#include <trademgen/service/Logger.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void SociSessionManager::init (soci::session*& ioSociSession_ptr,
                                 const DBParams& iDBParams) {
    
    // Database parameters
    std::ostringstream oStr;
    oStr << "db=" << iDBParams.getDBName() << " user=" << iDBParams.getUser()
         << " password=" << iDBParams.getPassword()
         << " port=" << iDBParams.getPort() << " host=" << iDBParams.getHost();
    const std::string lSociSessionConnectionString (oStr.str());

    // Instanciate a SOCI Session: nothing is performed at that stage
    ioSociSession_ptr = new soci::session();
    
    try {

      // Open the connection to the database
      ioSociSession_ptr->open (soci::mysql, lSociSessionConnectionString);
      
    } catch (std::exception const& lException) {
      TRADEMGEN_LOG_ERROR ("Error while opening a connection to database: "
                          << lException.what());
      TRADEMGEN_LOG_ERROR ("Database parameters used:"
                          << " db=" << iDBParams.getDBName()
                          << " user=" << iDBParams.getUser()
                          << " port=" << iDBParams.getPort()
                          << " host=" << iDBParams.getHost());
      throw SQLDatabaseConnectionImpossibleException();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SociSessionManager::finalise (soci::session*& ioSociSession_ptr) {
    delete ioSociSession_ptr; ioSociSession_ptr = NULL;
  }
  
}