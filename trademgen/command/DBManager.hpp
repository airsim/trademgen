#ifndef __FORECAST_CMD_DBMANAGER_HPP
#define __FORECAST_CMD_DBMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Forecast
#include <forecast/FORECAST_Types.hpp>

// Forward declarations
namespace soci {
  class session;
  class statement;
}

namespace FORECAST {

  // Forward declarations
  struct Place;

  /** Class building the Business Object Model (BOM) from data retrieved
      from the database. */
  class DBManager {
  public:
    /** Update the fields of the database row
        corresponding to the given BOM object. */
    static void updatePlaceInDB (soci::session&, const Place&);

    /** Retrieve, from the (MySQL) database, the row corresponding to
        the given BOM code, and fill the given BOM object with that retrieved
        data. */
    static bool retrievePlace (soci::session&, const std::string& iPlaceCode,
                               Place&);

    
  public:
    /** Prepare (parse and put in cache) the SQL statement. */
    static void prepareSelectStatement (soci::session&, soci::statement&,
                                        Place&);

    /** Iterate on the SQL statement.
        <br>The SQL has to be already prepared.
        @parameter const bool Tells whether the Place object should be reset. */
    static bool iterateOnStatement (soci::statement&, Place&,
                                    const bool iShouldDoReset);

    
  private:
    /** Prepare (parse and put in cache) the SQL statement. */
    static void prepareSelectOnPlaceCodeStatement(soci::session&,
                                                  soci::statement&,
                                                  const std::string& iPlaceCode,
                                                  Place&);

    
  private:
    /** Constructors. */
    DBManager() {}
    DBManager(const DBManager&) {}
    /** Destructor. */
    ~DBManager() {}
  };
  
}
#endif // __FORECAST_CMD_DBMANAGER_HPP
