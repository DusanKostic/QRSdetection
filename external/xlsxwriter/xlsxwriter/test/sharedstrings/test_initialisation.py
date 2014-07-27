###############################################################################
#
# Tests for XlsxWriter.
#
# Copyright (c), 2013, John McNamara, jmcnamara@cpan.org
#

import unittest
from ..compatibility import StringIO
from ...sharedstrings import SharedStrings


class TestInitialisation(unittest.TestCase):
    """
    Test initialisation of the SharedStrings class and call a method.

    """

    def setUp(self):
        self.fh = StringIO()
        self.sharedstrings = SharedStrings()
        self.sharedstrings._set_filehandle(self.fh)

    def test_xml_declaration(self):
        """Test Sharedstrings xml_declaration()"""

        self.sharedstrings._xml_declaration()

        exp = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n"""
        got = self.fh.getvalue()

        self.assertEqual(got, exp)


if __name__ == '__main__':
    unittest.main()
