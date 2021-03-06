/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Testing IO for Sesam FEM `BNLOAD` cards.

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_test_fem_cards_bnload[]) =
        "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <catch.hpp>

#ifdef __GNUC__
#include "config.h"
#endif

#include "fem/cards.h"

#if defined(_DEBUG) && defined(DEBUG_NEW)
#define new DEBUG_NEW
#ifdef THIS_FILE
#undef THIS_FILE
#endif // THIS_FILE
namespace {
    char const THIS_FILE[] = __FILE__;
}
#endif

using namespace std;

using namespace dnvgl::extfem::fem;
using namespace dnvgl::extfem::fem::cards;

TEST_CASE("FEM BNLOAD definitions.", "[fem_bnload]" ) {

   vector<double> ref_rload({0., 0., 2.e6, 0., 0., 0.});
   vector<std::string> lines;
   size_t len;

   SECTION("BNLOAD (1)") {
      vector<std::string> data({
         "BNLOAD   1.00000000e+000 0.00000000e+000 0.00000000e+000 0.00000000e+000\n",
         "         1.52470000e+004 6.00000000e+000 0.00000000e+000 0.00000000e+000\n",
         "         2.00000000e+006 0.00000000e+000 0.00000000e+000 0.00000000e+000\n"});
      len = __base::card::card_split(data, data.size(), lines);
      bnload probe(lines, len);

      CHECK(probe.LLC == 1);
      CHECK(probe.LOTYP == 0);
      CHECK_FALSE(probe.COMPLX);
      CHECK(probe.NODENO == 15247);
      CHECK(probe.NDOF == 6);
      CHECK(probe.RLOAD == ref_rload);
      CHECK(probe.ILOAD == vector<double>({}));
   }

   SECTION("BNLOAD (2)") {
      vector<std::string> data({
         "BNLOAD   1.00000000e+000 0.00000000e+000 0.00000000e+000 0.00000000e+000\n",
         "         1.52470000e+004 6.00000000e+000 0.00000000e+000 0.00000000e+000\n",
         "         2.00000000e+006 0.00000000e+000 0.00000000e+000 0.00000000e+000\n"});
      len = __base::card::card_split(data, data.size(), lines);
      bnload probe(lines, len);

      CHECK(probe.LLC == 1);
      CHECK(probe.LOTYP == 0);
      CHECK_FALSE(probe.COMPLX);
      CHECK(probe.NODENO == 15247);
      CHECK(probe.NDOF == 6);
      CHECK(probe.RLOAD == ref_rload);
      CHECK(probe.ILOAD == vector<double>({}));
   }
}

