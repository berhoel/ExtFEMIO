/**
   \file fem/fem_element_ilst.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief FEM element definition for ilst.

   Plane Lin. Strain Triangle
*/

// ID:
namespace {
   const char  cID[]
#ifdef __GNUC__
   __attribute__ ((__unused__))
#endif
      = "@(#) $Id$";
}

#include "fem/elements.h"

using namespace ::dnvgl::extfem::fem::elements;

const long ilst::nnodes = 6;

el_types ilst::get_type() const {return ILST;}

namespace {
   const size_t procs_len = 3;
   el_processor procs[procs_len] = {general, Prefem, Sestra};
}
const ::std::set<el_processor> ilst::processors(procs, procs+procs_len);

ilst::ilst(const ::dnvgl::extfem::fem::cards::gelmnt1 &data) :
   ::dnvgl::extfem::fem::elements::__base(data) {}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../.. check -j8"
// End: