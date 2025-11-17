// Copyright (C) 2024 Cade Weinberg
//
// This file is part of inf.
//
// inf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// inf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with inf.  If not, see <http://www.gnu.org/licenses/>.
#ifndef INF_IMR_ERROR_HPP
#define INF_IMR_ERROR_HPP

#include <exception>
#include <ostream>
#include <source_location>
#include <sstream>
#include <stacktrace>
#include <string>

#include "imr/location.hpp"

inline std::ostream &operator<<(std::ostream               &out,
                                std::source_location const &location) {
    out << location.file_name() << ":" << location.function_name() << ":"
        << location.line() << "." << location.column();
    return out;
}

namespace inf {
class Error : public std::exception {
  public:
    struct Internal {
        std::source_location location;
        std::stacktrace      trace;

        static Internal
        current(std::source_location location = std::source_location::current(),
                std::stacktrace      trace    = std::stacktrace::current()) {
            return {std::move(location), std::move(trace)};
        }
    };

  private:
    std::string m_message;

  public:
    Error() {}
    Error(Error &&other) noexcept : m_message(std::move(other.m_message)) {}
    Error(Error const &other) : m_message(other.m_message) {}
    Error(std::string message) : m_message(std::move(message)) {}
    Error(std::string message, yy::location location) {
        std::ostringstream stream;
        stream << "@[" << location << "]\n" << message;
        m_message = std::move(stream).str();
    }

    Error(std::string message, Internal internal) {
        std::ostringstream stream;
        stream << internal.trace << "\n@[" << internal.location << "]\n"
               << message;
        m_message = std::move(stream).str();
    }

    Error &operator=(Error &&other) noexcept {
        if (this == &other) { return *this; }
        m_message = std::move(other.m_message);
        return *this;
    }

    Error &operator=(Error const &other) {
        if (this == &other) { return *this; }
        m_message = other.m_message;
        return *this;
    }

    static Error current(std::string message, Internal internal = Internal::current()) {
        return {std::move(message), std::move(internal)};
    }

    char const *what() const noexcept override { return m_message.c_str(); }

    std::string const &message() const noexcept { return m_message; }

    friend std::ostream &operator<<(std::ostream &out, Error const &error);
};

inline std::ostream &operator<<(std::ostream &out, Error const &error) {
    out << error.m_message;
    return out;
}
} // namespace inf

#endif // !INF_IMR_ERROR_HPP
