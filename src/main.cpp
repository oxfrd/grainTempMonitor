//
// Created by oxford on 30.05.23.
//
#include <vector>
#include "IComponent.h"
#include "boardInit.h"
#include "main.h"
#include "delay.h"
//#include "IPressureSensor.h"
#include "string.h"

#include "IGpioOutput.h"

static void errHandler()
{
    while(1)
    {
        asm("NOP");
    }
}

// static void func(std::shared_ptr<hal::mcu::mcuManager> x)
// {
//     using namespace mcu;

//     std::shared_ptr<gpio::gpioOutput> led{nullptr};
//     auto getter = led->getPtr(static_cast<uint16_t>(eResourcesList::eGPIO_B2), x);
//     if (getter.second == eError::eOk)
//     {
//         led = std::dynamic_pointer_cast<gpio::gpioOutput>(getter.first);
//     }

//     led->toggle();
//     delayMe(40);
//     led->toggle();
//     delayMe(40);
//     led->toggle();
//     delayMe(40);
// }

int main()
{
    auto boardResources = board::init();
    
    std::shared_ptr<hal::gpio::IGpioOutput> ledRed{nullptr};
    {
        auto getter = ledRed->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_B2),boardResources);
        if (getter.second == eError::eOk)
        {
            ledRed = std::dynamic_pointer_cast<hal::gpio::IGpioOutput>(getter.first);
        } else { errHandler();}
    }

    std::shared_ptr<hal::gpio::IGpioOutput> ledGreen{nullptr};
    {
        auto getter = ledGreen->getPtr(static_cast<uint16_t>(board::eResourcesList::eGPIO_E8),boardResources);
        if (getter.second == eError::eOk)
        {
            ledGreen = std::dynamic_pointer_cast<hal::gpio::IGpioOutput>(getter.first);
        } else { errHandler();}
    }
    
    /*
    std::shared_ptr<gpio::gpioInput> A0{nullptr};
    {
        auto getter = A0->getPtr(static_cast<uint16_t>(eResourcesList::eGPIO_A0),mcu);
        if (getter.second == eError::eOk)
        {
            A0 = std::dynamic_pointer_cast<gpio::gpioInput>(getter.first);
        } else { errHandler();}
    }

    std::shared_ptr<gpio::gpioInput> A1{nullptr};
    {
        auto getter = A1->getPtr(static_cast<uint16_t>(eResourcesList::eGPIO_A1),mcu);
        if (getter.second == eError::eOk)
        {
            A1 = std::dynamic_pointer_cast<gpio::gpioInput>(getter.first);
        } else { errHandler();}
    }
    
    std::shared_ptr<gpio::gpioInput> A2{nullptr};
    {
        auto getter = A2->getPtr(static_cast<uint16_t>(eResourcesList::eGPIO_A2),mcu);
        if (getter.second == eError::eOk)
        {
            A2 = std::dynamic_pointer_cast<gpio::gpioInput>(getter.first);
        } else { errHandler();}
    }
    
    std::shared_ptr<gpio::gpioInput> A3{nullptr};
    {
        auto getter = A3->getPtr(static_cast<uint16_t>(eResourcesList::eGPIO_A3),mcu);
        if (getter.second == eError::eOk)
        {
            A3 = std::dynamic_pointer_cast<gpio::gpioInput>(getter.first);
        } else { errHandler();}
    }
    
    std::shared_ptr<gpio::gpioInput> A5{nullptr};
    {
        auto getter = A5->getPtr(static_cast<uint16_t>(eResourcesList::eGPIO_A5),mcu);
        if (getter.second == eError::eOk)
        {
            A5 = std::dynamic_pointer_cast<gpio::gpioInput>(getter.first);
        } else { errHandler();}
    }
    std::shared_ptr<interrupt::timeInterrupt> interrupt{nullptr};
    {
        auto getter = interrupt->getPtr(static_cast<uint16_t>(eResourcesList::eIntTim2),mcu);
        if (getter.second == eError::eOk)
        {
            interrupt = std::dynamic_pointer_cast<interrupt::timeInterrupt>(getter.first);
        } else { errHandler();}
    }

    std::shared_ptr<uart::uart> uart{nullptr};
    {
        auto getter = uart->getPtr(static_cast<uint16_t>(eResourcesList::eUART2),mcu);
        if (getter.second == eError::eOk)
        {
            uart = std::dynamic_pointer_cast<uart::uart>(getter.first);
        } else { errHandler();}
    }

    std::shared_ptr<hal::sensor::IPressureSensor> pressureSensor{nullptr};
    {
        auto getter = pressureSensor->getPtr(static_cast<uint16_t>(eResourcesList::eBMP280),mcu);
        if (getter.second == eError::eOk)
        {
            pressureSensor = std::dynamic_pointer_cast<hal::sensor::IPressureSensor>(getter.first);
        } else { errHandler();}
    }

    interrupt->enable();

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
    */
    while (true)
    {
        /*
        a0State = A0->getState();
        a1State = A1->getState();
        a2State = A2->getState();
        a3State = A3->getState();
        a5State = A5->getState();
        */
        ledRed->toggle();
/*
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
        sprintf((char*)temp, "Press:%.2fhPa\n", pressure);
        uart->send(temp, 18);

        pressureSensor->getTemperature(&temperature);
        sprintf((char*)temp, "Temp:%.2fC\n", temperature);
        uart->send(temp, 12);
        delayMe(500);
        
        while(a0State || a1State || a2State || a3State || a5State)
        {
            constexpr std::uint32_t x{500};
            ledGreen->toggle();
            delayMe(x);
            break;
        }
        err = uart->get(&newByte, 1);

        if((err == eError::eOk) && (newByte == '5'))
        {
            ledGreen->toggle();
        }*/
        delayMe(500);
    } 
    return 0;
}