TEST_CASE("FEM BNLOAD types output.", "[fem_bnload,out]") {

    std::ostringstream test;

    SECTION("empty") {
        bnload const probe;
        test << probe;
        CHECK(test.str() == "");
    }

    SECTION("fixed") {
        bnload const probe(1, 1, false, 4, 6,
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n");
    }

    SECTION("simple") {
        bnload const probe(1, 1, false, 4, 6,
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n");
    }

    SECTION("simple (with ILOAD)") {
        bnload const probe(1, 1, true, 4, 6,
                     vector<double>({1., 2., 3., 4., 5., 6.}),
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +1.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n"
              "        +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n"
              "        +5.000000000e+00+6.000000000e+00\n");
    }

    SECTION("simple (calc COMPLX)") {
        bnload const probe(1, 1, long(4), 6,
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n");
    }

    SECTION("simple (with ILOAD, calc COMPLX)") {
        bnload const probe(1, 1, long(4), 6,
                     vector<double>({1., 2., 3., 4., 5., 6.}),
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +1.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n"
              "        +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n"
              "        +5.000000000e+00+6.000000000e+00\n");
    }

    SECTION("calc ndof") {
        bnload const probe(1, 1, false, 4,
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n");
    }
    SECTION("calc NDOF (calc COMPLX)") {
        bnload const probe(1, 1, 4,
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n");
    }

    SECTION("calc NDOF (with ILOAD, calc COMPLX)") {
        bnload const probe(1, 1, 4,
                     vector<double>({1., 2., 3., 4., 5., 6.}),
                     vector<double>({1., 2., 3., 4., 5., 6.}));
        test << probe;
        CHECK(test.str() ==
              "BNLOAD  +1.000000000e+00+1.000000000e+00           +1.00            0.00\n"
              "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n"
              "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n"
              "        +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n"
              "        +5.000000000e+00+6.000000000e+00\n");
    }
}

TEST_CASE("FEM BNLOAD conversion from own output.", "[fem_bnload,in/out]") {

    vector<std::string> lines;
    size_t len;

    SECTION("BNLOAD (own output real)") {
        vector<std::string> data({
            "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n",
            "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n",
            "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n"});
        len = __base::card::card_split(data, data.size(), lines);
        bnload probe(lines, len);

        CHECK(probe.LLC == 1);
        CHECK(probe.LOTYP == 1);
        CHECK_FALSE(probe.COMPLX);
        CHECK(probe.NODENO == 4);
        CHECK(probe.NDOF == 6);
        CHECK(probe.RLOAD == vector<double>({1., 2., 3., 4., 5., 6.}));
        CHECK(probe.ILOAD == vector<double>({}));
    }

    SECTION("BNLOAD (own output real alt)") {
        vector<std::string> data({
            "BNLOAD  +1.000000000e+00+1.000000000e+00           +0.00            0.00\n",
            "        +4.000000000e+00+5.000000000e+00+1.000000000e+00+2.000000000e+00\n",
            "        +3.000000000e+00+4.000000000e+00+5.000000000e+00\n"});
        len = __base::card::card_split(data, data.size(), lines);
        bnload probe(lines, len);

        CHECK(probe.LLC == 1);
        CHECK(probe.LOTYP == 1);
        CHECK_FALSE(probe.COMPLX);
        CHECK(probe.NODENO == 4);
        CHECK(probe.NDOF == 5);
        CHECK(probe.RLOAD == vector<double>({1., 2., 3., 4., 5.}));
        CHECK(probe.ILOAD == vector<double>({}));
    }

    SECTION("BNLOAD (own output complex)") {
        vector<std::string> data({
            "BNLOAD  +1.000000000e+00+1.000000000e+00           +1.00            0.00\n",
            "        +4.000000000e+00+6.000000000e+00+1.000000000e+00+2.000000000e+00\n",
            "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+6.000000000e+00\n",
            "        +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n",
            "        +5.000000000e+00+6.000000000e+00\n"});
        len = __base::card::card_split(data, data.size(), lines);
        bnload probe(lines, len);

        CHECK(probe.LLC == 1);
        CHECK(probe.LOTYP == 1);
        CHECK(probe.COMPLX);
        CHECK(probe.NODENO == 4);
        CHECK(probe.NDOF == 6);
        CHECK(probe.RLOAD == vector<double>({1., 2., 3., 4., 5., 6.}));
        CHECK(probe.ILOAD == vector<double>({1., 2., 3., 4., 5., 6.}));
    }

    SECTION("BNLOAD (own output complex alt)") {
        vector<std::string> data({
            "BNLOAD  +1.000000000e+00+1.000000000e+00           +1.00            0.00\n",
            "        +4.000000000e+00+5.000000000e+00+1.000000000e+00+2.000000000e+00\n",
            "        +3.000000000e+00+4.000000000e+00+5.000000000e+00+1.000000000e+00\n",
            "        +2.000000000e+00+3.000000000e+00+4.000000000e+00+5.000000000e+00\n"});
        len = __base::card::card_split(data, data.size(), lines);
        bnload probe(lines, len);

        CHECK(probe.LLC == 1);
        CHECK(probe.LOTYP == 1);
        CHECK(probe.COMPLX);
        CHECK(probe.NODENO == 4);
        CHECK(probe.NDOF == 5);
        CHECK(probe.RLOAD == vector<double>({1., 2., 3., 4., 5.}));
        CHECK(probe.ILOAD == vector<double>({1., 2., 3., 4., 5.}));
    }
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j8&&make -C ../cbuild test"
// End:
