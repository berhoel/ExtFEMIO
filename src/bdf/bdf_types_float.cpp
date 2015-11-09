// Copyright © 2015 by DNV GL SE

// Definitions for Nastran Bulk data entry types.

// Author    Berthold Höllmann <berthold.hoellmann@dnvgl.com>

#include "StdAfx.h"

// ID:
namespace {
  const char  cID[]
#ifdef __GNUC__
  __attribute__ ((__unused__))
#endif
    = "@(#) $Id$";
}

#include <sstream>
#include <cmath>

#ifdef __GNUC__
#include "config.h"
#endif

#ifdef _MSC_VER
#include <stdio.h>
#endif

#ifdef HAVE_BOOST_REGEX_HPP
#include <boost/regex.hpp>
#else
#include <regex>
#endif

#include "bdf/types.h"
#include "bdf/errors.h"

using namespace ::std;
using namespace ::dnvgl::extfem;
using namespace ::dnvgl::extfem::bdf::types;

entry_type<double>::entry_type(::std::string name) :
  bdf::types::base(name), bounds(bdf::type_bounds::bound<double>()) {};

entry_type<double>::entry_type(::std::string name, bdf::type_bounds::bound<double> bounds) :
  bdf::types::base(name), bounds(bounds) {};

const
#ifdef HAVE_BOOST_REGEX_HPP
::boost::regex
#else
::std::regex
#endif
entry_type<double>::float_exp_re(
  "([\\+-]?[.0-9]+)([+-][0-9]+)",
#ifdef HAVE_BOOST_REGEX_HPP
  ::boost::regex_constants::ECMAScript);
#else
  ::std::regex_constants::ECMAScript);
#endif

const
#ifdef HAVE_BOOST_REGEX_HPP
::boost::regex
#else
::std::regex
#endif
entry_type<double>::float_re(
  "([\\+-]?((0|([1-9][0-9]*))?[.][0-9]*)|"
  "[.][0-9]+)(((E[+-]?)|[+-])[0-9]+)?",
#ifdef HAVE_BOOST_REGEX_HPP
  ::boost::regex_constants::ECMAScript);
#else
  ::std::regex_constants::ECMAScript);
#endif

const
#ifdef HAVE_BOOST_REGEX_HPP
::boost::regex
#else
::std::regex
#endif
entry_type<double>::float_lead_dot(
  "^[\\+-]?[.][0-9]+",
#ifdef HAVE_BOOST_REGEX_HPP
  ::boost::regex_constants::ECMAScript);
#else
  ::std::regex_constants::ECMAScript);
#endif

// Convert string to float
double *entry_type<double>::operator() (const ::std::string &inp) const {
  auto *value = new double();
  auto sval = extfem::string::string(inp).trim().upper();

  if (sval.length() == 0) {
    if (this->bounds.does_allow_empty())
      return nullptr;
    if (!this->bounds.has_default())
      throw errors::float_error(name, "empty entry without default");
    *value = this->bounds.get_default();
  } else {
    if (! regex_match(sval, float_re)) {
      ::std::string msg("illegal input, no float");
      throw errors::float_error(name, msg + "; !" + sval + "!");
    }

#ifdef HAVE_BOOST_REGEX_HPP
    ::boost::smatch m;
#else
    ::std::smatch m;
#endif

    if (regex_search(sval, m, float_exp_re))
      sval = m[1].str() + "E" + m[2].str();

    if (regex_match(sval, float_lead_dot)) {
        auto pos = sval.find('.');
        sval.insert(pos, 1, '0');
    }

    istringstream conv(sval);
    conv.imbue(locale("C"));
    conv >> *value;
  }
  if (!this->bounds.in_bounds(value))
    throw errors::float_error(name, "boundary condition violated");
  return value;
}

::std::string entry_type<double>::format(const ::std::unique_ptr<double> &inp) const {

  if (!inp)
    return bdf::types::empty().format(nullptr);

  ::std::ostringstream res;

  res.setf(ios_base::scientific, ios::floatfield);
  res.fill(' ');

#ifdef _MSC_VER
  // Set output to two digit exponetial format.
  unsigned int ext_exp_format = _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

  switch (out_form) {
  case LONG:
    res.setf(ios_base::right, ios_base::adjustfield);
    res.precision(11);
    break;
  case SHORT:
    { // Check on how much precision is lost when using SHORT format.
      // If too much precision is list raise exception which causes
      // calling routine to switch to LONG format.
      double order(pow(10., -floor(::std::log10(fabs(*inp)))+3.));
      if (fabs(fabs(round(*inp*order)/(*inp*order)) - 1.) > 1e-8) {
        ::std::ostringstream msg("output string for value ",
                               ::std::ostringstream::ate);
        msg << *inp
            << " looses too much precision of being crammed into string of "
            << out_form << " characters.";
        throw errors::float_error(name, msg.str());
      }
    }
    res.setf(ios_base::right, ios_base::adjustfield);
    res.precision(3);
    res.width(9);
    res.fill(' ');
    break;
  case FREE:
    break;
  }
  res.width(out_form+1);

  res << *inp;
  ::std::string out(res.str());
  out.erase(out.find('e'), 1);
  if (out.size() != static_cast<size_t>(out_form) && out_form > 0) {
    ::std::ostringstream msg("output string for value ", ::std::ostringstream::ate);
    msg << *inp << " of incorrect size, got length of " << out.size()
        << " instead of allowed length of " << out_form << ".";
    throw errors::output_error(name, msg.str());
  }

#ifdef _MSC_VER
  // Reset exponetial format to former settings.
  _set_output_format(ext_exp_format);
#endif

  return out;
}

/*
  Local Variables:
  mode: c++
  ispell-local-dictionary: "english"
  c-file-style: "dnvgl"
  indent-tabs-mode: nil
  compile-command: "make -C ../.. check -j 8"
  coding: utf-8
  End:
*/
