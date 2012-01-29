/*!   \file aerodynamicCoefficientGenerator.h
 *    This file contains the definition of the aerodynamic coefficient generator base class.
 *
 *    Path              : /Astrodynamics/ForceModels/
 *    Version           : 6
 *    Check status      : Checked
 *
 *    Author            : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Checker           : B. Romgens
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : B.Romgens@student.tudelft.nl
 *
 *    Date created      : 25 November, 2010
 *    Last modified     : 15 June, 2011
 *
 *    References
 *      Gentry, A., Smyth, D., and Oliver, W. The Mark IV Supersonic-Hypersonic
 *        Arbitrary Body Program, Volume II - Program Formulation, Douglas
 *        Aircraft Company, 1973.
 *
 *    Notes
 *
 *    Copyright (c) 2010-2011 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      102511    D. Dirkx          First version of file.
 *      110501    D. Dirkx          Added more comments.
 *      112701    D. Dirkx          Finalized for code check.
 *      110131    B. Romgens        Minor modifications during code check.
 *      110204    D. Dirkx          Finalized code.
 *      110615    F.M. Engelen      Made a child of Coefficient Database. Moved aerodynamic
 *                                  reference quatities to the parent class.
 */

#ifndef AERODYNAMICCOEFFICIENTGENERATOR_H
#define AERODYNAMICCOEFFICIENTGENERATOR_H

// Include statements.
#include <Eigen/Core>
#include "Astrodynamics/ForceModels/aerodynamicCoefficientInterface.h"

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

//! Base class for aerodynamic coefficient generator.
/*!
 *  Abstract base class for aerodynamic analysis method. Stores independent variable values
 *  and data points of independent variables.
 *  Currently supports Mach number, angles of attack sideslip, and Reynold number
 *  as independent variables, but others can be added easily. To do so, only the
 *  interface function need to be added. As an example, for adding the Mach number,
 *  only the  get/set numberOfMachPoints and get/set
 *  MachPoint, as well as a machIndex_, to let the code know which index of the
 *  numberOfPointsPerIndependentVariables_ and dataPointsOfIndependentVariables_
 *  arrays represent the Mach number. Coefficients are
 *  stored in a VectorXd of pointers which should be allocated and initialized
 *  to NULL by a derived class. The conversion from the indices of the independent
 *  variables to the index in this VectorXd is performed by the
 *  variableIndicesToListIndex function.
 */
class AerodynamicCoefficientGenerator: public AerodynamicCoefficientInterface
{
public:

    //! Default constructor.
    /*!
     * Default constructor, initializes arrays to NULL ans sets number of
     * independent variables to 0.
     */
    AerodynamicCoefficientGenerator( ) : vehicleCoefficients_( NULL ),
        numberOfIndependentVariables_( 0 ), numberOfPointsPerIndependentVariables_( NULL ),
        dataPointsOfIndependentVariables_( NULL ), machIndex_( -0 ), angleOfAttackIndex_( -0 ),
        angleOfSideslipIndex_( -0 ), reynoldsNumberIndex_( -0 ), numberOfCases_( 0 ) { }

    //! Custom destructor.
    /*!
     * Custom destructor, deletes arrays of data points and values of
     * independent variables and resets them to NULL.
     */
    virtual ~AerodynamicCoefficientGenerator( );

    //! Set the number of independent variables
    /*!
     * Sets the number of independent variables.
     * \param numberOfVariables Number of independent variables for analysis.
     */
    void setNumberOfIndependentVariables( int numberOfVariables );

    //! Set the number of points for Mach number.
    /*!
     * Sets the number of different Mach numbers at which coefficients are determined.
     * \param numberOfMachPoints Number of data points for Mach number.
     */
    void setNumberOfMachPoints( int numberOfMachPoints );

    //! Sets the number of points for angle of attack.
    /*!
     * Sets the number of different angles of attack at which coefficients are determined.
     * \param numberOfAngleOfAttackPoints Number of data points for angle of attack.
     */
    void setNumberOfAngleOfAttackPoints( int numberOfAngleOfAttackPoints );

    //! Set the number of points for angle of sideslip.
    /*!
     * Sets the number of different angles of sideslip at which coefficients are determined.
     * \param numberOfAngleOfSideslipPoints Number of data points for angle of sideslip.
     */
    void setNumberOfAngleOfSideslipPoints( int numberOfAngleOfSideslipPoints );

    //! Set the number of points for the Reynolds Number.
    /*!
     * Sets the number of different Reynolds Number. at which coefficients are determined.
     * \param numberOfReynoldsNumberPoints Number of data points for ReynoldsNumber.
     */
    void setNumberOfReynoldsNumberPoints( int numberOfReynoldsNumberPoints );

    //! Get the number of independent variables
    /*!
     * Returns the number of independent variables.
     * \return numberOfVariables Number of independent variables for analysis.
     */
    int getNumberOfIndependentVariables( ) { return numberOfIndependentVariables_; }

    //! Get the number of points for an independent variable.
    /*!
     * Returns the number of different values of a given independent variable
     * at which coefficients are determined.
     * \param independentVariable Independent variable from which to retrieve
     * number of data points.
     * \return Number of data points for Mach number
     */
    int getNumberOfValuesOfIndependentVariable( int independentVariable )
    { return numberOfPointsPerIndependentVariables_ [ independentVariable ]; }

    //! Get the number of points for Mach number.
    /*!
     * Returns the number of different Mach numbers at which coefficients are determined.
     * \return Number of data points for Mach number
     */
    int getNumberOfMachPoints( ) { return numberOfPointsPerIndependentVariables_ [ machIndex_ ]; }

    //! Get the number of points for angle of attack.
    /*!
     * Returns the number of different angles of attack at which coefficients are determined.
     * \return Number of data points for angle of attack.
     */
    int getNumberOfAngleOfAttackPoints ( )
    { return numberOfPointsPerIndependentVariables_ [ angleOfAttackIndex_ ]; }

    //! Get the number of points for angle of sideslip.
    /*!
     * Returns the number of different angles of sideslip at which coefficients are determined.
     * \return Number of data points for angle of sideslip.
     */
    int getNumberOfAngleOfSideslipPoints ( )
    { return numberOfPointsPerIndependentVariables_ [ angleOfSideslipIndex_ ]; }

    //! Get the number of points for Reynold number.
    /*!
     * Returns the number of different Reynolds numbers at which coefficients are determined.
     * \return Number of data points for Reynold number.
     */
    int getNumberOfReynoldsNumberPoints( )
    { return numberOfPointsPerIndependentVariables_ [ reynoldsNumberIndex_ ]; }

    //! Set a Mach number point.
    /*!
     * Sets a Mach number at which aerodynamic coefficients are to be determined.
     * \param index Index in Mach number data point list at which to set the value.
     * \param machPoint Value of Mach number to set.
     */
    void setMachPoint( int index, double machPoint )
    { dataPointsOfIndependentVariables_[ angleOfAttackIndex_ ][ index ] = machPoint; }

    //! Set an angle of attack point.
    /*!
     * Sets an angle of attack at which aerodynamic coefficients are to be determined.
     * \param index Index in angle of attack number data point list at which to set the value.
     * \param angleOfAttackPoint Value of angle of attack to set.
     */
    void setAngleOfAttackPoint( int index, double angleOfAttackPoint )
    { dataPointsOfIndependentVariables_[ angleOfAttackIndex_ ][ index ] = angleOfAttackPoint; }

    //! Set an angle of sideslip point.
    /*!
     * Sets an angle of sideslip at which aerodynamic coefficients are to be determined.
     * \param index Index in angle of sideslip number data point list at which to set the value.
     * \param angleOfSideslipPoint Value of angle of sideslip to set.
     */
    void setAngleOfSideslipPoint( int index, double angleOfSideslipPoint )
    { dataPointsOfIndependentVariables_[ angleOfSideslipIndex_ ][ index ] = angleOfSideslipPoint; }

    //! Set a Reynolds Number point.
    /*!
     * Sets a ReyNolds Number at which aerodynamic coefficients are to be determined.
     * \param index Index in Reynold number data point list at which to set the value.
     * \param reynoldsNumberPoint Value of Reynold number to set.
     */
    void setReynoldsNumberPoint( int index, double reynoldsNumberPoint )
    { dataPointsOfIndependentVariables_[ reynoldsNumberIndex_ ][ index ] = reynoldsNumberPoint; }

