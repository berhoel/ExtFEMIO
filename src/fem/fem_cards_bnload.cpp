/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Processing Sesam FEM `BNLOAD` cards.

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_fem_cards_bnload[]) =
        "@(#) $Id$";
}

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

using namespace dnvgl::extfem;
using namespace fem;
using namespace types;
using namespace cards;

const card bnload::head("BNLOAD");

const entry_type<long> bnload::_form_LLC("LLC");
const entry_type<long> bnload::_form_LOTYP("LOTYP");
const entry_type<bool> bnload::_form_COMPLX("COMPLX");
const entry_type<long> bnload::_form_NODENO("NODENO");
const entry_type<long> bnload::_form_NDOF("NDOF");
const entry_type<double> bnload::_form_RLOAD("RLOAD");
const entry_type<double> bnload::_form_ILOAD("ILOAD");

bnload::bnload(const std::vector<std::string> &inp, size_t const len) :
        card(types::BNLOAD) {
    bnload::read(inp, len);
}

void bnload::read(const std::vector<std::string> &inp, size_t const len) {

    if (len < 8)
        throw errors::parse_error(
            "BNLOAD", "Illegal number of entries.");

    LLC = _form_LLC(inp.at(1));
    LOTYP = _form_LOTYP(inp.at(2));
    COMPLX = _form_COMPLX(inp.at(3));
    NODENO = _form_NODENO(inp.at(5));
    NDOF = _form_NDOF(inp.at(6));
    for (size_t i{0}; i < static_cast<size_t>(NDOF); i++)
        RLOAD.push_back(_form_RLOAD(inp.at(7 + i)));
    if (COMPLX)
        for (size_t i{0}; i < static_cast<size_t>(NDOF); i++)
            ILOAD.push_back(_form_ILOAD(inp.at(7 + NDOF + i)));
}

bnload::bnload() :
        bnload(-1, 0, 0, {}) {}

bnload::bnload(long const LLC,
               long const LOTYP,
               bool const COMPLX,
               long const NODENO,
               long const NDOF,
               std::vector<double> const &RLOAD,
               std::vector<double> const &ILOAD) :
        card(types::BNLOAD), LLC(LLC), LOTYP(LOTYP), COMPLX(COMPLX),
        NODENO(NODENO), NDOF(NDOF),
        RLOAD(RLOAD), ILOAD(ILOAD) {}

bnload::bnload(long const LLC,
               long const LOTYP,
               bool const COMPLX,
               long const NODENO,
               std::vector<double> const &RLOAD,
               std::vector<double> const &ILOAD):
        card(types::BNLOAD), LLC(LLC), LOTYP(LOTYP), COMPLX(COMPLX),
        NODENO(NODENO), NDOF(long(RLOAD.size())),
        RLOAD(RLOAD), ILOAD(ILOAD) {}

bnload::bnload(long const LLC,
               long const LOTYP,
               long const NODENO,
               long const NDOF,
               std::vector<double> const &RLOAD,
               std::vector<double> const &ILOAD) :
        card(types::BNLOAD), LLC(LLC), LOTYP(LOTYP),
        COMPLX(ILOAD.size() > 0),
        NODENO(NODENO), NDOF(NDOF),
        RLOAD(RLOAD), ILOAD(ILOAD) {}

bnload::bnload(long const LLC,
               long const LOTYP,
               long const NODENO,
               std::vector<double> const &RLOAD,
               std::vector<double> const &ILOAD) :
        card(types::BNLOAD), LLC(LLC), LOTYP(LOTYP),
        COMPLX(ILOAD.size() > 0),
        NODENO(NODENO), NDOF(long(RLOAD.size())),
        RLOAD(RLOAD), ILOAD(ILOAD) {}

std::ostream &bnload::put(std::ostream& os) const {
    if (this->LLC == -1) return os;
    os << head.format()
       << this->_form_LLC.format(this->LLC)
       << this->_form_LOTYP.format(this->LOTYP)
       << this->_form_COMPLX.format(this->COMPLX)
       << this->empty.format() << std::endl
       << fem::types::card().format()
       << this->_form_NODENO.format(this->NODENO)
       << this->_form_NDOF.format(this->NDOF);
    long cnt = 2;
    for (long i = 0; i < this->NDOF; i++) {
        if (!(cnt++ % 4))
            os << std::endl << fem::types::card().format();
        os << this->_form_RLOAD.format(this->RLOAD.at(i));
    }
    if (this->COMPLX)
        for (long i = 0; i < this->NDOF; i++) {
            if (!(cnt++ % 4))
                os << std::endl << fem::types::card().format();
            os << this->_form_ILOAD.format(this->ILOAD.at(i));
        }
    os << std::endl;
    return os;
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
