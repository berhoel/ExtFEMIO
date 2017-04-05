/**
   \file bdf/cards_loads.h
   \author Berthold Höllmann <berthold.hoellmann@dnvgl.com>
   \copyright Copyright © 2015 by DNV GL SE
   \brief Classes for the different Nastran BDF cards dealing with loads.

   Detailed description
*/

// ID: $Id$

#pragma once

#if !defined _BDF_CARDS_LOADS_H_
#define _BDF_CARDS_LOADS_H_

#include <memory>

#include "bdf/cards_elements.h"

namespace dnvgl {
    namespace extfem {
        namespace bdf {
            namespace cards {
                namespace __base {

/// Base class for handling forces and moments.
                    class momforce : public card {

                    protected:

                        entry_type<long>
                        static const form_SID;
                        entry_type<long>
                        static const form_G;
                        entry_type<long>
                        static const form_CID;
                        entry_type<double>
                        static const form_F;
                        entry_type<double>
                        static const form_N1;
                        entry_type<double>
                        static const form_N2;
                        entry_type<double>
                        static const form_N3;

                    public:

/// Load std::set identification number. (Integer > 0)
                        entry_value<long> SID;
/// Grid point identification number. (Integer > 0)
                        entry_value<long> G;
/// Coordinate system identification number. (Integer > 0; Default = 0)
                        entry_value<long> CID;
///Scale factor. (Real)
                        entry_value<double> F;
/** *x* components of a vector measured in coordinate system defined
    by `CID`. (Real; at least one `Ni` ≠ 0.0.)
*/
                        entry_value<double> N1;
/** *y* components of a vector measured in coordinate system defined
    by `CID`. (Real; at least one `Ni` ≠ 0.0.)
*/
                        entry_value<double> N2;
/** *z* components of a vector measured in coordinate system defined
    by `CID`. (Real; at least one `Ni` ≠ 0.0.)
*/
                        entry_value<double> N3;

                    protected:

                        void read(std::list<std::string> const &) override;

                        momforce();

                        momforce(std::list<std::string> const &inp);

                        momforce(
                            long const *SID, long const *G, long const *CID,
                            double const *F,
                            double const *N1, double const *N2=nullptr,
                            double const *N3=nullptr);

                        virtual format_entry *get_head() const = 0;

                        card const &operator() (
                            long const *SID, long const *G, long const *CID,
                            double const *F,
                            double const *N1, double const *N2=nullptr,
                            double const *N3=nullptr);

                        void collect_outdata (
                            std::list<std::unique_ptr<format_entry> > &) const override;
                    };
                }

/// Handle Nastran Bulk `FORCE` entries.
/** # Static Force

Defines a static concentrated force at a grid point by specifying a
vector.

# Format

| 1       | 2     | 3   | 4     | 5   | 6    | 7    | 8    | 9 | 10 |
| ------- | ----- | --- | ----- | --- | ---- | ---- | ---- | - | -- |
| `FORCE` | `SID` | `G` | `CID` | `F` | `N1` | `N2` | `N3` |   |    |
*/
                class force : public __base::momforce {

                private:

                    static bdf::types::card head;

                public:

                    force();

                    force(const std::list<std::string> &inp);

                    force(
                        long const *SID, long const *G, long const *CID,
                        double const *F,
                        double const *N1, double const *N2=nullptr,
                        double const *N3=nullptr);

                    types card_type() const override;

                    using momforce::read;
                    using momforce::operator();

                private:

                    format_entry *get_head() const override;

                    using momforce::collect_outdata;
                };

/// Handle Nastran Bulk `MOMENT` entries.
/** # Static Force

Defines a static concentrated force at a grid point by specifying a
vector.

## Format

| 1        | 2     | 3   | 4     | 5   | 6    | 7    | 8    | 9 | 10 |
| -------- | ----- | --- | ----- | --- | ---- | ---- | ---- | - | -- |
| `MOMENT` | `SID` | `G` | `CID` | `F` | `N1` | `N2` | `N3` |   |    |
*/
                class moment : public __base::momforce {

                private:

                    static bdf::types::card head;

                public:

                    moment();

                    moment(const std::list<std::string> &inp);

                    moment(
                        long const *SID, long const *G, long const *CID,
                        double const *F,
                        double const *N1, double const *N2=nullptr,
                        double const *N3=nullptr);

                    types card_type() const override;

                    using momforce::read;
                    using momforce::operator();

                private:

                    format_entry *get_head() const override;

                    using momforce::collect_outdata;
                };

/// Handle Nastran Bulk CMASS2 entries.
/** # Parameter

Scalar Mass Property and Connection

Defines a scalar mass element without reference to a property entry.

# Format:

| 1        | 2     | 3   | 4    | 5    | 6    | 7    | 8 | 9 | 10 |
| -------- | ----- | --- | ---- | ---- | ---- | ---- | - | - | -- |
| `CMASS2` | `EID` | `M` | `G1` | `C1` | `G2` | `C2` |   |   |    |

# Example:

| 1        | 2  | 3    | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
| -------- | -- | ---- | - | - | - | - | - | - | -- |
| `CMASS2` | 32 | 9.25 | 6 | 1 |   |   |   |   |    |

# Remarks:

1. Scalar points may be used for `G1` and/or `G2`, in which case the
   corresponding `C1` and/or `C2` must be zero or blank. Zero or blank
   may be used to indicate a grounded terminal G1 or G2 with a
   corresponding blank or zero `C1` or `C2`. A grounded terminal is a
   point with a displacement that is constrained to zero. If only
   scalar points and/or ground are involved, it is more efficient to
   use the `CMASS4` entry.

2. Element identification numbers should be unique with respect to all
   other element identification numbers.

3. The two connection points (`G1`, `C1`) and (`G2`, `C2`) must be
   distinct. Except in unusual circumstances, one of them will be a
   grounded terminal with blank entries for `G*i*` and `C*i*`.

4. For a discussion of the scalar elements, see “Scalar Elements
   (`CELAS*i*`, `CMASS*i*`, `CDAMP*i*`)” on page 193 of the
   MSC.Nastran Reference Guide.

5. A scalar point specified on this entry need not be defined on an
   `SPOINT` entry.

6. If `G*i*` refers to a grid point then `C*i*` refers to
   degrees-of-freedom(s) in the displacement coordinate system
   specified by `CD` on the `GRID` entry.

7. Scalar elements input coupled mass matrices when the second pair of
   fields is entered. When uncoupled point masses are desired input
   only the first pair of fields. When a coupled mass matrix is
   requested the submatrix added has M on the diagonal, and -M on the
   off-diagonal. The element is not checked for internal constraints,
   which is the user’s responsibility if desired. There are instances
   where elements with internal constraints are desired, although not
   frequently. To identify the presence of internal constraints caused
   by coupled mass, inspect `GPWG` output, `OLOAD` output due to
   `GRAV` loads, and rigid body modes of free structures. Some forms
   of coupled mass will cause coupling of rigid body translational
   mass terms in `GPWG` output, and poor rigid body modes in modal
   analysis.

*/

                class cmass2 : public __base::element {

                    bdf::types::card static head;
                    // dnvgl::extfem::bdf::types::entry_type<long>
                    // static const form_EID;
                    entry_type<double>
                    static const form_M;
                    entry_type<long>
                    static const form_G1;
                    entry_type<long>
                    static const form_G2;
                    entry_type<std::list<int>>
                    static const form_C1;
                    entry_type<std::list<int>>
                    static const form_C2;

                public:
                    // /** Unique element identification number. (0 < Integer
                    //     < 100,000,000)
                    //  */
                    // dnvgl::extfem::bdf::types::entry_value<long> EID;
                    /** Value of the scalar mass. (Real)
                     */
                    entry_value<double> M;
                    /** Geometric grid or scalar point identification
                        number. (Integer > 0)
                    */
                    entry_value<long> G1;
                    /** Component number. (0 < Integer < 6; blank or zero
                        if scalar point)
                    */
                    entry_value<std::list<int>> C1;
                    /** Geometric grid or scalar point identification
                        number. (Integer > 0)
                    */
                    entry_value<long> G2;
                    /** Component number. (0 < Integer < 6; blank or zero
                        if scalar point)
                    */
                    entry_value<std::list<int>> C2;

                private:
                    explicit cmass2(std::string const&);

                public:

                    cmass2();

                    cmass2(std::list<std::string> const&);

                    cmass2(long const *EID, double const *M,
                           long const *G1, std::list<int> const *C1,
                           long const *G2=nullptr,
                           std::list<int> const *C2=nullptr);

                    card const &operator() (
                        long const *EID, double const *M,
                        long const *G1, std::list<int> const *C1,
                        long const *G2=nullptr,
                        std::list<int> const *C2=nullptr);

                    types card_type() const override;

                    void read(std::list<std::string> const &) override;

                private:

                    void collect_outdata(
                        std::list<std::unique_ptr<format_entry> >&) const override;
                };

/// Handle Nastran Bulk CMASS4 entries.
/** # Parameter

Scalar Mass Property and Connection to Scalar Points Only

Defines a scalar mass element that is connected only to scalar points, without
reference to a property entry.

# Format:

| 1        | 2     | 3   | 4    | 5    | 6 | 7 | 8 | 9 | 10 |
| -------- | ----- | --- | ---- | ---- | - | - | - | - | -- |
| `CMASS4` | `EID` | `M` | `S1` | `S2` |   |   |   |   |    |

# Example:

| 1        | 2  | 3    | 4 | 5  | 6 | 7 | 8 | 9 | 10 |
| -------- | -- | ---- | - | -- | - | - | - | - | -- |
| `CMASS4` | 23 |14.92 |   | 23 |   |   |   |   |    |

# Remarks:

1. `S1` or `S2` may be blank or zero, indicating a constrained
   coordinate. This is the usual case.

2. Element identification numbers should be unique with respect to all
   other element identification numbers.

3. Only one scalar mass element may be defined on a single entry.

4. For a discussion of the scalar elements, see “Scalar Elements
   (`CELAS*i*`, `CMASS*i*`, `CDAMP`i*`)” on page 193 of the
   MSC.Nastran Reference Guide.

5. A scalar point specified on this entry need not be defined on an
   `SPOINT` entry.
*/

                class cmass4 : public __base::element {
                    bdf::types::card static head;
                    // dnvgl::extfem::bdf::types::entry_type<long>
                    // static const form_EID;
                    entry_type<double>
                    static const form_M;
                    entry_type<long>
                    static const form_S1;
                    entry_type<long>
                    static const form_S2;

                public:
                    // /** Unique element identification number. (0 < Integer
                    //     < 100,000,000)
                    //  */
                    // dnvgl::extfem::bdf::types::entry_value<long> EID;
                    /** Scalar mass value. (Real)
                     */
                    entry_value<double> M;
                    /** Scalar point identification numbers. (Integer > 0;
                        S1 ≠ S2 )
                    */
                    entry_value<long> S1;
                    /** Scalar point identification numbers. (Integer > 0;
                        S1 ≠ S2 )
                    */
                    entry_value<long> S2;
                private:
                    explicit cmass4(std::string const&);

                public:

                    cmass4();

                    cmass4(std::list<std::string> const&);

                    cmass4(long const *EID, double const *M,
                           long const *S1, long const *S2=nullptr);

                    card const &operator() (
                        long const *EID, double const *M,
                        long const *S1, long const *S2=nullptr);

                    types card_type() const override;

                    void read(std::list<std::string> const &) override;

                private:

                    void collect_outdata(
                        std::list<std::unique_ptr<format_entry> >&) const override;
                };

/// Handle Nastran Bulk GRAV entries.
/** # Parameter

Acceleration or Gravity Load

Defines acceleration vectors for gravity or other acceleration loading.

# Format:

| 1      | 2     | 3     | 4   | 5    | 6    | 7    | 8    | 9 | 10 |
| ------ | ----- | ----- | --- | ---- | ---- | ---- | ---- | - | -- |
| `GRAV` | `SID` | `CID` | `A` | `N1` | `N2` | `N3` | `MB` |   |    |

# Example:

| 1      | 2 | 3 | 4    | 5   | 6   | 7    | 8 | 9 | 10 |
| ------ | - | - | ---- | --- | --- | ---- | - | - | -- |
| `GRAV` | 1 | 3 | 32.2 | 0.0 | 0.0 | -1.0 |   |   |    |

# Remarks:

1. The acceleration vector is defined by \f$\vec{a} = \vec{A}N\f$ ,
   where \f$N\f$ is the vector defined by (\f$N1\f$, \f$N2\f$,
   \f$N3\f$). The magnitude of a is equal to \f$A\f$ times the
   magnitude of \f$N\f$. The static loads generated by this entry are
   in the direction of \f$a\f$.

2. A `CID` of zero references the basic coordinate system.

3. Acceleration or gravity loads may be combined with “simple loads”
   (e.g., `FORCE`, `MOMENT`) only by specification on a `LOAD` entry.
   That is, the `SID? on a `GRAV` entry may not be the same as that on
   a simple load entry.

4. In the static solution sequences, the load set `ID` must be
   selected by the Case Control command `LOAD` = `SID`. In the dynamic
   solution sequences, `SID` must be referenced in the `LID` field of
   an `LSEQ` entry, which in turn must be selected by the Case Control
   command `LOADSET`.

5. At most nine `GRAV` entries can be selected in a given run either
   by Case Control or the `LOAD` Bulk Data entry. Multiples or
   reflections of a given acceleration or gravity load can be
   economically accomplished by use of the `LOAD` Bulk Data entry.

6. In cyclic symmetry solution sequences, the `T3` axis of the
   coordinate system referenced in field 3 must be parallel to the
   axis of symmetry. In dihedral cyclic symmetry (where `STYPE = "DIH"`
   on the `CYSYM` entry), the `T1` axis must, in addition, be parallel to
   Side 1 of segment 1R of the model.

7. For image superelements, the coordinate system must be rotated if
   the image is rotated relative to its primary superelement.

8. Acceleration or gravity loads do not include effects due to mass on
   scalar points.

9. The `RFORCE` entry may be used to specify rotational accelerations.

10. The coordinate systems in the main Bulk Data Section are defined
    relative to the assembly basic coordinate system which is fixed.
    This feature is useful when a superelement defined by a
    partitioned Bulk Data Section is rotated or mirrored and the
    gravity load is more conveniently defined in terms of coordinates
    which are fixed.
*/

                class grav : public __base::card {

                    bdf::types::card static head;
                    entry_type<long>
                    static const form_SID;
                    entry_type<long>
                    static const form_CID;
                    entry_type<double>
                    static const form_A;
                    entry_type<double>
                    static const form_Ni;
                    entry_type<long>
                    static const form_MB;

                public:
/// Set identification number. (Integer > 0)
                    entry_value<long> SID;
/// Coordinate system identification number. (Integer > 0; Default = 0)
                    entry_value<long> CID;
/// Acceleration vector scale factor. (Real)
                    entry_value<double> A;
/** Acceleration vector components measured in coordinate system CID.
    (Real; at least one Ni ≠ 0.0)
*/
                    entry_value<double> N1;
/** Acceleration vector components measured in coordinate system CID.
    (Real; at least one Ni ≠ 0.0)
*/
                    entry_value<double> N2;
/** Acceleration vector components measured in coordinate system CID.
    (Real; at least one Ni ≠ 0.0)
*/
                    entry_value<double> N3;
/** Indicates whether the CID coordinate system is defined in the main
    Bulk Data Section (`MB` = -1) or the partitioned superelement Bulk
    Data Section (`MB` = 0). Coordinate systems referenced in the main
    Bulk Data Section are considered stationary with respect to the
    assembly basic coordinate system. See Remark 10. (Integer; Default
    = 0)
*/
                    entry_value<long> MB;

                private:

                    explicit grav(std::string const&);

                public:

                    grav();

                    grav(std::list<std::string> const&);

                    grav(long const *SID, long const *CID,
                         double const *A,
                         double const *N1, double const*N2, double const *N3,
                         long const *MB=nullptr);

                    grav(long const *SID, long const *CID,
                         double const *A,
                         std::vector<double> const *N,
                         long const *MB=nullptr);

                    card const &operator() (
                        long const *SID, long const *CID,
                        double const *A,
                        std::vector<double> const *N,
                        long const *MB=nullptr);

                    card const &operator() (
                        long const *SID, long const *CID,
                        double const *A,
                        double const *N1, double const*N2, double const *N3,
                        long const *MB=nullptr);

                    types card_type() const override;

                    void read(std::list<std::string> const &) override;

                private:

                    void collect_outdata(
                        std::list<std::unique_ptr<format_entry> >&) const override;
                };

/// Handle Nastran Bulk `LOAD` entries.
/** # Static Load Combination (Superposition)

Defines a static load as a linear combination of load std::sets defined via
`FORCE`, `MOMENT`, `FORCE1`, `MOMENT1`, `FORCE2`, `MOMENT2`, `DAREA`
(if these entries have been converted), `PLOAD`, `PLOAD1`, `PLOAD2`,
`PLOAD4`, `PLOADX1`, `SLOAD`, `RFORCE`, and `GRAV` entries.

# Format

| 1      | 2     | 3    | 4      | 5    | 6    | 7    | 8    | 9    | 10 |
| ------ | ----- | ---- | ------ | ---- | ---- | ---- | ---- | ---- | -- |
| `LOAD` | `SID` | `S`  | `S1`   | `L1` | `S2` | `L2` | `S3` | `L3` |    |
|        | `S4`  | `L4` | *etc.* |      |      |      |      |      |    |
*/

                class load : public __base::card {

                private:

                    bdf::types::card static head;

                    entry_type<long>
                    static const form_SID;
                    entry_type<double>
                    static const form_S;
                    entry_type<double>
                    static const form_Si;
                    entry_type<long>
                    static const form_Li;

                public:

/// Load std::set identification number. (Integer > 0)
                    entry_value<long> SID;
/// Overall scale factor. (Real)
                    entry_value<double> S;
/// Scale factor on `Li`. (Real)
                    std::list<double> Si;
/** Load std::set identification numbers defined on entry types listed
    above. (Integer > 0)
*/
                    std::list<long> Li;

                    load();

                    load(const std::list<std::string> &inp);

                    load(long const *SID, double const *S,
                         std::list<double> const *Si,
                         std::list<long> const *Li);

                    card const &operator() (
                        long const *SID, double const *S,
                        std::list<double> const *Si,
                        std::list<long> const *Li);

                    types card_type() const override;

                    void read(std::list<std::string> const &) override;

                private:

                    void collect_outdata(
                        std::list<std::unique_ptr<format_entry> > &res) const override;
                };
            }
        }
    }
}

#endif // _BDF_CARDS_LOADS_H_

// Local Variables:
// mode: c++
// c-file-style: "dnvgl"
// indent-tabs-mode: nil
// compile-command: "make -C ../../cbuild -j8&&
//    (make -C ../../cbuild test;
//     ../../cbuild/tests/test_bdf_cards --use-colour no)"
// coding: utf-8
// End:
