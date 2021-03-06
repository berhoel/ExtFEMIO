/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Implementation for CASE SUBCASE

   Detailed description
   */
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_bdf_header_case_control_subcase[]) =
        "@(#) $Id$";
}

#include "bdf/header.h"

#if defined(_DEBUG) && defined(DEBUG_NEW)
#define new DEBUG_NEW
#ifdef THIS_FILE
#undef THIS_FILE
#endif // THIS_FILE
namespace {
    char const THIS_FILE[] = __FILE__;
}
#endif

using namespace dnvgl::extfem::bdf::header::case_control;

long subcase::max_n = 0;

subcase::subcase(long const n) : n(n) {
    max_n = std::max(n, max_n);
}

subcase::subcase() : n(++max_n) {}

std::ostream &subcase::put(std::ostream &os) const {
    os.imbue(std::locale::classic());
    return os << "SUBCASE = " << this->n << std::endl;
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
