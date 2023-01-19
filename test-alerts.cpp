#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("Breach test for High active cooling, all temperatures") {
  int DegreesC = HighActiveCoolingLimit;
  REQUIRE(inferBreach(CoolingType::HI_ACTIVE_COOLING, DegreesC)\
          == BreachType::NORMAL);
  DegreesC = HighActiveCoolingLimit + 1;
  REQUIRE(inferBreach(CoolingType::HI_ACTIVE_COOLING, DegreesC)\
          == BreachType::TOO_HIGH);
  DegreesC = LowLimit - 1;
  REQUIRE(inferBreach(CoolingType::HI_ACTIVE_COOLING, DegreesC)\
          == BreachType::TOO_LOW);
}

