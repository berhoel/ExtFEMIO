/**
   \file tests/test_fem_bool.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Tests for SEASAM FEM bool types.

   Detailed description
   */

#include "extfem_misc.h"

// ID:
namespace {
    const char cID[] _EXTFEMIO_UNUSED =
        "@(#) $Id$";
}

#include <limits>

// This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include <sstream>

#include "config.h"

#include "fem/types.h"
#include "fem/errors.h"

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dnvgl::extfem;
using namespace dnvgl::extfem::fem;
using namespace dnvgl::extfem::fem::types;
using namespace dnvgl::extfem::fem::type_bounds;

TEST_CASE("FEM bool types parsing.", "[fem_types]") {

    entry_type<bool> probe("dummy");

    //        12345678901e3456
    SECTION("' 0.00000000E+000'") {
        CHECK_FALSE(probe(" 0.00000000E+000"));
    }

    //        12345678901e3456
    SECTION("' 0.00000000E+00 '") {
        CHECK_FALSE(probe(" 0.00000000E+00 "));
    }

    //        12345678901e3456
    SECTION("' 1.00000000E+000'") {
        CHECK(probe(" 1.00000000E+000"));
    }

    //        12345678901e3456
    SECTION("' 1.00000000E+00 '") {
        CHECK(probe(" 1.00000000E+00 "));
    }

    //        12345678901e3456
    SECTION("'+1.23000000E+02 '") {
        CHECK_THROWS(probe("+1.23000000E+02"));
    }

    //        12345678901e3456
    SECTION("' 0.000000000e+00'") {
        CHECK_FALSE(probe(" 0.000000000e+00"));
    }

    SECTION("Own output") {
        CHECK(probe("           +1.00 "));
        CHECK_FALSE(probe("           +0.00"));
    }

    SECTION("FEMIO-6") {
        CHECK_FALSE(probe("  0.00000000E+00"));
    }
}

TEST_CASE("FEM bool types output.", "[fem_types]") {

    entry_type<bool> obj("dummy");

    bool lval(true);

    SECTION("Output (true)") {
        CHECK(obj.format(lval).size() == 16);
        CHECK(obj.format(lval) == "           +1.00");
    }

    SECTION("Output (false)") {
        bool lval(false);
        CHECK(obj.format(lval).size() == 16);
        CHECK(obj.format(lval) == "           +0.00");
    }
}

// Local Variables:
// mode: c++
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j8&&
//    (make -C ../cbuild test;
//     ../cbuild/tests/test_fem_bool --use-colour no)"
// coding: utf-8
// End:
