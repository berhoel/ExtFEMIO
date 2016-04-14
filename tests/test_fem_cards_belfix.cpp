/**
   \file test_fem_cards_belfix.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Testing the FEM `BELFIX` class.

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

TEST_CASE("FEM BELFIX definitions. (Small Field Format)", "[fem_belfix]" ) {

   SECTION("first") {

      std::deque<std::string> data({
            // 345678|234567890123456|234567890123456|234567890123456|234567890123456
            "BELFIX   2.30470000e+004 1.00000000e+000 0.00000000e+000 0.00000000e+000\n",
            "         1.00000000e+000 1.00000000e+000 1.00000000e+000 1.00000000e+000\n",
            "         1.00000000e+000 0.00000000e+000 0.00000000e+000 0.00000000e+000\n"});
      std::deque<std::string> lines;
      card::card_split(data, lines);
      belfix probe(lines);

      CHECK(probe.FIXNO == 23047);
      CHECK(probe.OPT == belfix::FIXATION);
      CHECK(probe.TRANO == 0);
      CHECK(probe.A == std::deque<double>({1., 1., 1., 1., 1., 0.}));
   }
}

TEST_CASE("FEM BELFIX types output.", "[fem_belfix,out]" ) {

   SECTION("write default") {
      std::ostringstream test;

      belfix probe;
      test << probe;

      CHECK(test.str() == "");
   }

   SECTION("write") {
      std::ostringstream test;

      long FIXNO(1);
      belfix::n_opt OPT(belfix::FIXATION);
      long TRANO(1);
      std::deque<double> A({1., 0., .5, 1., 1., 1.});

      belfix probe(FIXNO, OPT, TRANO, A);
      test << probe;

      CHECK(test.str() ==
            "BELFIX  +1.00000000e+00 +1.00000000e+00 +1.00000000e+00  0.00000000e+00 \n"
            "        +1.00000000e+00 +0.00000000e+00 +5.00000000e-01 +1.00000000e+00 \n"
            "        +1.00000000e+00 +1.00000000e+00  0.00000000e+00  0.00000000e+00 \n");
   }

   SECTION("write (const)") {
      std::ostringstream test;

      belfix probe(1, belfix::FIXATION, 1, {1., 0., .5, 1., 1., 1.});
      test << probe;

      CHECK(test.str() ==
            "BELFIX  +1.00000000e+00 +1.00000000e+00 +1.00000000e+00  0.00000000e+00 \n"
            "        +1.00000000e+00 +0.00000000e+00 +5.00000000e-01 +1.00000000e+00 \n"
            "        +1.00000000e+00 +1.00000000e+00  0.00000000e+00  0.00000000e+00 \n");
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
