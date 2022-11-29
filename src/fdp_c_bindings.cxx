#include <string>
#include <algorithm>
#include <map>

#include <ghc/filesystem.hpp>

#include "fdp/fdp.h"
#include "fdp/fdp.hxx"
#include "fdp/exceptions.hxx"
#include "fdp/utilities/logging.hxx"

namespace FDP = FairDataPipeline;
namespace fs = ghc::filesystem;

// Utility method to call exception-raising functions and return error codes instead.

template<typename F, typename R, typename... Args>
FDP_ERR_T exception_to_err_code(F&& f, R& r, Args&&... args){
    try{
        r = f(args...);
        return FDP_ERR_NONE;
    }
    catch(const FDP::config_parsing_error&){
        return FDP_ERR_CONFIG_PARSE;
    }
    catch(const FDP::rest_apiquery_error&){
        return FDP_ERR_REST_API_QUERY;
    }
    catch(const FDP::json_parse_error&){
        return FDP_ERR_JSON_PARSE;
    }
    catch(const FDP::validation_error&){
        return FDP_ERR_VALIDATION;
    }
    catch(const FDP::sync_error&){
        return FDP_ERR_SYNC;
    }
    catch(const FDP::write_error&){
        return FDP_ERR_WRITE;
    }
    catch(const FDP::toml_error&){
        return FDP_ERR_TOML;
    }
    catch(...){
        return FDP_ERR_OTHER;
    }
}

// ------------
// Core FDP API
// ------------

// Globally accessible pointer to DataPipeline object.
// This is accessed by fdp_init, fdp_finalise, fdp_link_read, and fdp_link_write.

FDP::DataPipeline::sptr _datapipeline;

FDP_ERR_T fdp_init(
    const char *config_path,
    const char *script_path,
    const char *token
){
    std::string _token = "";
    if(token != nullptr) _token = token;

    return exception_to_err_code(
        FDP::DataPipeline::construct,
        _datapipeline,
        config_path,
        script_path,
        _token
    );
}

// Require static interface to _datapipeline for fdp_finalise in order to use
// exception_to_err_code. Must also return something.
int _fdp_finalise(){
    _datapipeline->finalise();
    return 0;
}

FDP_ERR_T fdp_finalise(){
    int ret;
    return exception_to_err_code(_fdp_finalise, ret);
}

// Require static interfaces to _datapipeline for link_read/write
fs::path _fdp_link_read(std::string& path){
    return _datapipeline->link_read(path);
}

fs::path _fdp_link_write(std::string& path){
    return _datapipeline->link_write(path);
}

template<typename F>
FDP_ERR_T _fdp_link(F&& f, const char* path, char* output){
    std::string input_path = path;
    fs::path output_path;
    FDP_ERR_T err = exception_to_err_code(f, output_path, input_path);
    if(err){
        return err;
    } else {
        std::string output_str = output_path.string();
        std::copy(output_str.begin(), output_str.end(), output);
        return FDP_ERR_NONE;
    }
}


FDP_ERR_T fdp_link_read(const char* path, char* output){
    return _fdp_link(_fdp_link_read, path, output);
}

FDP_ERR_T fdp_link_write(const char* path, char* output){
    return _fdp_link(_fdp_link_write, path, output);
}


// -------
// Logging
// -------

std::map<FDP_LOG_LEVEL, FDP::logging::LOG_LEVEL> to_cpp_enum = {
    {FDP_LOG_TRACE, FDP::logging::TRACE},
    {FDP_LOG_DEBUG, FDP::logging::DEBUG},
    {FDP_LOG_INFO, FDP::logging::INFO},
    {FDP_LOG_WARN, FDP::logging::WARN},
    {FDP_LOG_ERROR, FDP::logging::ERROR},
    {FDP_LOG_CRITICAL, FDP::logging::CRITICAL},
    {FDP_LOG_OFF, FDP::logging::OFF}
};

std::map<FDP::logging::LOG_LEVEL, FDP_LOG_LEVEL> to_c_enum = {
    {FDP::logging::TRACE, FDP_LOG_TRACE},
    {FDP::logging::DEBUG, FDP_LOG_DEBUG},
    {FDP::logging::INFO, FDP_LOG_INFO},
    {FDP::logging::WARN, FDP_LOG_WARN},
    {FDP::logging::ERROR, FDP_LOG_ERROR},
    {FDP::logging::CRITICAL, FDP_LOG_CRITICAL},
    {FDP::logging::OFF, FDP_LOG_OFF}
};

void fdp_set_log_level(FDP_LOG_LEVEL log_level){
    FDP::logger::get_logger()->set_level(to_cpp_enum[log_level]);
}

FDP_LOG_LEVEL fdp_get_log_level(){
    return to_c_enum[FDP::logger::get_logger()->get_level()];
}

int fdp_log(const char* msg, FDP_LOG_LEVEL log_level){
    switch(log_level){
        case FDP_LOG_TRACE:
            FDP::logger::get_logger()->trace() << msg;
            break;
        case FDP_LOG_DEBUG:
            FDP::logger::get_logger()->debug() << msg;
            break;
        case FDP_LOG_INFO:
            FDP::logger::get_logger()->info() << msg;
            break;
        case FDP_LOG_WARN:
            FDP::logger::get_logger()->warn() << msg;
            break;
        case FDP_LOG_ERROR:
            FDP::logger::get_logger()->error() << msg;
            break;
        case FDP_LOG_CRITICAL:
            FDP::logger::get_logger()->critical() << msg;
            break;
        default:
            return 1;
    }
    return 0;
}
