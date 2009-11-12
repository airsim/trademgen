include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = forecast-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = forecast-paths.hpp

# Targets
all-local: forecast-paths.hpp

forecast-paths.hpp: Makefile
	@echo '#ifndef __@PACKAGE_NAME@_PATHS_HPP' > $@
	@echo '#define __@PACKAGE_NAME@_PATHS_HPP' >> $@
	@echo '#define PACKAGE "forecast"' >> $@
	@echo '#define PACKAGE_NAME "@PACKAGE_NAME@"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#endif // __@PACKAGE_NAME@_PATHS_HPP' >> $@
