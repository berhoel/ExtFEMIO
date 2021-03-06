/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Test of BDF SPCFORCES case control entries.

   Detailed description
   */
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_test_bdf_header_spcforces[]) =
        "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <catch.hpp>

#ifdef __GNUC__
#include "config.h"
#endif

#include "bdf/header.h"

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

using namespace dnvgl::extfem::bdf;
using namespace dnvgl::extfem::bdf::header;

TEST_CASE("BDF generate 'SPCFORCES' header entries", "[bdf_header,spcforces]") {

    ostringstream test;

    SECTION("default") {
        case_control::spcforces const probe({});
        test << probe;
        CHECK(test.str() == "SPCFORCES = NONE\n");
    }

    SECTION("first") {
        case_control::spcforces const probe({}, case_control::spcforces::restype::ALL);
        test << probe;
        CHECK(test.str() == "SPCFORCES = ALL\n");
    }

    SECTION("NOZPRINT") {
        case_control::spcforces const probe({
                make_shared<case_control::spcforces::print>(),
                make_shared<case_control::spcforces::nozprint>()
        }, case_control::spcforces::restype::ALL);
        test << probe;
        CHECK(test.str() == "SPCFORCES(PRINT, NOZPRINT) = ALL\n");
    }

    SECTION("num") {
        case_control::spcforces const probe({}, 5);
        test << probe;
        CHECK(test.str() == "SPCFORCES = 5\n");
    }

    SECTION("IMAG") {
        case_control::spcforces const probe({
            make_shared<case_control::spcforces::sort2>(),
            make_shared<case_control::spcforces::punch>(),
            make_shared<case_control::spcforces::print>(),
            make_shared<case_control::spcforces::imag>()
        }, case_control::spcforces::restype::ALL);
        test << probe;
        CHECK(test.str() == "SPCFORCES(SORT2, PUNCH, PRINT, IMAG) = ALL\n");
    }

    SECTION("PHASE") {
        case_control::spcforces const probe({
            make_shared<case_control::spcforces::phase>()
        }, case_control::spcforces::restype::NONE);
        test << probe;
        CHECK(test.str() == "SPCFORCES(PHASE) = NONE\n");
    }

    SECTION("SORT2") {
        case_control::spcforces const probe({
            make_shared<case_control::spcforces::sort2>(),
            make_shared<case_control::spcforces::print>(),
            make_shared<case_control::spcforces::psdf>(),
            make_shared<case_control::spcforces::crms>(),
            make_shared<case_control::spcforces::rpunch>()
        }, 20);
        test << probe;
        CHECK(test.str() == "SPCFORCES(SORT2, PRINT, PSDF, CRMS, RPUNCH) = 20\n");
    }

    SECTION("PRINT") {
        case_control::spcforces const probe({
            make_shared<case_control::spcforces::print>(),
            make_shared<case_control::spcforces::rall>(),
            make_shared<case_control::spcforces::norprint>()
        }, case_control::spcforces::restype::ALL);
        test << probe;
        CHECK(test.str() == "SPCFORCES(PRINT, RALL, NORPRINT) = ALL\n");
    }

}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j7 &&
//   (make -C ../cbuild test ;
//    ../cbuild/tests/test_bdf_header --use-colour no)"
// End:
