###############################################################################
#
# ChartColumn - A class for writing the Excel XLSX Column charts.
#
# Copyright 2013, John McNamara, jmcnamara@cpan.org
#

from . import chart


class ChartColumn(chart.Chart):
    """
    A class for writing the Excel XLSX Column charts.


    """

    ###########################################################################
    #
    # Public API.
    #
    ###########################################################################

    def __init__(self, options=None):
        """
        Constructor.

        """
        super(ChartColumn, self).__init__()

        if options is None:
            options = {}

        self.subtype = options.get('subtype')

        if not self.subtype:
            self.subtype = 'clustered'

        self.horiz_val_axis = 0

        if self.subtype == 'percent_stacked':
            self.y_axis['defaults']['num_format'] = '0%'

        self.set_y_axis({})

    ###########################################################################
    #
    # Private API.
    #
    ###########################################################################

    def _write_chart_type(self, args):
        # Override the virtual superclass method with a chart specific method.

        # Write the c:barChart element.
        self._write_bar_chart(args)

    def _write_bar_chart(self, args):
        # Write the <c:barChart> element.

        if args['primary_axes']:
            series = self._get_primary_axes_series()
        else:
            series = self._get_secondary_axes_series()

        if not len(series):
            return

        subtype = self.subtype
        if subtype == 'percent_stacked':
            subtype = 'percentStacked'

        # Set a default overlap for stacked charts.
        if 'stacked' in self.subtype:
            if self.series_overlap is None:
                self.series_overlap = 100

        self._xml_start_tag('c:barChart')

        # Write the c:barDir element.
        self._write_bar_dir()

        # Write the c:grouping element.
        self._write_grouping(subtype)

        # Write the c:ser elements.
        for data in series:
            self._write_ser(data)

        # Write the c:marker element.
        self._write_marker_value()

        # Write the c:gapWidth element.
        self._write_gap_width(self.series_gap)

        # Write the c:overlap element.
        self._write_overlap(self.series_overlap)

        # Write the c:axId elements
        self._write_axis_ids(args)

        self._xml_end_tag('c:barChart')

    ###########################################################################
    #
    # XML methods.
    #
    ###########################################################################

    def _write_bar_dir(self):
        # Write the <c:barDir> element.
        val = 'col'

        attributes = [('val', val)]

        self._xml_empty_tag('c:barDir', attributes)

    def _write_err_dir(self, val):
        # Overridden from Chart class since it is not used in Column charts.
        pass
