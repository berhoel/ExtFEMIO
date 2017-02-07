/**
   \file tests/test_bdf_cards_crod.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Testing the BDF `CROD` card class.

   Detailed description
   */

#include "extfem_misc.h"

// ID:
namespace {
    const char cID[] _EXTFEMIO_UNUSED =
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

using namespace dnvgl::extfem::bdf;
using namespace dnvgl::extfem::bdf::cards;

CATCH_TRANSLATE_EXCEPTION(errors::error& ex) {
    return ex.what();
}

CATCH_TRANSLATE_EXCEPTION(std::string& ex) {
    return ex;
}

TEST_CASE("BDF CROD definitions. (Small Field Format)", "[bdf_crod]") {

    std::list<std::string> data({"CROD,222,13,14,15\n"});
    std::list<std::string> lines;
    __base::card::card_split(data, lines);
    crod probe(lines);

    SECTION("first crod") {
        CHECK(probe.EID.value == 222);
        CHECK(probe.PID.value == 13);
        CHECK(probe.G1.value == 14);
        CHECK(probe.G2.value == 15);
    }
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j8&&make -C ../cbuild test"
// End:
