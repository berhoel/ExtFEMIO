/**
   \file fem/fem_element_ghex127.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief FEM element definition for ghex127.

   General Hexahedron, define with nodes 1 to 20, node 21, node 22, node 24, node 25 and node 27 present.

   Position of node in node array for element node numbers > 20:

        - node 21 at pos 21
        - node 22 at pos 22
        - node 24 at pos 23
        - node 25 at pos 24
        - node 27 at pos 25
*/

#include "StdAfx.h"

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

const long ghex127::nnodes = 25;

el_types ghex127::get_type() const {return GHEX127;}

namespace {
   const size_t procs_len = 2;
   el_processor procs[
      procs_len] = {general, Sestra};
}
const ::std::set<el_processor> ghex127::processors(procs, procs+procs_len);

ghex127::ghex127(const ::dnvgl::extfem::fem::cards::gelmnt1 *data) :
   ::dnvgl::extfem::fem::elements::ghex(data) {}

ghex127::ghex127(const ::dnvgl::extfem::fem::cards::gelref1 *data) :
   ::dnvgl::extfem::fem::elements::ghex(data) {}

ghex127::ghex127(const __base *data) :
   ghex(data) {}

el_types ghex127::get_type(void) const {
   return GHEX127;
}

// Local Variables:
// mode: c++
// ispell-local-dictionary: "english"
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../.. check -j8"
// End:
