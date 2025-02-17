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

#include "../CurveQuintLinear.hpp"
#include "../CurveQuintLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveQuintLinear_GettersSetters) {
  Model m;

  CurveQuintLinear curveQuintLinear(m);

  curveQuintLinear.setName("My CurveQuintLinear");

  // Coefficient1 Constant: Required Double
  EXPECT_TRUE(curveQuintLinear.setCoefficient1Constant(0.1));
  EXPECT_EQ(0.1, curveQuintLinear.coefficient1Constant());

  // Coefficient2 v: Required Double
  EXPECT_TRUE(curveQuintLinear.setCoefficient2v(0.2));
  EXPECT_EQ(0.2, curveQuintLinear.coefficient2v());

  // Coefficient3 w: Required Double
  EXPECT_TRUE(curveQuintLinear.setCoefficient3w(0.3));
  EXPECT_EQ(0.3, curveQuintLinear.coefficient3w());

  // Coefficient4 x: Required Double
  EXPECT_TRUE(curveQuintLinear.setCoefficient4x(0.4));
  EXPECT_EQ(0.4, curveQuintLinear.coefficient4x());

  // Coefficient5 y: Required Double
  EXPECT_TRUE(curveQuintLinear.setCoefficient5y(0.5));
  EXPECT_EQ(0.5, curveQuintLinear.coefficient5y());

  // Coefficient6 z: Required Double
  EXPECT_TRUE(curveQuintLinear.setCoefficient6z(0.6));
  EXPECT_EQ(0.6, curveQuintLinear.coefficient6z());

  // Minimum Value of v: Required Double
  EXPECT_TRUE(curveQuintLinear.setMinimumValueofv(1.0));
  EXPECT_EQ(1.0, curveQuintLinear.minimumValueofv());

  // Maximum Value of v: Required Double
  EXPECT_TRUE(curveQuintLinear.setMaximumValueofv(11.0));
  EXPECT_EQ(11.0, curveQuintLinear.maximumValueofv());

  // Minimum Value of w: Required Double
  EXPECT_TRUE(curveQuintLinear.setMinimumValueofw(2.0));
  EXPECT_EQ(2.0, curveQuintLinear.minimumValueofw());

  // Maximum Value of w: Required Double
  EXPECT_TRUE(curveQuintLinear.setMaximumValueofw(12.0));
  EXPECT_EQ(12.0, curveQuintLinear.maximumValueofw());

  // Minimum Value of x: Required Double
  EXPECT_TRUE(curveQuintLinear.setMinimumValueofx(3.0));
  EXPECT_EQ(3.0, curveQuintLinear.minimumValueofx());

  // Maximum Value of x: Required Double
  EXPECT_TRUE(curveQuintLinear.setMaximumValueofx(13.0));
  EXPECT_EQ(13.0, curveQuintLinear.maximumValueofx());

  // Minimum Value of y: Required Double
  EXPECT_TRUE(curveQuintLinear.setMinimumValueofy(4.0));
  EXPECT_EQ(4.0, curveQuintLinear.minimumValueofy());

  // Maximum Value of y: Required Double
  EXPECT_TRUE(curveQuintLinear.setMaximumValueofy(14.0));
  EXPECT_EQ(14.0, curveQuintLinear.maximumValueofy());

  // Minimum Value of z: Required Double
  EXPECT_TRUE(curveQuintLinear.setMinimumValueofz(5.0));
  EXPECT_EQ(5.0, curveQuintLinear.minimumValueofz());

  // Maximum Value of z: Required Double
  EXPECT_TRUE(curveQuintLinear.setMaximumValueofz(15.0));
  EXPECT_EQ(15.0, curveQuintLinear.maximumValueofz());

  // Minimum Curve Output: Optional Double
  EXPECT_TRUE(curveQuintLinear.setMinimumCurveOutput(0.03));
  ASSERT_TRUE(curveQuintLinear.minimumCurveOutput());
  EXPECT_EQ(0.03, curveQuintLinear.minimumCurveOutput().get());

  // Maximum Curve Output: Optional Double
  EXPECT_TRUE(curveQuintLinear.setMaximumCurveOutput(0.98));
  ASSERT_TRUE(curveQuintLinear.maximumCurveOutput());
  EXPECT_EQ(0.98, curveQuintLinear.maximumCurveOutput().get());

  // Input Unit Type for v: Optional String
  EXPECT_TRUE(curveQuintLinear.setInputUnitTypeforv("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforv());
  // Bad Value
  EXPECT_FALSE(curveQuintLinear.setInputUnitTypeforv("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforv());

  // Input Unit Type for w: Optional String
  EXPECT_TRUE(curveQuintLinear.setInputUnitTypeforw("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforw());
  // Bad Value
  EXPECT_FALSE(curveQuintLinear.setInputUnitTypeforw("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforw());

  // Input Unit Type for x: Optional String
  EXPECT_TRUE(curveQuintLinear.setInputUnitTypeforx("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforx());
  // Bad Value
  EXPECT_FALSE(curveQuintLinear.setInputUnitTypeforx("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforx());

  // Input Unit Type for y: Optional String
  EXPECT_TRUE(curveQuintLinear.setInputUnitTypefory("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypefory());
  // Bad Value
  EXPECT_FALSE(curveQuintLinear.setInputUnitTypefory("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypefory());

  // Input Unit Type for z: Optional String
  EXPECT_TRUE(curveQuintLinear.setInputUnitTypeforz("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforz());
  // Bad Value
  EXPECT_FALSE(curveQuintLinear.setInputUnitTypeforz("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuintLinear.inputUnitTypeforz());
}

TEST_F(ModelFixture, CurveQuintLinear_evaluate) {
  Model m;

  CurveQuintLinear c(m);
  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double c5 = 5.0;
  double c6 = 6.0;
  c.setCoefficient1Constant(c1);
  c.setCoefficient2v(c2);
  c.setCoefficient3w(c3);
  c.setCoefficient4x(c4);
  c.setCoefficient5y(c5);
  c.setCoefficient6z(c6);

  double v = 0.1;
  double w = 0.2;
  double x = 0.3;
  double y = 0.4;
  double z = 0.5;

  double result = c.evaluate(v, w, x, y, z);
  double expectedResult = c1 + c2 * v + c3 * w + c4 * x + c5 * y + c6 * z;
  EXPECT_EQ(expectedResult, result);
}
