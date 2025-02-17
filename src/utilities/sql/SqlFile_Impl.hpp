/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_SQL_SQLFILE_IMPL_HPP
#define UTILITIES_SQL_SQLFILE_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "SummaryData.hpp"
#include "SqlFileEnums.hpp"
#include "SqlFileDataDictionary.hpp"
#include "PreparedStatement.hpp"
#include "../data/DataEnums.hpp"
#include "../data/EndUses.hpp"
#include "../core/Optional.hpp"
#include "../data/Matrix.hpp"

#include <boost/optional.hpp>

#include <string>
#include <vector>

struct sqlite3;

namespace openstudio {

// forward declarations
class SqlFileTimeSeriesQuery;
class EpwFile;
class DateTime;
class Calendar;

// private namespace
namespace detail {

  class UTILITIES_API SqlFile_Impl
  {
   public:
    /// constructor from filesystem path, will throw ConstructorException if file does not exist
    /// or if file is not valid
    /// createIndexes will create useful indexes when opening an sqlite file but for faster opening
    /// pass in false if those indexes are not needed
    SqlFile_Impl(const openstudio::path& path, const bool createIndexes = true);

    /// createIndexes will create useful indexes when creating an sqlite file but for faster creation
    /// pass in false if those indexes are not needed
    SqlFile_Impl(const openstudio::path& t_path, const openstudio::EpwFile& t_epwFile, const openstudio::DateTime& t_simulationTime,
                 const openstudio::Calendar& t_calendar, const bool createIndexes = true);

    // virtual destructor
    virtual ~SqlFile_Impl();

    /// remove useful indexes, good for inserting lots of data
    void removeIndexes();

    /// Create useful indexes
    void createIndexes();

    /// returns whether or not connection is open
    bool connectionOpen() const;

    /// get the path
    openstudio::path path() const;

    /// close the file
    bool close();

    /// re-open the file
    bool reopen();

    /// hours simulated
    boost::optional<double> hoursSimulated() const;

    /// net site energy in GJ
    boost::optional<double> netSiteEnergy() const;

    /// net source energy in GJ
    boost::optional<double> netSourceEnergy() const;

    /// total site energy in GJ
    boost::optional<double> totalSiteEnergy() const;

    /// total source energy in GJ
    boost::optional<double> totalSourceEnergy() const;

    // return a vector of all the available time series
    // do not return variables only reported at run period intervals
    std::vector<std::string> availableTimeSeries();

    /// Returns the annual total cost associated with the given fuel type in dollars.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCost(const FuelType& fuel) const;

    /// Returns the annual total cost per total building area associated with the given fuel type in dollars per square meter.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCostPerBldgArea(const FuelType& fuel) const;

    /// Returns the annual total cost per net conditioned building area associated with the given fuel type in dollars per square meter.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const;

    /// Returns the annual total cost for all fuel types in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalUtilityCost() const;

    /// Returns an EndUses object containing all end uses for the simulation.
    boost::optional<EndUses> endUses() const;

    OptionalDouble getElecOrGasUse(bool getGas = true) const;
    OptionalDouble getElecOrGasCost(bool bGetGas = true) const;

    /// Returns the energy consumption for the given fuel type, category and month
    /// Requires BUILDING ENERGY PERFORMANCE tabular report
    boost::optional<double> energyConsumptionByMonth(const openstudio::EndUseFuelType& t_fuelType,
                                                     const openstudio::EndUseCategoryType& t_categoryType,
                                                     const openstudio::MonthOfYear& t_monthOfYear) const;

    /// Returns the energy demand for the given fuel type, category and month.
    /// Requires BUILDING ENERGY PERFORMANCE tabular report. Value is energy use in W.
    boost::optional<double> peakEnergyDemandByMonth(const openstudio::EndUseFuelType& t_fuelType,
                                                    const openstudio::EndUseCategoryType& t_categoryType,
                                                    const openstudio::MonthOfYear& t_monthOfYear) const;

    /// Returns the total energy cost over the analysis period in dollars according to the discounting convention.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsEnergyCost() const;

    /// Returns the electric energy used for heating in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityHeating() const;

    /// Returns the electric energy used for cooling in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityCooling() const;

    /// Returns the electric energy used for interior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityInteriorLighting() const;

    /// Returns the electric energy used for exterior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityExteriorLighting() const;

    /// Returns the electric energy used for interior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityInteriorEquipment() const;

    /// Returns the electric energy used for exterior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityExteriorEquipment() const;

    /// Returns the electric energy used for fans in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityFans() const;

    /// Returns the electric energy used for pumps in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityPumps() const;

    /// Returns the electric energy used for heat rejection in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityHeatRejection() const;

    /// Returns the electric energy used for humidification in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityHumidification() const;

    /// Returns the electric energy used for heat recovery in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityHeatRecovery() const;

    /// Returns the electric energy used for water systems in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityWaterSystems() const;

    /// Returns the electric energy used for refrigeration in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityRefrigeration() const;

    /// Returns the electric energy used for generators in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityGenerators() const;

    /// Returns the electric energy used for all uses in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> electricityTotalEndUses() const;

    /// Returns the natural gas energy used for heating in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasHeating() const;

    /// Returns the natural gas energy used for cooling in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasCooling() const;

    /// Returns the natural gas energy used for interior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasInteriorLighting() const;

    /// Returns the natural gas energy used for exterior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasExteriorLighting() const;

    /// Returns the natural gas energy used for interior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasInteriorEquipment() const;

    /// Returns the natural gas energy used for exterior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasExteriorEquipment() const;

    /// Returns the natural gas energy used for fans in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasFans() const;

    /// Returns the natural gas energy used for pumps in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasPumps() const;

    /// Returns the natural gas energy used for heat rejection in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasHeatRejection() const;

    /// Returns the natural gas energy used for humidification in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasHumidification() const;

    /// Returns the natural gas energy used for heat recovery in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasHeatRecovery() const;

    /// Returns the natural gas energy used for water systems in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasWaterSystems() const;

    /// Returns the natural gas energy used for refrigeration in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasRefrigeration() const;

    /// Returns the natural gas energy used for generators in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasGenerators() const;

    /// Returns the natural gas energy used for all end uses in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> naturalGasTotalEndUses() const;

    /// Returns the energy used for heating from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelHeating() const;

    /// Returns the energy used for cooling from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelCooling() const;

    /// Returns the energy used for interior lighting from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelInteriorLighting() const;

    /// Returns the energy used for exterior lighting from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelExteriorLighting() const;

    /// Returns the energy used for interior equipment from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelInteriorEquipment() const;

    /// Returns the energy used for exterior equipment from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelExteriorEquipment() const;

    /// Returns the energy used for fans from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelFans() const;

    /// Returns the energy used for pumps from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelPumps() const;

    /// Returns the energy used for heat rejection from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelHeatRejection() const;

    /// Returns the energy used for humidification from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelHumidification() const;

    /// Returns the energy used for heat recovery from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelHeatRecovery() const;

    /// Returns the energy used for water systems from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelWaterSystems() const;

    /// Returns the energy used for refrigeration from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelRefrigeration() const;

    /// Returns the energy used for generators from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelGenerators() const;

    /// Returns the energy used for all end uses from other fuel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuelTotalEndUses() const;

    /// Returns the district cooling energy used for heating in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingHeating() const;

    /// Returns the district cooling energy used for cooling in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingCooling() const;

    /// Returns the district cooling energy used for interior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingInteriorLighting() const;

    /// Returns the district cooling energy used for exterior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingExteriorLighting() const;

    /// Returns the district cooling energy used for interior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingInteriorEquipment() const;

    /// Returns the district cooling energy used for exterior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingExteriorEquipment() const;

    /// Returns the district cooling energy used for fans in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingFans() const;

    /// Returns the district cooling energy used for pumps in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingPumps() const;

    /// Returns the district cooling energy used for heat rejection in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingHeatRejection() const;

    /// Returns the district cooling energy used for humidification in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingHumidification() const;

    /// Returns the district cooling energy used for heat recovery in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingHeatRecovery() const;

    /// Returns the district cooling energy used for water systems in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingWaterSystems() const;

    /// Returns the district cooling energy used for refrigeration in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingRefrigeration() const;

    /// Returns the district cooling energy used for generators in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingGenerators() const;

    /// Returns the district cooling energy used for all end uses in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtCoolingTotalEndUses() const;

    /// Returns the district heating energy used for heating in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingHeating() const;

    /// Returns the district heating energy used for cooling in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingCooling() const;

    /// Returns the district heating energy used for interior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingInteriorLighting() const;

    /// Returns the district heating energy used for exterior lighting in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingExteriorLighting() const;

    /// Returns the district heating energy used for interior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingInteriorEquipment() const;

    /// Returns the district heating energy used for exterior equipment in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingExteriorEquipment() const;

    /// Returns the district heating energy used for fans in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingFans() const;

    /// Returns the district heating energy used for pumps in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingPumps() const;

    /// Returns the district heating energy used for heat rejection in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingHeatRejection() const;

    /// Returns the district heating energy used for humidification in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingHumidification() const;

    /// Returns the district heating energy used for heat recovery in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingHeatRecovery() const;

    /// Returns the district heating energy used for water systems in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingWaterSystems() const;

    /// Returns the district heating energy used for refrigeration in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingRefrigeration() const;

    /// Returns the district heating energy used for generators in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingGenerators() const;

    /// Returns the district heating energy used for all end uses in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> districtHeatingTotalEndUses() const;

    /// Returns the water used for heating in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterHeating() const;

    /// Returns the water used for cooling in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterCooling() const;

    /// Returns the water used for interior lighting in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterInteriorLighting() const;

    /// Returns the water used for exterior lighting in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterExteriorLighting() const;

    /// Returns the water used for interior equipment in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterInteriorEquipment() const;

    /// Returns the water used for exterior equipment in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterExteriorEquipment() const;

    /// Returns the water used for fans in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterFans() const;

    /// Returns the water used for pumps in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterPumps() const;

    /// Returns the water used for heat rejection in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterHeatRejection() const;

    /// Returns the water used for humidification in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterHumidification() const;

    /// Returns the water used for heat recovery in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterHeatRecovery() const;

    /// Returns the water used for water systems in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterWaterSystems() const;

    /// Returns the water used for refrigeration in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterRefrigeration() const;

    /// Returns the water used for generators in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterGenerators() const;

    /// Returns the water used for all uses in cubic meters.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> waterTotalEndUses() const;

    boost::optional<double> hoursHeatingSetpointNotMet() const;

    boost::optional<double> hoursCoolingSetpointNotMet() const;

    // return a vector of all the available reporting frequencies for a given timeseries and environment period
    // std::vector<ReportingFrequency> availableReportingFrequencies(const std::string& envPeriod, const std::string& timeSeriesName) ;

    // get the type of environment period
    boost::optional<EnvironmentType> environmentType(const std::string& envPeriod) const;

    // return a vector of all the available environment periods
    std::vector<std::string> availableEnvPeriods() const;

    // return a vector of all the available reporting frequencies for a given environment period
    std::vector<std::string> availableReportingFrequencies(const std::string& envPeriod);

    // version specific translation for reporting frequencies used in database to reporting frequency enum values
    openstudio::OptionalReportingFrequency reportingFrequencyFromDB(const std::string& dbReportingFrequency);

    // return a vector of all the available variableName for environment period and reporting frequency
    std::vector<std::string> availableVariableNames(const std::string& envPeriod, const std::string& reportingFrequency) const;

    // return a vector of all timeseries matching name, envPeriod, and reportingFrequency
    // this could be used to get "Mean Air Temperature" for all zones for instance
    std::vector<TimeSeries> timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName);

    // return a vector of all keyValues matching name, envPeriod, and reportingFrequency
    std::vector<std::string> availableKeyValues(const std::string& envPeriod, const std::string& reportingFrequency,
                                                const std::string& timeSeriesName);

    // return a runPeriod value e.g. Electricity::Facility
    boost::optional<double> runPeriodValue(const std::string& envPeriod, const std::string& timeSeriesName, const std::string& keyValue);

    // return a single timeseries matching name, keyValue, envPeriod, and reportingFrequency
    // this could be used to get "Mean Air Temperature" for a particular zone
    boost::optional<TimeSeries> timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName,
                                           const std::string& keyValue);

    /** Expands query to create a vector of all matching queries. The returned queries will have
       *  one environment period, one reporting frequency, and one time series name specified. The
       *  returned queries will also be "vetted". */
    std::vector<SqlFileTimeSeriesQuery> expandQuery(const SqlFileTimeSeriesQuery& query);

    std::vector<SqlFileTimeSeriesQuery> expandEnvironment(const SqlFileTimeSeriesQuery& query);

    std::vector<SqlFileTimeSeriesQuery> expandReportingFrequency(const SqlFileTimeSeriesQuery& query);

    std::vector<SqlFileTimeSeriesQuery> expandTimeSeries(const SqlFileTimeSeriesQuery& query);

    std::vector<SqlFileTimeSeriesQuery> expandKeyValues(const SqlFileTimeSeriesQuery& query);

    /** Executes query. ReportingFrequency must be specified. Use expandQuery first to break query
       *  down by ReportingFrequency and determine how many TimeSeries will be returned. */
    std::vector<TimeSeries> timeSeries(const SqlFileTimeSeriesQuery& query);

    // returns an optional pair of date times for begin and end of daylight savings time
    boost::optional<std::pair<openstudio::DateTime, openstudio::DateTime>> daylightSavingsPeriod() const;

    /// returns datadictionary of available timeseries
    DataDictionaryTable dataDictionary() const;

    /// Energy plus version number
    // DLM@20100511: can we query this?
    std::string energyPlusVersion() const;

    /// Energy Plus eplusout.sql file name
    std::string energyPlusSqliteFile() const;

    /// get names of all available illuminance maps
    std::vector<std::string> illuminanceMapNames() const;
    std::vector<std::string> illuminanceMapNames(const std::string& envPeriod) const;

    /// get zone names of for specified illuminance map
    std::vector<std::string> illuminanceMapZoneNames(const std::string& name) const;
    std::vector<std::string> illuminanceMapZoneNames(const int& mapIndex) const;

    /// reference point for map - form RefPtn=(x:y:illuminance)
    boost::optional<std::string> illuminanceMapRefPt(const std::string& name, const int& ptNum) const;
    boost::optional<std::string> illuminanceMapRefPt(const int& mapIndex, const int& ptNum) const;

    /// minimum value of map
    boost::optional<double> illuminanceMapMinValue(const std::string& name) const;
    boost::optional<double> illuminanceMapMinValue(const int& mapIndex) const;

    /// maximum value of map
    boost::optional<double> illuminanceMapMaxValue(const std::string& name) const;
    boost::optional<double> illuminanceMapMaxValue(const int& mapIndex) const;

    /// minimum and maximum of map
    void illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const;
    void illuminanceMapMaxValue(const int& mapIndex, double& minValue, double& maxValue) const;

    /// x position (m) of the illuminance map
    Vector illuminanceMapX(const std::string& name, const DateTime& dateTime) const;

    /// y position (m) of the illuminance map
    Vector illuminanceMapY(const std::string& name, const DateTime& dateTime) const;

    /// x position (m) of the illuminance map
    Vector illuminanceMapX(const int& hourlyReportIndex) const;

    /// y position (m) of the illuminance map
    Vector illuminanceMapY(const int& hourlyReportIndex) const;

    /// hourly report index for mapIndex at date and time
    boost::optional<int> illuminanceMapHourlyReportIndex(const int& mapIndex, const DateTime& dateTime) const;

    // find the illuminance map index by name
    boost::optional<int> illuminanceMapIndex(const std::string& name) const;

    /// date time at hourly report index
    boost::optional<DateTime> illuminanceMapDate(const int& hourlyReportIndex) const;

    /// hourly report indices of the illuminance map
    std::vector<int> illuminanceMapHourlyReportIndices(const std::string& name) const;

    /// hourly report indices and dates of the illuminance map
    std::vector<std::pair<int, DateTime>> illuminanceMapHourlyReportIndicesDates(const int& mapIndex) const;

    /// hourly report indices and dates of the illuminance map
    std::vector<std::pair<int, DateTime>> illuminanceMapHourlyReportIndicesDates(const std::string& name) const;

    /// hourly report indices of the illuminance map
    std::vector<int> illuminanceMapHourlyReportIndices(const int& mapIndex) const;

    /// value (lux) of the illuminance map at date and time
    /// value(i,j) is the illuminance at x(i), y(j)
    Matrix illuminanceMap(const std::string& name, const DateTime& dateTime) const;

    /// value (lux) of the illuminance map at hourlyReportIndex
    /// value(i,j) is the illuminance at x(i), y(j)
    Matrix illuminanceMap(const int& hourlyReportIndex) const;

    /// value (lux) of the illuminance map at hourlyReportIndex
    /// value(i,j) is the illuminance at x(i), y(j) - returns x, y and illuminance
    void illuminanceMap(const int& hourlyReportIndex, std::vector<double>& x, std::vector<double>& y, std::vector<double>& illuminance) const;

    // execute a statement and return the first (if any) value as a double.
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    boost::optional<double> execAndReturnFirstDouble(const std::string& statement, Args&&... args) const {
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        return stmt.execAndReturnFirstDouble();
      }
      return boost::none;
    }

    // execute a statement and return the first (if any) value as an int
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    boost::optional<int> execAndReturnFirstInt(const std::string& statement, Args&&... args) const {
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        return stmt.execAndReturnFirstInt();
      }
      return boost::none;
    }

    // execute a statement and return the first (if any) value as a string
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    boost::optional<std::string> execAndReturnFirstString(const std::string& statement, Args&&... args) const {
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        return stmt.execAndReturnFirstString();
      }
      return boost::none;
    }

    /// execute a statement and return the results (if any) in a vector of double
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    boost::optional<std::vector<double>> execAndReturnVectorOfDouble(const std::string& statement, Args&&... args) const {
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        return stmt.execAndReturnVectorOfDouble();
      }
      return boost::none;
    }

    /// execute a statement and return the results (if any) in a vector of int
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    boost::optional<std::vector<int>> execAndReturnVectorOfInt(const std::string& statement, Args&&... args) const {
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        return stmt.execAndReturnVectorOfInt();
      }
      return boost::none;
    }

    /// execute a statement and return the results (if any) in a vector of string
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    boost::optional<std::vector<std::string>> execAndReturnVectorOfString(const std::string& statement, Args&&... args) const {
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        return stmt.execAndReturnVectorOfString();
      }
      return boost::none;
    }

    // execute a statement and return the error code, used for create/drop tables
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    int execute(const std::string& statement, Args&&... args) const {
      // copied in here to avoid including sqlite3 header everywhere
      constexpr auto SQLITE_ERROR = 1;
      auto code = SQLITE_ERROR;
      if (m_db) {
        PreparedStatement stmt(statement, m_db, false, args...);
        code = stmt.execute();
      }
      return code;
    }

    /// Returns the summary data for each install location and fuel type found in report variables
    std::vector<openstudio::SummaryData> getSummaryData() const;

    // Insert a new report variable record into the database
    // This does not support meter data
    void insertTimeSeriesData(const std::string& t_variableType, const std::string& t_indexGroup, const std::string& t_timestepType,
                              const std::string& t_keyValue, const std::string& t_variableName,
                              const openstudio::ReportingFrequency& t_reportingFrequency, const boost::optional<std::string>& t_scheduleName,
                              const std::string& t_variableUnits, const openstudio::TimeSeries& t_timeSeries);

    int insertZone(const std::string& t_name, double t_relNorth, double t_originX, double t_originY, double t_originZ, double t_centroidX,
                   double t_centroidY, double t_centroidZ, int t_ofType, double t_multiplier, double t_listMultiplier, double t_minimumX,
                   double t_maximumX, double t_minimumY, double t_maximumY, double t_minimumZ, double t_maximumZ, double t_ceilingHeight,
                   double t_volume, int t_insideConvectionAlgo, int t_outsideConvectionAlgo, double t_floorArea, double t_extGrossWallArea,
                   double t_extNetWallArea, double t_extWindowArea, bool t_isPartOfTotalArea);

    void insertIlluminanceMap(const std::string& t_zoneName, const std::string& name, const std::string& t_environmentName,
                              const std::vector<DateTime>& t_times, const std::vector<double>& t_xs, const std::vector<double>& t_ys, double t_z,
                              const std::vector<Matrix>& t_maps);

    bool isSupportedVersion() const;

    // Sql got "year" in 8.9.0 and working for 9.0.1
    bool hasYear() const;

    // DaylightMapHourlyReports added Year in 9.2.0
    bool hasIlluminanceMapYear() const;

   private:
    void init();

    void retrieveDataDictionary();

    // executes **MULTIPLE** statement and throws if it failed, used for create/drop tables.
    // Since it uses sqlite3_exec internally, cannot use Variadic arguments for binding
    void execAndThrowOnError(const std::string& t_stmt);

    // execute a **SINGLE** statement and throws if it failed
    // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
    template <typename... Args>
    void execAndThrowOnError(const std::string& bindingStatement, Args&&... args) {
      if (m_db) {
        PreparedStatement stmt(bindingStatement, m_db, false, args...);
        stmt.execAndThrowOnError();
      }
      std::runtime_error("Error executing SQL statement as database connection is not open.");
    }

    void addSimulation(const openstudio::EpwFile& t_epwFile, const openstudio::DateTime& t_simulationTime, const openstudio::Calendar& t_calendar);
    int getNextIndex(const std::string& t_tableName, const std::string& t_columnName);

    // return a single timeseries matching recordIndex - internally used to retrieve timeseries
    boost::optional<TimeSeries> timeSeries(const DataDictionaryItem& dataDictionary);
    std::vector<double> timeSeriesValues(const DataDictionaryItem& dataDictionary);
    boost::optional<Date> timeSeriesStartDate(const DataDictionaryItem& dataDictionary);

    // return first date in time table used for start date of run period variables
    openstudio::DateTime firstDateTime(bool includeHourAndMinute, int envPeriodIndex);

    // return last date in time table used for end date of run period variables
    openstudio::DateTime lastDateTime(bool includeHourAndMinute, int envPeriodIndex);

    // DLM: timeSeriesInterval seems pretty useless, can we remove it?
    boost::optional<Time> timeSeriesInterval(const DataDictionaryItem& dataDictionary);

    std::vector<DateTime> dateTimeVec(const DataDictionaryItem& dataDictionary);

    bool isValidConnection();

    void mf_makeConsistent(std::vector<SqlFileTimeSeriesQuery>& queries);

    openstudio::path m_path;
    bool m_connectionOpen;
    DataDictionaryTable m_dataDictionary;
    sqlite3* m_db;
    std::string m_sqliteFilename;

    bool m_supportedVersion;

    bool m_hasYear;

    bool m_hasIlluminanceMapYear;

    REGISTER_LOGGER("openstudio.energyplus.SqlFile");
  };

  // ETH@20100920 SqlFile_Impl& cannot be const because function calls non-const getter.
  UTILITIES_API std::set<ReportingFrequency> availableReportingFrequencySet(SqlFile_Impl& sqlFileImpl, const std::string& envPeriod);
}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_SQL_SQLFILE_IMPL_HPP
