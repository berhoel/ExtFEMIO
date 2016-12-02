/**
   \file fem/fem_cards_misosel.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Processing Sesam FEM `MISOSEL` cards.

   Detailed description
*/
#include "StdAfx.h"

// ID:
namespace {
   const char cID_fem_cards_misosel[]
#ifdef __GNUC__
   __attribute__ ((__unused__))
#endif
      = "@(#) $Id$";
}

#include <memory>
#include <algorithm>

#include "fem/cards.h"
#include "fem/types.h"

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dnvgl::extfem;
using namespace dnvgl::extfem::fem;
using namespace dnvgl::extfem::fem::types;

using namespace dnvgl::extfem::fem::cards;

const fem::types::card misosel::head("MISOSEL");

// const entry_type<long> misosel::_form_MATNO("MATNO");
const entry_type<double> misosel::_form_YOUNG("YOUNG");
const entry_type<double> misosel::_form_POISS("POISS");
const entry_type<double> misosel::_form_RHO("RHO");
const entry_type<double> misosel::_form_DAMP("DAMP");
const entry_type<double> misosel::_form_ALPHA("ALPHA");
const entry_type<double> misosel::_form_DUMMY("DUMMY");
const entry_type<double> misosel::_form_YIELD("YIELD");

misosel::misosel(const std::list<std::string> &inp) :
        __base::material(inp), DUMMY(0.), YIELD(0.) {

    if (inp.size() < 7)
        throw errors::parse_error(
            "MISOSEL", "Illegal number of entries.");

    auto pos = inp.begin();

    ++pos;
    MATNO = _form_MATNO(*(pos++));
    YOUNG = _form_YOUNG(*(pos++));
    POISS = _form_POISS(*(pos++));
    RHO = _form_RHO(*(pos++));
    DAMP = _form_DAMP(*(pos++));
    ALPHA = _form_ALPHA(*(pos++));
    if (pos == inp.end()) return;
    if (*pos != "                ")
        DUMMY = _form_DUMMY(*(pos++));
    else
        pos++;
    if (pos == inp.end()) return;
    if (*pos != "                ")
        YIELD = _form_YIELD(*pos);
}

misosel::misosel(void) :
        misosel(-1, 0., 0., 0., 0., 0.) {}

misosel::misosel(const long &MATNO,
                 const double &YOUNG,
                 const double &POISS,
                 const double &RHO,
                 const double &DAMP,
                 const double &ALPHA,
                 const double &DUMMY/*=0.*/,
                 const double &YIELD/*=0.*/) :
        __base::material(MATNO), YOUNG(YOUNG), POISS(POISS),
        RHO(RHO), DAMP(DAMP), ALPHA(ALPHA), DUMMY(DUMMY),
        YIELD(YIELD) {}

cards::__base::card const &misosel::operator() (
    long const &MATNO,
    double const &YOUNG,
    double const &POISS,
    double const &RHO,
    double const &DAMP,
    double const &ALPHA,
    double const &DUMMY/*=0.*/,
    double const &YIELD/*=0.*/) {
    this->MATNO = MATNO;
    this->YOUNG = YOUNG;
    this->POISS = POISS;
    this->RHO = RHO;
    this->DAMP = DAMP;
    this->ALPHA = ALPHA;
    this->DUMMY = DUMMY;
    this->YIELD = YIELD;
    return *this;
}

const dnvgl::extfem::fem::cards::types
misosel::card_type(void) const {return types::MISOSEL;}

std::ostream &misosel::put(std::ostream& os) const {
    if (this->MATNO == -1) return os;
    os << misosel::head.format()
       << this->_form_MATNO.format(this->MATNO)
       << this->_form_YOUNG.format(this->YOUNG)
       << this->_form_POISS.format(this->POISS)
       << this->_form_RHO.format(this->RHO)
       << std::endl
       << dnvgl::extfem::fem::types::card().format()
       << this->_form_DAMP.format(this->DAMP)
       << this->_form_ALPHA.format(this->ALPHA);
    if ((this->DUMMY || this->YIELD))
        os << this->_form_DUMMY.format(this->DUMMY)
           << this->_form_YIELD.format(this->YIELD);
    os << std::endl;
    return os;
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
