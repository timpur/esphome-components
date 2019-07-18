import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, ICON_EMPTY, UNIT_DEGREES, UNIT_EMPTY


DEPENDENCIES = []

CONF_HEADING = 'heading'
CONF_SPEED = 'speed'
CONF_MAX_SPEED = 'max_speed'

CONF_DEP_HEADING = 'dep_heading_id'
CONF_DEP_FREQUENCY = 'dep_frequency_id'
CONF_DEP_MAX_FREQUENCY = 'dep_max_frequency_id'

wind_ns = cg.esphome_ns.namespace('wind')
WindComponent = wind_ns.class_('WindComponent', cg.Component)

heading_schema = sensor.sensor_schema(UNIT_DEGREES, ICON_EMPTY, 0)
speed_schema = sensor.sensor_schema("km/h", ICON_EMPTY, 2)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WindComponent),
    cv.Optional(CONF_HEADING): heading_schema,
    cv.Optional(CONF_SPEED): speed_schema,
    cv.Optional(CONF_MAX_SPEED): speed_schema,

    cv.Required(CONF_DEP_HEADING): cv.use_id(sensor.Sensor),
    cv.Required(CONF_DEP_FREQUENCY): cv.use_id(sensor.Sensor),
    cv.Required(CONF_DEP_MAX_FREQUENCY): cv.use_id(sensor.Sensor),

}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    heading = yield cg.get_variable(config[CONF_DEP_HEADING])
    frequency = yield cg.get_variable(config[CONF_DEP_FREQUENCY])
    max_frequency = yield cg.get_variable(config[CONF_DEP_MAX_FREQUENCY])
    cg.add(var.set_dependent_sensors(heading, frequency, max_frequency))

    if CONF_HEADING in config:
        sens = yield sensor.new_sensor(config[CONF_HEADING])
        cg.add(var.set_heading_sensor(sens))
    if CONF_SPEED in config:
        sens = yield sensor.new_sensor(config[CONF_SPEED])
        cg.add(var.set_speed_sensor(sens))
    if CONF_MAX_SPEED in config:
        sens = yield sensor.new_sensor(config[CONF_MAX_SPEED])
        cg.add(var.set_max_speed_sensor(sens))
