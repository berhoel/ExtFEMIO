/**
   \file tests/test_fem_cards_tdsetnam.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Testing input and output for Sesam FEM `TDSETNAM` cards.

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

using namespace ::std;
using namespace ::dnvgl::extfem::fem;
using namespace ::dnvgl::extfem::fem::cards;

CATCH_TRANSLATE_EXCEPTION( errors::error& ex ) {
   return ex();
}

CATCH_TRANSLATE_EXCEPTION( ::std::string& ex ) {
   return ex;
}

TEST_CASE("FEM TDSETNAM definitions.", "[fem_tdsetnam]" ) {

   SECTION("TDSETNAM (1)") {
      ::std::deque<string> data;
      data.push_back(
         // 345678|234567890123456|234567890123456|234567890123456|234567890123456
         "TDSETNAM 4.00000000e+000 1.80000000e+002 1.21000000e+002 0.00000000e+000\n");
      data.push_back(
         "        PLAN_No6_STR(5445A/B)\n");

      ::std::deque<string> lines = card::card_split(data);
      tdsetnam probe(lines);

      CHECK(probe.NFIELD == 4);
      CHECK(probe.ISREF == 180);
      CHECK(probe.CODNAM == 121);
      CHECK(probe.CODTXT == 0);
      CHECK(probe.SET_NAME == "PLAN_No6_STR(5445A/B)");
      CHECK(probe.CONT.size() == 0);
   }

   SECTION("TDSETNAM (2)") {
      ::std::deque<string> data;
      data.push_back(
         // 345678|234567890123456|234567890123456|234567890123456|234567890123456
         "TDSETNAM 4.00000000e+00  1.80000000e+02  1.21000000e+02  0.00000000e+00 \n");
      data.push_back(
         "        PLAN_No6_STR(5445A/B)\n");

      ::std::deque<string> lines = card::card_split(data);
      tdsetnam probe(lines);

      CHECK(probe.NFIELD == 4);
      CHECK(probe.ISREF == 180);
      CHECK(probe.CODNAM == 121);
      CHECK(probe.CODTXT == 0);
      CHECK(probe.SET_NAME == "PLAN_No6_STR(5445A/B)");
      CHECK(probe.CONT.size() == 0);
   }

   SECTION("TDSETNAM (2)") {
      ::std::deque<string> data;
      data.push_back(
         // 345678|234567890123456|234567890123456|234567890123456|234567890123456
         "TDSETNAM 4.00000000e+00  1.80000000e+02  1.21000000e+02  2.64000000e+02 \n");
      data.push_back(
         "        PLAN_No6_STR(5445A/B)\n");
      data.push_back(
         "        Meaningles comment.\n");
      data.push_back(
         "        abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNUPQRSTUVWXYZ1234567890#+\n");

      ::std::deque<string> lines = card::card_split(data);
      tdsetnam probe(lines);

      CHECK(probe.NFIELD == 4);
      CHECK(probe.ISREF == 180);
      CHECK(probe.CODNAM == 121);
      CHECK(probe.CODTXT == 264);
      CHECK(probe.SET_NAME == "PLAN_No6_STR(5445A/B)");
      CHECK(probe.CONT.size() == 2);
      CHECK(probe.CONT[0] == "Meaningles comment.                                             ");
      //                      1234567891123456789212345678931234567894123456789512345678961234
      CHECK(probe.CONT[1] == "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNUPQRSTUVWXYZ1234567890#+");
   }
}

TEST_CASE("FEM TDSETNAM types output.", "[fem_tdsetnam,out]" ) {

   std::ostringstream test;

   SECTION("simple") {
      tdsetnam probe(4, 123, 122, "1234567890123456789012");
      test << probe;
      CHECK(test.str() ==
            "TDSETNAM+4.00000000e+00 +1.23000000e+02 +1.22000000e+02 +0.00000000e+00 \n"
            "        1234567890123456789012\n");
   }

   SECTION("calc internal values") {
      tdsetnam probe(123, "1234567890123456789012");
      test << probe;
      CHECK(test.str() ==
            "TDSETNAM+4.00000000e+00 +1.23000000e+02 +1.22000000e+02 +0.00000000e+00 \n"
            "        1234567890123456789012\n");
   }

   SECTION("with comment") {
      ::std::deque<::std::string> comments(2);
      comments[0] = "test";
      comments[1] = "123456789112345678921234567893123";
      tdsetnam probe(4, 123, 122, 233, "1234567890123456789012", comments);
      test << probe;
      CHECK(test.str() ==
            "TDSETNAM+4.00000000e+00 +1.23000000e+02 +1.22000000e+02 +2.33000000e+02 \n"
            "        1234567890123456789012\n"
            "        test                             \n"
            "        123456789112345678921234567893123\n");
   }

   SECTION("with comment (calc internal values)") {
      ::std::deque<::std::string> comments(2);
      comments[0] = "test";
      comments[1] = "123456789112345678921234567893123";
      tdsetnam probe(123, "1234567890123456789012", comments);
      test << probe;
      CHECK(test.str() ==
            "TDSETNAM+4.00000000e+00 +1.23000000e+02 +1.22000000e+02 +2.33000000e+02 \n"
            "        1234567890123456789012\n"
            "        test                             \n"
            "        123456789112345678921234567893123\n");
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
