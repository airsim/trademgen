#ifndef __TRADEMGEN_SVC_LOGGER_HPP
#define __TRADEMGEN_SVC_LOGGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// STL
#include <sstream>
#include <string>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Types.hpp>

// /////////////// LOG MACROS /////////////////
#define TRADEMGEN_LOG_CORE(iLevel, iToBeLogged) \
  { std::ostringstream ostr; ostr << iToBeLogged; \
    TRADEMGEN::Logger::instance().log (iLevel, __LINE__, __FILE__, ostr.str()); }

#define TRADEMGEN_LOG_CRITICAL(iToBeLogged) \
  TRADEMGEN_LOG_CORE (TRADEMGEN::LOG::CRITICAL, iToBeLogged)

#define TRADEMGEN_LOG_ERROR(iToBeLogged) \
  TRADEMGEN_LOG_CORE (TRADEMGEN::LOG::ERROR, iToBeLogged)

#define TRADEMGEN_LOG_NOTIFICATION(iToBeLogged) \
  TRADEMGEN_LOG_CORE (TRADEMGEN::LOG::NOTIFICATION, iToBeLogged)

#define TRADEMGEN_LOG_WARNING(iToBeLogged) \
  TRADEMGEN_LOG_CORE (TRADEMGEN::LOG::WARNING, iToBeLogged)

#define TRADEMGEN_LOG_DEBUG(iToBeLogged) \
  TRADEMGEN_LOG_CORE (TRADEMGEN::LOG::DEBUG, iToBeLogged)

#define TRADEMGEN_LOG_VERBOSE(iToBeLogged) \
  TRADEMGEN_LOG_CORE (TRADEMGEN::LOG::VERBOSE, iToBeLogged)
// /////////// (END OF) LOG MACROS /////////////


namespace TRADEMGEN {

  /** Class holding the stream for logs. 
      <br>Note that the error logs are seen as standard output logs, 
      but with a higher level of visibility. */
  class Logger {
    // Friend classes
    friend class FacSupervisor;
  public:
    
    /** Main log entry. */
    template <typename T>
    void log (const LOG::EN_LogLevel iLevel, const int iLineNumber,
              const std::string& iFileName, const T& iToBeLogged) {
      if (iLevel <= _level) {
        assert (_logStream != NULL);
        *_logStream << iFileName << ":" << iLineNumber
                    << ": " << iToBeLogged << std::endl;
      }
    }
    
    /** Get the log level. */
    LOG::EN_LogLevel getLogLevel();
    
    /** get the log stream. */
    std::ostream& getLogStream();
    
    /** Set the logger parameters (level and stream). */
    void setLogParameters (const LOG::EN_LogLevel iLogLevel, 
                           std::ostream& ioLogStream);
    
    /** Returns a current Logger instance.*/
    static Logger& instance();
    
  private:
    /** Default constructors are private so that only the required 
        constructor can be used. */
    Logger ();
    Logger (const Logger&);
    Logger (const LOG::EN_LogLevel iLevel, std::ostream& ioLogStream);
    /** Destructor. */
    ~Logger ();
    
  private:
    /** Log level. */
    LOG::EN_LogLevel _level;
    
    /** Stream dedicated to the logs. */
    std::ostream* _logStream;
    
    /** Instance object.*/
    static Logger* _instance;
  };
  
}
#endif // __TRADEMGEN_SVC_LOGGER_HPP
