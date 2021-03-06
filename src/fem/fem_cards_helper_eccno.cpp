/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2017 by DNV GL SE
   \brief Implementing the fem::cards::__base::eccno class.

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    char const _EXTFEMIO_UNUSED(cID_fem_cards_helper_eccno[]) =
        "@(#) $Id$";
}

#include "fem/cards.h"

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
using namespace cards;

fem::types::entry_type<long> const
cards::__base::eccno::_form_ECCNO("ECCNO");

unordered_set<long> cards::__base::eccno::used_eccno;
long cards::__base::eccno::eccno_maxset = 0;

void cards::__base::eccno::set_eccno(long const ECCNO/*=0*/) {
    if (ECCNO < 0) {
        this->ECCNO = ECCNO;
        return;
    }
    if (ECCNO == 0) {
        for (long i = eccno_maxset + 1; i < numeric_limits<long>::max(); i++) {
            if (!used_eccno.count(i)) {
                eccno_maxset = i;
                used_eccno.insert(ECCNO);
                this->ECCNO = i;
                return;
            }
        }
    }
    if (used_eccno.count(ECCNO)) {
        ostringstream msg("ECCNO ", ostringstream::ate);
        msg << ECCNO << " value used twice.";
        throw errors::usage_error("ECCNO", msg.str());
    }
    used_eccno.insert(ECCNO);
    this->ECCNO = ECCNO;
}

cards::__base::eccno::eccno() : card(types::UNKNOWN), ECCNO(-1) {}

cards::__base::eccno::eccno(long const ECCNO) : card(types::UNKNOWN) {
    set_eccno(ECCNO);
}

cards::__base::eccno::eccno(
    vector<std::string> const &inp, size_t const len) : card(types::UNKNOWN) {
    eccno::read(inp, len);
}

void cards::__base::eccno::read(
    vector<std::string> const &inp, size_t const len) {
    if (len < 2)
        throw errors::parse_error(
            "CARD", "Illegal number of entries.");

    set_eccno(_form_ECCNO(inp.at(1)));
}

void cards::__base::eccno::reset_eccno() {
    eccno_maxset = 0;
    used_eccno.clear();
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
