/**
   \file test_fem_cards_glsecr.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2017 by DNV GL SE
   \brief Testing IO for Sesam FEM `GLSECR` cards.

   Detailed description
*/

#include "extfem_misc.h"

// ID:
namespace {
    char const cID_test_fem_cards_glsecr[] _EXTFEMIO_UNUSED =
        "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <limits>

#include <catch.hpp>

#include "config.h"

#include "fem/cards.h"

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

using namespace dnvgl::extfem::fem;
using namespace dnvgl::extfem::fem::cards;

CATCH_TRANSLATE_EXCEPTION(exception &ex) {
    return ex.what();
}

CATCH_TRANSLATE_EXCEPTION(std::string const &ex) {
    return ex;
}

TEST_CASE("FEM GLSECR definitions.", "[fem_glsecr]" ) {

    vector<std::string> lines;
    size_t len;
    __base::geoprop::reset_geono();

    SECTION("GLSECR (1)") {
        vector<std::string> data(
            // 2345678|234567890123456|234567890123456|234567890123456|234567890123456
            {"GLSECR   5.00000000e+000 4.66000000e+002 1.45000000e+001 1.25000000e+002\n",
             "         1.60000000e+001 1.45000000e+001 1.00000000e+000 1.00000000e+000\n",
             "         5.00000000e+000 1.20000000e+001 1.30000000e+001\n"});
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 5);
        CHECK(probe.HZ == 466.);
        CHECK(probe.TY == 14.5);
        CHECK(probe.BY == 125);
        CHECK(probe.TZ == 16.);
        CHECK(probe.SFY == 14.5);
        CHECK(probe.SFZ == 1.);
        CHECK(probe.K == 1.);
        CHECK(probe.R == 5.);
        CHECK(probe.NLOBY == 12);
        CHECK(probe.NLOBZ == 13);
    }

    SECTION("GLSECR (2)") {
        vector<std::string> data(
            // 2345678|234567890123456|234567890123456|234567890123456|234567890123456
            {"GLSECR   5.00000000e+000 4.66000000e+002 1.45000000e+001 1.25000000e+002\n",
             "         1.60000000e+001 1.00000000e+000 1.00000000e+000 1.00000000e+000\n",
             "         1.60000000e+001\n"});
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 5);
        CHECK(probe.HZ == 466.);
        CHECK(probe.TY == 14.5);
        CHECK(probe.BY == 125);
        CHECK(probe.TZ == 16.);
        CHECK(probe.SFY == 1.);
        CHECK(probe.SFZ == 1.);
        CHECK(probe.K == 1.);
        CHECK(probe.R == 16.);
        CHECK(probe.NLOBY == 0);
        CHECK(probe.NLOBZ == 0);
    }

    SECTION("GLSECR (3)") {
        vector<std::string> data({
                // 345678|234567890123456|234567890123456|234567890123456|234567890123456
                "GLSECR  +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n",
                "        +5.000000000e+00+6.000000000e+00+7.000000000e+00+8.000000000e+00\n",
                "        +9.000000000e+00+1.000000000e+01+1.100000000e+01\n"});
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 1);
        CHECK(probe.HZ == 2.);
        CHECK(probe.TY == 3.);
        CHECK(probe.BY == 4);
        CHECK(probe.TZ == 5.);
        CHECK(probe.SFY == 6.);
        CHECK(probe.SFZ == 7.);
        CHECK(probe.K == 8.);
        CHECK(probe.R == 9.);
        CHECK(probe.NLOBY == 10);
        CHECK(probe.NLOBZ == 11);
    }

    SECTION("GLSECR (4)") {
        gbeamg _EXTFEMIO_UNUSED dummy;
        vector<std::string> data({
                // 345678|234567890123456|234567890123456|234567890123456|234567890123456
                "GLSECR  +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n",
                "        +5.000000000e+00+6.000000000e+00+7.000000000e+00+8.000000000e+00\n",
                "        +9.000000000e+00+1.000000000e+01+1.100000000e+01\n"});
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 1);
        CHECK(probe.HZ == 2.);
        CHECK(probe.TY == 3.);
        CHECK(probe.BY == 4);
        CHECK(probe.TZ == 5.);
        CHECK(probe.SFY == 6.);
        CHECK(probe.SFZ == 7.);
        CHECK(probe.K == 8.);
        CHECK(probe.R == 9.);
        CHECK(probe.NLOBY == 10);
        CHECK(probe.NLOBZ == 11);
    }

    SECTION("GLSECR (5)") {
        gbeamg _EXTFEMIO_UNUSED dummy;
        vector<std::string> data({
                // 345678|234567890123456|234567890123456|234567890123456|234567890123456
                "GLSECR    1.00000000E+00  1.00000001E-01  9.99999978E-03  9.99999978E-03\n",
                "          9.99999978E-03  1.00000001E-01  1.00000000E+00  1.00000000E+00\n",
                "          9.99999978E-03\n" });
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 1);
        CHECK(probe.HZ == .100000001);
        CHECK(probe.TY == .00999999978);
        CHECK(probe.BY == .00999999978);
        CHECK(probe.TZ == .00999999978);
        CHECK(probe.SFY == .100000001);
        CHECK(probe.SFZ == 1.);
        CHECK(probe.R == .00999999978);
        CHECK(probe.NLOBY == 0);
        CHECK(probe.NLOBZ == 0);
    }
}

