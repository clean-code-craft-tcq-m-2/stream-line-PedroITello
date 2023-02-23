import stream_line_sender_constants as slsc


def classify_temperature_breach(coolingType, temperatureInC):
    limits = temperature_breach_limit(coolingType)
    return (coolingType,
        temperatureInC,
        limits['lowerLimit'],
        limits['upperLimit']
    )


def check_and_send(batteryChar, temperatureInC):
    result = send_to_stream(
        classify_temperature_breach(
            batteryChar['coolingType'],
            temperatureInC
            ))
    return result
    # return classify_temperature_breach(
    #     batteryChar['coolingType'],
    #     temperatureInC
    # )


def send_to_stream(breachType):
  header = 0xfeed
  print(f'{header}, {breachType}')
  return True


def temperature_breach_limit(coolingType):
  return slsc.COOLING_TYPE_LIMITS[coolingType]