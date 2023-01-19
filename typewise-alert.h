#pragma once

static const int LowLimit                 = 0;
static const int PassiveCoolingLimit      = 35;
static const int MediumActiveCoolingLimit = 40;
static const int HighActiveCoolingLimit   = 45;

#include "typewise-alert.h"
#include <stdio.h>
#include <vector>
#include <unordered_map>

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH,
  UNDEFINED
} BreachType;

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING,
  UNDEFINED_COOLING
} CoolingType;

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL,
  UNDEFINED_TARGET
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

typedef std::unordered_map<BreachType, std::string> breachPrompt;

//=======================
//====Breach Handling====
//=======================
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
    breachPrompt bPrompt{
      {BreachType::NORMAL,    "Normal"},
      {BreachType::TOO_HIGH,  "Too High"},
      {BreachType::TOO_LOW,   "Too Low"},
      {BreachType::UNDEFINED, "Undefined"}
    };
    
    if(breach != BreachType::NORMAL){
      const char* recepient = "a.b@c.com";
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is %s", bPrompt.at(breach).c_str());
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

BreachType inferBreach(CoolingType coolingType, double temperatureInC);
BreachBaseHandler* getBreachHandler(AlertTarget, BreachHandlerFactory &);
bool notifyHandler(BreachBaseHandler* handler, BreachType breach);

void checkAndAlert(
  AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);
