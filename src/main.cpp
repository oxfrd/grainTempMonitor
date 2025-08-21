//
// Created by oxford on 30.05.23.
//
#include <cstring>

#include "boardInit.h"
#include "main.h"
#include "delay.h"

#include "IComponent.h"
#include "IGpioOutput.h"
#include "IInterrupt.h"
#include "IUart.h"
#include "IGpioInput.h"
#include "IPressureSensor.h"
#include "IGpioOutAndInput.h"
#include "IPort.h"
#include "IDelay.h"
#include "ITemperatureSensor.h"
#include <cassert>

#pragma GCC optimize ("O0")

std::shared_ptr<hal::delay::IDelay> mgDelay{nullptr};

static void errHandler()
{
    while(1)
    {
        asm("NOP");
    }
}

class fastLogger
{
    public:
        fastLogger(std::shared_ptr<hal::uart::IUart> uart):
        mUart(uart)
        {
            assert(mUart != nullptr);
            printWelcomeMessage();
        }
        
        void log(const char* text)
        {
            mSizeToSend = sprintf((char*)mAcc, text);
            if (mSizeToSend > cAccSize)
                return;

            mUart->send(mAcc, mSizeToSend);
        }

        void log(const char* text, float fltVal)
        {
            mSizeToSend = sprintf((char*)mAcc, text, fltVal);
            if (mSizeToSend > cAccSize)
                return;

            mUart->send(mAcc, mSizeToSend);
        }

        void log(const char* text, uint64_t val)
        {
            mSizeToSend = sprintf((char*)mAcc, text, val);
            if (mSizeToSend > cAccSize)
                return;

            mUart->send(mAcc, mSizeToSend);
        }

        void log(const char* text, uint8_t val)
        {
            mSizeToSend = sprintf((char*)mAcc, text, val);
            if (mSizeToSend > cAccSize)
                return;

            mUart->send(mAcc, mSizeToSend);
        }

    private:
        static constexpr uint8_t cAccSize{60};
        std::shared_ptr<hal::uart::IUart> mUart{nullptr};
        uint8_t mAcc[cAccSize];
        uint8_t mSizeToSend{};

        void printWelcomeMessage()
        {
            log("/*************************************************/'\r\n");
            log("/*                                               */'\r\n");
            log("/*    !!! WELCOME TO GRAIN TEMPERATURE APP !!!   */'\r\n");
            log("/*       author: oxfrd                           */'\r\n");
            log("/*       v0.1                                    */'\r\n");
            log("/*       MIT license                             */'\r\n");
            log("/*                                               */'\r\n");
            log("/*************************************************/'\r\n");
        }
};

