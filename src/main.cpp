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

    std::shared_ptr<hal::gpio::IGpioInput> A0{nullptr};
    {
        auto getter = A0->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_A0),boardResources);
        if (getter.second == eError::eOk)
        {
            A0 = std::dynamic_pointer_cast<hal::gpio::IGpioInput>(getter.first);
        } else { errHandler();}
    }

    std::shared_ptr<hal::gpio::IGpioInput> A1{nullptr};
    {
        auto getter = A1->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_A1),boardResources);
        if (getter.second == eError::eOk)
        {
            A1 = std::dynamic_pointer_cast<hal::gpio::IGpioInput>(getter.first);
        } else { errHandler();}
    }
    
    std::shared_ptr<hal::gpio::IGpioInput> A2{nullptr};
    {
        auto getter = A2->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_A2),boardResources);
        if (getter.second == eError::eOk)
        {
            A2 = std::dynamic_pointer_cast<hal::gpio::IGpioInput>(getter.first);
        } else { errHandler();}
    }
    
    std::shared_ptr<hal::gpio::IGpioInput> A3{nullptr};
    {
        auto getter = A3->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_A3),boardResources);
        if (getter.second == eError::eOk)
        {
            A3 = std::dynamic_pointer_cast<hal::gpio::IGpioInput>(getter.first);
        } else { errHandler();}
    }
    
    std::shared_ptr<hal::gpio::IGpioInput> A5{nullptr};
    {
        auto getter = A5->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_A5),boardResources);
        if (getter.second == eError::eOk)
        {
            A5 = std::dynamic_pointer_cast<hal::gpio::IGpioInput>(getter.first);
        } else { errHandler();}
    }

    std::shared_ptr<hal::sensor::IPressureSensor> pressureSensor{nullptr};
    {
        auto getter = pressureSensor->getPtr(static_cast<uint16_t>(board::eResourcesList::eBMP280),boardResources);
        if (getter.second == eError::eOk)
        {
            pressureSensor = std::dynamic_pointer_cast<hal::sensor::IPressureSensor>(getter.first);
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

    auto a0State = false;
    auto a1State = false;
    auto a2State = false;
    auto a3State = false;
    auto a5State = false;
    
    uint8_t text[12] = "yee buddy\n\r";
    uint8_t text2[11] = "lajtlejt\n\r";
    bool tick = true;
    std::uint8_t newByte = 0;
    eError err = eError::eUninitialized;
    float pressure{0};
    float temperature{0};
    uint8_t temp[30];
    uint8_t sizeToSend{};

    while (true)
    {
        
        a0State = A0->getState();
        a1State = A1->getState();
        a2State = A2->getState();
        a3State = A3->getState();
        a5State = A5->getState();
        
        ledRed->toggle();
        
        if(tick)
        {
            uart->send(text, 12);
            tick = false;
        }
        else
        {
            uart->send(text2, 11);
            tick = true;
        }

        pressureSensor->getPressure(&pressure);
        pressure /= 100; // Pa -> hPa
        sizeToSend = sprintf((char*)temp, "Press:%.2fhPa\n", pressure);
        uart->send(temp, sizeToSend);

        pressureSensor->getTemperature(&temperature);
        sizeToSend = sprintf((char*)temp, "Temp:%.2fC\n", temperature);
        uart->send(temp, sizeToSend);
        mgDelay->delayMs(500ms);
        
        while(a0State || a1State || a2State || a3State || a5State)
        {
            ledGreen->toggle();
            mgDelay->delayMs(500ms);
            break;
        }
        err = uart->get(&newByte, 1);

        if((err == eError::eOk) && (newByte == '5'))
        {
            ledGreen->toggle();
        }

        letsPlayAGame(D0, ledGreen);
        mgDelay->delayMs(500ms);
    } 
    return 0;
}