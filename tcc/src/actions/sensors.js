import axios from 'axios'

export const fetchSensorData = createPollingAction(dispatch => {
  fetch('http://aqueous-beach-59482.herokuapp.com/data').then(
    response => {
      response.json() 
      .then(sensors => dispatch({ type: 'FETCH_SENSOR_SUCCESS', sensors }))
    }, 
  );
}, 10000);

export function saveConfig(config) {
  return (dispatch) => {
    axios({
      method: 'post',
      url: 'http://aqueous-beach-59482.herokuapp.com/config',
      headers: {
        'Content-Type': 'application/json',
      },
      data: config 
    }).then(() => dispatch(fetchSensorData()))
  }
}

function createPoller(interval, initialDelay) {
  let timeoutId = null;
  let poller = () => {};
  return fn => {
    window.clearTimeout(timeoutId);
    poller = () => {
      timeoutId = window.setTimeout(poller, interval);
      return fn();
    };
    if (initialDelay) {
      return timeoutId = window.setTimeout(poller, interval);
    }
    return poller();
  };
}

function createPollingAction (action, interval, initialDelay) {
  const poll = createPoller(action, initialDelay);
  return () => (dispatch, getState) => poll(() => action(dispatch, getState));
}
