#!/usr/bin/env python

import sys
import getopt
from datetime import datetime, date, time, tzinfo
import re
import matplotlib.pyplot as plt

#------------------------------------------------------------------------------	
def usage():
    print
    print
    print "-h, --help                        : outputs help and quits"
    print "-o <path>                         : path to output file (if blank, stdout)"
    print "<path>                            : input file (if blank, stdin)"
    print "-p                                : plot (if blank, no plot is produced)"
    print

#------------------------------------------------------------------------------	
def handle_opt():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h:o:p", 
                                   ["help", "output", "plot"])
    except getopt.GetoptError, err:
        # Will print something like "option -a not recognized"
        print str(err)
        usage()
        sys.exit(2)
	
    # Default options
    plotFlag = False
    input_filename = ''
    output_filename = ''
    input_file = sys.stdin
    output_file = sys.stdout

    # Input stream/file
    if len (args) != 0:
        input_filename = args[0]

    # Handling
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o == "-o":
            output_filename = a
        elif o == "-p":
            plotFlag = True
        else:
            assert False, "Unhandled option"

    # Input file (bookign requests). That file is normally compressed
    # with GNU Zip (gzip)
    if (input_filename != ''):
	flag_gz = True
	if len (input_filename) < 2:
            flag_gz = False
	elif input_filename[-2:] != 'gz':
            flag_gz = False
	if flag_gz:
            input_file = gzip.open (input_filename, 'rb')
	else:
            input_file = open (input_filename, 'r')

    if (output_filename != ''):
        output_file = open (output_filename, 'w')

    #
    print "Produces a plot: " + str(plotFlag)
    print "Input stream/file: '" + input_filename + "'"
    print "Output stream/file: '" + output_filename + "'"
    return plotFlag, input_file, output_file

#------------------------------------------
def plot_simple(yVals):
    plt.plot(yVals,'b-o')
    plt.show()

#--------------------------------------------
def main():
    # Parse command options
    plotFlag, input_file, output_file = handle_opt()

    # Iterate while there is something to read from the file
    bkgIdx = 0

    # The O&D statistics holder is a dictionary made of:
    #  - a key, itself made of the origin, destination and flight departure date
    #  - the list of booking dates corresponding to all the booking requests
    OnDStatsHolder = {}

    # The O&D statistics object is a dictionary made of:
    #  - a key, itself made of the origin, destination and flight departure date
    #  - the cumulative number of booking requests made at the corresponding
    #    booking date for that key
    OnDStats = {}

    # Iterate while there are booking requests in the TraDemGen log file
    for line in input_file:
        # The lines of interest contain the "Poped" key-word
        matchedLine = re.search ("Poped", line)

        # If there is no match, the loop will continue on to the next line
        if not matchedLine:
            continue

        # There is a match. Now, the booking request information will
        # be extracted
        extractor = re.compile ("^.*(20[0-9]{2}-[A-Z][a-z]{2}-[0-9]{2}).*([A-Z]{3}-[A-Z]{3}).*(20[0-9]{2}-[A-Z][a-z]{2}-[0-9]{2}).*$")
        extractedList = extractor.search (line)

        # DEBUG
        #print ("1: " + extractedList.group(1) 
        #       + ", 2: " + extractedList.group(2)
        #       + ", 3: " + extractedList.group(3))

        #
        bkgIdx += 1

        # Booking date
        bookingDateStr = extractedList.group(1)
        bookingDate = datetime.strptime (bookingDateStr, "%Y-%b-%d")

        # O&D
        OnD = extractedList.group(2).split('-')
        origin = OnD[0]
        destination = OnD[1]

        # Flight departure date
        departureDateStr = extractedList.group(3)
        departureDate = datetime.strptime (departureDateStr, "%Y-%b-%d")

        # Create an entry key, made of the origin, destination and
        # flight departure date
        entryKey = (origin, destination, departureDate.strftime ("%Y-%m-%d"))
        if entryKey in OnDStatsHolder:
            # There are already entries for that key (i.e., origin, destination
            # and departure date). So, just add the new booking request
            # characteristics for that key.
            #
            # The cumulative number of booking requests is incremented
            # (as well as the corresponding list, used only to ease
            # later plotting).
            nbOfBkgs = OnDStats[entryKey][-1]
            OnDStats[entryKey].append (nbOfBkgs+1)
            OnDStatsHolder[entryKey].append (bookingDate.toordinal())

        else:
            # There is no entry yet for that key (i.e., origin,
            # destination, departure date). So, create a new entry
            # with the booking request characteristics for that key.
            OnDStats[entryKey] = [1]
            OnDStatsHolder[entryKey] = [bookingDate.toordinal()]

        # DEBUG
        #print "BookingRequest[" + str(bkgIdx) + "]: ", bookingRequest,"\n"

    # Browse through the (O&D, departure date) statistics
    for (OnDEntryKey, OnDEntryList) in OnDStatsHolder.iteritems():
        #
        output_file.write ("[" + str(OnDEntryKey) + "] " + str(OnDEntryList)
                           + "\n")
        output_file.write ("[" + str(OnDEntryKey) + "] " 
                           + str(OnDStats[OnDEntryKey]) + "\n")

        # Plot with MatplotLib
        if plotFlag:
            # Plot with solid line ('-') and blue ('b') circles ('o')
            plt.plot_date (sorted(OnDEntryList), OnDStats[OnDEntryKey], 'b-o')
            plt.show()

    #
    output_file.write ("\n")

#--------------------------------------------
if __name__ == "__main__":
    main()
