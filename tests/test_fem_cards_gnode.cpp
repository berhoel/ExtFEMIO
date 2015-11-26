/**
  \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
  \copyright Copyright © 2015 by DNV GL SE
  \brief Testing IO for Sesam FEM GNODE cards.

  Detailed description
*/

// ID:
namespace {
  const char  cID[]
#ifdef __GNUC__
  __attribute__ ((__unused__))
#endif
    = "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numdric_limits"

#include <limits>

#include <catch.hpp>

#ifndef _MSC_VER
#include <config.h>
#endif
#include "fem/cards.h"

using namespace ::std;
using namespace ::dnvgl::extfem::fem;
using namespace ::dnvgl::extfem::fem::cards;

CATCH_TRANSLATE_EXCEPTION( errors::error& ex ) {
  return ex();
}

CATCH_TRANSLATE_EXCEPTION( ::std::string& ex ) {
  return ex;
}

TEST_CASE("FEM GNODE definitions.", "[fem_gnode]" ) {

  SECTION("GNODE (1)") {
    ::std::deque<string> data;
    data.push_back("GNODE    1.00000000e+000 1.00000000e+000 3.00000000e+000 1.34000000e+002\n");
    ::std::deque<string> lines = card::card_split(data);
    gnode probe(lines);

    CHECK(probe.NODEX == 1);
    CHECK(probe.NODENO == 1);
    CHECK(probe.NDOF == 3);
    CHECK(probe.ODOF.size() == 3);
    CHECK(probe.ODOF[0] == 1);
    CHECK(probe.ODOF[1] == 3);
    CHECK(probe.ODOF[2] == 4);
  }

  SECTION("GNODE (2)") {
    ::std::deque<string> data;
    data.push_back("GNODE    1.00000000e+00  1.00000000e+00  3.00000000e+00  1.34000000e+02 \n");
    ::std::deque<string> lines = card::card_split(data);
    gnode probe(lines);

    CHECK(probe.NODEX == 1);
    CHECK(probe.NODENO == 1);
    CHECK(probe.NDOF == 3);
    CHECK(probe.ODOF.size() == 3);
    CHECK(probe.ODOF[0] == 1);
    CHECK(probe.ODOF[1] == 3);
    CHECK(probe.ODOF[2] == 4);
  }
}

TEST_CASE("FEM GNODE types output.", "[fem_gnode,out]" ) {

  std::ostringstream test;

  long NODEX(1), NODENO(222), NDOF(3);
  ::std::deque<int> ODOF;
  ODOF.push_back(2);
  ODOF.push_back(6);
  ODOF.push_back(3);

  SECTION("simple") {
    gnode probe(&NODEX, &NODENO, &NDOF, &ODOF);
    test << probe;
    CHECK(test.str() == "GNODE   +1.00000000e+00 +2.22000000e+02 +3.00000000e+00  2.36000000e+02 \n");
  }

  SECTION("simple (2)") {
    gnode probe(&NODEX, &NODENO, &ODOF);
    test << probe;
    CHECK(test.str() == "GNODE   +1.00000000e+00 +2.22000000e+02 +3.00000000e+00  2.36000000e+02 \n");
  }
}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C .. check -j8"
// End: