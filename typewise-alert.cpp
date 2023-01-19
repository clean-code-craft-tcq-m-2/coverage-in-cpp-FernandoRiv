#include "typewise-alert.h"
#include <stdio.h>
#include <vector>

//=========================
//====Cooling Sollution====
//=========================
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

//=====================
//====Check Methods====
//=====================
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

BreachBaseHandler* getBreachHandler(AlertTarget alertTarget, BreachHandlerFactory& bHandlerFactory){
  return bHandlerFactory.getHandler(alertTarget);
}

bool notifyHandler(BreachBaseHandler* handler, BreachType breach){
  if(handler != nullptr)
    return handler->sendTarget(breach);
  else
    return false;
}