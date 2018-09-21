import React, { Component } from 'react'
import { connect } from 'react-redux'
import { Row, Button } from 'react-materialize'
import { BarChart, Bar, Legend, CartesianGrid, XAxis, YAxis, Tooltip } from 'recharts'
import { fetchSensorData, saveConfig } from '../../actions/sensors'
import NumericInput from 'react-numeric-input';

class Home extends Component {
  constructor(props) {
    super(props)

    this.updateFields = this.updateFields.bind(this)
    this.sendConfig = this.sendConfig.bind(this)
    
  }
  
  componentWillMount() {
    this.props.fetchData();
  }

  updateFields(valueAsNumber, valueAsString, input) {
    this.props.updateFields({ [input.name]: valueAsString });
  }
  
  sendConfig() {
    this.props.sendConfig(this.props.config)
  }

  render() {
    return (
      <div>
        <div className="header-spacing"></div>
        <Row>
          LM35
          <NumericInput s={4} className="custom-input" label="Limite de Temperatura" name="lm35" value={this.props.config.lm35} onChange={this.updateFields}/>
          DHT11
          <NumericInput s={4} className="custom-input" label="Limite de Umidade" name="dht11" value={this.props.config.dht11} onChange={this.updateFields}/>
          LDR
          <NumericInput s={4} className="custom-input" label="Limite de Luz" name="ldr" value={this.props.config.ldr} onChange={this.updateFields}/>
        </Row>
        <Row>
          <Button className="red" onClick={this.sendConfig}>Configurar</Button>
        </Row>
        <Row>
          <BarChart width={730} height={250} data={this.props.sensors}>
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="sensor" />
            <YAxis />
            <Tooltip />
            <Legend />
            <Bar dataKey="Medida" fill="#8884d8" />
          </BarChart>
        </Row>
      </div>
    )
  }
}

Home.defaultProps = {
  config: {
    lm35: 25,
    dht11: 40,
    ldr: 25,
  }
}

export default connect(
  state => state.sensors,
  dispatch => ({
    fetchData: () => dispatch(fetchSensorData()),
    updateFields: field => dispatch({ type: 'UPDATE_SENSOR_FIELDS', field }),
    sendConfig: (config) => dispatch(saveConfig(config))
  })
)(Home)
