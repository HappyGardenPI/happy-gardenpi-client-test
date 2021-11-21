#include "maincontroller.hpp"

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <hgardenpi-protocol/utilities/numberutils.hpp>
#include <hgardenpi-protocol/utilities/stringutils.hpp>
#include <hgardenpi-protocol/protocol.hpp>
#include <hgardenpi-protocol/packages/synchro.hpp>


namespace hgarden::test
{

  static MainController *self;

  void mqttClientCallbackRX( mosquitto *, void *payload, int len) noexcept;

  MainController::MainController()
      : transactionId(generateRandomIntegral<uint8_t>())
      , clientOnDataUpdate([](const Head::Ptr &){})
      , serverOnDataUpdate([](const Head::Ptr &){})
      , clientSerial(move(generateRandomString(16)))
  {
    self = this;
    mosquitto_lib_init();

    flags[Flags::AGG] = "Aggregation";
    flags[Flags::DAT] = "Data";
    flags[Flags::ERR] = "Error";
    flags[Flags::FIN] = "Finish";
    flags[Flags::STA] = "Station";
    flags[Flags::SYN] = "Synchro";

    bool cleanSession = true;
    mqttClinetRX = mosquitto_new("ClientRX", cleanSession, nullptr);
    if(!mqttClinetRX){
      throw runtime_error("out of memory for rx");
    }

    mqttClinetTX = mosquitto_new("ClientTX", cleanSession, nullptr);
    if(!mqttClinetTX){
      throw runtime_error("out of memory for tx");
    }

    mosquitto_log_callback_set(mqttClinetRX, [](struct mosquitto *, void *, int, const char *msg)
    {
      cout << "rx mosquitto_log_callback_set" << msg << endl;
    });
    mosquitto_log_callback_set(mqttClinetTX, [](struct mosquitto *, void *, int, const char *msg)
    {
      cout << "tx mosquitto_log_callback_set" << msg << endl;
    });

    mosquitto_connect_callback_set(mqttClinetRX, [](struct mosquitto *, void *, int result)
    {
      if (result != MOSQ_ERR_SUCCESS)
      {
          string err("rx mosquitto client connection error: ");
          err.append(mosquitto_strerror(result));
          cerr << err << endl;
      }
    });
    mosquitto_connect_callback_set(mqttClinetTX, [](struct mosquitto *, void *, int result)
    {
      if (result != MOSQ_ERR_SUCCESS)
      {
          string err("tx mosquitto client connection error: ");
          err.append(mosquitto_strerror(result));
          cerr << err << endl;
      }
    });
//    mosquitto_publish_callback_set(mqttClinetRX, [](struct mosquitto *, void *payload, int len){
//      cout << "A:" << stringHexToString((uint8_t *)payload, len) << endl;
//    });
    mosquitto_publish_callback_set(mqttClinetTX, &mqttClientCallbackRX);
//    mosquitto_message_callback_set(mqttClinetRX, [](struct mosquitto *, void *, const struct mosquitto_message *message){
//      cout << "C:" << stringHexToString((uint8_t *)message->payload, message->payloadlen) << endl;
//    });
//    mosquitto_message_callback_set(mqttClinetTX, [](struct mosquitto *, void *, const struct mosquitto_message *message){
//      cout << "D:" << stringHexToString((uint8_t *)message->payload, message->payloadlen) << endl;
//    });


  }

  MainController::~MainController() noexcept
  {
    mosquitto_destroy(mqttClinetRX);
    mosquitto_destroy(mqttClinetTX);
    mosquitto_lib_cleanup();
  }

  void MainController::connect(const string &url, const string &user, const string &pwd, const string &serial)
  {


    this->topicTX = move(BASE_TOPIC + serial);
    this->topicRX = move(BASE_TOPIC + this->clientSerial);

    mosquitto_username_pw_set(mqttClinetRX, user.c_str(), pwd.c_str());
    mosquitto_username_pw_set(mqttClinetTX, user.c_str(), pwd.c_str());

    int ver = MQTT_PROTOCOL_V311;
    mosquitto_opts_set(mqttClinetRX, MOSQ_OPT_PROTOCOL_VERSION, &ver);

    if(auto rc = mosquitto_connect(mqttClinetRX, url.c_str(), PORT, KEEPALIVE))
    {
      string err("rx mosquitto_connect() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

    if(auto rc = mosquitto_connect(mqttClinetTX, url.c_str(), PORT, KEEPALIVE))
    {
      string err("tx mosquitto_connect() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

    if(auto rc = mosquitto_subscribe(mqttClinetRX, nullptr, topicRX.c_str(), 0))
    {
      string err("rx mosquitto_subscribe() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

  }

  void MainController::disconnect() noexcept
  {
    mosquitto_disconnect(mqttClinetRX);
    mosquitto_disconnect(mqttClinetTX);
  }

  void MainController::next() noexcept
  {

    auto synToServer = new Synchro;
    synToServer->setSerial(clientSerial);
    auto encoded = encode(synToServer);
    delete synToServer;


    updateIdToBufferEncoded(encoded, transactionId);

    auto dataSend = decodeFirst(encoded);

    cout << stringHexToString(encoded[0].first.get(), encoded[0].second) << endl;

    clientOnDataUpdate(dataSend);

    if (int rc = mosquitto_publish(mqttClinetTX, nullptr, topicTX.c_str(), encoded[0].second, reinterpret_cast<const void *>(encoded[0].first.get()), 0, 0) != MOSQ_ERR_SUCCESS)
    {
        string err("mosquitto_publish() error: ");
        err.append(mosquitto_strerror(rc));
    }

  }

  void mqttClientCallbackRX(mosquitto *, void *payload, int len) noexcept try
  {

//    uint8_t *payload = new (nothrow) uint8_t[message->payloadlen];
//    if(!payload)
//    {
//      throw runtime_error("no memory for payload");
//    }

//    memset(payload, 0, message->payloadlen);

    cout << "" << stringHexToString(reinterpret_cast<uint8_t *>(payload), len) << endl;

    //decode message
    auto head = decode((uint8_t *)payload);

    self->clientOnDataUpdate(head);

    //delete[] payload;

  }
  catch (const runtime_error &e)
  {
     cerr << e.what() << endl;
  }
  catch (...)
  {
     cerr << "generic exception" << endl;
  }

}
