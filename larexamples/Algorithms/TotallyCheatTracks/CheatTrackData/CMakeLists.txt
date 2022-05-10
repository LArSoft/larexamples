cet_make_library(LIBRARY_NAME CheatTrack INTERFACE
  SOURCE CheatTrack.h
  LIBRARIES INTERFACE
  lardataobj::RecoBase
  ROOT::EG
)

build_dictionary(DICTIONARY_LIBRARIES PRIVATE
  larexamples::CheatTrack
  nusimdata::SimulationBase
  canvas::canvas
)

install_source()
install_headers()
