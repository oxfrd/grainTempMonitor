//
// Created by oxford on 30.05.23.
//
#include <chrono>

#include "boardInit.h"
#include "main.h"
#include "delay.h"

#include "IComponent.h"
#include "IGpioOutput.h"
#include "IInterrupt.h"
#include "IUart.h"
#include "IGpioInput.h"
#include "IPressureSensor.h"
#include "timeInterrupt.h"
#include "IGpioOutAndInput.h"
#include "IPort.h"
#include "IDelay.h"

#pragma GCC optimize ("O0")
using namespace std::chrono_literals;

std::shared_ptr<hal::delay::IDelay> mgDelay{nullptr};

static void errHandler()
{
    while(1)
    {
        asm("NOP");
    }
}

void letsPlayAGame(std::shared_ptr<hal::gpio::IGpioOutAndInput> pin,
                    std::shared_ptr<hal::gpio::IGpioOutput> ledGreen)
{
    bool pingPong = true;
    pin->setPinMode(hal::gpio::eMode::eInput);
    
    if (pin->getState() == true)
    {
        pin->setPinMode(hal::gpio::eMode::eOutput);
        
        for(int i = 0 ; i < 8 ; i++)
        {
            ledGreen->toggle();
            if (pingPong == true)
            {
                pin->on();
                pingPong = false;
            }
            else
            {
                pin->off();
                pingPong = true;
            }
            mgDelay->delayMs(500ms);
        }

        pin->setPinMode(hal::gpio::eMode::eInput);
    }

    return;
}

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

    std::shared_ptr<hal::gpio::IGpioOutAndInput> D0{nullptr};
    {
        auto getter = D0->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_D0),boardResources);
        if (getter.second == eError::eOk)
        {
            D0 = getter.first;
        } else { errHandler();}
    }

    {
        auto getter = mgDelay->getPtr(static_cast<uint16_t>(board::eResourcesList::eDelay),boardResources);
        if (getter.second == eError::eOk)
        {
            mgDelay = getter.first;
        } else { errHandler();}
    }

    while (true)
    {
        ledRed->toggle();
        mgDelay->delayMs(10ms);
    }

    return 0;
}