//
// Created by Mateusz Goslinowski on 07/09/2021.
//

#include "logging.hpp"

#include <fstream>

std::fstream log_stream;

void set_logging(std::string& log_file) {
    log_stream.open(log_file, std::fstream::out);
}