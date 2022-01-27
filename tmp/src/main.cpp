#include <iostream>
using namespace std;

#include <hgardenpi-protocol/protocol.hpp>
#include <hgardenpi-protocol/packages/synchro.hpp>
using namespace hgardenpi::protocol;

#include <mosquitto.h>
#include <cstring>
#include <iomanip>

static string stringHexToString(const uint8_t *bytes, size_t size, bool upperCase = false) noexcept
{
    ostringstream ret;
    for (int i = 0; i < size; i++)
    {
        ret << hex << setfill('0') << setw(2) << (upperCase ? uppercase : nouppercase) << (int)bytes[i];
    }
    return ret.str();
}

int main(int argc, char *argv[])
{
    int i;
    char host[]{"passy-raspberrypi.local"};
    int port = 1883;
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = nullptr;

    mosquitto_lib_init();
    mosq = mosquitto_new(nullptr, clean_session, nullptr);
    if(!mosq){
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }
    mosquitto_log_callback_set(mosq, [](struct mosquitto *, void *, int, const char *){
        cout << "mosquitto_log_callback_set" << endl;
    });
    mosquitto_connect_callback_set(mosq, [](struct mosquitto *, void *, int){
        cout << "mosquitto_connect_callback_set" << endl;
    });
    mosquitto_message_callback_set(mosq, [](struct mosquitto *, void *, const struct mosquitto_message *){
        cout << "mosquitto_message_callback_set" << endl;
    });
    mosquitto_publish_callback_set(mosq, [](struct mosquitto *, void *, int){
        cout << "mosquitto_publish_callback_set" << endl;
    });

    mosquitto_username_pw_set(mosq, "hgardenpi", "hgardenpi");

    //set protocol version
    int ver = MQTT_PROTOCOL_V311;
    mosquitto_opts_set(mosq, MOSQ_OPT_PROTOCOL_VERSION, &ver);

    if(mosquitto_connect(mosq, host, port, keepalive)){
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }

    mosquitto_subscribe(mosq, nullptr, "/HappyGardenPI/serialClient", 0);

    auto synToServer = new Synchro;
    synToServer->setSerial("clientTestSerial");

    auto sync1 = encode(synToServer);

    delete synToServer;

    cout << stringHexToString(reinterpret_cast<uint8_t *>(sync1[0].first.get()), sync1[0].second) << endl;
    //auto synFromClient = decode(reinterpret_cast<const uint8_t *>(send1[0].get()));

    if (int rc = mosquitto_publish(mosq, nullptr, "/HappyGardenPI/100000006be629ae", sync1[0].second, reinterpret_cast<const void *>(sync1[0].first.get()), 0, 0) != MOSQ_ERR_SUCCESS)
    {
        string err("mosquitto_publish() error: ");
        err.append(mosquitto_strerror(rc));
        throw runtime_error(err);
    }

    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}