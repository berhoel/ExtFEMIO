/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Testing the BDF `FORCE` card class.

   Detailed description
   */
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    char const _EXTFEMIO_UNUSED(cID_test_bdf_cards_force[]) =
        "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <catch.hpp>

#ifdef __GNUC__
#include "config.h"
#endif

#include "bdf/cards.h"

#if defined(_DEBUG) && defined(DEBUG_NEW)
#define new DEBUG_NEW
#ifdef THIS_FILE
#undef THIS_FILE
#endif // THIS_FILE
namespace {
    char const THIS_FILE[] = __FILE__;
}
#endif

using namespace dnvgl::extfem::bdf;
using namespace dnvgl::extfem::bdf::cards;

TEST_CASE("BDF FORCE definitions. (Small Field Format)", "[bdf_force]") {

    std::list<std::string> const data({
        // 345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
        "FORCE   2       5       6       2.9     0.0     1.9     0.0               \n"});
    std::list<std::string> lines;
    __base::card::card_split(data, lines);
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

TEST_CASE("BDF FORCE types output.", "[bdf_force,out]") {

    std::ostringstream test;

    SECTION("reverse") {
        long SID{2}, G{5}, CID{6};
        double F(2.9), N1(0.), N2(1.9), N3(0.);
        force const probe(&SID, &G, &CID, &F, &N1, &N2, &N3);
        test << probe;
        CHECK(test.str() ==
              "FORCE          2       5       62.900+00 0.00+001.900+00 0.00+00\n");
    }

    SECTION("reverse part") {
        long SID{2}, G{5}, CID{6};
        double F(2.9), N1(0.), N2(1.9);
        force const probe(&SID, &G, &CID, &F, &N1, &N2);
        test << probe;
        CHECK(test.str() ==
              "FORCE          2       5       62.900+00 0.00+001.900+00\n");
    }

    SECTION("reverse part (2)") {
        long SID{2}, G{5}, CID{6};
        double F(2.9), N1(1.9);
        force const probe(&SID, &G, &CID, &F, &N1);
        test << probe;
        CHECK(test.str() ==
              "FORCE          2       5       62.900+001.900+00\n");
    }

    SECTION("reverse part (3)") {
        long SID{2}, G{5}, CID{6};
        double F(2.9), N1(1234.5);
        force const probe(&SID, &G, &CID, &F, &N1);
        test << probe;
        CHECK(test.str() ==
              "FORCE*                 2               5               62.90000000000+00\n"
              "*       1.23450000000+03\n");
    }

    SECTION("reuse") {
        long SID{2}, G{5}, CID{6};
        double F(2.9), N1(0.), N2(1.9), N3(0.);
        force probe;
        test << probe;
        test << probe(&SID, &G, &CID, &F, &N1, &N2, &N3);
        SID++;
        G++;
        CID++;
        F += 4.;
        test << probe(&SID, &G, &CID, &F, &N1, &N2, &N3);
        SID++;
        test << probe(&SID, &G, &CID, &F, &N1, &N2);
        SID++;
        test << probe(&SID, &G, &CID, &F, &N1);
        test << probe;
        CHECK(test.str() ==
              "FORCE          2       5       62.900+00 0.00+001.900+00 0.00+00\n"
              "FORCE          3       6       76.900+00 0.00+001.900+00 0.00+00\n"
              "FORCE          4       6       76.900+00 0.00+001.900+00\n"
              "FORCE          5       6       76.900+00 0.00+00\n"
              "FORCE          5       6       76.900+00 0.00+00\n");
    }

    SECTION("Exception, mkoe 2015-12-17") {
        long const lg{2}, nodeId{2}, zero(0);
        double const one(1.), fx(15.505163191247204),
            fy(-11.104650284500055), fz(94.254443646696117);

        std::stringstream s;
        s << dnvgl::extfem::bdf::cards::force(&lg, &nodeId, &zero, &one, &fx, &fy, &fz);
        CHECK(s.str() ==
              "FORCE*                 2               2               01.00000000000+00\n"
              "*       1.55051631912+01-1.1104650285+019.42544436467+01\n");
    }

    SECTION("Locale, mkoe 2016-01-07 [FEMIO-1]") {
        long const lg(2222), nodeId(22222), zero(0);
        double const one(1.), fx(1550.5163191247204),
            fy(-11104.650284500055), fz(942544.43646696117);

        std::stringstream s;
        s << dnvgl::extfem::bdf::cards::force(&lg, &nodeId, &zero, &one, &fx, &fy, &fz);
        CHECK(s.str() ==
              "FORCE*              2222           22222               01.00000000000+00\n"
              "*       1.55051631912+03-1.1104650285+049.42544436467+05\n");
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
