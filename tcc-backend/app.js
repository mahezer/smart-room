const express = require('express')
const path = require('path')
const PORT = process.env.PORT || 5000
const bodyParser = require('body-parser')
const mqtt = require('mqtt')
const redis = require('redis')
const cors = require('cors')
const app = express()
app.use(bodyParser.json())
app.use(cors({ origin: '*' }))

const mqttUrl = 'http://m10.cloudmqtt.com:15026' 
const mqttOptions = {
  username: 'qxuufkxb',
  password: 'iz1cz4OG_42S',
}

const redisClient = redis.createClient(process.env.HEROKU_REDIS_BLUE_URL);
redisClient.on('connect', function() {
  console.log('Redis client connected')
})

redisClient.on('error', function(err) {
  console.log(err)
})

const client = mqtt.connect(mqttUrl, mqttOptions)

client.subscribe('ldr')
client.subscribe('lm35')
client.subscribe('dht11')

client.on('message', function(topic, message, packet) {
  switch(topic) {
    case 'ldr':
      redisClient.set('ldr', message.toString())
      break;
    case 'lm35':
      redisClient.set('lm35', message.toString())
      break;
    case 'dht11':
      redisClient.set('dht11', message.toString())
      break;
    default:
      break;
  }
})

app.get('/data', (req, res) => {
  redisClient.mget(['ldr', 'dht11', 'lm35'], function (error, result) {
    if (error) {
      console.log(error);
      throw error;
    }
    const body = {
      ldr: result[0],
      dht11: result[1],
      lm35: result[2]
    }
    res.send(body);
  });
})

app.post('/config', function (req, res) {
  redisClient.set('ldr_config', req.body.ldr)
  redisClient.set('lm35_config', req.body.lm35)
  redisClient.set('dht11_config', req.body.dht11)
  res.send({ message: 'OK' })
})

app.get('/config', function (req, res) {
  redisClient.mget(['ldr_config', 'dht11_config', 'lm35_config'], function (error, result) {
    if (error) {
      console.log(error);
      throw error;
    }
    const body = {
      ldr: result[0],
      dht11: result[1],
      lm35: result[2]
    }
    res.send(body);
  });
})

app.listen(PORT)
