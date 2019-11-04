import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ADDRESS, CONF_ID, UNIT_EMPTY, ICON_PULSE
from esphome.py_compat import text_type

DEPENDENCIES = ['i2c']

CONF_PIN = 'pin'
CONF_PIN_PULLUP = 'pin_pullup'
CONF_PULSE_WIDTH_FILTER = 'pulse_width_filter'
CONF_FREQUENCY = 'frequency'
CONF_PWM = 'pwm'
CONF_MAX_FREQUENCY = 'max_frequency'
CONF_MAX_PWM = 'max_pwm'
CONF_MIN_FREQUENCY = 'min_frequency'
CONF_MIN_PWM = 'min_pwm'


freqtiny_ns = cg.esphome_ns.namespace('freqtiny')
FreqTinyComponent = freqtiny_ns.class_(
    'FreqTinyComponent', cg.PollingComponent, i2c.I2CDevice)

FreqTinyPin = freqtiny_ns.enum('FreqTinyPin')
FreqTinyPins = {
    5: FreqTinyPin.FREQTINY_PIN_5,
    3: FreqTinyPin.FREQTINY_PIN_3,
    4: FreqTinyPin.FREQTINY_PIN_4,
    1: FreqTinyPin.FREQTINY_PIN_1,
}

FreqTinyPulseWidthFilter = freqtiny_ns.enum('FreqTinyPulseWidthFilter')
FreqTinyPulseWidthFilters = {
    0: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_OFF,
    50: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_50_US,
    100: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_100_US,
    150: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_150_US,
    200: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_200_US,
    250: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_250_US,
    300: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_300_US,
    350: FreqTinyPulseWidthFilter.FREQTINY_PULSE_WIDTH_FILTER_350_US,
}


def validate_enum(enum_values, units=None, int=True):
    _units = []
    if units is not None:
        _units = units if isinstance(units, list) else [units]
        _units = [text_type(x) for x in _units]
    enum_bound = cv.enum(enum_values, int=int)

    def validate_enum_bound(value):
        value = cv.string(value)
        for unit in _units:
            if value.endswith(unit):
                value = value[:-len(unit)]
                break
        return enum_bound(value)
    return validate_enum_bound


frequency_schema = sensor.sensor_schema("Hz", ICON_PULSE, 2)
pwm_schema = sensor.sensor_schema(UNIT_EMPTY, ICON_PULSE, 0)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(FreqTinyComponent),
    cv.Optional(CONF_ADDRESS): cv.i2c_address,
    cv.Optional(CONF_PIN, default='3'): validate_enum(FreqTinyPins),
    cv.Optional(CONF_PIN_PULLUP, default=True): cv.boolean,
    cv.Optional(CONF_PULSE_WIDTH_FILTER, default='100us'): validate_enum(FreqTinyPulseWidthFilters, units="us"),
    cv.Optional(CONF_FREQUENCY): frequency_schema,
    cv.Optional(CONF_PWM): pwm_schema,
    cv.Optional(CONF_MAX_FREQUENCY): frequency_schema,
    cv.Optional(CONF_MAX_PWM): pwm_schema,
    cv.Optional(CONF_MIN_FREQUENCY): frequency_schema,
    cv.Optional(CONF_MIN_PWM): pwm_schema,
}).extend(cv.polling_component_schema('60s')).extend(i2c.i2c_device_schema(0x40))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    cg.add(var.set_pin(config[CONF_PIN]))
    cg.add(var.set_pin_pullup(config[CONF_PIN_PULLUP]))
    cg.add(var.set_filter(config[CONF_PULSE_WIDTH_FILTER]))

    if CONF_FREQUENCY in config:
        sens = yield sensor.new_sensor(config[CONF_FREQUENCY])
        cg.add(var.set_frequency_sensor(sens))
    if CONF_PWM in config:
        sens = yield sensor.new_sensor(config[CONF_PWM])
        cg.add(var.set_pwm_sensor(sens))

    if CONF_MAX_FREQUENCY in config:
        sens = yield sensor.new_sensor(config[CONF_MAX_FREQUENCY])
        cg.add(var.set_max_frequency_sensor(sens))
    if CONF_MAX_PWM in config:
        sens = yield sensor.new_sensor(config[CONF_MAX_PWM])
        cg.add(var.set_max_pwm_sensor(sens))

    if CONF_MIN_FREQUENCY in config:
        sens = yield sensor.new_sensor(config[CONF_MIN_FREQUENCY])
        cg.add(var.set_min_frequency_sensor(sens))
    if CONF_MIN_PWM in config:
        sens = yield sensor.new_sensor(config[CONF_MIN_PWM])
        cg.add(var.set_min_pwm_sensor(sens))
