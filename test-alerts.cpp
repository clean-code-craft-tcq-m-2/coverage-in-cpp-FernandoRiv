#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  double DegreesC = 20.0;
  REQUIRE(inferBreach(CoolingType::HI_ACTIVE_COOLING, DegreesC)\
          == BreachType::NORMAL);
}
