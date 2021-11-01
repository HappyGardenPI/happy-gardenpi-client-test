#include "maincontroller.hpp"

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <hgardenpi-protocol/utilities/numberutils.hpp>

namespace hgardenpi::protocol
{
  inline namespace v2 {
    extern std::string generateRandomString(size_t length);
  }
}


namespace hgarden::test
{

  void mqttClientCallback(mosquitto *, void *, const mosquitto_message *message);


  MainController::MainController()
      : transactionId(generateRandomIntegral<uint8_t>())
      , clientOnDataUpdate([](const Head::Ptr &){})
      , serverOnDataUpdate([](const Head::Ptr &){})
      , serial(move(generateRandomString(16)))
  {
    mosquitto_lib_init();

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
    mosquitto_message_callback_set(mqttClient, &mqttClientCallback);
    mosquitto_publish_callback_set(mqttClient, [](struct mosquitto *, void *, int)
    {
        cout << "tx mosquitto_publish_callback_set" << endl;
    });


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

    clientOnDataUpdate(dataSend);


    if (int rc = mosquitto_publish(mqttClient, nullptr, topic.c_str(), encoded[0].second, reinterpret_cast<const void *>(encoded[0].first.get()), 0, 0) != MOSQ_ERR_SUCCESS)
    {
        string err("mosquitto_publish() error: ");
        err.append(mosquitto_strerror(rc));
    }

  }

  void mqttClientCallback(mosquitto *, void *, const mosquitto_message *message)
  {

  }

}
