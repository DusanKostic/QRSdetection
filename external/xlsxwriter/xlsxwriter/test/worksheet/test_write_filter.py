###############################################################################
#
# Tests for XlsxWriter.
#
# Copyright (c), 2013, John McNamara, jmcnamara@cpan.org
#

import unittest
from ..compatibility import StringIO
from ...worksheet import Worksheet


class TestWriteFilter(unittest.TestCase):
    """
    Test the Worksheet _write_filter() method.

    """

    def setUp(self):
        self.fh = StringIO()
        self.worksheet = Worksheet()
        self.worksheet._set_filehandle(self.fh)

    def test_write_filter(self):
        """Test the _write_filter() method"""

        self.worksheet._write_filter('East')

        exp = """<filter val="East"/>"""
        got = self.fh.getvalue()

        self.assertEqual(got, exp)


if __name__ == '__main__':
    unittest.main()
