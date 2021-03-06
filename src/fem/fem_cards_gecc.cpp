/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2017 by DNV GL SE
   \brief Processing Sesam FEM `GECC` cards.

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_fem_cards_gecc[]) =
        "@(#) $Id$";
}

#include <cassert>

#include "fem/cards.h"
#include "fem/types.h"

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

using namespace dnvgl::extfem;
using namespace fem;
using namespace types;
using namespace cards;

const card gecc::head("GECC");


// const entry_type<long> gecc::_form_ECCNO("ECCNO");
const entry_type<long> gecc::_form_IOPT("IOPT");
const entry_type<double> gecc::_form_EX("EX");
const entry_type<double> gecc::_form_EY("EY");
const entry_type<double> gecc::_form_EZ("EZ");

gecc::gecc() : gecc(-1, ecc_opt::UNDEF_TYPE, 0., 0., 0.) {}

gecc::gecc(const vector<std::string> &inp, size_t const len) {
    gecc::read(inp, len);
}

void gecc::read(const vector<std::string> &inp, size_t const len) {
    eccno::read(inp, len);
    if (len < 4)
        throw errors::parse_error(
            "GECC", "Illegal number of entries.");

    // ECCNO = _form_ECCNO(inp.at(1));
    long const tmp = _form_IOPT(inp.at(2));
    switch (tmp) {
    case 1: IOPT = ecc_opt::Z_ONLY; break;
    case 2: IOPT = ecc_opt::XYZ; break;
    default: IOPT = ecc_opt::UNDEF_TYPE; break;
    }
    if (IOPT == ecc_opt::Z_ONLY) {
        EX = {0.};
        EY = {0};
        EZ = _form_EZ(inp.at(3));
    } else if (IOPT == ecc_opt::XYZ) {
        if (len < 6)
            throw errors::parse_error(
                "GECC", "Illegal number of entries.");
        EX = _form_EX(inp.at(3));
        EY = _form_EY(inp.at(4));
        EZ = _form_EZ(inp.at(5));
    }
}

gecc::gecc(
    long const ECCNO, ecc_opt IOPT, 
    double const EX, double const EY, double const EZ) :
    eccno(ECCNO), IOPT(IOPT), EX(EX), EY(EY), EZ(EZ) {}

cards::__base::card const& gecc::operator()(
    long const ECCNO, ecc_opt IOPT,
    double const EX, double const EY, double const EZ) {
    this->ECCNO = ECCNO;
    this->IOPT = IOPT;
    this->EX = EX;
    this->EY = EY;
    this->EZ = EZ;
    return *this;
}

ostream &gecc::put(ostream& os) const {
    if (ECCNO == -1) return os;
    os << head.format()
       << _form_ECCNO.format(ECCNO)
       << _form_IOPT.format(static_cast<long>(IOPT));
    if (IOPT == ecc_opt::Z_ONLY) {
        os << _form_EZ.format(EZ);
    } else if (IOPT == ecc_opt::XYZ)
        os << _form_EX.format(EX) << _form_EY.format(EY) << endl
           << fem::types::card().format() << _form_EZ.format(EZ);
    return os << endl;
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&
//    (make -C ../../cbuild test;
//     ../../cbuild/tests/test_fem_cards --use-colour no)"
// End:
