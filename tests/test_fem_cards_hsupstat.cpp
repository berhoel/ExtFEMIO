/**
   \file tests/test_fem_cards_hsupstat.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Testing the FEM `HSUPSTAT` class.

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

TEST_CASE("FEM HSUPSTAT definitions. (Small Field Format)", "[fem_hsupstat]" ) {

   std::deque<std::string> data({
         // 345678|234567890123456|234567890123456|234567890123456|234567890123456
         "HSUPSTAT  9.00000000E+00  2.00000000E+00  6.00000000E+00  0.00000000E+00\n",
         "          5.00000000E+00  0.00000000E+00  0.00000000E+00  0.00000000E+00\n",
         "         -1.00000000E+00  0.00000000E+00  0.00000000E+00  0.00000000E+00\n"});
   std::deque<std::string> lines;
   card::card_split(data, lines);
   hsupstat probe(lines);

   SECTION("first moment") {
      CHECK(probe.NFIELD == 9);
      CHECK(probe.ISELTY == 2);
      CHECK(probe.NIDOF == 6);
      CHECK(probe.NRDOF == 0);
      CHECK(probe.NBAND == 5);
      CHECK(probe.NELT == 0);
      CHECK(probe.LINDEP == 0);
      CHECK(probe.RELOADC == 0);
      CHECK(probe.COMPLC == -1);
   }
}

TEST_CASE("FEM HSUPSTAT types output.", "[fem_hsupstat,out]" ) {
   std::ostringstream test;

   long NFIELD(1);
   long ISELTY(2);
   long NIDOF(3);
   long NRDOF(4);
   long NBAND(5);
   long NELT(6);
   long LINDEP(7);
   long RELOADC(8);
   long COMPLC(9);

   SECTION("write (1)") {
      hsupstat probe(NFIELD, ISELTY, NIDOF, NRDOF, NBAND, NELT,
                     LINDEP, RELOADC, COMPLC);

      test << probe;
      CHECK(test.str() ==
            "HSUPSTAT+1.00000000e+00 +2.00000000e+00 +3.00000000e+00 +4.00000000e+00 \n"
            "        +5.00000000e+00 +6.00000000e+00 +7.00000000e+00 +8.00000000e+00 \n"
            "        +9.00000000e+00  0.00000000e+00  0.00000000e+00  0.00000000e+00 \n");
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
