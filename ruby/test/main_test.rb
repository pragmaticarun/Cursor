require 'minitest/autorun'

class TestDemo < Minitest::Test
  def test_math
    assert_equal 4, 2 + 2
  end
end