#pragma once

#include <hgardenpi-protocol/protocol.hpp>
#include <hgardenpi-protocol/packages/synchro.hpp>
#include <hgardenpi-protocol/head.hpp>

#include <mosquitto.h>

#include <string>
#include <map>
#include <functional>

//generic no copy no move constructor
#define HGARDENPI_NO_COPY_NO_MOVE(clazz)      \
    clazz(clazz &) = delete;                  \
    clazz &operator=(const clazz &) = delete; \
    clazz(clazz &&) = delete;                 \
    clazz &operator=(clazz &&) = delete;


struct mosquitto;
namespace hgarden::test
{
  using namespace hgardenpi::protocol;
  using namespace std;

  class MainController final
  {
  public:
    using OnDataUpdate = function<void (const Head::Ptr &)>;
    typedef map<Flags, string> MapFlags;

  private:
    static constexpr inline int PORT = 1883;
    static constexpr inline int KEEPALIVE = 60;

    string topic;
    uint8_t transactionId = 0;
    string serial;
    MapFlags flags;

    mosquitto *mqttClient = nullptr;

    OnDataUpdate clientOnDataUpdate;
    OnDataUpdate serverOnDataUpdate;
    friend void mqttClientCallback(struct mosquitto *, void *, int) noexcept;
  public:


    MainController();
    ~MainController() noexcept;
    HGARDENPI_NO_COPY_NO_MOVE(MainController)

    void connect(const string &url, const string &user, const string &pwd, const string &serial);

    void disconnect() noexcept;

    void next() noexcept;

    inline void setClientOnDataUpdate(const OnDataUpdate &newClientOnDataUpdate) noexcept
    {
      clientOnDataUpdate = newClientOnDataUpdate;
    }

    inline void setServerOnDataUpdate(const OnDataUpdate &newServerOnDataUpdate) noexcept
    {
      serverOnDataUpdate = newServerOnDataUpdate;
    }


    inline const MapFlags & getFlags() const noexcept
    {
      return flags;
    }
  };

}


