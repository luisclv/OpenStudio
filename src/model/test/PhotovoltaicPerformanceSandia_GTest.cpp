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

#include "ModelFixture.hpp"

#include "../PhotovoltaicPerformanceSandia.hpp"
#include "../PhotovoltaicPerformanceSandia_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PhotovoltaicPerformanceSandia_GettersSetters) {
  Model m;
  PhotovoltaicPerformanceSandia photovoltaicPerformanceSandia(m);

  photovoltaicPerformanceSandia.setName("My PhotovoltaicPerformanceSandia");

  // Active Area: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setActiveArea(0.1));
  EXPECT_EQ(0.1, photovoltaicPerformanceSandia.activeArea());
  // Bad Value
  EXPECT_FALSE(photovoltaicPerformanceSandia.setActiveArea(-10.0));
  EXPECT_EQ(0.1, photovoltaicPerformanceSandia.activeArea());

  // Number of Cells in Series: Required Integer
  EXPECT_TRUE(photovoltaicPerformanceSandia.setNumberofCellsinSeries(2));
  EXPECT_EQ(2, photovoltaicPerformanceSandia.numberofCellsinSeries());
  // Bad Value
  EXPECT_FALSE(photovoltaicPerformanceSandia.setNumberofCellsinSeries(-9));
  EXPECT_EQ(2, photovoltaicPerformanceSandia.numberofCellsinSeries());

  // Number of Cells in Parallel: Required Integer
  EXPECT_TRUE(photovoltaicPerformanceSandia.setNumberofCellsinParallel(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.numberofCellsinParallel());
  // Bad Value
  EXPECT_FALSE(photovoltaicPerformanceSandia.setNumberofCellsinParallel(-9));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.numberofCellsinParallel());

  // Short Circuit Current: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setShortCircuitCurrent(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.shortCircuitCurrent());

  // Open Circuit Voltage: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setOpenCircuitVoltage(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.openCircuitVoltage());

  // Current at Maximum Power Point: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setCurrentatMaximumPowerPoint(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.currentatMaximumPowerPoint());

  // Voltage at Maximum Power Point: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setVoltageatMaximumPowerPoint(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.voltageatMaximumPowerPoint());

  // Sandia Database Parameter aIsc: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameteraIsc(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameteraIsc());

  // Sandia Database Parameter aImp: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameteraImp(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameteraImp());

  // Sandia Database Parameter c0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc0());

  // Sandia Database Parameter c1: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc1(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc1());

  // Sandia Database Parameter BVoc0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterBVoc0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterBVoc0());

  // Sandia Database Parameter mBVoc: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametermBVoc(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametermBVoc());

  // Sandia Database Parameter BVmp0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterBVmp0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterBVmp0());

  // Sandia Database Parameter mBVmp: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametermBVmp(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametermBVmp());

  // Diode Factor: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setDiodeFactor(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.diodeFactor());

  // Sandia Database Parameter c2: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc2(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc2());

  // Sandia Database Parameter c3: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc3(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc3());

  // Sandia Database Parameter a0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametera0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametera0());

  // Sandia Database Parameter a1: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametera1(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametera1());

  // Sandia Database Parameter a2: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametera2(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametera2());

  // Sandia Database Parameter a3: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametera3(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametera3());

  // Sandia Database Parameter a4: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametera4(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametera4());

  // Sandia Database Parameter b0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb0());

  // Sandia Database Parameter b1: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb1(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb1());

  // Sandia Database Parameter b2: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb2(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb2());

  // Sandia Database Parameter b3: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb3(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb3());

  // Sandia Database Parameter b4: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb4(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb4());

  // Sandia Database Parameter b5: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb5(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb5());

  // Sandia Database Parameter Delta(Tc): Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterDeltaTc(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterDeltaTc());

  // Sandia Database Parameter fd: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterfd(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterfd());

  // Sandia Database Parameter a: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParametera(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParametera());

  // Sandia Database Parameter b: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterb(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterb());

  // Sandia Database Parameter c4: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc4(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc4());

  // Sandia Database Parameter c5: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc5(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc5());

  // Sandia Database Parameter Ix0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterIx0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterIx0());

  // Sandia Database Parameter Ixx0: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterIxx0(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterIxx0());

  // Sandia Database Parameter c6: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc6(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc6());

  // Sandia Database Parameter c7: Required Double
  EXPECT_TRUE(photovoltaicPerformanceSandia.setSandiaDatabaseParameterc7(3));
  EXPECT_EQ(3, photovoltaicPerformanceSandia.sandiaDatabaseParameterc7());
}

TEST_F(ModelFixture, PhotovoltaicPerformanceSandia_FactoryMethod) {
  Model m;

  std::vector<std::string> perfNames = PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames();
  EXPECT_GT(perfNames.size(), 0);

  std::string perfName = perfNames[0];
  EXPECT_NO_THROW(PhotovoltaicPerformanceSandia perf = PhotovoltaicPerformanceSandia::fromSandiaDatabase(m, perfName));
}
