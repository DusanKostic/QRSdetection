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

        filename = 'cond_format07.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename

        self.ignore_files = []
        self.ignore_elements = {}

    def test_create_file(self):
        """Test the creation of a simple XlsxWriter file with conditional formatting."""
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)

        worksheet = workbook.add_worksheet()

        format1 = workbook.add_format({'bg_color': '#FF0000', 'dxf_index': 1})
        format2 = workbook.add_format({'bg_color': '#92D050', 'dxf_index': 0})

        data = [
            [90, 80, 50, 10, 20, 90, 40, 90, 30, 40],
            [20, 10, 90, 100, 30, 60, 70, 60, 50, 90],
            [10, 50, 60, 50, 20, 50, 80, 30, 40, 60],
            [10, 90, 20, 40, 10, 40, 50, 70, 90, 50],
            [70, 100, 10, 90, 10, 10, 20, 100, 100, 40],
            [20, 60, 10, 100, 30, 10, 20, 60, 100, 10],
            [10, 60, 10, 80, 100, 80, 30, 30, 70, 40],
            [30, 90, 60, 10, 10, 100, 40, 40, 30, 40],
            [80, 90, 10, 20, 20, 50, 80, 20, 60, 90],
            [60, 80, 30, 30, 10, 50, 80, 60, 50, 30],
        ]

        for row, row_data in enumerate(data):
            worksheet.write_row(row, 0, row_data)
            row += 1

        worksheet.conditional_format('A1:J10',
                                     {'type': 'cell',
                                      'format': format1,
                                      'criteria': '>=',
                                      'value': 50,
                                      })

        worksheet.conditional_format('A1:J10',
                                     {'type': 'cell',
                                      'format': format2,
                                      'criteria': '<',
                                      'value': 50,
                                      })

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
