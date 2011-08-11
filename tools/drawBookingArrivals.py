#!/usr/bin/env python

import sys
import getopt
from datetime import datetime, date, time, tzinfo
import matplotlib.pyplot as plt

#------------------------------------------------------------------------------	
def usage():
    print
    print
    print "-h, --help                        : outputs help and quits"
    print "-o <path>                         : path to output file (if blank, stdout)"
    print "<path>                            : input file (if blank, stdin)"
    print

#------------------------------------------------------------------------------	
def handle_opt():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h:o:", ["help", "output"])
    except getopt.GetoptError, err:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
	
    # Default options
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

    print "Input stream/file: '" + input_filename + "'"
    print "Output stream/file: '" + output_filename + "'"
    return input_file, output_file

#------------------------------------------
def plot_simple(yVals):
    plt.plot(yVals,'b-o')
    plt.show()

#--------------------------------------------
def main():
    # Parse command options
    input_file, output_file = handle_opt()

    # Iterate while there is something to read from the file
    bkgIdx = 0

    # Iterate while there are booking requests within the read chunk
    for line in input_file:
        bkgIdx += 1
        # Extract the booking request parameters
        bookingRequest = line.rstrip().split(',')

        # Booking date
        bookingDateStr = bookingRequest[1].lstrip()
        bookingDate = datetime.strptime (bookingDateStr, "%Y-%b-%d")

        # O&D
        OnD = bookingRequest[2].lstrip().split('-')
        origin = OnD[0]
        destination = OnD[1]

        # Flight departure date
        departureDateStr = bookingRequest[3].lstrip()
        departureDate = datetime.strptime (departureDateStr, "%Y-%b-%d")

        # DEBUG
        #print "BookingRequest[" + str(bkgIdx) + "]: ", bookingRequest,"\n"

        # Main output
        output_file.write (str(bkgIdx) + ", " 
                           + bookingDate.strftime("%Y-%b-%d") + ", "
                           + origin + "-" + destination + ", "
                           + departureDate.strftime("%Y-%b-%d") + "\n")

    #
    output_file.write ("\n")

#--------------------------------------------
if __name__ == "__main__":
    main()

