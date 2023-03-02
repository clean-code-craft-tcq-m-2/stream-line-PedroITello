import unittest
import stream_line_sender as sls
import stream_line_sender_constants as slsc
import random

class TypewiseTest(unittest.TestCase):

  def test_check_and_send(self):
    for x in range(50):
      random_index = random.randrange(3)
      self.assertTrue(
        sls.check_and_send(
          {'coolingType': slsc.COOLING_TYPE[random_index]},
          random.randrange(-1, 80)
        ) == True
      )


if __name__ == '__main__':
  unittest.main()
