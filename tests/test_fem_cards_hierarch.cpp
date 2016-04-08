/**
   \file tests/test_fem_cards_hierarch.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Testing the FEM `HIERARCH` class.

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
#include "fem/errors.h"

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dnvgl::extfem::fem;
using namespace dnvgl::extfem::fem::cards;

CATCH_TRANSLATE_EXCEPTION( errors::error& ex ) {
   return ex();
}

CATCH_TRANSLATE_EXCEPTION( std::string& ex ) {
   return ex;
}

TEST_CASE("FEM HIERARCH definitions. (Small Field Format)", "[fem_hierarch]" ) {
   std::deque<std::string> data({
         // 345678|234567890123456|234567890123456|234567890123456|234567890123456
         "HIERARCH  9.00000000E+00  1.00000000E+00  2.00000000E+00  1.00000000E+00\n",
         "          2.00000000E+00  0.00000000E+00  0.00000000E+00  1.00000000E+00\n",
         "          2.00000000E+00  0.00000000E+00  0.00000000E+00  0.00000000E+00\n"});
   std::deque<std::string> lines;
   card::card_split(data, lines);
   hierarch probe(lines);

   SECTION("first moment") {

      CHECK(probe.NFIELD == 9);
      CHECK(probe.IHREF == 1);
      CHECK(probe.ISELTY == 2);
      CHECK(probe.INDSEL == 1);
      CHECK(probe.ISLEVL == 2);
      CHECK(probe.ITREF == 0);
      CHECK(probe.IHPREF == 0);
      CHECK(probe.NSUB == 1);
      CHECK(probe.IHSREFi.size() == 1);
      CHECK(probe.IHSREFi[0] == 2);
   }
}

TEST_CASE("FEM HIERARCH types output.", "[fem_hierarch,out]" ) {
   std::ostringstream test;

   long NFIELD(1);
   long IHREF(2);
   long ISELTY(3);
   long INDSEL(4);
   long ISLEVL(5);
   long ITREF(6);
   long IHPREF(7);
   long NSUB(8);
   std::deque<long> IHSREFi({9, 10, 11, 12, 13, 14, 15, 16});

   std::string ref(
      "HIERARCH+1.00000000e+00 +2.00000000e+00 +3.00000000e+00 +4.00000000e+00 \n"
      "        +5.00000000e+00 +6.00000000e+00 +7.00000000e+00 +8.00000000e+00 \n"
      "        +9.00000000e+00 +1.00000000e+01 +1.10000000e+01 +1.20000000e+01 \n"
      "        +1.30000000e+01 +1.40000000e+01 +1.50000000e+01 +1.60000000e+01 \n");

   SECTION("write (1)") {
      hierarch probe(NFIELD, IHREF, ISELTY, INDSEL, ISLEVL, ITREF,
                     IHPREF, NSUB, IHSREFi);

      test << probe;
      CHECK(test.str() == ref);
   }

   SECTION("write (1a)") {
      CHECK_THROWS(
         hierarch probe(NFIELD, IHREF, ISELTY, INDSEL, ISLEVL, ITREF,
                        IHPREF, NSUB+1, IHSREFi));
   }

   SECTION("write (2)") {
      hierarch probe(NFIELD, IHREF, ISELTY, INDSEL, ISLEVL, ITREF,
                     IHPREF, IHSREFi);

      test << probe;
      CHECK(test.str() == ref);
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