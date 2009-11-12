#!/usr/bin/env python

import sys, getopt
sys.path.append('/remote/projteams/projects/ori-data/deliveries/forecast-99.99.99/lib')
sys.path.append('.libs')

# Default search string
defaultSearchString = 'my good old python query string'

# Usage
def usage(script_name):
	print
	print "Usage: ./%s [options] search string" % script_name
	print
	print "The default output is short."
	print
	print "Options:"
	print "    -h, --help              : outputs this help and exits"
	print "    -l, --long              : long output version"
	print

# Handle opt
def handle_opt():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hl", ["help", "long"])
	except getopt.GetoptError, err:
		# print help information and exit:
		print str(err) # will print something like "option -a not recognized"
		usage()
		sys.exit(2)

	# search string
	searchString = "" # default
	if len(args) >= 1: searchString = ' '.join(args)

	# options
	for o, a in opts:
		if o in ("-h", "--help"):
			usage(sys.argv[0])
			sys.exit()
		else:
			assert False, "unhandled option"
	return searchString

searchString = handle_opt()

# Initialise the Forecast C++ library
import libpyforecast
forecastLibrary = libpyforecast.Forecaster()
forecastLibrary.init('pyforecast.log', 'forecast', 'forecast', 'localhost', '3306', 'forecast')

# If no search string was supplied as arguments of the command-line,
# ask the user for some
if searchString == '':
    # Ask for the user input
    searchString = raw_input('Enter a search string, or just Enter for the default one (' + defaultSearchString + '): ')
if searchString == '' : searchString = defaultSearchString

# DEBUG
print "searchString: " + searchString

# Call the Forecast C++ library
TestDataCollectionPoints=[-262,-154,-110,-92,-82,-77,-65,-60,-50]
TestUnconstrainedDemand=[3,14,22,28,41,86,100,112,123]
#result = forecastLibrary.forecast (searchString)
result = forecastLibrary.forecast(searchString, TestUnconstrainedDemand, TestDataCollectionPoints)

print 'Raw result from the Forecast library:'
print result
