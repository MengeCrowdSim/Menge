file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(mengeCoreTest ${SRCS})


TARGET_LINK_LIBRARIES(
  mengeCoreTest
  mengeCore

   mengeVis
  libgtest
  libgmock
)


include_directories("../../Menge")

include_directories("../../")

add_test(NAME mengeCoreTest
  COMMAND mengeCoreTest)
