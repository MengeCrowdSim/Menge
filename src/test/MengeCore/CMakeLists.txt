file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(mengeCoreTest ${SRCS})

TARGET_LINK_LIBRARIES(
  mengeCoreTest
  #testfoo
  #  libfoo
      libgtest
     libgmock
  )

add_test(NAME mengeCoreTest
  COMMAND mengeCoreTest)
