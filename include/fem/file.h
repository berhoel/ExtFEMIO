/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Processing Sesam FEM files.

   Detailed description
*/
// ID: $Id$

#pragma once

#if !defined _FEM_FILE_H_
#define _FEM_FILE_H_

#include <set>
// ReSharper disable CppUnusedIncludeDirective
#include <string>
#include <iostream>
// ReSharper restore CppUnusedIncludeDirective

namespace dnvgl {
    namespace extfem {
        namespace fem {
            namespace input {

                class fem_file {

                private:

                    static const std::set<char> cont_chars;
                    std::string cur_line;
                    std::istream &data;

                public:

                    std::string last_comment;

                    bool eof() const;

                    explicit fem_file(std::istream&);

                    size_t get(std::vector<std::string>&);

                    /// actual byte position (hopefully no fem > 2Gybte will be
                    /// readin ...)
                    std::streampos size() const;

                    /// actual byte position (hopefully no fem > 2Gybte will be
                    /// readin ...)
                    std::streampos pos() const;
                };
            }
        }
    }
}

#endif // _FEM_FILE_H_

// Local Variables:
// mode: c++
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&make -C ../../cbuild test"
// coding: utf-8
// End:
