// Copyright © 2015 by DNV GL SE

// Purpose: Definitions for Nastran BDF MAT1 cards.

// Author Berthold Höllmann <berthold.hoellmann@dnvgl.com>

#include "stdafx.h"

// ID:
namespace {
  const char  cID[]
#ifdef __GNUC__
  __attribute__ ((__unused__))
#endif
    = "@(#) $Id$";
}

#include <deque>
#include <string>
#include <memory>

#include "bdf_cards.h"
#include "bdf_types.h"

using namespace ::std;
using namespace ::bdf;
using namespace ::bdf::cards;
using namespace ::bdf::types;
using namespace ::bdf::type_bounds;

bdf_int mat1::_MID(
  "MID", num<long>(make_unique<long>(1).get()));
bdf_float mat1::_E(
  "E", num<double>(
    make_unique<double>(0.).get(), nullptr, nullptr, true));
bdf_float mat1::_G(
  "G", num<double>(
    make_unique<double>(0.).get(), nullptr, nullptr, true));
bdf_float mat1::_NU(
  "NU", num<double>(make_unique<double>(-1.).get(),
                    make_unique<double>(.5).get(), nullptr, true));
bdf_float mat1::_RHO(
  "RHO", num<double>(nullptr, nullptr, nullptr, true));
bdf_float mat1::_A(
  "A", num<double>(nullptr, nullptr, nullptr, true));
bdf_float mat1::_TREF(
  "TREF", num<double>(nullptr, nullptr, nullptr, true));
bdf_float mat1::_GE(
  "GE", num<double>(nullptr, nullptr, nullptr, true));
bdf_float mat1::_ST(
  "ST",
  num<double>(make_unique<double>(0.).get(), nullptr, nullptr, true));
bdf_float mat1::_SC(
  "SC",
  num<double>(make_unique<double>(0.).get(), nullptr, nullptr, true));
bdf_float mat1::_SS(
  "SS",
  num<double>(make_unique<double>(0.).get(), nullptr, nullptr, true));
bdf_int mat1::_MCSID(
  "MCSID",
  num<long>(make_unique<long>(0).get(), nullptr, nullptr, true));

mat1::mat1(const deque<::std::string> &inp) : bdf_card(inp) {
  long *l_ptr;
  double *d_ptr;

  MCSID = nullptr;
  SS = nullptr;
  SC = nullptr;
  ST = nullptr;
  GE = nullptr;
  TREF = nullptr;
  A = nullptr;
  RHO = nullptr;
  NU = nullptr;
  G = nullptr;

  deque<::std::string>::const_reverse_iterator pos = inp.rbegin();

  switch (inp.size()-1) {
  case 16:
    ++pos;
  case 15:
    ++pos;
  case 14:
    ++pos;
  case 13:
    ++pos;
  case 12:
    l_ptr = _MCSID(*pos);
    MCSID = (l_ptr == nullptr) ? nullptr : make_unique<long>(*l_ptr);
    ++pos;
  case 11:
    d_ptr = _SS(*pos);
    SS = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 10:
    d_ptr = _SC(*pos);
    SC = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 9:
    d_ptr = _ST(*pos);
    ST = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 8:
    d_ptr = _GE(*pos);
    GE = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 7:
    d_ptr = _TREF(*pos);
    TREF = (d_ptr == nullptr) ? nullptr :  make_unique<double>(*d_ptr);
    ++pos;
  case 6:
    d_ptr = _A(*pos);
    A = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 5:
    d_ptr = _RHO(*pos);
    RHO = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 4:
    d_ptr = _NU(*pos);
    NU = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 3:
    d_ptr = _G(*pos);
    G = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
  case 2:
    d_ptr = _E(*pos);
    E = (d_ptr == nullptr) ? nullptr : make_unique<double>(*d_ptr);
    ++pos;
    MID = make_unique<long>(*_MID(*pos));
    break;
  default:
    throw "Illegal number of entries for MID1\n";
  }

  // remark 2
  if (!E && !G)
    throw bdf_parse_error("Either G or E has to be given in MAT1.");
  if (!NU) {
    if (!E) {
      NU = make_unique<double>(0.);
      E = make_unique<double>(0.);
    } else if (!G) {
      NU = make_unique<double>(0.);
      G = make_unique<double>(0.);
    } else
      NU = make_unique<double>((*E / 2. / *G) - 1.);
  } else if (!E)
    E = make_unique<double>((2. * (1 + *NU) * *G));
  else if (!G)
    G = make_unique<double>(*E / (2. * (1 + *NU)));
}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C .. check -j 8"
// End: