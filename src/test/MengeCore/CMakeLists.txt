file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(mengeCoreTest ${SRCS})


TARGET_LINK_LIBRARIES(
  mengeCoreTest
  mengeCore
  libgtest
  libgmock
)


include_directories("../../Menge")


add_test(NAME mengeCoreTest
  COMMAND mengeCoreTest)
