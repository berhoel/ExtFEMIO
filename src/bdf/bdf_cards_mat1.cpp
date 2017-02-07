/**
   \file bdf/bdf_cards_mat1.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Definitions for Nastran BDF MAT1 cards.

   Detailed description
   */
#include "StdAfx.h"

// ID:
namespace {
    const char cID_bdf_cards_mat1[] _EXTFEMIO_UNUSED =
        "@(#) $Id$";
}

#include <list>
#include <string>
#include <memory>

#include "bdf/cards.h"
#include "bdf/types.h"
#include "bdf/errors.h"

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
    static const long cl0 = 0, cl1 = 1;
    static const double cd0 = 0., cd05 = 0.5, cd_1 = -1.;
}

using namespace dnvgl::extfem::bdf;
using dnvgl::extfem::bdf::types::entry_type;
using namespace dnvgl::extfem::bdf::type_bounds;

using namespace dnvgl::extfem::bdf::cards;

const entry_type<double> mat1::form_E(
    "E", bound<double>(&cd0, nullptr, nullptr, true));
const entry_type<double> mat1::form_NU(
    "NU", bound<double>(&cd_1, &cd05, nullptr, true));

mat1::mat1(std::list<std::string> const &inp) :
__base::mat(inp) {
    this->read(inp);
}

void mat1::read(std::list<std::string> const &inp) {

    form_MCSID.set_value(MCSID, "");
    form_SS.set_value(SS, "");
    form_SC.set_value(SC, "");
    form_ST.set_value(ST, "");
    form_GE.set_value(GE, "");
    form_TREF.set_value(TREF, "");
    form_A.set_value(A, "");
    form_RHO.set_value(RHO, "");
    form_NU.set_value(NU, "");
    form_G.set_value(G, "");

    auto pos = inp.rbegin();

    switch (inp.size() - 1) {
    case 16:
        ++pos;
    case 15:
        ++pos;
    case 14:
        ++pos;
    case 13:
        ++pos;
    case 12:
        form_MCSID.set_value(MCSID, *(pos++));
    case 11:
        form_SS.set_value(SS, *(pos++));
    case 10:
        form_SC.set_value(SC, *(pos++));
    case 9:
        form_ST.set_value(ST, *(pos++));
    case 8:
        form_GE.set_value(GE, *(pos++));
    case 7:
        form_TREF.set_value(TREF, *(pos++));
    case 6:
        form_A.set_value(A, *(pos++));
    case 5:
        form_RHO.set_value(RHO, *(pos++));
    case 4:
        form_NU.set_value(NU, *(pos++));
    case 3:
        form_G.set_value(G, *(pos++));
    case 2:
        form_E.set_value(E, *(pos++));
        form_MID.set_value(MID, *(pos));
        break;
    default:
        throw errors::parse_error("MAT1", "Illegal number of entries.");
    }

    if ((bool)A && !(bool)TREF) form_TREF.set_value(TREF, "");

    // remark 2
    if (!((bool)E || (bool)G))
        throw errors::parse_error(
        "MAT1", "Either G or E has to be given.");
    if (!(bool)NU) {
        if (!(bool)E) {
            NU.value = 0.;
            NU.is_value = true;
            E.value = 0.;
            E.is_value = true;
        } else if (!(bool)G) {
            NU.value = 0.;
            NU.is_value = true;
            G.value = 0.;
            G.is_value = true;
        } else {
            NU.value = ((double)E / 2. / (double)G) - 1.;
            NU.is_value = true;
        }
    } else if (!(bool)E) {
        E.value = 2. * (1 + (double)NU) * (double)G;
        E.is_value = true;
    } else if (!(bool)G) {
        G.value = (double)E / (2. * (1 + (double)NU));
        G.is_value = true;
    }
}

const dnvgl::extfem::bdf::cards::types
mat1::card_type(void) const {
    return types::MAT1;
};

void mat1::collect_outdata(
    std::list<std::unique_ptr<format_entry> > &res) const {
    throw errors::error("MAT1", "can't write MAT1.");
    return;
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
