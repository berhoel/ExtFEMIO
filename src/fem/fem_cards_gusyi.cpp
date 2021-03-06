/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Processing Sesam FEM `GUSYI` cards.

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_fem_cards_gusyi[]) =
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

using namespace std;

using namespace dnvgl::extfem;
using namespace fem;
using namespace types;

using namespace cards;

card const gusyi::head("GUSYI");

entry_type<double> const gusyi::_form_HZ("HZ");
entry_type<double> const gusyi::_form_TY("TY");
entry_type<double> const gusyi::_form_BT("BT");
entry_type<double> const gusyi::_form_B1("B1");
entry_type<double> const gusyi::_form_TT("TT");
entry_type<double> const gusyi::_form_BB("BB");
entry_type<double> const gusyi::_form_B2("B2");
entry_type<double> const gusyi::_form_TB("TB");
entry_type<double> const gusyi::_form_SFY("SFY");
entry_type<double> const gusyi::_form_SFZ("SFZ");
entry_type<long> const gusyi::_form_NLOBYT("NLOBYT");
entry_type<long> const gusyi::_form_NLOBYB("NLOBYB");
entry_type<long> const gusyi::_form_NLOBZ("NLOBZ");

gusyi::gusyi(const vector<std::string> &inp, size_t const len) :
        beam_prop(inp, len, false) {
    __base::card::this_type = types::GUSYI;
    gusyi::read(inp, len);
}

gusyi::gusyi(long const GEONO,
             double const HZ, double const TY, double const BT,
             double const B1, double const TT, double const BB,
             double const B2, double const TB,
             double const SFY, double const SFZ) :
        gusyi(GEONO, HZ, TY, BT, B1, TT, BB, B2, TB,
              SFY, SFZ, 0, 0, 0) {}

gusyi::gusyi(long const GEONO,
             double const HZ, double const TY, double const BT,
             double const B1, double const TT, double const BB,
             double const B2, double const TB) :
        gusyi(GEONO, HZ, TY, BT, B1, TT, BB, B2, TB,
              1., 1., 0, 0, 0) {}

void gusyi::read(const vector<std::string> &inp, size_t const len) {
    std::string static const empty{"                "};
    if (len < 12)
        throw errors::parse_error(
            "GUSYI", "Illegal number of entries.");

    HZ = _form_HZ(inp.at(2));
    TY = _form_TY(inp.at(3));
    BT = _form_BT(inp.at(4));
    B1 = _form_B1(inp.at(5));
    TT = _form_TT(inp.at(6));
    BB = _form_BB(inp.at(7));
    B2 = _form_B2(inp.at(8));
    TB = _form_TB(inp.at(9));
    SFY = _form_SFY(inp.at(10));
    SFZ = _form_SFZ(inp.at(11));
    if (len >= 13 && inp.at(12) != empty)
        NLOBYT = _form_NLOBYT(inp[12]);
    else
        NLOBYT = {0};
    if (len >= 14 && inp.at(13) != empty)
        NLOBYB = _form_NLOBYB(inp[13]);
    else
        NLOBYB = {0};
    if (len >= 15 && inp.at(14) != empty)
        NLOBZ = _form_NLOBZ(inp[14]);
    else
        NLOBZ = {0};
}

gusyi::gusyi() : gusyi(-1, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.) {}

gusyi::gusyi(
    long const GEONO,
    double const HZ, double const TY,
    double const BT, double const B1, double const TT,
    double const BB, double const B2, double const TB,
    double const SFY, double const SFZ,
    long const NLOBYT, long const NLOBYB, long const NLOBZ) :
        beam_prop(GEONO, false),
        HZ(HZ), TY(TY),
        BT(BT), B1(B1), TT(TT),
        BB(BB), B2(B2),TB(TB),
        SFY(SFY), SFZ(SFZ),
        NLOBYT(NLOBYT), NLOBYB(NLOBYB), NLOBZ(NLOBZ) {
    __base::card::this_type = types::GUSYI;
}

cards::__base::card const &gusyi::operator() (
    long const GEONO,
    double const HZ, double const TY,
    double const BT, double const B1, double const TT,
    double const BB, double const B2, double const TB,
    double const SFY, double const SFZ,
    long const NLOBYT, long const NLOBYB, long const NLOBZ) {
    set_geono(GEONO, false);
    this->HZ = HZ;
    this->TY = TY;
    this->BT = BT;
    this->B1 = B1;
    this->TT = TT;
    this->BB = BB;
    this->B2 = B2;
    this->TB = TB;
    this->SFY = SFY;
    this->SFZ = SFZ;
    this->NLOBYT = NLOBYT;
    this->NLOBYB = NLOBYB;
    this->NLOBZ = NLOBZ;
    return *this;
}

ostream &gusyi::put(ostream& os) const {
    if (GEONO == -1) return os;
    os << head.format()
       << _form_GEONO.format(GEONO) << _form_HZ.format(HZ)
       << _form_TY.format(TY) << _form_BT.format(BT) << endl
       << fem::types::card().format()
       << _form_B1.format(B1) << _form_TT.format(TT)
       << _form_BB.format(BB) << _form_B2.format(B2) << endl
       << fem::types::card().format()
       << _form_TB.format(TB) << _form_SFY.format(SFY)
       << _form_SFZ.format(SFZ);
    if ((NLOBYT || NLOBYB || NLOBZ))
        os << _form_NLOBYT.format(NLOBYT) << endl << fem::types::card().format()
           << _form_NLOBYB.format(NLOBYB)
           << _form_NLOBZ.format(NLOBZ);
    os << endl;
    return os;
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j7 &&
//   (make -C ../../cbuild test ;
//    ../../cbuild/tests/test_fem_cards --use-colour no)"
// End:
