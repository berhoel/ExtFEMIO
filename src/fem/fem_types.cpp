/*!
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Definitions for SESAM FEM data entry types.

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_fem_types[]) =
        "@(#) $Id$";
}

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

using namespace dnvgl::extfem::fem::types;

__base::b_type::b_type(const std::string &name) : name(name) {}

std::istringstream __base::b_type::conv;

__base::imbue_helper::imbue_helper(std::locale const &loc) : b_type("") {
    conv.imbue(loc);
}

fem_types __base::imbue_helper::type() const {
    return fem_types::None;
}

std::string __base::imbue_helper::format(void const*) {
    return "";
}

card::card(std::string const &name) : b_type(name) {}

card::card() : b_type("") {}

std::string card::format() const {
    std::ostringstream res;
    res.setf(std::ios_base::left, std::ios_base::adjustfield);
    res.fill(' ');
    res.width(8);
    res << name;
    return res.str();
}

empty::empty() : b_type("") {}

fem_types empty::type() const {return fem_types::None;}

std::string empty::format() {
    std::ostringstream res;
    res.fill(' ');
    res.width(16);
    res << "            0.00";
    return res.str();
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
