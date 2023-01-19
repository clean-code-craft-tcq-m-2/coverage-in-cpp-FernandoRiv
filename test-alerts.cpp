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

TEST_CASE("Breach test for Undefined cooling sollution") {
  int DegreesC = HighActiveCoolingLimit;
  REQUIRE(inferBreach(CoolingType::UNDEFINED_COOLING, DegreesC)\
          == BreachType::UNDEFINED);
}

TEST_CASE("Handler test for all targets") {
  BreachBaseHandler* handlerTester;
  BreachHandlerFactory handlerFactory;
  handlerTester = getBreachHandler(AlertTarget::TO_CONTROLLER, handlerFactory);
  REQUIRE(handlerTester->target == AlertTarget::TO_CONTROLLER);
  handlerTester = getBreachHandler(AlertTarget::TO_EMAIL, handlerFactory);
  REQUIRE(handlerTester->target == AlertTarget::TO_EMAIL);
  handlerTester = getBreachHandler(AlertTarget::UNDEFINED_TARGET, handlerFactory);
  REQUIRE(handlerTester == nullptr);
}

TEST_CASE("Test for controller notification routines") {
  BreachBaseHandler* handlerTester;
  BreachHandlerFactory handlerFactory;
  handlerTester = getBreachHandler(AlertTarget::TO_CONTROLLER, handlerFactory);
  REQUIRE(notifyHandler(handlerTester, BreachType::NORMAL)     == true);
  REQUIRE(notifyHandler(handlerTester, BreachType::TOO_HIGH)   == true);
  REQUIRE(notifyHandler(handlerTester, BreachType::TOO_LOW)    == true);
  REQUIRE(notifyHandler(handlerTester, BreachType::UNDEFINED)  == true);
  REQUIRE(notifyHandler(nullptr, BreachType::UNDEFINED)        == false);
}

TEST_CASE("Test for email notification routines") {
  BreachBaseHandler* handlerTester;
  BreachHandlerFactory handlerFactory;
  handlerTester = getBreachHandler(AlertTarget::TO_EMAIL, handlerFactory);
  REQUIRE(notifyHandler(handlerTester, BreachType::NORMAL)     == false);
  REQUIRE(notifyHandler(handlerTester, BreachType::TOO_HIGH)   == true);
  REQUIRE(notifyHandler(handlerTester, BreachType::TOO_LOW)    == true);
  REQUIRE(notifyHandler(handlerTester, BreachType::UNDEFINED)  == true);
  REQUIRE(notifyHandler(nullptr, BreachType::UNDEFINED)        == false);
}