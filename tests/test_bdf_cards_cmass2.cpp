/**
   \file tests/test_bdf_cards_cmass2.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Testing the BDF `CMASS2` card class.

   Detailed description
*/

#include "extfem_misc.h"

// ID:
namespace {
    char const cID_test_bdf_cards_cmass2[] _EXTFEMIO_UNUSED =
        "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <limits>

#include <catch.hpp>

#include "config.h"

#include "bdf/cards.h"
#include "bdf/errors.h"

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

using namespace dnvgl::extfem::bdf;
using namespace dnvgl::extfem::bdf::cards;

TEST_CASE("BDF CMASS2 definitions. (Small Field Format)", "[bdf_cmass2]" ) {

    SECTION("first cmass2") {
        std::list<std::string> data({
                // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
                "CMASS2  1       3.      32      8\n"});
        std::list<std::string> lines;
        __base::card::card_split(data, lines);
        cmass2 probe(lines);

        REQUIRE((long)probe.EID == 1);
        REQUIRE((double)probe.M == 3.);
        REQUIRE((long)probe.G1 == 32);
        REQUIRE(probe.C1 == std::list<int>(1, 8));
        REQUIRE_FALSE(probe.G2);
        REQUIRE(probe.C2 == std::list<int>());
    }

    SECTION("full cmass2") {
        std::list<std::string> data({
                // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
                "CMASS2  1       3.      32      8       43      2\n"});
        std::list<std::string> lines;
        __base::card::card_split(data, lines);
        cmass2 probe(lines);

        REQUIRE((long)probe.EID == 1);
        REQUIRE((double)probe.M == 3.);
        REQUIRE((long)probe.G1 == 32);
        REQUIRE(probe.C1 == std::list<int>(1, 8));
        REQUIRE((long)probe.G2 == 43);
        REQUIRE(probe.C2 == std::list<int>(1, 2));
    }

    SECTION("only second vals cmass2") {
        std::list<std::string> data({
                // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
                "CMASS2  1       3.                      32      12\n"});
        std::list<std::string> lines;
        __base::card::card_split(data, lines);
        cmass2 probe(lines);

        REQUIRE((long)probe.EID == 1);
        REQUIRE((double)probe.M == 3.);
        REQUIRE_FALSE(probe.G1);
        REQUIRE(probe.C1 == std::list<int>());
        REQUIRE((long)probe.G2 == 32);
        REQUIRE(probe.C2 == std::list<int>({1, 2}));
    }
}

TEST_CASE("BDF CMASS2 types output.", "[bdf_cmass2,out]" ) {

    std::ostringstream test;

    SECTION("empty") {
        cmass2 probe;
        test << probe;
        REQUIRE(test.str() == "");
    }

    SECTION("reverse") {
        long EID(2), S1(6), S2(1);
        std::list<int> C1({1, 2, 3}), C2({4, 5, 6});
        double M(2.9);
        cmass2 probe(&EID, &M, &S1, &C1, &S2, &C2);
        test << probe;
        REQUIRE(test.str() ==
                // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
                "CMASS2         22.900+00       6     123       1     456\n");
    }

    SECTION("reverse part") {
        long EID(2), S1(6);
        double M(2.9);
        std::list<int> C1({1, 2, 3});
        cmass2 probe(&EID, &M, &S1, &C1);
        test << probe;
        REQUIRE(test.str() ==
                // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
                "CMASS2         22.900+00       6     123\n");
    }

    SECTION("failed part") {
        long EID(0), S1(6);
        double M(2.9);
        std::list<int> C1({1, 2, 3});
        REQUIRE_THROWS(cmass2(&EID, &M, &S1, &C1));
    }

    SECTION("multiple") {
        long EID(2), S1(6);
        double M(2.9);
        std::list<int> C1({1, 2, 3});
        cmass2 probe;
        test << probe;
        test << probe(&EID, &M, &S1, &C1);
        EID++;
        test << probe(&EID, &M, &S1, &C1);
        test << probe;
        REQUIRE(test.str() ==
                // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
                "CMASS2         22.900+00       6     123\n"
                "CMASS2         32.900+00       6     123\n"
                "CMASS2         32.900+00       6     123\n");
    }
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j8&&
//   (make -C ../cbuild test;
//    ../cbuild/tests/test_bdf_cards --use-colour no)"
// End:
