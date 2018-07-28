file(GLOB SRCS ${MENGE_ROOT_TEST_DIR}/MengeCore/*.cpp)
message(${SRCS})
ADD_EXECUTABLE(mengeCoreTest ${SRCS})

TARGET_LINK_LIBRARIES(
  mengeCoreTest
  mengeCore
  mengeVis
  libgtest
  libgmock
)

add_test(NAME mengeCoreTest
  COMMAND mengeCoreTest)
