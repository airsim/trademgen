#!/usr/bin/env python

import sys, getopt
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

# Initialise the Trademgen C++ library
import libpytrademgen
trademgenLibrary = libpytrademgen.Trademgener()
trademgenLibrary.init('pytrademgen.log', 'trademgen', 'trademgen', 'localhost', '3306', 'trademgen')

# If no search string was supplied as arguments of the command-line,
# ask the user for some
if searchString == '':
    # Ask for the user input
    searchString = raw_input('Enter a search string, or just Enter for the default one (' + defaultSearchString + '): ')
if searchString == '' : searchString = defaultSearchString

# DEBUG
print "searchString: " + searchString

# Call the Trademgen C++ library
TestDataCollectionPoints=[-262,-154,-110,-92,-82,-77,-65,-60,-50]
TestUnconstrainedDemand=[3,14,22,28,41,86,100,112,123]
result = trademgenLibrary.trademgen (searchString)
#result = trademgenLibrary.trademgen(searchString, TestUnconstrainedDemand, TestDataCollectionPoints)

print 'Raw result from the Trademgen library:'
print result

