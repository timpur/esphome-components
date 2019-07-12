import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ADDRESS, CONF_ID, UNIT_EMPTY, ICON_PULSE

DEPENDENCIES = ['i2c']

CONF_FREQUENCY = 'frequency'
CONF_PWM = 'pwm'

CONF_MAX_FREQUENCY = 'max_frequency'
CONF_MAX_PWM = 'max_pwm'

CONF_MIN_FREQUENCY = 'min_frequency'
CONF_MIN_PWM = 'min_pwm'


freqtiny_ns = cg.esphome_ns.namespace('freqtiny')
FreqTinyComponent = freqtiny_ns.class_('FreqTinyComponent', cg.PollingComponent, i2c.I2CDevice)

frequency_schema = sensor.sensor_schema("Hz", ICON_PULSE, 2)
pwm_schema = sensor.sensor_schema(UNIT_EMPTY, ICON_PULSE, 0)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(FreqTinyComponent),
    cv.Optional(CONF_ADDRESS): cv.i2c_address,
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
