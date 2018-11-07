add_library(xml_loader xml_loader.cpp ../../include/loaders/xml_loader.hpp)
target_link_libraries(xml_loader PUBLIC gpt_base)