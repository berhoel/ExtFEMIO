/**
   \file test_fem_cards_gelth.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Testing IO for Sesam FEM `GELTH` cards.

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

#define NOMINMAX // To avoid problems with "numeric_limits"

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

TEST_CASE("FEM GELTH definitions.", "[fem_gelth]" ) {

   SECTION("GELTH (1)") {
      ::std::deque<string> data;
      data.push_back(
         "GELTH    6.54394000e+005 1.00000000e-001 0.00000000e+000 0.00000000e+000\n");
      ::std::deque<string> lines = card::card_split(data);
      gelth probe(lines);

      CHECK(probe.GEONO == 654394);
      CHECK(probe.TH == .1);
      CHECK(probe.NINT == 0);
   }

   SECTION("GELTH (2)") {
      ::std::deque<string> data;
      data.push_back(
         "GELTH    6.54394000e+05  1.00000000e-01  0.00000000e+00  0.00000000e+00 \n");
      ::std::deque<string> lines = card::card_split(data);
      gelth probe(lines);

      CHECK(probe.GEONO == 654394);
      CHECK(probe.TH == .1);
      CHECK(probe.NINT == 0);
   }
}

TEST_CASE("FEM GELTH types output.", "[fem_gelth,out]" ) {

   std::ostringstream test;

   SECTION("simple") {
      gelth probe(1, 2., 3);
      test << probe;
      CHECK(test.str() ==
            "GELTH   +1.00000000e+00 +2.00000000e+00 +3.00000000e+00 +0.00000000e+00 \n");
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



// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make test"
// End:
