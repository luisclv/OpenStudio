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

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "CoilCoolingDXVariableRefrigerantFlow_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                       bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType());
    }

    ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType());
    }

    ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(
      const ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Zone VRF Air Terminal Total Cooling Rate",       "Zone VRF Air Terminal Total Cooling Energy",
        "Zone VRF Air Terminal Sensible Cooling Rate",    "Zone VRF Air Terminal Sensible Cooling Energy",
        "Zone VRF Air Terminal Latent Cooling Rate",      "Zone VRF Air Terminal Latent Cooling Energy",
        "Zone VRF Air Terminal Total Heating Rate",       "Zone VRF Air Terminal Total Heating Energy",
        "Zone VRF Air Terminal Sensible Heating Rate",    "Zone VRF Air Terminal Sensible Heating Energy",
        "Zone VRF Air Terminal Latent Heating Rate",      "Zone VRF Air Terminal Latent Heating Energy",
        "Zone VRF Air Terminal Cooling Electricity Rate", "Zone VRF Air Terminal Cooling Electricity Energy",
        "Zone VRF Air Terminal Heating Electricity Rate", "Zone VRF Air Terminal Heating Electricity Energy",
        "Zone VRF Air Terminal Fan Availability Status"};
      return result;
    }

    IddObjectType ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::iddObjectType() const {
      return ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilityschedule) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACTerminalUnitVariableRefrigerantFlow", "Terminal Unit Availability schedule"));
      }
      if (std::find(b, e, OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeSchedule) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACTerminalUnitVariableRefrigerantFlow", "Supply Air Fan Operating Mode Schedule"));
      }
      return result;
    }

    Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::terminalUnitAvailabilityschedule() const {
      boost::optional<Schedule> value = optionalTerminalUnitAvailabilityschedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Terminal Unit Availabilityschedule attached.");
      }
      return value.get();
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateWhenNoCoolingisNeeded() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateWhenNoHeatingisNeeded() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFanOperatingModeSchedule() const {
      boost::optional<Schedule> value = optionalSupplyAirFanOperatingModeSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan Operating Mode Schedule attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFan);
    }

    boost::optional<CoilCoolingDXVariableRefrigerantFlow> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::coolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<CoilCoolingDXVariableRefrigerantFlow>(
        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoil);
    }

    boost::optional<CoilHeatingDXVariableRefrigerantFlow> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::heatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<CoilHeatingDXVariableRefrigerantFlow>(
        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoil);
    }

    double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::zoneTerminalUnitOnParasiticElectricEnergyUse() const {
      boost::optional<double> value =
        getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::zoneTerminalUnitOffParasiticElectricEnergyUse() const {
      boost::optional<double> value =
        getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ratedTotalHeatingCapacitySizingRatio() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setTerminalUnitAvailabilityschedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilityschedule,
                                "ZoneHVACTerminalUnitVariableRefrigerantFlow", "Terminal Unit Availability schedule", schedule);
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (supplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation,
                           supplyAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateWhenNoCoolingisNeeded(
      boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded) {
      bool result(false);
      if (supplyAirFlowRateWhenNoCoolingisNeeded) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded,
                           supplyAirFlowRateWhenNoCoolingisNeeded.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateWhenNoCoolingisNeeded() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateDuringHeatingOperation(
      boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (supplyAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation,
                           supplyAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateWhenNoHeatingisNeeded(
      boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded) {
      bool result(false);
      if (supplyAirFlowRateWhenNoHeatingisNeeded) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded,
                           supplyAirFlowRateWhenNoHeatingisNeeded.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateWhenNoHeatingisNeeded() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setOutdoorAirFlowRateDuringCoolingOperation(
      boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (outdoorAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation,
                           outdoorAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setOutdoorAirFlowRateDuringHeatingOperation(
      boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (outdoorAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation,
                           outdoorAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result(false);
      if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeSchedule,
                                "ZoneHVACTerminalUnitVariableRefrigerantFlow", "Supply Air Fan Operating Mode Schedule", schedule);
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setZoneTerminalUnitOnParasiticElectricEnergyUse(
      double zoneTerminalUnitOnParasiticElectricEnergyUse) {
      bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse,
                              zoneTerminalUnitOnParasiticElectricEnergyUse);
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setZoneTerminalUnitOffParasiticElectricEnergyUse(
      double zoneTerminalUnitOffParasiticElectricEnergyUse) {
      bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse,
                              zoneTerminalUnitOffParasiticElectricEnergyUse);
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
      bool result =
        setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio, ratedTotalHeatingCapacitySizingRatio);
      return result;
    }

    boost::optional<Schedule> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::optionalTerminalUnitAvailabilityschedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilityschedule);
    }

    boost::optional<Schedule> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::optionalSupplyAirFanOperatingModeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeSchedule);
    }

    unsigned ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::inletPort() const {
      return OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNode;
    }

    unsigned ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outletPort() const {
      return OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNode;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFan(const HVACComponent& component) {
      return setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFan, component.handle());
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::resetSupplyAirFan() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFan, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setCoolingCoil(const CoilCoolingDXVariableRefrigerantFlow& component) {
      return setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoil, component.handle());
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setHeatingCoil(const CoilHeatingDXVariableRefrigerantFlow& component) {
      return setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoil, component.handle());
    }

    boost::optional<HVACComponent> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplementalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilName);
    }

    boost::optional<ModelObject> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplementalHeatingCoilAsModelObject() const {
      OptionalModelObject result;
      if (boost::optional<HVACComponent> _coil = supplementalHeatingCoil()) {
        result = _coil.get();
      }
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplementalHeatingCoil(const HVACComponent& coil) {
      bool result = setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilName, coil.handle());
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
        if (intermediate) {
          HVACComponent heatingCoilName(*intermediate);
          setSupplementalHeatingCoil(heatingCoilName);
          return true;
        }
      }
      return false;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::resetSupplementalHeatingCoil() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilName, "");
      OS_ASSERT(result);
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::maximumSupplyAirTemperaturefromSupplementalHeater() const {
      boost::optional<double> value =
        getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, false);
      return value;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(
      double maximumSupplyAirTemperaturefromSupplementalHeater) {
      bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater,
                              maximumSupplyAirTemperaturefromSupplementalHeater);
      // OS_ASSERT(result);
      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
      OS_ASSERT(result);
    }

    double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
      bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
      // OS_ASSERT(result);
      return result;
    }

    std::string ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFanPlacement() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement) {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement, supplyAirFanPlacement);
      // OS_ASSERT(result);
      return result;
    }

    boost::optional<ThermalZone> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::controllingZoneorThermostatLocation() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(
        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ControllingZoneorThermostatLocation);
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setControllingZoneorThermostatLocation(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ControllingZoneorThermostatLocation, thermalZone.handle());
      return result;
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::resetControllingZoneorThermostatLocation() {
      bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ControllingZoneorThermostatLocation, "");
      OS_ASSERT(result);
    }

    ModelObject ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::clone(Model model) const {
      ModelObject terminalClone = ZoneHVACComponent_Impl::clone(model);

      if (auto fan = supplyAirFan()) {
        HVACComponent fanClone = fan->clone(model).cast<HVACComponent>();
        terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFan(fanClone);
      }

      if (auto coil = coolingCoil()) {
        auto coilClone = coil->clone(model).cast<CoilCoolingDXVariableRefrigerantFlow>();
        terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setCoolingCoil(coilClone);
      }

      if (auto coil = heatingCoil()) {
        auto coilClone = coil->clone(model).cast<CoilHeatingDXVariableRefrigerantFlow>();
        terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setHeatingCoil(coilClone);
      }

      if (auto coil = supplementalHeatingCoil()) {
        auto coilClone = coil->clone(model).cast<HVACComponent>();
        terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplementalHeatingCoil(coilClone);
      }

      // TODO Move this into base clase
      terminalClone.setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNode, "");
      terminalClone.setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNode, "");

      return terminalClone;
    }

    std::vector<ModelObject> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::children() const {
      std::vector<ModelObject> result;

      if (auto fan = supplyAirFan()) {
        result.push_back(fan.get());
      }

      if (auto coil = coolingCoil()) {
        result.push_back(coil.get());
      }

      if (auto coil = heatingCoil()) {
        result.push_back(coil.get());
      }

      if (auto coil = supplementalHeatingCoil()) {
        result.push_back(coil.get());
      }

      return result;
    }

    bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::addToNode(Node& node) {
      // Try connecting as a (classic) ZoneHVACComponent
      bool result = ZoneHVACComponent_Impl::addToNode(node);

      // If that fails, check if it's the supply side of an AirLoopHVAC or on an AirLoopHVACOutdoorAirSystem
      if (!result) {
        auto _model = node.model();
        auto thisModelObject = getObject<ModelObject>();
        auto t_airLoop = node.airLoopHVAC();
        auto t_oaSystem = node.airLoopHVACOutdoorAirSystem();

        boost::optional<unsigned> componentInletPort;
        boost::optional<unsigned> componentOutletPort;

        boost::optional<HVACComponent> systemStartComponent;
        boost::optional<HVACComponent> systemEndComponent;

        if (node.getImpl<Node_Impl>()->isConnected(thisModelObject)) {
          return false;
        }

        if (t_airLoop && !t_oaSystem) {
          if (t_airLoop->demandComponent(node.handle())) {
            return false;
          }

          systemStartComponent = t_airLoop->supplyInletNode();
          auto nodes = t_airLoop->supplyOutletNodes();
          OS_ASSERT(!nodes.empty());
          if ((nodes.size() == 2u) && (!t_airLoop->supplyComponents(node, nodes[1]).empty())) {
            systemEndComponent = nodes[1];
          } else {
            systemEndComponent = nodes[0];
          }
          OS_ASSERT(systemEndComponent);
          componentInletPort = inletPort();
          componentOutletPort = outletPort();

          removeFromAirLoopHVAC();
        } else if (t_oaSystem) {
          if (t_oaSystem->oaComponent(node.handle())) {
            systemStartComponent = t_oaSystem->outboardOANode();
            systemEndComponent = t_oaSystem.get();
            componentInletPort = inletPort();
            componentOutletPort = outletPort();
          } else if (t_oaSystem->reliefComponent(node.handle())) {
            systemStartComponent = t_oaSystem.get();
            systemEndComponent = t_oaSystem->outboardReliefNode();
            componentInletPort = inletPort();
            componentOutletPort = outletPort();
          }
          removeFromAirLoopHVAC();
        }

        if (systemStartComponent && systemEndComponent && componentOutletPort && componentInletPort) {
          result = HVACComponent_Impl::addToNode(node, systemStartComponent.get(), systemEndComponent.get(), componentInletPort.get(),
                                                 componentOutletPort.get());
        }
      }

      return result;
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Design Size Cooling Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedSupplyAirFlowRateWhenNoCoolingisNeeded() const {
      return getAutosizedValue("Design Size No Cooling Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Design Size Heating Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedSupplyAirFlowRateWhenNoHeatingisNeeded() const {
      return getAutosizedValue("Design Size No Heating Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedOutdoorAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Design Size Outdoor Air Flow Rate During Cooling Operation", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedOutdoorAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Design Size Outdoor Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("Design Size Outdoor Air Flow Rate When No Cooling or Heating is Needed", "m3/s");
    }

    boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const {
      return getAutosizedValue("Design Size Maximum Supply Air Temperature from Supplemental Heater", "C");
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosize() {
      autosizeSupplyAirFlowRateDuringCoolingOperation();
      autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();
      autosizeSupplyAirFlowRateDuringHeatingOperation();
      autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();
      autosizeOutdoorAirFlowRateDuringCoolingOperation();
      autosizeOutdoorAirFlowRateDuringHeatingOperation();
      autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
      autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    }

    void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedSupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSupplyAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSupplyAirFlowRateWhenNoCoolingisNeeded();
      if (val) {
        setSupplyAirFlowRateWhenNoCoolingisNeeded(val.get());
      }

      val = autosizedSupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSupplyAirFlowRateWhenNoHeatingisNeeded();
      if (val) {
        setSupplyAirFlowRateWhenNoHeatingisNeeded(val.get());
      }

      val = autosizedOutdoorAirFlowRateDuringCoolingOperation();
      if (val) {
        setOutdoorAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedOutdoorAirFlowRateDuringHeatingOperation();
      if (val) {
        setOutdoorAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
      if (val) {
        setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(val.get());
      }

      val = autosizedMaximumSupplyAirTemperaturefromSupplementalHeater();
      if (val) {
        setMaximumSupplyAirTemperaturefromSupplementalHeater(val.get());
      }
    }

    std::vector<EMSActuatorNames> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Variable Refrigerant Flow Terminal Unit", "Part Load Ratio"}};
      return actuators;
    }

    std::vector<std::string> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

  }  // namespace detail

  ZoneHVACTerminalUnitVariableRefrigerantFlow::ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model)
    : ZoneHVACComponent(ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>());

    Schedule alwaysOnSchedule = model.alwaysOnDiscreteSchedule();
    setTerminalUnitAvailabilityschedule(alwaysOnSchedule);

    autosizeSupplyAirFlowRateDuringCoolingOperation();

    autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

    autosizeSupplyAirFlowRateDuringHeatingOperation();

    autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

    autosizeOutdoorAirFlowRateDuringCoolingOperation();

    autosizeOutdoorAirFlowRateDuringHeatingOperation();

    autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    setSupplyAirFanOperatingModeSchedule(alwaysOnSchedule);

    setZoneTerminalUnitOnParasiticElectricEnergyUse(30);

    setZoneTerminalUnitOffParasiticElectricEnergyUse(20);

    setRatedTotalHeatingCapacitySizingRatio(1.0);

    autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(21.0);

    setSupplyAirFanPlacement("DrawThrough");

    CoilCoolingDXVariableRefrigerantFlow coolingCoil(model);
    coolingCoil.setName(name().get() + " Cooling Coil");
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setCoolingCoil(coolingCoil);

    CoilHeatingDXVariableRefrigerantFlow heatingCoil(model);
    heatingCoil.setName(name().get() + " Heating Coil");
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setHeatingCoil(heatingCoil);

    FanOnOff fan(model, alwaysOnSchedule);
    fan.setName(name().get() + " Fan");
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFan(fan);
  }

  ZoneHVACTerminalUnitVariableRefrigerantFlow::ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model,
                                                                                           const CoilCoolingDXVariableRefrigerantFlow& coolingCoil,
                                                                                           const CoilHeatingDXVariableRefrigerantFlow& heatingCoil,
                                                                                           const HVACComponent& fan)
    : ZoneHVACComponent(ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>());

    Schedule alwaysOnSchedule = model.alwaysOnDiscreteSchedule();
    setTerminalUnitAvailabilityschedule(alwaysOnSchedule);

    autosizeSupplyAirFlowRateDuringCoolingOperation();

    autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

    autosizeSupplyAirFlowRateDuringHeatingOperation();

    autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

    autosizeOutdoorAirFlowRateDuringCoolingOperation();

    autosizeOutdoorAirFlowRateDuringHeatingOperation();

    autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    setSupplyAirFanOperatingModeSchedule(alwaysOnSchedule);

    setZoneTerminalUnitOnParasiticElectricEnergyUse(30);

    setZoneTerminalUnitOffParasiticElectricEnergyUse(20);

    setRatedTotalHeatingCapacitySizingRatio(1.0);

    autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(21.0);

    setSupplyAirFanPlacement("DrawThrough");

    bool ok = setCoolingCoil(coolingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Cooling Coil to " << coolingCoil.briefDescription() << ".");
    }

    ok = setHeatingCoil(heatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Heating Coil to " << fan.briefDescription() << ".");
    }

    ok = getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFan(fan);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Supply Air Fan to " << fan.briefDescription() << ".");
    }
  }

  IddObjectType ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
  }

  std::vector<std::string> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement);
  }

  Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow::terminalUnitAvailabilityschedule() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->terminalUnitAvailabilityschedule();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateWhenNoCoolingisNeeded() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateWhenNoCoolingisNeeded();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateWhenNoCoolingisNeededAutosized();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateWhenNoHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateWhenNoHeatingisNeeded();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateWhenNoHeatingisNeededAutosized();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::outdoorAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->outdoorAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isOutdoorAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::outdoorAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->outdoorAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isOutdoorAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
  }

  Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFanOperatingModeSchedule() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFanOperatingModeSchedule();
  }

  HVACComponent ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFan() const {
    boost::optional<HVACComponent> fan_ = getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFan();
    if (!fan_) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
    }
    return fan_.get();
  }

  boost::optional<CoilCoolingDXVariableRefrigerantFlow> ZoneHVACTerminalUnitVariableRefrigerantFlow::coolingCoil() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->coolingCoil();
  }

  boost::optional<CoilHeatingDXVariableRefrigerantFlow> ZoneHVACTerminalUnitVariableRefrigerantFlow::heatingCoil() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->heatingCoil();
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow::zoneTerminalUnitOnParasiticElectricEnergyUse() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->zoneTerminalUnitOnParasiticElectricEnergyUse();
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow::zoneTerminalUnitOffParasiticElectricEnergyUse() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->zoneTerminalUnitOffParasiticElectricEnergyUse();
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow::ratedTotalHeatingCapacitySizingRatio() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->ratedTotalHeatingCapacitySizingRatio();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setTerminalUnitAvailabilityschedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setTerminalUnitAvailabilityschedule(schedule);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(
      supplyAirFlowRateDuringCoolingOperation);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateWhenNoCoolingisNeeded(double supplyAirFlowRateWhenNoCoolingisNeeded) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateWhenNoCoolingisNeeded(
      supplyAirFlowRateWhenNoCoolingisNeeded);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateWhenNoCoolingisNeeded() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(
      supplyAirFlowRateDuringHeatingOperation);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateWhenNoHeatingisNeeded(double supplyAirFlowRateWhenNoHeatingisNeeded) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateWhenNoHeatingisNeeded(
      supplyAirFlowRateWhenNoHeatingisNeeded);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateWhenNoHeatingisNeeded() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(
      outdoorAirFlowRateDuringCoolingOperation);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(
      outdoorAirFlowRateDuringHeatingOperation);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
    double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
      outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setZoneTerminalUnitOnParasiticElectricEnergyUse(
    double zoneTerminalUnitOnParasiticElectricEnergyUse) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setZoneTerminalUnitOnParasiticElectricEnergyUse(
      zoneTerminalUnitOnParasiticElectricEnergyUse);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setZoneTerminalUnitOffParasiticElectricEnergyUse(
    double zoneTerminalUnitOffParasiticElectricEnergyUse) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setZoneTerminalUnitOffParasiticElectricEnergyUse(
      zoneTerminalUnitOffParasiticElectricEnergyUse);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacitySizingRatio(
      ratedTotalHeatingCapacitySizingRatio);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setCoolingCoil(const CoilCoolingDXVariableRefrigerantFlow& coil) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setCoolingCoil(coil);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setHeatingCoil(const CoilHeatingDXVariableRefrigerantFlow& coil) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setHeatingCoil(coil);
  }

  boost::optional<HVACComponent> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplementalHeatingCoil() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplementalHeatingCoil();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplementalHeatingCoil(const HVACComponent& coil) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplementalHeatingCoil(coil);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::resetSupplementalHeatingCoil() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->resetSupplementalHeatingCoil();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::maximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->maximumSupplyAirTemperaturefromSupplementalHeater();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setMaximumSupplyAirTemperaturefromSupplementalHeater(
    double maximumSupplyAirTemperaturefromSupplementalHeater) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setMaximumSupplyAirTemperaturefromSupplementalHeater(
      maximumSupplyAirTemperaturefromSupplementalHeater);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized();
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  }

  boost::optional<ThermalZone> ZoneHVACTerminalUnitVariableRefrigerantFlow::controllingZoneorThermostatLocation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->controllingZoneorThermostatLocation();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setControllingZoneorThermostatLocation(const ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setControllingZoneorThermostatLocation(thermalZone);
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow::resetControllingZoneorThermostatLocation() {
    getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->resetControllingZoneorThermostatLocation();
  }

  std::string ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFanPlacement() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFanPlacement();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement) {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFanPlacement(supplyAirFanPlacement);
  }

  /// @cond
  ZoneHVACTerminalUnitVariableRefrigerantFlow::ZoneHVACTerminalUnitVariableRefrigerantFlow(
    std::shared_ptr<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedSupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedSupplyAirFlowRateWhenNoCoolingisNeeded() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedSupplyAirFlowRateWhenNoCoolingisNeeded();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedSupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedSupplyAirFlowRateWhenNoHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedSupplyAirFlowRateWhenNoHeatingisNeeded();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedOutdoorAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedOutdoorAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedOutdoorAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedOutdoorAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizedMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

}  // namespace model
}  // namespace openstudio
