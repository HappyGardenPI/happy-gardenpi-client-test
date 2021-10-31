#include "maincontroller.hpp"

#include <stdexcept>
#include <iostream>

namespace hgarden::test
{

  void mosquittoRxCallback(mosquitto *, void *, const mosquitto_message *message);
  void mosquittoTxCallback(mosquitto *, void *, const mosquitto_message *message);

  static string generateRandomString(size_t length)
  {
      const char* charmap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      const size_t charmapLength = strlen(charmap);
      auto generator = [&](){ return charmap[arc4random()%charmapLength]; };
      string result;
      result.reserve(length);
      generate_n(back_inserter(result), length, generator);
      return result;
  }






  MainController::MainController()
  {
    mosquitto_lib_init();

    bool cleanSession = true;
    tx = mosquitto_new(nullptr, cleanSession, nullptr);
    if(!tx){
      throw runtime_error("out of memory for tx");
    }

    rx = mosquitto_new(nullptr, cleanSession, nullptr);
    if(!rx){
      throw runtime_error("out of memory for rx");
    }

    //------tx-------
    mosquitto_log_callback_set(tx, [](struct mosquitto *, void *, int, const char *msg)
    {
      cout << "tx mosquitto_log_callback_set" << msg << endl;
    });

    mosquitto_connect_callback_set(tx, [](struct mosquitto *, void *, int result)
    {
      if (result != MOSQ_ERR_SUCCESS)
      {
          string err("tx mosquitto client connection error: ");
          err.append(mosquitto_strerror(result));
          cerr << err << endl;
      }
    });
    mosquitto_message_callback_set(tx, &mosquittoTxCallback);
    mosquitto_publish_callback_set(tx, [](struct mosquitto *, void *, int)
    {
        cout << "tx mosquitto_publish_callback_set" << endl;
    });


    //------rx-------
    mosquitto_log_callback_set(rx, [](struct mosquitto *, void *, int, const char *msg)
    {
      cout << "rx mosquitto_log_callback_set" << msg << endl;
    });
    mosquitto_connect_callback_set(rx, [](struct mosquitto *, void *, int result)
    {
      if (result != MOSQ_ERR_SUCCESS)
      {
          string err("rx mosquitto client connection error: ");
          err.append(mosquitto_strerror(result));
          cerr << err << endl;
      }
    });
    mosquitto_message_callback_set(rx, &mosquittoRxCallback);
    mosquitto_publish_callback_set(rx, [](struct mosquitto *, void *, int)
    {
        cout << "rx mosquitto_publish_callback_set" << endl;
    });

  }

  MainController::~MainController() noexcept
  {
    mosquitto_destroy(rx);
    mosquitto_destroy(tx);
    mosquitto_lib_cleanup();
  }

  void MainController::connect(const string &url, const string &user, const string &pwd, const string &serail)
  {
    int ver = MQTT_PROTOCOL_V311;

    mosquitto_username_pw_set(tx, user.c_str(), pwd.c_str());
    mosquitto_username_pw_set(rx, user.c_str(), pwd.c_str());

    mosquitto_opts_set(tx, MOSQ_OPT_PROTOCOL_VERSION, &ver);
    mosquitto_opts_set(rx, MOSQ_OPT_PROTOCOL_VERSION, &ver);

    if(auto rc = mosquitto_connect(tx, url.c_str(), PORT, KEEPALIVE))
    {
      string err("tx mosquitto_connect() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

    if(auto rc = mosquitto_connect(rx, url.c_str(), PORT, KEEPALIVE))
    {
      string err("rx mosquitto_connect() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

    string topic("/HappyGardenPI/");
    topic += serail;
    if(auto rc = mosquitto_subscribe(rx, nullptr, topic.c_str(), 0))
    {
      string err("rx mosquitto_subscribe() error: ");
      err.append(mosquitto_strerror(rc));
      throw runtime_error(err);
    }

  }

  void MainController::disconnect() noexcept
  {
    mosquitto_disconnect(tx);
    mosquitto_disconnect(rx);
  }

  void MainController::next() noexcept
  {
    auto synToServer = new Synchro;
    synToServer->setSerial(move(generateRandomString(16)));
    auto encoded = encode(synToServer);

    delete synToServer;

    auto dataSend = decode(encoded[0].first.get());

    clientOnDataUpdate(dataSend);


    if (int rc = mosquitto_publish(tx, nullptr, "/HappyGardenPI/100000006be629ae", encoded[0].second, reinterpret_cast<const void *>(encoded[0].first.get()), 0, 0) != MOSQ_ERR_SUCCESS)
    {
        string err("mosquitto_publish() error: ");
        err.append(mosquitto_strerror(rc));
    }

  }

  void mosquittoRxCallback(mosquitto *, void *, const mosquitto_message *message)
  {

  }

  void mosquittoTxCallback(mosquitto *, void *, const mosquitto_message *message)
  {

  }

}
