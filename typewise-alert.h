#pragma once

static const int LowLimit                 = 0;
static const int PassiveCoolingLimit      = 35;
static const int MediumActiveCoolingLimit = 40;
static const int HighActiveCoolingLimit   = 45;

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING,
  UNDEFINED_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH,
  UNDEFINED
} BreachType;

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL,
  UNDEFINED_TARGET
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

BreachType inferBreach(CoolingType coolingType, double temperatureInC);

void checkAndAlert(
  AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);