TEST_CASE("FEM GLSECR types output.", "[fem_glsecr,out]" ) {

    std::ostringstream test;

    __base::geoprop::reset_geono();

    SECTION("empty") {
        glsecr probe;
        test << probe;
        CHECK(test.str() == "");
    }

    SECTION("simple") {
        glsecr probe(1, 2., 3., 4., 5., 6., 7., 8, 9., 10, 11);
        test << probe;
        CHECK(test.str() ==
              "GLSECR  +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n"
              "        +5.000000000e+00+6.000000000e+00+7.000000000e+00+8.000000000e+00\n"
              "        +9.000000000e+00+1.000000000e+01+1.100000000e+01\n");
    }

    SECTION("default NLOB*") {
        glsecr probe(1, 2., 3., 4., 5., 6., 7., 8, 9.);
        test << probe;
        CHECK(test.str() ==
              "GLSECR  +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n"
              "        +5.000000000e+00+6.000000000e+00+7.000000000e+00+8.000000000e+00\n"
              "        +9.000000000e+00\n");
    }
}

TEST_CASE("FEM GLSECR conversion from own output.", "[fem_glsecr,in/out]") {

    vector<std::string> lines;
    size_t len;
    __base::geoprop::reset_geono();

    SECTION("GLSECR (1)") {
        vector<std::string> data(
            // 2345678|234567890123456|234567890123456|234567890123456|234567890123456
            {"GLSECR  +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n",
             "        +5.000000000e+00+6.000000000e+00+7.000000000e+00+8.000000000e+00\n",
             "        +9.000000000e+00+1.000000000e+01+1.100000000e+01\n"});
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 1);
        CHECK(probe.HZ == 2.);
        CHECK(probe.TY == 3.);
        CHECK(probe.BY == 4);
        CHECK(probe.TZ == 5.);
        CHECK(probe.SFY == 6.);
        CHECK(probe.SFZ == 7.);
        CHECK(probe.K == 8.);
        CHECK(probe.R == 9.);
        CHECK(probe.NLOBY == 10);
        CHECK(probe.NLOBZ == 11);
    }

    SECTION("GLSECR (2)") {
        vector<std::string> data(
            // 2345678|234567890123456|234567890123456|234567890123456|234567890123456
            {"GLSECR  +1.000000000e+00+2.000000000e+00+3.000000000e+00+4.000000000e+00\n",
             "        +5.000000000e+00+6.000000000e+00+7.000000000e+00+8.000000000e+00\n",
             "        +9.000000000e+00"});
        len = __base::card::card_split(data, data.size(), lines);
        glsecr probe(lines, len);

        CHECK(probe.GEONO == 1);
        CHECK(probe.HZ == 2.);
        CHECK(probe.TY == 3.);
        CHECK(probe.BY == 4);
        CHECK(probe.TZ == 5.);
        CHECK(probe.SFY == 6.);
        CHECK(probe.SFZ == 7.);
        CHECK(probe.K == 8);
        CHECK(probe.R == 9.);
        CHECK(probe.NLOBY == 0);
        CHECK(probe.NLOBZ == 0);
    }
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j8&&
//    (make -C ../cbuild test;
//     ../cbuild/tests/test_fem_cards --use-colour no)"
// End:
