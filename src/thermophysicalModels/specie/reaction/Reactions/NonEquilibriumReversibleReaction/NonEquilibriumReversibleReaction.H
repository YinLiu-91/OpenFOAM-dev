/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::NonEquilibriumReversibleReaction

Description
    Simple extension of Reaction to handle reversible reactions using
    equilibrium thermodynamics.

SourceFiles
    NonEquilibriumReversibleReaction.C

\*---------------------------------------------------------------------------*/

#ifndef NonEquilibriumReversibleReaction_H
#define NonEquilibriumReversibleReaction_H

#include "Reaction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class NonEquilibriumReversibleReaction Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ReactionRate>
class NonEquilibriumReversibleReaction
:
    public Reaction<ReactionThermo>
{
    // Private Data

        ReactionRate fk_;
        ReactionRate rk_;


public:

    //- Runtime type information
    TypeName("nonEquilibriumReversible");


    // Constructors

        //- Construct from components
        NonEquilibriumReversibleReaction
        (
            const Reaction<ReactionThermo>& reaction,
            const ReactionRate& forwardReactionRate,
            const ReactionRate& reverseReactionRate
        );

        //- Construct as copy given new speciesTable
        NonEquilibriumReversibleReaction
        (
            const NonEquilibriumReversibleReaction
            <
                ReactionThermo,
                ReactionRate
            >&,
            const speciesTable& species
        );

        //- Construct from dictionary
        NonEquilibriumReversibleReaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            const dictionary& dict
        );

        //- Construct from objectRegistry and dictionary
        NonEquilibriumReversibleReaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            const objectRegistry& ob,
            const dictionary& dict
        );

        //- Construct and return a clone
        virtual autoPtr<Reaction<ReactionThermo>> clone() const
        {
            return autoPtr<Reaction<ReactionThermo>>
            (
                new NonEquilibriumReversibleReaction
                <
                    ReactionThermo,
                    ReactionRate
                >(*this)
            );
        }

        //- Construct and return a clone with new speciesTable
        virtual autoPtr<Reaction<ReactionThermo>> clone
        (
            const speciesTable& species
        ) const
        {
            return autoPtr<Reaction<ReactionThermo>>
            (
                new NonEquilibriumReversibleReaction
                <
                    ReactionThermo,
                    ReactionRate
                >
                (
                    *this,
                    species
                )
            );
        }


    //- Destructor
    virtual ~NonEquilibriumReversibleReaction()
    {}


    // Member Functions

        // NonEquilibriumReversibleReaction rate coefficients

            //- Forward rate constant
            virtual scalar kf
            (
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li
            ) const;

            //- Reverse rate constant from the given formard rate constant
            virtual scalar kr
            (
                const scalar kfwd,
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li
            ) const;

            //- Reverse rate constant.
            //  Note this evaluates the forward rate constant and divides by the
            //  equilibrium constant
            virtual scalar kr
            (
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li
            ) const;


        // ReversibleReaction Jacobian functions

            //- Temperature derivative of forward rate
            virtual scalar dkfdT
            (
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li
            ) const;

            //- Temperature derivative of backward rate
            virtual scalar dkrdT
            (
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li,
                const scalar dkfdT,
                const scalar kr
            ) const;

            //- Third-body efficiencies (beta = 1-alpha)
            //  non-empty only for third-body reactions
            //  with enhanced molecularity (alpha != 1)
            virtual const List<Tuple2<label, scalar>>& beta() const;

            //- Species concentration derivative of the pressure dependent term
            //  By default this value is 1 as it multiplies the third-body term
            virtual void dcidc
            (
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li,
                scalarField& dcidc
            ) const;

            //- Temperature derivative of the pressure dependent term
            //  By default this value is 0 since ddT of molecularity is approx.0
            virtual scalar dcidT
            (
                const scalar p,
                const scalar T,
                const scalarField& c,
                const label li
            ) const;


        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=
        (
            const NonEquilibriumReversibleReaction
            <
                ReactionThermo,
                ReactionRate
            >&
        ) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "NonEquilibriumReversibleReaction.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
