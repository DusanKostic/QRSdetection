###############################################################################
#
# Tests for XlsxWriter.
#
# Copyright (c), 2013, John McNamara, jmcnamara@cpan.org
#

import unittest
import os
from ...workbook import Workbook
from ..helperfunctions import _compare_xlsx_files


class TestCompareXLSXFiles(unittest.TestCase):
    """
    Test file created by XlsxWriter against a file created by Excel.

    """

    def setUp(self):
        self.maxDiff = None

        filename = 'comment05.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename

        # It takes about 50 times longer to run this test with this file
        # included. Only turn it on for pre-release testing.
        self.ignore_files = ['xl/drawings/vmlDrawing1.vml']
        self.ignore_elements = {}

    def test_create_file(self):
        """
        Test the creation of a simple XlsxWriter file with comments.
        Test the VML data and shape ids for blocks of comments > 1024.
        """
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)

        worksheet1 = workbook.add_worksheet()
        worksheet2 = workbook.add_worksheet()
        worksheet3 = workbook.add_worksheet()

        for row in range(0, 127 + 1):
            for col in range(0, 15 + 1):
                worksheet1.write_comment(row, col, 'Some text')

        worksheet3.write_comment('A1', 'More text')

        worksheet1.set_comments_author('John')
        worksheet3.set_comments_author('John')

        workbook.close()

        ####################################################

        got, exp = _compare_xlsx_files(self.got_filename,
                                       self.exp_filename,
                                       self.ignore_files,
                                       self.ignore_elements)

        self.assertEqual(got, exp)

    def tearDown(self):
        # Cleanup.
        if os.path.exists(self.got_filename):
            os.remove(self.got_filename)


if __name__ == '__main__':
    unittest.main()
