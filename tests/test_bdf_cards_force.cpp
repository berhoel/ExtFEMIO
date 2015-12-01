/**
   \file tests/test_bdf_cards_force.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Testing the BDF `FORCE` card class.

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

#include "bdf/cards.h"

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <limits>

#include <catch.hpp>

#ifndef _MSC_VER
#include <config.h>
#endif

#include "bdf/cards.h"
#include "bdf/errors.h"

using namespace ::std;
using namespace ::dnvgl::extfem::bdf;
using namespace ::dnvgl::extfem::bdf::cards;

CATCH_TRANSLATE_EXCEPTION( errors::error& ex ) {
   return ex();
}

CATCH_TRANSLATE_EXCEPTION( ::std::string& ex ) {
   return ex;
}

TEST_CASE("BDF FORCE definitions. (Small Field Format)", "[bdf_force]" ) {

   ::std::deque<string> data;
   data.push_back(
      // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
      "FORCE   2       5       6       2.9     0.0     1.9     0.0               \n");
   ::std::deque<string> lines;
   card::card_split(data, lines);
   force probe(lines);

   SECTION("first force") {
      CHECK((long)probe.SID == 2);
      CHECK((long)probe.G == 5);
      CHECK((long)probe.CID == 6);
      CHECK((double)probe.F == 2.9);
      CHECK((double)probe.N1 == 0.);
      CHECK((double)probe.N2 == 1.9);
      CHECK((double)probe.N3 == 0.);
   }
}

TEST_CASE("BDF FORCE types output.", "[bdf_force,out]" ) {

   std::ostringstream test;

   SECTION("reverse") {
      long SID(2), G(5), CID(6);
      double F(2.9), N1(0.), N2(1.9), N3(0.);
      force probe(&SID, &G, &CID, &F, &N1, &N2, &N3);
      test << probe;
      CHECK(test.str() ==
            "FORCE          2       5       62.900+000.000+001.900+000.000+00\n");
   }

   SECTION("reverse part") {
      long SID(2), G(5), CID(6);
      double F(2.9), N1(0.), N2(1.9);
      force probe(&SID, &G, &CID, &F, &N1, &N2);
      test << probe;
      CHECK(test.str() ==
            "FORCE          2       5       62.900+000.000+001.900+00\n");
   }

   SECTION("reverse part (2)") {
      long SID(2), G(5), CID(6);
      double F(2.9), N1(1.9);
      force probe(&SID, &G, &CID, &F, &N1);
      test << probe;
      CHECK(test.str() ==
            "FORCE          2       5       62.900+001.900+00\n");
   }

   SECTION("reverse part (2)") {
      long SID(2), G(5), CID(6);
      double F(2.9), N1(1234.5);
      force probe(&SID, &G, &CID, &F, &N1);
      test << probe;
      CHECK(test.str() ==
            "FORCE*                 2               5               62.90000000000+00\n"
            "*       1.23450000000+03\n");
   }
}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C .. check -j 8"
// End:
