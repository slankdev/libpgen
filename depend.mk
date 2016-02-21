src/io/stream.o: src/io/stream.cc include/pgen2/io/stream.h \
  include/pgen2/types.h include/pgen2/exception.h \
  include/pgen2/io/util.h
src/io/util.o: src/io/util.cc include/pgen2/io/util.h include/pgen2/types.h \
  include/pgen2/arch/arch.h include/pgen2/exception.h
src/data_container.o: src/data_container.cc include/pgen2/data_container.h \
  include/pgen2/exception.h
src/arch/arch.o: src/arch/arch.cc include/pgen2/arch/arch.h \
  include/pgen2/exception.h include/pgen2/io/util.h \
  include/pgen2/types.h
src/core/address.o: src/core/address.cc include/pgen2/arch/arch.h \
  include/pgen2/exception.h include/pgen2/core/address.h
src/core/header.o: src/core/header.cc include/pgen2/core/header.h \
  include/pgen2/core/address.h include/pgen2/exception.h
src/core/protocol/packet.o: src/core/packet.cc include/pgen2/core/packet.h \
  include/pgen2/io/util.h include/pgen2/types.h \
  include/pgen2/data_container.h
src/core/protocol/ethernet.o: src/core/protocol/ethernet.cc include/pgen2/core/header.h \
  include/pgen2/core/address.h include/pgen2/core/packet.h \
  include/pgen2/io/util.h include/pgen2/types.h \
  include/pgen2/data_container.h include/pgen2/core/protocol/ethernet.h \
  include/pgen2/exception.h
src/core/protocol/ip.o: src/core/protocol/ip.cc include/pgen2/core/protocol/ip.h
