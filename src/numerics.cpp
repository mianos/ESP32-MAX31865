#include <math.h>
#include "Max31865.h"

// Taken from
// https://www.analog.com/media/en/technical-documentation/application-notes/AN709_0.pdf

static constexpr float RTD_A = 3.9083e-3f;
static constexpr float RTD_B = -5.775e-7f;
static constexpr float RTD_C = -4.183e-12f;
static constexpr float A[6] = {-242.02f,     2.2228f,    2.5859e-3f,
                               -4.8260e-6f, -2.8183e-8f, 1.5243e-10f};

float Max31865::RTDtoTemperature(uint16_t rtd,
                                 max31865_rtd_config_t rtdConfig) {
  float Rrtd = (rtd * rtdConfig.ref) / (float)(1U << 15U);

  float Z1, Z2, Z3, Z4, temperature;
  Z1 = -RTD_A;
  Z2 = RTD_A * RTD_A - (4 * RTD_B);
  Z3 = (4 * RTD_B) / rtdConfig.nominal;
  Z4 = 2 * RTD_B;
  temperature = Z2 + (Z3 * Rrtd);
  temperature = (sqrt(temperature) + Z1) / Z4;

  if (temperature > 0.0f) {
    return temperature;
  }

  Rrtd /= rtdConfig.nominal;
  Rrtd *= 100.0f;
  return A[0] + 
         A[1] * Rrtd + 
         A[2] * pow(Rrtd, 2) + 
         A[3] * pow(Rrtd, 3) +
         A[4] * pow(Rrtd, 4) + 
         A[5] * pow(Rrtd, 5);
}

uint16_t Max31865::temperatureToRTD(float temperature,
                                    max31865_rtd_config_t rtdConfig) {
  float Rrtd = rtdConfig.nominal *
               (1.0 + RTD_A * temperature + RTD_B * pow(temperature, 2));
  if (temperature < 0.0) {
    Rrtd +=
        rtdConfig.nominal * RTD_C * (temperature - 100.0) * pow(temperature, 3);
  }
  return lroundf(Rrtd * (1U << 15U) / rtdConfig.ref);
}
