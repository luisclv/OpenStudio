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

#ifndef MODEL_LUMINAIRE_IMPL_HPP
#define MODEL_LUMINAIRE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  class Schedule;
  class LuminaireDefinition;
  class Luminaire;

  namespace detail {

    /** Luminaire_Impl is a SpaceLoadInstance_Impl that is the implementation class for Luminaire.*/
    class MODEL_API Luminaire_Impl : public SpaceLoadInstance_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Luminaire_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Luminaire_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Luminaire_Impl(const Luminaire_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Luminaire_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;

      //@}
      /** @name Getters */
      //@{

      LuminaireDefinition luminaireDefinition() const;

      boost::optional<Schedule> schedule() const;

      bool isScheduleDefaulted() const;

      double positionXcoordinate() const;

      double positionYcoordinate() const;

      double positionZcoordinate() const;

      double psiRotationAroundXaxis() const;

      bool isPsiRotationAroundXaxisDefaulted() const;

      double thetaRotationAroundYaxis() const;

      bool isThetaRotationAroundYaxisDefaulted() const;

      double phiRotationAroundZaxis() const;

      bool isPhiRotationAroundZaxisDefaulted() const;

      double fractionReplaceable() const;

      bool isFractionReplaceableDefaulted() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setLuminaireDefinition(const LuminaireDefinition& definition);

      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      bool setSchedule(Schedule& schedule);

      void resetSchedule();

      bool setPositionXcoordinate(double positionXcoordinate);

      bool setPositionYcoordinate(double positionYcoordinate);

      bool setPositionZcoordinate(double positionZcoordinate);

      bool setPsiRotationAroundXaxis(double psiRotationAroundXaxis);

      void resetPsiRotationAroundXaxis();

      bool setThetaRotationAroundYaxis(double thetaRotationAroundYaxis);

      void resetThetaRotationAroundYaxis();

      bool setPhiRotationAroundZaxis(double phiRotationAroundZaxis);

      void resetPhiRotationAroundZaxis();

      bool setFractionReplaceable(double fractionReplaceable);

      void resetFractionReplaceable();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      bool setEndUseSubcategory(std::string endUseSubcategory);

      void resetEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      openstudio::Point3d position() const;

      bool setPosition(const openstudio::Point3d& position);

      openstudio::Transformation transformation() const;

      bool setTransformation(const openstudio::Transformation& transformation);

      double lightingPower() const;

      double getPowerPerFloorArea(double floorArea) const;

      double getPowerPerPerson(double numPeople) const;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Luminaire");

      boost::optional<ModelObject> luminaireDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setLuminaireDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LUMINAIRE_IMPL_HPP
