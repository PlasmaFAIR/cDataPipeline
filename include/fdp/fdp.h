#ifndef __CFAIRDATAPIPELINE_H
#define __CFAIRDATAPIPELINE_H

#ifdef __cplusplus
extern "C" {
#endif


// ------
// Errors
// ------


/**
 * @brief Enumeration used to denote different error types.
 *
 * The underlying C++ API will raise a number of different exception types. These
 * map to integer error codes for C compatibility.
 */
enum FDP_ERR_T {
    FDP_ERR_NONE = 0,
    FDP_ERR_CONFIG_PARSE = 1,
    FDP_ERR_REST_API_QUERY = 2,
    FDP_ERR_JSON_PARSE = 3,
    FDP_ERR_VALIDATION = 4,
    FDP_ERR_SYNC = 5,
    FDP_ERR_WRITE = 6,
    FDP_ERR_TOML = 6,
    FDP_ERR_OTHER = 7
};
typedef enum FDP_ERR_T FDP_ERR_T;


// ------------
// FDP Core API
// ------------


/**
 * @brief Initialise the Data Pipeline.
 * 
 * @param config_file_path 
 * @param script_file_path
 * @param token May be set to NULL.
 * @return Error code
 */
FDP_ERR_T fdp_init(const char *config_path, const char *script_path, const char *token);


/**
 * @brief Finalise the pipeline.
 * 
 * Record all data products and meta data to the registry.
 * Update the code run with all appropriate meta data.
 * 
 * @return Error code
 */
FDP_ERR_T fdp_finalise();


/**
 * @brief Set a path to a given data product while recording it's meta data for the
 *        code run.
 * 
 * @param data_product    Path to the input file
 * @param data_store_path Path to the assigned data store location. The user should
 *                        allocate sufficient memory beforehand.
 * @return Error code
 */
FDP_ERR_T fdp_link_read(const char *data_product, char *data_store_path);


/**
 * @brief Set a path to a given data product while recording it's meta data for the
 *        code run.
 * 
 * @param data_product    Path to the output file
 * @param data_store_path Path to the assigned data store location. The user should
 *                        allocate sufficient memory beforehand.
 * @return Error code
 */
FDP_ERR_T fdp_link_write(const char *data_product, char *data_store_path);


// -------
// Logging
// -------

/**
 * @brief Enumeration used to denote the different levels of logging.
 *
 * Each level of logging includes all log levels greater than it, so setting the log
 * level to 'DEBUG' will include all log types except 'TRACE'.
 */
enum FDP_LOG_LEVEL {
    FDP_LOG_TRACE = 0,
    FDP_LOG_DEBUG = 1,
    FDP_LOG_INFO = 2,
    FDP_LOG_WARN = 3,
    FDP_LOG_ERROR = 4,
    FDP_LOG_CRITICAL = 5,
    FDP_LOG_OFF = 6
};
typedef enum FDP_LOG_LEVEL FDP_LOG_LEVEL;


/**
 * @brief Set the log level. Must call fdp_init first.
 *
 * @param log_level
 */
void fdp_set_log_level(FDP_LOG_LEVEL log_level);


/**
 * @brief Get the current log level. Must call fdp_init first.
 * @return Log level
 */
FDP_LOG_LEVEL fdp_get_log_level();

/**
 * @brief Write a message to the log.
 *
 * @param msg The message to be written to log.
 * @param log_level The type of log message to write, e.g. FDP_LOG_INFO, FDP_LOG_ERROR.
 * @return Error code, 1 if logging unsuccessful, 0 otherwise.
 */
int fdp_log(const char* msg, FDP_LOG_LEVEL log_level);


#ifdef __cplusplus
} // close extern "C"
#endif

#endif // __CFAIRDATAPIPELINE_H
