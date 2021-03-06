/**
   \file
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2017 by DNV GL SE
   \brief Handle BDF comments

   Detailed description
*/
#include "extfemio_stdafx.h"

#include "extfem_misc.h"

// ID:
namespace {
    // ReSharper disable once CppDeclaratorNeverUsed
    char const _EXTFEMIO_UNUSED(cID_test_bdf_cards_comment[]) =
        "@(#) $Id$";
}

#define NOMINMAX // To avoid problems with "numeric_limits"

#include <catch.hpp>

#ifdef __GNUC__
#include "config.h"
#endif

#include "bdf/cards.h"
#include "bdf/file.h"

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
using namespace bdf;
using namespace bdf::cards;
using namespace bdf::cards::__base;

using bdf::types::entry_type;
using bdf::types::entry_value;

class test_comment : public comment {
public:
    using comment::find_yield;
};

TEST_CASE("BDF COMMENT yield stress definition.", "[bdf_comment]") {
#ifdef HAVE_BOOST_REGEX_HPP
        boost::smatch
#else
        std::smatch
#endif
            m;
		const std::string ref_235("$ 235");
        CHECK(regex_search(ref_235, m, test_comment::find_yield));
		const std::string ref_315("$ Yield: 315 test");
        CHECK(regex_search(ref_315, m, test_comment::find_yield));
		const std::string ref_355("$ 355");
	    CHECK(regex_search(ref_355, m, test_comment::find_yield));
		const std::string ref_390("$ 390");
		CHECK(regex_search(ref_390, m, test_comment::find_yield));
		const std::string ref_460("$ 460");
		CHECK(regex_search(ref_460, m, test_comment::find_yield));
}

TEST_CASE("BDF COMMENT definitions; empty comment.", "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ "});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({""}));
}

TEST_CASE("BDF COMMENT definitions; long comment.", "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ a",
            "$+     a"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"a     a"}));
}

TEST_CASE("BDF COMMENT definitions; long comment (2).", "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ a",
            "$+12345a"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"a12345a"}));
}

TEST_CASE("BDF COMMENT definitions; one line.",
          "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$foo"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"foo"}));
}

TEST_CASE("BDF COMMENT output; several lines.",
          "[bdf_comment]") {
    ostringstream test;

    SECTION("several lines in one call") {
        std::vector<std::string> const content({"foo", "bar"});
        comment probe;
        test << probe;
        test << probe(content);

        CHECK(test.str() == "$ foo\n"
              "$ bar\n");
    }

    SECTION("several lines in multiple calls") {
        comment probe;
        test << probe;
        test << probe("foo");
        test << probe("bar");

        CHECK(test.str() == "$ foo\n"
              "$ bar\n");
    }
}

TEST_CASE("BDF COMMENT definitions; with yield stress (235).",
          "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ 235"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"235"}));
    CHECK((comment::yield && *comment::yield == 235.));
}

TEST_CASE("BDF COMMENT definitions; with yield stress (315).",
          "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ yield stress for the following material definition:315"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({
                "yield stress for the following material definition:315"}));
    CHECK((probe.yield && *probe.yield == 315.));
    CHECK((comment::yield && *comment::yield == 315.));

    comment::clear_yield();

    CHECK(probe.yield == nullptr);
    CHECK(comment::yield == nullptr);
}

TEST_CASE("BDF COMMENT definitions; with yield stress (355).",
          "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ yield stress: 355"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"yield stress: 355"}));
    CHECK((probe.yield && *probe.yield == 355.));
    CHECK((comment::yield && *comment::yield == 355.));
}

TEST_CASE("BDF COMMENT definitions; with yield stress (390).",
          "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ 390"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"390"}));
    CHECK((probe.yield && *probe.yield == 390.));
    CHECK((comment::yield && *comment::yield == 390.));
}

TEST_CASE("BDF COMMENT definitions; with yield stress (460).",
          "[bdf_comment]") {

    std::list<std::string> const data({
            // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
            "$ 460"});
    std::list<std::string> lines;
    card::card_split(data, lines);
    comment probe(lines);

    CHECK(probe.content == list<std::string>({"460"}));
    CHECK((probe.yield && *probe.yield == 460.));
    CHECK((comment::yield && *comment::yield == 460.));
}

