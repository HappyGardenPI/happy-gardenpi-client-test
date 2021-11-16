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

  void mqttClientCallback(struct mosquitto *, void *, int) noexcept;

  MainController::MainController()
      : transactionId(generateRandomIntegral<uint8_t>())
      , clientOnDataUpdate([](const Head::Ptr &){})
      , serverOnDataUpdate([](const Head::Ptr &){})
      , serial(move(generateRandomString(16)))
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
    mqttClient = mosquitto_new(nullptr, cleanSession, nullptr);
    if(!mqttClient){
      throw runtime_error("out of memory for tx");
    }


    //------tx-------
    mosquitto_log_callback_set(mqttClient, [](struct mosquitto *, void *, int, const char *msg)
    {
      cout << "tx mosquitto_log_callback_set" << msg << endl;
    });

    mosquitto_connect_callback_set(mqttClient, [](struct mosquitto *, void *, int result)
    {
      if (result != MOSQ_ERR_SUCCESS)
      {
          string err("tx mosquitto client connection error: ");
          err.append(mosquitto_strerror(result));
          cerr << err << endl;
      }
    });
    mosquitto_publish_callback_set(mqttClient, mqttClientCallback);


  }

  MainController::~MainController() noexcept
  {
    mosquitto_destroy(mqttClient);
    mosquitto_lib_cleanup();
  }

  void MainController::connect(const string &url, const string &user, const string &pwd, const string &serail)
  {
    int ver = MQTT_PROTOCOL_V311;

    this->topic = move("/HappyGardenPI/" + serail);

    mosquitto_username_pw_set(mqttClient, user.c_str(), pwd.c_str());

    mosquitto_opts_set(mqttClient, MOSQ_OPT_PROTOCOL_VERSION, &ver);

    if(auto rc = mosquitto_connect(mqttClient, url.c_str(), PORT, KEEPALIVE))
    {
      string err("tx mosquitto_connect() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

    if(auto rc = mosquitto_subscribe(mqttClient, nullptr, topic.c_str(), 0))
    {
      string err("rx mosquitto_subscribe() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

  }

  void MainController::disconnect() noexcept
  {
    mosquitto_disconnect(mqttClient);
  }

  void MainController::next() noexcept
  {

    auto synToServer = new Synchro;
    synToServer->setSerial(serial);
    auto encoded = encode(synToServer);
    delete synToServer;


    updateIdToBufferEncoded(encoded, transactionId);

    auto dataSend = decodeFirst(encoded);

    cout << stringHexToString(encoded[0].first.get(), encoded[0].second) << endl;

    clientOnDataUpdate(dataSend);

    if (int rc = mosquitto_publish(mqttClient, nullptr, topic.c_str(), encoded[0].second, reinterpret_cast<const void *>(encoded[0].first.get()), 0, 0) != MOSQ_ERR_SUCCESS)
    {
        string err("mosquitto_publish() error: ");
        err.append(mosquitto_strerror(rc));
    }

  }

  void mqttClientCallback(struct mosquitto *, void *data, int length) noexcept try
  {

    uint8_t *payload = new (nothrow) uint8_t[length];
    if(!payload)
    {
      throw runtime_error("no memory for payload");
    }

    memset(payload, 0, length);



    //decode message
    auto head = decode(payload);

    self->clientOnDataUpdate(head);

    delete[] payload;

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
