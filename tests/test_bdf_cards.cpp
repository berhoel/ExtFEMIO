// Copyright © 2015 by DNV GL SE

// Task      Testing the BDF cards classes.

// Author    Berthold Höllmann <berthold.hoellmann@dnvgl.com>

// ID:
namespace {
  const char  cID[]
#ifdef __GNUC__
  __attribute__ ((__unused__))
#endif
    = "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numdric_limits"

#include <limits>

// This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_MAIN

#include <iostream>
#include <deque>

#include <catch.hpp>

#ifndef _MSC_VER
#include <config.h>
#endif
#include "bdf/cards.h"
#include "bdf/file.h"

CATCH_TRANSLATE_EXCEPTION( bdf_error& ex ) {
  return Catch::toString( ex() );
}

using namespace std;
using namespace bdf::cards;
using namespace bdf::input;

TEST_CASE("BDF file reader.", "[bdf_cards]" ) {

  ::std::string s(
    //  45678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
    "$Only a comment\n"
    "MAT1    1       2.305+6 80000.0 0.3     7.850-6\n"
    "MAT1    4       2.305+6 80000.0 0.3     7.850-6\n"
    "PBEAML  104010  4               L     \n"
    "           63.0   340.0    35.0    14.0\n"
    "PBEAM   4000001 3       1.046+4 9.369+7 1.694+6 6.856+6 1.316+6\n");
  istringstream ist(s);
  bdf_file probe(ist);
  deque<::std::string> l;
  deque<::std::string> ref;

  ref.push_back("MAT1    1       2.305+6 80000.0 0.3     7.850-6");
  l = probe.get();
  CAPTURE( l[0] );
  CHECK(l == ref);

  ref.clear();
  ref.push_back("MAT1    4       2.305+6 80000.0 0.3     7.850-6");
  l = probe.get();
  CAPTURE( l[0] );
  CHECK(l == ref);

  ref.clear();
  ref.push_back("PBEAML  104010  4               L     ");
  ref.push_back("           63.0   340.0    35.0    14.0");
  l = probe.get();
  CAPTURE( l[0] );
  CHECK(l == ref);

  ref.clear();
  ref.push_back("PBEAM   4000001 3       1.046+4 9.369+7 1.694+6 6.856+6 1.316+6");
  l = probe.get();
  CAPTURE( l[0] );
  CHECK(l == ref);
}

TEST_CASE("Split Free Field Cards", "[bdf_cards]") {

  // Test data as found in the BDF documentation.

  ::std::deque<string> data;

  SECTION("Sample 1") {
    data.empty();
    data.push_back("GRID,2,1.0,-2.0,3.0,,136");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("GRID");
    ref.push_back("2");
    ref.push_back("1.0");
    ref.push_back("-2.0");
    ref.push_back("3.0");
    ref.push_back("");
    ref.push_back("136");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 2") {
    data.empty();
    data.push_back("MATT9,1101,2 ,3 ,4 ,,,,8 ,+P101\n");
    data.push_back("+P101,9 ,,,,13\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("MATT9");
    ref.push_back("1101");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("13");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 3") {
    data.empty();
    data.push_back("GRID,100,,1.0,0.0,0.0,,456\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("GRID");
    ref.push_back("100");
    ref.push_back("");
    ref.push_back("1.0");
    ref.push_back("0.0");
    ref.push_back("0.0");
    ref.push_back("");
    ref.push_back("456");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 4") {
    data.empty();
    data.push_back("SPC1,100,12456,1,2,3,4,5,6,+SPC-A+SPC-A,7,8,9,10\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("SPC1");
    ref.push_back("100");
    ref.push_back("12456");
    ref.push_back("1");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("5");
    ref.push_back("6");
    ref.push_back("7");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("10");
    // TODO: possible allow the above line syntax.
    CHECK_FALSE(probe.size() == ref.size());
    // for (size_t i=0; i<probe.size(); ++i) {
    //   CAPTURE( i << ": !" << probe[i] << "!" << ref[i] << "!" );
    //   CHECK(probe[i] == ref[i]);
    // }
  }

  SECTION("Sample 5") {
    data.empty();
    data.push_back("SPC1,100,12456,1,2,3,4,5,6,7,8,9,10\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("SPC1");
    ref.push_back("100");
    ref.push_back("12456");
    ref.push_back("1");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("5");
    ref.push_back("6");
    ref.push_back("7");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("10");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 6") {
    data.empty();
    data.push_back("MATT9,1151,2 ,3 ,4 ,,,,8 \n");
    data.push_back(",9 ,,,,13\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("MATT9");
    ref.push_back("1151");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("13");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 7") {
    data.empty();
    data.push_back("MATT9,1151,2 ,3 ,4 ,,,,8 ,+\n");
    data.push_back("+,9 ,,,,13\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("MATT9");
    ref.push_back("1151");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("13");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 8") {
    data.empty();
    data.push_back("MATT9*,1302,2 ,,4 ,+\n");
    data.push_back("+,,,,,,13\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("MATT9");
    ref.push_back("1302");
    ref.push_back("2");
    ref.push_back("");
    ref.push_back("4");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("13");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 9") {
    data.empty();
    data.push_back("MATT9,1303,2 ,3 ,4 ,,,,8 ,+\n");
    data.push_back("*,9 ,,,,\n");
    data.push_back("*,13\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("MATT9");
    ref.push_back("1303");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("13");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 10") {
    data.empty();
    data.push_back("MATT9,1355,2 ,3 ,,5 ,,,8 ,+\n");
    data.push_back("*,,10,,,\n");
    data.push_back("*,17\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("MATT9");
    ref.push_back("1355");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("");
    ref.push_back("5");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("8");
    ref.push_back("");
    ref.push_back("10");
    ref.push_back("");
    ref.push_back("");
    ref.push_back("17");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }

  SECTION("Sample 11") {
    data.empty();
    data.push_back("CHEXA,200,200,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("CHEXA");
    ref.push_back("200");
    ref.push_back("200");
    ref.push_back("1");
    ref.push_back("2");
    ref.push_back("3");
    ref.push_back("4");
    ref.push_back("5");
    ref.push_back("6");
    ref.push_back("7");
    ref.push_back("8");
    ref.push_back("9");
    ref.push_back("10");
    ref.push_back("11");
    ref.push_back("12");
    ref.push_back("13");
    ref.push_back("14");
    ref.push_back("15");
    ref.push_back("16");
    ref.push_back("17");
    ref.push_back("18");
    ref.push_back("19");
    ref.push_back("20");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }
}

TEST_CASE("Split Small Field Cards", "[cards]") {

  // Test data as found in the BDF documentation.

  ::std::deque<string> data;

  SECTION("Sample 1") {
    data.empty();
    //              12345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678
    data.push_back("GRID           2             1.0    -2.0     3.0             136");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("GRID");
    ref.push_back("2");
    ref.push_back("");
    ref.push_back("1.0");
    ref.push_back("-2.0");
    ref.push_back("3.0");
    ref.push_back("");
    ref.push_back("136");
    ref.push_back("");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }
}
TEST_CASE("Split Large Field Cards", "[cards]") {

  // Test data as found in the BDF documentation.

  ::std::deque<string> data;

  SECTION("Sample 1") {
    data.empty();
    //              12345678|234567812345678|234567812345678|234567812345678|234567812345678|2345678
    data.push_back("GRID*                  2                             1.0            -2.0 *GRID10\n");
    data.push_back("*GRID10              3.0                             136\n");
    deque<::std::string> probe = card::card_split(data);
    deque<::std::string> ref;
    ref.push_back("GRID");
    ref.push_back("2");
    ref.push_back("");
    ref.push_back("1.0");
    ref.push_back("-2.0");
    ref.push_back("3.0");
    ref.push_back("");
    ref.push_back("136");
    ref.push_back("");
    CHECK(probe.size() == ref.size());
    for (size_t i=0; i<probe.size(); ++i) {
      CAPTURE( i );
      CHECK(probe[i] == ref[i]);
    }
  }
}
TEST_CASE("BDF_Dispatch", "[cards]") {

  ::std::string s(
    //  45678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
    "MAT1    1       2.305+6 80000.0 0.3     7.850-6\n"
    "MAT1    4       2.305+6 80000.0 0.3     7.850-6\n"
    "PBEAML  104010  4               L     \n"
    "           63.0   340.0    35.0    14.0\n"
    "PBEAM   4000001 3       1.046+4 9.369+7 1.694+6 6.856+6 1.316+6\n"
    "PROD    6000001 1       3000.00\n"
    "PSHELL  1       4         23.00 4               4\n"
    "GRID           1        111525. 18000.  21000.\n"
    "GRID          76        111522. 18002.  21002.\n"
    "GRID         153        111522. 18001.  21002.\n"
    "CQUAD4  1       1       16      200     141     17\n"
    //  45678|234567890123456|234567890123456|234567890123456|234567890123456|2
    "CQUAD4* 2               1               16              200             "
    "+\n"
    "+       140             15\n"
    "CTRIA3  2606    1       1066    1065    1133\n"
    "CBEAM   7869    104010  76      153     0.0     66.5206 997.785\n"
    "                        0.0     -22.617 -339.25 0.0     -22.617 -339.25\n"
    "CROD    11316   6000001 1028    2139\n"
    "CBEAM    465144 104010  76      153     1.       0.      0.\n"
    "$ Elements and Element Properties for region : pbeaml.59\n"
    "ENDDATA\n");

  istringstream ist(s);
  bdf_file probe(ist);
  deque<::std::string> l;
  deque<::std::string> ref;

  std::unique_ptr<bdf::cards::card> current;

  SECTION("Processing several cards.") {
    l = probe.get();
    CAPTURE( l[0] );
    INFO( "The line is " << l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    CHECK(current->card_type() == ::bdf::cards::MAT1);
    // 12345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
    //         MID     E       G       NU      RHO
    // MAT1    1       2.305+6 80000.0 0.3     7.850-6
    CHECK(*static_cast<mat1*>(current.get())->MID == 1);
    CHECK(*static_cast<mat1*>(current.get())->E == 2.305e6);
    CHECK(*static_cast<mat1*>(current.get())->G == 8e4);
    CHECK(*static_cast<mat1*>(current.get())->NU == .3);
    CHECK(*static_cast<mat1*>(current.get())->RHO == 7.85e-6);
    CHECK_FALSE(static_cast<mat1*>(current.get())->A);
    CHECK_FALSE(static_cast<mat1*>(current.get())->TREF);
    CHECK_FALSE(static_cast<mat1*>(current.get())->GE);
    CHECK_FALSE(static_cast<mat1*>(current.get())->ST);
    CHECK_FALSE(static_cast<mat1*>(current.get())->SC);
    CHECK_FALSE(static_cast<mat1*>(current.get())->SS);
    CHECK_FALSE(static_cast<mat1*>(current.get())->MCSID);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    CHECK(current->card_type() == ::bdf::cards::MAT1);
    // 12345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
    //         MID     E       G       NU      RHO
    // MAT1    4       2.305+6 80000.0 0.3     7.850-6
    CHECK(*static_cast<mat1*>(current.get())->MID == 4);
    CHECK(*static_cast<mat1*>(current.get())->E == 2.305e6);
    CHECK(*static_cast<mat1*>(current.get())->G == 8e4);
    CHECK(*static_cast<mat1*>(current.get())->NU == .3);
    CHECK(*static_cast<mat1*>(current.get())->RHO == 7.85e-6);
    CHECK_FALSE(static_cast<mat1*>(current.get())->A);
    CHECK_FALSE(static_cast<mat1*>(current.get())->TREF);
    CHECK_FALSE(static_cast<mat1*>(current.get())->GE);
    CHECK_FALSE(static_cast<mat1*>(current.get())->ST);
    CHECK_FALSE(static_cast<mat1*>(current.get())->SC);
    CHECK_FALSE(static_cast<mat1*>(current.get())->SS);
    CHECK_FALSE(static_cast<mat1*>(current.get())->MCSID);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    // PBEAML  104010  4               L
    //           63.0   340.0    35.0    14.0
    CHECK(current->card_type() == ::bdf::cards::PBEAML);
    CHECK(*static_cast<pbeaml*>(current.get())->PID == 104010);
    CHECK(*static_cast<pbeaml*>(current.get())->MID == 4);
    CHECK(*static_cast<pbeaml*>(current.get())->GROUP == "MSCBML0");
    CHECK(*static_cast<pbeaml*>(current.get())->TYPE == "L");
    CHECK(static_cast<pbeaml*>(current.get())->DIM.size() == 1);
    CHECK(static_cast<pbeaml*>(current.get())->DIM[0]->size() == 4);
    CHECK(*(*static_cast<pbeaml*>(current.get())->DIM[0])[0] == 63.);
    CHECK(*(*static_cast<pbeaml*>(current.get())->DIM[0])[1] == 340.);
    CHECK(*(*static_cast<pbeaml*>(current.get())->DIM[0])[2] == 35.);
    CHECK(*(*static_cast<pbeaml*>(current.get())->DIM[0])[3] == 14.);
    CHECK(static_cast<pbeaml*>(current.get())->NSM.size() == 1);
    CHECK(*static_cast<pbeaml*>(current.get())->NSM[0] == 0.);
    CHECK(static_cast<pbeaml*>(current.get())->SO.size() == 0);
    CHECK(static_cast<pbeaml*>(current.get())->X_XB.size() == 0);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    // PBEAM   4000001 3       1.046+4 9.369+7 1.694+6 6.856+6 1.316+6
    CHECK(current->card_type() == ::bdf::cards::PBEAM);
    CHECK(*static_cast<pbeam*>(current.get())->PID == 4000001);
    CHECK(*static_cast<pbeam*>(current.get())->MID == 3);
    CHECK(static_cast<pbeam*>(current.get())->A.size() == 1);
    CHECK(*static_cast<pbeam*>(current.get())->A[0] == 10460.);
    CHECK(static_cast<pbeam*>(current.get())->I1.size() == 1);
    CHECK(*static_cast<pbeam*>(current.get())->I1[0] == 93690000.);
    CHECK(static_cast<pbeam*>(current.get())->I2.size() == 1);
    CHECK(*static_cast<pbeam*>(current.get())->I2[0] == 1694000.);
    CHECK(static_cast<pbeam*>(current.get())->I12.size() == 1);
    CHECK(*static_cast<pbeam*>(current.get())->I12[0] == 6.856e6);
    CHECK(static_cast<pbeam*>(current.get())->J.size() == 1);
    CHECK(*static_cast<pbeam*>(current.get())->J[0] == 1.316e6);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    // PROD    6000001 1       3000.00\n"
    CHECK(current->card_type() == ::bdf::cards::PROD);
    CHECK(*static_cast<prod*>(current.get())->PID == 6000001);
    CHECK(*static_cast<prod*>(current.get())->MID == 1);
    CHECK(*static_cast<prod*>(current.get())->A == 3000);
    CHECK_FALSE(static_cast<prod*>(current.get())->J);
    CHECK(*static_cast<prod*>(current.get())->C == 0.);
    CHECK_FALSE(static_cast<prod*>(current.get())->NSM);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    CHECK(current->card_type() == ::bdf::cards::PSHELL);
    // 12345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2345678|2
    //         PID     MID1    T       MID2    12I/T**3 MID3
    // PSHELL  1       4         23.00 4               4
    CHECK(*static_cast<pshell*>(current.get())->PID == 1);
    CHECK(*static_cast<pshell*>(current.get())->MID1 == 4);
    CHECK(*static_cast<pshell*>(current.get())->T == 23.);
    CHECK(*static_cast<pshell*>(current.get())->MID2 == 4);
    CHECK(*static_cast<pshell*>(current.get())->x12I_T__3 == 1.);
    CHECK(*static_cast<pshell*>(current.get())->MID3 == 4);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    CHECK(current->card_type() == ::bdf::cards::GRID);
    CHECK(*static_cast<grid*>(current.get())->ID == 1);
    CHECK(*static_cast<grid*>(current.get())->X1 == 111525.);
    CHECK(*static_cast<grid*>(current.get())->X2 == 18000.);
    CHECK(*static_cast<grid*>(current.get())->X3 == 21000.);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    CHECK(current->card_type() == ::bdf::cards::GRID);
    CHECK(*static_cast<grid*>(current.get())->ID == 76);
    CHECK(*static_cast<grid*>(current.get())->X1 == 111522.);
    CHECK(*static_cast<grid*>(current.get())->X2 == 18002.);
    CHECK(*static_cast<grid*>(current.get())->X3 == 21002.);

    l = probe.get();
    CAPTURE( l[0] );
    current = ::bdf::cards::dispatch(card::card_split(l));
    CHECK(current->card_type() == ::bdf::cards::GRID);
    CHECK(*static_cast<grid*>(current.get())->ID == 153);
    CHECK(*static_cast<grid*>(current.get())->X1 == 111522.);
    CHECK(*static_cast<grid*>(current.get())->X2 == 18001.);
    CHECK(*static_cast<grid*>(current.get())->X3 == 21002.);
  }
}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// c-file-style: "gl"
// indent-tabs-mode: nil
// compile-command: "make -C .. check -j 8"
// coding: utf-8
// End: