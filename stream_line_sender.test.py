import unittest
import stream_line_sender as sls
import stream_line_sender_constants as slsc


class TypewiseTest(unittest.TestCase):

  def test_check_and_send(self):
    for coolingType in slsc.COOLING_TYPE:
        self.assertTrue(
          sls.check_and_send(
            {'coolingType': coolingType},
            80
          ) == True
        )


if __name__ == '__main__':
  unittest.main()
