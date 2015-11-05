// Copyright © 2015 by DNV GL SE

// Purpose: Error Handling for processing FEM data.

// Author Berthold Höllmann <berthold.hoellmann@dnvgl.com>

// ID:
namespace {
  const char  cID[]
#ifdef __GNUC__
  __attribute__ ((__unused__))
#endif
    = "@(#) $Id$";
}

#include <string>

#include "fem/errors.h"

using namespace ::dnvgl::extfem::fem::errors;

error::error(
  const std::string &msg, const std::string &err_class) :
  msg(msg), name(""), err_class(err_class) {}


error::error(
  const std::string &name, const std::string &msg,
  const std::string &err_class) :
  msg(msg), name(name), err_class(err_class) {};

std::string error::get_msg(void) const {
  if (name.length())
    return err_class + ":" + name + ":" + msg;
  return err_class + ":" + msg;
}

std::string error::operator() (void) const {
  return this->get_msg() + "\n";
}

types_error::types_error(const std::string &msg) :
  error("", msg, "types_error") {}

float_error::float_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "float_error") {}

int_error::int_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "int_error") {}

output_error::output_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "output_error") {}

list_error::list_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "list_error") {}

str_error::str_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "str_error") {};

string_error::string_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "string_error") {};

parse_error::parse_error(
  const std::string &name, const std::string &msg) :
  error(name, msg, "parse_error") {}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../.. check -j8"
// End: