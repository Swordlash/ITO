//
// Created by Mateusz Goslinowski on 25/06/2021.
//

#ifndef ITO_LOGGING_HPP
#define ITO_LOGGING_HPP

#include <fstream>
#include "logging.hpp"
#include <string>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>

static std::string log_filename = "";
static std::fstream log_stream;

static void set_logging(std::string& log_file) {
    log_filename = log_file;
    log_stream.open(log_file, std::fstream::out | std::fstream::trunc);
}

static void set_logging(std::string&& log_file) {
    log_filename = std::move(log_file);
    log_stream.open(log_filename, std::fstream::out | std::fstream::trunc);
}

// workaround for std::format not being supported by any compiler right now :(
// source: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/8098080

template<typename ... Args>
void log( const std::string&& format, Args ... args )
{
    int size_s = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    snprintf( buf.get(), size, format.c_str(), args ... );

    std::string msg( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside

    auto now = std::chrono::high_resolution_clock::now();
    auto tc = std::chrono::high_resolution_clock::to_time_t(now);

    std::cout << std::put_time(std::localtime(&tc), "[%F %T] ") << msg << std::endl;

    if(log_filename != "")
        log_stream << std::put_time(std::localtime(&tc), "[%F %T] ") << msg << std::endl;
}

#endif //ITO_LOGGING_HPP
