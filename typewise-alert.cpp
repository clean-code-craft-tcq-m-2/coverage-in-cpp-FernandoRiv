#include "typewise-alert.h"
#include <stdio.h>
#include <vector>

const int lowLimit                 = 0;
const int PassiveCoolingLimit      = 35;
const int MediumActiveCoolingLimit = 40;
const int HighActiveCoolingLimit   = 45;

class CoolingBaseSollution{
  public:
  CoolingType coolType = UNDEFINED_COOLING;
  int lowerLimit = 0;
  int upperLimit = 0;

  CoolingBaseSollution(CoolingType cType, int lowLimit, int highLimit):
    coolType(cType),
    lowerLimit(lowLimit),
    upperLimit(highLimit)
  {}

  bool isTemperatureLow(double value){
    return (value < lowerLimit);
  }
  bool isTemperatureHigh(double value){
    return (value > upperLimit);
  }

  BreachType checkTemperatureBreach(double value){
    if(isTemperatureLow(value)) return BreachType::TOO_LOW;
    if(isTemperatureHigh(value)) return BreachType::TOO_HIGH; 
    return BreachType::NORMAL;
  }
};

class PassiveCooling : public CoolingBaseSollution{
  public: 
    PassiveCooling():CoolingBaseSollution(
      CoolingType::PASSIVE_COOLING,
      lowLimit,
      PassiveCoolingLimit)
    {};
};


class MediumActiveCooling : public CoolingBaseSollution{
  public: 
    MediumActiveCooling():CoolingBaseSollution(
      CoolingType::MED_ACTIVE_COOLING,
      lowLimit,
      MediumActiveCoolingLimit)
    {};
};

class HighActiveCooling : public CoolingBaseSollution{
  public: 
    HighActiveCooling():CoolingBaseSollution(
      CoolingType::HI_ACTIVE_COOLING,
      lowLimit,
      HighActiveCoolingLimit)
    {};
};

class CoolingSollutionFactory{
  public:
  PassiveCooling      pCooling;
  MediumActiveCooling mCooling;
  HighActiveCooling   hCooling;

  std::vector<CoolingBaseSollution*> coolingSollutions = \
    {&pCooling, &mCooling, &hCooling};

  CoolingSollutionFactory(){}

  CoolingBaseSollution* getSollution(CoolingType activeType){
    for(std::vector<CoolingBaseSollution*>::iterator it = coolingSollutions.begin();
        it != coolingSollutions.end(); it++){
      if((*it)->coolType == activeType){
        return (*it);
      }
    }
    return nullptr;
  }
};

BreachType inferBreach(
    CoolingType coolingType, double temperatureInC) {
  CoolingSollutionFactory coolingFactory;
  CoolingBaseSollution* coolingSollution;
  coolingSollution = coolingFactory.getSollution(coolingType);
  return coolingSollution->checkTemperatureBreach(temperatureInC);
}

void monitorTemperature(
  AlertTarget alertTarget, BatteryCharacter batteryChar,\
  double temperatureInC){

  BreachType breachType = inferBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  switch(breachType) {
    case TOO_LOW:
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too low\n");
      break;
    case TOO_HIGH:
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too high\n");
      break;
    case NORMAL:
      break;
  }
}
