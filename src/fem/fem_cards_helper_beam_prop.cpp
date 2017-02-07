/**
   \file fem_cards_helper_beam_prop.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2017 by DNV GL SE
   \brief Implementing fem::cards::__base::beam_prop class.

   Detailed description
*/

#include "StdAfx.h"

// ID:
namespace {
    char const cID_fem_cards_helper_beam_prop[]
#ifdef __GNUC__
    __attribute__ ((__unused__))
#endif
        = "@(#) $Id$";
}

#include "fem/cards.h"

using namespace std;

using namespace dnvgl::extfem;
using namespace dnvgl::extfem::fem;
using namespace dnvgl::extfem::fem::cards;

unordered_set<long> cards::__base::beam_prop::used_gbeamg;
unordered_set<long> cards::__base::beam_prop::used_cross_desc;

cards::__base::beam_prop::beam_prop(
    vector<std::string> const &inp, size_t const len) {
    read(inp, len);
}

cards::__base::beam_prop::beam_prop(
    vector<std::string> const &inp, size_t const len, bool const is_gbeamg) {
    read(inp, len, is_gbeamg);
}

cards::__base::beam_prop::beam_prop() :
        geoprop() {}

cards::__base::beam_prop::beam_prop(
    long const GEONO, bool const is_gbeamg/*=false*/) {
    set_geono(GEONO, is_gbeamg);
}

void cards::__base::beam_prop::reset_geono(void) {
    used_gbeamg.clear();
    used_cross_desc.clear();
}

void cards::__base::beam_prop::set_geono(
    long const GEONO/*=0*/, bool const is_gbeamg/*=false*/) {
    if (GEONO < 0) {
        return cards::__base::geoprop::set_geono(GEONO);
    }
    if (GEONO == 0) {
        cards::__base::geoprop::set_geono(GEONO);
        if (is_gbeamg)
            used_gbeamg.insert(GEONO);
        else
            used_cross_desc.insert(GEONO);
        return;
    }
    if (is_gbeamg) {
        if (used_cross_desc.count(GEONO)){
            this->GEONO = GEONO;
        } else {
            cards::__base::geoprop::set_geono(GEONO);
        }
        used_gbeamg.insert(GEONO);
    } else {
        if (used_gbeamg.count(GEONO)){
            this->GEONO = GEONO;
        } else {
            cards::__base::geoprop::set_geono(GEONO);
        }
        used_cross_desc.insert(GEONO);;
    }
}

void cards::__base::beam_prop::read(
    vector<std::string> const &inp, size_t const len, bool const is_gbeamg) {
    if (len < 2)
        throw errors::parse_error(
            "CARD", "Illegal number of entries.");

    set_geono(_form_GEONO(inp.at(1)), is_gbeamg);
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make test"
// End: