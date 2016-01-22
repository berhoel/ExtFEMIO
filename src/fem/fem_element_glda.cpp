/**
   \file fem/fem_element_glda.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief FEM element definition for glda.

   2-Noded Damping-Matrix
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

const long glda::nnodes = 2;

el_types glda::get_type() const {return GLDA;}

namespace {
   const size_t procs_len = 1;
   el_processor procs[procs_len] = {general};
}
const ::std::set<el_processor> glda::processors(procs, procs+procs_len);

glda::glda(const ::dnvgl::extfem::fem::cards::gelmnt1 &data) :
   ::dnvgl::extfem::fem::elements::__base(data) {}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../.. check -j8"
// End: