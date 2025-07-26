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

    std::shared_ptr<hal::uart::IUart> uart{nullptr};
    {
        auto getter = uart->getPtr(static_cast<uint16_t>(board::eResourcesList::eUART2),boardResources);
        if (getter.second == eError::eOk)
        {
            uart = getter.first;
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

    auto log = fastLogger(uart);
    std::uint64_t address{};

    auto err = temperature1->getAddress(&address);
    if (err == eError::eOk)
    {
        ledRed->off();
        log.log("[0]: sensor address: 0x%llx\r\n", address);
    }
    else
    {
        ledRed->on();
    }

    float temperature{};

    while (true)
    {
        ledGreen->toggle();
        err = temperature1->getTemperature(&temperature);
        log.log("[0]: %.2fC\r\n", temperature);

        mgDelay->delayMs(500);
    }

    return 0;
}