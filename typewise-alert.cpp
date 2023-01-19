#include "typewise-alert.h"
#include <stdio.h>
#include <vector>

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
    PassiveCooling():
      CoolingBaseSollution(
        CoolingType::PASSIVE_COOLING,
        LowLimit,
        PassiveCoolingLimit)
    {};
};


class MediumActiveCooling : public CoolingBaseSollution{
  public: 
    MediumActiveCooling():
      CoolingBaseSollution(
        CoolingType::MED_ACTIVE_COOLING,
        LowLimit,
        MediumActiveCoolingLimit)
    {};
};

class HighActiveCooling : public CoolingBaseSollution{
  public: 
    HighActiveCooling():
      CoolingBaseSollution(
        CoolingType::HI_ACTIVE_COOLING,
        LowLimit,
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

class BreachBaseHandler{
  public:
  AlertTarget target = AlertTarget::UNDEFINED_TARGET;

  BreachBaseHandler(AlertTarget targetType):
    target(targetType)
  {}

  virtual bool sendTarget(BreachType breach){
    (void)breach;
    return false;
  }
};

class ControllerHandler : public BreachBaseHandler{
  public:
  ControllerHandler() : 
    BreachBaseHandler(AlertTarget::TO_CONTROLLER)
  {}

  bool sendTarget(BreachType breach) override{
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breach);
    return true;
  }
};

class EmailHandler : public BreachBaseHandler{
  public:
  EmailHandler() :
    BreachBaseHandler(AlertTarget::TO_EMAIL)
  {}

  bool sendTarget(BreachType breach) override{
    const char* recepient = "a.b@c.com";
    switch(breach) {
      case TOO_LOW:
        printf("To: %s\n", recepient);
        printf("Hi, the temperature is too low\n");
        break;
      case TOO_HIGH:
        printf("To: %s\n", recepient);
        printf("Hi, the temperature is too high\n");
        break;
      case NORMAL:
      default:
        break;
    }
    return true;
  }
};

class BreachHandlerFactory{
  public:
  EmailHandler      eHandling;
  ControllerHandler chandling;

  std::vector<BreachBaseHandler*> handlingSollutions = \
    {&eHandling, &chandling};

  BreachHandlerFactory(){}

  BreachBaseHandler* getHandler(AlertTarget aTarget){
    for(std::vector<BreachBaseHandler*>::iterator it = handlingSollutions.begin();
        it != handlingSollutions.end(); it++){
      if((*it)->target == aTarget){
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
  if(coolingSollution != nullptr)
    return coolingSollution->checkTemperatureBreach(temperatureInC);
  else
    return BreachType::UNDEFINED;
}

void monitorTemperature(
  AlertTarget alertTarget, BatteryCharacter batteryChar,\
  double temperatureInC){

  BreachType breachType = inferBreach(
    batteryChar.coolingType, temperatureInC
  );

  BreachHandlerFactory bHandlerFactory;
  BreachBaseHandler* breachHandler;
  breachHandler = bHandlerFactory.getHandler(alertTarget);
  breachHandler->sendTarget(breachType);
}