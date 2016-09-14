/**
   \file bdf/bdf_cards_bar_prop.cpp
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Common definitions for Nastran BDF PBAR and PBARL cards.

   Detailed description
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

#include "bdf/cards.h"

#include <cstdlib>
#include <memory>

#if defined(__AFX_H__) && defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
   static const long cl1 = 1;
}

namespace dnvgl {
   namespace extfem {
      namespace bdf {

         using types::entry_type;
         using namespace type_bounds;

         namespace cards {
            namespace __base {
               const entry_type<long> bar_prop::form_PID(
                  "PID", bound<long>(&cl1));
               const entry_type<long> bar_prop::form_MID(
                  "MID", bound<long>(&cl1, nullptr, nullptr, true));
            }
         }
      }
   }
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
