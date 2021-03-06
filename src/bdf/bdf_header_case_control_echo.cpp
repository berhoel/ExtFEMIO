/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2016 by DNV GL SE
   \brief Implement output for CASE ECHO

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"


// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    const char _EXTFEMIO_UNUSED(cID_bdf_header_case_control_echo[]) =
        "@(#) $Id$";
}

#if (__GNUC__ && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9))) || \
    (_MSC_VER && _MSC_VER <= 1700)
#include "my_c++14.h"
#endif

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

using namespace std;

using namespace dnvgl::extfem::bdf::header::case_control;

echo::echo(vector<shared_ptr<describer>> const &_oper) {
    if (_oper.size() > 0)
        for (const auto &p : _oper)
            oper.push_back(p);
    else
        oper.push_back(make_shared<none>());
}

echo::~echo() {
    oper.clear();
}

ostream &echo::put(ostream &os) const {
    os << "ECHO = ";
    auto first = true;
    for (auto &p : this->oper) {
        if (first)
            first = false;
        else
            os << ", ";
        os << p->str();
    }
    return os << endl;
}

string echo::unsort::str() const {
    return "UNSORT";
}

echo::sort::sort(
    vector<shared_ptr<cdni_entry>> const &cdni) :
        describer(), cdni(cdni) {}

string echo::sort::str() const {
    auto l_cdni(cdni);
    ostringstream res(
        "SORT(", ostringstream::ate);
    res << l_cdni.front()->str();
    l_cdni.erase(l_cdni.begin());
    for (auto const p : l_cdni)
        res << ", " << p->str();
    res << ")";
    return res.str();
}

echo::sort::~sort() {
    cdni.clear();
}

echo::sort::cdni_entry::cdni_entry(
    std::string const &name, bool const except) :
        name(name), except(except) {}

string echo::sort::cdni_entry::str() const {
    std::string res("");
    if (except)
        res += "EXCEPT ";
    return res + name;
}

string echo::both::str() const {
    return "BOTH";
}

string echo::none::str() const {
    return "NONE";
}

echo::punch::punch(vector<option_type> const &options) :
        options(options) {}

string echo::punch::str() const {
    ostringstream res(
        "PUNCH", ostringstream::ate);
    if (options.size() > 0) {
        res << "(";
        bool first = true;
        for (auto p : options) {
            if (!first) {
                res << ", ";
                first = false;
            }
            switch (p) {
            case option_type::SORT:
                res << "SORT";
                break;
            case option_type::BOTH:
                res << "BOTH";
                break;
            case option_type::NEWBULK:
                res << "NEWBULK";
            }
        }
        res << ")";
    }
    return res.str();
}

string echo::file::str() const {
    return "FILE";
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// End:
