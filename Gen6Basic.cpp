/*!
 *  \file      Gen6Basic.cpp
 *
 *  \brief     Содержит реализацию класса Gen6Basic
 *
 */

//! Подключение заголовочных файлов

#include "Gen6Basic.h"

//! Реализация класса

const char *FW_LOAD_ERROR::FW_LOAD_OK              = "FIRMWARE_LOAD_OK";
const char *FW_LOAD_ERROR::FW_LOAD_ERROR_SL_MASTER = "FIRMWARE_LOAD_ERROR_1";
const char *FW_LOAD_ERROR::FW_LOAD_ERROR_NOT_FOUND = "FIRMWARE_LOAD_ERROR_2";
const char *FW_LOAD_ERROR::FW_LOAD_ERROR_ARG       = "FIRMWARE_LOAD_ERROR_3";

Gen6Basic::~Gen6Basic()
{
    if (p_r6 != NULL)
        delete p_r6;           
    
    if (p_r4 != NULL)
        delete p_r4;           
    
    if (p_led_keyring != NULL)
        delete p_led_keyring;  
    
    if (p_lcd_keyring != NULL)
        delete p_lcd_keyring;  
    
    if (p_tag != NULL)
        delete p_tag;          
    
    if (p_service_btn != NULL)
        delete p_service_btn;  
    
    if (p_gsm != NULL)
        delete p_gsm;          
    
    if (p_can_actions != NULL)
        delete p_can_actions;  
    
    if (p_st_online != NULL)
        delete p_st_online;    
    
    if (p_slp != NULL)
        delete p_slp;          
    
    if (p_serial != NULL)
        delete p_serial;  

    if (p_temp_sensor != NULL)
        delete p_temp_sensor;     
}

void Gen6Basic::initSigmaEnable()
{
    p_sigma_enable = new DigitalOut(SIGMA_ENABLE);

    sigmaSettingMode();
    printf("SIGMA INIT\n");
}

Gen6Basic::Gen6Basic(const Gen6Basic &right)
{
    *this = right;
}

Gen6Basic &Gen6Basic::operator=(const Gen6Basic &right)
{
    if (this == &right)
    {
        return *this;
    }
    
    DevicesBasic::operator=(right);
    Gen6Basic::~Gen6Basic();

    m_device_modules = right.m_device_modules;

    if (right.p_temp_sensor != NULL)
        p_temp_sensor = new AnalogOut(*right.p_temp_sensor);

    strcpy(m_device_pin, right.m_device_pin);

    return *this;
}

bool Gen6Basic::getVcc()
{
    return (bool)*p_vcc;
}

void Gen6Basic::on()
{
    printf("INFO Включение основного блока\n");
    *p_vcc = ON;
}

void Gen6Basic::off()
{
    printf("INFO Выключение основного блока\n");
    *p_vcc = OFF;
}

void Gen6Basic::sigmaSettingMode()
{
    printf("INFO Перевод ACK-лки в режим приема настроек\n");

    if (m_device_name == GEN6_S96)
    {
        *p_sigma_enable = 1;
    }
    else
    {
        *p_sigma_enable = 0;
    }

    wait_s(TIME_DELAY::THREE_S);
}

void Gen6Basic::sigmaWorkMode()
{
    printf("INFO Перевод ACK-лки в режим работы\n");

    if (m_device_name == GEN6_S96)
    {
        *p_sigma_enable = 0;
    }
    else
    {
        *p_sigma_enable = 1;
    }

    wait_s(TIME_DELAY::THREE_S);
    
    // Сообщения необходимые, чтобы разбудить ACK-лку
    for (uint8_t i = 0; i < 5; ++i)
    {
        p_can_actions->sendWakeUp();
    }
}

void Gen6Basic::addTempSensor(AnalogOut *temp_sensor)
{
    p_temp_sensor = temp_sensor;
}

void Gen6Basic::addKeyring(LedKeyring *ledkeyring)
{
    p_led_keyring = ledkeyring;
}

void Gen6Basic::addKeyring(LcdKeyring *lcdkeyring)
{
    p_lcd_keyring = lcdkeyring;
}

void Gen6Basic::addTag(SmartTag *tag)
{
    p_tag = tag;
}

void Gen6Basic::addR6(R6 *r6)
{
    p_r6 = r6;
}

void Gen6Basic::addR6Eco(R6Eco *r6eco)
{
    p_r6eco = r6eco;
}

void Gen6Basic::addR4(R4 *r4)
{
    p_r4 = r4;
}

void Gen6Basic::addGsm(Sim900 *sim900)
{
    p_gsm = sim900;
}

void Gen6Basic::addCan(CanBusActions *can_actions)
{
    p_can_actions = can_actions;
}

CanBus* Gen6Basic::getCan1()
{
    return p_can_actions->getCan1();
}

CanBusActions* Gen6Basic::getCanBusActions()
{
    return p_can_actions;
}

void Gen6Basic::addServiceButton(ServiceButton *btn)
{
    p_service_btn = btn;
}

void Gen6Basic::addStarlineOnline(StarlineOnline *st_online)
{
    p_st_online = st_online;
}

void Gen6Basic::addSlpCmd(SlpCmd *slp)
{
    p_slp = slp;
}

void Gen6Basic::addSerial(Serial *serial)
{
    p_serial = serial;
}

void Gen6Basic::addDeviceName(devices_t device_name)
{
    m_device_name = device_name;
}

devices_t Gen6Basic::getDeviceName()
{
    return m_device_name;
}

void Gen6Basic::addModules(int32_t modules)
{
    m_device_modules = modules;
    printf("DEBUG Добавлены модули сновного блока: %#.2x\n", m_device_modules);
}

void Gen6Basic::addDevicePhone(const char *device_phone)
{
    m_gen6_phones.insert(m_gen6_phones.end(), device_phone);
    printf("DEBUG Добавлен телефон основного блока: %s\n", device_phone);
}

void Gen6Basic::addMobileApp(MobileApp *app)
{
    p_mobileapp = app;
}

void Gen6Basic::addValidator(Validator *validator)
{
    p_validator = validator;
}

vector<const char *> &Gen6Basic::getDevicePhones()
{
    return m_gen6_phones;
}

bool Gen6Basic::authStarlineOnline()
{
    return p_st_online->auth();
}

bool Gen6Basic::addStarlineOnlineDevice(const char *tel,
                                        const char *name,
                                        const char *device_type)
{
    return p_st_online->addDevice(tel, name, device_type);
}

bool Gen6Basic::setStarlineOnlineParam(StarlineOnline::command_type_t command_type, bool value, char *pin)
{
    return p_st_online->setParam(command_type, value, pin);
}

sms_state_t Gen6Basic::outputSms(float timeout_s)
{
    sms_state_t result;


    printf("DEBUG Ожидание СМС (без проверки), время ожидания: %.2f сек.\n", timeout_s);

    result = p_gsm->outputSms(timeout_s);

    return result;
}

sms_state_t Gen6Basic::waitSms(const char *checkmsg)
{
    return p_gsm->waitSms(checkmsg);
}

sms_state_t Gen6Basic::waitSmsFor(const char *checkmsg, float time)
{
    return p_gsm->waitSmsCmpFor(checkmsg, strlen(checkmsg), time);
}

sms_state_t Gen6Basic::inspectionSms(const char *to,
                                     const char *message,
                                     const char *checkmsg)
{
    return p_gsm->inspectionSms(to, message, checkmsg);
}

sms_state_t Gen6Basic::inspectionSmsFor(const char *to,
                                        const char *message,
                                        const char *checkmsg,
                                        float       time)
{
    return p_gsm->inspectionSmsFor(to,
                                   message,
                                   checkmsg,
                                   time);
}

Sim900 *Gen6Basic::getSim900()
{
    return p_gsm;
}

StarlineOnline *Gen6Basic::getStarlineOnline()
{
    return p_st_online;
}

void Gen6Basic::pushLedKeyringBut1(float type)
{
    printf("DEBUG Нажатие кнопки 1 доп. брелка\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED брелок не определен\n");
        return;
    }

    p_led_keyring->pushButton1(type);
}

void Gen6Basic::pushLedKeyringBut2(float type)
{
    printf("DEBUG Нажатие кнопки 2 доп. брелка\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED брелок не определен\n");
        return;
    }

    p_led_keyring->pushButton2(type);
}

void Gen6Basic::pushLedKeyringBut3(float type)
{
    printf("DEBUG Нажатие кнопки 3 доп. брелка\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED брелок не определен\n");
        return;
    }

    p_led_keyring->pushButton3(type);
}

void Gen6Basic::pushLcdKeyringBut1(float type)
{
    printf("DEBUG Нажатие кнопки 1 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButton1(type);
}

void Gen6Basic::pushLcdKeyringBut2(float type)
{
    printf("DEBUG Нажатие кнопки 2 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButton2(type);
}

void Gen6Basic::pushLcdKeyringBut3(float type)
{
    printf("DEBUG Нажатие кнопки 3 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButton3(type);
}

void Gen6Basic::pushLcdKeyringBut4(float type)
{
    printf("DEBUG Нажатие кнопки 4 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButton4(type);
}

void Gen6Basic::pushLedKeyringBut23(float type)
{
    printf("DEBUG Нажатие кнопок 2 и 3 доп. брелка\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED брелок не определен\n");
        return;
    }

    p_led_keyring->pushButtons23(type);
}

void Gen6Basic::pushLcdKeyringBut23(float type)
{
    printf("DEBUG Нажатие кнопок 2 и 3 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButtons23(type);
}

void Gen6Basic::pushLcdKeyringBut14(float push_time_s)
{
    printf("DEBUG Нажатие кнопок 1 и 4 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButtons14(push_time_s);
}

void Gen6Basic::pushLcdKeyringBut24(float push_time_s)
{
    printf("DEBUG Нажатие кнопок 2 и 4 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButtons24(push_time_s);
}

void Gen6Basic::pushLcdKeyringBut34(float push_time_s)
{
    printf("DEBUG Нажатие кнопок 3 и 4 ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButtons34(push_time_s);
}

void Gen6Basic::setPanicKeyring()
{
    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED брелок не определен\n");
        return;
    }

    p_led_keyring->pushButton1(KEYRING_SHORT);
    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
    p_led_keyring->pushButton1(KEYRING_SHORT);
}

void Gen6Basic::setHandBreak(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Ручник поднят\n" 
                                : "INFO Ручник опущен\n";

    printf("%s", message);

    if (p_out_hand_break == NULL)
    {
        execution_method = CAN_USE;
    }

    switch (execution_method)
    {
        case CAN_USE:
            (value == ON) ? p_can_actions->sendHandBreakOn()
                          : p_can_actions->sendHandBreakOff();

            break;

        case PIN:
            (value == ON) ? *p_out_hand_break = 1
                          : *p_out_hand_break = 0;

            break;

        default:
            printf("FAIL Управление ручником: метод управления не определен\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setDoor(int8_t         value,
                        command_type_t execution_method, 
                        float          delay)
{
    const char* message = value ? "INFO Дверь открыта\n" : "INFO Дверь закрыта\n";

    printf("%s", message);
    execution_method = getExecutionMethod(execution_method, CAR_ACTION, p_out_door);

    switch (execution_method)
    {
        case CAN_USE:
            value ? p_can_actions->sendDoorOpen()
                  : p_can_actions->sendDoorClose();
            break;

        case PIN:
            (value == ON) ? *p_out_door = 1
                          : *p_out_door = 0;

            break;

        default:
            printf("FAIL Управление дверью: метод управления не определен\n");
            break;
    }
    wait(delay);
}

void Gen6Basic::setEngine(int8_t value, command_type_t execution_method, int16_t rpm_val, float delay)
{
    const char* message = value ? "INFO Двигателель включен (поданы обороты 1000)\n"
                                : "INFO Двигатель выключен (сняты обороты)\n";
    printf("%s", message);

    if (p_pwm == NULL)
    {
        execution_method = CAN_USE;
    }

    switch (execution_method)
    {
        case CAN_USE:
            if (value == ON)
            {
                p_can_actions->sendEngineOn();
            }
            else
            {
                p_can_actions->sendEngineOff();
            }

            break;

        case PIN:
            setRpm(rpm_val, PIN, TIME_DELAY::NULL_DELAY_S);
            p_pwm->pulsewidth_ms(value);
            break;

        default:
            printf("FAIL Управление оборотами: метод управления не определен\n");
            break;
    }

    wait_s(delay);
}

void Gen6Basic::setRpm(int16_t rpm_val, command_type_t execution_method, float delay)
{
    printf("INFO Установлены обороты значение = %d\n", rpm_val);

    if (p_pwm == NULL || m_device_name == GEN6_M96)
    {
        execution_method = CAN_USE;
    }

    switch (execution_method)
    {
        case CAN_USE:
            (rpm_val > 0) ? setIgn(CAN_USE, ON)
                          : setIgn(CAN_USE, OFF);

            p_can_actions->sendSetRpm(rpm_val);
            break;

        case PIN:
            p_pwm->period_ms(rpm_val);
            p_pwm->pulsewidth_ms(1);
            break;

        default:
            printf("FAIL Управление оборотами: метод управления не определен\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setHood(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Капот открыт\n" : "INFO Капот закрыт\n";

    printf("%s", message);

    if (   ((m_device_modules & BLE_MODULE) == BLE_MODULE)
        && (p_out_hood                      == NULL)
        && (execution_method                != CAN_USE))
    {
        execution_method = THROUGH_R6;
    }
    else
    {
        if (p_out_hood == NULL)
        {
            execution_method = CAN_USE;
        }
        else
        {
            execution_method = PIN;
        }
    }

    switch (execution_method)
    {
        case PIN:
            (value == ON) ? *p_out_hood = 1
                          : *p_out_hood = 0;
            break;
        
        case THROUGH_R6:
            (value == ON) ? p_r6->setInput()
                          : p_r6->resetInput();
            break;
                
        case CAN_USE:
           if (value != 0)
           {
               p_can_actions->sendHoodOpen();
           }
           else
           {
               p_can_actions->sendHoodClose();
           }

           break;

        default:
            printf("FAIL Управление капотом: метод управления не определен\n");
            break;
    }

    wait(delay);
}

void Gen6Basic::setKillEngine(int8_t value, float delay_s)
{
    if (p_out_killengine == NULL)
    {
        printf("FAIL Запись состояния. Пин \"Вход запрета запуска двигателя\" не определен\n");
        return;
    }
    
    const char* message = value ? "INFO Включение входа запрета запуска\n" 
                                : "INFO Выключение входа запрета запуска\n";
    printf("%s", message);

    (value == ON) ? *p_out_killengine = 1
                  : *p_out_killengine = 0;

//    setDigOutByFunc(p_out_killengine, state, ID_IN_ENG_START_FORBID);
    wait(delay_s);
}

bool Gen6Basic::setTrunk(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Багажник открыт\n" : "INFO Багажник закрыт\n";


    printf("%s", message);

    execution_method = getExecutionMethod(execution_method, CAR_ACTION, p_out_trunk);

    switch (execution_method)
    {
        case CAN_USE:
            value ? p_can_actions->sendTrunkOpen()
                  : p_can_actions->sendTrunkClose();
            break;

        case PIN:
            (value == ON) ? *p_out_trunk = 1
                          : *p_out_trunk = 0;
            break;
            
        case SLO:
            break;            

        default:
            printf("FAIL Управление оборотами: метод управления не определен\n");
            break;
    }

    wait(delay);    

    return true;
}

void Gen6Basic::setIgn(command_type_t         execution_method,
                       int8_t                 value,
                       float                  delay_s,
                       reg_queue_leave_type_t reg_queue_leave_type)
{
    const char* message = value ? "INFO Зажигание включено\n"
                                : "INFO Зажигание выключено\n";


    printf("%s", message);

    execution_method = getExecutionMethod(execution_method,
                                          CAR_ACTION,
                                          p_out_ign);

    switch (execution_method)
    {
        case PIN:
            (value == ON) ? *p_out_ign = 1
                          : *p_out_ign = 0;
            break;

       case CAN_USE:
           (value == ON) ? p_can_actions->sendIgnOn()
                         : p_can_actions->sendIgnOff();
            break;

        default:
            printf("FAIL Неподдерживаемый метод включения зажигания\n");
            break;
    }

    if (   value                == OFF
        && reg_queue_leave_type == IGN_OFF_REG_QUEUE_LEAVE)
    {
        RegistrationQueue::removeFromQueueNoAnsw();
    }

    wait(delay_s);
}

void Gen6Basic::setRelayIgn(RelayBase *relay, bool status)
{
    if (relay == NULL)
    {
        printf("FAIL Реле не задано\n");
        return;
    }

    if (status)
    {
        relay->setIgn();
    }
    else
    {
        relay->resetIgn();
    }
}

void Gen6Basic::setR6Ign()
{
    printf("INFO Включение контакта IGN реле R6\n");

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return;
    }

    setRelayIgn(p_r6, ON);
}

void Gen6Basic::resetR6Ign()
{
    printf("INFO Выключение контакта IGN реле R6\n");

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return;
    }

    setRelayIgn(p_r6, OFF);
}

void Gen6Basic::setR4Ign()
{
    printf("INFO Включение контакта IGN реле R4\n");

    if (p_r4 == NULL)
    {
        printf("FAIL Отсутствует R4\n");
        return;
    }

    setRelayIgn(p_r4, ON);
}

void Gen6Basic::resetR4Ign()
{
    printf("INFO Выключение контакта IGN реле R4\n");

    if (p_r4 == NULL)
    {
        printf("FAIL Отсутствует R4\n");
        return;
    }

    setRelayIgn(p_r4, OFF);
}

void Gen6Basic::setBreak(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Педаль тормоза нажата\n" : "INFO Педаль тормоза отпущена\n";

    static DigitalOut *or_purple = NULL;

    printf("%s", message);
    execution_method = getExecutionMethod(execution_method, CAR_ACTION, p_out_break);

    switch (execution_method)
    {
        case PIN:
            if (value == ON)
            {
                if (or_purple)
                {
                    delete or_purple;
                }

                *p_out_break = 1;
            }
            else
            {
                if (   (   (m_device_name == GEN6_A96)
                        || (m_device_name == GEN6_E96)
                        || (m_device_name == GEN6_S96))
                    && (or_purple == NULL))
                {
                    or_purple  = new DigitalOut(IN_X96_ORANGE_PURPLE);
                    *or_purple = 0;
                }

                *p_out_break = 0;
            }
            break;

        case CAN_USE:
            (value == ON) ? p_can_actions->sendBreakOn()
                          : p_can_actions->sendBreakOff();
            break;

        default:
            printf("FAIL Неподдерживаемый метод управления педалью тормоза\n");
            break;
    }

    wait(delay);
}

void Gen6Basic::setRLight(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Правый свет включен\n"
                                : "INFO Правый свет выключен\n";

    printf("%s", message);

    if (p_out_light_right != NULL)
    {
        (value == ON) ? *p_out_light_right = 1
                      : *p_out_light_right = 0;

        wait(delay);

        return;
    }

    printf("FAIL Управление левым поворотником: контакт не настроен\n");
}

void Gen6Basic::setLLight(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Левый свет включен\n"
                                : "INFO Левый свет выключен\n";

    printf("%s", message);

    if (p_out_light_left != NULL)
    {
        (value == ON) ? *p_out_light_left = 1
                      : *p_out_light_left = 0;

        wait(delay);

        return;
    }

    printf("FAIL Управление правым поворотником: контакт не настроен\n");
}

void Gen6Basic::setCLock(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ЦЗ включен\n"
                                : "INFO ЦЗ выключен\n";

    printf("%s", message);

    if (p_out_central_lock_open != NULL)
    {
        (value == ON) ? *p_out_central_lock_open = 0
                      : *p_out_central_lock_open = 1;
    }
    else
    {
        printf("INFO Открытие ЦЗ: контакт не настроен\n");
    }

    if (p_out_central_lock_close != NULL)
    {
        (value == ON) ? *p_out_central_lock_close = 1
                      : *p_out_central_lock_close = 0;
    }
    else
    {
        printf("INFO Закрытие ЦЗ: контакт не настроен\n");
    }

    wait(delay);
}

void Gen6Basic::setPpp(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Предпусковой подогреватель включен\n"
                                : "INFO Предпусковой подогреватель выключен\n";

    printf("%s", message);
    execution_method = getExecutionMethod(execution_method, CAR_ACTION, p_out_ppp);

    switch (execution_method)
    {
        case PIN:
            (value == ON) ? *p_out_ppp = 1
                          : *p_out_ppp = 0;
            break;

        case CAN_USE:
            
            break;

        default:
            printf("FAIL Управление ППП: контакт не настроен\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setParking(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO Паркинг включен\n" 
                                : "INFO Паркинг выключен\n";

    printf("%s", message);
    execution_method = getExecutionMethod(execution_method, CAR_ACTION, p_out_parking);

    switch (execution_method)
    {
        case PIN:
            (value == ON) ? *p_out_parking = 1
                          : *p_out_parking = 0;
            break;

        case CAN_USE:
            (value != 0) ? p_can_actions->sendParkingOn() 
                         : p_can_actions->sendParkingOff();
            break;

        default:
            printf("FAIL Неподдерживаемый метод управления Паркингом\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setSecure(int8_t             value,
                          command_type_t     execution_method,
                          float              delay_s,
                          tag_voltage_ctrl_t tag_voltage_ctrl)
{
    const char* message = value ? "INFO Постановка в охрану\n"
                                : "INFO Снятие с охраны\n";


    printf("%s", message);
    execution_method = getExecutionMethod(execution_method, USER_ACTION);

    switch (execution_method)
    {
        case KEYRING:

            (value == ON) ? pushLedKeyringBut1(KEYRING_SHORT)
                          : pushLedKeyringBut2(KEYRING_SHORT);

            break;

        case TAG:

            if (tag_voltage_ctrl == TAG_VOLTAGE_CTRL_EN)
            {
                setTagVcc();
     
                // костыль - задержка в 10 сек. вместо ожидания статуса метки, 
                // т.к. иначе не проходят тесты на циклы тревоги
                wait(TIME_DELAY::DELAY_AFTER_TAG_VCC_EN_S);
            }

            for (uint8_t i = 0; i < 2; ++i)
            {
                if (value == ON)
                {
                    if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                         SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                         DIAG_GUARD_STATE_ARM))
                    {
                        printf("INFO Система не в охране\n");
                        pushTagBtn(TAG_SHORT);
                        wait(TIME_DELAY::DELAY_AFTER_PUSH_TAG_BTN_S);
                    }
                }
                else
                {
                    if (   !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                            DIAG_GUARD_STATE_DISARM)
                        && !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                            DIAG_GUARD_STATE_IMMO)          // запрет поездки
                        && !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                            DIAG_GUARD_STATE_WAITING_AUTH)) // ожидание авторизации
                    {
                        printf("INFO Система не в \"Снято с охраны\"\n");
                        pushTagBtn(TAG_SHORT);
                        wait(TIME_DELAY::DELAY_AFTER_PUSH_TAG_BTN_S);
                    }
                }
            }

            break;
            
        case TAG_M66:

            if (value == OFF)
            {
                setTagVcc();
            }
            else
            {
                resetTagVcc();
            }
            
            break;
            
        case CAN_USE:

            if (value == ON)
            {
                p_can_actions->sendStdSecureOn();
            }
            else
            {
                p_can_actions->sendStdSecureOff();
            }
            
            break;
            
        case STD:

            if (value == ON)
            {
                p_tag->setVcc();
                wait(TIME_DELAY::DELAY_AFTER_TAG_VCC_EN_S);
                p_can_actions->sendStdSecureOn();
                p_tag->resetVcc();
            }
            else
            {
                p_tag->setVcc();
                wait(TIME_DELAY::DELAY_AFTER_TAG_VCC_EN_S);
                p_can_actions->sendStdSecureOff();
                p_tag->resetVcc();
            }

            break;

        case SMS:

            if (value == ON)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1], SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_PROT_ON);
                p_gsm->outputSms();
            }
            else
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1], SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_PROT_OFF);
                p_gsm->outputSms();
            }

            break;

        case MOBILE:

            if (value == ON)
            {
                p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_ARM);
            }
            else
            {
                p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_DISARM);
            }

            break;

        default:
            printf("FAIL Неподдерживаемый метод управления охраной: %d\n", execution_method);
            return;

    }

    wait_s(delay_s);

    if (   execution_method == TAG
        && tag_voltage_ctrl == TAG_VOLTAGE_CTRL_EN)
    {
        resetTagVcc();
    }
}

void Gen6Basic::setRemoteRun(command_type_t execution_method,
                             float          sms_delay_s)
{
    printf("INFO Запуск двигателя\n");
    execution_method = getExecutionMethod(execution_method, RUN_ACTION);
        
    switch (execution_method)
    {
        case KEYRING:
            p_led_keyring->pushButton1(KEYRING_LONG_2);
            break;

        case SMS:
            if (m_gen6_phones.size() > 0)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1], "21");
            }
            else
            {
                printf("FAIL Не задан телефон устройства\n");
            }

            waitInspector(sms_delay_s, 0);
            break;

        case STD:
            for (uint8_t i = 0; i < 3; i++)
            {
                setSecureNoDelay(ON, CAN_USE);
            }
            break;
                
        default:
            printf("FAIL Неизвестный метод запуска двигателя\n");
            return;
    }
}

void Gen6Basic::setRemoteStop(command_type_t execution_method, float sms_delay_s)
{
    printf("INFO Глушение двигателя\n");
    execution_method = getExecutionMethod(execution_method, RUN_ACTION);
        
    switch (execution_method)
    {
        case KEYRING:
            p_led_keyring->pushButton2(KEYRING_LONG_2);
            break;

        case SMS:

            if (m_gen6_phones.size() > 0)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                               SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_ENGINE_STOP);
            }
            else
            {
                printf("FAIL Не задан телефон устройства\n");
            }

            // Для системы с кнопкой Старт-Стоп с неподключенным зажиганием
            if (p_in_ign1 == NULL)
            {
                if (p_in_eng_start_stop_btn != NULL)
                {
                    waitStartStopBtn(sms_delay_s, getPolarity(ID_OUT_START_STOP_BTN));
                }
                else if (p_in_stop_btn != NULL)
                {
                    waitStopBtn(sms_delay_s, getPolarity(ID_OUT_STOP_BTN));
                }
            }
            else
            {
                waitIgn1(sms_delay_s, !getPolarity(ID_OUT_IGN1));
            }

            break;
                
        default:
            printf("FAIL Неизвестный метод глушения двигателя\n");
            return;
    }
}

void Gen6Basic::runEngine(command_type_t        execution_method,
                          engine_start_system_t engine_start_system,
                          bool                  acc_must_be_off,
                          int16_t               rpm_val)
{
    printf("INFO Дистанционный запуск двигателя \n");
    setRemoteRun(execution_method);

    switch (engine_start_system)
    {
        case IGNITION_LOCK:
        case EXTERNAL:
            checkIgnLockEngine(TIME_DELAY::STD_WIRE_EN_DELAY_S, acc_must_be_off);
            break;

        case START_STOP_ONE_IMPULSE:
            checkStartStopOneImpulseEngine(TIME_DELAY::STD_WIRE_EN_DELAY_S, rpm_val);
            break;

        case START_STOP_TWO_IMPULSE:
            checkStartStopTwoImpulseEngine(TIME_DELAY::STD_WIRE_EN_DELAY_S, rpm_val);
            break;

        case START_STOP_THREE_IMPULSE:
            checkStartStopThreeImpulseEngine(TIME_DELAY::STD_WIRE_EN_DELAY_S, rpm_val);
            break;

        default:
            printf("INFO Проверка запуска двигателя (после ДЗ): %d - не поддерживаемая система запуска\n",
                   engine_start_system);
            break;
    }
}

engine_start_error_t Gen6Basic::checkEngine(engine_start_system_t engine_start_system,
                                            float                 bypass_waiting_time_s,
                                            bool                  acc_must_be_off,
                                            log_label_t           log_label,
                                            command_type_t        execution_method)
{
    switch (engine_start_system)
    {
        case IGNITION_LOCK:
        case EXTERNAL:
            return checkIgnLockEngine(bypass_waiting_time_s, acc_must_be_off, log_label, execution_method);

        case START_STOP_ONE_IMPULSE:
            return checkStartStopOneImpulseEngine(bypass_waiting_time_s, RPM_ON, log_label);

        case START_STOP_TWO_IMPULSE:
            return checkStartStopTwoImpulseEngine(bypass_waiting_time_s, RPM_ON, log_label);

        case START_STOP_THREE_IMPULSE:
            return checkStartStopThreeImpulseEngine(bypass_waiting_time_s, RPM_ON, log_label);

        default:
            printf("INFO Проверка запуска двигателя (после ДЗ): %d - не поддерживаемая система запуска\n",
                   engine_start_system);
            return UNDEFINED_ENG_ERROR;
    }
}

engine_start_error_t Gen6Basic::checkIgnLockEngine(float          bypass_waiting_time_s,
                                                   bool           acc_must_be_off,
                                                   log_label_t    log_label,
                                                   command_type_t execution_method)
{
    char log_label_buf[LEN_STRUCT::MAX_STR_LOG_LABEL_LEN];

    getLogLabelStr(log_label, log_label_buf);

    if (!waitInspector(bypass_waiting_time_s, 0))
    {
        printf("%s Обходчик не включен\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS Обходчик включен\n");
            
    if (!waitAcc(DELAY_ACC + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s На акссесуары не подан потенциал\n", log_label_buf);
        return ACC_ENG_ERROR;
    }

    printf("PASS На акссесуары подан потенциал\n");

    if (!waitIgn1(DELAY_IGN_AFTER_ACC + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0))
    {
        printf("%s На зажигание не подан потенциал\n", log_label_buf);
        return IGN1_ENG_ERROR;
    }

    printf("PASS На зажигание подан потенциал\n");

    if (waitStarter(DELAY_STARTER_AFTER_IGN + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("PASS На стартер подан потенциал \n"); 

        if (acc_must_be_off)
        {
            if (waitAcc(TIME_DELAY::MIN_WIRE_EN_DELAY_S, 1))
            {
                printf("PASS С акссесуаров снят потенциал\n"); 
            }
            else 
            {
                printf("%s С акссесуаров не снят потенциал\n", log_label_buf);
                return ACC_ENG_ERROR;
            }
        }
        else
        {
            if (waitAcc(TIME_DELAY::MIN_WIRE_EN_DELAY_S, 0))
            {
                printf("PASS С акссесуаров не снят потенциал\n"); 
            }
            else 
            {
                printf("%s С акссесуаров снят потенциал\n", log_label_buf);
                return ACC_ENG_ERROR;
            }
        }

        wait(TIME_DELAY::TWO_S);
        setEngine(ON, execution_method);
    }
    else
    {
        printf("%s На стартер не подан потенциал\n", log_label_buf);
        return STARTER_ENG_ERROR;
    }

    if (waitStarter(STARTER_MAX_TIME - TIME_DELAY::TWO_S, 1)) 
    {
        printf("PASS Со стартера снят потенциал\n");

        if (waitAcc(TIME_DELAY::MIN_WIRE_EN_DELAY_S, 0)) 
        {
            printf("PASS На акссесуары подан потенциал\n"); 
        }
        else 
        {
            printf("%s На акссесуары не подан потенциал\n", log_label_buf);
            return STARTER_ENG_ERROR;
        }
    }
    else
    {
        printf("%s Со стартера не снят потенциал\n", log_label_buf);
        return STARTER_ENG_ERROR;
    }

    return NO_ENG_ERROR;
}

engine_start_error_t Gen6Basic::checkStartStopEngine(engine_start_system_t engine_start_system,
                                                     float                 bypass_waiting_time_s,
                                                     log_label_t           log_label)
{
    switch (engine_start_system)
    {
        case START_STOP_ONE_IMPULSE:
            return checkStartStopOneImpulseEngine(bypass_waiting_time_s, RPM_ON, log_label);

        case START_STOP_TWO_IMPULSE:
            return checkStartStopTwoImpulseEngine(bypass_waiting_time_s, RPM_ON, log_label);

        case START_STOP_THREE_IMPULSE:
            return checkStartStopThreeImpulseEngine(bypass_waiting_time_s, RPM_ON, log_label);

        default:
            printf("INFO Проверка запуска двигателя (Старт-Стоп): не поддерживаемая система запуска\n");
            return UNDEFINED_ENG_ERROR;
    }
}

engine_start_error_t Gen6Basic::checkStartStopOneImpulseEngine(float       bypass_waiting_time_s,
                                                               int16_t     rpm_val,
                                                               log_label_t log_label)
{
    char log_label_buf[LEN_STRUCT::MAX_STR_LOG_LABEL_LEN];

    getLogLabelStr(log_label, log_label_buf);

    if (!waitInspector(bypass_waiting_time_s, 0))
    {
        printf("%s Не подан потенциал на модуль обходчика иммобилайзера\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS Подан потенциал на модуль обходчика иммобилайзера\n");

    if (!waitImitBreak(DELAY_FIRST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s Не включена имитация педали тормоза\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS Включена имитация педали тормоза\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0))
    {
        printf("%s На кнопку старт-стоп не подан потенциал\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS На кнопку старт-стоп подан потенциал\n");
    wait(TIME_DELAY::TWO_AND_HALF_S);
    
    setIgn(PIN, ON);
    setEngine(ON, PIN, rpm_val);

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1))
    {
        printf("%s С кнопки старт-стоп не снят потенциал\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS С кнопки старт-стоп снят потенциал\n");

    if (!waitImitBreak(TIME_DELAY::ONE_AND_HALF_S, 1))
    {
        printf("%s Имитация педали тормоза не выключена\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS Имитация педали тормоза выключена\n");
    return NO_ENG_ERROR;
}

engine_start_error_t Gen6Basic::checkStartStopTwoImpulseEngine(float       bypass_waiting_time_s,
                                                               int16_t     rpm_val,
                                                               log_label_t log_label)
{
    char log_label_buf[LEN_STRUCT::MAX_STR_LOG_LABEL_LEN];

    getLogLabelStr(log_label, log_label_buf);

    if (!waitInspector(bypass_waiting_time_s, 0))
    {
        printf("%s Не подан потенциал на модуль обходчика иммобилайзера\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS Подан потенциал на модуль обходчика иммобилайзера\n"); 

    if (!waitStartStopBtn(DELAY_FIRST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s На кнопку старт-стоп не подан потенциал\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS На кнопку старт-стоп подан потенциал\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1))
    {
        printf("%s С кнопки старт-стоп не снят потенциал\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS С кнопки старт-стоп снят потенциал\n");

    if (!waitImitBreak(DELAY_LAST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s Не включена имитация педали тормоза\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS Включена имитация педали тормоза\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0))
    {
        printf("%s На кнопку старт-стоп не подан потенциал\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS На кнопку старт-стоп подан потенциал\n");

    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
    setIgn(PIN, ON);
    setEngine(ON, PIN, rpm_val);
    wait(TIME_DELAY::ONE_S);

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s С кнопки старт-стоп не снят потенциал\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS С кнопки старт-стоп снят потенциал\n");

    if (!waitImitBreak(TIME_DELAY::ONE_S, 1)) 
    {
        printf("%s Имитация педали тормоза не выключена\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS Имитация педали тормоза выключена\n");
    return NO_ENG_ERROR;
}

engine_start_error_t Gen6Basic::checkStartStopThreeImpulseEngine(float       bypass_waiting_time_s,
                                                                 int16_t     rpm_val,
                                                                 log_label_t log_label)
{
    char log_label_buf[LEN_STRUCT::MAX_STR_LOG_LABEL_LEN];

    getLogLabelStr(log_label, log_label_buf);

    if (!waitInspector(bypass_waiting_time_s, 0)) 
    {
        printf("%s Не подан потенциал на модуль обходчика иммобилайзера\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS Подан потенциал на модуль обходчика иммобилайзера\n");

    if (!waitStartStopBtn(DELAY_FIRST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s На кнопку старт-стоп не подан потенциал 1\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS На кнопку старт-стоп подан потенциал 1\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s С кнопки старт-стоп не снят потенциал 1\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS С кнопки старт-стоп снят потенциал 1\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0)) 
    {
        printf("%s На кнопку старт-стоп не подан потенциал 2\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS На кнопку старт-стоп подан потенциал 2\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s С кнопки старт-стоп не снят потенциал 2\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS С кнопки старт-стоп снят потенциал 2\n");

    if (!waitImitBreak(DELAY_LAST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s Не включена имитация педпли тормоза\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR; 
    }

    printf("PASS Включена имитация педпли тормоза\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0)) 
    {
        printf("%s На кнопку старт-стоп не подан потенциал 3\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS На кнопку старт-стоп подан потенциал 3\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s С кнопки старт-стоп не снят потенциал 3\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS С кнопки старт-стоп снят потенциал 3\n");

    setIgn(PIN, ON);
    setEngine(ON, PIN, rpm_val);
    
    if (!waitImitBreak(DELAY_RPM_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 1)) 
    {
        printf("%s Имитация педали тормоза не выключена\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS Имитация педали тормоза выключена\n");
    return NO_ENG_ERROR; 
}

void Gen6Basic::setSupportIgn(eng_start_ign_supp_type_t sup_ign_var,
                              engine_start_system_t     engine_start_system,
                              ign_statuses_t            ign_status)
{
    setIgn(PIN, ON);
    setEngine(ON, PIN, RPM_ON);
    setHandBreak(ON, PIN);

    if (sup_ign_var == IGN_SUPP_TYPE_HAND_BRAKE_OR_PARKING)
    {
        if (   (   engine_start_system == IGNITION_LOCK
                || engine_start_system == EXTERNAL)
            && (ign_status == IGN_OFF))
        {
            setIgn(PIN, OFF);
        }
    }
    else 
    {
        if (ign_status == IGN_OFF)
        {
            setIgn(PIN, OFF);
        }
        else
        {
            printf("FAIL Зажигание не выключено\n");
        }
    }

    if (!getIgn1())
    {
        printf("PASS Поддержка зажигания включена\n");
    }
    else 
    {
        printf("FAIL Поддержка зажигания не включена\n");
    }

    wait(TIME_DELAY::ONE_S);
}

void Gen6Basic::setProgramNeutral(command_type_t                  execution_method,
                                  eng_start_neutral_detect_type_t program_neutral_type,
                                  eng_start_ign_supp_type_t       sup_ign_var,
                                  engine_start_system_t           engine_start_system,
                                  eng_start_gearbox_type_t        engine_gearbox_type,
                                  ign_statuses_t                  ign_status,
                                  float                           delay_s)
{
    if (engine_gearbox_type == ENG_GEARBOX_AUTO)
    {
        setIgn(PIN, ON);
        setParking(ON, PIN);
        setHandBreak(ON, PIN);
        setIgn(PIN, OFF);
        return;
    }

    setSupportIgn(sup_ign_var,
                  engine_start_system,
                  ign_status);

    if (program_neutral_type == NEUTRAL_DETECT_TYPE_DOOR)
    {
        setDoor(OPEN, PIN);
        setDoor(CLOSE, PIN);
        setIgn(PIN, OFF);        
        setEngine(OFF, PIN);
    }
    else
    {
        setDoor(OPEN, PIN);            
        setDoor(CLOSE, PIN); 
        setSecure(ON, execution_method, TIME_DELAY::ONE_S);
        setIgn(PIN, OFF);
        setEngine(OFF, PIN);
        setSecure(OFF, execution_method, TIME_DELAY::NULL_DELAY_S);  
    }

    wait(delay_s);
}

void Gen6Basic::setAdditionalSensor(command_type_t execution_method, bool state)
{
    const char *msg;

    state ? msg = "Включение доп. датчика"
          : msg = "Выключение доп. датчика";

    printf("INFO %s\n", msg);

    switch (execution_method)
    {
        case PIN:
            *p_out_additsensor = state;
            break;
                
        default:
            printf("INFO Управление доп. датчиком: метод управления не определен\n");
            break;
    }
}

bool Gen6Basic::getProgramNeutral()
{
    return getSlpCmdStatus(DIAG_DATA_FLAG,
                           SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                           DIAG_FLAG_BIT_NEUTRAL);
}

bool Gen6Basic::getSupportIgn()
{
    return getSlpCmdStatus(DIAG_DATA_FLAG,
                           SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                           DIAG_FLAG_BIT_IGN_SUPP);
}

bool Gen6Basic::getBleStatus(command_type_t execution_method)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE\n");
        return false;
    }

    bool result = false;

    switch (execution_method)
    {
        case SLP:
            result = getSlpCmdStatus(DIAG_DATA_FLAG,
                                     SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                     DIAG_FLAG_BIT_TAG | DIAG_FLAG_BIT_RELAY);
            break;
            
        case PIN:
            result = isTagVisible();
            break;
        
        default:
            printf("FAIL Неверный способ исполнения\n");
            break;
    }

    return result;
}

bool Gen6Basic::getR6Status(command_type_t execution_method)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE\n");
        return false;
    }

    bool result = false;

    switch (execution_method)
    {
        case SLP:
            result = getSlpCmdStatus(DIAG_DATA_FLAG,
                                     SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                     DIAG_FLAG_BIT_RELAY);
            break;
            
        case PIN:
            result = isTagVisible();
            break;
        
        default:
            printf("FAIL Неверный способ исполнения\n");
            break;
    }

    return result;
}

bool Gen6Basic::getTagStatus(command_type_t execution_method)
{
    bool result = false;

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE\n");
        return false;
    }

    switch (execution_method)
    {
        case SLP:
            result = getSlpCmdStatus(DIAG_DATA_FLAG,
                                     SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                     DIAG_FLAG_BIT_TAG);
            break;
            
        case PIN:
            result = isTagVisible();
            break;
        
        default:
            printf("FAIL Неверный способ исполнения\n");
            break;
    }

    return result;
}

bool Gen6Basic::getTurboTimer()
{
    return getSlpCmdStatus(DIAG_DATA_FLAG,
                           SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                           DIAG_FLAG_BIT_TT_RUNNING);
}

void Gen6Basic::setDisarmSt()
{
    command_type_t     execution_method = getExecutionMethod(KEYRING, USER_ACTION);
    tag_voltage_ctrl_t tag_voltage_ctrl = TAG_VOLTAGE_CTRL_EN;


    if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                         SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                         DIAG_GUARD_STATE_DISARM))
    {
        if (execution_method == TAG)
        {
            tag_voltage_ctrl = TAG_VOLTAGE_CTRL_DIS;

            setTagVcc();
            wait(TIME_DELAY::DELAY_AFTER_TAG_VCC_EN_S);
        }

        setSecure(ON,
                  KEYRING,
                  TIME_DELAY::STD_REMOTE_STATUS_EN_TIME_S,
                  tag_voltage_ctrl);
        setSecure(OFF,
                  KEYRING,
                  TIME_DELAY::STD_REMOTE_STATUS_EN_TIME_S,
                  tag_voltage_ctrl);

        if (execution_method == TAG)
        {
            resetTagVcc();
        }
    }
}

void Gen6Basic::setState(int8_t state_num)
{
    printf("INFO ---Сброс настроек---\n");

    if (m_device_name == GEN6_M66)
    {
        state_num = AUTOTESTS_M66;
    }

    if (state_num != NO_RESET)
    {
        sigmaWorkMode();
    }

    switch (state_num)
    {
        case AUTOTESTS_MS:
            setSpeed(0);

            resetDigOutsVoltage();
            setBreak(OFF, PIN);
            setBreak(OFF, CAN_USE);
            resetDiagDataPerim();

            setEngine(OFF, PIN);
            setDisarmSt();
            isServiceModeOn(ON);
            setDoor(OPEN, PIN);
            setDoor(CLOSE, PIN);
            break;
            
        case FULL:
        case AUTOTESTS_SLM:
        case AUTOTESTS_SENSORS: 
        case AUTOTESTS_MODES:
        case AUTOTESTS_TELEMATICS:
        case AUTOTESTS_REMOTE:
        case AUTOTESTS_CHANNELS:
        case AUTOTESTS_FLEX:
        case AUTOTESTS_R6ECO:

            if (!getVcc())
            {
                on();
                wait(TIME_DELAY::DEVICE_VCC_INIT_WAIT_TIME_S);
            }

            p_gsm->set4xSim(SIM1);
            p_slp->closeRegion();
            resetDigOutsVoltage();
            setPassengerDoor(CLOSE, CAN_USE, TIME_DELAY::NULL_DELAY_S);
            setDriverDoor(CLOSE, CAN_USE, TIME_DELAY::NULL_DELAY_S);
            setBreak(OFF, PIN);
            setBreak(OFF, CAN_USE);
            setEngine(OFF, PIN);
            resetDiagDataPerim();

            setLedKeyringVcc(ON);
            setLcdKeyringVcc(ON);

            setHood(CLOSE, THROUGH_R6);

            if (p_r4 != NULL)
            {
                p_r4->resetIgn();
                p_r4->setVcc(ON);
            }

            if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
            {
                p_r6->resetIgn();
                p_r6->resetInput();
                p_r6->setVcc(ON);
                p_r6eco->setVcc(ON);
                resetTagVcc();
                disconnectMobile();

                if (getSlpCmdStatus(DIAG_DATA_FLAG,
                                    SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                    DIAG_FLAG_BIT_ANTI_HIJACK_TAG_EN))
                {
                    setAhjTagSetting(OFF);
                }
            }

            // на случай, если в системе остался активный статус паркинга
            setIgn(CAN_USE, ON);
            setIgn(PIN, ON);
            setHandBreak(OFF, CAN_USE);
            setParking(OFF, CAN_USE);
            setParking(OFF, PIN);
            setIgn(CAN_USE, OFF);

            if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                DIAG_GUARD_STATE_REG))
            {
                
                setIgn(PIN,                              // Если входили в режим регистрации через SLP или сервисную кнопку без зажигания
                       ON,
                       TIME_DELAY::MAX_WIRE_EN_DELAY_S);
                setIgn(PIN, OFF);
                wait(TIME_DELAY::TEN_S);
            }
            else
            {
                setIgn(PIN, OFF);
            }

            // отключаем сервисный режим, если включен
            isServiceModeOn(ON);

            setDisarmSt();

            if ((m_device_modules & R868_MODULE) == R868_MODULE)
            {
                for (uint8_t i = 0; i < 2; ++i)
                {
                    pushLcdKeyringBut1(KEYRING_SHORT);
                    wait(TIME_DELAY::MIN_WAITING_REGION_STATE_S);

                    if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                         SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                         DIAG_GUARD_STATE_ARM))
                    {
                        printf("INFO Сброс настроек. Ожидаемое состояние: охрана. Ошибка при чтении региона\n");
                        pushLcdKeyringBut23(KEYRING_SHORT);
                        wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);
                    }

                    pushLcdKeyringBut2(KEYRING_SHORT);
                    wait(TIME_DELAY::MIN_WAITING_REGION_STATE_S);

                    if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                        SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                        DIAG_GUARD_STATE_ARM))
                    {
                        printf("INFO Сброс настроек. Ожидаемое состояние: снято с охраны. Ошибка при чтении региона\n");
                        pushLcdKeyringBut23(KEYRING_SHORT);
                        wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);
                    }
                }
            }

            resetAuthAndImmo();
            setDangerParking(OFF);

            if ((getModules() & BLE_MODULE) == BLE_MODULE)
            {
                setBleSettingMode(OFF);
            }

            p_gsm->resetCall(false);
            setSpeed(0);
            setIgn(CAN_USE, OFF);
            break;

        case AUTOTESTS_SE:
        case AUTOTESTS_BLE:

            if (   m_device_name != GEN6_X96V2 // На стенде X96v2 нет датчика температуры
                && m_device_name != GEN6_A96)  // AT-7342
            {
                setTemperatureValue(TEMP_DEFAULT);
            }
            else
            {
                setEngineTemperatureSensor(COUNT_STRUCT::DEFAULT_SLP_TEMP_VALUE);
            }

            setLedKeyringVcc(ON);
            setLcdKeyringVcc(ON);

            p_slp->closeRegion();
            resetDigOutsVoltage();
            setEngine(OFF, PIN);
            setPassengerDoor(CLOSE, CAN_USE, TIME_DELAY::NULL_DELAY_S);
            setDriverDoor(CLOSE, CAN_USE, TIME_DELAY::NULL_DELAY_S);
            setBreak(OFF, PIN);
            setBreak(OFF, CAN_USE);

            resetDiagDataPerim();
            setHood(CLOSE, THROUGH_R6);
            setDisarmSt();
            resetAuthAndImmo();
            break;

        case AUTOTESTS_M66:
            setAdditionalSensor(PIN, OFF);

            if (getSlpCmdStatus(DIAG_DATA_FLAG,
                                SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                DIAG_FLAG_BIT_ANTI_HIJACK))
            {
                p_gsm->sendSms(getDevicePhones()[0], Sim900::combineStr("30 %s", getDevicePin()).c_str());
                p_gsm->outputSms();
            }

            if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                DIAG_GUARD_STATE_SERVICE))
            {
                p_gsm->sendSms(getDevicePhones()[0], "50");
                p_gsm->outputSms();
            }

            setIgn(CAN_USE, OFF);
            resetTag();
            setTagVcc();
            p_can_actions->setSpeed(0);
            break;

        case NO_RESET:
            break;

        default:
            printf("DEBUG Неизвестный вариант сброса настроек\n");
            break;
    }

    p_gsm->clearSms();
    p_can_actions->clearFifo();

    printf("INFO --------------------\n");
}
        
int8_t Gen6Basic::getAlarm()
{
    if (m_device_name == GEN6_M96)
    {
        return p_r6->isOutSet();
    }

    if (p_in_siren == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Сирена\" не определен\n");

        return -1;
    }

    return *p_in_siren;
}


int8_t Gen6Basic::getBlock(command_type_t execution_method)
{
    switch (execution_method)
    {
        case PIN:
            if (   m_device_name == GEN6_M96
                || m_device_name == GEN6_M66)
            {
                return p_r6->isNoSet();
            }
            else
            {
                if (p_in_block == NULL)
                {
                    printf("FAIL Считывание состояния. Пин \"Блокировка двигателя\" не определен\n");
                    return -1;
                }

                return getDigInByFunc(p_in_block, ID_OUT_ENG_BLOCK);
            }

        case THROUGH_R6:
            return p_r6->isNoSet();

        case THROUGH_R6ECO:
            return p_r6eco->getBlock();

        default:
            printf("FAIL Считывание блокировки. Имя устройства не определено\n");
            return -1;
        
    }
}

bool Gen6Basic::waitBleStatus(float time, bool error_flag, float error)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE \n");
        return false;
    }

    Timer timer;
    bool  result = true;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    printf("INFO Ожидание Включения BLE\n");

    while (end < time)
    {
        if (getBleStatus() == 1)
        {
            printf("INFO Статусы метки и R6 появились в регионе\n");
            break;
        }

        end = timer.read() - begin;
    }

    if (error_flag && (end > (time + error) || end < (time - error)))
    {
        result = false;
    }
    else if (end >= time)
    {
        result = false;
    }

    printf("INFO Прошло времени: %f\n", end);
    timer.stop();

    return result;
}

bool Gen6Basic::waitR6Status(float time,
                             bool  error_flag,
                             float error,
                             bool  waiting_status)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE \n");
        return false;
    }

    Timer timer;
    bool  result = true;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    printf("INFO Ожидание подключения R6\n");

    while (end < time)
    {
        if (waiting_status && getR6Status())
        {
            printf("INFO Статус R6 появились в регионе\n");
            break;
        }
        else if (!waiting_status && !getR6Status())
        {
            printf("INFO Чтение из региона: R6 потерян\n");
            break;
        }

        end = timer.read() - begin;
    }

    if (error_flag && (end > (time + error) || end < (time - error)))
    {
        result = false;
    }
    else if (end >= time)
    {
        result = false;
    }

    printf("INFO Прошло времени: %f\n", end);
    timer.stop();

    return result;
}

bool Gen6Basic::waitRelayLockAndUnlock(RelayBase *relay,
                                       uint8_t    lock_pulse_count,
                                       uint8_t    unlock_pulse_count,
                                       float      timeout_s)
{
    const float min_pulse_width_s = TIME_DELAY::MIN_PULSE_WIDTH_S; // Минимальная длительность импульса которая будет обработана

    Timer   timer;                                                 // Таймер
    float   begin_time_s;                                          // Время начала отсчета таймаута
    float   current_time_s;                                        // Текущее время
                       
    float   lock_start_s;                                          // Время получения потенциала закрытия замка
    float   lock_end_s;                                            // Время получения импульса закрытия замка (второй потенциал)
                       
    float   unlock_start_s;                                        // Время получения потенциала открытия замка
    float   unlock_end_s;                                          // Время получения импульса закрытия замка (второй потенциал)
                       
    bool    is_lock_potential    = false;                          // Получен потенциал закрытия замка капота
    bool    is_unlock_potential  = false;                          // Получен потенциал открытия замка капота
                       
    uint8_t current_lock_count   = 0;                              // Количество полученных импульсов закрытия замка капота
    uint8_t current_unlock_count = 0;                              // Количество полученных импульсов открытия замка капота


    if (relay == NULL)
    {
        printf("FAIL Реле не задано\n");
        return false;
    }

    printf("INFO Начало ожидания импульсов запирания/отпирания замка капота от реле\n");

    timer.start();

    begin_time_s   = timer.read();
    current_time_s = timer.read();

    while (current_time_s - begin_time_s < timeout_s)
    {
        // Потенциал закрытия капота
        if (relay->isLockSet())
        {
            is_lock_potential = true;
            lock_start_s      = timer.read();
        }

        // Потенциал открытия капота
        if (relay->isUnlockSet())
        {
            is_unlock_potential = true;
            unlock_start_s      = timer.read();
        }

        // Импульс закрытия капота
        if (is_lock_potential && !relay->isLockSet())
        {
            lock_end_s = timer.read();

            if (lock_end_s - lock_start_s > min_pulse_width_s)
            {
                is_lock_potential = false;
                printf("INFO Получен %d-й импульс запирания замка капота от реле\n", ++current_lock_count);
                printf("INFO Прошло времени: %f\n", timer.read());
            }
        }

        // Импульс открытия капота
        if (is_unlock_potential && !relay->isUnlockSet())
        {
            unlock_end_s = timer.read();

            if (unlock_end_s - unlock_start_s > min_pulse_width_s)
            {
                is_unlock_potential = false;
                printf("INFO Получен %d-й импульс отпирания замка капота от реле\n", ++current_unlock_count);
                printf("INFO Прошло времени: %f\n", timer.read());
            }
        }

        current_time_s = timer.read();
    }

    timer.stop();

    printf("INFO Конец ожидания, прошло времени: %f\n", timer.read());
    printf("INFO Импульсов от реле на запирание замка капота ожидалось: %d, получено: %d\n", lock_pulse_count,   current_lock_count);
    printf("INFO Импульсов от реле на отпирание замка капота ожидалось: %d, получено: %d\n", unlock_pulse_count, current_unlock_count);

    if (   current_lock_count   == lock_pulse_count
        && current_unlock_count == unlock_pulse_count)
    {
        printf("INFO Количество полученных импульсов соответствует ожидаемому\n");
        return true;
    }
    else
    {
        printf("INFO Количество полученных импульсов не соответствует ожидаемому\n");
        return false;
    }
}

bool Gen6Basic::waitR4LockAndUnlock(uint8_t lock_pulse_count,
                                    uint8_t unlock_pulse_count,
                                    float   timeout_s)
{
    return waitRelayLockAndUnlock(p_r4,
                                  lock_pulse_count,
                                  unlock_pulse_count,
                                  timeout_s);
}

bool Gen6Basic::waitSirenType(siren_type_mask_t type, float timeout_s)
{
    Timer timer;

    bool  is_base_siren = false;
    bool  is_r6_siren   = false;
    bool  result        = true;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с R6\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (   !is_base_siren
            &&  p_in_siren != NULL
            && *p_in_siren)         // Если вызывать getAlarm(), то будет FAIL если пин не определен (тест может требовать чтобы сирена не была настроена)
        {
            is_base_siren = true;
            printf("INFO Получен сигнал штатной сирены\n");
        }

        if (   !is_r6_siren
            &&  p_r6->isOutSet())
        {
            is_r6_siren = true;
            printf("INFO Получен сигнал сирены R6\n");
        }
    }

    if (   (type & SIREN_TYPE_BASE) == SIREN_TYPE_BASE
        && !is_base_siren)
    {
        printf("INFO Ожидался сигнал штатной сирены, но получен не был\n");
        result = false;
    }

    if (   (type & SIREN_TYPE_R6) == SIREN_TYPE_R6
        && !is_r6_siren)
    {
        printf("INFO Ожидался сигнал сирены R6, но получен не был\n");
        result = false;
    }

    if (result)
    {
        printf("INFO Все ожидаемые сигналы сирены были получены\n");
    }

    return result;
}

bool Gen6Basic::waitR6LockAndUnlock(uint8_t lock_pulse_count,
                                    uint8_t unlock_pulse_count,
                                    float   timeout_s)
{
    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return false;
    }

    return waitRelayLockAndUnlock(p_r6,
                                  lock_pulse_count,
                                  unlock_pulse_count,
                                  timeout_s);
}

bool Gen6Basic::waitR6LockPotential(float timeout_s)
{
    Timer timer;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r6->isLockSet())
        {
            printf("INFO Потенциал закрытия замка капота получен, прошло времени: %.2f\n", timer.read());
            return true;
        }
    }

    return false;
}

bool Gen6Basic::waitR6UnlockPotential(float timeout_s)
{
    Timer timer;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r6->isUnlockSet())
        {
            printf("INFO Потенциал открытия замка капота получен, прошло времени: %.2f\n", timer.read());
            return true;
        }
    }

    return false;
}

bool Gen6Basic::waitR6SirenSignals(uint8_t signal_count, float timeout_s)
{
    Timer   timer;
    uint8_t current_signal_count = 0;
    bool    is_potential         = false;

    float   signal_min_pulse_width_s = TIME_DELAY::MIN_PULSE_WIDTH_S; //!< Длительность сигнала, который не будет считаться шумом
    float   signal_start_time_s      = 0.0f;                          //!< Время получения потенциала
    float   signal_end_time_s        = 0.0f;                          //!< Время получения второго потенциала (импульс)


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return false;
    }

    printf("INFO Ожидаемое количество сигналов сирены R6: %d шт.\n", signal_count);
    timer.start();

    while (timer.read() < timeout_s)
    {
        if (   !is_potential
            && p_r6->isOutSet())
        {
            is_potential        = true;
            signal_start_time_s = timer.read();
            printf("INFO Получен потенциал сирены R6\n");
        }

        if (   is_potential
            && !p_r6->isOutSet())
        {
            signal_end_time_s = timer.read();

            if (signal_end_time_s - signal_start_time_s > signal_min_pulse_width_s)
            {
                is_potential = false;
                printf("INFO Получен %d-й сигнал сирены R6\n", ++current_signal_count);
            }
        }
    }

    timer.stop();

    if (current_signal_count == signal_count)
    {
        printf("PASS Ожидаемое количество сигналов сирены R6 получено\n");
        return true;
    }
    else
    {
        printf("FAIL Ожидаемое количество сигналов сирены R6 не получено\n");
        return false;
    }
}

bool Gen6Basic::waitRelayBlockNormalOpen(RelayBase *relay,
                                         bool       status,
                                         float      timeout_s)
{
    Timer timer;
    bool  block_status; // Статус блокировки двигателя


    if (relay == NULL)
    {
        printf("FAIL Реле не задано\n");
        return false;
    }

    printf("INFO Проверка блокировки реле на контакте NO, время ожидания потенциала: %.6f\n", timeout_s);
    timer.start();

    while (timer.read() < timeout_s)
    {
        block_status = !relay->isNoSet(); // Получить статус блокировки:
                                          // Блокировка включена при логическом нуле контакта NO (т.к. это означает, что цепь разорвана и двигатель заблокирован)
                                          // При логической единице цепь не разорвана, что означает блокировка выключена

        // Проверяем нужный нам статус блокировки (вкл/выкл)
        if (block_status == status)
        {
            timer.stop();
            printf("INFO Потенциал на контакте NO от реле получен через: %.6f\n", timer.read());
            return true;
        }
    }

    timer.stop();
    printf("INFO Потенциал на контакте NO от реле не получен, прошло времени: %.6f\n", timer.read());

    return false;
}

bool Gen6Basic::waitR6BlockNormalOpen(bool status, float timeout_s)
{
    printf("INFO Проверка блокировки реле R6 на контакте NO\n");

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Нет модуля для работы с R6\n");
        return false;
    }

    return waitRelayBlockNormalOpen(p_r6,
                                    status,
                                    timeout_s);
}

bool Gen6Basic::waitR6BlockNormalClose(bool status, float timeout_s)
{
    bool nc_status = !status; // Состояние пина NC будет противоположным пину NO
    bool result;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Нет модуля для работы с R6\n");
        return false;
    }

    // Т.к. мы не подключаем пин NC, то будем проверять НЗ блокировку через пин NO
    printf("INFO Проверка R6 блокировки NC через контакт NO\n");
    result = waitR6BlockNormalOpen(nc_status, timeout_s);

    return result;
}

bool Gen6Basic::waitR4Block(bool status, float timeout_s)
{
    printf("INFO Проверка блокировки реле R4 на контакте NO\n");

    if (p_r4 == NULL)
    {
        printf("FAIL Отсутствует R4\n");
        return false;
    }

    return waitRelayBlockNormalOpen(p_r4,
                                    !status,    // Т.к. R4 замыкает контакты COM и NO при блокировке, то при вкл. блокировке будет 1
                                    timeout_s);
}

bool Gen6Basic::waitR4Output(bool status, float timeout_s)
{
    Timer timer;


    if (p_r4 == NULL)
    {
        printf("FAIL Отсутствует R4\n");
        return false;
    }

    printf("INFO Проверка контакта Output реле R4\n");
    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r4->isOutSet() == status)
        {
            printf("INFO Получен ожидаемый статус на выходе Output реле R4\n");
            return true;
        }
    }

    timer.stop();
    printf("INFO Ожидаемый статус на выходе Output реле R4 не был получен\n");

    return false;
}

bool Gen6Basic::waitR6Alarm(r6_alarm_signal_t type, float timeout_s)
{
    Timer timer;
    float begin_time_s;
    float current_time_s;

    bool  is_potential = false;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return false;
    }

    timer.start();

    begin_time_s   = timer.read();
    current_time_s = timer.read();

    while (current_time_s - begin_time_s < timeout_s)
    {
        if (p_r6->isOutSet())
        {
            if (type == R6_SIREN)
            {
                printf("INFO Сработала сирена R6\n");
                return true;
            }

            is_potential = true;
        }

        if (is_potential && !p_r6->isOutSet())
        {
            if (type == R6_KLAXON)
            {
                printf("INFO Сработал клаксон R6\n");
                return true;
            }
        }

        current_time_s = timer.read();
    }

    return false;
}

bool Gen6Basic::waitR6SirenPotential(r6_siren_potential_t polarity, float timeout_s)
{
    Timer timer;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6\n");
        return false;
    }

    printf("INFO Ожидание потенциала сирены R6, полярность: %d\n", polarity);
    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r6->isOutSet() == polarity)
        {
            printf("INFO Получен потенциал сирены R6\n");
            return true;
        }
    }

    printf("INFO Потенциал сирены R6 не получен\n");

    return false;
}

bool Gen6Basic::checkR6EcoLedExt(r6eco_led_ext_mode_t led_ext_mode, float timeout_s)
{
    Timer        timer;
    const int8_t err_percent = 5;    // Процент допустимой ошибки
    float        err_time_s;         // Допустимая ошибка

    float  active_time_s  = 0.0;     // Время активного уровня
    float  passive_time_s = 0.0;     // Время пассивного уровня

    float  begin_s = 0.0;            // Время получения активного уровня
    float  end_s   = 0.0;            // Время получения пассивного уровня

    bool   is_potential     = false; // Получен потенциал
    bool   is_first_impulse = true;  // Первый ли это импульс
    int8_t pulse_count      = 0;     // Количество полученных импульсов

    bool   is_passive = false;;      // Пассивен ли светодиод (всегда не горит)


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует модуль для работы с R6 ECO\n");
        return false;
    }

    printf("INFO Проверка работы сервисного светодиода R6 ECO\n");

    // Определить время активного и пассивного уровня
    switch (led_ext_mode)
    {
        // Сервисный светодиод не мигает в Снято
        case R6ECO_LED_EXT_MODE_DISARM:
            is_passive = true;
            break;

        // Мигает раз в 1.4 секунды
        case R6ECO_LED_EXT_MODE_ARM:
            active_time_s  = 0.100f;
            passive_time_s = 1.300f;
            break;

        // Мигает раз в секунду
        case R6ECO_LED_EXT_MODE_ARM_DISCONNECTED:
            active_time_s  = 0.100f;
            passive_time_s = 0.900f;
            break;

        default:
            printf("FAIL Неизвестный режим работы сервисного светодиода R6 ECO\n");
            return false;
    }

    err_time_s = max(active_time_s, passive_time_s) * err_percent / 100.0f; // Допустимая погрешность
    timer.start();

    while (timer.read() < timeout_s)
    {
        // Получен активный уровень
        if (   !is_potential
            &&  p_r6eco->getLedExt())
        {
            begin_s      = timer.read();
            is_potential = true;

            printf("INFO Получен потенциал сервисного светодиода: %.6f\n", begin_s);

            if (!is_passive) // Длина импульса проверяется только если светодиод не пассивен
            {
                if (   begin_s - end_s < passive_time_s + err_time_s
                    && begin_s - end_s > passive_time_s - err_time_s)
                {
                    printf("PASS Пауза между импульсами сервисного светодиода выдержана\n");
                }
                else if (!is_first_impulse) // При первом пойманном импульсе нет смысла проверять паузу т.к. не было предыдущего
                {
                    printf("FAIL Пауза между импульсами сервисного светодиода не выдержана\n");
                    return false;
                }
            } // if !is_passive
        } // if !is_potential

        // Получен пассивный уровень
        if (    is_potential
            && !p_r6eco->getLedExt())
        {
            end_s        = timer.read();
            is_potential = false;

            printf("INFO Получен импульс сервисного светодиода: %.6f\n", end_s);
            ++pulse_count;

            if (!is_passive) // Длина импульса проверяется только если светодиод не пассивен
            {
                if (   end_s - begin_s < active_time_s + err_time_s
                    && end_s - begin_s > active_time_s - err_time_s)
                {
                    printf("PASS Длительность импульса сервисного светодиода верная\n");
                }
                else if (!is_first_impulse) // Ложная ошибка может быть в случае, если мы начали проверку уже во время импульса
                {
                    printf("FAIL Длительность импульса сервисного светодиода неверная\n");
                    return false;
                }
            } // if !is_passive

            is_first_impulse = false;
        }
    } // while (timer.read() < timeout_s)

    if (!is_passive)
    {
        // Проверка того, что светодиод отмигал один из следуюших вариантов:
        // 1. Точное количество раз, соответствующее времени
        // 2. На 1 раз меньше чем мог отмигать за отведенное время (т.к. проверка могла начаться чуть позже после начала миганий)
        if ((int8_t)(timeout_s / (active_time_s + passive_time_s)) - pulse_count < 2)
        {
            printf("PASS Сервисный светодиод мигал с ожидаемой частотой\n");
        }
        else
        {
            printf("FAIL Сервисный светодиод не мигал с ожидаемой частотой\n");
            return false;
        }
    }
    else
    {
        // Проверка того, что светодиод не горел вообще
        if (    is_first_impulse // Флаг первого импульса не был снят
            && !is_potential
            &&  pulse_count == 0)
        {
            printf("PASS Сервисный светодиод все время пребывал в пассивном состоянии\n");
        }
        else
        {
            printf("FAIL Сервисный светодиод не все время пребывал в пассивном состоянии\n");
            return false;
        }
    } // if (!is_passive)

    return true;
}

bool Gen6Basic::waitTagStatus(float time_s,
                              bool  error_flag,
                              float error_s)
{
    Timer timer;
    bool  result  = true;
    float begin_s = 0.0;  // начало отсчета
    float end_s   = 0.0;  // прошло времени с начала отсчета


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE \n");
        return false;
    }

    timer.start();

    begin_s = timer.read();
    end_s   = begin_s;

    printf("INFO Ожидание подключения метки\n");

    while (end_s < time_s)
    {
        if (getTagStatus())
        {
            printf("INFO Статус метки появился в регионе\n");
            break;
        }

        end_s = timer.read() - begin_s;
    }

    if (error_flag && (end_s > (time_s + error_s) || end_s < (time_s - error_s)))
    {
        result = false;
    }
    else if (end_s >= error_s)
    {
        result = false;
    }

    printf("INFO Прошло времени: %f\n", end_s);
    timer.stop();

    return result;
}

int8_t Gen6Basic::getHood(command_type_t execution_method)
{
    switch (execution_method)
    {
        case PIN:
            return p_r6->isInput();

        default:
            printf("INFO Считывание состояния капота. Неизвестный метод исполнения\n");
            return -1;
    }
}

int8_t Gen6Basic::getIgn(command_type_t execution_method)
{
    if (p_out_ign == NULL)
    {
        printf("FAIL Пин \"Зажигание\" не определен\n");

        return -1;
    }

    switch (execution_method)
    {
        case PIN:
            return *p_out_ign;

        default:
            printf("INFO Считывание состояния зажигания. Неизвестный метод исполнения\n");
            return -1;
    }
}

bool Gen6Basic::isBlockSet()
{
    if (p_in_block == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Блокировка двигателя\" не определен\n");
        return false;
    }

    return !(*p_in_block);
}

bool Gen6Basic::isTagVisible(command_type_t execution_method)
{
    switch (execution_method)
    {
        case PIN:
            if (p_in_tag_status == NULL)
            {
                printf("FAIL Считывание состояния. Пин \"Статус метки\" не определен\n");
                return false;
            }

            return !(*p_in_tag_status);

        case SLP:
            return getSlpCmdStatus(DIAG_DATA_FLAG,
                                   SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                   DIAG_FLAG_BIT_TAG);

        default:
            printf("FAIL Считывание состояния \"Статус метки\": не определен метод исполнения\n");
            return false;
    }
}

void Gen6Basic::checkEngineStop(engine_start_system_t engine_start_system,
                                stop_impulse_t        stop_after_impulse_num,
                                float                 time_s)
{
    if (   engine_start_system == IGNITION_LOCK
        || engine_start_system == EXTERNAL)
    {        
        printf("INFO Проверка глушения двигателя \n");
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);

        if (waitIgn1(time_s, 1))
        {
            printf("PASS Снят потенциал с зажигания\n"); 
            
            if (stop_after_impulse_num != WITHOUT_STOP)
            {
                setIgn(PIN, OFF, TIME_DELAY::NULL_DELAY_S);
            }
        }
        else
        {
            printf("FAIL Не снят потенциал с зажигания\n");   
        }

        if (waitAcc(TIME_DELAY::FIVE_S + TIME_DELAY::STD_WIRE_EN_DELAY_S, 1)) 
        {
            printf("PASS Снят потенциал с акссесуаров\n"); 
        }
        else
        {
            printf("FAIL Не снят потенциал с акссесуаров\n");   
        }

        if (waitInspector(TIME_DELAY::FIVE_S + TIME_DELAY::STD_WIRE_EN_DELAY_S, 1)) 
        {
            printf("PASS Снят потенциал с модуля обхода иммобилайзера\n");
        }
        else
        {
            printf("FAIL Не снят потенциал с модуля обхода иммобилайзера\n");
        }
    }
    else
    {
        if (waitStartStopBtn(time_s, 0))
        {
            printf("PASS Первый импульс получен\n");

            if (stop_after_impulse_num == STOP_AFTER_FIRST_IMPULSE_RECV)
            {
                setIgn(PIN, OFF, TIME_DELAY::NULL_DELAY_S);
            }
        }
        else 
        {
            printf("FAIL Первый импульс не получен\n");
        }
                
        if (waitStartStopBtn(TIME_DELAY::FIRST_STOP_IMPULSE_REMOVE_WAIT_S, 1))
        {
            printf("PASS Первый импульс снят\n");

            if (stop_after_impulse_num == STOP_AFTER_FIRST_IMPULSE_REMOVED)
            {
                setIgn(PIN, OFF, TIME_DELAY::NULL_DELAY_S);
            }
        }
        else 
        {
            printf("FAIL Первый импульс не снят\n");
        }

        if (waitStartStopBtn(TIME_DELAY::SECOND_STOP_IMPULSE_RECV_WAIT_S, 0))
        {
            printf("PASS Второй импульс получен\n");

            if (stop_after_impulse_num == STOP_AFTER_SECOND_IMPULSE_RECV)
            {
                setIgn(PIN, OFF);
            }
            else if (stop_after_impulse_num <= STOP_AFTER_FIRST_IMPULSE_REMOVED)
            {
                printf("PASS Второй импульс получен (после выключения зажигания)\n");
            }
        }
        else 
        {
            if (stop_after_impulse_num <= STOP_AFTER_FIRST_IMPULSE_REMOVED)
            {
                printf("PASS Второй импульс не получен (после выключения зажигания)\n");
            }
            else
            {
                printf("FAIL Второй импульс не получен\n");
            }
        }

        if (waitStartStopBtn(TIME_DELAY::SECOND_STOP_IMPULSE_REMOVE_WAIT_S, 1))
        {
            printf("PASS Второй импульс снят\n");

            if (stop_after_impulse_num == STOP_AFTER_SECOND_IMPULSE_REMOVED)
            {
                setIgn(PIN, OFF);
            }
        }
        else 
        {
            printf("FAIL Второй импульс не снят\n");
        }

        if (waitStartStopBtn(TIME_DELAY::THIRD_STOP_IMPULSE_RECV_WAIT_S, 0))
        {
            printf("PASS Третий импульс получен\n");

            if (stop_after_impulse_num == STOP_AFTER_THIRD_IMPULSE_RECV)
            {
                setIgn(PIN, OFF);
            }
            else if (stop_after_impulse_num <= STOP_AFTER_SECOND_IMPULSE_REMOVED)
            {
                printf("PASS Третий импульс получен (после выключения зажигания)\n");
            }
        }
        else 
        {
            if (stop_after_impulse_num <= STOP_AFTER_SECOND_IMPULSE_REMOVED)
            {
                printf("PASS Третий импульс не получен (после выключения зажигания)\n");
            }
            else
            {
                printf("FAIL Третий импульс не получен\n");
            }
        }
        
        if (waitStartStopBtn(TIME_DELAY::THIRD_STOP_IMPULSE_REMOVE_WAIT_S, 1))
        {
            printf("PASS Третий импульс снят\n");

            if (stop_after_impulse_num == STOP_AFTER_THIRD_IMPULSE_REMOVED)
            {
                setIgn(PIN, OFF);
            }
            else if (stop_after_impulse_num == UNSUCCESSFUL_STOP)
            {
                setIgn(PIN, ON);
            }
        }
        else
        {
            printf("FAIL Третий импульс не снят\n");
        }
    }
}

void Gen6Basic::checkSupIgn(ign_statuses_t ign_status, out_statuses_t state)
{
    const char* message;


    if (!getIgn1())
    {
        message = state ? "PASS Поддержка зажигания включена\n" 
                        : "FAIL Поддержка зажигания включена\n";
    }
    else 
    {
        message = !state ? "PASS Поддержка зажигания не включена\n" 
                         : "FAIL Поддержка зажигания не включена\n";
    }

    printf("%s", message);

    if (ign_status == IGN_OFF)
    {
        setIgn(PIN, OFF);
        setIgn(PIN, OFF);
    }
}

int8_t Gen6Basic::checkSecure(int8_t model)
{
    if (model == ON)
    {
        return getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                               SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                               DIAG_GUARD_STATE_ARM);
    }
    else
    {
        return getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                               SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                               DIAG_GUARD_STATE_DISARM);
    }
}

int8_t Gen6Basic::checkSecureMobile(int8_t model)
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    printf("INFO Проверка состояния охраны через мобильное приложение (BLE)\n");

    if (model == ON)
    {
        result = p_mobileapp->getMobileStatusCurState(MOBILEAPP_DIAG_DATA_GUARD_STATE, DIAG_GUARD_STATE_ARM);
    }
    else
    {
        result = p_mobileapp->getMobileStatusCurState(MOBILEAPP_DIAG_DATA_GUARD_STATE, DIAG_GUARD_STATE_DISARM);
    }

    return result;
}

bool Gen6Basic::waitGuardState(slp_diag_data_statuses_t status,
                               bool                     state,
                               float                    time_s,
                               float                    error_time_s)
{
    return waitSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                            status,
                            state,
                            time_s,
                            error_time_s);
}

void Gen6Basic::checkInspector(int8_t state)
{
    const char* message;


    if (!getInspector()) 
    {
        message  = state ? "PASS На модуль обхода иммбобилайзера подан потенциал\n"
                         : "FAIL На модуль обхода иммбобилайзера подан потенциал\n";
    }
    else 
    {
        message  = !state ? "PASS На модуль обхода иммбобилайзера не подан потенциал\n"
                          : "FAIL На модуль обхода иммбобилайзера не подан потенциал\n";
    }

    printf("%s", message);
}

void Gen6Basic::synByIgn1(float time_s)
{
    bool ign1_potential       = 0;
    bool start_stop_potential = 0;


    // если Старт-Стоп и пытаемяся ждать потенциал на зажигании 1
    if (   p_in_ign1               == NULL
        && p_in_eng_start_stop_btn != NULL)
    {
        waitStartStopBtn(time_s, start_stop_potential);
    }
    else
    {
        waitIgn1(time_s, !ign1_potential);
    }
}

void Gen6Basic::synByInspector()
{
    int16_t exit_cycle = 0;


    while (!getInspector())
    {
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
        exit_cycle++;

        if (exit_cycle >= 3000)
        {
            break;
        }
    }
}

void Gen6Basic::waitTurbotimerTime(float                 turbotimer_time_s,
                                   engine_start_system_t engine_start_system,
                                   float                 error_time_s)
{
    Timer timer;
    float begin_s      = 0.0;
    float curr_s       = 0.0;
    bool  is_potential = false;


    printf("INFO Ожидание окончания времени работы ТТ, время ожидания: %.6f сек., погрешность: %.6f сек.\n",
           turbotimer_time_s,
           error_time_s);

    timer.start();

    if (   engine_start_system != IGNITION_LOCK
        && engine_start_system != EXTERNAL)
    {
        while (curr_s < turbotimer_time_s + error_time_s)
        {
            if (!getStartStopBtn())
            {
                is_potential = true;

                printf("INFO На кнопку старт-стоп подан потенциал (импульс остановки двигателя) через: %.6f\n", curr_s);
                break;
            }

            curr_s = timer.read() - begin_s;
        }

        if (!is_potential)
        {
            printf("INFO На кнопку старт-стоп не подан потенциал\n");
        }
        else if (curr_s < (turbotimer_time_s - error_time_s))
        {
            printf("FAIL На кнопку старт-стоп подан потенциал (импульс остановки двигателя) до окончания работы ТТ: через %.2f с\n",
                   curr_s);
        }
    }
    else
    {
        while (curr_s < turbotimer_time_s + error_time_s)
        {
            if (getIgn1())
            {
                is_potential = true;

                printf("INFO Зажигание выключено через: %.6f\n", curr_s);
                break;
            }

            curr_s = timer.read() - begin_s;
        }

        if (!is_potential)
        {
            printf("INFO С зажигания не снят потенциал\n");
        }
        else if (curr_s < (turbotimer_time_s - error_time_s))
        {
            printf("FAIL Зажигание выключено до окончания турботаймера: через %.2f с\n",
                   curr_s);
        }
    }

    timer.stop();
}

void Gen6Basic::waitAutoStartTime(float                 autostart_time_s,
                                  engine_start_system_t engine_start_system,
                                  float                 error_time_s)
{
    Timer timer;
    float as_begin_s = 0.0;
    float as_curr_s  = 0.0;


    printf("DEBUG Время ожидания окончания работы двигателя: %.2f сек.\n", autostart_time_s);
    timer.start();

    as_begin_s = timer.read();

    while (as_curr_s < autostart_time_s + error_time_s)
    {
        switch (engine_start_system)
        {
            case START_STOP_ONE_IMPULSE:
            case START_STOP_TWO_IMPULSE:
            case START_STOP_THREE_IMPULSE:

                if (!getStartStopBtn())
                {
                    if (as_curr_s < autostart_time_s - error_time_s)
                    {
                        printf("FAIL На кнопку старт-стоп подан потенциал (импульс остановки двигателя) до окончания запуска: через %.2f сек.\n", as_curr_s);
                    }

                    return;
                }

                break;
                
            case IGNITION_LOCK:

                if (getIgn1())
                {
                    if (as_curr_s < autostart_time_s - error_time_s)
                    {
                        printf("FAIL Зажигание выключено до окончания запуска: через %.2f сек.\n", as_curr_s);
                    }

                    return;
                }

                break;
                
            case EXTERNAL:

                if (getInspector())
                {
                    if (as_curr_s < autostart_time_s - error_time_s)
                    {
                        printf("FAIL Обходчик выключен до окончания запуска: через %.2f сек.\n", as_curr_s);
                    }

                    return;
                }

                break;
        }

        as_curr_s = timer.read() - as_begin_s;
    }
}

bool Gen6Basic::waitPppStartSignal(bool polarity, float wait_time_s)
{
    Timer timer;

    bool  is_potential     = false; // Потенциал получен
    bool  result           = false; // Результат
    float potential_time_s = 0.0f;  // Время получения потенциала


    timer.start();

    while (timer.read() < wait_time_s)
    {
        if (   !is_potential
            && getPppStart() == polarity)
        {
            potential_time_s = timer.read();
            is_potential     = true;
            printf("INFO Потенциал с провода контроля ППП получен через: %.2f\n", potential_time_s);
        }

        if (   is_potential
            && getPppStart() != polarity
            && timer.read() - potential_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            printf("INFO Сигнал с провода контроля ППП получен через: %.2f\n", timer.read());
            result = true;
            break;
        }
    }

    timer.stop();

    if (!result)
    {
        printf("INFO Сигнал с провода контроля ППП не получен\n");
    }

    return result;
}

void Gen6Basic::waitPppImpulse(float ppp_max_time_s,
                               bool  polarity,
                               float max_error_time_s)
{
    Timer timer;
    float ppp_begin_s = 0.0;
    float ppp_curr_s  = 0.0;


    timer.start();

    ppp_begin_s = timer.read();

    printf("INFO Ожидаем импульс остановки/запуска ППП: %.2f сек. с погрешностью %.2f сек.\n",
           ppp_max_time_s,
           max_error_time_s);

    while (ppp_curr_s < ppp_max_time_s + max_error_time_s)
    {
        if (getPppStart() == polarity)
        {
            printf("INFO Импульс остановки/запуска ППП получен после %.2f секунд\n", ppp_curr_s);
            break;
        }

        ppp_curr_s = timer.read() - ppp_begin_s;
    }

    if (ppp_curr_s < ppp_max_time_s - max_error_time_s)
    {
        printf("FAIL Импульс остановки/запуска ППП получен за пределами ожидаемого диапазона: через %.2f сек.\n", ppp_curr_s);
    }
}

bool Gen6Basic::waitPppPulse(uint32_t count,
                             bool     polarity,
                             float    time,
                             float    pulse_time,
                             bool     error_flag,
                             float    error)
{
    return waitPulse(p_in_ppp_start,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitPppPotential(float time_s,
                                 bool  polarity,
                                 float max_error_s)
{
    bool error_flag = (max_error_s == TIME_DELAY::NO_ERROR_TIME_S) ? false
                                                                   : true;


    return waitPotential(p_in_ppp_start,
                         polarity,
                         time_s,
                         error_flag,
                         max_error_s);
}

void Gen6Basic::setTagVcc()
{
    printf("INFO Включение питания метки\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE \n");
        return;
    }

    p_tag->setVcc();
}

void Gen6Basic::resetTagVcc()
{
    printf("INFO Выключение питания метки\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE\n");
        return;
    }

    p_tag->resetVcc();
}

void Gen6Basic::setR4Vcc()
{
    printf("INFO Включение питания R4\n");

    if (p_r4 == NULL)
    {
        printf("INFO Отсутствует R4\n");
        return;
    }

    p_r4->setVcc(ON);
}

void Gen6Basic::resetR4Vcc()
{
    printf("INFO Выключение питания R4\n");

    if (p_r4 == NULL)
    {
        printf("INFO Отсутствует R4\n");
        return;
    }

    p_r4->setVcc(OFF);
}

void Gen6Basic::setR6Vcc()
{
    printf("INFO Включение питания R6\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE\n");
        return;
    }

    p_r6->setVcc(ON);
}

bool Gen6Basic::getR6Vcc()
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        return false;
    }

    return p_r6->getVcc();
}

void Gen6Basic::resetR6Vcc()
{
    printf("INFO Выключение питания R6\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Нет модуля для работы с BLE\n");
        return;
    }

    p_r6->setVcc(OFF);
}

void Gen6Basic::setTagBtn()
{
    printf("INFO Зажатие кнопки метки\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Нет модуля для работы с меткой\n");
        return;
    }

    p_tag->setBtn();
}

void Gen6Basic::resetTagBtn()
{
    printf("INFO Разжатие кнопки метки\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Нет модуля для работы с меткой\n");
        return;
    }

    p_tag->resetBtn();
}

void Gen6Basic::pushTagBtn(float type)
{
    printf("INFO Нажатие кнопки метки\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Нет модуля для работы с меткой\n");
        return;
    }

    p_tag->pushButton(type);
}

void Gen6Basic::startPpp(command_type_t execution_method,
                         float          btn1_time_s)
{
    printf("INFO Запуск ППП\n");

    if ((m_device_modules & R868_MODULE) != R868_MODULE)
    {
        execution_method = SMS;
    }

    if (m_device_name == GEN6_M66)
    {
        printf("FAIL M66 не поддерживает ППП\n");
        return;
    }

    switch (execution_method)
    {
        case KEYRING:
            p_led_keyring->pushButton2Next1(btn1_time_s);
            break;

        case SMS:

            if (m_gen6_phones.size() > 0)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                               SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_ENG_HEAT_START);
            }
            else
            {
                printf("FAIL Не задан телефон устройства\n");
            }

            waitPotential(p_in_ppp_start,
                          getPolarity(ID_OUT_HEAT_CTRL),
                          TIME_DELAY::PPP_STOP_SMS_WAIT_TIME_S);
            break;

        default:
            printf("FAIL Неизвестный метод запуска ППП\n");
            break;
    }
}

void Gen6Basic::stopPpp(command_type_t execution_method,
                        int8_t         ppp_type,
                        float          btn2_time_s)
{
    printf("INFO Остановка ППП\n");

    if ((m_device_modules & R868_MODULE) != R868_MODULE)
    {
        execution_method = SMS;
    }

    if (m_device_name == GEN6_M66)
    {
        printf("FAIL M66 не поддерживает ППП\n");
        return;
    }

    switch (execution_method)
    {
        case KEYRING:
            p_led_keyring->pushButton2Next2(btn2_time_s);
            break;

        case SMS:

            if (m_gen6_phones.size() > 0)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                               SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_ENG_HEAT_STOP);
            }
            else
            {
                printf("FAIL Не задан телефон устройства\n");
            }

            if (ppp_type == 0)
            {
                waitPotential(p_in_ppp_start,
                              0,
                              TIME_DELAY::PPP_STOP_SMS_WAIT_TIME_S);
            }
            else
            {
                waitPotential(p_in_ppp_start,
                              1,
                              TIME_DELAY::PPP_STOP_SMS_WAIT_TIME_S);
            }

            break;

        default:
            printf("FAIL Неизвестный метод остановки ППП\n");
            break;
    }
}

void Gen6Basic::setPppCmdStart(command_type_t execution_method, float eng_heat_after_sms_wait_time_s)
{
    printf("INFO Запуск ППП\n");

    execution_method = getExecutionMethod(execution_method, RUN_ACTION);

    switch (execution_method)
    {
        case KEYRING:
            p_led_keyring->pushButton2Next1(KEYRING_SHORT);
            break;

        case SMS:

            if (m_gen6_phones.size() > 0)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                               SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_ENG_HEAT_START);
            }
            else
            {
                printf("FAIL Не задан телефон устройства\n");
            }

            if (   p_in_ppp_start                 != NULL
                && eng_heat_after_sms_wait_time_s >  TIME_DELAY::NULL_DELAY_S)
            {
                waitPotential(p_in_ppp_start,
                              getPolarity(ID_OUT_HEAT_CTRL),
                              eng_heat_after_sms_wait_time_s);
            }

            break;

        default:
            printf("FAIL Неизвестный метод запуска ППП\n");
    }
}

void Gen6Basic::setPppCmdStop(command_type_t execution_method, float eng_heat_after_sms_wait_time_s)
{
    printf("INFO Остановка ППП\n");

    execution_method = getExecutionMethod(execution_method, RUN_ACTION);

    switch (execution_method)
    {
        case KEYRING:
            p_led_keyring->pushButton2Next2(KEYRING_SHORT);
            break;

        case SMS:

            if (m_gen6_phones.size() > 0)
            {
                p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                               SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_ENG_HEAT_STOP);
            }
            else
            {
                printf("FAIL Не задан телефон устройства\n");
                return;
            }

            if (   p_in_ppp_start                 != NULL
                && eng_heat_after_sms_wait_time_s >  TIME_DELAY::NULL_DELAY_S)
            {
                waitPotential(p_in_ppp_start,
                              !getPolarity(ID_OUT_HEAT_CTRL),
                              eng_heat_after_sms_wait_time_s);
            }

            break;

        default:
            printf("FAIL Неизвестный тип остановки ППП: %d\n", execution_method);
    }
}
            
void Gen6Basic::setServiceBtn(bool state)
{
    printf("INFO Зажатие сервисной кнопки\n");

    if (p_service_btn == NULL)
    {
        printf("DEBUG Сервисная кнопка не определена\n");
        return;
    }

    p_service_btn->setBtn(state);
}

void Gen6Basic::clickServiceBtn(uint32_t count, 
                                float    delay_s)
{
    for (uint32_t i = 0; i < count; ++i)
    {
        printf("INFO Нажатие сервисной кнопки\n");

        if (p_service_btn == NULL)
        {
            printf("DEBUG Сервисная кнопка не определена\n");
            return;
        }

        p_service_btn->clickBtn();

        if (i < (count - 1))
        {
            wait(delay_s);
        }
    }
}

bool Gen6Basic::isServiceLedOn()
{
    printf("INFO Проверка состояния светодиода свервисной кнопки\n");

    if (p_service_btn == NULL)
    {
        printf("DEBUG Сервисная кнопка не определена\n");
        return false;
    }

    return p_service_btn->isLedOn();
}

bool Gen6Basic::waitServiceLed(out_statuses_t out_status, float timeout_s)
{
    const bool expected_status = out_status == ON; // Ожидаемый статус сервисного светодиода

    Timer      timer;                              // Таймер, отсчитывает время до конца времени ожидания


    if (expected_status)
    {
        printf("INFO Ожидание включения сервисного светодиода\n");
    }
    else
    {
        printf("INFO Ожидание отключения сервисного светодиода\n");
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_service_btn->isLedOn() == expected_status)
        {
            printf("INFO Получен ожидаемый статус сервисного светодиода через: %.6f сек.\n", timer.read());

            return true;
        }
    }

    printf("INFO Ожидаемый статус сервисного светодиода не был получен, прошло времени: %.6f сек.\n", timer.read());

    return false;
}

bool Gen6Basic::waitSrvBtnPulse(float pulse_width_s,
                                float err_time_s,
                                float timeout_s)
{
    Timer timer;
    bool  result           = false;
    bool  is_potential     = false;                    // Получен ли потенциал света
    float potential_time_s = TIME_DELAY::NULL_DELAY_S; // Время получения потенциала
    float pulse_time_s     = TIME_DELAY::NULL_DELAY_S; // Время получения импульса


    printf("INFO Ожидание сигнала светодиода сервисной кнопки, ожидаемая длительность: %.6f, погрешность: %.6f, время ожидания: %.6f\n",
           pulse_width_s,
           err_time_s,
           timeout_s);

    if (p_service_btn == NULL)
    {
        printf("DEBUG Сервисная кнопка не определена\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (   !is_potential
            &&  p_service_btn->isLedOn())
        {
            is_potential     = true;
            potential_time_s = timer.read();
        }

        if (    is_potential
            && !p_service_btn->isLedOn())
        {
            pulse_time_s = timer.read();

            printf("INFO Сигнал сервисного светодиода получен\n");

            if (   pulse_time_s - potential_time_s > pulse_width_s - err_time_s
                && pulse_time_s - potential_time_s < pulse_width_s + err_time_s)
            {
                result = true;

                printf("INFO Длительность сигнала верная\n");
                break;
            }
            else
            {
                printf("INFO Длительность сигнала неверная: %.6f\n", pulse_time_s);
                break;
            }
        } // if is_potential
    } // while (timer.read() < timeout_s)

    if (   !result
        && !is_potential)
    {
        printf("INFO Сигнал сервисного светодиода не получен\n");
    }

    return result;
}

bool Gen6Basic::waitServicePulseAmount(uint8_t amount, float time_s)
{
    if (m_device_name == GEN6_X96V2)
    {
        wait(time_s);

        return true;
    }

    return p_service_btn->waitPulseAmount(amount, time_s);
}

bool Gen6Basic::emergencyDisarming(const char *pin)
{
    char click_count_ch[5] = { 0, 0, 0, 0, 0 };
    char click_count[4]    = { 0, 0, 0, 0 };

    uint32_t pin_digit = 0;

    // при снятии кодом экстренного снятия, есть вероятность перейти в режим
    // регистрации без включения зажигания (7 раз + 1 раз при долгой вспышке
    // светодиодом), если продолжим вводить код экстр. снятия,
    // выпишутся штатные кнопки
    bool     is_reg_en = false;


    printf("INFO Запущено экстренное снятие с охраны без использования зажигания\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL Код экстренного снятия не задан\n");
            return false;
        }

        pin = m_device_pin;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (isdigit(pin[i]))
        {
            click_count_ch[i] = pin[i];
        }
    }

    pin_digit = atoi(click_count_ch);

    for (int8_t i = 3; i >= 0; --i)
    {
        click_count[i] = pin_digit % 10;
        pin_digit /= 10;
    }

    printf("INFO Код экстренного снятия:");

    for (uint8_t i = 0; i < 4; ++i)
    {
        printf(" %d", click_count[i]);
    }
    
    printf("\n");

    for (uint8_t i = 0; i < click_count[0]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");
    wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S); // даем отморгаться в АО - лишний импульс

    p_service_btn->waitPulseAmount(1, TIME_DELAY::AFTER_FIRST_PIN_NUM_SBTN_S);

    for (uint8_t i = 0; i < click_count[1]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    p_service_btn->waitPulseAmount(2, TIME_DELAY::AFTER_SECOND_PIN_NUM_SBTN_S);

    if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                        SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                        DIAG_GUARD_STATE_REG))
    {
        is_reg_en = true;
    }

    if (!is_reg_en)
    {
        for (uint8_t i = 0; i < click_count[2]; ++i)
        {
            clickServiceBtn();
            wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
        }
        
        printf("INFO Ожидание\n");

        p_service_btn->waitPulseAmount(3, TIME_DELAY::AFTER_THIRD_PIN_NUM_SBTN_S);

        if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                            DIAG_GUARD_STATE_REG))
        {
            is_reg_en = true;
        }

        if (!is_reg_en)
        {
            for (uint8_t i = 0; i < click_count[3]; ++i)
            {
                clickServiceBtn();
                wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
            }
            
            printf("INFO Ожидание\n");

            p_service_btn->waitPulseAmount(4, TIME_DELAY::AFTER_FOURTH_PIN_NUM_SBTN_S);

            // ожидание успешности ввода кода
            p_service_btn->waitPulseAmount(2, TIME_DELAY::AFTER_ENTER_PIN_SBTN_S);

            for (uint8_t i = click_count[3]; i < 9; ++i)
            {
                wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
            }
        }
    }

    if (is_reg_en)
    {
        setIgn(PIN, ON);
        setIgn(CAN_USE, ON);
        setIgn(PIN, OFF);
        setIgn(CAN_USE, OFF);
    }

    return true;
}

void Gen6Basic::changeEmergencyDisarming(const char *old_pin, const char *new_pin)
{
    uint8_t click_numbers = 4;
    
    for (uint8_t i = 0; i < click_numbers; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    setIgn(PIN, ON, TIME_DELAY::NULL_DELAY_S);
    
    if (waitSirenPulse(click_numbers, 1, TIME_DELAY::FIVE_S, TIME_DELAY::LONG_SIREN_IMPULSE_S))
    {
        printf("INFO Длинные сигналы сиреной присутствуют\n");
    }
    else
    {
        printf("INFO Длинные сигналы сиреной отсутствуют\n");
    }
    
    uint8_t short_number_impulse = 2;
    
    if (waitSirenPulse(short_number_impulse, 1, TIME_DELAY::FIVE_S, TIME_DELAY::SHORT_SIREN_IMPULSE_S))
    {
        printf("INFO Короткие сигналы сиреной присутствуют\n");
    }
    else
    {
        printf("INFO Короткие сигналы сиреной отсутствуют\n");
    }

    if (checkChangeEmergencyDisarming(old_pin, NOT_EXIT))
    {
        printf("INFO Алгоритм проверки смены кода экстренного снятия верен\n");
    }
    else
    {
        printf("INFO Алгоритм проверки смены кода экстренного снятия не верен\n");
    }
    
    if (!waitServicePulseAmount(short_number_impulse, TIME_DELAY::TEN_S))
    {
        printf("INFO Вспышки подтверждения светодиодом не получены\n");
    }
    else
    {
        printf("INFO Вспышки подтверждения светодиодом получены\n");
    }

    setIgn(PIN, ON);
    
    if (checkChangeEmergencyDisarming(new_pin, NOT_EXIT))
    {
        printf("INFO Алгоритм проверки смены кода экстренного снятия верен\n");
    }
    else
    {
        printf("INFO Алгоритм проверки смены кода экстренного снятия не верен\n");
    }
    
    if (!waitServicePulseAmount(2, TIME_DELAY::TEN_S))
    {
        printf("INFO Вспышки подтверждения светодиодом не получены\n");
    }
    else
    {
        printf("INFO Вспышки подтверждения светодиодом получены\n");
    }

    setIgn(PIN, ON);
  
    if (checkChangeEmergencyDisarming(new_pin, NOT_EXIT))
    {
        printf("INFO Алгоритм проверки смены кода экстренного снятия верен\n");
    }
    else
    {
        printf("INFO Алгоритм проверки смены кода экстренного снятия не верен\n");
    }
    
    if (!waitServicePulseAmount(short_number_impulse, TIME_DELAY::TEN_S))
    {
        printf("INFO Вспышки подтверждения светодиодом не получены\n");
    }
    else
    {
        printf("INFO Вспышки подтверждения светодиодом получены\n");
    }
}

bool Gen6Basic::checkChangeEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits)
{
    char click_count_ch[5] = { 0, 0, 0, 0, 0 };
    char click_count[4]    = { 0, 0, 0, 0 };

    uint32_t pin_digit = 0;


    printf("INFO Запущено экстренное снятие с охраны без использования зажигания\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL Код экстренного снятия не задан\n");
            return false;
        }
        pin = m_device_pin;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (isdigit(pin[i]))
        {
            click_count_ch[i] = pin[i];
        }
    }

    pin_digit = atoi(click_count_ch);

    for (int8_t i = 3; i >= 0; --i)
    {
        click_count[i] = pin_digit % 10;
        pin_digit /= 10;
    }

    printf("INFO Код экстренного снятия:");

    for (uint8_t i = 0; i < 4; ++i)
    {
        printf(" %d", click_count[i]);
    }

    printf("\n");

    if (digits == BEFORE_FIRST_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[0]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO Ожидание\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_FIRST_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_FIRST_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO Вспышка подтверждения 1 цифры не получена\n");
        return false;
    }
    else
    {
        printf("INFO Вспышка подтверждения 1 цифры получена\n");
    }

    wait(TIME_DELAY::BTN_BEFORE_IGN_ON_DELAY_S);

    setIgn(PIN, ON);

    if (digits == BEFORE_SECOND_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[1]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO Ожидание\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_SECOND_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_SECOND_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO Вспышки подтверждения 2 цифры не получены\n");
        return false;
    }
    else
    {
        printf("INFO Вспышки подтверждения 2 цифры получены\n");
    }

    wait(TIME_DELAY::BTN_BEFORE_IGN_ON_DELAY_S);

    setIgn(PIN, ON);

    if (digits == BEFORE_THIRD_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[2]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO Ожидание\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_THIRD_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_THIRD_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO Вспышки подтверждения 3 цифры не получены\n");
        return false;
    }
    else
    {
        printf("INFO Вспышки подтверждения 3 цифры получены\n");
    }

    wait(TIME_DELAY::BTN_BEFORE_IGN_ON_DELAY_S);

    setIgn(PIN, ON);

    if (digits == BEFORE_FOURTH_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[3]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO Ожидание\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_FOURTH_DIGIT)
    {
        if (!waitServicePulseAmount(1, TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO Вспышка ошибки светодиодом не получена\n");
            return true;
        }
        else
        {
            printf("INFO Вспышка ошибки светодиодом получена\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_FOUTH_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO Вспышки подтверждения 4 цифры не получены\n");
        return false;
    }
    else
    {
        printf("INFO Вспышки подтверждения 4 цифры получены\n");
    }

    return true;
}

bool Gen6Basic::checkEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits)
{
    char click_count_ch[5] = { 0, 0, 0, 0, 0 };
    char click_count[4]    = { 0, 0, 0, 0 };

    uint32_t pin_digit = 0;

    printf("INFO Запущено экстренное снятие с охраны без использования зажигания\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL Код экстренного снятия не задан\n");
            return false;
        }
        pin = m_device_pin;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (isdigit(pin[i]))
        {
            click_count_ch[i] = pin[i];
        }
    }

    pin_digit = atoi(click_count_ch);

    for (int8_t i = 3; i >= 0; --i)
    {
        click_count[i] = pin_digit % 10;
        pin_digit /= 10;
    }

    printf("INFO Код экстренного снятия:");

    for (uint8_t i = 0; i < 4; ++i)
    {
        printf(" %d", click_count[i]);
    }
    
    printf("\n");

    for (uint8_t i = 0; i < click_count[0]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");
    
    bool result = true;
    
    if (!waitServicePulseAmount(1, TIME_DELAY::AFTER_FIRST_PIN_NUM_SBTN_S))
    {
        printf("INFO Вспышки светодиодом не получены после ввода 1 цифры\n");
        result = false;
    }
    else
    {
        printf("INFO Вспышки светодиодом получены после ввода 1 цифры\n");
        result = true;
    }
        
    if (digits == AFTER_FIRST_DIGIT)
    {
        return result;
    }
    
    wait(TIME_DELAY::HALF_S);

    for (uint8_t i = 0; i < click_count[1]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    if (!waitServicePulseAmount(2, TIME_DELAY::AFTER_SECOND_PIN_NUM_SBTN_S))
    {
        printf("INFO Вспышки светодиодом не получены после ввода 2 цифры\n");
        result = false;
    }
    else
    {
        printf("INFO Вспышки светодиодом получены после ввода 2 цифры\n");
        result = true;
    }
    
    if (digits == AFTER_SECOND_DIGIT)
    {
        return result;
    }

    wait(TIME_DELAY::HALF_S);
    
    for (uint8_t i = 0; i < click_count[2]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    if (!waitServicePulseAmount(3, TIME_DELAY::AFTER_THIRD_PIN_NUM_SBTN_S))
    {
        printf("INFO Вспышки светодиодом не получены после ввода 3 цифры\n");
        result = false;
    }
    else
    {
        printf("INFO Вспышки светодиодом получены после ввода 3 цифры\n");
        result = true;
    }
    
    if (digits == AFTER_THIRD_DIGIT)
    {
        return result;
    }

    wait(TIME_DELAY::HALF_S);
    
    for (uint8_t i = 0; i < click_count[3]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    if (!waitServicePulseAmount(4, TIME_DELAY::AFTER_FOURTH_PIN_NUM_SBTN_S))
    {
        printf("INFO Вспышки светодиодом не получены после ввода 4 цифры\n");
        result = false;
    }
    else
    {
        printf("INFO Вспышки светодиодом получены после ввода 4 цифры\n");
        result = true;
    }
    
    if (digits == AFTER_FOURTH_DIGIT)
    {
        return result;
    }
    
    return true;
}

bool Gen6Basic::emergencyDisarmingIgn(const char *pin)
{
    char click_count_ch[5] = { 0, 0, 0, 0, 0 };
    char click_count[4]    = { 0, 0, 0, 0 };

    uint32_t pin_digit = 0;

    printf("INFO Запущено экстренное снятие с охраны\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL Код экстренного снятия не задан\n");
            return false;
        }

        pin = m_device_pin;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (isdigit(pin[i]))
        {
            click_count_ch[i] = pin[i];
        }
    }

    pin_digit = atoi(click_count_ch);

    for (int8_t i = 3; i >= 0; --i)
    {
        click_count[i] = pin_digit % 10;
        pin_digit /= 10;
    }

    printf("INFO Код экстренного снятия:");

    for (uint8_t i = 0; i < 4; ++i)
    {
        printf(" %d", click_count[i]);
    }

    printf("\n");

    setIgn(PIN, ON);
    wait(TIME_DELAY::ONE_S);

    for (uint8_t i = 0; i < click_count[0]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    setIgn(PIN, OFF);
    wait(TIME_DELAY::AFTER_FIRST_PIN_NUM_SBTN_S);

    setIgn(PIN, ON);
    wait(TIME_DELAY::ONE_S);

    for (uint8_t i = 0; i < click_count[1]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    setIgn(PIN, OFF);
    wait(TIME_DELAY::AFTER_SECOND_PIN_NUM_SBTN_S);

    setIgn(PIN, ON);
    wait(TIME_DELAY::ONE_S);

    for (uint8_t i = 0; i < click_count[2]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    setIgn(PIN, OFF);
    wait(TIME_DELAY::AFTER_THIRD_PIN_NUM_SBTN_S);

    setIgn(PIN, ON);
    wait(TIME_DELAY::ONE_S);

    for (uint8_t i = 0; i < click_count[3]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    setIgn(PIN, OFF);
    wait(TIME_DELAY::AFTER_FOURTH_PIN_NUM_SBTN_S);

    return true;
}

bool Gen6Basic::registrationMode(bool state, registration_queue_mode_t reg_queue_mode)
{
    char msg[9];

    switch (m_device_name)
    {
        case GEN6_X96:
        case GEN6_X96V2:
        case GEN6_A96:
        case GEN6_E96:
        case GEN6_M96:
        case GEN6_S96:

            if (!state)
            {
                printf("INFO Выход из режима регистрации\n");
                setIgn(PIN, OFF);

                // Выйти из очереди устройств на регистрацию
                if (reg_queue_mode == REGISTRATION_QUEUE_MODE_EN)
                {
                    return RegistrationQueue::removeFromQueue(p_serial);
                }
            }
            else
            {
                printf("INFO Вход в режим регистрации\n");

                // Встать в очередь устройств на регистрацию
                if (reg_queue_mode == REGISTRATION_QUEUE_MODE_EN)
                {
                    if (!RegistrationQueue::addToQueueAndWait(p_serial))
                    {
                        // Конец очереди не достигнут, выходим с очереди не регистрируясь
                        RegistrationQueue::removeFromQueue(p_serial);
                        return false;
                    }
                }

                for (uint8_t i = 0; i < 7; ++i)
                {
                    clickServiceBtn();
                    wait(TIME_DELAY::MIN_BTN_BETWEEN_CLICK_S);
                }

                setIgn(PIN, ON);
                setIgn(CAN_USE, ON);
                p_service_btn->waitPulseAmount(7, TIME_DELAY::FIFTEEN_S);
            }

            break;

        case GEN6_M66:
            setIgn(CAN_USE, ON);
            strcpy(msg, "reg ");
            strcat(msg, m_device_pin);

            if (p_gsm->inspectionCyrillicSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                                                    msg,
                                                    SMS_MSG_STR::SMS_ANSW_STR_REG_DEV_ENTER) != SIM900ACT_TRUESMS)
            {
                printf("FAIL Test result: Fault. Режим регистрации. Ошибка чтения СМС сообщения.\n");
            }

            break;

        default:
            return false;
    }

    return true;
}

bool Gen6Basic::addDeviceToRegistrationQueue()
{
    if (!RegistrationQueue::addToQueueAndWait(p_serial))
    {
        // Конец очереди не достигнут, выходим с очереди
        printf("FAIL Ошибка на стороне AT: Неудачная постановка устройства в очередь на регистрацию\n");
        RegistrationQueue::removeFromQueue(p_serial);

        return false;
    }

    return true;
}

void Gen6Basic::setHandsFreeKeyring()
{
    if ((getModules() & R868_MODULE) != R868_MODULE)
    {
        return;
    }

    pushLcdKeyringBut4(KEYRING_LONG_1);
    wait(TIME_DELAY::REMOTE_MENU_DELAY_S);

    pushLcdKeyringBut23(KEYRING_SHORT);
    wait(TIME_DELAY::TWO_S);

    pushLcdKeyringBut4(KEYRING_LONG_1);
    wait(TIME_DELAY::TWO_S);

    printf("INFO Алгоритм включения/выключения свободных рук по брелку выполнен\n");
}

void Gen6Basic::setServiceModeKeyring()
{
    pushLcdKeyringBut4(KEYRING_LONG_1);
    wait(TIME_DELAY::REMOTE_MENU_DELAY_S);

    pushLcdKeyringBut1(KEYRING_SHORT);
    wait(TIME_DELAY::TWO_S);

    pushLcdKeyringBut23(KEYRING_SHORT);
    wait(TIME_DELAY::TWO_S);

    pushLcdKeyringBut4(KEYRING_SHORT);
    wait(TIME_DELAY::TWO_S);

    pushLcdKeyringBut4(KEYRING_LONG_1);
    wait(TIME_DELAY::TWO_S);

    printf("INFO Алгоритм включения/выключения сервисного режима по брелку выполнен\n");
}

bool Gen6Basic::setServiceModeTag(bool state)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        setServiceModeKeyring();
    }
    else
    {
        for (uint8_t i = 0; i < 2; ++i)
        {
            if (   (   state
                    && !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                        SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                        DIAG_GUARD_STATE_SERVICE))
                || (   !state
                    && getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                       SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                       DIAG_GUARD_STATE_SERVICE)))

            {
                setTagVcc();
                wait(TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);
                pushTagBtn(TAG_LONG_2);
                wait(TIME_DELAY::EIGHT_S);
                resetTagVcc();
                wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
                printf("INFO Алгоритм включения/выключения сервисного режима с помощью метки выполнен\n");
            }
        }
    }

    return waitSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                            DIAG_GUARD_STATE_SERVICE,
                            state,
                            TIME_DELAY::TWO_S);
}

bool Gen6Basic::setServiceMode(out_statuses_t out_status)
{
    const bool status = out_status == ON; // Желаемый статус сервисного режима


    printf("INFO Управление сервисным режимом\n");

    if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
    {
        printf("INFO Управление сервисным режимом с помощью метки\n");
        setServiceModeTag(status);
    }
    else if ((m_device_modules & R868_MODULE) == R868_MODULE)
    {
        printf("INFO Управление сервисным режимом с помощью брелока\n");
        setServiceModeKeyring();
    }
    else
    {
        printf("INFO Нет модулей для включения сервисного режима\n");

        return false;
    }

    return true;
}

bool Gen6Basic::getSlpCmdStatus(slp_statuses_t addr,
                                uint32_t       size,
                                uint32_t       status)
{
    return p_slp->getSlpStatus(addr, size, status) == READSLP_OK;
}

bool Gen6Basic::waitSlpCmdStatus(slp_statuses_t addr,
                                 uint32_t       size,
                                 uint32_t       status,
                                 bool           state,
                                 float          time_s,
                                 float          error_time_s,
                                 bool           error_time_use)
{
    Timer timer;
    timer.start();

    float begin     = timer.read();
    float end       = begin;
    float comm_time = time_s;
    bool  result    = false;

    if (error_time_s != TIME_DELAY::NO_ERROR_TIME_S)
    {
        comm_time += error_time_s;
    }

    while (end < comm_time)
    {
        if (state && getSlpCmdStatus(addr, size, status))
        {
            result = true;
            break;
        }
        else if (!state && !getSlpCmdStatus(addr, size, status))
        {
            result = true;
            break;
        }

        end = timer.read() - begin;
    }

    if (error_time_use)
    {
        if (   (error_time_s >= TIME_DELAY::NULL_DELAY_S)
            && (   (end  > (time_s + error_time_s))
                || (end <= (time_s - error_time_s))))
        {
            printf("INFO Статус системы получен за пределами погрешности\n");
            result = NONE;
        }
    }

    printf("DEBUG Время ожидания статуса системы: %.2f\n", end);
    timer.stop();

    return result;
}

bool Gen6Basic::setBypassType(slp_imit_bp_type_t bp_type)
{
    printf("INFO Установка типа обхода: %d\n", bp_type);

    return p_slp->setBypassType(bp_type);
}

bool Gen6Basic::setBypassMode(slp_imit_bp_mode_t bp_mode)
{
    printf("INFO Установка режима обхода: %d\n", bp_mode);

    return p_slp->setBypassMode(bp_mode);
}
  
void Gen6Basic::setArmBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_ARM);
}

void Gen6Basic::setDisarmBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_DISARM);
}

void Gen6Basic::setEngineStartBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_ENGINE_START);
}

void Gen6Basic::setEngineStopBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_ENGINE_STOP);
}

void Gen6Basic::setPreheaterStartBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_PREHEATER_START);
}

void Gen6Basic::setPreheaterStopBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_PREHEATER_STOP);
}

void Gen6Basic::setSignalBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_SIGNAL);
}

void Gen6Basic::setOpenTrunkBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_OPEN_TRUNK);
}

void Gen6Basic::setBypassLearnBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_BYPASS_LEARN);
}

void Gen6Basic::setBypassLearnClearBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_BYPASS_LEARN_CLEAR);
}

bool Gen6Basic::setRegBit(registration_queue_mode_t reg_queue_mode)
{
    printf("INFO Включение режима регистрации через регион\n");

    if (   (    (reg_queue_mode == REGISTRATION_QUEUE_MODE_EN)
             &&  !addDeviceToRegistrationQueue())
        ||  !p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_REG))
    {
        printf("INFO Включение режима регистрации неудачно\n");

        return false;
    }

    return true;
}

bool Gen6Basic::setAndCheckRegBit(registration_queue_mode_t reg_queue_mode)
{
    if (!setRegBit(reg_queue_mode))
    {
        return false;
    }

    if (!waitGuardState(DIAG_GUARD_STATE_REG,
                        ON,
                        TIME_DELAY::MAX_WAITING_REGION_STATE_S))
    {
        printf("INFO Переход в режим регистрации не выполнен\n");

        return false;
    }

    return true;
}

void Gen6Basic::setResetBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_RESET);
}

void Gen6Basic::setBleSettingModeOffBit()
{
    p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_BLE_PROG_OFF);
}

void Gen6Basic::setAuxSensor1()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, DIAG_SENSOR_BIT_AUX_1);
}

void Gen6Basic::setAuxSensorFirstAnalog(out_statuses_t status, float delay_s)
{
    const std::string status_str = status == ON ? "включен" 
                                                : "выключен";


    printf("INFO Выход \"Доп. датчик 1\" %s\n", status_str.c_str());

    if (p_out_additsensor == NULL)
    {
        printf("INFO Пин \"Доп. датчик 1\" не определен, управление произведено не будет\n");
        return;
    }

    *p_out_additsensor = status == ON;

    wait(delay_s);
}

void Gen6Basic::setAuxSensor2()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, DIAG_SENSOR_BIT_AUX_2);
}

void Gen6Basic::setAuxSensorSecondAnalog(out_statuses_t status, float delay_s)
{
    const std::string status_str = status == ON ? "включен" 
                                                : "выключен";


    printf("INFO Выход \"Доп. датчик 2\" %s\n", status_str.c_str());

    if (p_out_sensor_aux2 == NULL)
    {
        printf("INFO Пин \"Доп. датчик 2\" не определен, управление произведено не будет\n");
        return;
    }

    *p_out_sensor_aux2 = status == ON;

    wait(delay_s);
}

void Gen6Basic::setShockSensorHigh()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, DIAG_SENSOR_BIT_SHOCK_HIGH);
}
        
void Gen6Basic::setShockSensorLow()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, DIAG_SENSOR_BIT_SHOCK_LOW);
}

void Gen6Basic::setMoveSensor()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, DIAG_SENSOR_BIT_MOVE);
}        

void Gen6Basic::setTiltSensor()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, DIAG_SENSOR_BIT_TILT);
}    

void Gen6Basic::setSensor(slp_diag_data_sensors_statuses_t sensor_type)
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, sensor_type);
}

void Gen6Basic::setSensorNoDelay(slp_diag_data_sensors_statuses_t sensor_type)
{
    p_slp->setSlpStatusNoWaitAnsw(DIAG_DATA_SENSORS, sensor_type);
}

void Gen6Basic::setEngineTemperatureSensor(int8_t value)
{
    printf("INFO Установка температуры двигателя по SLP, значение: %d* C\n", value);
    p_slp->setSlpStatus(DIAG_DATA_TEMP_ENG, value);
}

void Gen6Basic::setEngineTemperatureSensorNoDelay(int8_t value)
{
    printf("INFO Установка температуры двигателя по SLP, значение: %d* C\n", value);
    p_slp->setSlpStatusNoWaitAnsw(DIAG_DATA_TEMP_ENG, value);
}

void Gen6Basic::setBoardTemperatureSensor(int8_t value)
{
    printf("INFO Установка температуры на плате по SLP, значение: %d* C\n", value);
    p_slp->setSlpStatus(DIAG_DATA_TEMP_BOARD, value);
}

void Gen6Basic::setEngineTemperatureCanLadaVesta(int8_t value)
{
    printf("INFO Установка температуры двигателя по CAN, значение: %d* C\n", value);
    p_can_actions->sendEngineTemperatureLadaVesta(value);
}

alarm_signal_t Gen6Basic::getAlarmSignal(float time, float time_error)
{
    Timer timer;
    alarm_signal_t result = NOSIGNAL;

    timer.start();
    float begin = timer.read();
    float end   = 0.0;
    float pulse = begin;

    float curr_pulse = 0.0;

    if (time_error >= TIME_DELAY::NULL_DELAY_S)
    {
        time = time + time_error;
    }

    while (end < time)
    {
        if (getAlarm())
        {
            pulse = timer.read();

            while (curr_pulse < TIME_DELAY::ONE_S)
            {
                if (!getAlarm())
                {
                    result = PULSED;
                    break;
                }

                curr_pulse = timer.read() - pulse;
            }

            if (curr_pulse >= TIME_DELAY::ONE_S && getAlarm())
            {
                result = CONST;            
            }

            break;
        }

        end = timer.read() - begin;
    }

    if (end >= time)
    {
        curr_pulse = TIME_DELAY::NULL_DELAY_S;
        pulse = timer.read();

        while (curr_pulse < TIME_DELAY::MIN_WIRE_EN_DELAY_S)
        {
            if (getAlarm())
            {
                result = PULSED;
                break;
            }

            curr_pulse = timer.read() - pulse;
        }
    }

    if (   (time_error >= TIME_DELAY::NULL_DELAY_S)
        && (   (end  > (time + time_error))
            || (end <= (time - time_error))))
    {
        printf("INFO Сигнал получен за пределами погрешности\n");
        result = NONE;
    }

    switch (result)
    {
        case NONE:
            printf("TRACE Состояние сирены не определено\n");
            break;

        case PULSED:
            printf("TRACE Сигнал сирены: смена потенциалов\n");
            break;

        case CONST:
            printf("TRACE Сигнал сирены постоянен\n");
            break;

        case NOSIGNAL:
            printf("TRACE Сигнал сирены отсутствует\n");
            break;
    }

    timer.stop();

    return result;
}

bool Gen6Basic::waitBlock(float          time_s,
                          float          error_time_s,
                          bool           status,
                          command_type_t execution_method)
{
    Timer timer;
    bool  result = false;

    float wait_time_s  = time_s; // Время ожидания блокировки с учетом погрешности
    float block_time_s = 0.0f;   // Время получения статуса блокировки


    printf("INFO Время ожидания блокировки: %.2f сек.\n", time_s);

    if (error_time_s >= TIME_DELAY::NULL_DELAY_S)
    {
        wait_time_s += error_time_s;

        printf("INFO Погрешность ожидания: %.2f сек.\n", error_time_s);
    }

    timer.start();

    while (timer.read() < wait_time_s)
    {
        if (status && getBlock(execution_method))
        {
            block_time_s = timer.read();
            result       = true;

            printf("INFO Выход блокировки включен (потенциал получен)\n");
            break;
        }
        else if (!status && !getBlock(execution_method))
        {
            block_time_s = timer.read();
            result       = true;

            printf("INFO Выход блокировки выключен (потенциал снят)\n");
            break;
        }
    }

    if (status && !getBlock(execution_method))
    {
        printf("INFO Выход блокировки не включен (потенциал снят)\n");
    }
    else if (!status && getBlock(execution_method))
    {
        printf("INFO Выход блокировки не выключен (потенциал не снят)\n");
    }

    timer.stop();
    printf("INFO Прошло времени: %.2f\n", timer.read());

    if (   (error_time_s                >= TIME_DELAY::NULL_DELAY_S)
        && (fabs(time_s - block_time_s) >  error_time_s))
    {
        printf("INFO Статус блокировки получен за пределами ожидаемого диапазона\n");
        result = false;
    }

    return result;
}

bool Gen6Basic::setAnj(bool        state,
                       const char *pin,
                       const char *gsm_owner_access_pw,
                       bool        no_wait_message)
{
    char buf_in_msg[MAX_BIGMSG_SIZE];
    char buf_curr_msg[MAX_BIGMSG_SIZE];
    char gsm_pw[6];

    char *buf_cmd;
    char *buf_pin;

    char *curr_ch;

    int32_t count = 0;

    char out_msg[8];
    char dis_sms[7];


    memset(gsm_pw, 0, 6);

    if (   (m_device_name == GEN6_X96)
        || (m_device_name == GEN6_M96) 
        || (m_device_name == GEN6_M66))
    {
        strcpy(dis_sms, "%s\n%s\n");
    }
    else
    {
        strcpy(dis_sms, "%s %s\n");
    }

    if (m_gen6_phones.size() < 1)
    {
        printf("INFO Телефон устройства не добавлен\n");
        return false;
    }

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL Код экстренного снятия не задан\n");
            return false;
        }

        pin = m_device_pin;
    }

    if (gsm_owner_access_pw != NULL)
    {
        strcpy(gsm_pw, gsm_owner_access_pw);
        gsm_pw[4] = ' ';
    }

    if (!state)
    {
        char command_off[8];

        strcpy(command_off, gsm_pw);
        strcat(command_off, "30 ");
        strcat(command_off, pin);
        printf("INFO Сформирована команда: %s\n", command_off);

        if (p_gsm->inspectionCyrillicSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                                         command_off,
                                         Sim900::combineStr(dis_sms,
                                                            SMS_MSG_STR::SMS_ANSW_STR_AHJ_OFF,
                                                            SMS_MSG_STR::SMS_ANSW_STR_USER_PIN_ENTERED).c_str()) != SIM900ACT_TRUESMS)
        {
            printf("INFO Ошибка при чтении СМС сообщения\n");
            return false;
        }

        return true;
    }

    memset(buf_in_msg,   0, MAX_BIGMSG_SIZE);
    memset(buf_curr_msg, 0, MAX_BIGMSG_SIZE);
    memset(out_msg,      0,               8);

    strcpy(buf_in_msg, sms_msg_str_t::SMS_ANSW_STR_ATTENTION);
    strcat(buf_in_msg, sms_msg_str_t::SMS_ANSW_STR_WARNING_AHJ_ON);

    count = p_gsm->getSymbolsCount(buf_in_msg);

    if (p_gsm->inspectionCyrillicSmsCmpFor(m_gen6_phones[DEVICE_PHONE_NO_1],
                                           Sim900::combineStr("%s31", gsm_pw).c_str(),
                                           buf_in_msg,
                                           count,
                                           buf_curr_msg,
                                           TIME_DELAY::THREE_MINUTE) != SIM900ACT_TRUESMS)
    {
        printf("INFO Ошибка при чтении СМС сообщения\n");
        return false;
    }

    curr_ch = buf_curr_msg;

    for (uint32_t i = 0; !isdigit(*curr_ch); ++i)
    {
        if (i >= MAX_BIGMSG_SIZE)
        {
            return false;
        }
        ++curr_ch;
    }

    buf_cmd = curr_ch;

    if (strstr(buf_cmd, "31") == NULL)
    {
        printf("INFO Не найдена команда 31\n");
        return false;
    }

    do
    {
        ++curr_ch;
    } while (!isspace(*curr_ch));

    buf_pin = curr_ch;

    do
    {
        ++curr_ch;
    } while (*curr_ch != 0);

    curr_ch = 0;

    printf("INFO Получен пин код: %s\n", buf_pin);

    strcpy(out_msg, gsm_pw);
    strcat(out_msg, "31");
    strcat(out_msg, buf_pin);

    printf("INFO Сформировано сообщение %s\n", out_msg);

    memset(buf_in_msg,   0, MAX_BIGMSG_SIZE);
    memset(buf_curr_msg, 0, MAX_BIGMSG_SIZE);

    strcpy(buf_in_msg, sms_msg_str_t::SMS_ANSW_STR_AHJ_ON);
    strcat(buf_in_msg, " ");
    strcat(buf_in_msg, sms_msg_str_t::SMS_ANSW_STR_GUARD_ALERT_MAIN);

    count = p_gsm->getSymbolsCount(buf_in_msg);

    if (!no_wait_message)
    {
        if (p_gsm->inspectionCyrillicSmsCmpFor(m_gen6_phones[DEVICE_PHONE_NO_1],
                                               out_msg,
                                               buf_in_msg,
                                               count,
                                               buf_curr_msg,
                                               TIME_DELAY::THREE_MINUTE) != SIM900ACT_TRUESMS)
        {
            printf("INFO Ошибка при чтении СМС сообщения\n");
            return false;
        }
    }
    else
    {
        if (!p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                            out_msg,
                            TIME_DELAY::MIN_SMS_STATUS_CONFIRMATION_TIME_MS))
        {
            printf("INFO Ошибка отправки смс\n");
            return false;            
        }
    }

    return true;
}

bool Gen6Basic::registerMobile()
{
    bool result = false;

    printf("INFO Регистрация мобильного приложения (BLE)\n");

    if (checkMobile())
    {
        printf("REGMOBILE \n");

        result = p_service_btn->waitPulseAmount(2, TIME_DELAY::MAX_MOBILEAPP_ANSW_DELAY_S);
    }

    return result;
}

bool Gen6Basic::checkMobile()
{
    if (!p_mobileapp->getInitStatus())
    {
        printf("INFO Мобильное приложение (BLE) не инициализировано\n");

        return false;
    }

    return true;
}

bool Gen6Basic::registerTag()
{
    printf("INFO Регистрация метки\n");

    p_tag->resetVcc();
    wait(TIME_DELAY::ONE_S);
    p_tag->setBtn();
    wait(TIME_DELAY::ONE_S);
    p_tag->setVcc(false);
    wait(TIME_DELAY::ONE_S);
    p_tag->resetBtn();

    if (m_device_name != GEN6_M66)
    {
        p_service_btn->waitPulseAmount(2, TIME_DELAY::FIFTEEN_S);
    }
    else
    {
        wait(TIME_DELAY::FIFTEEN_S);
    }

    p_tag->resetVcc();

    return true;
}

bool Gen6Basic::registerLedKeyring()
{
    printf("INFO Регистрация доп. брелка\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED брелок не определен\n");
        return false;
    }

    p_led_keyring->pushButtons12(KEYRING_SHORT);

    return waitServicePulseAmount(2, TIME_DELAY::STD_REGISTER_DEVICE_TIME_S);
}

bool Gen6Basic::registerLcdKeyring()
{
    printf("INFO Регистрация ЖК брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return false;
    }

    p_lcd_keyring->pushButtons12(KEYRING_SHORT);

    if (!waitServicePulseAmount(2, TIME_DELAY::STD_REGISTER_DEVICE_TIME_S))
    {
        p_lcd_keyring->pushButtons23(KEYRING_SHORT);
        wait(TIME_DELAY::TWO_S);

        p_lcd_keyring->pushButtons12(KEYRING_SHORT);

        return waitServicePulseAmount(2, TIME_DELAY::STD_REGISTER_DEVICE_TIME_S);
    }

    return true;
}

bool Gen6Basic::registerR6()
{
    bool result;


    printf("INFO Регистрация R6\n");

    if (p_r6 == NULL)
    {
        printf("FAIL R6 не инициализирован\n");
        return false;
    }

    p_r6->setVcc(OFF);
    wait(TIME_DELAY::VCC_R6_OFF_DELAY_S);

    p_r6->setCtrl(ON);
    wait(TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    p_r6->setVcc(ON);

    if (m_device_name != GEN6_M66)
    {
        result = waitServicePulseAmount(COUNT_STRUCT::BTN_CMD_SUCCESS_LED_LIGHTS_COUNT,
                                        TIME_DELAY::BTN_RELAY_REG_LIGHTS_WAIT_TIME_S);
    }
    else
    {
        wait(TIME_DELAY::BTN_RELAY_REG_LIGHTS_WAIT_TIME_S);

        result = true;
    }

    p_r6->setCtrl(OFF);

    return result;
}

bool Gen6Basic::registerR6Eco()
{
    bool result;


    printf("INFO Регистрация R6 ECO\n");

    if (p_r6eco == NULL)
    {
        printf("FAIL R6 ECO не инициализирован\n");
        return false;
    }

    p_r6eco->setVcc(OFF);
    wait(TIME_DELAY::R6ECO_STATUS_WAIT_TIME_S);

    p_r6eco->setCtrl(ON);
    wait(TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    p_r6eco->setVcc(ON);

    result = waitServicePulseAmount(COUNT_STRUCT::BTN_CMD_SUCCESS_LED_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_RELAY_REG_LIGHTS_WAIT_TIME_S);

    p_r6eco->setCtrl(OFF);

    return result;
}

R6 *Gen6Basic::getR6()
{
    return p_r6;
}

R6Eco *Gen6Basic::getR6Eco()
{
    return p_r6eco;
}

bool Gen6Basic::isServiceModeOn(bool mode)
{
    if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                        SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                        DIAG_GUARD_STATE_SERVICE))
    {
        if (mode)
        {
            setServiceModeTag(OFF);
        }

        return true;
    }

    return false;
}

bool Gen6Basic::waitPulse(DigitalIn *pin,
                          uint32_t   count,
                          bool       polarity,
                          float      time,
                          float      pulse_time,
                          bool       error_flag,
                          float      error)
{
    Timer timer;
    bool result = true;

    if (pin == NULL)
    {
        printf("FAIL Ожидание импульса: пин не определен\n");
    }

    timer.start();

    float    begin        = timer.read();
    float    end          = 0.0;
    float    pulse        = begin;
    float    curr_pulse   = 0.0;
    uint32_t buf_count    = count;

    printf("INFO Ожидание %d импульса(ов) (полярность: %d, время: %.2f)\n",
           count,
           polarity,
           time + error);

    while ((end < time + error) && (count > 0))
    {
        if (polarity ? *pin : !*pin)
        {
            pulse = timer.read();

            while (curr_pulse < pulse_time)
            {
                if (polarity ? !*pin : *pin)
                {
                    printf("INFO %d-й импулс получен через %.2f сек\n",
                           (buf_count - count) + 1,
                           end);

                    --count;

                    break;
                }

                curr_pulse = timer.read() - pulse;
            }
        }

        end = timer.read() - begin;
    }

    if (error_flag && (end > (time + error) || end < (time - error)))
    {
        result = false;
    }
    else if (count > 0)
    {
        printf("INFO Не получено %d импульсов\n", count);
        result = false;
    }

    timer.stop();

    return result;
}

void Gen6Basic::sendPulse(DigitalOut *pin,
                          uint32_t    count,
                          bool        polarity,
                          float       pulse_time_s,
                          float       interval_s)
{
    if (pin == NULL)
    {
        printf("INFO Отправка импульса: пин не определен\n");
    }

    *pin = !polarity;

    for (uint32_t i = 0; i < count; ++i)
    {
        *pin = polarity;
        wait(pulse_time_s);
        *pin = !polarity;

        if (i < count - 1)
            wait(interval_s);
    }
}

void Gen6Basic::resetTag()
{
    resetTagVcc();
    wait(TIME_DELAY::FOUR_S);
    setTagVcc();
    wait(TIME_DELAY::FIVE_S);
    resetTagVcc();
    wait(TIME_DELAY::FOUR_S);
}

int32_t Gen6Basic::getModules()
{
    return m_device_modules;
}

bool Gen6Basic::sendDtmf(const char *to,
                         const char *msg,
                         uint16_t    lenght)
{
    bool result = false;

    p_gsm->resetCall();
    p_gsm->call(to);
    wait(TIME_DELAY::DTMF_INTERVAL_TIME_S);

    for (uint8_t i = 0; i < 2; ++i)
    {
        if (p_gsm->sendDtmfTone(to, msg, lenght))
        {
            printf("INFO DTMF команда %s отправлена успешно\n", msg);
            result = true;
            wait(TIME_DELAY::THREE_S);
            p_gsm->resetCall(false);
            break;
        }
    }

    return result;
}

void Gen6Basic::bypassLearn(bool state)
{
    const uint8_t bypass_push_count = 14;

    if (state)
    {
        for (uint8_t i = 0; i < bypass_push_count; ++i)
        {
            clickServiceBtn();
            wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
        }
    }
    else
    {
        for (uint8_t i = 0; i < bypass_push_count; ++i)
        {
            clickServiceBtn();
            wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
        }

        wait(TIME_DELAY::FIVE_S);
    }
}

void Gen6Basic::addDevicePin(const string &pin)
{
    strcpy(m_device_pin, pin.c_str());
    printf("INFO Добавлен код экстренного снятия: %s\n", m_device_pin);
}

char *Gen6Basic::getDevicePin()
{
    return m_device_pin;
}

char *Gen6Basic::getWrongDevicePin(emergency_dis_pin_num_t pin_num)
{
    printf("INFO Получение неверного кода экстренного снятия\n");

    // Каждый раз копируем текущий PIN, т.к. m_wrong_device_pin уже мог меняться до этого
    strncpy(m_wrong_device_pin,
            m_device_pin,
            sizeof(m_wrong_device_pin));

    // Все цифры в коде неверные
    if (pin_num == EMERGENCY_DIS_PIN_NUM_ALL)
    {
        printf("INFO Получение неверного кода экстренного снятия: каждая цифра будет отлична от оригинальной\n");

        for (uint8_t i = 0; i < 4; ++i)
        {
            m_wrong_device_pin[i] -= 0x30;                           // Преобразовать символ цифры в цифру, согласно ASCII
            m_wrong_device_pin[i] = (m_wrong_device_pin[i] % 9) + 1; // Прибавить 1, избегая переполнения
            m_wrong_device_pin[i] += 0x30;                           // Преобразовать цифру в символ цифры, согласно ASCII
        }
    }
    // Одна цифра в коде неверная
    else
    {
        if (   pin_num < 1
            || pin_num > 4)
        {
            printf("INFO Неверный номер цифры кода экстренного снятия: %d\n", pin_num);

            pin_num = EMERGENCY_DIS_PIN_NUM_FIRST;
        }

        printf("INFO Получение неверного кода экстренного снятия: цифра №%d будет отлична от оригинальной\n", pin_num);

        m_wrong_device_pin[pin_num - 1] -= 0x30;                                     // Преобразовать символ цифры в цифру, согласно ASCII
        m_wrong_device_pin[pin_num - 1] = (m_wrong_device_pin[pin_num - 1] % 9) + 1; // Прибавить 1, избегая переполнения
        m_wrong_device_pin[pin_num - 1] += 0x30;                                     // Преобразовать цифру в символ цифры, согласно ASCII
    }

    printf("INFO Получен неверный код экстренного снятия: %s\n", m_wrong_device_pin);

    return m_wrong_device_pin;
}

bool Gen6Basic::checkProgramNeutral(int8_t model,
                                    float  timeout_s,
                                    float  max_error_s)
{
    int8_t pn_status = 0;
    float  begin_s   = 0.0;
    float  end_s     = begin_s;
    float  time_s    = 0.0;
    bool   result    = true;
    Timer  timer;


    timer.start();
    printf("INFO Время ожидания ПН: %.2f (погрешность: %.2f)\n",
           timeout_s,
           max_error_s);

    begin_s = timer.read();

    while (end_s - begin_s <= timeout_s + max_error_s)
    {
        pn_status = getProgramNeutral();
        end_s     = timer.read();

        wait(TIME_DELAY::STD_ERROR_TIME_S);

        if (pn_status == model)
        {
            break;
        }
    }

    time_s = end_s - begin_s;

    if (timeout_s == TIME_DELAY::NULL_DELAY_S)
    {
        time_s = TIME_DELAY::NULL_DELAY_S;
    }

    if (model != 0)
    {
        if (pn_status != 0) 
        {
            if (   (time_s < timeout_s - max_error_s)
                || (time_s > timeout_s + max_error_s))
            {
                result = false;

                printf("FAIL Программная нейтраль выполнена через %.2f\n", time_s);
            }
            else
            {
                printf("PASS Программная нейтраль выполнена через %.2f\n", time_s);
            }
        }
        else 
        {
            result = false;

            printf("FAIL Программная нейтраль не выполнена время ожидания %.2f\n", time_s);
        }            
    }
    else
    {
        if (pn_status == 0) 
        {
            printf("PASS Программная нейтраль сброшена\n");
        }
        else
        {
            result = false;

            printf("FAIL Программная нейтраль не сброшена\n");
        }
    }

    timer.stop();
    return result;
}

void Gen6Basic::checkSupportIgn(int8_t model)
{
    if (model != 0)
    {
        if (getSupportIgn() != 0) 
        {
            printf("PASS ПЗ включена\n"); 
        }
        else
        {
            printf("FAIL ПЗ выключена\n");   
        }
    }
    else
    {
        if (getSupportIgn() == 0) 
        {
            printf("PASS ПЗ выключена\n"); 
        }
        else
        {
            printf("FAIL ПЗ включена\n");   
        }
    }
}

void Gen6Basic::checkTurboTimer(int8_t model)
{
    if (model != 0)
    {
        if (getTurboTimer() != 0) 
        {
            printf("PASS ТТ включен\n"); 
        }
        else
        {
            printf("FAIL ТТ выключен\n");   
        }
    }
    else
    {
        if (getTurboTimer() == 0) 
        {
            printf("PASS ТТ выключен\n"); 
        }
        else
        {
            printf("FAIL ТТ включен\n");   
        }
    }

    wait(TIME_DELAY::ONE_S);
}

void Gen6Basic::checkAlarm(int8_t model, float timeout)
{
    if (model != 0)
    {
        if (getAlarmSignal(timeout) == CONST)
        {
            printf("PASS Тревога сработала\n");
        }
        else
        {
            printf("FAIL Тревога не сработала\n");
        }
    }
    else
    {
        if (getAlarmSignal(timeout) == NOSIGNAL)
        {
            printf("PASS Тревога не сработала\n");
        }
        else
        {
            printf("FAIL Тревога сработала\n");
        }
    }
}

const std::vector<can_sts_btn_t>& Gen6Basic::getValidatorCode()
{
    return p_validator->getCode();
}

const std::vector<can_sts_btn_t>& Gen6Basic::getWrongValidatorCode()
{
    return p_validator->getCode(VALIDATOR_CODE_WRONG);
}

void Gen6Basic::sendValidatorCode()
{
    p_validator->sendCode();
}

void Gen6Basic::sendWrongValidatorCode()
{
    p_validator->sendCode(VALIDATOR_CODE_WRONG);
}

bool Gen6Basic::registerValidatorCode()
{
    for (uint8_t i = 0; i < 2; ++i)
    {
        sendValidatorCode();
        p_service_btn->waitPulseAmount(COUNT_STRUCT::BTN_VALIDATOR_REG_LED_LIGHTS_COUNT,
                                       TIME_DELAY::BTN_VALIDATOR_REG_LIGHTS_WAIT_TIME_S);
        wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);
    }

    return true;
}

bool Gen6Basic::registerAll(registration_queue_mode_t reg_queue_mode)
{
    switch (m_device_name)
    {
        case GEN6_X96:
            getR6()->setVcc(OFF);
            wait(TIME_DELAY::ONE_S);

            if (!registrationMode(ON, reg_queue_mode))
            {
                return false;
            }

            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerValidatorCode();
            registerLedKeyring();
            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerLcdKeyring();
            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerTag();
            registerMobile();
            registerR6();
            wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S);
            setIgn(PIN, OFF);
            break;

        case GEN6_X96V2:
            p_r6eco->setVcc(OFF);
            wait(TIME_DELAY::VCC_R6_OFF_DELAY_S);
            p_r6eco->setCtrl(ON);
            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            getR6()->setVcc(OFF);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);
            p_r4->setCtrl(ON);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

            if (!registrationMode(ON, reg_queue_mode))
            {
                return false;
            }

            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            p_r4->setCtrl(OFF);
            p_r6eco->setVcc(ON);
            wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S);
            p_r6eco->setCtrl(OFF);
            registerValidatorCode();
            registerLedKeyring();
            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerLcdKeyring();
            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerTag();
            registerMobile();
            registerR6();
            wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S);
            break;

        case GEN6_A96:
        case GEN6_E96:
            if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
            {
                p_r6eco->setVcc(OFF);
                wait(TIME_DELAY::VCC_R6_OFF_DELAY_S);
                p_r6eco->setCtrl(ON);
                wait(TIME_DELAY::REG_DEVICE_DELAY_S);
                getR6()->setVcc(OFF);
            }

            p_r4->setCtrl(ON);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

            if (!registrationMode(ON, reg_queue_mode))
            {
                return false;
            }

            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            p_r4->setCtrl(OFF);

            if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
            {
                p_r6eco->setVcc(ON);
                wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S);
                p_r6eco->setCtrl(OFF);
            }

            registerValidatorCode();

            if ((m_device_modules & R868_MODULE) == R868_MODULE)
            {
                wait(TIME_DELAY::REG_DEVICE_DELAY_S);
                registerLedKeyring();
                wait(TIME_DELAY::REG_DEVICE_DELAY_S);
                registerLcdKeyring();
                wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            }

            if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
            {
                registerTag();
                registerMobile();
                registerR6();
                wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            }

            wait(TIME_DELAY::DELAY_AFTER_REGISTER_R6_S);
            break;

        case GEN6_M96:
            getR6()->setVcc(OFF);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

            if (!registrationMode(ON, reg_queue_mode))
            {
                return false;
            }

            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerValidatorCode();
            registerTag();
            registerR6();
            wait(TIME_DELAY::DELAY_AFTER_REGISTER_R6_S);
            break;

        case GEN6_M66:
            p_r6->setVcc(OFF);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

            if (!registrationMode(ON, reg_queue_mode))
            {
                return false;
            }

            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            registerTag();
            registerR6();
            wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S + TIME_DELAY::WAITING_REGION_STATE_S);
            resetTag();
            break;

        case GEN6_S96:
            p_r6eco->setVcc(OFF);
            wait(TIME_DELAY::VCC_R6_OFF_DELAY_S);
            p_r6eco->setCtrl(ON);
            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            getR6()->setVcc(OFF);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);
            p_r4->setCtrl(ON);
            wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

            if (!registrationMode(ON, reg_queue_mode))
            {
                return false;
            }

            wait(TIME_DELAY::REG_DEVICE_DELAY_S);
            p_r4->setCtrl(OFF);
            p_r6eco->setVcc(ON);
            wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S);
            p_r6eco->setCtrl(OFF);
            registerValidatorCode();
            registerTag();
            registerMobile();
            registerR6();
            wait(TIME_DELAY::MAX_DELAY_AFTER_REGISTER_R6_S);
            break;

        default:
            return false;
    }

    registrationMode(OFF, reg_queue_mode);

    return true;
}

bool Gen6Basic::loadFirmware(const char *firmware,
                             float       time,
                             bool        from_dropbox,
                             const char *file_path)
{
    const uint16_t c_max_command_size     = 1024;
    const uint8_t  c_max_dropbox_str_size =    9;
    const uint8_t  c_max_buf_size         =  255;

    char buf[c_max_buf_size];

    bool result = false;

    char firmware_path[c_max_command_size];
    char dropbox[c_max_dropbox_str_size];


    memset(buf,           0, c_max_buf_size);
    memset(firmware_path, 0, c_max_command_size);
    memset(dropbox,       0, c_max_dropbox_str_size);

    if (from_dropbox)
    {
        strcpy(dropbox, "DROPBOX ");
    }

    if (file_path != NULL)
    {
        strcpy(firmware_path, file_path);
    }
    else
    {
        strcpy(firmware_path, firmware);
    }

    printf("INFO Отправка команды на прошивку: %s\n", firmware);
    printf("SET FIRMWARE %s \n", firmware);

    if (UartInteract::getString(p_serial, buf, time))
    {
        if (strstr(buf, "FIRMWARE_LOAD_OK") != NULL)
        {
            printf("INFO Ответ об успешной прошивке получен\n");
            result = true;
        }
    }

    return result;
}

bool Gen6Basic::parseFirmwareLoadError(const char *buf)
{
    // Обработчик сообщений
    if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_OK) != NULL)
    {
        printf("INFO Ответ об успешной прошивке получен\n");
    }
    else
    {
        printf("INFO Ответ об успешной прошивке не получен\n");

        // Обработчик кодов ошибок
        if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_ERROR_SL_MASTER))
        {
            printf("FAIL Неудачное обновление прошивки через StarLine Мастер\n");
        }
        else if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_ERROR_NOT_FOUND))
        {
            printf("FAIL Запрашиваемая прошивка не найдена на сервере\n");
        }
        else if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_ERROR_ARG))
        {
            printf("FAIL Получены не все параметры для загрузки прошивки\n");
        }
        else
        {
            printf("FAIL Получен неизвестный ответ при загрузке прошивки: %s\n", buf);
        }

        return false;
    }

    return true;
}

bool Gen6Basic::loadFirmwareFromServer(fw_download_type_t  fw_download_type,
                                       float               timeout_s,
                                       const char         *special_fw_ver)
{
    const uint8_t buf_size = LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN;
    char          buf[buf_size];


    memset(buf,
           0,
           buf_size);

    printf("INFO Отправка команды на прошивку ПО устройства\n");

    switch (fw_download_type)
    {
        case FW_DOWNLOAD_TYPE_OLDEST:
            printf("DEBUG Отправка запроса на скачивание самой старой версии ПО на сервере\n");
            printf("LOAD FIRMWARE OLDEST\n");
            break;

        case FW_DOWNLOAD_TYPE_SECOND_PREV:
            printf("DEBUG Отправка запроса на скачивание второй предыдущей версии ПО\n");

            if (special_fw_ver != NULL)
            {
                printf("DEBUG Будет скачана вторая предыдущая версия ПО относительно заданной: %s\n", special_fw_ver);
                printf("LOAD FIRMWARE SECOND_PREV %s\n", special_fw_ver);
            }
            else
            {
                printf("DEBUG Будет скачана вторая предыдущая версия ПО относительно текущей\n");
                printf("LOAD FIRMWARE SECOND_PREV\n");
            }

            break;

        case FW_DOWNLOAD_TYPE_PREV:
            printf("DEBUG Отправка запроса на скачивание предыдущей версии ПО\n");

            if (special_fw_ver != NULL)
            {
                printf("DEBUG Будет скачана предыдущая версия ПО относительно заданной: %s\n", special_fw_ver);
                printf("LOAD FIRMWARE PREV %s\n", special_fw_ver);
            }
            else
            {
                printf("DEBUG Будет скачана предыдущая версия ПО относительно текущей\n");
                printf("LOAD FIRMWARE PREV\n");
            }

            break;

        case FW_DOWNLOAD_TYPE_LATEST_BETA:
            printf("DEBUG Отправка запроса на скачивание последней бета версии ПО на сервере\n");
            printf("LOAD FIRMWARE LAST BETA\n");
            break;

        case FW_DOWNLOAD_TYPE_LATEST_RELEASE:
            printf("DEBUG Отправка запроса на скачивание последней релизной версии ПО на сервере\n");
            printf("LOAD FIRMWARE LAST RELEASE\n");
            break;

        default:
            printf("INFO Неизвестный тип скачивания прошивки: %d\n", fw_download_type);
            return false;
    }

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        return parseFirmwareLoadError(buf);
    }

    return false;
}

fw_type_t Gen6Basic::checkHotFirmware(float timeout_s)
{
    const uint8_t buf_size = LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN;
    char  buf[buf_size];


    memset(buf,
           0,
           buf_size);

    printf("INFO Отправка команды для уточнения версии ПО\n");
    printf("CHECK HOT_FIRMWARE \n");

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        if (strstr(buf, "FIRMWARE_CHECK_OK") != NULL)
        {
            printf("INFO Ответ об успешной прошивке получен\n");
            return FW_TYPE_HOT;
        }
        else
        {
            return FW_TYPE_NOT_HOT;
        }
    }

    return FW_TYPE_UNKNOWN;
}

bool Gen6Basic::loadLastFirmware(float timeout_s)
{
    const uint8_t buf_size = LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN;
    char  buf[buf_size];


    memset(buf,
           0,
           buf_size);

    printf("INFO Отправка команды на прошивку ПО устройства\n");
    printf("SET CURR_FIRMWARE\n");

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        return parseFirmwareLoadError(buf);
    }

    return false;
}

bool Gen6Basic::loadFirmwareByVersion(const char *version, float timeout_s)
{
    const uint8_t buf_size = LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN;
    char  buf[buf_size];


    if (version == NULL)
    {
        printf("INFO Версия ПО не указана\n");
        return false;
    }

    memset(buf,
           0,
           buf_size);

    printf("INFO Отправка команды на прошивку ПО устройства версии: %s\n", version);
    printf("SET LOAD_FIRMWARE %s\n", version);

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        return parseFirmwareLoadError(buf);
    }

    return false;
}

bool Gen6Basic::getFirmwareVersion(char      *version,
                                   fw_type_t  type,
                                   float      time)
{
    bool result           = false;
    const char *delimiter = "_";   // Разделитель токенов
    char       *token;             // Указатель на начало токена
    char        buf[255];


    if (version == NULL)
    {
        printf("INFO Буфер для записи не инициализирован\n");
        return version;
    }

    memset(buf, 0, 255);
    printf("INFO Отправка команды на получение версии ПО устройства\n");

    switch (type)
    {
        case FW_TYPE_CURRENT:
            printf("GET CURR_FIRMWARE\n");
            break;

        case FW_TYPE_LATEST_BETA:
            printf("GET LAST_FIRMWARE BETA\n");
            break;

        case FW_TYPE_LATEST_RELEASE:
            printf("GET LAST_FIRMWARE RELEASE\n");
            break;
    }

    if (UartInteract::getString(p_serial,
                                buf,
                                time))
    {
        // Строка представляет собой "FIRMWARE_GET_%s_OK\n", где %s - версия (напр.: FIRMWARE_GET_2.14.0_OK)
        if (strstr(buf, "FIRMWARE_GET") != NULL)
        {
            token = strtok(buf,  delimiter); // Получить первый токен (FIRMWARE)
            token = strtok(NULL, delimiter); // Получить второй токен (GET)
            token = strtok(NULL, delimiter); // Получить третий токен (версия, напр.: 2.14.0)
            sprintf(version, "%s", token);   // Записать токен

            if (version != NULL)
            {
                printf("INFO Получена версия прошивки: %s\n", version);
                result = true;
            }
        }
    }

    return result;
}

bool Gen6Basic::resetFirmware(float time)
{
    Timer timer;
    bool  result = false;

    const uint16_t c_max_command_size = 1024;

    char command[c_max_command_size];

    memset(command, 0, c_max_command_size);

    printf("INFO Отправка команды на переключение версии ПО\n");
    printf("SET RELOAD_FIRMWARE \n");

    timer.start();
    float begin = timer.read();
    float end   = begin;

    while (end < time)
    {
        if (p_serial->readable() && p_serial->gets(command, c_max_command_size - 1))
        {
            printf("TRACE Получена строка: %s\n", command);
        }

        if (strstr(command, "FIRMWARE_RELOAD_OK") != NULL)
        {
            printf("INFO Ответ об успешном переключении версии ПО получен\n");
            result = true;
            break;
        }
        else if (strstr(command, "FIRMWARE_RELOAD_ERROR") != NULL)
        {
            break;
        }

        end = timer.read() - begin;
    }

    printf("DEBUG Ожидание ответа: %.2f\n", end);

    timer.stop();

    return result;
}

bool Gen6Basic::loadCanFirmware(const char *can_car_num_str, float timeout_s)
{
    const char    *cmd_can_update = "LOAD CAN"; // Команда на обновление индивидуальной прошивки CAN
    const uint8_t  buf_size       = 255;
    char           buf[buf_size];


    memset(buf,
           0,
           buf_size);

    printf("INFO Отправка команды на обновление индивидуальной прошивки CAN (автомобиль: %s)\n", can_car_num_str);

    // Отправить команду на обновление индивидуальной прошивки CAN, пример команды: LOAD CAN 9999
    printf("%s %s \n",
           cmd_can_update,
           can_car_num_str);

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        if (strstr(buf, "CAN_LOAD_OK") != NULL)
        {
            printf("INFO Обновление индивидуальной прошивки CAN успешно\n");

            return true;
        }
    }

    printf("INFO Обновление индивидуальной прошивки CAN неуспешно\n");

    return false;
}

void Gen6Basic::analogSlave(slave_lights_t lights,
                            bool           state,
                            float          lock_pulse_time_s,
                            float          light_pulse_time_s,
                            float          interval_s)
{
    float pulse_interval_time = TIME_DELAY::NULL_DELAY_S;
    float max_pulse_interval  = TIME_DELAY::NULL_DELAY_S;

    bool lock_close_polarity = true;
    bool lock_open_polarity  = true;
    bool light_l_polarity    = true;
    bool light_r_polarity    = true;
    bool lights_polarity     = true;

    /*if (m_device_name == GEN6_A96)
    {
        lock_close_polarity = false;
        lock_open_polarity  = false;
        light_l_polarity    = false;
        light_r_polarity    = false;
        lights_polarity     = false;
    }*/

    /*if (p_out_central_lock_close != NULL)
        *p_out_central_lock_close = !lock_close_polarity;

    if (p_out_central_lock_open != NULL)
        *p_out_central_lock_open = !lock_open_polarity;

    if (p_out_light_left != NULL)
        *p_out_light_left = !light_l_polarity;

    if (p_out_light_right != NULL)
        *p_out_light_right = !light_r_polarity;

    if (p_out_lights != NULL)
        *p_out_lights = !lights_polarity;*/

    if (lights == SLAVE_LIGHT_LR && (p_out_light_left == NULL || p_out_light_right == NULL))
    {
        lights = SLAVE_LIGHTS;
    }

    if (lights == SLAVE_LIGHT_LR)
    {
        max_pulse_interval = interval_s - (lock_pulse_time_s + light_pulse_time_s * 2);
    }
    else
    {
        max_pulse_interval = interval_s - (lock_pulse_time_s + light_pulse_time_s);
    }
    printf("DEBUG Максимальное суммарное время между импульсами: %.2f\n", max_pulse_interval);

    pulse_interval_time = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / max_pulse_interval);
    //printf("DEBUG pulse_interval_time %.2f\n", pulse_interval_time);
    wait(pulse_interval_time);

    if (state)
    {
        sendPulse(p_out_central_lock_close, 1, lock_close_polarity, lock_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
    }
    else
    {
        sendPulse(p_out_central_lock_open,  1, lock_open_polarity,  lock_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
    }

    pulse_interval_time = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (max_pulse_interval - pulse_interval_time));
    //printf("DEBUG pulse_interval_time2 %.2f\n", pulse_interval_time);

    switch (lights)
    {
        case SLAVE_LIGHT_L:
            sendPulse(p_out_light_left,  1, light_l_polarity, light_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
            break;

        case SLAVE_LIGHT_R:
            sendPulse(p_out_light_right, 1, light_r_polarity, light_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
            break;

        case SLAVE_LIGHTS:
            sendPulse(p_out_lights,      1, lights_polarity,  light_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
            break;

        case SLAVE_LIGHT_LR:
            sendPulse(p_out_light_left,  1, light_l_polarity, light_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
            wait(pulse_interval_time);
            sendPulse(p_out_light_right, 1, light_r_polarity, light_pulse_time_s, TIME_DELAY::NULL_DELAY_S);
            break;

        default:
            break;
    }
}

bool Gen6Basic::checkShield(float time)
{
    return p_gsm->checkShield(time);
}

buzzer_status_t Gen6Basic::waitBuzzerStatus(bool status, float time)
{
    if (m_device_name != GEN6_A96)
    {
        return BUZZER_NONE;
    }

    Timer timer;
    timer.start();

    buzzer_status_t result = BUZZER_OFF;
    float           begin  = timer.read();

    if (!status)
    {
        result = BUZZER_ON;
    }

    while (timer.read() - begin < time)
    {
        if (status && p_slp->getSlpStatus(DIAG_DEBUG_BUZZER,
                                          SLP_STATUSES_SIZES::DIAG_DEBUG_BUZZER_SIZE,
                                          ON))
        {
            result = BUZZER_ON;
            break;
        }
        else if (!status && !p_slp->getSlpStatus(DIAG_DEBUG_BUZZER,
                                                 SLP_STATUSES_SIZES::DIAG_DEBUG_BUZZER_SIZE,
                                                 ON))
        {
            result = BUZZER_OFF;
            break;
        }
    }

    timer.stop();

    return result;
}

void Gen6Basic::setLedKeyringVcc(bool state)
{
    if ((m_device_modules & R868_MODULE) == R868_MODULE)
    {
        if (p_led_keyring == NULL)
        {
            printf("FAIL Управление питанием доп. брелка: указатель на брелок не определен\n");
            return;
        }

        if (state)
        {
            p_led_keyring->onVcc();
        }
        else
        {
            p_led_keyring->offVcc();
        }
    }
}

void Gen6Basic::setLcdKeyringVcc(bool state, float delay_s)
{
    if ((m_device_modules & R868_MODULE) == R868_MODULE)
    {
        if (p_lcd_keyring == NULL)
        {
            printf("FAIL Управление питанием ЖК брелка: указатель на брелок не определен\n");
            return;
        }

        if (state)
        {
            p_lcd_keyring->onVcc();
        }
        else
        {
            p_lcd_keyring->offVcc();
        }
    }

    wait(delay_s);
}

command_type_t Gen6Basic::getExecutionMethod(command_type_t  execution_method,
                                             action_type_t   action_type,
                                             DigitalOut     *pin)
{
    command_type_t result = execution_method;
    
    switch (execution_method)
    {
        case KEYRING:

            if (m_device_name == GEN6_M96)
            {
                if (action_type != CAR_ACTION && action_type != RUN_ACTION)
                {
                    result = TAG;
                }
                else
                {
                    result = SMS;
                }
            } 
            else if (m_device_name == GEN6_M66)
            {
                if (action_type != CAR_ACTION && action_type != RUN_ACTION)
                {
                    result = TAG_M66;
                }
                else
                {
                    result = ERROR_TYPE;
                }
            }
            else if (m_device_name == GEN6_A96 || m_device_name == GEN6_E96)
            {
                if (   (m_device_modules & R868_MODULE) == R868_MODULE
                    && action_type != CAR_ACTION)
                {   
                    execution_method = KEYRING;
                }
                else if (   ((m_device_modules & BLE_MODULE) == BLE_MODULE)
                         && action_type != RUN_ACTION
                         && action_type != CAR_ACTION)
                {   
                    result = TAG;
                }
                else if (   (m_device_modules & GSM_MODULE) == GSM_MODULE
                         && action_type != CAR_ACTION)
                {
                    result = SMS;
                }
            }
            else if (m_device_name == GEN6_S96)
            {
                if (action_type == USER_ACTION)
                {
                    result = TAG;
                }
                else if (action_type == RUN_ACTION)
                {
                    result = SMS;
                }
                else if (action_type == SLO_ACTION)
                {
                    result = SLO;
                }
            }
            else 
            {
                result = KEYRING;
            }

            break;
            
        case TAG:
        case TAG_M66:

            if (m_device_name == GEN6_M96)
            {
                if (action_type != CAR_ACTION && action_type != RUN_ACTION)
                {
                    result = TAG;
                }
                else
                {
                    result = SMS;
                }
            } 
            else if (m_device_name == GEN6_M66)
            {
                if (action_type != CAR_ACTION && action_type != RUN_ACTION)
                {
                    result = TAG_M66;
                }
                else
                {
                    result = ERROR_TYPE;
                }
            }
            else if (m_device_name == GEN6_A96 || m_device_name == GEN6_E96)
            {
                if (((m_device_modules & BLE_MODULE) == BLE_MODULE) && action_type != RUN_ACTION)
                {   
                    if (action_type != CAR_ACTION)
                    {
                        result = TAG;
                    }
                }
                else if ((m_device_modules & R868_MODULE) == R868_MODULE)
                {   
                    if (action_type != CAR_ACTION)
                    {
                        result = KEYRING;
                    }
                }
                else if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
                {
                    if (action_type != CAR_ACTION)
                    {
                        result = SMS;
                    }
                }
            }
            else
            {
                result = TAG;
            }

            break;

        case MOBILE:

            if (action_type != CAR_ACTION)
            {
                if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
                {
                    result = MOBILE;
                }
                else if ((m_device_modules & R868_MODULE) == R868_MODULE)
                {
                    result = KEYRING;
                }
                else if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
                {
                    result = SMS;
                }
            }

            break;

        case SMS:

            if (m_device_name == GEN6_M96)
            {
                if (action_type != CAR_ACTION && action_type != RUN_ACTION)
                {
                    result = TAG;
                }
                else
                {
                    result = SMS;
                }
            } 
            else if (m_device_name == GEN6_M66)
            {
                if (action_type != CAR_ACTION && action_type != RUN_ACTION)
                {
                    result = TAG_M66;
                }
                else
                {
                    result = ERROR_TYPE;
                }
            }
            else if (m_device_name == GEN6_A96 || m_device_name == GEN6_E96)
            {
                if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
                {
                    if (action_type != CAR_ACTION)
                    {
                        result = SMS;
                    }
                }
                else if ((m_device_modules & R868_MODULE) == R868_MODULE)
                {   
                    if (action_type != CAR_ACTION)
                    {
                        result = KEYRING;
                    }
                }
                else if (((m_device_modules & BLE_MODULE) == BLE_MODULE) && action_type != RUN_ACTION)
                {   
                    if (action_type != CAR_ACTION)
                    {
                        result = TAG;
                    }
                }
            }
            else 
            {
                result = SMS;
            }

            break;
            
        case PIN:

            if (action_type == CAR_ACTION)
            {
                if (pin == NULL)
                {
                    printf("INFO Пин не назначен. Управление будет произведено по CAN\n");
                    result = CAN_USE;
                }
                else
                {
                    result = PIN;
                }
            }
            else
            {
                result = ERROR_TYPE;
            }

            break;

        default:
            break;
    }

    return result;
}

void Gen6Basic::setSpeed(int8_t value)
{
    printf("INFO Установка скорости \n");
    p_can_actions->sendSpeed(value);
}

bool Gen6Basic::checkSpeed(int8_t speed_kmph, float timeout_s)
{
    uint16_t speed;

    speed = (uint16_t)speed_kmph; // По документации региона, размер DIAG_DATA_SPEED 16 бит

    // По документации региона, единицы измерения км/ч / 100
    // Но при выключенном зажигании, единицы измерения будут км/ч(в документации не указано)
    if (getIgn(PIN))
    {
        speed *= 100;
    }

    return waitSlpCmdStatus(DIAG_DATA_SPEED,
                            SLP_STATUSES_SIZES::DIAG_DATA_SPEED_SIZE,
                            speed,
                            ON,
                            timeout_s);
}

ahj_status_t Gen6Basic::getAntiHijacking()
{
    ahj_status_t result = AHJ_STATUS_OFF;
    
    if (getSlpCmdStatus(DIAG_DATA_FLAG,
                        SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                        DIAG_FLAG_BIT_ANTI_HIJACK))
    {
        if (getSlpCmdStatus(DIAG_DATA_FLAG,
                            SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                            DIAG_FLAG_BIT_ANTI_HIJACK_TAG))
        {
            result = AHJ_STATUS_TAG;
        }
        else
        {
            result = AHJ_STATUS_CMD;
        }
    }

    return result;
}

bool Gen6Basic::waitAntiHijacking(ahj_status_t status, float timeout)
{
    Timer timer;
    bool result = false;

    timer.start();
    float begin = timer.read();

    while (timer.read() - begin < timeout)
    {
        if (getAntiHijacking() == status)
        {
            printf("INFO Получен ожидаемый статус Антиограбления\n");
            result = true;
            return result;
        }
    }

    if (getAntiHijacking() == AHJ_STATUS_OFF)
    {
        printf("INFO Антиограбление выключено (Таймаут)\n");
    }
    else
    {
        if (getAntiHijacking() != status)
        {
            printf("INFO Не получен ожидаемый статус Антиограбления\n");
        }
    }

    timer.stop();

    return result;
}

bool Gen6Basic::waitLightAndAlarm(float time)
{
    Timer timer;
    bool result = false;

    timer.start();
    float begin = timer.read();

    while (timer.read() - begin < time)
    {
        if (getLights())
        {
            printf("INFO Световые сигналы получены\n");
            break;
        }
    }
        
    while (timer.read() - begin < time)
    {
        if (getAlarm())
        {
            printf("INFO Звуковой сигнал получен\n");
            result = true;
            break;
        }
    }

    timer.stop();

    return result;
}

void Gen6Basic::offAntiHijacking(command_type_t execution_method,
                                 bool           confirmation)
{ 
    printf("INFO Выключение режима антиограбления\n");

    switch (execution_method)
    {
        case TAG:
            p_tag->resetVcc(); 
            break;

        case EMERGENCY:
            emergencyDisarming();
            break;

        case SMS:
            if (setAnj(false, m_device_pin))
            {
                printf("PASS Команда выхода из режима антиограбления принята\n");
            }
            else
            {
                printf("FAIL Команда выхода из режима антиограбления не принята\n");
            }
            break;
                                    
        case SLO:
            setStarlineOnlineParam(StarlineOnline::HIJACK, OFF, m_device_pin);
            break;
                
        default:
            printf("FAIL Неизвестный метод включения антиограбления\n");
            return;
    }
}

void Gen6Basic::onAntiHijacking(command_type_t  execution_method,
                                bool            confirmation,
                                const char     *gsm_pass,
                                bool            ahj_check)
{
    printf("INFO Включение режима антиограбления\n");
        
    switch (execution_method)
    {
        case KEYRING:
            printf("INFO Включение режима антиограбления по брелоку\n");
            setLcdKeyringVcc(OFF);
            setIgn(PIN, ON);
            setDoor(OPEN, PIN);
            setMoveSensor();
            printf("INFO Алгоритм антиограбление по брелоку выполнен\n");
            break;

        case TAG:
            printf("INFO Включение режима антиограбления по метке\n");
            setIgn(PIN, ON);
            setTagVcc();
            wait(TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);
            p_tag->resetVcc();
            setDoor(OPEN, PIN);

            if (waitSlpCmdStatus(DIAG_DATA_FLAG,
                                 SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                 DIAG_FLAG_BIT_TAG,
                                 OFF,
                                 TIME_DELAY::MAX_TAG_VISIBLE_TIME_S))
            {
                setMoveSensor();
                printf("INFO Алгоритм антиограбление по метке выполнен\n");
            }
            else
            {
                printf("INFO Метка в зоне видимости\n");
            }

            break;

        case SMS:
            printf("INFO Включение режима антиограбления по СМС\n");

            if (setAnj(true, m_device_pin))
            {
                printf("PASS Команда входа в режим антиограбления принята\n");
            }
            else
            {
                printf("FAIL Команда входа в режим антиограбления не принята\n");
            }
            break;

        case DTMF:
            printf("INFO Включение режима антиограбления по DTMF\n");

            p_gsm->resetCall();

            if (p_gsm->call(m_gen6_phones[DEVICE_PHONE_NO_1], 61000))
            {
                wait(TIME_DELAY::DTMF_INTERVAL_TIME_S);

                for (uint8_t i = 0; i < 2; ++i)
                {
                    for (uint8_t j = 0; j < (confirmation ? 2 : 1); ++j)
                    {
                        if (   (j == 0)
                            && (gsm_pass != NULL)
                            &&  p_gsm->sendDtmfTone(m_gen6_phones[DEVICE_PHONE_NO_1], gsm_pass, strlen(gsm_pass)))
                        {
                            printf("INFO DTMF команда %s отправлена успешно\n", gsm_pass);
                            wait(TIME_DELAY::FIVE_S);
                        }

                        if (p_gsm->sendDtmfTone(m_gen6_phones[DEVICE_PHONE_NO_1], "31", 2))
                        {
                            printf("INFO DTMF команда 31 отправлена успешно\n");

                            if (j < 1)
                            {
                                wait(25.0f);
                            }
                        }
                    }

                    if (ahj_check)
                    {
                        wait(TIME_DELAY::FOUR_S);
    
                        if (getSlpCmdStatus(DIAG_DATA_FLAG,
                                            SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                            DIAG_FLAG_BIT_ANTI_HIJACK))
                        {
                            break;
                        }
                    }
                }

                p_gsm->resetCall(false);
            }
            break;

        case SLO:
            printf("INFO Включение режима антиограбления по через сервер\n");

            setStarlineOnlineParam(StarlineOnline::HIJACK, ON, m_device_pin);
            break;

        case MOBILE:
            printf("INFO Включение режима антиограбления через мобильное приложение (BLE)\n");
            p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_ANTIROBBERY_ON);
            break;

        default:
            printf("FAIL Неизвестный метод включения антиограбления\n");
            return;
    }
}

void Gen6Basic::setCapSensor(bool state)
{
    if (p_out_cap_sensor == NULL)
    {
        printf("DEBUG Сенсор ручки двери не определен\n");
        return;
    }

    char msg[6];

    memset(msg, 0, 6);

    if (state)
    {
        strcpy(msg, "вкл.");
    }
    else
    {
        strcpy(msg, "выкл.");
    }

    printf("DEBUG Управление сенсором ручки двери: %s\n", msg);

    *p_out_cap_sensor = state;
}

void Gen6Basic::setCapSensor(cap_sensor_type_t cap_sensor_type, bool state)
{
    DigitalOut        *sensor_pin;                             // Указатель на пин, отвечающий за активируемый датчик
    std::string        sensor_type_str;                        // Текстовое представление типа активируемого датчика
    const std::string  state_str = state == ON ? "Включение"   // Текстовое представление применяемого состояния (вкл/выкл)
                                               : "Выключение";


    switch (cap_sensor_type)
    {
        case CAP_SENSOR_TYPE_DEFAULT:
            sensor_type_str = "ручки двери";
            sensor_pin      = p_out_cap_sensor;
            break;

        case CAP_SENSOR_TYPE_ARM:
            sensor_type_str = "ручки двери (постановка)";
            sensor_pin      = p_out_cap_sensor_arm;
            break;

        case CAP_SENSOR_TYPE_DISARM:
            sensor_type_str = "ручки двери (снятие)";
            sensor_pin      = p_out_cap_sensor_disarm;
            break;

        case CAP_SENSOR_TYPE_TRUNK:
            sensor_type_str = "ручки багажника";
            sensor_pin      = p_out_cap_sensor_trunk;
            break;

        default:
            printf("FAIL Неизвестный тип датчика: %d\n", cap_sensor_type);
            return;
    }

    if (sensor_pin != NULL)
    {
        printf("INFO %s датчика %s\n",
               state_str.c_str(),
               sensor_type_str.c_str());

        *sensor_pin = state;
    }
}

void Gen6Basic::sendCapSensorPulse(bool polarity, float duration_s)
{
    printf("TRACE Отправка импульса сенсора ручки двери\n");
    sendPulse(p_out_cap_sensor,
              1,
              polarity,
              duration_s,
              TIME_DELAY::NULL_DELAY_S);
}

void Gen6Basic::pushCapSensor(cap_sensor_type_t cap_sensor_type,
                              int8_t            count,
                              float             press_time_s)
{
    printf("INFO Нажатие датчика ручки %d раз, длительность каждого нажатия: %.2f сек.\n",
           count,
           press_time_s);

    for (int8_t i = 0; i < count; ++i)
    {
        setCapSensor(cap_sensor_type, ON);
        wait(press_time_s);
        setCapSensor(cap_sensor_type, OFF);

        wait(TIME_DELAY::STD_WIRE_EN_DELAY_S); // Пауза между нажатиями
    }
}

bool Gen6Basic::waitCapSensor(cap_sensor_type_t cap_sensor_type,
                              bool              state,
                              float             timeout_s)
{
    const std::string            state_str = state ? "включен"   // Текстовое представление ожидаемого состояния датчика
                                                   : "выключен";

    std::string                  sensor_type_str;                // Текстовое представление типа проверяемого датчика
    slp_diag_data_zone_sysdata_t diag_data_zone;                 // Проверяемая зона


    switch (cap_sensor_type)
    {
        case CAP_SENSOR_TYPE_DEFAULT:
            sensor_type_str = "ручки двери";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR;
            break;

        case CAP_SENSOR_TYPE_ARM:
            sensor_type_str = "ручки двери (постановка)";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR_ARM;
            break;

        case CAP_SENSOR_TYPE_DISARM:
            sensor_type_str = "ручки двери (снятие)";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR_DISARM;
            break;

        case CAP_SENSOR_TYPE_TRUNK:
            sensor_type_str = "ручки багажника";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR_TRUNK;
            break;

        default:
            printf("FAIL Неизвестный тип датчика: %d\n", cap_sensor_type);
            return false;
    }

    printf("INFO Проверка состояния датчика %s, ожидаемое состояние: %s\n",
           sensor_type_str.c_str(),
           state_str.c_str());

    if (waitSlpCmdStatus(DIAG_DATA_ZONE_SYSDATA,
                         SLP_STATUSES_SIZES::DIAG_DATA_ZONE_SYSDATA_SIZE,
                         diag_data_zone,
                         state,
                         timeout_s))
    {
        printf("INFO Состояние соотвестствует ожидаемому\n");
    }
    else
    {
        printf("INFO Состояние не соотвестствует ожидаемому\n");
        return false;
    }

    return true;
}

void Gen6Basic::setArmBeachMode(bool  polarity,
                                float p1_duration_s,
                                float p2_duration_s,
                                float p3_duration_s)
{
    printf("INFO Запущена постановка в охрану сервисной кнопкой (Пляж)\n");

    for (uint8_t i = 0; i < 3; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    setIgn(PIN, ON,  TIME_DELAY::THREE_S);
    setIgn(PIN, OFF, TIME_DELAY::THREE_S);

    sendCapSensorPulse(polarity, p1_duration_s);
    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
    sendCapSensorPulse(polarity, p2_duration_s);
    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
    sendCapSensorPulse(polarity, p3_duration_s);
    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
}

void Gen6Basic::setDisarmBeachMode(bool        polarity,
                                   float       duration_s,
                                   const char *pin,
                                   uint8_t     pin_num_count,
                                   bool        wait_last_signal_flag)
{
    uint8_t pin_digit[4] = { 0, 0, 0, 0 };

    printf("INFO Запущено снятие с охраны (Пляж)\n");

    getPinFromChar(pin, pin_digit);

    if (pin_num_count < 1)
    {
        return;
    }

    for (uint8_t i = 0; i < pin_digit[0]; ++i)
    {
        sendCapSensorPulse(polarity, duration_s);
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    p_service_btn->waitPulseAmount(1, TIME_DELAY::AFTER_FIRST_PIN_NUM_SBTN_S);

    if (pin_num_count < 2)
    {
        return;
    }

    for (uint8_t i = 0; i < pin_digit[1]; ++i)
    {
        sendCapSensorPulse(polarity, duration_s);
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    p_service_btn->waitPulseAmount(2, TIME_DELAY::AFTER_SECOND_PIN_NUM_SBTN_S);

    if (pin_num_count < 3)
    {
        return;
    }

    for (uint8_t i = 0; i < pin_digit[2]; ++i)
    {
        sendCapSensorPulse(polarity, duration_s);
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO Ожидание\n");

    p_service_btn->waitPulseAmount(3, TIME_DELAY::AFTER_THIRD_PIN_NUM_SBTN_S);

    if (pin_num_count < 4)
    {
        return;
    }

    for (uint8_t i = 0; i < pin_digit[3]; ++i)
    {
        sendCapSensorPulse(polarity, duration_s);
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    if (wait_last_signal_flag)
    {
        printf("INFO Ожидание\n");

        p_service_btn->waitPulseAmount(4, TIME_DELAY::AFTER_FOURTH_PIN_NUM_SBTN_S);

        for (uint8_t i = pin_digit[3]; i < 9; ++i)
        {
            wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
        }
    }
}

bool Gen6Basic::getPinFromChar(const char *pin, uint8_t *pin_arr)
{
    char pin_arr_ch[5] = { 0, 0, 0, 0, 0 };

    uint32_t pin_digit = 0;

    if (   pin         == NULL
        || strlen(pin) != 4)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL Код экстренного снятия не задан\n");
            return false;
        }

        pin = m_device_pin;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (isdigit(pin[i]))
        {
            pin_arr_ch[i] = pin[i];
        }
    }

    pin_digit = atoi(pin_arr_ch);

    for (int8_t i = 3; i >= 0; --i)
    {
        pin_arr[i] = pin_digit % 10;
        pin_digit /= 10;
    }

    printf("INFO Код экстренного снятия:");

    for (uint8_t i = 0; i < 4; ++i)
    {
        printf(" %d", pin_arr[i]);
    }

    printf("\n");

    return true;
}

void Gen6Basic::sendSlaveLightsPulses(bool           polarity, 
                                      float          delay_light_r, 
                                      float          delay_light_l, 
                                      slave_lights_t lights)
{
    if (   (delay_light_r > TIME_DELAY::NULL_DELAY_S)
        && (delay_light_l > TIME_DELAY::NULL_DELAY_S))
    {
        switch (lights)
        {
            case SLAVE_LIGHT_NO:
                break;
            
            case SLAVE_LIGHT_L:
                if (p_out_light_left != NULL)
                {
                    printf("INFO Подача импульса на Левый поворотник. Длительность %.2f сек\n", delay_light_l);
                    *p_out_light_left = polarity;
                    wait(delay_light_l);
                    *p_out_light_left = !polarity;
                }

                break;
                
            case SLAVE_LIGHT_R:
                if (p_out_light_right != NULL)
                {
                    printf("INFO Подача импульса на Правый поворотник. Длительность %.2f сек\n", delay_light_r);
                    *p_out_light_right = polarity;
                    wait(delay_light_r);
                    *p_out_light_right = !polarity;
                }

                break;
              
            case SLAVE_LIGHTS:
                if (p_out_lights != NULL)
                {
                    printf("INFO Подача импульса на Общий вход поворотников. Длительность %.2f сек\n", delay_light_r);
                    *p_out_lights = polarity;
                    wait(delay_light_r);
                    *p_out_lights = !polarity;
                }

                break;
                
            case SLAVE_LIGHT_LR:
                if (p_out_light_left != NULL || p_out_light_right != NULL)
                {
                    if (delay_light_r == delay_light_l)
                    {
                        printf("INFO Подача импульса на Левый и Правый поворотники. Длительность %.2f сек\n", delay_light_r);
                        *p_out_light_right = polarity;
                        *p_out_light_left  = polarity;
                        wait(delay_light_r);
                        *p_out_light_right = !polarity;
                        *p_out_light_left  = !polarity;
                    } 
                    else if (delay_light_r > delay_light_l)
                    {
                        printf("INFO Подача импульса на Левый и Правый поворотники. Длительность правого=%.2f сек, левого=%.2f сек\n", delay_light_r, delay_light_l);
                        *p_out_light_right = polarity;
                        *p_out_light_left  = polarity;
                        wait(delay_light_l);
                        *p_out_light_right = !polarity;
                        wait(delay_light_r - delay_light_l);
                        *p_out_light_left  = !polarity;
                    }
                    else if (delay_light_r < delay_light_l)
                    {
                        printf("INFO Подача импульса на Левый и Правый поворотники. Длительность правого=%.2f сек, левого=%.2f сек\n", delay_light_r, delay_light_l);
                        *p_out_light_right = polarity;
                        *p_out_light_left  = polarity;
                        wait(delay_light_r);
                        *p_out_light_right = !polarity;
                        wait(delay_light_l - delay_light_r);
                        *p_out_light_left  = !polarity;
                    }
                }

                break;
        }
    }
}

void Gen6Basic::doSlaveAct(float           delay_between, 
                           slave_actions_t need_open)
{
    printf("INFO Задержка между ЦЗ и поворотниками. Длительность %.2f сек\n",
           delay_between);

    switch (need_open)
    {
        case SLAVE_ACT_OPEN_DOOR:
            setDoor(OPEN, PIN);

            if (delay_between > TIME_DELAY::STD_WIRE_EN_DELAY_S)
            {
                wait(delay_between - TIME_DELAY::STD_WIRE_EN_DELAY_S);
            }

            break;

        case SLAVE_ACT_OPEN_TRUNK:
            setTrunk(OPEN, PIN);

            if (delay_between > TIME_DELAY::STD_WIRE_EN_DELAY_S)
            {
                wait(delay_between - TIME_DELAY::STD_WIRE_EN_DELAY_S);
            }

            break;

        case SLAVE_ACT_NONE:
        default:
            wait(delay_between);
    }
}

void Gen6Basic::sendAnalogSlaveStatus(int8_t          value, 
                                      float           delay_clock, 
                                      float           delay_between, 
                                      float           delay_light_r, 
                                      float           delay_light_l, 
                                      slave_lights_t  lights, 
                                      slave_actions_t need_open,
                                      bool            light_before_clock)
{
    const char* message = value ? "INFO Подача команды на включение охраны через аналоговый Slave\n"
                                : "INFO Подача команды на выключение охраны через аналоговый Slave\n";

    printf("%s", message);

    bool polarity = true;

    if (   (p_out_light_left  == NULL)
        || (p_out_light_right == NULL))
    {
        lights = SLAVE_LIGHTS;
    }

    if (light_before_clock)
    {
        sendSlaveLightsPulses(polarity,
                              delay_light_r,
                              delay_light_l,
                              lights);

        printf("INFO Задержка между ЦЗ и поворотниками. Длительность %.2f сек\n",
               delay_between);
        wait(delay_between);
    }
    
    if (p_out_central_lock_open != NULL || p_out_central_lock_close != NULL)
    {
        if (value == ON) 
        {
            printf("INFO Подача импульса на ЦЗ Закрыть. Длительность %.2f сек\n",
                   delay_clock);
            *p_out_central_lock_close = polarity;
            wait(delay_clock);
            *p_out_central_lock_close = !polarity;
        }
        else
        {
            printf("INFO Подача импульса на ЦЗ Открыть. Длительность %.2f сек\n",
                   delay_clock);
            *p_out_central_lock_open = polarity;
            wait(delay_clock);
            *p_out_central_lock_open = !polarity;
        }
    }
    else
    {
        printf("INFO Открытие ЦЗ: контакт не настроен или Закрытие ЦЗ: контакт не настроен\n");
    }
  
    if (!light_before_clock)
    {
        doSlaveAct(delay_between, need_open);

        sendSlaveLightsPulses(polarity,
                              delay_light_r,
                              delay_light_l,
                              lights);
    }
}

bool Gen6Basic::waitClockAndSignal(arm_disarm_indication_t indication,
                                   float                   time_s,
                                   float                   time_error_s,
                                   bool                    lock_indication)
{
    Timer timer;
    Timer check_light_width_timer; // для фильтрации шума

    uint8_t signal_no    = 0,
            light_no     = 0,
            signal_count = 2,
            light_count  = 2;

    bool    is_signal    = false,
            is_light     = false,
            is_clock_set = false,
            is_clock     = false,
            result       = false;

    switch (indication)
    {
        case DISARM_INDICATION:
            signal_count = 2;
            light_count  = 2;
            break;

        case HF_DISARM_INDICATION:
            signal_count = 3;
            light_count  = 2;
            break;

        case CAP_SENSOR_INDICATION:
        case ARM_INDICATION:
            signal_count = 1;
            light_count  = 1;
            break;

        case HF_ARM_INDICATION:
            signal_count = 2;
            light_count  = 1;
            break;

        case BROKEN_ARM_INDICATION:
            signal_count = 4;
            light_count  = 4;
            break;

        case HF_BROKEN_ARM_INDICATION:
            signal_count = 5;
            light_count  = 4;
            break;

        default:
            printf("INFO Тип индикации после снятия/постановки охраны не определен\n");
            return result;
    }

    printf("INFO Ожидание импульсов звуковых, световых сигналов и ЦЗ\n");
    timer.start();

    float begin      = timer.read();
    float curr_time  = begin;
    float time_error = TIME_DELAY::NULL_DELAY_S;

    if (time_error_s > TIME_DELAY::NULL_DELAY_S)
    {
        time_error = time_error_s;
    }

    while (curr_time < time_s + time_error)
    {
        if (lock_indication)
        {
            if (   (   (indication == DISARM_INDICATION)
                    || (indication == HF_DISARM_INDICATION)
                    || (indication == CAP_SENSOR_INDICATION))
                && !is_clock
                && getLockOpen())
            {
                is_clock_set = true;
            }
            else if (   (   (indication == ARM_INDICATION)
                         || (indication == HF_ARM_INDICATION)
                         || (indication == BROKEN_ARM_INDICATION)
                         || (indication == HF_BROKEN_ARM_INDICATION)
                         || (indication == CAP_SENSOR_INDICATION))
                     && !is_clock
                     && getLockClose())
            {
                is_clock_set = true;
            }

            if (   (   (indication == DISARM_INDICATION)
                    || (indication == HF_DISARM_INDICATION)
                    || (indication == CAP_SENSOR_INDICATION))
                && is_clock_set
                && !getLockOpen())
            {
                printf("INFO Импульс открытия ЦЗ получен\n");

                is_clock     = true;
                is_clock_set = false;
            }
            else if (   (   (indication == ARM_INDICATION)
                         || (indication == HF_ARM_INDICATION)
                         || (indication == BROKEN_ARM_INDICATION)
                         || (indication == HF_BROKEN_ARM_INDICATION)
                         || (indication == CAP_SENSOR_INDICATION))
                     && is_clock_set
                     && !getLockClose())
            {
                printf("INFO Импульс закрытия ЦЗ получен\n");

                is_clock     = true;
                is_clock_set = false;
            }
        }
        else
        {
            is_clock = true;
        }

        if (getAlarm())
        {
            is_signal = true;
        }

        if (is_signal && !getAlarm())
        {
            printf("INFO %d звуковой импульс получен\n", ++signal_no);

            is_signal = false;
        }

        if (!is_light && getLights())
        {
            check_light_width_timer.start();

            is_light = true;
        }

        if (   is_light
            && !getLights()
            && check_light_width_timer.read_ms() > TIME_DELAY::MIN_LIGHT_ERROR_PULSE_WIDTH_MS)
        {
            check_light_width_timer.stop();
            check_light_width_timer.reset();
            printf("INFO %d импульс световых сигналов получен\n", ++light_no);

            is_light = false;
        }

        if (   (signal_no == signal_count)
            && (light_no == light_count)
            && is_clock)
        {
            printf("INFO Соответствующее количество звуковых/световых сигналов получено\n");

            result = true;
            break;
        }

        curr_time = timer.read() - begin;
    }
    
    // Для сенсора ручки должны отсутствовать импульсы ЦЗ
    // Нужно ожидать их все время, поэтому проверяем после цикла(таймаута)
    if (indication == CAP_SENSOR_INDICATION)
    {
        if (   (signal_no == signal_count)
            && (light_no  == light_count))
        {
            if (lock_indication)
            {
                if (!is_clock)
                {
                    printf("INFO Соответствующее количество сигналов поворотников/сирены получено\n"); // Пишем здесь, т.к. при is_clock уже написано раннее(в цикле)
                    printf("INFO Сигналы ЦЗ отсутствуют\n");
                    
                    result = true;
                }
                else
                {
                    printf("INFO Сигналы ЦЗ присутствуют\n");
                    
                    result = false;
                }
            } // if lock_indication
        } // if signal_no == signal_count
        else
        {
            printf("INFO Cигналы поворотников/сирены несоответствуют ожидаемому количеству\n");
            
            if (lock_indication && is_clock)
            {
                printf("INFO Сигналы ЦЗ присутствуют\n");
            }
            
            result = false;
        }
    } // if indication == CAP_SENSOR_INDICATION

    if ((time_error_s > TIME_DELAY::NULL_DELAY_S) && (curr_time < time_s - time_error))
    {
        printf("INFO Импульсы получены за пределами диапазона %.2f +- %.2f с (ожидание %.2f с)\n",
               time_s,
               time_error,
               curr_time);

        result = false;
    }

    timer.stop();

    return result;
}

string Gen6Basic::getWeekDay(uint8_t buff)
{
    switch (buff)
    {
        case 0:
            return "64";

        case 1:
            return "1";

        case 2:
            return "2";

        case 3:
            return "4";

        case 4:
            return "8";

        case 5:
            return "16";

        case 6:
            return "32";

        default:
            printf("FAIL Неверный день недели: %d\n", buff);
            return "1";
    }
}

bool Gen6Basic::getSysTime(time_t &sys_time)
{
    char *ch;

    char buf[255];

    memset(buf, 0, 255);

    printf("INFO Запрос на получение системного времени\n");

    printf("GET_SYS_TIME \n");

    if (UartInteract::getString(p_serial, buf, TIME_DELAY::ONE_MINUTE))
    {
        if (strstr(buf, "STATUS_SYS_TIME_OK") != NULL)
        {
            for (ch = buf; !isdigit(*ch); ++ch)
            {
                continue;
            }

            sys_time = atoi(ch);

            printf("INFO Ответ об успешном получении системного времени получен (%d)\n", sys_time);

            return true;
        }
    }

    printf("INFO Ответ об успешном получении времени не получен\n");

    sys_time = time(NULL);

    return false;
}

bool Gen6Basic::waitGsmTime(float timeout_s)
{
    const char *default_hour  = "00"; // Час в системе после сброса
    const char *default_day   = "01"; // День в системе после сброса
    const char *default_month = "01"; // Месяц в системе после сброса

    char        hour_buf[3];          // Буфер для хранения текущего часа в системе
    char        day_buf[3];           // Буфер для хранения текущего дня в системе
    char        month_buf[3];         // Буфер для хранения текущего месяца в системе
    char        datetime_buf[32];     // Буфер для хранения полной текущей даты и времени

    Timer       timer;                // Таймер, для расчета таймаута
    time_t      sys_time_s;           // Текущее время системы
    struct tm  *timeinfo;             // Текущее время системы в особом формате

    bool        is_gsm_init = false;  // Инициализировался ли GSM (системное время отлично от значения по умолчанию)


    printf("INFO Ожидание получения системой времени по GSM\n");
    printf("INFO Примечание: если текущий час = %s, день = %s, месяц = %s, то ожидание будет неуспешным\n",
           default_hour,
           default_day,
           default_month);

    if ((m_device_modules & GSM_MODULE) != GSM_MODULE)
    {
        printf("INFO Отсутствует модуль GSM\n");
        return true;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (!getSysTime(sys_time_s))
        {
            continue;
        }

        timeinfo = localtime(&sys_time_s);

        memset(datetime_buf,
               0,
               sizeof(datetime_buf));
        strftime(datetime_buf,
                 sizeof(datetime_buf),
                 "%d-%m-%Y %H:%M:%S",
                 timeinfo);
        printf("INFO Получено время системы: %s\n", datetime_buf);

        // Изъятие и проверка часа
        memset(hour_buf,
               0,
               sizeof(hour_buf));
        strftime(hour_buf,
                 sizeof(hour_buf),
                 "%H",
                 timeinfo);

        if (strncmp(hour_buf,
                    default_hour,
                    sizeof(hour_buf)) != 0)
        {
            is_gsm_init = true;
            break;
        }

        // Изъятие и проверка дня
        memset(day_buf,
               0,
               sizeof(day_buf));
        strftime(day_buf,
                 sizeof(day_buf),
                 "%d",
                 timeinfo);

        if (strncmp(day_buf,
                    default_day,
                    sizeof(day_buf)) != 0)
        {
            is_gsm_init = true;
            break;
        }

        // Изъятие и проверка месяца
        memset(month_buf,
               0,
               sizeof(month_buf));
        strftime(month_buf,
                 sizeof(month_buf),
                 "%m",
                 timeinfo);

        if (strncmp(month_buf,
                    default_month,
                    sizeof(month_buf)) != 0)
        {
            is_gsm_init = true;
            break;
        }

        printf("TRACE Ожидание окончания паузы между запросами на получение времени системы\n");
        wait(TIME_DELAY::WAITING_REGION_STATE_S); // Пауза между попытками получения времени, чтобы не слишком часто выводить логи
    } // while

    timer.stop();

    if (is_gsm_init)
    {
        printf("INFO Время по GSM получено\n");
    }
    else
    {
        printf("INFO Время по GSM не получено\n");
    }

    printf("INFO Прошло времени: %.2f сек.\n", timer.read());

    return is_gsm_init;
}

void Gen6Basic::wait_s(float time_s)
{
    Timer timer;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    printf("INFO Начало ожидания %.2f сек.\n", time_s);

    while (end < time_s)
    {
        end = timer.read() - begin;
    }

    timer.stop();

    printf("INFO Ожидание завершено\n");
}

bool Gen6Basic::getLogLabelStr(log_label_t  log_label,
                               char        *str,
                               uint32_t     len)
{
    switch (log_label)
    {
        case LOG_LABEL_INFO:
            strncpy(str, "INFO", len);
            return true;

        case LOG_LABEL_FAIL:
            strncpy(str, "FAIL", len);
            return true;

        case LOG_LABEL_PASS:
            strncpy(str, "PASS", len);
            return true;

        default:
            return false;
    }
}

bool Gen6Basic::waitLightsPulse(int32_t count,
                                bool    polarity,
                                float   time,
                                float   pulse_time,
                                bool    error_flag,
                                float   error)
{
    return waitPulse(p_in_light_combined,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitAltLightPulse(int32_t count,
                                  bool    polarity,
                                  float   time,
                                  float   pulse_time,
                                  bool    error_flag,
                                  float   error)
{
    return waitPulse(p_in_light_alt,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitUnlockDriverDoor(int32_t count,
                                     bool    polarity,
                                     float   time,
                                     float   pulse_time,
                                     bool    error_flag,
                                     float   error)
{
    return waitPulse(p_in_lock_b_door,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitAltLockPulse(int32_t count,
                                 bool    polarity,
                                 float   time,
                                 float   pulse_time,
                                 bool    error_flag,
                                 float   error)
{
    return waitPulse(p_in_lock_alt,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitSirenPulse(int32_t count,
                               bool    polarity,
                               float   time,
                               float   pulse_time,
                               bool    error_flag,
                               float   error)
{
    return waitPulse(p_in_siren,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitKlaxonPulse(int32_t count,
                                bool    polarity,
                                float   time_s,
                                float   pulse_time_s,
                                bool    error_flag,
                                float   error_s)
{
    return waitPulse(p_in_klaxon,
                     count,
                     polarity,
                     time_s,
                     pulse_time_s,
                     error_flag,
                     error_s); 
}

bool Gen6Basic::waitBuzzerExtPulse(int32_t count,
                                   bool    polarity,
                                   float   time,
                                   float   pulse_time,
                                   bool    error_flag,
                                   float   error)
{
    return waitPulse(p_in_buzzer_ext,
                     count,
                     polarity,
                     time,
                     pulse_time,
                     error_flag,
                     error);
}

bool Gen6Basic::waitHoodOpenPulse(int32_t count,
                                  bool    polarity,
                                  float   time,
                                  float   pulse_time_s,
                                  bool    error_flag,
                                  float   error_s)
{
    return waitPulse(p_in_hood_lock_open,
                     count,
                     polarity,
                     time,
                     pulse_time_s,
                     error_flag,
                     error_s);
}       

bool Gen6Basic::waitHoodClosePulse(int32_t count,
                                   bool    polarity,
                                   float   time,
                                   float   pulse_time_s,
                                   bool    error_flag,
                                   float   error_s)
{
    return waitPulse(p_in_hood_lock_close,
                     count,
                     polarity,
                     time,
                     pulse_time_s,
                     error_flag,
                     error_s);
}                         

bool Gen6Basic::checkDigIn(PinName dig_pin, channel_polarity_t polarity)
{
    DigitalOut  y_r_pin(OUT_X96_YELLOW_RED);
    DigitalIn  *dig_in;

    bool result = true;

    if (dig_pin == NC)
    {
        return false;
    }

    dig_in = new DigitalIn(dig_pin);
    setSecure(ON, KEYRING);

    if (*dig_in != (polarity == CNL_POLARITY_POSITIVE))
    {
        result = false;
    }

    setSecure(OFF, KEYRING);

    if (*dig_in != (polarity == CNL_POLARITY_NEGATIVE))
    {
        result = false;
    }

    delete dig_in;

    return result;
}

bool Gen6Basic::setDigOut(PinName dig_pin, bool state)
{
    DigitalOut *dig_out;

    if (dig_pin == NC)
    {
        return false;
    }

    dig_out  = new DigitalOut(dig_pin);
    *dig_out = state;

    delete dig_out;

    return true;
}

void Gen6Basic::setPanic(panic_method_t method)
{
    switch (method)
    {
        case PANIC_REMOTE:
            pushLedKeyringBut1(KEYRING_MIN);
            wait(KEYRING_MIN);
            pushLedKeyringBut1(KEYRING_MIN);
            break;

        case PANIC_SMS:
            p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                           SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_PANIC);
            break;
        
        case PANIC_SLO:
            setStarlineOnlineParam(StarlineOnline::PANIC, ON);

        default:
            break;
    }
}

bool Gen6Basic::setAhjTagSetting(bool state)
{
    bool result = false;

    setTagVcc();
    wait(TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);

    for (uint8_t i = 0; i < COUNT_STRUCT::MAX_TAG_CTRL_COUNT; ++i)
    {
        if (waitSlpCmdStatus(DIAG_DATA_FLAG,
                             SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                             DIAG_FLAG_BIT_ANTI_HIJACK_TAG_EN,
                             state,
                             TIME_DELAY::AHJ_TAG_EN_TIME_S))
        {
            result = true;;
            printf("INFO Функция \"Антиограбление по пропаданию метки\" %s\n", state ? "вкл" : "выкл");
            break;
        }

        pushTagBtn(TAG_LONG_1);
    }

    resetTagVcc();
    wait(TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);

    return result;
}

bool Gen6Basic::setDangerParking(bool state)
{
    bool    result        = false;
    uint8_t srv_led_count = COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT; // Количество морганий сервисного светодиода


    if (state)
    {
        printf("INFO Включение режима \"Опасная парковка\"\n");
        srv_led_count += COUNT_STRUCT::DANGER_PARKING_EN_SIGNALS_COUNT;
    }
    else
    {
        printf("INFO Выключение режима \"Опасная парковка\"\n");
        srv_led_count += COUNT_STRUCT::DANGER_PARKING_DIS_SIGNALS_COUNT;
    }

    if (checkDangerParking(state))
    {
        printf("INFO Режим \"Опасная парковка\" уже находится в желаемом состоянии\n");
        return true;
    }

    clickServiceBtn(COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT);
    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);

    setIgn(PIN,
           ON,
           TIME_DELAY::NULL_DELAY_S);

    setIgn(CAN_USE,
           ON,
           TIME_DELAY::NULL_DELAY_S);

    result = p_service_btn->waitPulseAmount(srv_led_count, TIME_DELAY::DANGER_PARKING_EN_TIME_S); // Ожидание входа в ОП (и проверка сервисного светодиода)

    setIgn(PIN, OFF);
    setIgn(CAN_USE, OFF);

    // На X96v2 светодиод не проверяется
    if (m_device_name != GEN6_X96V2)
    {
        if (result)
        {
            printf("INFO Сигналы сервисного светодиода соответствуют ожидаемым\n");
        }
        else
        {
            printf("INFO Сигналы сервисного светодиода несоответствуют ожидаемым\n");
        }
    }
    else
    {
        printf("INFO Сервисный светодиод не проверяется на системе X96v2\n");
        result = true;
    }

    return result;
}

bool Gen6Basic::checkDangerParking(bool state)
{
    return (state == ON ?  getSlpCmdStatus(DIAG_DATA_EEFLAG_SYSDATA,
                                           SLP_STATUSES_SIZES::DIAG_DATA_EEFLAG_SYSDATA_SIZE,
                                           DIAG_EEFLAG_SYSDATA::ID_FLAG_DANGER_PARKING)
                        : !getSlpCmdStatus(DIAG_DATA_EEFLAG_SYSDATA,
                                           SLP_STATUSES_SIZES::DIAG_DATA_EEFLAG_SYSDATA_SIZE,
                                           DIAG_EEFLAG_SYSDATA::ID_FLAG_DANGER_PARKING));
}

bool Gen6Basic::setAndCheckDangerParking(bool state, float wait_time_s)
{    
    setDangerParking(state);

    return waitSlpCmdStatus(DIAG_DATA_EEFLAG_SYSDATA,
                            SLP_STATUSES_SIZES::DIAG_DATA_EEFLAG_SYSDATA_SIZE,
                            DIAG_EEFLAG_SYSDATA::ID_FLAG_DANGER_PARKING,
                            state,
                            wait_time_s);
}

bool Gen6Basic::checkDangerParkingSignals(bool state, float timeout_s)
{
    const uint8_t exp_srv_btn_led_count = state ? COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT + COUNT_STRUCT::DANGER_PARKING_EN_SIGNALS_COUNT   // Ожидаемое количество вспышек сервисного светодиода
                                                : COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT + COUNT_STRUCT::DANGER_PARKING_DIS_SIGNALS_COUNT;

    const uint8_t exp_siren_count       = exp_srv_btn_led_count;                                                                                       // Ожидаемое количество сигналов сирены

    const uint8_t exp_lights_count      = state ? COUNT_STRUCT::DANGER_PARKING_EN_SIGNALS_COUNT                                                        // Ожидаемое количество сигналов света
                                                : COUNT_STRUCT::DANGER_PARKING_DIS_SIGNALS_COUNT;

    Timer   timer;
    bool    result                  = true;                     // Результат

    bool    is_srv_led_on           = false;                    // Включен ли сервисный светодиод
    bool    is_siren_on             = false;                    // Включена ли сирена
    bool    is_lights_on            = false;                    // Включен ли свет

    uint8_t srv_btn_led_count       = 0;                        // Количество вспышек сервисного светодиода
    uint8_t siren_signal_count      = 0;                        // Количество сигналов сирены
    uint8_t lights_signal_count     = 0;                        // Количество сигналов света

    float   srv_led_time_s          = TIME_DELAY::NULL_DELAY_S; // Время получения потенциала светодиода сервисной кнопки
    float   siren_potential_time_s  = TIME_DELAY::NULL_DELAY_S; // Время получения потенциала сирены
    float   lights_potential_time_s = TIME_DELAY::NULL_DELAY_S; // Время получения потенциала света

    // Определение функции получения звуковых сигналов
    int8_t (Gen6Basic::*getSirenSignal)(void) = m_device_name == GEN6_X96V2 ? &Gen6Basic::getBuzzerExt // Для X96v2 подтверждение команды выходит на бузер
                                                                            : &Gen6Basic::getSiren;    // Для остальных систем - на сирену

    timer.start();

    while (timer.read() < timeout_s)
    {
        // Потенциал сервисного светодиода
        if (   !is_srv_led_on
            &&  p_service_btn->isLedOn())
        {
            is_srv_led_on  = true;
            srv_led_time_s = timer.read();
        }

        // Импульс сервисного светодиода
        if (    is_srv_led_on
            && !p_service_btn->isLedOn()
            &&  timer.read() - srv_led_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            is_srv_led_on = false;
            printf("INFO Получен %d сигнал сервисного светодиода\n", ++srv_btn_led_count);
        }

        // Потенциал сирены
        if (   !is_siren_on
            && (this->*getSirenSignal)())
        {
            is_siren_on            = true;
            siren_potential_time_s = timer.read();
        }

        // Импульс сирены
        if (     is_siren_on
            && !(this->*getSirenSignal)()
            &&   timer.read() - siren_potential_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            is_siren_on = false;
            printf("INFO Получен %d сигнал сирены\n", ++siren_signal_count);
        }

        // Потенциал света
        if (   !is_lights_on
            &&  getLights())
        {
            is_lights_on            = true;
            lights_potential_time_s = timer.read();
        }

        // Импульс света
        if (    is_lights_on
            && !getLights()
            &&  timer.read() - lights_potential_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            is_lights_on = false;
            printf("INFO Получен %d сигнал света\n", ++lights_signal_count);
        }
    }

    // На X96v2 сервисный светодиод не проверяется
    if (m_device_name != GEN6_X96V2)
    {
        if (srv_btn_led_count != exp_srv_btn_led_count)
        {
            result = false;
            printf("FAIL Количество сигналов сервисного светодиода несоответствует ожидаемому. Получено: %d, ожидалось: %d\n",
                   srv_btn_led_count,
                   exp_srv_btn_led_count);
        }
    }
    else
    {
        printf("INFO Сервисный светодиод не проверяется на системе X96v2\n");
    }

    if (siren_signal_count != exp_siren_count)
    {
        result = false;
        printf("FAIL Количество сигналов сирены несоответствует ожидаемому. Получено: %d, ожидалось: %d\n",
               siren_signal_count,
               exp_siren_count);

    }

    if (lights_signal_count != exp_lights_count)
    {
        result = false;
        printf("FAIL Количество сигналов света несоответствует ожидаемому. Получено: %d, ожидалось: %d\n",
               lights_signal_count,
               exp_lights_count);

    }

    if (result)
    {
        printf("PASS Полученные сигналы соответствуют ожидаемым\n");
    }
    else
    {
        printf("FAIL Полученные сигналы несоответствуют ожидаемым\n");
    }

    return result;
}

bool Gen6Basic::setBleSettingMode(bool state)
{
    bool result = false;


    if (state)
    {
        printf("INFO Включение режима настройки по BLE\n");
    }
    else
    {
        printf("INFO Выключение режима настройки по BLE\n");
    }

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL Отсутствует BLE модуль\n");
        return false;
    }

    if (checkBleSettingMode(state))
    {
        printf("INFO Режим настройки по BLE уже находится в желаемом состоянии\n");
        return true;
    }

    for (uint8_t i = 0; i < COUNT_STRUCT::BTN_MODE_BLE_CLICKS_COUNT; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_BTN_BETWEEN_CLICK_S);
    }

    setIgn(PIN, ON);
    setIgn(CAN_USE, ON);

    if (getDeviceName() != GEN6_X96V2)
    {
        result = p_service_btn->waitPulseAmount(COUNT_STRUCT::BTN_MODE_BLE_CLICKS_COUNT,
                                                TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S);
    }
    else
    {
        wait(TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S);
        result = true;
    }

    setIgn(PIN, OFF);
    setIgn(CAN_USE, OFF);

    return result;
}

bool Gen6Basic::checkBleSettingMode(bool state)
{
    return (state == ON ?  getSlpCmdStatus(DIAG_DATA_EEFLAG_SYSDATA,
                                           SLP_STATUSES_SIZES::DIAG_DATA_EEFLAG_SYSDATA_SIZE,
                                           DIAG_EEFLAG_SYSDATA::ID_FLAG_BLE_UPDATER)
                        : !getSlpCmdStatus(DIAG_DATA_EEFLAG_SYSDATA,
                                           SLP_STATUSES_SIZES::DIAG_DATA_EEFLAG_SYSDATA_SIZE,
                                           DIAG_EEFLAG_SYSDATA::ID_FLAG_BLE_UPDATER));
}

void Gen6Basic::setSecureNoDelay(int8_t         value,
                                 command_type_t execution_method)
{
    printf("INFO Управление охраной без задержки после управления. Метод исполнения: %d\n", execution_method);

    execution_method = getExecutionMethod(execution_method, USER_ACTION);

    printf("INFO Метод исполнения после коррекции (при отсутствии необходимых модулей): %d\n", execution_method);

    switch (execution_method)
    {
        case CAN_USE:

            value == ON ? p_can_actions->sendStdSecureOnOnePkg()
                        : p_can_actions->sendStdSecureOffOnePkg();

            break;

        case KEYRING:
            setSecure(value,
                      execution_method,
                      TIME_DELAY::NULL_DELAY_S);
            break;

        case TAG:
            setTagVcc();
            wait_s(TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);
            pushTagBtn(TAG_SHORT);
            break;

        case SMS:

            value == ON ? p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1], SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_PROT_ON)
                        : p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1], SMS_CMD_TOKENS_NUMERIC::SMS_PARSE_CMD_PROT_OFF);

            break;

        default:
            printf("FAIL Неизвестный метод исполнения для управления охраной: %d\n", execution_method);
    }
}

bool Gen6Basic::unlockTrunk(command_type_t command_type,
                            float          delay_s,
                            repeat_mode_t  repeat_mode)
{
    bool result = false;

    command_type = getExecutionMethod(command_type, SLO_ACTION);

    if (command_type == SLO_ACTION)
    {
        printf("INFO Отпирание багажника через сервер\n");
    }
    else
    {
        printf("INFO Отпирание багажника брелком\n");
    }

    switch (command_type)
    {
        case KEYRING:
            if (repeat_mode == REPEAT_MODE_ON)
            {   
                for (uint8_t i = 0; i <= 2 ; ++i)
                {
                    if (!waitSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                          SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                          DIAG_GUARD_STATE_ARM_TRUNK,
                                          ON,
                                          TIME_DELAY::ONE_S))
                    {
                        p_led_keyring->unlockTrunkCommand();
                    }
                    else
                    {
                        result = true;
                        break;
                    }
                }
            }
            else
            {
                p_led_keyring->unlockTrunkCommand();
                result = true;
                break;
            }
        
        case FLEX_LOGIC:
            return true;

        case SLO:
            result = setStarlineOnlineParam(StarlineOnline::DISARM_TRUNK, ON);
            break;

        case CAN_USE:
            p_can_actions->sendTrunkUnlock(TIME_DELAY::NULL_DELAY_S);
            result = true;
            break;

        default:
            printf("INFO Тип управления не определен\n");
            return false;
    }

    if (result == true)
    {
        wait(delay_s);
        return result;
    }
    
    return result;
}

PinName Gen6Basic::getPinNameIn(DigitalIn *digitalIn)
{
    gen6_functional_t temp;

    for (map<gen6_functional_t, DigitalIn *>::iterator iter = s_in_channels_map.getMap().begin(); iter != s_in_channels_map.getMap().end(); ++iter)
    {
        if (iter->second == digitalIn)
        {
            temp = iter->first;
            break;
        }
    }
    
    return s_in_channels_pins_map[temp];
}

void Gen6Basic::setPolaritySrvBtn()
{
    if (p_srv_btn_pw != NULL)
    {
        delete p_srv_btn_pw;
    }

    p_srv_btn_pw = new PulseWidth();

    p_srv_btn_pw->setParams(IN_SERVICE_LED, 1);
}

bool Gen6Basic::checkPulseWidthSrvBtn(float pulse_time_s,
                                      float timeout_s,
                                      float error_time_s)
{
    if (p_srv_btn_pw == NULL)
    {
        printf("FAIL Измеритель длины импульса не настроен\n");

        return false;
    }

    return p_srv_btn_pw->checkPulseWidth(pulse_time_s,
                                         timeout_s,
                                         error_time_s);
}

void Gen6Basic::setPolarity(gen6_functional_t gen6_functional)
{
    if (s_in_channels_map.getMap().find(gen6_functional) != s_in_channels_map.getMap().end())
    {
        // освобождаем для повторного использования
        if (s_pw_channels_map.getMap().find(gen6_functional) != s_pw_channels_map.getMap().end())
        {
            delete s_pw_channels_map[gen6_functional];
        }

        s_pw_channels_map[gen6_functional] = new PulseWidth();

        s_pw_channels_map[gen6_functional]->setParams(getPinNameIn(s_in_channels_map[gen6_functional]),
                                                      getPolarity(gen6_functional));
    }
    else
    {
        printf("FAIL На провод с данной функциональностью %d невозможно установить полярность для измерения длительности импульса\n",
               gen6_functional);
    }
}

bool Gen6Basic::checkPulseWidth(gen6_functional_t gen6_functional,
                                float             pulse_time_s,
                                float             timeout_s,
                                float             error_time_s)
{
    if (s_pw_channels_map.getMap().find(gen6_functional) == s_pw_channels_map.getMap().end())
    {
        printf("FAIL На проводе с данной функциональностью %d невозможно измерить длительность импульса\n", gen6_functional);

        return false;
    }

    return s_pw_channels_map[gen6_functional]->checkPulseWidth(pulse_time_s,
                                                               timeout_s,
                                                               error_time_s);
}

bool Gen6Basic::checkPulseWithoutWidth(gen6_functional_t gen6_functional,
                                       float             timeout_s)
{
    if (s_pw_channels_map.getMap().find(gen6_functional) == s_pw_channels_map.getMap().end())
    {
        printf("FAIL На проводе с данной функциональностью %d невозможно измерить импульс\n", gen6_functional);

        return false;
    }

    return s_pw_channels_map[gen6_functional]->checkPulseWithoutWidth(timeout_s);
}

bool Gen6Basic::checkArmState(bool state, float delay)
{
    switch (state)
    {
        case OFF:
            setSecure(ON, KEYRING);
            setSecure(OFF, KEYRING);

            if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                 SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                 DIAG_GUARD_STATE_DISARM))
            {
                printf("FAIL Начальные условия: охрана включена.\n");
                return false;
            }
            
            printf("PASS Начальные условия: охрана не включена.\n");
            break;

        case ON:
            setSecure(OFF, KEYRING);
            setSecure(ON, KEYRING);

            if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                 SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                 DIAG_GUARD_STATE_ARM))
            {
                printf("FAIL Начальные условия: охрана не включена.\n");
                return false;
            }
            
            printf("PASS Начальные условия: охрана включена.\n");
            break;

        default:
            printf("PASS Начальное условие не задано.\n");
            break;
    }

    wait(delay);
    return true;
}

void Gen6Basic::clearSensors()
{
    p_slp->setSlpStatus(DIAG_DATA_SENSORS, 0);
}

bool Gen6Basic::checkStdOnMessages(float time)
{   
    p_can_actions->clearFifo();
    
    if (p_can_actions->waitWakeUp(time))
    {
        if (!p_can_actions->waitStdOn(TIME_DELAY::STD_WIRE_EN_DELAY_S))
        {
            p_can_actions->sendStatusStdOff();
            
            return p_can_actions->waitStdOn(); 
        }
        
        return true;
    }
    
    return false;
}

bool Gen6Basic::checkStdOffMessages(float time)
{   
    p_can_actions->clearFifo();
    
    if (   (m_device_name == GEN6_X96)
        || (m_device_name == GEN6_M96))
    {
        return p_can_actions->waitStdOff(time);
    }
    else
    {  
        if (p_can_actions->waitWakeUp(time))
        {
            if (!p_can_actions->waitStdOff(TIME_DELAY::MIN_WIRE_EN_DELAY_S))
            {
                p_can_actions->sendStatusStdOn();
                
                return p_can_actions->waitStdOff(time); 
            }
            
            return true;
        }
        
        return false;
    }
}

bool Gen6Basic::checkLockEnabledMessages(float time)
{    
    p_can_actions->clearFifo();
    
    return p_can_actions->waitLockEnabled(time); 
}

bool Gen6Basic::checkLockDisabledMessages(float time)
{    
    p_can_actions->clearFifo();
    
    return p_can_actions->waitLockDisabled(time);
}

bool Gen6Basic::waitSirenAndLightSignal(light_status_t light_indication,
                                        siren_status_t siren_indication,
                                        float          time_s)
{
    Timer timer;

    bool  is_light = false;
    bool  is_siren = false;


    printf("INFO Начало ожидания световых и звуковых сигналов\n");
    timer.start();

    while (timer.read() < time_s)
    {
        if (   getDeviceName() == GEN6_M96
            || getDeviceName() == GEN6_X96)
        {
            if (!getLights())
            {
                is_light = true;
            }
        }
        else
        {
            if (getLights())
            {
                is_light = true;
            }
        }

        if (getAlarm())
        {
            is_siren = true;
        }

        if (   (light_indication || siren_indication)
            && light_indication == is_light
            && siren_indication == is_siren)
        {
            printf("INFO Состояние сирены и световых сигналов соответствует ожидааемому\n");
            return true;
        }
    }

    timer.stop();

    if (!light_indication && !siren_indication)
    {
        printf("INFO Состояние сирены и световых сигналов соответствует ожидааемому\n");
        return true;
    }

    printf("INFO Состояние сирены и световых сигналов не соответствует ожидааемому\n");

    return false;
}

bool Gen6Basic::waitLightsImpulseOrPotentialStart(float wait_time_s)
{
    Timer timer;


    printf("INFO Ожидание появления световых сигналов (импульсное или потенциальное управление)\n");
    timer.start();

    while (timer.read() < wait_time_s)
    {
        if (   getLightsImpulse()
            || getLightsPotential())
        {
            return true;
        }
    }

    return false;
}

bool Gen6Basic::checkLightsImpulseAndPotential(float                pulse_width_s,
                                               float                pulse_wait_time_s,
                                               float                lights_impulse_width_s,
                                               float                lights_impulse_pause_s,
                                               float                lights_potential_len_s,
                                               lights_pulse_count_t impulse_count,
                                               float                pulse_width_error_time_s)
{
    Timer  timer;
    bool   result    =   true;                  // Результат
    float  timeout_s =   pulse_wait_time_s      // Время проверки сигналов
                       + pulse_width_s;
    int8_t err_perc  =   5;                     // Процент допустимой ошибки (отклонение от нуля)
                                                // Также нужно учитывать, что мы можем поймать импульс чуть позже из-за долгих рассчетов + вывода лога

    float  impulse_width_error_s;               // Ошибка длительности импульса               (импульсное управление светом)
    float  impulse_pause_error_s;               // Ошибка длительности паузы между импульсами (импульсное управление светом)
    float  potential_len_error_s;               // Ошибка длительности потенциала             (потенциальное управление светом)

    float  min_impulse_pause_s = pulse_width_s; // Минимальная пауза между импульсами    (импульсное управление светом)
    float  min_potential_len_s = pulse_width_s; // Минимальная длительность потенциала   (потенциальное управление светом)

    bool   is_impulse_start    = false;         // Был ли получен сигнал начала импульса (импульсное управление светом)
    bool   is_potential_start  = false;         // Был ли получен потенциал              (потенциальное управление светом)

    int8_t curr_impulse_count  = 0;             // Количество полученных импульсов света (импульсное управление светом)
    bool   is_potential_end    = false;         // Закончен потенциал света              (потенциальное управление светом)

    float  impulse_start_time_s;                // Время получения начала импульса       (импульсное управление светом)
    float  impulse_end_time_s;                  // Время конца полученного импульса      (импульсное управление светом)
    float  impulse_pause_s;                     // Длительность паузы между импульсами   (импульсное управление светом)
    float  impulse_width_s;                     // Длительность полученного импульса     (импульсное управление светом)
    float  expected_impulse_pause_s;            // Ожидаемая пауза между импульсами      (импульсное управление светом)

    float  potential_start_time_s;              // Время начала полученного потенциала   (потенциальное управление светом)
    float  potential_len_s;                     // Длительность полученного потенциала   (потенциальное управление светом)


    // Если ожидаемая длина потенциала меньше настроенной - то приводим к минимуму
    if (pulse_width_s < lights_potential_len_s)
    {
        min_potential_len_s = lights_potential_len_s;
    }

    // Если ожидаемая длина импульса меньше настроенной - то приводим к минимуму
    if (pulse_width_s < lights_impulse_pause_s)
    {
        min_impulse_pause_s = lights_impulse_pause_s;
    }

    // Ошибки импульсов
    impulse_width_error_s = lights_impulse_width_s * err_perc / 100.0f;

    if (pulse_width_error_time_s == TIME_DELAY::NULL_DELAY_S)
    {
        impulse_pause_error_s = min_impulse_pause_s * err_perc / 100.0f;
        potential_len_error_s = min_potential_len_s * err_perc / 100.0f;
    }
    else
    {
        impulse_pause_error_s = pulse_width_error_time_s;
        potential_len_error_s = pulse_width_error_time_s;
    }

    printf("INFO Ожидание импульсов света (импульсное и потенциальное управление)\n");
    timer.start();

    // Структура цикла:
    // 0 Проверка, что хотя бы один сигнал был получен в пределах времени ожидания (pulse_wait_time_s)
    // 1 Проверка потенциального управления
    //     1.1 Получен активный уровень
    //         1.1.1 Потенциал еще не был получен
    //         1.1.2 Потенциал уже был получен и получаем снова (ошибка) -> выход из цикла
    //
    //     1.2 Получен пассивный уровень
    //         1.2.1 Длина потенциала верная
    //         1.2.2 Длина потенциала неверная (ошибка) -> выход из цикла
    //
    // 2 Проверка импульсного управления
    //     2.1 Получен активный уровень
    //         2.1.1 Активный уровень первого импульса
    //         2.1.2 Активный уровень второго импульса и последующих
    //             2.1.2.1 Расчет паузы между импульсами
    //             2.1.2.2 Пауза между импульсами выдержана
    //             2.1.2.3 Пауза между импульсами не выдержана (ошибка) -> выход из цикла
    //
    //     2.2 Получен пассивный уровень
    //         2.2.1 Длина импульса верная
    //         2.2.2 Длина импульса неверная (ошибка) -> выход из цикла
    while (timer.read() < timeout_s)
    {
        // 0 Проверка, что хотя бы один сигнал был получен в пределах времени ожидания
        if (    timer.read()       >  pulse_wait_time_s
            &&  curr_impulse_count == 0
            && !is_impulse_start
            && !is_potential_start
            && !is_potential_end)
        {
            printf("FAIL В течение времени ожидания не было получено ни одного сигнала света\n");
            result = false;
            break;
        }

        // 1   Проверка потенциального управления
        // 1.1 Активный уровень потенциального управления
        if (   !is_potential_start
            &&  getLightsPotential())
        {
            // 1.1.1 Потенциал света еще не был получен - все так и должно быть
            if (!is_potential_end)
            {
                potential_start_time_s = timer.read();
                is_potential_start     = true;
                printf("PASS Получен потенциал света (потенциальное управление)\n");
            }
            // 1.1.2 Получен лишний потенциал света
            else
            {
                printf("FAIL Получен лишний потенциал света (потенциальное управление)\n");
                result = false;
                break;
            }
        }

        // 1.2 Пассивный уровень потенциального управления
        if (    is_potential_start
            && !is_potential_end
            && !getLightsPotential())
        {
            potential_len_s = timer.read() - potential_start_time_s;

            if (potential_len_s > TIME_DELAY::MIN_PULSE_WIDTH_S) // Фильтр дребезга
            {
                // 1.2.1 Проверка, что длина полученного потенциала соответствует заданной (с погрешностью potential_len_error_s)
                if (   potential_len_s < min_potential_len_s + potential_len_error_s
                    && potential_len_s > min_potential_len_s - potential_len_error_s)
                {
                    is_potential_end   = true;
                    is_potential_start = false;
                    printf("PASS Получен световой сигнал (потенциальное управление)\n");
                }
                // 1.2.2 Длина импульса неверная
                else
                {
                    printf("FAIL Длительность полученного потенциала света неверная (потенциальное управление): %.6f\n", potential_len_s);
                    result = false;
                    break;
                }
            }
        }

        // 2   Проверка импульсного управления
        // 2.1 Получен активный уровень импульса
        if (   !is_impulse_start
            &&  getLightsImpulse())
        {
            impulse_start_time_s = timer.read();

            // 2.1.1 Первый импульс
            if (curr_impulse_count == 0)
            {
                is_impulse_start = true;
                printf("PASS Получен первый потенциал света (импульсное управление)\n");
            }
            // 2.1.2 Последующие импульсы, между ними должна быть выдержана пауза
            else
            {
                impulse_pause_s = impulse_start_time_s - impulse_end_time_s;

                // 2.1.2.1 Расчет паузы между импульсами:
                // При снятии с охраны будет получено 4 импульса (две пары импульсов)
                // Пауза между импульсами в паре (между 1 и 2 импульсом; между 3 и 4 импульсом) будет равна min_impulse_pause_s
                // Пауза между парами импульсов (между 2 и 3 импульсом) будет равна TIME_DELAY::MIN_DISARM_LIGHTS_IMPULSE_PAUSE_S
                if (curr_impulse_count == 2)
                {
                    expected_impulse_pause_s = TIME_DELAY::MIN_DISARM_LIGHTS_IMPULSE_PAUSE_S;
                }
                else
                {
                    expected_impulse_pause_s = min_impulse_pause_s;
                }

                if (impulse_pause_s > TIME_DELAY::MIN_PULSE_WIDTH_S) // Фильтр дребезга
                {
                    // 2.1.2.2 Проверка, что длина паузы между импульсами соответствует заданной (с погрешностью impulse_pause_error_s)
                    if (   impulse_pause_s < expected_impulse_pause_s + impulse_pause_error_s
                        && impulse_pause_s > expected_impulse_pause_s - impulse_pause_error_s)
                    {
                        is_impulse_start = true;
                        printf("PASS Получен потенциал света после паузы (импульсное управление)\n");
                    }
                    // 2.1.2.3 Пауза не выдержана
                    else
                    {
                        printf("FAIL Пауза между импульсами света неверная (импульсное управление): %.6f\n", impulse_pause_s);
                        result = false;
                        break;
                    }
                } // if (impulse_pause_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
            } // if (curr_impulse_count == 0)
        } // if (!is_impulse_start)

        // 2.2 Получен пассивный уровень импульса
        if (    is_impulse_start
            && !getLightsImpulse())
        {
            impulse_end_time_s = timer.read();
            impulse_width_s    = impulse_end_time_s - impulse_start_time_s;

            if (impulse_width_s > TIME_DELAY::MIN_PULSE_WIDTH_S) // Фильтр дребезга
            {
                // 2.2.1 Проверка, что длина импульса соответствует заданной (с погрешностью impulse_width_error_s)
                if (   impulse_width_s < lights_impulse_width_s + impulse_width_error_s
                    && impulse_width_s > lights_impulse_width_s - impulse_width_error_s)
                {
                    impulse_end_time_s = timer.read();
                    is_impulse_start   = false;
                    printf("PASS Получен %d-й световой сигнал (импульсное управление)\n", ++curr_impulse_count);
                }
                // 2.2.2 Длина импульса неверная
                else
                {
                    printf("FAIL Длительность полученного светового сигнала неверная (импульсное управление): %.6f\n", impulse_width_s);
                    result = false;
                    break;
                }
            } // if (impulse_width_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        } // if (is_impulse_start)
    } // while (timer.read() < timeout_s)

    timer.stop();

    if (   result
        && is_potential_end
        && curr_impulse_count == impulse_count)
    {
        printf("PASS Импульсное и потенциальное управление светом: проверка успешна\n");
    }
    else
    {
        printf("FAIL Импульсное или потенциальное управление: проверка неуспешна\n");
        result = false; // В цикле проверялись только тайминги импульсов, а не их количество (поэтому result мог остаться true)
                        // При несоответствии количества импульсов результат должен быть false
    }

    return result;
}

void Gen6Basic::setTemperatureValue(temperature_celsius_t temperature)
{
    float                               voltage_coeff                   = 0.0f;                            // Коэффициент напряжения для текущего автостенда относительно эталонного
                                                                                                           // Возникает из-за особенностей автостенда

    const float                         voltage_coeff_a96               = (float)(0.18 / 3.3);             // Коэффициент для автостенда A96       (180 мВ)
    const float                         voltage_coeff_e96_s96           = (float)(0.05 / 3.3);             // Коэффициент для автостендов E96, S96 (50  мВ)


    // Массив возможных напряжений
    const float voltage_arr[TEMP_CELSIUS_COUNT] = 
                {
                    VOLTAGE_TEMP_SENSOR::PLUS_125_VOLTAGE,  
                    VOLTAGE_TEMP_SENSOR::PLUS_100_VOLTAGE,  
                    VOLTAGE_TEMP_SENSOR::PLUS_87_VOLTAGE,   
                    VOLTAGE_TEMP_SENSOR::PLUS_70_VOLTAGE,   
                    VOLTAGE_TEMP_SENSOR::PLUS_35_VOLTAGE,   
                    VOLTAGE_TEMP_SENSOR::PLUS_11_VOLTAGE,   
                    VOLTAGE_TEMP_SENSOR::MINUS_5_VOLTAGE,   
                    VOLTAGE_TEMP_SENSOR::MINUS_20_VOLTAGE,  
                    VOLTAGE_TEMP_SENSOR::MINUS_35_VOLTAGE,  
                    VOLTAGE_TEMP_SENSOR::MINUS_51_VOLTAGE,  
                    VOLTAGE_TEMP_SENSOR::MINUS_126_VOLTAGE, 
                };

    // Имена возможных напряжений (для логирования)
    const char *voltage_str_arr[TEMP_CELSIUS_COUNT] = 
                      {
                          "+125",
                          "+100",
                          "+87", 
                          "+70", 
                          "+35", 
                          "+11", 
                          "-5", 
                          "-20", 
                          "-35", 
                          "-51", 
                          "-126"
                      };


    printf("INFO Установка температуры двигателя через датчик температуры, значение: %s* C\n", voltage_str_arr[temperature]);

    if (p_temp_sensor == NULL)
    {
        printf("FAIL Выход ЦАП для датчика температуры не определен\n");
        return;
    }

    switch (m_device_name)
    {
        case GEN6_A96:
            voltage_coeff = voltage_coeff_a96;
            break;
        
        case GEN6_E96:
        case GEN6_S96:
            voltage_coeff = voltage_coeff_e96_s96;
            break;

        default:
            printf("FAIL Автостенд не поддерживает датчик температуры на данном устройстве: %d\n", m_device_name);
            return;
    }

    if (temperature < TEMP_CELSIUS_COUNT)
    {
        p_temp_sensor->write(voltage_arr[temperature] - voltage_coeff);
    }
    else
    {
        printf("FAIL Данное значение температуры не определено - %d\n", temperature);
    }
}

void Gen6Basic::disableHumanInCar()
{
    // алгоритм имеет смысл только при включенных свободных руках и если система не в охране
    if (   getSlpCmdStatus(DIAG_DATA_FLAG,
                           SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                           DIAG_FLAG_BIT_HANDS_FREE)
        && !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                            DIAG_GUARD_STATE_ARM))
    {
        // вернуть после исправления FB-1725
        // if (getSlpCmdStatus(DIAG_DATA_FLAG_SYSDATA,
        //                     SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SYSDATA_SIZE,
        //                     DIAG_FLAG_SYSDATA::ID_FLAG_HUMAN_IN_CAR))

        printf("INFO Выполнение алгоритма, показывающего системе, что человек вышел из машины\n");

        setIgn(PIN, ON);
        wait(TIME_DELAY::STD_ERROR_TIME_S);
        setIgn(PIN, OFF);

        setDoor(OPEN,
                PIN,
                TIME_DELAY::MAX_WIRE_EN_DELAY_S);
        setDoor(CLOSE,
                PIN,
                TIME_DELAY::MAX_WIRE_EN_DELAY_S);
    }
}

bool Gen6Basic::setTagAndDisableHumanInCar()
{
    if (!setAndCheckTag(ON))
    {
        printf("INFO Метка вне зоны видимости\n");

        return false;
    }

    disableHumanInCar();

    return true;
}

void Gen6Basic::activateTag()
{
    if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
    {
        setTagVcc();
        wait(TIME_DELAY::ONE_S);

        for (uint8_t i = 0; i < 3; ++i)
        {
            pushTagBtn(TAG_SHORT);
            wait(TIME_DELAY::HALF_S);
        }

        resetTagVcc();
    }
}

void Gen6Basic::updateR6ecoFw()
{
    if (p_r6eco == NULL)
    {
        printf("FAIL R6Eco не инициализирован\n");
        return;
    }

    p_r6eco->setVcc(OFF);
    wait(TIME_DELAY::VCC_R6_OFF_DELAY_S);
    p_r6eco->setCtrl(ON);
    wait(TIME_DELAY::ONE_S);
    p_r6eco->setVcc(ON);
    wait(TIME_DELAY::THREE_S);
    p_r6eco->setCtrl(OFF);
    wait(TIME_DELAY::FIVE_S);
}

int8_t Gen6Basic::getOutHood()
{
    return p_r6->isLockSet();
}

int8_t Gen6Basic::getLockOpen()     
{
    if (p_in_central_lock_open == NULL)
    {
        return p_can_actions->waitLockEnabled(TIME_DELAY::ONE_S);
    }

    return getDigInByFunc(p_in_central_lock_open, ID_OUT_LOCK_B);
}

int8_t Gen6Basic::getLockClose()
{
    if (p_in_central_lock_close == NULL)
    {
        return p_can_actions->waitLockDisabled(TIME_DELAY::ONE_S);
    }

    return getDigInByFunc(p_in_central_lock_close, ID_OUT_LOCK_A);
}

void Gen6Basic::resetGen6Power()
{
    off();

    // ресет по питанию брелков на случай их зависания
    setLcdKeyringVcc(OFF);
    setLedKeyringVcc(OFF);

    // Если нажали сервисную 6 раз, переводим R6 в режим программирования
    // Выключается режим программирования путем ресета по питанию
    if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
    {
        p_r6->setVcc(OFF);
        p_r6eco->setVcc(OFF);
    }

    // отключаем регистрацию в сети
    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        p_gsm->disNetReg();
    }

    wait(TIME_DELAY::DELAY_AFTER_VCC_DIS_S);
    on();
    setLcdKeyringVcc(ON);
    setLedKeyringVcc(ON);

    if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
    {
        p_r6->setVcc(ON);
        p_r6eco->setVcc(ON);
    }

    // включаем регистрацию в сети (для исключения возможных зависаний Sim900)
    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        p_gsm->enNetReg();
    }

    wait(TIME_DELAY::MIN_DELAY_AFTER_VCC_EN_S);

    // Ждем ЦБ появления в GSM сети
    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        wait(TIME_DELAY::DELAY_AFTER_VCC_EN_S);
        p_gsm->clearSms();
    }
}

void Gen6Basic::resetAuthAndImmo(ign_statuses_t ign_status)
{
    // кол-во попыток до блокировки на 15 мин. ввода кода экстренного снятия
    const uint8_t c_disarm_attempts_num = 3;


    printf("INFO Сброс состояния авторизации/запрета поездки\n");
    setSecure(OFF, KEYRING);

    if (   getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                           SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                           DIAG_GUARD_STATE_IMMO)
        || getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                           SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                           DIAG_GUARD_STATE_WAITING_AUTH)
        || getSlpCmdStatus(DIAG_DATA_EEFLAG_SYSDATA,
                           SLP_STATUSES_SIZES::DIAG_DATA_EEFLAG_SYSDATA_SIZE,
                           DIAG_EEFLAG_SYSDATA::ID_FLAG_ANTI_HIJACK))
    {
        // Попробовать все способы авторизации
        if ((getModules() & BLE_MODULE) == BLE_MODULE)
        {
            setTagVcc();
        }

        if ((getModules() & R868_MODULE) == R868_MODULE)
        {
            setLcdKeyringVcc(ON);
        }

        sendValidatorCode();

        // Снятие запрета поездки происходит при вкл. зажигании
        setIgn(PIN, ON);
        setIgn(CAN_USE, ON);

        // Ожидаем, что система пройдет авторизацию
        if (waitGuardState(DIAG_GUARD_STATE_DISARM,
                           ON,
                           TIME_DELAY::MAX_WAITING_REGION_STATE_S))
        {
            printf("INFO Выход из ожидания авторизации/запрета поездки произведен\n");
        }
        else
        {
            printf("INFO Выход из ожидания авторизации/запрета поездки не произведен\n");
        }

        if (ign_status == IGN_OFF)
        {
            setIgn(PIN, OFF);
            setIgn(CAN_USE, OFF);
        }

        if ((getModules() & BLE_MODULE) == BLE_MODULE)
        {
            resetTagVcc();
        }
    } // if getSlpCmdStatus

    if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                         SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                         DIAG_GUARD_STATE_DISARM))
    {
        if (ign_status == IGN_OFF)
        {
            setIgn(PIN, OFF);
            setIgn(CAN_USE, OFF);
        }

        setSecure(ON, KEYRING);

        // на случай, если не все нажатия кнопки считались системой,
        // и система распознала ввод неверного кода экстренного сняти
        for (uint8_t i = 0; i < c_disarm_attempts_num; ++i)
        {
            emergencyDisarming(m_device_pin);

            if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                DIAG_GUARD_STATE_DISARM))
            {
                break;
            }
        }

        // если все еще не в снято - ввод кода уже заблокирован
        if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                             SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                             DIAG_GUARD_STATE_DISARM))
        {
            printf("INFO Код экстренного снятия заблокирован. Ожидаем разблокировки...\n");
            wait(TIME_DELAY::WAITING_UNBLOCKING_EMERGENCY_CODE_S);
        }

        if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
        {
            outputSms(TIME_DELAY::STD_WAITING_SMS_TIME_S);
        }
    } // if getSlpCmdStatus

    printf("INFO Сброс состояния авторизации/запрета поездки произведен\n");
}

void Gen6Basic::registerRemote(Gen6Basic *obj, reg_remote_func_t func)
{
    bool result = (obj->*func)();


    if (!result)
    {
        (obj->*func)();
    }
}

bool Gen6Basic::registerBtnAndRemote(registration_queue_mode_t reg_queue_mode)
{
    if (m_device_name != GEN6_M66)
    {
        if (!setAndCheckRegBit(reg_queue_mode))
        {
            return false;
        }

        registerValidatorCode();
        wait(TIME_DELAY::MIN_REGISTER_DEVICE_TIME_S);

        if ((m_device_modules & R868_MODULE) == R868_MODULE)
        {
            registerRemote(this, &Gen6Basic::registerLedKeyring);
            registerRemote(this, &Gen6Basic::registerLcdKeyring);
        }

        // для выхода из режима по SLP нужно включить-выключить зажигание
        wait(TIME_DELAY::MIN_REGISTER_DEVICE_TIME_S);
        setIgn(PIN, ON);
        setIgn(PIN, OFF);
        wait(TIME_DELAY::MIN_REGISTER_DEVICE_TIME_S);
    }

    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        // SMS, отправляемая после регистрации, влияет на прохождение тестов
        outputSms(TIME_DELAY::STD_WAITING_SMS_TIME_S);
    }

    return true;
}

bool Gen6Basic::waitStatusOut(status_out_t status_out,
                              float        time_s,
                              bool         status_out_state)
{
    switch (status_out)
    {
        case NO_STATUS_OUT:

            printf("INFO выход статуса метки не выбран\n");
            break;

        case STATUS_OUT_1:

            if (waitStateStatusOut(p_in_tag_status_1,
                                   ID_OUT_TAG_STATUS_1,
                                   status_out_state,
                                   time_s))               
            {
                printf("INFO Потенциал статусного выхода 1 соответствует ожидаемому\n");
                return true;
            }

            printf("INFO Потенциал статусного выхода 1 не соответствует ожидаемому\n");

            return false;

        case STATUS_OUT_2:

            if (waitStateStatusOut(p_in_tag_status_2,
                                   ID_OUT_TAG_STATUS_2,
                                   status_out_state,
                                   time_s))                 
            {
                printf("INFO Потенциал статусного выхода 2 соответствует ожидаемому\n");
                return true;
            }

            printf("INFO Потенциал статусного выхода 2 не соответствует ожидаемому\n");

            return false;

        case STATUS_OUT_3:

            if (waitStateStatusOut(p_in_tag_status_3,
                                   ID_OUT_TAG_STATUS_3,
                                   status_out_state,
                                   time_s))                 
            {
                printf("INFO Потенциал статусного выхода 3 соответствует ожидаемому\n");
                return true;
            }

            printf("INFO Потенциал статусного выхода 3 не соответствует ожидаемому\n");

            return false;
    }

    return false;
}

bool Gen6Basic::setAndCheckTag(bool tag_mode)
{
    if (tag_mode)
    {
        setTagVcc();

        printf("INFO Питание на метку подано\n");
    }
    else
    {
        resetTagVcc();

        printf("INFO Питание с метки снято\n");
    }

    return waitSlpCmdStatus(DIAG_DATA_FLAG,
                            SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                            DIAG_FLAG_BIT_TAG,
                            tag_mode,
                            TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);
}

bool Gen6Basic::setAndCheckSecure(bool                     secure_mode,
                                  command_type_t           execution_method,
                                  slp_diag_data_statuses_t arm_state,
                                  bool                     expect_state,
                                  float                    check_time_s)
{
    setSecure(secure_mode, execution_method, TIME_DELAY::NULL_DELAY_S);

    return waitGuardState(arm_state,
                          expect_state,
                          check_time_s);
}

bool Gen6Basic::setAndCheckIgn(int8_t         ign_value,
                               command_type_t execution_method,
                               float          delay_s)
{
    if (ign_value)
    {
        printf("INFO Отправлена команда на включение зажигания\n");
    }
    else
    {
        printf("INFO Отправлена команда на выключение зажигания\n");
    }

    setIgn(execution_method,
           ign_value,
           delay_s);

    return waitSlpCmdStatus(DIAG_DATA_PERIM,
                            SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                            DIAG_PERIM_BIT_IGN,
                            (bool)ign_value,
                            TIME_DELAY::STD_WIRE_EN_DELAY_S);
}

bool Gen6Basic::setAndCheckBreak(int8_t         value,
                                 command_type_t execution_method,
                                 float          delay_s)
{
    setBreak(value,
             execution_method,
             delay_s);

    return (value == ON ?  getSlpCmdStatus(DIAG_DATA_PERIM,
                                           SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                           DIAG_PERIM_BIT_BRAKE_PEDAL)
                        : !getSlpCmdStatus(DIAG_DATA_PERIM,
                                           SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                           DIAG_PERIM_BIT_BRAKE_PEDAL));
}

int8_t Gen6Basic::getIgn1()
{
    if (p_in_ign1 == NULL)
    {
        if (p_in_eng_start_stop_btn != NULL)
        {
            // инвертируем, т. к. ожидаем по-умолчанию 0-й потенциал на проводе
            // если ПЗ в 4F регионе включена - возвращает true
            return !getSupportIgn();
        }

        printf("FAIL Считывание состояния. Пин \"Зажигание 1\" не определен\n");
        return -1;
    }

    return *p_in_ign1;
}

int8_t Gen6Basic::checkStatusIn(DigitalIn *in_tag_status)
{
    if (in_tag_status == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Выход статуса метки\" не определен\n");
    
        return false;
    }

    return *in_tag_status;
}

bool Gen6Basic::waitStateStatusOut(DigitalIn              *in_tag_status,
                                   gen6_functional_t       status_out,
                                   bool                    status_out_state,
                                   float                   waiting_time_s)
{
    Timer timer;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    while (end < waiting_time_s)
    {
        if (status_out_state)
        {
            if (checkStatusIn(in_tag_status) == getPolarity(status_out))
            {
                return true;
            }
        }
        else
        {
            if (checkStatusIn(in_tag_status) != getPolarity(status_out))
            {
                return true;
            }
        }

        end = timer.read() - begin;
    }

    timer.stop();

    return false;
}

bool Gen6Basic::setAndCheckHandBreak(int8_t         hand_break_value,
                                     command_type_t execution_method,
                                     float          delay,
                                     float          waiting_time_s)
{
    if (hand_break_value)
    {
        printf("INFO Затягивание ручника\n");
    }
    else
    {
        printf("INFO Отпускание ручника\n");
    }

    setHandBreak(hand_break_value, execution_method, delay);

    return (waitSlpCmdStatus(DIAG_DATA_PERIM,
                             SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                             DIAG_PERIM_BIT_HAND_BRAKE,
                             hand_break_value,
                             waiting_time_s));
}

bool Gen6Basic::setAndCheckHandBreakCan(int8_t hand_break_value,
                                        float  delay)
{
    return setAndCheckHandBreak(hand_break_value,
                                CAN_USE,
                                delay);
}

bool Gen6Basic::setAndCheckParking(int8_t         parking_value,
                                   command_type_t execution_method,
                                   float          delay_s,
                                   float          waiting_time_s)
{
    if (parking_value)
    {
        printf("INFO Включение паркинга\n");
    }
    else
    {
        printf("INFO Выключение паркинга\n");
    }

    setParking(parking_value, execution_method, delay_s);

    return (waitSlpCmdStatus(DIAG_DATA_PERIM,
                             SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                             DIAG_PERIM_BIT_GEARBOX_P,
                             parking_value,
                             waiting_time_s));
}

bool Gen6Basic::setAndCheckParkingCan(int8_t parking_value,
                                      float  delay_s)
{
    return setAndCheckParking(parking_value, CAN_USE, delay_s);
}

bool Gen6Basic::setAndCheckEngine(int8_t         engine_value,
                                  command_type_t execution_method,
                                  int16_t        rpm_val,
                                  float          delay_s)
{
    setEngine(engine_value,
              execution_method,
              rpm_val,
              delay_s);

    return (engine_value == ON ?  getSlpCmdStatus(DIAG_DATA_FLAG, 
                                                  SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE, 
                                                  DIAG_FLAG_BIT_ENG_RUNNING)
                               : !getSlpCmdStatus(DIAG_DATA_FLAG, 
                                                  SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE, 
                                                  DIAG_FLAG_BIT_ENG_RUNNING));
}

bool Gen6Basic::prepAhjTag(command_type_t secure_method,
                           command_type_t ign_method,
                           command_type_t engine_method,
                           command_type_t hand_break_method,
                           command_type_t parking_method)
{
    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        p_lcd_keyring->offVcc();
        printf("INFO Выключение брелков\n");
    }
    else
    {
        if (!setAhjTagSetting(ON))
        {
            printf("INFO Антиограбление по метке не включено\n");
            return false;
        }
    }

    if (!setAndCheckSecure(OFF,
                           secure_method,
                           DIAG_GUARD_STATE_DISARM))
    {
        printf("INFO Охрана не выключена\n");
        return false;
    }

    if (!setAndCheckIgn(ON, ign_method))
    {
        printf("INFO Зажигание не включено\n");
        return false;
    }

    if (!setAndCheckEngine(ON, engine_method))
    {
        printf("INFO Двигатель не запущен\n");
        return false;
    }

    if (!setAndCheckHandBreak(OFF, hand_break_method))
    {
        printf("INFO Ручник не опущен\n");
        return false;
    }

    if (!setAndCheckParking(OFF, parking_method))
    {
        printf("INFO Паркинг не выключен\n");
        return false;
    }

    if ((getModules() & BLE_MODULE) == BLE_MODULE)
    {
        if (!setAndCheckTag(ON))
        {
            printf("INFO Метки нет в зоне видимости\n");
            return false;
        }
    }

    return true;
}

bool Gen6Basic::doAhj(command_type_t door_method)
{
    setIgn(PIN, ON);

    if ((getModules() & BLE_MODULE) == BLE_MODULE)
    {
        setTagVcc();
        wait(TIME_DELAY::MAX_TAG_VISIBLE_TIME_S);

        if (!setAndCheckTag(OFF))
        {
            printf("INFO Метка в зоне видимости\n");
            return false;
        }
    }
    else
    {
        p_lcd_keyring->offVcc();
        printf("INFO Выключение брелков\n");
    }

    setDoor(OPEN, door_method);
    setMoveSensor();
    
    return true;
}

bool Gen6Basic::waitEngineBlockCan(float time_s)
{
    wait_ms(TIME_DELAY::CAN_BLOCK_DELAY_MS);

    return p_can_actions->waitBlockEnabled(time_s);
}

void Gen6Basic::setAccelerometerAfterDelay(sensor_type_t sensor_type,
                                           float         delay_s)
{
    wait(delay_s);

    switch (sensor_type)
    {
        case SHOCK_L:
            setShockSensorLow();
            break;
    
        case SHOCK_H:
            setShockSensorHigh();
            break;

        case MOVE:
            setMoveSensor();
            break;

        case TILT:
            setTiltSensor();
            break;
    }
}

bool Gen6Basic::setAndCheckTrunk(int8_t           trunk_value,
                                 command_type_t   execution_method,
                                 invers_logic_t   logic_state,
                                 float            delay_s)
{
    if (logic_state == INVERSED)
    {
        setTrunkInvPin(trunk_value, delay_s);
    }
    else
    {
        setTrunk(trunk_value, execution_method, delay_s);
    }

    if (trunk_value == ON ?  getSlpCmdStatus(DIAG_DATA_PERIM,
                                             SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                             DIAG_PERIM_BIT_TRUNK)
                          : !getSlpCmdStatus(DIAG_DATA_PERIM,
                                             SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                             DIAG_PERIM_BIT_TRUNK))
    {
        return true;
    }

    return false;
}

bool Gen6Basic::checkTrunk(bool           status,
                           command_type_t execution_method,
                           float          timeout_s)
{
    bool   result;
    string status_str = status ? "открыт"
                               : "закрыт";


    printf("INFO Проверка состояния багажника\n");

    switch (execution_method)
    {
        case SLP:
            result = waitSlpCmdStatus(DIAG_DATA_PERIM,
                                      SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                      DIAG_PERIM_BIT_TRUNK,
                                      status,
                                      timeout_s);
            break;

        case CAN_USE:
            result = status ? p_can_actions->waitTrunkOpen(timeout_s)
                            : p_can_actions->waitTrunkClose(timeout_s);
            break;

        default:
            printf("FAIL Неизвестный тип проверки состояния багажника\n");
            result = false;
    }

    if (result)
    {
        printf("INFO Багажник %s\n", status_str.c_str());
    }
    else
    {
        printf("INFO Багажник не %s\n", status_str.c_str());
    }

    return result;
}

bool Gen6Basic::waitGearboxAutoBlock(float timeout_s)
{
    return p_can_actions->waitGearboxAutoBlock(timeout_s);
}

bool Gen6Basic::waitGearboxAutoUnblock(float timeout_s)
{
    return p_can_actions->waitGearboxAutoUnblock(timeout_s);
}

bool Gen6Basic::setAndCheckDoor(int8_t           door_value,
                                command_type_t   execution_method,
                                invers_logic_t   logic_state,  
                                float            delay_s,
                                float            wait_time_s)
{
    if (logic_state == INVERSED)
    {
        setDoorInvPin(door_value, delay_s);
    }
    else
    {
        setDoor(door_value, execution_method, delay_s);
    }

    return (waitSlpCmdStatus(DIAG_DATA_PERIM,
                             SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                             DIAG_PERIM_BIT_DOORS,
                             door_value,
                             wait_time_s));
}

bool Gen6Basic::setAndCheckHood(int8_t           hood_value,
                                command_type_t   execution_method,
                                invers_logic_t   logic_state,
                                float            delay_s)
{
    if (logic_state == INVERSED)
    {
        setHoodInvPin(hood_value, delay_s);
    }
    else
    {
        setHood(hood_value, execution_method, delay_s);
    }

    return (hood_value == OPEN ?  getSlpCmdStatus(DIAG_DATA_PERIM,
                                                  SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                                  DIAG_PERIM_BIT_HOOD)
                               : !getSlpCmdStatus(DIAG_DATA_PERIM,
                                                  SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                                                  DIAG_PERIM_BIT_HOOD));
}

bool Gen6Basic::waitBuzzerExt(bool  buzzer_state,
                              float waiting_time_s)
{
    if (p_in_buzzer_ext == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Выход внешнего звукового извещателя\" не определен\n");
    
        return false;
    }

    Timer timer;
    bool  buzz_in;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    while (end < waiting_time_s)
    {
        buzz_in = (bool)*p_in_buzzer_ext;

        if (buzzer_state)
        {
            if (buzz_in == getPolarity(ID_OUT_BUZZER_EXT))
            {
                return true;
            }
        }
        else
        {
            if (buzz_in != getPolarity(ID_OUT_BUZZER_EXT))
            {
                return true;
            }
        }

        end = timer.read() - begin;
    }

    timer.stop();

    return false;
}

 void Gen6Basic::setDoorInvPin(int8_t value, 
                               float  delay_s)
{
    const char* message = value ? "INFO Дверь закрыта(инверсия входа)\n" 
                                : "INFO Дверь открыта(инверсия входа)\n";
    printf("%s", message);

    if (p_out_door_inv == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Вход двери не определён\" не определен\n");
        return ;
    }

    (value == ON) ? *p_out_door_inv = 0
                  : *p_out_door_inv = 1;

    wait(delay_s);
}

 void Gen6Basic::setHoodInvPin(int8_t value, 
                               float  delay_s)
{
    const char* message = value ? "INFO Капот открыт(инверсия входа)\n" 
                                : "INFO Капот закрыт(инверсия входа)\n";
    printf("%s", message);

    if (p_out_hood_inv == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Вход капота не определён\" не определен\n");
        return ;
    }

    (value == OPEN) ? *p_out_hood_inv = 0
                    : *p_out_hood_inv = 1;

    wait(delay_s);
}

 void Gen6Basic::setTrunkInvPin(int8_t value, 
                                float  delay_s)
{
    const char* message = value ? "INFO Багажник закрыт(инверсия входа)\n" 
                                : "INFO Багажник открыт(инверсия входа)\n";
    printf("%s", message);

    if (p_out_trunk_inv == NULL)
    {
        printf("FAIL Считывание состояния. Пин \"Вход багажника не определён\" не определен\n");
        return ;
    }

    (value == ON) ? *p_out_trunk_inv = 0
                  : *p_out_trunk_inv = 1;

    wait(delay_s);
}

void Gen6Basic::setKillEngineInv(int8_t value,
                                 float  delay_s)
{
    if (p_out_killengine_inv == NULL)
    {
        printf("FAIL Запись состояния. Пин \"Вход запрета запуска двигателя с инверсией \" не определен\n");
        return;
    }

    const char* message = value ? "INFO Включение входа запрета запуска с инверсией\n" 
                                : "INFO Выключение входа запрета запуска с инверсией\n";
    printf("%s", message);

    (value == ON) ? *p_out_killengine_inv = 1
                  : *p_out_killengine_inv = 0;

    wait(delay_s);
}

void Gen6Basic::pushLcdKeyringBut33(float push_type_first,
                                    float push_type_second)
{
    printf("DEBUG Двойное нажатие кнопки 3 брелка\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG ЖК брелок не определен\n");
        return;
    }

    p_lcd_keyring->pushButton33(push_type_first, push_type_second);
}

void Gen6Basic::setDriverDoor(int8_t         value,
                              command_type_t execution_method,
                              float          delay_s)
{
    const char* message = value ? "INFO Дверь водителя открыта\n" 
                                : "INFO Дверь водителя закрыта\n"; 

    printf("%s", message);

    execution_method = getExecutionMethod(execution_method,
                                          CAR_ACTION,
                                          p_out_driver_door);

    switch (execution_method)
    {
        case CAN_USE:
            value ? p_can_actions->sendDoorOpen()
                  : p_can_actions->sendDoorClose();
            break;

        case PIN:
            if (p_out_driver_door == NULL)
            {
                printf("FAIL Запись состояния. Пин \"Вход двери водителя\" не определен\n");
                return;
            }

            (value == ON) ? *p_out_driver_door = 1
                          : *p_out_driver_door = 0;

            break;

        default:
            printf("FAIL Управление дверью водителя: метод управления не определен\n");
            break;
    }

    wait(delay_s);
}

