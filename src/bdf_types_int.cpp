// Copyright © 2015 by DNV GL SE

// Definitions for Nastran Bulk data entry types.

// Author    Berthold Höllmann <berthold.hoellmann@dnvgl.com>

// ID:
namespace {
  const char  cID[]
#ifdef __GNUC__
  __attribute__ ((__unused__))
#endif
    = "@(#) $Id$";
}

#include <sstream>
#include <limits>
#include <algorithm>
#include <string>
#ifdef __GNUC__
#include <boost/regex.hpp>
#else
#include <regex>
#endif

#ifndef _MSC_VER
#include <config.h>
#endif
#include "bdf_types.h"
#include "bdf_string.h"

using namespace std;
using namespace bdf::types;
using namespace bdf::type_bounds;
using namespace bdf::string;
#ifdef __GNUC__
using namespace boost;
#endif

bdf_int::bdf_int(std::string name) :
  bdf_type_base(name), bounds(bdf_num_bounds<long>()) {};

bdf_int::bdf_int(std::string name, bdf_num_bounds<long> _bounds) :
  bdf_type_base(name), bounds(_bounds) {};

const regex bdf_int::int_re("[[:space:]]*[\\+-]?[[:digit:]]+[[:space:]]*");

void bdf_int::parse(std::string inp) {
  std::string sval = bdf::string::string(inp).trim();
  if (sval.length() == 0) {
    if (!this->bounds.has_default())
      throw "** BDF INP ERROR **: empty entry without default";
    value = this->bounds.get_default();
  } else {
    if (! regex_match(inp, int_re)) {
      std::string msg("illegal input, no integer\n");
      throw msg;
    }
    istringstream buffer(sval);
    buffer >> value;
  }
  if (!this->bounds.in_bounds(value))
    throw  "** BDF INP ERROR **: boundary condition violated";
}

/*
  Local Variables:
  mode: c++
  ispell-local-dictionary: "english"
  c-file-style: "gl"
  indent-tabs-mode: nil
  compile-command: "make -C .. check"
  coding: utf-8
  End:
*/