int main()
{
    auto boardResources = board::init();

    std::shared_ptr<hal::gpio::IGpioOutput> ledRed{nullptr};
    {
        auto getter = ledRed->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_B2),boardResources);
        if (getter.second == eError::eOk)
        {
            ledRed = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::gpio::IGpioOutput> ledGreen{nullptr};
    {
        auto getter = ledGreen->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_E8),boardResources);
        if (getter.second == eError::eOk)
        {
            ledGreen = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::uart::IUart> uart2{nullptr};
    {
        auto getter = uart2->getPtr(static_cast<uint16_t>(board::eResourcesList::eUART2),boardResources);
        if (getter.second == eError::eOk)
        {
            uart2 = getter.first;
        } else { errHandler();}
    }

    {
        auto getter = mgDelay->getPtr(static_cast<uint16_t>(board::eResourcesList::eDelay),boardResources);
        if (getter.second == eError::eOk)
        {
            mgDelay = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::sensor::ITemperatureSensor> temperature1{nullptr};
    {
        auto getter = temperature1->getPtr(static_cast<uint16_t>(board::eResourcesList::eDS18B20_1),boardResources);
        if (getter.second == eError::eOk)
        {
            temperature1 = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::sensor::ITemperatureSensor> temperature2{nullptr};
    {
        auto getter = temperature2->getPtr(static_cast<uint16_t>(board::eResourcesList::eDS18B20_2),boardResources);
        if (getter.second == eError::eOk)
        {
            temperature2 = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::sensor::ITemperatureSensor> temperature3{nullptr};
    {
        auto getter = temperature3->getPtr(static_cast<uint16_t>(board::eResourcesList::eDS18B20_3),boardResources);
        if (getter.second == eError::eOk)
        {
            temperature3 = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::sensor::ITemperatureSensor> temperature4{nullptr};
    {
        auto getter = temperature4->getPtr(static_cast<uint16_t>(board::eResourcesList::eDS18B20_4),boardResources);
        if (getter.second == eError::eOk)
        {
            temperature4 = getter.first;
        } else { errHandler();}
    }

    std::shared_ptr<hal::uart::IUart> uart1{nullptr};
    {
        auto getter = uart1->getPtr(static_cast<uint16_t>(board::eResourcesList::eUART1),boardResources);
        if (getter.second == eError::eOk)
        {
            uart1 = getter.first;
        } else { errHandler();}
    }

    auto log = fastLogger(uart2);
    auto logBluetooth = fastLogger(uart1);

    float temperature{};

    // uint8_t testSensorAddress[8] = {0x28, 0xda, 0x20, 0xbd, 0x00, 0x00, 0x00, 0x00};
    // uint8_t lowestSensorAddress[8] = {0x28, 0xe8, 0x3b, 0xbd, 0x00, 0x00, 0x00, 0x00};
    // uint8_t middleSensorAddress[8] = {0x28, 0xb9, 0xf5, 0xbf, 0x00, 0x00, 0x00, 0x00};
    // uint8_t highestSensorAddress[8] = {0x28, 0xbc, 0xe6, 0xbf, 0x00, 0x00, 0x00, 0x00};

    // testSensorAddress[7] = crc8(testSensorAddress,7);
    // lowestSensorAddress[7] = crc8(lowestSensorAddress,7);
    // middleSensorAddress[7] = crc8(middleSensorAddress,7);
    // highestSensorAddress[7] = crc8(highestSensorAddress,7);

    // temperature1->setAddress(testSensorAddress);
    // temperature2->setAddress(lowestSensorAddress);
    // temperature3->setAddress(middleSensorAddress);
    // auto err = temperature4->setAddress(highestSensorAddress);

    const uint64_t u64testSensorAddress=0xbd20da28;
    const uint64_t u64lowestSensorAddress=0xbd3be828;
    const uint64_t u64middleSensorAddress=0xbff5b928;
    const uint64_t u64highestSensorAddress=0xbfe6bc28;


    logBluetooth.log("[0]: sensor address: 0x%llx\r\n", u64testSensorAddress);
    logBluetooth.log("[1]: sensor address: 0x%llx\r\n", u64lowestSensorAddress);
    logBluetooth.log("[2]: sensor address: 0x%llx\r\n", u64middleSensorAddress);
    logBluetooth.log("[3]: sensor address: 0x%llx\r\n", u64highestSensorAddress);
    logBluetooth.log("\r\n");

    log.log("[0]: sensor address: 0x%llx\r\n", u64testSensorAddress);
    log.log("[1]: sensor address: 0x%llx\r\n", u64lowestSensorAddress);
    log.log("[2]: sensor address: 0x%llx\r\n", u64middleSensorAddress);
    log.log("[3]: sensor address: 0x%llx\r\n", u64highestSensorAddress);
    log.log("\r\n");

    logBluetooth.log("[0]: test sensor\r\n");
    logBluetooth.log("[1]: lowest sensor\r\n");
    logBluetooth.log("[2]: middle sensor\r\n");
    logBluetooth.log("[3]: top sensor\r\n");
    logBluetooth.log("\r\n");

    log.log("[0]: test sensor\r\n");
    log.log("[1]: lowest sensor\r\n");
    log.log("[2]: middle sensor\r\n");
    log.log("[3]: top sensor\r\n");
    log.log("\r\n");
    
    eError err;
    while (true)
    {
        ledGreen->toggle();
        
        err = temperature1->getTemperature(&temperature);
        if (err == eError::eOk)
        {
            log.log("[0]: %.2fC\r\n", temperature);
            logBluetooth.log("[0]: %.2fC\r\n", temperature);
        }

        err = temperature2->getTemperature(&temperature);
        if (err == eError::eOk)
        {
            log.log("[1]: %.2fC\r\n", temperature);
            logBluetooth.log("[1]: %.2fC\r\n", temperature);
        }

        err = temperature3->getTemperature(&temperature);
        if (err == eError::eOk)
        {
            log.log("[2]: %.2fC\r\n", temperature);
            logBluetooth.log("[2]: %.2fC\r\n", temperature);
        }

        err = temperature4->getTemperature(&temperature);
        if (err == eError::eOk)
        {
            log.log("[3]: %.2fC\r\n", temperature);
            logBluetooth.log("[3]: %.2fC\r\n", temperature);
        }

        log.log("\r\n");
        logBluetooth.log("\r\n");

        mgDelay->delayMs(500);
    }

    return 0;
}
