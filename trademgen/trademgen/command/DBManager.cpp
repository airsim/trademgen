// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// SOCI
#include <soci/core/soci.h>
#include <soci/backends/mysql/soci-mysql.h>
// Trademgen
#include <trademgen/command/DBManager.hpp>
#include <trademgen/service/Logger.hpp>

namespace TRADEMGEN {

  //
  struct Place {
  };

  // //////////////////////////////////////////////////////////////////////
  void DBManager::prepareSelectStatement (soci::session& ioSociSession,
                                          soci::statement& ioSelectStatement,
                                          Place& ioPlace) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_place_details rpd, ref_place_names rpn
         where rpd.code = rpn.code

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_place_details rpd, ref_place_names rpn "
         << "where rpd.code = rpn.code", soci::into (ioPlace));

      // Execute the SQL query
      ioSelectStatement.execute();
      */

    } catch (std::exception const& lException) {
      TRADEMGEN_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectOnPlaceCodeStatement (soci::session& ioSociSession,
                                     soci::statement& ioSelectStatement,
                                     const std::string& iPlaceCode,
                                     Place& ioPlace) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_place_details rpd, ref_place_names rpn
         where rpd.code = iPlaceCode
           and rpn.code = rpd.code;

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_place_details rpd, ref_place_names rpn "
         << "where rpd.code = :place_code "
         << "and rpn.code = rpd.code",
         soci::into (ioPlace), soci::use (iPlaceCode));

      // Execute the SQL query
      ioSelectStatement.execute();
      */

    } catch (std::exception const& lException) {
      TRADEMGEN_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::iterateOnStatement (soci::statement& ioStatement,
                                      Place& ioPlace,
                                      const bool iShouldDoReset) {
    bool hasStillData = false;
  
    try {

      // Reset the list of names of the given Place object
      if (iShouldDoReset == true) {
        // ioPlace.resetMatrix();
      }

      // Retrieve the next row of Place object
      hasStillData = ioStatement.fetch();
      
    } catch (std::exception const& lException) {
      TRADEMGEN_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }

    return hasStillData;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::updatePlaceInDB (soci::session& ioSociSession,
                                   const Place& iPlace) {
  
    try {
    
      // Begin a transaction on the database
      ioSociSession.begin();
      
      // Instanciate a SQL statement (no request is performed at that stage)
      std::string lPlaceCode;
      /*
      soci::statement lUpdateStatement =
        (ioSociSession.prepare
         << "update ref_place_details "
         << "set xapian_docid = :xapian_docid "
         << "where code = :code", soci::use (lDocID), soci::use (lPlaceCode));
      
      // Execute the SQL query
      lDocID = iPlace.getDocID();
      lPlaceCode = iPlace.getPlaceCode();
      lUpdateStatement.execute (true);
      */
      
      // Commit the transaction on the database
      ioSociSession.commit();
        
      // Debug
      // TRADEMGEN_LOG_DEBUG ("[" << lDocID << "] " << iPlace);
      
    } catch (std::exception const& lException) {
      TRADEMGEN_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::retrievePlace (soci::session& ioSociSession,
                                 const std::string& iPlaceCode,
                                 Place& ioPlace) {
    bool oHasRetrievedPlace = false;
      
    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      DBManager::prepareSelectOnPlaceCodeStatement (ioSociSession,
                                                    lSelectStatement,
                                                    iPlaceCode, ioPlace);
      const bool shouldDoReset = true;
      bool hasStillData = iterateOnStatement (lSelectStatement, ioPlace,
                                              shouldDoReset);
      if (hasStillData == true) {
        oHasRetrievedPlace = true;
      }

      // Sanity check
      const bool shouldNotDoReset = false;
      hasStillData = iterateOnStatement (lSelectStatement, ioPlace,
                                         shouldNotDoReset);
      // Debug
      // TRADEMGEN_LOG_DEBUG ("[" << iDocID << "] " << ioPlace);
      
    } catch (std::exception const& lException) {
      TRADEMGEN_LOG_ERROR ("Error: " << lException.what());
      throw SQLDatabaseException();
    }

    return oHasRetrievedPlace;
  }

}