void Gen6Basic::setPassengerDoor(int8_t         value,
                                 command_type_t execution_method,
                                 float          delay_s)
{
    const char* message = value ? "INFO Дверь пассажира открыта\n" 
                                : "INFO Дверь пассажира закрыта\n"; 

    printf("%s", message);

    execution_method = getExecutionMethod(execution_method,
                                          CAR_ACTION,
                                          p_out_passenger_door);

    switch (execution_method)
    {
        case CAN_USE:
            value ? p_can_actions->sendPassangerDoorOpen()
                  : p_can_actions->sendDoorClose();
            break;

        case PIN:
            if (p_out_passenger_door == NULL)
            {
                printf("FAIL Запись состояния. Пин \"Вход двери пассажира\" не определен\n");
                return;
            }

            (value == ON) ? *p_out_passenger_door = 1
                          : *p_out_passenger_door = 0;

            break;

        default:
            printf("FAIL Управление дверью пассажира: метод управления не определен\n");
            break;
    }

    wait(delay_s);
}

bool Gen6Basic::setAndCheckSpecialDoor(int8_t         door_value,
                                       command_type_t execution_method,
                                       special_door_t door_name,
                                       float          delay_s,
                                       float          wait_time_s)
{
    if (door_name)
    {
        setPassengerDoor(door_value, execution_method, delay_s);
    }
    else
    {
        setDriverDoor(door_value, execution_method, delay_s);
    }

    return (waitSlpCmdStatus(DIAG_DATA_PERIM,
                             SLP_STATUSES_SIZES::DIAG_DATA_PERIM_SIZE,
                             DIAG_PERIM_BIT_DOORS,
                             door_value,
                             wait_time_s));
}

bool Gen6Basic::waitTurboTimer(bool  tt_value,
                               float wait_time_s)
{

    return (waitSlpCmdStatus(DIAG_DATA_FLAG,
                             SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                             DIAG_FLAG_BIT_TT_RUNNING,
                             tt_value,
                             wait_time_s));
}

void Gen6Basic::setStdAlertMercedesCla(bool state)
{
    if (state)
    {
        printf("INFO Включение штатной тревоги по CAN\n");
        p_can_actions->sendAlertOnMercedesCla();
    }
    else
    {
        printf("INFO Выключение штатной тревоги по CAN\n");
        p_can_actions->sendAlertOffMercedesCla();
    }
}

void Gen6Basic::setIgnRenaultCaptur(bool status, float delay_s)
{
    printf("INFO Управление зажиганием на Renault Captur\n");


    if (status)
    {
        p_can_actions->sendIgnOnRenaultCapture();
        printf("INFO Зажигание включено\n");
    }
    else
    {
        p_can_actions->sendIgnOffRenaultCapture();
        printf("INFO Зажигание выключено\n");
    }

    wait(delay_s);
}

void Gen6Basic::setRpmRenaultCaptur(rpm_statuses_t value, float delay_s)
{
    printf("INFO Управление оборотами на Renault Captur\n");
    p_can_actions->sendSetRpmRenaultCapture(value);
    wait(delay_s);
}

void Gen6Basic::setRemoteRunRenaultCaptur(float delay_s)
{
    printf("INFO Дистанционный запуск Renault Captur\n");
    p_can_actions->sendRemoteRunRenaultCapture();
    wait(delay_s);
}

bool Gen6Basic::waitKlaxonLexusRx(float timeout_s)
{
    bool result;


    printf("INFO Ожидание пакетов включения клаксона для автомобиля Lexus RX, время ожидания: %.2f\n", timeout_s);

    result = p_can_actions->waitKlaxonOnLexusRx(timeout_s);

    if (result)
    {
        printf("INFO Пакеты получены\n");
    }
    else
    {
        printf("INFO Пакеты не получены\n");
    }

    return result;
}

bool Gen6Basic::setArmSuppIgn(arm_supp_trig_t          arm_supp_trig,
                              command_type_t           execution_method)
{
    setIgn(PIN, ON);
    setEngine(ON, PIN);
    setDoor(OPEN, PIN); 
    setHandBreak(ON, PIN);

    if (arm_supp_trig == IGN_SUPP_TRIG_BUTTON)
    {
        pushLcdKeyringBut2(KEYRING_SHORT);
    }

    wait(TIME_DELAY::ARM_SUPP_INIT_TIME_S);
    setIgn(PIN, OFF);
    setDoor(CLOSE, PIN); 

    if (!getSlpCmdStatus(DIAG_DATA_FLAG, 
                         SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                         DIAG_FLAG_BIT_ENG_RUNNING))
    {    
        printf("INFO Двигатель не работает\n");
        return false;
    }

    printf("INFO Двигатель работает\n");

    if (!setAndCheckSecure(ON,                                   
                           execution_method,
                           DIAG_GUARD_STATE_ARM))
    {
        printf("INFO Охрана не включена.\n");
        return false;
    }

    printf("INFO Охрана включена.\n");
    return true;
}

bool Gen6Basic::checkAlertStateAfterAccel(sensor_type_t            sensor_type,
                                          slp_diag_data_statuses_t arm_state,
                                          float                    delay_s)
{
    setAccelerometerAfterDelay(sensor_type, delay_s);

    return waitGuardState(arm_state,
                          ON,
                          TIME_DELAY::ALARM_SHOCK_LOW_TIME_S);
}

bool Gen6Basic::resetDiagDataPerim()
{
    return p_slp->setSlpStatus(DIAG_DATA_PERIM, 0x00);
}

void Gen6Basic::setGlowPlugs(out_statuses_t status, float delay_s)
{
    const std::string status_str = status == ON ? "включен" 
                                                : "выключен";


    printf("INFO Выход \"Контроль свечей накала\" %s\n", status_str.c_str());

    if (p_out_glowplugs == NULL)
    {
        printf("INFO Пин \"Контроль свечей накала\" не определен, управление произведено не будет\n");
        return;
    }

    *p_out_glowplugs = status == ON;

    wait(delay_s);
}

bool Gen6Basic::setAndCheckGlowPlugs(out_statuses_t status, float timeout_s)
{
    setGlowPlugs(status, TIME_DELAY::NULL_DELAY_S);

    return waitSlpCmdStatus(DIAG_DATA_FLAG_SYSDATA,
                            SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SYSDATA_SIZE,
                            DIAG_FLAG_SYSDATA::ID_FLAG_GLOWPLUGS,
                            status,
                            timeout_s);
}

bool Gen6Basic::checkSettingValue(const char     *setting_name,
                                  const char     *setting_value,
                                  setting_type_t  setting_type)
{
    std::string setting_type_str;                           // Тип настройки в текстовом виде
    char        buf[LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN]; // Буфер для входных данных


    if (   setting_name  == NULL
        || setting_value == NULL)
    {
        printf("INFO Имя или значение настройки не задано\n");

        return false;
    }

    switch (setting_type)
    {
        case SETTING_TYPE_INT:
            setting_type_str = "INT";
            break;

        case SETTING_TYPE_STR:
            setting_type_str = "STR";
            break;

        default:
            printf("INFO Тип настройки неизвестен: %d\n", setting_type);

            return false;
    }

    memset(buf,
           0,
           LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN);

    printf("INFO Отправка команды на проверку значения настройки: %s\n", setting_name);
    printf("GETSETTING %s %s %s \n",
           setting_name,
           setting_value,
           setting_type_str.c_str());

    if (UartInteract::getString(p_serial,
                                buf,
                                TIME_DELAY::MAX_SLP_CMD_ANSW_DELAY_S))
    {
        if (strstr(buf, "GETSETTING_OK") != NULL)
        {
            printf("INFO Значение настройки соответствует ожидаемому\n");

            return true;
        }
    }

    printf("INFO Значение настройки не соответствует ожидаемому\n");

    return false;
}

bool Gen6Basic::waitSideMirrorsCan(mirrors_statuses_t mirrors_status, float timeout_s)
{
    bool result = false;


    printf("INFO Ожидание пакетов (CAN) разложения/сложения зеркал, время ожидания: %.2f\n", timeout_s);

    switch (mirrors_status)
    {
        case MIRRORS_OPEN:
            printf("INFO Ожидание разложения зеркал\n");

            result = p_can_actions->waitSideMirrorsOpen(timeout_s);
            break;

        case MIRRORS_CLOSE:
            printf("INFO Ожидание сложения зеркал\n");

            result = p_can_actions->waitSideMirrorsClose(timeout_s);
            break;

        case MIRRORS_NONE:
            printf("INFO Проверка отсутствия управления зеркалами\n");

            result = p_can_actions->waitNoSideMirrors(timeout_s);
            break;

        default:
            printf("INFO Неизвестный тип проверки зеркал: %d\n", mirrors_status);
    }

    return result;
}

bool Gen6Basic::waitComfortCan(out_statuses_t status, float timeout_s)
{
    bool result;


    if (status == ON)
    {
        printf("INFO Ожидание включения функции Комфорт (CAN)\n");

        result = p_can_actions->waitComfortOn(timeout_s);
    }
    else
    {
        printf("INFO Ожидание отключения функции Комфорт (CAN)\n");

        result = p_can_actions->waitComfortOff(timeout_s);
    }

    if (result)
    {
        printf("INFO Ожидаемые пакеты функции Комфорт (CAN) получены\n");
    }
    else
    {
        printf("INFO Ожидаемые пакеты функции Комфорт (CAN) не получены\n");
    }

    return result;
}

bool Gen6Basic::waitSunroofCanLexusGx(doors_statuses_t sunroof_status, float timeout_s)
{
    bool result;


    printf("INFO Ожидание пакетов (CAN) открытия/закрытия люка на крыше для автомобиля Lexus GX, время ожидания: %.2f\n", timeout_s);


    if (sunroof_status == OPEN)
    {
        printf("INFO Ожидание открытия люка\n");
        printf("FAIL Функция не поддерживается\n");

        result = false;
    }
    else
    {
        printf("INFO Ожидание закрытия люка\n");

        result = p_can_actions->waitSunroofCloseLexusGx(timeout_s);
    }

    if (result)
    {
        printf("INFO Ожидаемые пакеты (CAN) получены\n");
    }
    else
    {
        printf("INFO Ожидаемые пакеты (CAN) не получены\n");
    }

    return result;
}

bool Gen6Basic::waitInspectorCan(float timeout_s)
{
    bool result;


    printf("INFO Ожидание пакетов включения штатного обходчика CAN\n");

    result = p_can_actions->waitInspector(timeout_s);

    if (result)
    {
        printf("INFO Пакеты получены\n");
    }
    else
    {
        printf("INFO Пакеты не получены\n");
    }

    return result;
}

bool Gen6Basic::waitObdErrorRequest(float timeout_s)
{
    bool result;


    printf("INFO Ожидание сообщений запросов OBD ошибок\n");

    result = p_can_actions->waitObdErrorRequest(timeout_s);

    if (result)
    {
        printf("INFO Сообщения (пакеты CAN) получены\n");
    }
    else
    {
        printf("INFO Сообщения (пакеты CAN) не получены\n");
    }

    return result;
}

bool Gen6Basic::connectMobile()
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    result = p_mobileapp->connect();

    return result;
}

bool Gen6Basic::disconnectMobile()
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    result = p_mobileapp->disconnect();

    return result;
}

bool Gen6Basic::clearMobileBuf()
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    result = p_mobileapp->clearBuf();

    return result;
}

bool Gen6Basic::getMobileStatusCurState(mobileapp_cur_state_shift_t shift,
                                        int32_t                     status,
                                        mobileapp_read_type_t       read_type)
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    result = p_mobileapp->getMobileStatusCurState(shift,
                                                  status,
                                                  read_type);

    return result;
}

bool Gen6Basic::setMobileStatus(mobileapp_user_cmd_t command)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    return p_mobileapp->setMobileStatus(command);
}

bool Gen6Basic::setServiceModeMobile(out_statuses_t status)
{
    bool result;


    printf("INFO Управление сервисным режимом через мобильное приложение (BLE)\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    if (status == ON)
    {
        printf("INFO Включение сервисного режима\n");

        result = p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_SERVICE_ON);
    }
    else
    {
        printf("INFO Отключение сервисного режима\n");

        result = p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_SERVICE_OFF);
    }

    return result;
}

void Gen6Basic::setAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type)
{
    printf("INFO Выполнение алгоритма включения режима альтернативной сервисной кнопки\n");

    switch (alt_srv_btn_type)
    {
        case ALT_SRV_BTN_TYPE_DOOR:
        case ALT_SRV_BTN_TYPE_IGN:

            for (uint8_t i = 0; i < COUNT_STRUCT::DOOR_MODE_ALT_SRV_BTN_OPEN_COUNT; ++i)
            {
                setDoor(OPEN,
                        PIN,
                        TIME_DELAY::MIN_WIRE_EN_DELAY_S);
                setDoor(CLOSE,
                        PIN,
                        TIME_DELAY::MIN_WIRE_EN_DELAY_S);
            }

            if (!setAndCheckDoor(OPEN, PIN))
            {
                printf("INFO Дверь не открыта\n");
            }

            break;

        case ALT_SRV_BTN_TYPE_STD_BTN:

            if (!setAndCheckIgn(ON,
                                PIN,
                                TIME_DELAY::STD_WIRE_EN_DELAY_S))
            {
                printf("INFO Зажигание не включено\n");
            }

            break;

        default:
            printf("INFO Неизвестный тип альтернативной сервисной кнопки: %d\n", alt_srv_btn_type);
            return;
    }

    printf("INFO Алгоритм выполнен\n");
}

bool Gen6Basic::setAndCheckAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type)
{
    bool lights_polarity = getPolarity(ID_OUT_LIGHTS); // Полярность выхода световых сигналов


    printf("INFO Включение и проверка режима альтернативной сервисной кнопки\n");
    setAltSrvBtnModeEn(alt_srv_btn_type);

    if (alt_srv_btn_type != ALT_SRV_BTN_TYPE_STD_BTN) // Экстренное снятия штатной кнопкой включается путем вкл. зажигания
                                                      // Поэтому световые сигналы не будут отключены (т.к. для системы это будет включением зажигания до набора кода)
    {
        if (    waitLights(TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S, !lights_polarity)  // Световые сигналы отключены
            && !waitLights(TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S,  lights_polarity)) // Световые сигналы не включились
        {
            printf("INFO Световые сигналы отключились\n");
        }
        else
        {
            printf("INFO Световые сигналы не отключены\n");
            return false;
        }
    }

    return true;
}

bool Gen6Basic::clickAltSrvBtn(alt_srv_btn_type_t alt_srv_btn_type,
                               uint8_t            count)
{
    printf("INFO Нажатие альтернативной сервисной кнопки, количество нажатий: %d\n", count);

    switch (alt_srv_btn_type)
    {
        // Дверь. Нажатие кнопки: закрыть и открыть дверь (дверь открыта после нажатия)
        case ALT_SRV_BTN_TYPE_DOOR:
            printf("INFO Нажатие альтернативной сервисной кнопки: дверь\n");

            for (uint8_t i = 0; i < count; ++i)
            {
                setDoor(CLOSE, PIN); // Без проверки по SLP, чтобы уложиться в таймаут
                setDoor(OPEN, PIN);
            }

            break;

        // Зажигание. Нажатие кнопки: включить и выключить зажигание (зажигание отключено после нажатия)
        case ALT_SRV_BTN_TYPE_IGN:
            printf("INFO Нажатие альтернативной сервисной кнопки: зажигание\n");

            for (uint8_t i = 0; i < count; ++i)
            {
                setIgn(PIN,                              // Без проверки по SLP, чтобы уложиться в таймаут
                       ON,
                       TIME_DELAY::STD_WIRE_EN_DELAY_S);
                setIgn(PIN,
                       OFF,
                       TIME_DELAY::STD_WIRE_EN_DELAY_S);
            }

            break;

        // Штатная кнопка. Нажатие сервисной кнопки: нажатие штатной кнопки "Да" (Автомобиль: 9999 Batmobile)
        case ALT_SRV_BTN_TYPE_STD_BTN:
            printf("INFO Нажатие альтернативной сервисной кнопки: штатная кнопка автомобиля\n");
            p_can_actions->sendBtnOff();

            for (uint8_t i = 0; i < count; ++i)
            {
                p_can_actions->sendBtn(CAN_BTN4_OK);
                wait(TIME_DELAY::MAX_CAN_BTN_CLICK_DELAY_S);
            }

            break;

        default:
            printf("INFO Неизвестный тип альтернативной сервисной кнопки: %d\n", alt_srv_btn_type);
            return false;
    }

    return true;
}

bool Gen6Basic::setAndCheckAltSrvBtnArm(alt_srv_btn_type_t alt_srv_btn_type)
{
    float arm_en_err_time_s; // Погрешность включения охраны


    printf("INFO Включение охраны через альтернативную сервисную кнопку\n");


    if (alt_srv_btn_type == ALT_SRV_BTN_TYPE_IGN)
    {
        printf("INFO Невозможно выполнить постановку в охрану через альт. сервисную кнопку \"зажигание\"\n");
        return false;
    }

    if (alt_srv_btn_type == ALT_SRV_BTN_TYPE_DOOR)
    {
        if (!setAndCheckDoor(OPEN, PIN))
        {
            printf("INFO Дверь не открыта\n");
            return false;
        }
    }

    if (!setAndCheckIgn(ON,
                        PIN,
                        TIME_DELAY::STD_WIRE_EN_DELAY_S))
    {
        printf("INFO Зажигание не включилось (первое включение)\n");
        return false;
    }

    clickAltSrvBtn(alt_srv_btn_type, COUNT_STRUCT::ALT_SRV_BTN_MODE_ARM_CLICKS_COUNT);

    if (!setAndCheckIgn(OFF,
                        PIN,
                        TIME_DELAY::STD_WIRE_EN_DELAY_S))
    {
        printf("INFO Зажигание не отключилось (первое выключение)\n");
        return false;
    }

    setIgn(PIN,                       // Без проверки по SLP, т.к. нужно сразу проверять сирену
           ON,
           TIME_DELAY::NULL_DELAY_S);

    if (alt_srv_btn_type != ALT_SRV_BTN_TYPE_DOOR)
    {
        wait_s(TIME_DELAY::STD_WIRE_EN_DELAY_S);                                           // Чтобы не моментально нажимать кнопки после вкл. зажигания
        clickAltSrvBtn(alt_srv_btn_type, COUNT_STRUCT::ALT_SRV_BTN_MODE_ARM_CLICKS_COUNT);
    }

    // На системах без буззера - оповещение о постановке выводится на сирену (аналогично постановке через обычную сервисную кнопку)
    if (m_device_name != GEN6_X96V2)
    {
        if (isChFunctionalSet(ID_OUT_SIREN))
        {
            if (!waitSiren(TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S, getPolarity(ID_OUT_SIREN)))
            {
                printf("INFO Сирена не включилась\n");
                return false;
            }

            for (uint8_t i = 0; i < COUNT_STRUCT::ALT_SRV_BTN_ARM_EN_SIGNALS_COUNT_TOTAL; ++i)
            {
                setPolarity(ID_OUT_SIREN);
                
                if (!checkPulseWithoutWidth(ID_OUT_SIREN, TIME_DELAY::MAX_WIRE_EN_DELAY_S))
                {
                    printf("INFO %d-й сигнал сирены не получен\n", i + 1);
                    return false;
                }
            }

            arm_en_err_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S;
        }
        else
        {
            printf("INFO Канал \"Сирена\" не настроен, ожидание конца времени оповещения\n");
            wait_s(TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S);

            arm_en_err_time_s = TIME_DELAY::WAITING_REGION_STATE_S;
        }
    }
    else
    {
        if (isChFunctionalSet(ID_OUT_BUZZER_EXT))
        {
            if (!waitBuzzerExt(ON, TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
            {
                printf("INFO Внешний звукоизвещатель не включился\n");
                return false;
            }

            for (uint8_t i = 0; i < COUNT_STRUCT::BEACH_EN_SIGNALS_COUNT_TOTAL; ++i)
            {
                if (!(   waitBuzzerExt(ON,  TIME_DELAY::MAX_WIRE_EN_DELAY_S)
                      && waitBuzzerExt(OFF, TIME_DELAY::SIREN_DIS_WAIT_TIME_S)))
                {
                    printf("INFO %d-й сигнал внешнего звукоизвещателя не получен\n", i + 1);
                    return false;
                }
            }

            arm_en_err_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S;
        }
        else
        {
            printf("INFO Канал \"Внешний звукоизвещатель\" не настроен, ожидание конца времени оповещения\n");
            wait_s(TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S);

            arm_en_err_time_s = TIME_DELAY::WAITING_REGION_STATE_S;
        }
    } // if (m_device_name != GEN6_X96V2)

    if (!setAndCheckIgn(OFF,
                        PIN,
                        TIME_DELAY::STD_WIRE_EN_DELAY_S))
    {
        printf("INFO Зажигание не отключилось (второе выключение)\n");
        return false;
    }

    if (alt_srv_btn_type == ALT_SRV_BTN_TYPE_DOOR)
    {
        if (!setAndCheckDoor(CLOSE, PIN))
        {
            printf("INFO Дверь не закрыта\n");
            return false;
        }
    }

    if (waitGuardState(DIAG_GUARD_STATE_ARM,
                       ON,
                       TIME_DELAY::ALT_SRV_BTN_ARM_EN_DELAY_S,
                       arm_en_err_time_s))
    {
        printf("INFO Охрана включена\n");
    }
    else
    {
        printf("INFO Охрана не включилась\n");
        return false;
    }

    return true;
}

bool Gen6Basic::setAltSrvBtnDisarm(alt_srv_btn_type_t  alt_srv_btn_type,
                                   const char         *pin)
{
    bool    lights_polarity; // Полярность выхода световых сигналов
    uint8_t pin_num;         // Очередная цифра кода экстренного снятия


    printf("INFO Ввод кода экстренного снятия через альтернативную сервисную кнопку\n");

    if (isChFunctionalSet(ID_OUT_LIGHTS))
    {
        lights_polarity = getPolarity(ID_OUT_LIGHTS);
    }

    if (pin == NULL)
    {
        pin = m_device_pin;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        pin_num = pin[i] - 0x30; // Преобразовать символ цифры в цифру, согласно ASCII

        printf("INFO Ввод %d-й цифры кода, цифра: %d\n",
               i + 1,
               pin_num);
        clickAltSrvBtn(alt_srv_btn_type, pin_num);

        if (isChFunctionalSet(ID_OUT_LIGHTS))
        {
            // При вводе первый цифры кода штатной кнопкой сначала нужно дождаться отключения световых сигналов
            if (   i                == 0
                && alt_srv_btn_type == ALT_SRV_BTN_TYPE_STD_BTN)
            {
                printf("INFO Ожидание отключения световых сигналов после ввода первой цифры штатной кнопкой\n");
                wait_s(  TIME_DELAY::ALT_SRV_BTN_AFTER_PIN_NUM_DELAY_S
                       - TIME_DELAY::MAX_CAN_BTN_CLICK_DELAY_S              // Время задержки после нажатия штатной кнопки
                       - TIME_DELAY::MIN_ERROR_TIME_S);                     // Погрешность

                if (!waitLights(TIME_DELAY::MAX_WIRE_EN_DELAY_S, !lights_polarity))
                {
                    printf("INFO Световые сигналы не отключились при вводе кода штатной кнопкой\n");
                    return false;
                }
            }

            if (waitLights(TIME_DELAY::ALT_SRV_BTN_ALARM_LIGHTS_WAIT_TIME_S, lights_polarity))
            {
                printf("INFO Получен световой сигнал после ввода %d-й цифры\n", i + 1);

                if (!waitLights(TIME_DELAY::LIGHTS_DIS_WAIT_TIME_S, !lights_polarity))
                {
                    printf("INFO Потенциал света не снят (поворотники не затухают)\n");
                    return false;
                }

                // Проверка световых сигналов после ввода последней цифры
                if (i == 3)
                {
                    // Получено подтверждение светом после ввода последней цифры неверного кода (а не должно)
                    if (strstr(pin, m_device_pin) == NULL)
                    {
                        return false;
                    }

                    // Проверка получения второго и третьего светового сигнала
                    for (uint8_t z = 2; z <= COUNT_STRUCT::ALT_SRV_BTN_DISARM_LIGHTS_COUNT; ++z)
                    {
                        if (   waitLights(TIME_DELAY::BTN_CMD_SUCCESS_LIGHTS_WAIT_TIME_S,  lights_polarity)
                            && waitLights(TIME_DELAY::LIGHTS_DIS_WAIT_TIME_S,             !lights_polarity))
                        {
                            printf("INFO Получен %d-й световой сигнал после ввода последней цифры\n", z);
                        }
                        else
                        {
                            printf("INFO %d-й световой сигнал не получен после ввода последней цифры\n", z);
                            return false;
                        }
                    } // for
                } // if i == 3

                wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S); // Чтобы не моментально вводить следующую цифру после получения сигнала света
            }
            else
            {
                printf("INFO Световые сигналы не получены после ввода %d-й цифры\n", i + 1);

                // Подтверждение светом отсутствует только если введена последня цифра неверного кода (т.к. снятие не произойдет)
                // В остальных случаях - должно быть подтверждение светом
                if (   i                         != 3
                    || strstr(pin, m_device_pin) != NULL)
                {
                    return false;
                }
            } // if waitLights
        }
        else
        {
            printf("INFO Канал \"Световые сигналы: общий\" не настроен, ожидание разрешения ввода следующей цифры\n");
            wait_s(TIME_DELAY::ALT_SRV_BTN_AFTER_PIN_NUM_DELAY_S + TIME_DELAY::MAX_WIRE_EN_DELAY_S);
        } // if p_out_lights != NULL
    } // for

    printf("INFO Успешная проверка ввода кода экстренного снятия через альтернативную сервисную кнопку\n");

    return true;
}

bool Gen6Basic::getSlpPolarity()
{
    bool result;


    if (isChFunctionalSet(ID_OUT_SLP_V2))
    {
        result = getPolarity(ID_OUT_SLP_V2);
    }
    else
    {
        result = getPolarity(ID_OUT_SLP_V1);
    }

    return result;
}

bool Gen6Basic::checkImmoDisDelayAfterAcc(float immo_delay_s,
                                          float err_time_s,
                                          float timeout_s)
{
    Timer      timer;
    DigitalIn *in_acc;                                      // Вход аксессуаров (либо с выкл. при работе стартера, либо без выкл. при работе стартера)

    bool       immo_polarity;                               // Полярность обходчика
    bool       acc_polarity;                                // Полярность аксессуаров

    float      acc_dis_time_s  = TIME_DELAY::NULL_DELAY_S;  // Время отключения аксессуаров
    float      immo_dis_time_s = TIME_DELAY::NULL_DELAY_S;; // Время отключения обходчика

    float      immo_after_acc_delay_s;                      // Полученная задержка отключения обходчика после выключения аксессуаров


    if (isChFunctionalSet(ID_OUT_ACC_EXC))
    {
        in_acc       = p_in_acc_axc;
        acc_polarity = getPolarity(ID_OUT_ACC_EXC);
    }
    else if (isChFunctionalSet(ID_OUT_ACC))
    {
        in_acc       = p_in_acc;
        acc_polarity = getPolarity(ID_OUT_ACC);
    }
    else
    {
        printf("INFO Выход \"Запуск двигателя: аксессуары\" не назначен\n");
        return false;
    }

    if (isChFunctionalSet(ID_OUT_IMMO))
    {
        immo_polarity = getPolarity(ID_OUT_IMMO);
    }
    else
    {
        printf("INFO Выход \"Управление потенциалом модуля обхода штатного иммобилайзера\" не назначен\n");
        return false;
    }

    printf("INFO Проверка задержки отключения обходчика после выключения аксессуаров, ожидаемая задержка: %.6f, погрешность: %.6f\n",
           immo_delay_s,
           err_time_s);

    timer.start();

    // Ждем отключения аксессуаров
    while (timer.read() < timeout_s)
    {
        if (*in_acc == !acc_polarity)
        {
            acc_dis_time_s = timer.read();

            printf("INFO Аксессуары отключены через: %.6f сек.\n", acc_dis_time_s);
            break;
        }
    }

    // Ждем отключения обходчика
    while (timer.read() < timeout_s)
    {
        if (*p_in_inspector == !immo_polarity)
        {
            immo_dis_time_s = timer.read();

            printf("INFO Обходчик отключен через: %.6f сек.\n", immo_dis_time_s);
            break;
        }
    }

    timer.stop();

    if (   immo_dis_time_s == TIME_DELAY::NULL_DELAY_S
        || acc_dis_time_s  == TIME_DELAY::NULL_DELAY_S)
    {
        printf("INFO Аксессуары или обходчик не отключились\n");
        return false;
    }

    immo_after_acc_delay_s = immo_dis_time_s - acc_dis_time_s;

    printf("INFO Обходчик отключен через %.6f сек. с момента отключения аксессуаров\n", immo_after_acc_delay_s);

    if (fabs(immo_after_acc_delay_s - immo_delay_s) > err_time_s)
    {
        printf("INFO Обходчик отключен вне ожидаемого времени погрешности\n");
        return false;
    }

    printf("INFO Проверка обходчика успешна\n");

    return true;
}

bool Gen6Basic::openDebugRegion(slp_open_type_t slp_open_type)
{
    bool result;


    printf("INFO Открытие региона для отладки\n");

    result = p_slp->openDebugRegion(slp_open_type);

    return result;
}

bool Gen6Basic::openEventsRegion()
{
    bool result;


    printf("INFO Открытие региона для чтения событий\n");

    result = p_slp->openEventsRegion();

    return result;
}

bool Gen6Basic::closeRegion()
{
    bool result;


    printf("INFO Закрытие региона\n");

    result = p_slp->closeRegion();

    return result;
}

bool Gen6Basic::waitSlpEvents(const std::vector<slp_user_event_t> &event_ids, float timeout_s)
{
    bool result;


    printf("INFO Ожидание наступления событий в регионе\n");

    result = p_slp->waitEvents(event_ids, timeout_s);

    return result;
}

void Gen6Basic::setDismantlingSensor(out_statuses_t value,
                                     command_type_t execution_method,
                                     invers_logic_t logic_type,
                                     float          delay_s)
{
    bool sensor_value; // Устанавливаемое значение датчика демонтажа


    if (value == ON)
    {
        printf("INFO Включение датчика демонтажа\n");

        sensor_value = 1;
    }
    else
    {
        printf("INFO Выключение датчика демонтажа\n");

        sensor_value = 0;
    }

    execution_method = getExecutionMethod(execution_method,
                                          CAR_ACTION,
                                          p_out_dismantling_sensor);

    switch (execution_method)
    {
        case PIN:

            if (logic_type == NORMAL)
            {
                *p_out_dismantling_sensor = sensor_value;
            }
            else
            {
                *p_out_dismantling_sensor_inv = !sensor_value;
            }

            break;

        default:
            printf("FAIL Неподдерживаемый метод включения датчика демонтажа\n");
    }

    wait(delay_s);
}

bool Gen6Basic::setAndCheckDismantlingSensor(out_statuses_t value,
                                             command_type_t execution_method,
                                             invers_logic_t logic_type,
                                             float          wait_time_s)
{
    setDismantlingSensor(value,
                         execution_method,
                         logic_type,
                         TIME_DELAY::NULL_DELAY_S);

    return waitSlpCmdStatus(DIAG_DATA_ZONE_SYSDATA,
                            SLP_STATUSES_SIZES::DIAG_DATA_ZONE_SYSDATA_SIZE,
                            SYSDATA_ID_ZONE_DISMANTLING_SENSOR,
                            value,
                            wait_time_s);
}

void Gen6Basic::setCanExtBtnAudiA7(out_statuses_t value, float delay_s)
{
    if (value == ON)
    {
        printf("INFO Включение (зажатие) внешней кнопки CAN (Audi A7)\n");

        p_can_actions->sendExtBtnOnAudiA7();
    }
    else
    {
        printf("INFO Выключение (отпускание) внешней кнопки CAN (Audi A7)\n");

        p_can_actions->sendExtBtnOffAudiA7();
    }

    wait(delay_s);
}

bool Gen6Basic::checkSrvLedAndSiren(uint8_t pulses_count, float timeout_s)
{
    const float err_between_pulse_s    = TIME_DELAY::MIN_ERROR_TIME_S; // Погрешность между одновременным включением сирены и сервисного светодиода
    const bool  siren_polarity         = getPolarity(ID_OUT_SIREN);    // Полярность сирены

    bool        is_led_potential       = false;                        // Получен ли потенциал сервисного светодиода
    bool        is_siren_potential     = false;                        // Получен ли потенциал сирены

    uint8_t     led_pulse_count        = 0;                            // Кол-во полученных импульсов сервисного светодиода
    uint8_t     siren_pulse_count      = 0;                            // Кол-во полученных импульсов сирены

    float       led_potential_time_s   = TIME_DELAY::NULL_DELAY_S;     // Время получения потенциала сервисного светодиода
    float       siren_potential_time_s = TIME_DELAY::NULL_DELAY_S;     // Время получения потенциала сирены

    float       led_pulse_time_s       = TIME_DELAY::NULL_DELAY_S;     // Время получения импульса сервисного светодиода
    float       siren_pulse_time_s     = TIME_DELAY::NULL_DELAY_S;     // Время получения импульса сирены

    float       led_and_siren_diff_s   = TIME_DELAY::NULL_DELAY_S;     // Разница между получением сигнала (импульса или потенциала) сервисного светодиода и сирены

    Timer       timer;


    printf("INFO Ожидание одновременных сигналов сервисного светодиода и сирены. Кол-во сигналов: %u, время ожидания: %.6f сек.\n",
           pulses_count,
           timeout_s);
    timer.start();

    while (timer.read() < timeout_s)
    {
        // Проверка сервисного светодиода
        if (   !is_led_potential
            &&  p_service_btn->isLedOn())
        {
            printf("TRACE Получен потенциал сервисного светодиода через: %.6f\n", timer.read());

            is_led_potential     = true;
            led_potential_time_s = timer.read();
        }

        if (    is_led_potential
            && !p_service_btn->isLedOn())
        {
            ++led_pulse_count;
            is_led_potential = false;
            led_pulse_time_s = timer.read();

            printf("INFO Получен %u-й сигнал сервисного светодиода через %.6f сек.\n",
                   led_pulse_count,
                   timer.read());
        }

        // Проверка сирены
        if (   !is_siren_potential
            &&  getSiren() == siren_polarity)
        {
            printf("TRACE Получен потенциал сирены через: %.6f сек.\n", timer.read());

            is_siren_potential     = true;
            siren_potential_time_s = timer.read();
        }

        if (   is_siren_potential
            && getSiren() == !siren_polarity)
        {
            ++siren_pulse_count;
            is_siren_potential = false;
            siren_pulse_time_s = timer.read();

            printf("INFO Получен %u-й сигнал сирены через %.6f сек.\n",
                   led_pulse_count,
                   timer.read());
        }

        // Проверка одновременного включения сирены и сервисного светодиода
        if (   is_led_potential
            && is_siren_potential)
        {
            led_and_siren_diff_s = fabs(led_potential_time_s - siren_potential_time_s);

            if (led_and_siren_diff_s > err_between_pulse_s)
            {
                printf("INFO Разница между включением сигналами светодиода и сирены: %.6f сек.\n", led_and_siren_diff_s);

                return false;
            }
        }

        if (   !is_led_potential
            && !is_siren_potential)
        {
            led_and_siren_diff_s = fabs(led_pulse_time_s - siren_pulse_time_s);

            if (led_and_siren_diff_s > err_between_pulse_s)
            {
                printf("INFO Разница между выключением светодиода и сирены: %.6f сек.\n", led_and_siren_diff_s);

                return false;
            }
        }
    } // while timer.read() < timeout_s

    timer.stop();

    if (   led_pulse_count   == pulses_count
        && siren_pulse_count == pulses_count)
    {
        printf("INFO Все сигналы сервисного светодиода и сирены были получены\n");
    }
    else
    {
        printf("INFO Кол-во сигналов сервисного светодиода или сирены не соответствует ожидаемому\n");

        return false;
    }

    return true;
}

bool Gen6Basic::waitEngineStatus(out_statuses_t status, float timeout_s)
{
    return waitSlpCmdStatus(DIAG_DATA_FLAG, 
                            SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                            DIAG_FLAG_BIT_ENG_RUNNING,
                            status,
                            timeout_s);
}

bool Gen6Basic::setSettingsMobile(const SlmSettings &settings)
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    result = p_mobileapp->setSettings(settings);

    return result;
}

bool Gen6Basic::checkSettingsMobile(const SlmSettings &settings)
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO Отсутствует BLE модуль для работы с мобильным приложением (BLE)\n");

        return false;
    }

    result = p_mobileapp->checkSettings(settings);

    return result;
}