    //! Get a Mach number point.
    /*!
     * Returns a Mach number at which aerodynamic coefficients are to be determined.
     * \param index Index from Mach number data point list from which to retrieve the value.
     * \return Value of Mach number at index.
     */
    double getMachPoint( int index )
    { return dataPointsOfIndependentVariables_[ machIndex_ ][ index ]; }

    //! Get an angle of attack point.
    /*!
     * Returns an angle of attack at which aerodynamic coefficients are to be determined.
     * \param index Index from angle of attack data point list from which to retrieve the value.
     * \return Value of angle of attack at index.
     */
    double getAngleOfAttackPoint( int index )
    { return dataPointsOfIndependentVariables_[ angleOfAttackIndex_ ][ index ]; }

    //! Get an angle of sideslip point.
    /*!
     * Returns an angle of sideslip at which aerodynamic coefficients are to be determined.
     * \param index Index from angle of attack data point list from which to retrieve the value.
     * \return Value of angle of attack at index.
     */
    double getAngleOfSideslipPoint( int index )
    { return dataPointsOfIndependentVariables_[ angleOfSideslipIndex_ ][ index ]; }

    //! Get an Reynold Number point.
    /*!
     * Returns an Reynold Number at which aerodynamic coefficients are to be determined.
     * \param index Index from Reynold Number data point list from which to retrieve the value.
     * \return Value of Reynold Number at index.
     */
    double getReynoldsNumberPoint( int index )
    { return dataPointsOfIndependentVariables_[ reynoldsNumberIndex_ ][ index ]; }

    //! Get a value of an independent variable.
    /*!
     * Returns a value of an independent variable at which aerodynamic
     * coefficients are to be determined.
     * \param independentVariable Independent variable index.
     * \param index Index from independent variable data point list from which to retrieve the
     *                  value.
     * \return Value of angle of attack at index.
     */
    double getIndependentVariablePoint( int independentVariable, int index )
    { return dataPointsOfIndependentVariables_[ independentVariable ][ index ]; }

    //! Get aerodynamic coefficients.
    /*!
     * Returns aerodynamic coefficients.
     * \param independentVariables Array of values of independent variable
     * indices in dataPointsOfIndependentVariables_.
     */
    virtual Eigen::VectorXd getAerodynamicCoefficients( int* independentVariables ) = 0;

    //! List of pointers to VectorXds containing coefficients.
    /*!
     * List of pointers to VectorXds containing coefficients.
     */
    Eigen::VectorXd** vehicleCoefficients_;

protected:

    //! Convert the independent variable indices to list index in vehicleCoefficients_.
    /*!
     * Converts the independent variable indices to list index in vehicleCoefficients_.
     * \param independentVariableIndices Array of indices of independent variables.
     * \return Resulting index in vehicleCoefficients_.
     */
    int variableIndicesToListIndex( int* independentVariableIndices );

    //! Number of independent variables in analysis.
    /*!
     * Number of independent variables in analysis. To be set from derived
     * class, depending on analysis type.
     */
    int numberOfIndependentVariables_;

    //! Array of number of points per independent variable in analysis.
    /*!
     * Array of number of points per independent variable. Physical meaning
     * of indices are determined by the machIndex, angleOfAttackIndex, etc.
     */
    int* numberOfPointsPerIndependentVariables_;

    //! Array of arrays of data points for independent variables.
    /*!
     * Array of arrays of data points for independent variables. Physical
     * meaning of indices are determined by the machIndex, angleOfAttackIndex, etc.
     */
    double** dataPointsOfIndependentVariables_;

    //! Index in independent variables arrays representing Mach number.
    /*!
     * Index in independent variables arrays representing Mach number.
     */
    int machIndex_;

    //! Index in independent variables arrays representing angle of attack.
    /*!
     * Index in independent variables arrays representing angle of attack.
     */
    int angleOfAttackIndex_;

    //! Index in independent variables arrays representing angle of sideslip.
    /*!
     * Index in independent variables arrays representing angle of sideslip.
     */
    int angleOfSideslipIndex_;

    //! Index in independent variables arrays representing Reynold Number.
    /*!
     * Index in independent variables arrays representing Reynold Number.
     */
    int reynoldsNumberIndex_;

    //! Number of data points in aerodynamic database.
    /*!
     * Number of data points in aerodynamic database, should equal product of number of data
     * points per independent variables tha are used.
     */
    int numberOfCases_;
};

}

#endif // AERODYNAMICCOEFFICIENTGENERATOR_H

// End of file.