########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class UUID_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_uuid_constuctors

    # created UUID is not null
    uuid = OpenStudio::UUID::create()
    assert(uuid)

    # copy UUID
    uuid2 = uuid
    assert(uuid2)
    assert_equal(uuid, uuid2)

    # to string
    s = uuid.to_s
    assert(s)
  end

  def test_uuid_big_set
    n = 1000
    nGroup = 3

    (0...nGroup).each do |group|
      assert(system("'#{OpenStudio::getOpenStudioCLI}' '#{File.join(File.dirname(__FILE__), 'UUID_BigSet_Aux.rb')}' '#{n}'"))
    end

    File.delete("UUIDSet.txt")
  end
  
  def test_uuid_hash
    model = OpenStudio::Model::Model.new
    space1 = OpenStudio::Model::Space.new(model)
    space1.setName("Space 1")
    space2 = OpenStudio::Model::Space.new(model)
    space2.setName("Space 2")
    
    space1_handle1 = space1.handle
    space1_handle2 = space1.handle
    space2_handle1 = space2.handle
    space2_handle2 = space2.handle
    
    assert(space1_handle1 == space1_handle1)
    assert(space1_handle1 == space1_handle2)
    assert(space1_handle1 != space2_handle1)
    assert(space1_handle1 != space2_handle2)
    
    assert(space1_handle1.to_s == space1_handle1.to_s)
    assert(space1_handle1.to_s == space1_handle2.to_s)
    assert(space1_handle1.to_s != space2_handle1.to_s)
    assert(space1_handle1.to_s != space2_handle2.to_s)
    
    assert(space1_handle1.hash == space1_handle1.hash)
    assert(space1_handle1.hash == space1_handle2.hash)
    assert(space1_handle1.hash != space2_handle1.hash)
    assert(space1_handle1.hash != space2_handle2.hash)
    
    assert(space1_handle1.eql?(space1_handle1))
    assert(space1_handle1.eql?(space1_handle2))
    assert(!space1_handle1.eql?(space2_handle1))
    assert(!space1_handle1.eql?(space2_handle2))
    
    assert(space1_handle1.to_s != space1_handle1)
    assert(space1_handle1.to_s != space1_handle2)
    assert(space1_handle1.to_s != space2_handle1)
    assert(space1_handle1.to_s != space2_handle2)
    
    handle_to_space_map = {}
    handle_to_space_map[space1_handle1] = space1
    handle_to_space_map[space2_handle1] = space2
    assert(handle_to_space_map[space1_handle1] == space1)
    assert(handle_to_space_map[space1_handle2] == space1) 
    assert(handle_to_space_map[space2_handle1] == space2)
    assert(handle_to_space_map[space2_handle2] == space2) 
    assert(handle_to_space_map[space1_handle1.to_s] != space1)
    assert(handle_to_space_map[space1_handle2.to_s] != space1)
    assert(handle_to_space_map[space2_handle1.to_s] != space2)
    assert(handle_to_space_map[space2_handle2.to_s] != space2)
    
    handle_str_to_space_map = {}
    handle_str_to_space_map[space1_handle1.to_s] = space1
    handle_str_to_space_map[space2_handle1.to_s] = space2
    assert(handle_str_to_space_map[space1_handle1.to_s] == space1)
    assert(handle_str_to_space_map[space1_handle2.to_s] == space1)
    assert(handle_str_to_space_map[space2_handle1.to_s] == space2)
    assert(handle_str_to_space_map[space2_handle2.to_s] == space2)
    assert(handle_str_to_space_map[space1_handle1] != space1)
    assert(handle_str_to_space_map[space1_handle2] != space1)
    assert(handle_str_to_space_map[space2_handle1] != space2)
    assert(handle_str_to_space_map[space2_handle2] != space2)
  end

end


