const initialState = {
  sensors: [
    { sensor: 'lm35', value: '' },
    { sensor: 'dht11', value: '' },
    { sensor: 'ldr', value: '' },
  ],
  config: {
    lm35: 25,
    dht11: 40,
    ldr: 25,
  }
}

export default (state = initialState, action) => {
  switch (action.type) {
    case 'FETCH_SENSOR_SUCCESS': {
      return {
        ...state,
        sensors: [
          { sensor: 'lm35', Medida: parseInt(action.sensors.lm35) },
          { sensor: 'dht11', Medida: parseInt(action.sensors.dht11) },
          { sensor: 'ldr', Medida: parseInt(action.sensors.ldr) },
        ]
      }
    }
    case 'UPDATE_SENSOR_FIELDS': {
      const config = {
          ...state.config,
          ...action.field
      }
      return {
        ...state,
        config,
      }
    }
    default:
      return state
  }
}