TEST_CASE("BDF COMMENT roundtrip test", "[bdf_comment]") {
    ostringstream test;

    std::string content;

    comment const probe(&content);
    test << probe;

    SECTION("check output") {
        CHECK(test.str() == "$ \n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        input::bdf_file file(raw);
        file.get(data);

        card::card_split(data, lines);
        comment probe_l(lines);

        CHECK(probe_l.content == list<std::string>({""}));
    }
}

TEST_CASE("BDF COMMENT roundtrip test (reuse)", "[bdf_comment]") {
    ostringstream test;

    std::string content;

    comment probe;
    test << probe;
    test << probe(&content);

    SECTION("check output") {
        CHECK(test.str() ==
              "$ \n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l;
        probe_l(lines);

        CHECK(probe_l.content == list<std::string>({""}));
    }
}

TEST_CASE("BDF COMMENT roundtrip test; two lines.", "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content{"This is a test", "one two"};

    comment const probe(content);
    test << probe;

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
              "$ This is a test\n"
              "$ one two\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l(lines);

        CHECK(probe_l.content == list<std::string>({
                    "This is a test",
                    "one two"}));
        CHECK(probe_l.yield == nullptr);
        CHECK(comment::yield == nullptr);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; two lines (reuse).", "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content{"This is a test", "one two"};

    comment probe;
    test << probe;
    test << probe(content);

    SECTION("check output") {
        CHECK(test.str() ==
              "$ This is a test\n"
              "$ one two\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l;
        probe_l(lines);

        CHECK(probe_l.content == list<std::string>({
                    "This is a test",
                    "one two"}));
        CHECK(probe_l.yield == nullptr);
        CHECK(comment::yield == nullptr);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; two lines (2).", "[bdf_comment]") {
    ostringstream test;

    std::vector<std::string> const content{"This is a test"};

    comment probe(content);
    probe("one two");
    test << probe;

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() == "$ one two\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l(lines);

        CHECK(probe_l.content == list<std::string>({"one two"}));
        CHECK(probe_l.yield == nullptr);
        CHECK(comment::yield == nullptr);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; long line.", "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content{
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean "
        "commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus"};

    comment const probe(content);
    test << probe;

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "$ Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligul\n"
        "$+a eget dolor. Aenean massa. Cum sociis natoque penatibus\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l(lines);

        CHECK(probe_l.content == list<std::string>({
                    "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. "
                    "Aenean commodo ligula eget dolor. Aenean massa. Cum "
                    "sociis natoque penatibus"}));
        CHECK(probe_l.yield == nullptr);
        CHECK(comment::yield == nullptr);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; long line (reuse).", "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content{
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean "
        "commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus"};

    comment probe;
    test << probe;
    test << probe(content);

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "$ Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligul\n"
        "$+a eget dolor. Aenean massa. Cum sociis natoque penatibus\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l;
        probe_l(lines);

        CHECK(probe_l.content == list<std::string>({
                    "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. "
                    "Aenean commodo ligula eget dolor. Aenean massa. Cum "
                    "sociis natoque penatibus"}));
        CHECK(probe_l.yield == nullptr);
        CHECK(comment::yield == nullptr);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; only yield stress.", "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content;
    auto *yield = new double(315.);

    comment const probe(content, yield);
    test << probe;

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "$ YIELD: 315\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l(lines);

        CHECK(probe_l.content == list<std::string>({"YIELD: 315"}));
        CHECK(*probe_l.yield == 315.);
        CHECK(*comment::yield == 315.);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; only yield stress (reuse).",
          "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content;
	const auto yield = new double(315.);

    comment probe;
    test << probe;
    test << probe(content, yield);

    // reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "$ YIELD: 315\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l;
        probe_l(lines);

        CHECK(probe_l.content == list<std::string>({"YIELD: 315"}));
        CHECK(*probe_l.yield == 315.);
        CHECK(*comment::yield == 315.);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; yield stress and comment.",
          "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content{
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean "
        "commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus"};
	const auto yield = new double(315.);

    comment const probe(content, yield);
    test << probe;

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
              // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
              "$ Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligul\n"
              "$+a eget dolor. Aenean massa. Cum sociis natoque penatibus\n"
              "$ YIELD: 315\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l(lines);

        CHECK(probe_l.content == list<std::string>({
                    "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. "
                    "Aenean commodo ligula eget dolor. Aenean massa. Cum "
                    "sociis natoque penatibus",
                    "YIELD: 315"}));
        CHECK(*probe_l.yield == 315.);
        CHECK(*comment::yield == 315.);
    }
}

TEST_CASE("BDF COMMENT roundtrip test; yield stress and comment (reuse).",
          "[bdf_comment]") {
    ostringstream test;

    vector<std::string> const content{
        // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
        "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean "
        "commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus"};
	auto*yield = new double(315.);

    comment probe;
    test << probe;
    test << probe(content, yield);

    reset_statics();

    SECTION("check output") {
        CHECK(test.str() ==
              // 34567a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567i1234567j
              "$ Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligul\n"
              "$+a eget dolor. Aenean massa. Cum sociis natoque penatibus\n"
              "$ YIELD: 315\n");
    }

    SECTION("check reading") {
        list<std::string> data;
        list<std::string> lines;
        std::string tmp;
        istringstream raw(test.str());

        while (getline(raw, tmp))
            data.push_back(tmp);
        card::card_split(data, lines);
        comment probe_l;
        probe_l(lines);

        CHECK(probe_l.content == list<std::string>({
                    "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean "
                    "commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus",
                    "YIELD: 315"}));
        CHECK(*probe_l.yield == 315.);
        CHECK(*comment::yield == 315.);
    }
}

// Local Variables:
// mode: c++
// coding: utf-8
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../cbuild -j7 &&
//    (make -C ../cbuild test ;
//     ../cbuild/tests/test_bdf_cards --use-colour no)"
// End:
