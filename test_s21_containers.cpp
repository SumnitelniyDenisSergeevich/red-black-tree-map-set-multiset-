#include "s21_mapTests.h"
#include "s21_multisetTests.h"
#include "s21_setTests.h"
#include "s21_vectorTests.h"
#include "s21_arrayTests.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
