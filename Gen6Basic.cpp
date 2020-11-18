/*!
 *  \file      Gen6Basic.cpp
 *
 *  \brief     �������� ���������� ������ Gen6Basic
 *
 */

//! ����������� ������������ ������

#include "Gen6Basic.h"

//! ���������� ������

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
    printf("INFO ��������� ��������� �����\n");
    *p_vcc = ON;
}

void Gen6Basic::off()
{
    printf("INFO ���������� ��������� �����\n");
    *p_vcc = OFF;
}

void Gen6Basic::sigmaSettingMode()
{
    printf("INFO ������� ACK-��� � ����� ������ ��������\n");

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
    printf("INFO ������� ACK-��� � ����� ������\n");

    if (m_device_name == GEN6_S96)
    {
        *p_sigma_enable = 0;
    }
    else
    {
        *p_sigma_enable = 1;
    }

    wait_s(TIME_DELAY::THREE_S);
    
    // ��������� �����������, ����� ��������� ACK-���
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
    printf("DEBUG ��������� ������ �������� �����: %#.2x\n", m_device_modules);
}

void Gen6Basic::addDevicePhone(const char *device_phone)
{
    m_gen6_phones.insert(m_gen6_phones.end(), device_phone);
    printf("DEBUG �������� ������� ��������� �����: %s\n", device_phone);
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


    printf("DEBUG �������� ��� (��� ��������), ����� ��������: %.2f ���.\n", timeout_s);

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
    printf("DEBUG ������� ������ 1 ���. ������\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED ������ �� ���������\n");
        return;
    }

    p_led_keyring->pushButton1(type);
}

void Gen6Basic::pushLedKeyringBut2(float type)
{
    printf("DEBUG ������� ������ 2 ���. ������\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED ������ �� ���������\n");
        return;
    }

    p_led_keyring->pushButton2(type);
}

void Gen6Basic::pushLedKeyringBut3(float type)
{
    printf("DEBUG ������� ������ 3 ���. ������\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED ������ �� ���������\n");
        return;
    }

    p_led_keyring->pushButton3(type);
}

void Gen6Basic::pushLcdKeyringBut1(float type)
{
    printf("DEBUG ������� ������ 1 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButton1(type);
}

void Gen6Basic::pushLcdKeyringBut2(float type)
{
    printf("DEBUG ������� ������ 2 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButton2(type);
}

void Gen6Basic::pushLcdKeyringBut3(float type)
{
    printf("DEBUG ������� ������ 3 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButton3(type);
}

void Gen6Basic::pushLcdKeyringBut4(float type)
{
    printf("DEBUG ������� ������ 4 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButton4(type);
}

void Gen6Basic::pushLedKeyringBut23(float type)
{
    printf("DEBUG ������� ������ 2 � 3 ���. ������\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED ������ �� ���������\n");
        return;
    }

    p_led_keyring->pushButtons23(type);
}

void Gen6Basic::pushLcdKeyringBut23(float type)
{
    printf("DEBUG ������� ������ 2 � 3 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButtons23(type);
}

void Gen6Basic::pushLcdKeyringBut14(float push_time_s)
{
    printf("DEBUG ������� ������ 1 � 4 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButtons14(push_time_s);
}

void Gen6Basic::pushLcdKeyringBut24(float push_time_s)
{
    printf("DEBUG ������� ������ 2 � 4 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButtons24(push_time_s);
}

void Gen6Basic::pushLcdKeyringBut34(float push_time_s)
{
    printf("DEBUG ������� ������ 3 � 4 �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButtons34(push_time_s);
}

void Gen6Basic::setPanicKeyring()
{
    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED ������ �� ���������\n");
        return;
    }

    p_led_keyring->pushButton1(KEYRING_SHORT);
    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
    p_led_keyring->pushButton1(KEYRING_SHORT);
}

void Gen6Basic::setHandBreak(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ������ ������\n" 
                                : "INFO ������ ������\n";

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
            printf("FAIL ���������� ��������: ����� ���������� �� ���������\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setDoor(int8_t         value,
                        command_type_t execution_method, 
                        float          delay)
{
    const char* message = value ? "INFO ����� �������\n" : "INFO ����� �������\n";

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
            printf("FAIL ���������� ������: ����� ���������� �� ���������\n");
            break;
    }
    wait(delay);
}

void Gen6Basic::setEngine(int8_t value, command_type_t execution_method, int16_t rpm_val, float delay)
{
    const char* message = value ? "INFO ����������� ������� (������ ������� 1000)\n"
                                : "INFO ��������� �������� (����� �������)\n";
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
            printf("FAIL ���������� ���������: ����� ���������� �� ���������\n");
            break;
    }

    wait_s(delay);
}

void Gen6Basic::setRpm(int16_t rpm_val, command_type_t execution_method, float delay)
{
    printf("INFO ����������� ������� �������� = %d\n", rpm_val);

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
            printf("FAIL ���������� ���������: ����� ���������� �� ���������\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setHood(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ����� ������\n" : "INFO ����� ������\n";

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
            printf("FAIL ���������� �������: ����� ���������� �� ���������\n");
            break;
    }

    wait(delay);
}

void Gen6Basic::setKillEngine(int8_t value, float delay_s)
{
    if (p_out_killengine == NULL)
    {
        printf("FAIL ������ ���������. ��� \"���� ������� ������� ���������\" �� ���������\n");
        return;
    }
    
    const char* message = value ? "INFO ��������� ����� ������� �������\n" 
                                : "INFO ���������� ����� ������� �������\n";
    printf("%s", message);

    (value == ON) ? *p_out_killengine = 1
                  : *p_out_killengine = 0;

//    setDigOutByFunc(p_out_killengine, state, ID_IN_ENG_START_FORBID);
    wait(delay_s);
}

bool Gen6Basic::setTrunk(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO �������� ������\n" : "INFO �������� ������\n";


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
            printf("FAIL ���������� ���������: ����� ���������� �� ���������\n");
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
    const char* message = value ? "INFO ��������� ��������\n"
                                : "INFO ��������� ���������\n";


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
            printf("FAIL ���������������� ����� ��������� ���������\n");
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
        printf("FAIL ���� �� ������\n");
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
    printf("INFO ��������� �������� IGN ���� R6\n");

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ����������� ������ ��� ������ � R6\n");
        return;
    }

    setRelayIgn(p_r6, ON);
}

void Gen6Basic::resetR6Ign()
{
    printf("INFO ���������� �������� IGN ���� R6\n");

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ����������� ������ ��� ������ � R6\n");
        return;
    }

    setRelayIgn(p_r6, OFF);
}

void Gen6Basic::setR4Ign()
{
    printf("INFO ��������� �������� IGN ���� R4\n");

    if (p_r4 == NULL)
    {
        printf("FAIL ����������� R4\n");
        return;
    }

    setRelayIgn(p_r4, ON);
}

void Gen6Basic::resetR4Ign()
{
    printf("INFO ���������� �������� IGN ���� R4\n");

    if (p_r4 == NULL)
    {
        printf("FAIL ����������� R4\n");
        return;
    }

    setRelayIgn(p_r4, OFF);
}

void Gen6Basic::setBreak(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ������ ������� ������\n" : "INFO ������ ������� ��������\n";

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
            printf("FAIL ���������������� ����� ���������� ������� �������\n");
            break;
    }

    wait(delay);
}

void Gen6Basic::setRLight(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ������ ���� �������\n"
                                : "INFO ������ ���� ��������\n";

    printf("%s", message);

    if (p_out_light_right != NULL)
    {
        (value == ON) ? *p_out_light_right = 1
                      : *p_out_light_right = 0;

        wait(delay);

        return;
    }

    printf("FAIL ���������� ����� ������������: ������� �� ��������\n");
}

void Gen6Basic::setLLight(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ����� ���� �������\n"
                                : "INFO ����� ���� ��������\n";

    printf("%s", message);

    if (p_out_light_left != NULL)
    {
        (value == ON) ? *p_out_light_left = 1
                      : *p_out_light_left = 0;

        wait(delay);

        return;
    }

    printf("FAIL ���������� ������ ������������: ������� �� ��������\n");
}

void Gen6Basic::setCLock(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO �� �������\n"
                                : "INFO �� ��������\n";

    printf("%s", message);

    if (p_out_central_lock_open != NULL)
    {
        (value == ON) ? *p_out_central_lock_open = 0
                      : *p_out_central_lock_open = 1;
    }
    else
    {
        printf("INFO �������� ��: ������� �� ��������\n");
    }

    if (p_out_central_lock_close != NULL)
    {
        (value == ON) ? *p_out_central_lock_close = 1
                      : *p_out_central_lock_close = 0;
    }
    else
    {
        printf("INFO �������� ��: ������� �� ��������\n");
    }

    wait(delay);
}

void Gen6Basic::setPpp(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ������������ ������������� �������\n"
                                : "INFO ������������ ������������� ��������\n";

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
            printf("FAIL ���������� ���: ������� �� ��������\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setParking(int8_t value, command_type_t execution_method, float delay)
{
    const char* message = value ? "INFO ������� �������\n" 
                                : "INFO ������� ��������\n";

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
            printf("FAIL ���������������� ����� ���������� ���������\n");
            return;
    }

    wait(delay);
}

void Gen6Basic::setSecure(int8_t             value,
                          command_type_t     execution_method,
                          float              delay_s,
                          tag_voltage_ctrl_t tag_voltage_ctrl)
{
    const char* message = value ? "INFO ���������� � ������\n"
                                : "INFO ������ � ������\n";


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
     
                // ������� - �������� � 10 ���. ������ �������� ������� �����, 
                // �.�. ����� �� �������� ����� �� ����� �������
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
                        printf("INFO ������� �� � ������\n");
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
                                            DIAG_GUARD_STATE_IMMO)          // ������ �������
                        && !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                            DIAG_GUARD_STATE_WAITING_AUTH)) // �������� �����������
                    {
                        printf("INFO ������� �� � \"����� � ������\"\n");
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
            printf("FAIL ���������������� ����� ���������� �������: %d\n", execution_method);
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
    printf("INFO ������ ���������\n");
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
                printf("FAIL �� ����� ������� ����������\n");
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
            printf("FAIL ����������� ����� ������� ���������\n");
            return;
    }
}

void Gen6Basic::setRemoteStop(command_type_t execution_method, float sms_delay_s)
{
    printf("INFO �������� ���������\n");
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
                printf("FAIL �� ����� ������� ����������\n");
            }

            // ��� ������� � ������� �����-���� � �������������� ����������
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
            printf("FAIL ����������� ����� �������� ���������\n");
            return;
    }
}

void Gen6Basic::runEngine(command_type_t        execution_method,
                          engine_start_system_t engine_start_system,
                          bool                  acc_must_be_off,
                          int16_t               rpm_val)
{
    printf("INFO ������������� ������ ��������� \n");
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
            printf("INFO �������� ������� ��������� (����� ��): %d - �� �������������� ������� �������\n",
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
            printf("INFO �������� ������� ��������� (����� ��): %d - �� �������������� ������� �������\n",
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
        printf("%s �������� �� �������\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS �������� �������\n");
            
    if (!waitAcc(DELAY_ACC + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s �� ���������� �� ����� ���������\n", log_label_buf);
        return ACC_ENG_ERROR;
    }

    printf("PASS �� ���������� ����� ���������\n");

    if (!waitIgn1(DELAY_IGN_AFTER_ACC + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0))
    {
        printf("%s �� ��������� �� ����� ���������\n", log_label_buf);
        return IGN1_ENG_ERROR;
    }

    printf("PASS �� ��������� ����� ���������\n");

    if (waitStarter(DELAY_STARTER_AFTER_IGN + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("PASS �� ������� ����� ��������� \n"); 

        if (acc_must_be_off)
        {
            if (waitAcc(TIME_DELAY::MIN_WIRE_EN_DELAY_S, 1))
            {
                printf("PASS � ����������� ���� ���������\n"); 
            }
            else 
            {
                printf("%s � ����������� �� ���� ���������\n", log_label_buf);
                return ACC_ENG_ERROR;
            }
        }
        else
        {
            if (waitAcc(TIME_DELAY::MIN_WIRE_EN_DELAY_S, 0))
            {
                printf("PASS � ����������� �� ���� ���������\n"); 
            }
            else 
            {
                printf("%s � ����������� ���� ���������\n", log_label_buf);
                return ACC_ENG_ERROR;
            }
        }

        wait(TIME_DELAY::TWO_S);
        setEngine(ON, execution_method);
    }
    else
    {
        printf("%s �� ������� �� ����� ���������\n", log_label_buf);
        return STARTER_ENG_ERROR;
    }

    if (waitStarter(STARTER_MAX_TIME - TIME_DELAY::TWO_S, 1)) 
    {
        printf("PASS �� �������� ���� ���������\n");

        if (waitAcc(TIME_DELAY::MIN_WIRE_EN_DELAY_S, 0)) 
        {
            printf("PASS �� ���������� ����� ���������\n"); 
        }
        else 
        {
            printf("%s �� ���������� �� ����� ���������\n", log_label_buf);
            return STARTER_ENG_ERROR;
        }
    }
    else
    {
        printf("%s �� �������� �� ���� ���������\n", log_label_buf);
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
            printf("INFO �������� ������� ��������� (�����-����): �� �������������� ������� �������\n");
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
        printf("%s �� ����� ��������� �� ������ ��������� �������������\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS ����� ��������� �� ������ ��������� �������������\n");

    if (!waitImitBreak(DELAY_FIRST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s �� �������� �������� ������ �������\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS �������� �������� ������ �������\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0))
    {
        printf("%s �� ������ �����-���� �� ����� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �� ������ �����-���� ����� ���������\n");
    wait(TIME_DELAY::TWO_AND_HALF_S);
    
    setIgn(PIN, ON);
    setEngine(ON, PIN, rpm_val);

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1))
    {
        printf("%s � ������ �����-���� �� ���� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS � ������ �����-���� ���� ���������\n");

    if (!waitImitBreak(TIME_DELAY::ONE_AND_HALF_S, 1))
    {
        printf("%s �������� ������ ������� �� ���������\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS �������� ������ ������� ���������\n");
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
        printf("%s �� ����� ��������� �� ������ ��������� �������������\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS ����� ��������� �� ������ ��������� �������������\n"); 

    if (!waitStartStopBtn(DELAY_FIRST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s �� ������ �����-���� �� ����� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �� ������ �����-���� ����� ���������\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1))
    {
        printf("%s � ������ �����-���� �� ���� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS � ������ �����-���� ���� ���������\n");

    if (!waitImitBreak(DELAY_LAST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s �� �������� �������� ������ �������\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS �������� �������� ������ �������\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0))
    {
        printf("%s �� ������ �����-���� �� ����� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �� ������ �����-���� ����� ���������\n");

    wait(TIME_DELAY::STD_WIRE_EN_DELAY_S);
    setIgn(PIN, ON);
    setEngine(ON, PIN, rpm_val);
    wait(TIME_DELAY::ONE_S);

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s � ������ �����-���� �� ���� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS � ������ �����-���� ���� ���������\n");

    if (!waitImitBreak(TIME_DELAY::ONE_S, 1)) 
    {
        printf("%s �������� ������ ������� �� ���������\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR;
    }

    printf("PASS �������� ������ ������� ���������\n");
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
        printf("%s �� ����� ��������� �� ������ ��������� �������������\n", log_label_buf);
        return BYPASS_ENG_ERROR;
    }

    printf("PASS ����� ��������� �� ������ ��������� �������������\n");

    if (!waitStartStopBtn(DELAY_FIRST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s �� ������ �����-���� �� ����� ��������� 1\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �� ������ �����-���� ����� ��������� 1\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s � ������ �����-���� �� ���� ��������� 1\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS � ������ �����-���� ���� ��������� 1\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0)) 
    {
        printf("%s �� ������ �����-���� �� ����� ��������� 2\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �� ������ �����-���� ����� ��������� 2\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s � ������ �����-���� �� ���� ��������� 2\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS � ������ �����-���� ���� ��������� 2\n");

    if (!waitImitBreak(DELAY_LAST_IMPULSE_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 0)) 
    {
        printf("%s �� �������� �������� ������ �������\n", log_label_buf);
        return IMIT_BEAK_ENG_ERROR; 
    }

    printf("PASS �������� �������� ������ �������\n"); 

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 0)) 
    {
        printf("%s �� ������ �����-���� �� ����� ��������� 3\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �� ������ �����-���� ����� ��������� 3\n");

    if (!waitStartStopBtn(TIME_DELAY::ONE_AND_HALF_S, 1)) 
    {
        printf("%s � ������ �����-���� �� ���� ��������� 3\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS � ������ �����-���� ���� ��������� 3\n");

    setIgn(PIN, ON);
    setEngine(ON, PIN, rpm_val);
    
    if (!waitImitBreak(DELAY_RPM_SS + TIME_DELAY::STD_WIRE_EN_DELAY_S, 1)) 
    {
        printf("%s �������� ������ ������� �� ���������\n", log_label_buf);
        return START_BTN_ENG_ERROR;
    }

    printf("PASS �������� ������ ������� ���������\n");
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
            printf("FAIL ��������� �� ���������\n");
        }
    }

    if (!getIgn1())
    {
        printf("PASS ��������� ��������� ��������\n");
    }
    else 
    {
        printf("FAIL ��������� ��������� �� ��������\n");
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

    state ? msg = "��������� ���. �������"
          : msg = "���������� ���. �������";

    printf("INFO %s\n", msg);

    switch (execution_method)
    {
        case PIN:
            *p_out_additsensor = state;
            break;
                
        default:
            printf("INFO ���������� ���. ��������: ����� ���������� �� ���������\n");
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
        printf("INFO ��� ������ ��� ������ � BLE\n");
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
            printf("FAIL �������� ������ ����������\n");
            break;
    }

    return result;
}

bool Gen6Basic::getR6Status(command_type_t execution_method)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE\n");
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
            printf("FAIL �������� ������ ����������\n");
            break;
    }

    return result;
}

bool Gen6Basic::getTagStatus(command_type_t execution_method)
{
    bool result = false;

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE\n");
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
            printf("FAIL �������� ������ ����������\n");
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
    printf("INFO ---����� ��������---\n");

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

            // �� ������, ���� � ������� ������� �������� ������ ��������
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
                
                setIgn(PIN,                              // ���� ������� � ����� ����������� ����� SLP ��� ��������� ������ ��� ���������
                       ON,
                       TIME_DELAY::MAX_WIRE_EN_DELAY_S);
                setIgn(PIN, OFF);
                wait(TIME_DELAY::TEN_S);
            }
            else
            {
                setIgn(PIN, OFF);
            }

            // ��������� ��������� �����, ���� �������
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
                        printf("INFO ����� ��������. ��������� ���������: ������. ������ ��� ������ �������\n");
                        pushLcdKeyringBut23(KEYRING_SHORT);
                        wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);
                    }

                    pushLcdKeyringBut2(KEYRING_SHORT);
                    wait(TIME_DELAY::MIN_WAITING_REGION_STATE_S);

                    if (getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                        SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                        DIAG_GUARD_STATE_ARM))
                    {
                        printf("INFO ����� ��������. ��������� ���������: ����� � ������. ������ ��� ������ �������\n");
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

            if (   m_device_name != GEN6_X96V2 // �� ������ X96v2 ��� ������� �����������
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
            printf("DEBUG ����������� ������� ������ ��������\n");
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
        printf("FAIL ���������� ���������. ��� \"������\" �� ���������\n");

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
                    printf("FAIL ���������� ���������. ��� \"���������� ���������\" �� ���������\n");
                    return -1;
                }

                return getDigInByFunc(p_in_block, ID_OUT_ENG_BLOCK);
            }

        case THROUGH_R6:
            return p_r6->isNoSet();

        case THROUGH_R6ECO:
            return p_r6eco->getBlock();

        default:
            printf("FAIL ���������� ����������. ��� ���������� �� ����������\n");
            return -1;
        
    }
}

bool Gen6Basic::waitBleStatus(float time, bool error_flag, float error)
{
    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE \n");
        return false;
    }

    Timer timer;
    bool  result = true;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    printf("INFO �������� ��������� BLE\n");

    while (end < time)
    {
        if (getBleStatus() == 1)
        {
            printf("INFO ������� ����� � R6 ��������� � �������\n");
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

    printf("INFO ������ �������: %f\n", end);
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
        printf("INFO ��� ������ ��� ������ � BLE \n");
        return false;
    }

    Timer timer;
    bool  result = true;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    printf("INFO �������� ����������� R6\n");

    while (end < time)
    {
        if (waiting_status && getR6Status())
        {
            printf("INFO ������ R6 ��������� � �������\n");
            break;
        }
        else if (!waiting_status && !getR6Status())
        {
            printf("INFO ������ �� �������: R6 �������\n");
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

    printf("INFO ������ �������: %f\n", end);
    timer.stop();

    return result;
}

bool Gen6Basic::waitRelayLockAndUnlock(RelayBase *relay,
                                       uint8_t    lock_pulse_count,
                                       uint8_t    unlock_pulse_count,
                                       float      timeout_s)
{
    const float min_pulse_width_s = TIME_DELAY::MIN_PULSE_WIDTH_S; // ����������� ������������ �������� ������� ����� ����������

    Timer   timer;                                                 // ������
    float   begin_time_s;                                          // ����� ������ ������� ��������
    float   current_time_s;                                        // ������� �����
                       
    float   lock_start_s;                                          // ����� ��������� ���������� �������� �����
    float   lock_end_s;                                            // ����� ��������� �������� �������� ����� (������ ���������)
                       
    float   unlock_start_s;                                        // ����� ��������� ���������� �������� �����
    float   unlock_end_s;                                          // ����� ��������� �������� �������� ����� (������ ���������)
                       
    bool    is_lock_potential    = false;                          // ������� ��������� �������� ����� ������
    bool    is_unlock_potential  = false;                          // ������� ��������� �������� ����� ������
                       
    uint8_t current_lock_count   = 0;                              // ���������� ���������� ��������� �������� ����� ������
    uint8_t current_unlock_count = 0;                              // ���������� ���������� ��������� �������� ����� ������


    if (relay == NULL)
    {
        printf("FAIL ���� �� ������\n");
        return false;
    }

    printf("INFO ������ �������� ��������� ���������/��������� ����� ������ �� ����\n");

    timer.start();

    begin_time_s   = timer.read();
    current_time_s = timer.read();

    while (current_time_s - begin_time_s < timeout_s)
    {
        // ��������� �������� ������
        if (relay->isLockSet())
        {
            is_lock_potential = true;
            lock_start_s      = timer.read();
        }

        // ��������� �������� ������
        if (relay->isUnlockSet())
        {
            is_unlock_potential = true;
            unlock_start_s      = timer.read();
        }

        // ������� �������� ������
        if (is_lock_potential && !relay->isLockSet())
        {
            lock_end_s = timer.read();

            if (lock_end_s - lock_start_s > min_pulse_width_s)
            {
                is_lock_potential = false;
                printf("INFO ������� %d-� ������� ��������� ����� ������ �� ����\n", ++current_lock_count);
                printf("INFO ������ �������: %f\n", timer.read());
            }
        }

        // ������� �������� ������
        if (is_unlock_potential && !relay->isUnlockSet())
        {
            unlock_end_s = timer.read();

            if (unlock_end_s - unlock_start_s > min_pulse_width_s)
            {
                is_unlock_potential = false;
                printf("INFO ������� %d-� ������� ��������� ����� ������ �� ����\n", ++current_unlock_count);
                printf("INFO ������ �������: %f\n", timer.read());
            }
        }

        current_time_s = timer.read();
    }

    timer.stop();

    printf("INFO ����� ��������, ������ �������: %f\n", timer.read());
    printf("INFO ��������� �� ���� �� ��������� ����� ������ ���������: %d, ��������: %d\n", lock_pulse_count,   current_lock_count);
    printf("INFO ��������� �� ���� �� ��������� ����� ������ ���������: %d, ��������: %d\n", unlock_pulse_count, current_unlock_count);

    if (   current_lock_count   == lock_pulse_count
        && current_unlock_count == unlock_pulse_count)
    {
        printf("INFO ���������� ���������� ��������� ������������� ����������\n");
        return true;
    }
    else
    {
        printf("INFO ���������� ���������� ��������� �� ������������� ����������\n");
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
        printf("INFO ��� ������ ��� ������ � R6\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (   !is_base_siren
            &&  p_in_siren != NULL
            && *p_in_siren)         // ���� �������� getAlarm(), �� ����� FAIL ���� ��� �� ��������� (���� ����� ��������� ����� ������ �� ���� ���������)
        {
            is_base_siren = true;
            printf("INFO ������� ������ ������� ������\n");
        }

        if (   !is_r6_siren
            &&  p_r6->isOutSet())
        {
            is_r6_siren = true;
            printf("INFO ������� ������ ������ R6\n");
        }
    }

    if (   (type & SIREN_TYPE_BASE) == SIREN_TYPE_BASE
        && !is_base_siren)
    {
        printf("INFO �������� ������ ������� ������, �� ������� �� ���\n");
        result = false;
    }

    if (   (type & SIREN_TYPE_R6) == SIREN_TYPE_R6
        && !is_r6_siren)
    {
        printf("INFO �������� ������ ������ R6, �� ������� �� ���\n");
        result = false;
    }

    if (result)
    {
        printf("INFO ��� ��������� ������� ������ ���� ��������\n");
    }

    return result;
}

bool Gen6Basic::waitR6LockAndUnlock(uint8_t lock_pulse_count,
                                    uint8_t unlock_pulse_count,
                                    float   timeout_s)
{
    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ����������� ������ ��� ������ � R6\n");
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
        printf("FAIL ����������� ������ ��� ������ � R6\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r6->isLockSet())
        {
            printf("INFO ��������� �������� ����� ������ �������, ������ �������: %.2f\n", timer.read());
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
        printf("FAIL ����������� ������ ��� ������ � R6\n");
        return false;
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r6->isUnlockSet())
        {
            printf("INFO ��������� �������� ����� ������ �������, ������ �������: %.2f\n", timer.read());
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

    float   signal_min_pulse_width_s = TIME_DELAY::MIN_PULSE_WIDTH_S; //!< ������������ �������, ������� �� ����� ��������� �����
    float   signal_start_time_s      = 0.0f;                          //!< ����� ��������� ����������
    float   signal_end_time_s        = 0.0f;                          //!< ����� ��������� ������� ���������� (�������)


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ����������� ������ ��� ������ � R6\n");
        return false;
    }

    printf("INFO ��������� ���������� �������� ������ R6: %d ��.\n", signal_count);
    timer.start();

    while (timer.read() < timeout_s)
    {
        if (   !is_potential
            && p_r6->isOutSet())
        {
            is_potential        = true;
            signal_start_time_s = timer.read();
            printf("INFO ������� ��������� ������ R6\n");
        }

        if (   is_potential
            && !p_r6->isOutSet())
        {
            signal_end_time_s = timer.read();

            if (signal_end_time_s - signal_start_time_s > signal_min_pulse_width_s)
            {
                is_potential = false;
                printf("INFO ������� %d-� ������ ������ R6\n", ++current_signal_count);
            }
        }
    }

    timer.stop();

    if (current_signal_count == signal_count)
    {
        printf("PASS ��������� ���������� �������� ������ R6 ��������\n");
        return true;
    }
    else
    {
        printf("FAIL ��������� ���������� �������� ������ R6 �� ��������\n");
        return false;
    }
}

bool Gen6Basic::waitRelayBlockNormalOpen(RelayBase *relay,
                                         bool       status,
                                         float      timeout_s)
{
    Timer timer;
    bool  block_status; // ������ ���������� ���������


    if (relay == NULL)
    {
        printf("FAIL ���� �� ������\n");
        return false;
    }

    printf("INFO �������� ���������� ���� �� �������� NO, ����� �������� ����������: %.6f\n", timeout_s);
    timer.start();

    while (timer.read() < timeout_s)
    {
        block_status = !relay->isNoSet(); // �������� ������ ����������:
                                          // ���������� �������� ��� ���������� ���� �������� NO (�.�. ��� ��������, ��� ���� ��������� � ��������� ������������)
                                          // ��� ���������� ������� ���� �� ���������, ��� �������� ���������� ���������

        // ��������� ������ ��� ������ ���������� (���/����)
        if (block_status == status)
        {
            timer.stop();
            printf("INFO ��������� �� �������� NO �� ���� ������� �����: %.6f\n", timer.read());
            return true;
        }
    }

    timer.stop();
    printf("INFO ��������� �� �������� NO �� ���� �� �������, ������ �������: %.6f\n", timer.read());

    return false;
}

bool Gen6Basic::waitR6BlockNormalOpen(bool status, float timeout_s)
{
    printf("INFO �������� ���������� ���� R6 �� �������� NO\n");

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ��� ������ ��� ������ � R6\n");
        return false;
    }

    return waitRelayBlockNormalOpen(p_r6,
                                    status,
                                    timeout_s);
}

bool Gen6Basic::waitR6BlockNormalClose(bool status, float timeout_s)
{
    bool nc_status = !status; // ��������� ���� NC ����� ��������������� ���� NO
    bool result;


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ��� ������ ��� ������ � R6\n");
        return false;
    }

    // �.�. �� �� ���������� ��� NC, �� ����� ��������� �� ���������� ����� ��� NO
    printf("INFO �������� R6 ���������� NC ����� ������� NO\n");
    result = waitR6BlockNormalOpen(nc_status, timeout_s);

    return result;
}

bool Gen6Basic::waitR4Block(bool status, float timeout_s)
{
    printf("INFO �������� ���������� ���� R4 �� �������� NO\n");

    if (p_r4 == NULL)
    {
        printf("FAIL ����������� R4\n");
        return false;
    }

    return waitRelayBlockNormalOpen(p_r4,
                                    !status,    // �.�. R4 �������� �������� COM � NO ��� ����������, �� ��� ���. ���������� ����� 1
                                    timeout_s);
}

bool Gen6Basic::waitR4Output(bool status, float timeout_s)
{
    Timer timer;


    if (p_r4 == NULL)
    {
        printf("FAIL ����������� R4\n");
        return false;
    }

    printf("INFO �������� �������� Output ���� R4\n");
    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r4->isOutSet() == status)
        {
            printf("INFO ������� ��������� ������ �� ������ Output ���� R4\n");
            return true;
        }
    }

    timer.stop();
    printf("INFO ��������� ������ �� ������ Output ���� R4 �� ��� �������\n");

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
        printf("FAIL ����������� ������ ��� ������ � R6\n");
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
                printf("INFO ��������� ������ R6\n");
                return true;
            }

            is_potential = true;
        }

        if (is_potential && !p_r6->isOutSet())
        {
            if (type == R6_KLAXON)
            {
                printf("INFO �������� ������� R6\n");
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
        printf("FAIL ����������� ������ ��� ������ � R6\n");
        return false;
    }

    printf("INFO �������� ���������� ������ R6, ����������: %d\n", polarity);
    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_r6->isOutSet() == polarity)
        {
            printf("INFO ������� ��������� ������ R6\n");
            return true;
        }
    }

    printf("INFO ��������� ������ R6 �� �������\n");

    return false;
}

bool Gen6Basic::checkR6EcoLedExt(r6eco_led_ext_mode_t led_ext_mode, float timeout_s)
{
    Timer        timer;
    const int8_t err_percent = 5;    // ������� ���������� ������
    float        err_time_s;         // ���������� ������

    float  active_time_s  = 0.0;     // ����� ��������� ������
    float  passive_time_s = 0.0;     // ����� ���������� ������

    float  begin_s = 0.0;            // ����� ��������� ��������� ������
    float  end_s   = 0.0;            // ����� ��������� ���������� ������

    bool   is_potential     = false; // ������� ���������
    bool   is_first_impulse = true;  // ������ �� ��� �������
    int8_t pulse_count      = 0;     // ���������� ���������� ���������

    bool   is_passive = false;;      // �������� �� ��������� (������ �� �����)


    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ����������� ������ ��� ������ � R6 ECO\n");
        return false;
    }

    printf("INFO �������� ������ ���������� ���������� R6 ECO\n");

    // ���������� ����� ��������� � ���������� ������
    switch (led_ext_mode)
    {
        // ��������� ��������� �� ������ � �����
        case R6ECO_LED_EXT_MODE_DISARM:
            is_passive = true;
            break;

        // ������ ��� � 1.4 �������
        case R6ECO_LED_EXT_MODE_ARM:
            active_time_s  = 0.100f;
            passive_time_s = 1.300f;
            break;

        // ������ ��� � �������
        case R6ECO_LED_EXT_MODE_ARM_DISCONNECTED:
            active_time_s  = 0.100f;
            passive_time_s = 0.900f;
            break;

        default:
            printf("FAIL ����������� ����� ������ ���������� ���������� R6 ECO\n");
            return false;
    }

    err_time_s = max(active_time_s, passive_time_s) * err_percent / 100.0f; // ���������� �����������
    timer.start();

    while (timer.read() < timeout_s)
    {
        // ������� �������� �������
        if (   !is_potential
            &&  p_r6eco->getLedExt())
        {
            begin_s      = timer.read();
            is_potential = true;

            printf("INFO ������� ��������� ���������� ����������: %.6f\n", begin_s);

            if (!is_passive) // ����� �������� ����������� ������ ���� ��������� �� ��������
            {
                if (   begin_s - end_s < passive_time_s + err_time_s
                    && begin_s - end_s > passive_time_s - err_time_s)
                {
                    printf("PASS ����� ����� ���������� ���������� ���������� ���������\n");
                }
                else if (!is_first_impulse) // ��� ������ ��������� �������� ��� ������ ��������� ����� �.�. �� ���� �����������
                {
                    printf("FAIL ����� ����� ���������� ���������� ���������� �� ���������\n");
                    return false;
                }
            } // if !is_passive
        } // if !is_potential

        // ������� ��������� �������
        if (    is_potential
            && !p_r6eco->getLedExt())
        {
            end_s        = timer.read();
            is_potential = false;

            printf("INFO ������� ������� ���������� ����������: %.6f\n", end_s);
            ++pulse_count;

            if (!is_passive) // ����� �������� ����������� ������ ���� ��������� �� ��������
            {
                if (   end_s - begin_s < active_time_s + err_time_s
                    && end_s - begin_s > active_time_s - err_time_s)
                {
                    printf("PASS ������������ �������� ���������� ���������� ������\n");
                }
                else if (!is_first_impulse) // ������ ������ ����� ���� � ������, ���� �� ������ �������� ��� �� ����� ��������
                {
                    printf("FAIL ������������ �������� ���������� ���������� ��������\n");
                    return false;
                }
            } // if !is_passive

            is_first_impulse = false;
        }
    } // while (timer.read() < timeout_s)

    if (!is_passive)
    {
        // �������� ����, ��� ��������� ������� ���� �� ��������� ���������:
        // 1. ������ ���������� ���, ��������������� �������
        // 2. �� 1 ��� ������ ��� ��� �������� �� ���������� ����� (�.�. �������� ����� �������� ���� ����� ����� ������ �������)
        if ((int8_t)(timeout_s / (active_time_s + passive_time_s)) - pulse_count < 2)
        {
            printf("PASS ��������� ��������� ����� � ��������� ��������\n");
        }
        else
        {
            printf("FAIL ��������� ��������� �� ����� � ��������� ��������\n");
            return false;
        }
    }
    else
    {
        // �������� ����, ��� ��������� �� ����� ������
        if (    is_first_impulse // ���� ������� �������� �� ��� ����
            && !is_potential
            &&  pulse_count == 0)
        {
            printf("PASS ��������� ��������� ��� ����� �������� � ��������� ���������\n");
        }
        else
        {
            printf("FAIL ��������� ��������� �� ��� ����� �������� � ��������� ���������\n");
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
    float begin_s = 0.0;  // ������ �������
    float end_s   = 0.0;  // ������ ������� � ������ �������


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE \n");
        return false;
    }

    timer.start();

    begin_s = timer.read();
    end_s   = begin_s;

    printf("INFO �������� ����������� �����\n");

    while (end_s < time_s)
    {
        if (getTagStatus())
        {
            printf("INFO ������ ����� �������� � �������\n");
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

    printf("INFO ������ �������: %f\n", end_s);
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
            printf("INFO ���������� ��������� ������. ����������� ����� ����������\n");
            return -1;
    }
}

int8_t Gen6Basic::getIgn(command_type_t execution_method)
{
    if (p_out_ign == NULL)
    {
        printf("FAIL ��� \"���������\" �� ���������\n");

        return -1;
    }

    switch (execution_method)
    {
        case PIN:
            return *p_out_ign;

        default:
            printf("INFO ���������� ��������� ���������. ����������� ����� ����������\n");
            return -1;
    }
}

bool Gen6Basic::isBlockSet()
{
    if (p_in_block == NULL)
    {
        printf("FAIL ���������� ���������. ��� \"���������� ���������\" �� ���������\n");
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
                printf("FAIL ���������� ���������. ��� \"������ �����\" �� ���������\n");
                return false;
            }

            return !(*p_in_tag_status);

        case SLP:
            return getSlpCmdStatus(DIAG_DATA_FLAG,
                                   SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                                   DIAG_FLAG_BIT_TAG);

        default:
            printf("FAIL ���������� ��������� \"������ �����\": �� ��������� ����� ����������\n");
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
        printf("INFO �������� �������� ��������� \n");
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);

        if (waitIgn1(time_s, 1))
        {
            printf("PASS ���� ��������� � ���������\n"); 
            
            if (stop_after_impulse_num != WITHOUT_STOP)
            {
                setIgn(PIN, OFF, TIME_DELAY::NULL_DELAY_S);
            }
        }
        else
        {
            printf("FAIL �� ���� ��������� � ���������\n");   
        }

        if (waitAcc(TIME_DELAY::FIVE_S + TIME_DELAY::STD_WIRE_EN_DELAY_S, 1)) 
        {
            printf("PASS ���� ��������� � �����������\n"); 
        }
        else
        {
            printf("FAIL �� ���� ��������� � �����������\n");   
        }

        if (waitInspector(TIME_DELAY::FIVE_S + TIME_DELAY::STD_WIRE_EN_DELAY_S, 1)) 
        {
            printf("PASS ���� ��������� � ������ ������ �������������\n");
        }
        else
        {
            printf("FAIL �� ���� ��������� � ������ ������ �������������\n");
        }
    }
    else
    {
        if (waitStartStopBtn(time_s, 0))
        {
            printf("PASS ������ ������� �������\n");

            if (stop_after_impulse_num == STOP_AFTER_FIRST_IMPULSE_RECV)
            {
                setIgn(PIN, OFF, TIME_DELAY::NULL_DELAY_S);
            }
        }
        else 
        {
            printf("FAIL ������ ������� �� �������\n");
        }
                
        if (waitStartStopBtn(TIME_DELAY::FIRST_STOP_IMPULSE_REMOVE_WAIT_S, 1))
        {
            printf("PASS ������ ������� ����\n");

            if (stop_after_impulse_num == STOP_AFTER_FIRST_IMPULSE_REMOVED)
            {
                setIgn(PIN, OFF, TIME_DELAY::NULL_DELAY_S);
            }
        }
        else 
        {
            printf("FAIL ������ ������� �� ����\n");
        }

        if (waitStartStopBtn(TIME_DELAY::SECOND_STOP_IMPULSE_RECV_WAIT_S, 0))
        {
            printf("PASS ������ ������� �������\n");

            if (stop_after_impulse_num == STOP_AFTER_SECOND_IMPULSE_RECV)
            {
                setIgn(PIN, OFF);
            }
            else if (stop_after_impulse_num <= STOP_AFTER_FIRST_IMPULSE_REMOVED)
            {
                printf("PASS ������ ������� ������� (����� ���������� ���������)\n");
            }
        }
        else 
        {
            if (stop_after_impulse_num <= STOP_AFTER_FIRST_IMPULSE_REMOVED)
            {
                printf("PASS ������ ������� �� ������� (����� ���������� ���������)\n");
            }
            else
            {
                printf("FAIL ������ ������� �� �������\n");
            }
        }

        if (waitStartStopBtn(TIME_DELAY::SECOND_STOP_IMPULSE_REMOVE_WAIT_S, 1))
        {
            printf("PASS ������ ������� ����\n");

            if (stop_after_impulse_num == STOP_AFTER_SECOND_IMPULSE_REMOVED)
            {
                setIgn(PIN, OFF);
            }
        }
        else 
        {
            printf("FAIL ������ ������� �� ����\n");
        }

        if (waitStartStopBtn(TIME_DELAY::THIRD_STOP_IMPULSE_RECV_WAIT_S, 0))
        {
            printf("PASS ������ ������� �������\n");

            if (stop_after_impulse_num == STOP_AFTER_THIRD_IMPULSE_RECV)
            {
                setIgn(PIN, OFF);
            }
            else if (stop_after_impulse_num <= STOP_AFTER_SECOND_IMPULSE_REMOVED)
            {
                printf("PASS ������ ������� ������� (����� ���������� ���������)\n");
            }
        }
        else 
        {
            if (stop_after_impulse_num <= STOP_AFTER_SECOND_IMPULSE_REMOVED)
            {
                printf("PASS ������ ������� �� ������� (����� ���������� ���������)\n");
            }
            else
            {
                printf("FAIL ������ ������� �� �������\n");
            }
        }
        
        if (waitStartStopBtn(TIME_DELAY::THIRD_STOP_IMPULSE_REMOVE_WAIT_S, 1))
        {
            printf("PASS ������ ������� ����\n");

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
            printf("FAIL ������ ������� �� ����\n");
        }
    }
}

void Gen6Basic::checkSupIgn(ign_statuses_t ign_status, out_statuses_t state)
{
    const char* message;


    if (!getIgn1())
    {
        message = state ? "PASS ��������� ��������� ��������\n" 
                        : "FAIL ��������� ��������� ��������\n";
    }
    else 
    {
        message = !state ? "PASS ��������� ��������� �� ��������\n" 
                         : "FAIL ��������� ��������� �� ��������\n";
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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

        return false;
    }

    printf("INFO �������� ��������� ������ ����� ��������� ���������� (BLE)\n");

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
        message  = state ? "PASS �� ������ ������ �������������� ����� ���������\n"
                         : "FAIL �� ������ ������ �������������� ����� ���������\n";
    }
    else 
    {
        message  = !state ? "PASS �� ������ ������ �������������� �� ����� ���������\n"
                          : "FAIL �� ������ ������ �������������� �� ����� ���������\n";
    }

    printf("%s", message);
}

void Gen6Basic::synByIgn1(float time_s)
{
    bool ign1_potential       = 0;
    bool start_stop_potential = 0;


    // ���� �����-���� � ��������� ����� ��������� �� ��������� 1
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


    printf("INFO �������� ��������� ������� ������ ��, ����� ��������: %.6f ���., �����������: %.6f ���.\n",
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

                printf("INFO �� ������ �����-���� ����� ��������� (������� ��������� ���������) �����: %.6f\n", curr_s);
                break;
            }

            curr_s = timer.read() - begin_s;
        }

        if (!is_potential)
        {
            printf("INFO �� ������ �����-���� �� ����� ���������\n");
        }
        else if (curr_s < (turbotimer_time_s - error_time_s))
        {
            printf("FAIL �� ������ �����-���� ����� ��������� (������� ��������� ���������) �� ��������� ������ ��: ����� %.2f �\n",
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

                printf("INFO ��������� ��������� �����: %.6f\n", curr_s);
                break;
            }

            curr_s = timer.read() - begin_s;
        }

        if (!is_potential)
        {
            printf("INFO � ��������� �� ���� ���������\n");
        }
        else if (curr_s < (turbotimer_time_s - error_time_s))
        {
            printf("FAIL ��������� ��������� �� ��������� ������������: ����� %.2f �\n",
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


    printf("DEBUG ����� �������� ��������� ������ ���������: %.2f ���.\n", autostart_time_s);
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
                        printf("FAIL �� ������ �����-���� ����� ��������� (������� ��������� ���������) �� ��������� �������: ����� %.2f ���.\n", as_curr_s);
                    }

                    return;
                }

                break;
                
            case IGNITION_LOCK:

                if (getIgn1())
                {
                    if (as_curr_s < autostart_time_s - error_time_s)
                    {
                        printf("FAIL ��������� ��������� �� ��������� �������: ����� %.2f ���.\n", as_curr_s);
                    }

                    return;
                }

                break;
                
            case EXTERNAL:

                if (getInspector())
                {
                    if (as_curr_s < autostart_time_s - error_time_s)
                    {
                        printf("FAIL �������� �������� �� ��������� �������: ����� %.2f ���.\n", as_curr_s);
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

    bool  is_potential     = false; // ��������� �������
    bool  result           = false; // ���������
    float potential_time_s = 0.0f;  // ����� ��������� ����������


    timer.start();

    while (timer.read() < wait_time_s)
    {
        if (   !is_potential
            && getPppStart() == polarity)
        {
            potential_time_s = timer.read();
            is_potential     = true;
            printf("INFO ��������� � ������� �������� ��� ������� �����: %.2f\n", potential_time_s);
        }

        if (   is_potential
            && getPppStart() != polarity
            && timer.read() - potential_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            printf("INFO ������ � ������� �������� ��� ������� �����: %.2f\n", timer.read());
            result = true;
            break;
        }
    }

    timer.stop();

    if (!result)
    {
        printf("INFO ������ � ������� �������� ��� �� �������\n");
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

    printf("INFO ������� ������� ���������/������� ���: %.2f ���. � ������������ %.2f ���.\n",
           ppp_max_time_s,
           max_error_time_s);

    while (ppp_curr_s < ppp_max_time_s + max_error_time_s)
    {
        if (getPppStart() == polarity)
        {
            printf("INFO ������� ���������/������� ��� ������� ����� %.2f ������\n", ppp_curr_s);
            break;
        }

        ppp_curr_s = timer.read() - ppp_begin_s;
    }

    if (ppp_curr_s < ppp_max_time_s - max_error_time_s)
    {
        printf("FAIL ������� ���������/������� ��� ������� �� ��������� ���������� ���������: ����� %.2f ���.\n", ppp_curr_s);
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
    printf("INFO ��������� ������� �����\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE \n");
        return;
    }

    p_tag->setVcc();
}

void Gen6Basic::resetTagVcc()
{
    printf("INFO ���������� ������� �����\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE\n");
        return;
    }

    p_tag->resetVcc();
}

void Gen6Basic::setR4Vcc()
{
    printf("INFO ��������� ������� R4\n");

    if (p_r4 == NULL)
    {
        printf("INFO ����������� R4\n");
        return;
    }

    p_r4->setVcc(ON);
}

void Gen6Basic::resetR4Vcc()
{
    printf("INFO ���������� ������� R4\n");

    if (p_r4 == NULL)
    {
        printf("INFO ����������� R4\n");
        return;
    }

    p_r4->setVcc(OFF);
}

void Gen6Basic::setR6Vcc()
{
    printf("INFO ��������� ������� R6\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE\n");
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
    printf("INFO ���������� ������� R6\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ��� ������ ��� ������ � BLE\n");
        return;
    }

    p_r6->setVcc(OFF);
}

void Gen6Basic::setTagBtn()
{
    printf("INFO ������� ������ �����\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ��� ������ ��� ������ � ������\n");
        return;
    }

    p_tag->setBtn();
}

void Gen6Basic::resetTagBtn()
{
    printf("INFO �������� ������ �����\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ��� ������ ��� ������ � ������\n");
        return;
    }

    p_tag->resetBtn();
}

void Gen6Basic::pushTagBtn(float type)
{
    printf("INFO ������� ������ �����\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ��� ������ ��� ������ � ������\n");
        return;
    }

    p_tag->pushButton(type);
}

void Gen6Basic::startPpp(command_type_t execution_method,
                         float          btn1_time_s)
{
    printf("INFO ������ ���\n");

    if ((m_device_modules & R868_MODULE) != R868_MODULE)
    {
        execution_method = SMS;
    }

    if (m_device_name == GEN6_M66)
    {
        printf("FAIL M66 �� ������������ ���\n");
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
                printf("FAIL �� ����� ������� ����������\n");
            }

            waitPotential(p_in_ppp_start,
                          getPolarity(ID_OUT_HEAT_CTRL),
                          TIME_DELAY::PPP_STOP_SMS_WAIT_TIME_S);
            break;

        default:
            printf("FAIL ����������� ����� ������� ���\n");
            break;
    }
}

void Gen6Basic::stopPpp(command_type_t execution_method,
                        int8_t         ppp_type,
                        float          btn2_time_s)
{
    printf("INFO ��������� ���\n");

    if ((m_device_modules & R868_MODULE) != R868_MODULE)
    {
        execution_method = SMS;
    }

    if (m_device_name == GEN6_M66)
    {
        printf("FAIL M66 �� ������������ ���\n");
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
                printf("FAIL �� ����� ������� ����������\n");
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
            printf("FAIL ����������� ����� ��������� ���\n");
            break;
    }
}

void Gen6Basic::setPppCmdStart(command_type_t execution_method, float eng_heat_after_sms_wait_time_s)
{
    printf("INFO ������ ���\n");

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
                printf("FAIL �� ����� ������� ����������\n");
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
            printf("FAIL ����������� ����� ������� ���\n");
    }
}

void Gen6Basic::setPppCmdStop(command_type_t execution_method, float eng_heat_after_sms_wait_time_s)
{
    printf("INFO ��������� ���\n");

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
                printf("FAIL �� ����� ������� ����������\n");
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
            printf("FAIL ����������� ��� ��������� ���: %d\n", execution_method);
    }
}
            
void Gen6Basic::setServiceBtn(bool state)
{
    printf("INFO ������� ��������� ������\n");

    if (p_service_btn == NULL)
    {
        printf("DEBUG ��������� ������ �� ����������\n");
        return;
    }

    p_service_btn->setBtn(state);
}

void Gen6Basic::clickServiceBtn(uint32_t count, 
                                float    delay_s)
{
    for (uint32_t i = 0; i < count; ++i)
    {
        printf("INFO ������� ��������� ������\n");

        if (p_service_btn == NULL)
        {
            printf("DEBUG ��������� ������ �� ����������\n");
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
    printf("INFO �������� ��������� ���������� ���������� ������\n");

    if (p_service_btn == NULL)
    {
        printf("DEBUG ��������� ������ �� ����������\n");
        return false;
    }

    return p_service_btn->isLedOn();
}

bool Gen6Basic::waitServiceLed(out_statuses_t out_status, float timeout_s)
{
    const bool expected_status = out_status == ON; // ��������� ������ ���������� ����������

    Timer      timer;                              // ������, ����������� ����� �� ����� ������� ��������


    if (expected_status)
    {
        printf("INFO �������� ��������� ���������� ����������\n");
    }
    else
    {
        printf("INFO �������� ���������� ���������� ����������\n");
    }

    timer.start();

    while (timer.read() < timeout_s)
    {
        if (p_service_btn->isLedOn() == expected_status)
        {
            printf("INFO ������� ��������� ������ ���������� ���������� �����: %.6f ���.\n", timer.read());

            return true;
        }
    }

    printf("INFO ��������� ������ ���������� ���������� �� ��� �������, ������ �������: %.6f ���.\n", timer.read());

    return false;
}

bool Gen6Basic::waitSrvBtnPulse(float pulse_width_s,
                                float err_time_s,
                                float timeout_s)
{
    Timer timer;
    bool  result           = false;
    bool  is_potential     = false;                    // ������� �� ��������� �����
    float potential_time_s = TIME_DELAY::NULL_DELAY_S; // ����� ��������� ����������
    float pulse_time_s     = TIME_DELAY::NULL_DELAY_S; // ����� ��������� ��������


    printf("INFO �������� ������� ���������� ��������� ������, ��������� ������������: %.6f, �����������: %.6f, ����� ��������: %.6f\n",
           pulse_width_s,
           err_time_s,
           timeout_s);

    if (p_service_btn == NULL)
    {
        printf("DEBUG ��������� ������ �� ����������\n");
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

            printf("INFO ������ ���������� ���������� �������\n");

            if (   pulse_time_s - potential_time_s > pulse_width_s - err_time_s
                && pulse_time_s - potential_time_s < pulse_width_s + err_time_s)
            {
                result = true;

                printf("INFO ������������ ������� ������\n");
                break;
            }
            else
            {
                printf("INFO ������������ ������� ��������: %.6f\n", pulse_time_s);
                break;
            }
        } // if is_potential
    } // while (timer.read() < timeout_s)

    if (   !result
        && !is_potential)
    {
        printf("INFO ������ ���������� ���������� �� �������\n");
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

    // ��� ������ ����� ����������� ������, ���� ����������� ������� � �����
    // ����������� ��� ��������� ��������� (7 ��� + 1 ��� ��� ������ �������
    // �����������), ���� ��������� ������� ��� �����. ������,
    // ��������� ������� ������
    bool     is_reg_en = false;


    printf("INFO �������� ���������� ������ � ������ ��� ������������� ���������\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL ��� ����������� ������ �� �����\n");
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

    printf("INFO ��� ����������� ������:");

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
    
    printf("INFO ��������\n");
    wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S); // ���� ����������� � �� - ������ �������

    p_service_btn->waitPulseAmount(1, TIME_DELAY::AFTER_FIRST_PIN_NUM_SBTN_S);

    for (uint8_t i = 0; i < click_count[1]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }
    
    printf("INFO ��������\n");

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
        
        printf("INFO ��������\n");

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
            
            printf("INFO ��������\n");

            p_service_btn->waitPulseAmount(4, TIME_DELAY::AFTER_FOURTH_PIN_NUM_SBTN_S);

            // �������� ���������� ����� ����
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
        printf("INFO ������� ������� ������� ������������\n");
    }
    else
    {
        printf("INFO ������� ������� ������� �����������\n");
    }
    
    uint8_t short_number_impulse = 2;
    
    if (waitSirenPulse(short_number_impulse, 1, TIME_DELAY::FIVE_S, TIME_DELAY::SHORT_SIREN_IMPULSE_S))
    {
        printf("INFO �������� ������� ������� ������������\n");
    }
    else
    {
        printf("INFO �������� ������� ������� �����������\n");
    }

    if (checkChangeEmergencyDisarming(old_pin, NOT_EXIT))
    {
        printf("INFO �������� �������� ����� ���� ����������� ������ �����\n");
    }
    else
    {
        printf("INFO �������� �������� ����� ���� ����������� ������ �� �����\n");
    }
    
    if (!waitServicePulseAmount(short_number_impulse, TIME_DELAY::TEN_S))
    {
        printf("INFO ������� ������������� ����������� �� ��������\n");
    }
    else
    {
        printf("INFO ������� ������������� ����������� ��������\n");
    }

    setIgn(PIN, ON);
    
    if (checkChangeEmergencyDisarming(new_pin, NOT_EXIT))
    {
        printf("INFO �������� �������� ����� ���� ����������� ������ �����\n");
    }
    else
    {
        printf("INFO �������� �������� ����� ���� ����������� ������ �� �����\n");
    }
    
    if (!waitServicePulseAmount(2, TIME_DELAY::TEN_S))
    {
        printf("INFO ������� ������������� ����������� �� ��������\n");
    }
    else
    {
        printf("INFO ������� ������������� ����������� ��������\n");
    }

    setIgn(PIN, ON);
  
    if (checkChangeEmergencyDisarming(new_pin, NOT_EXIT))
    {
        printf("INFO �������� �������� ����� ���� ����������� ������ �����\n");
    }
    else
    {
        printf("INFO �������� �������� ����� ���� ����������� ������ �� �����\n");
    }
    
    if (!waitServicePulseAmount(short_number_impulse, TIME_DELAY::TEN_S))
    {
        printf("INFO ������� ������������� ����������� �� ��������\n");
    }
    else
    {
        printf("INFO ������� ������������� ����������� ��������\n");
    }
}

bool Gen6Basic::checkChangeEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits)
{
    char click_count_ch[5] = { 0, 0, 0, 0, 0 };
    char click_count[4]    = { 0, 0, 0, 0 };

    uint32_t pin_digit = 0;


    printf("INFO �������� ���������� ������ � ������ ��� ������������� ���������\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL ��� ����������� ������ �� �����\n");
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

    printf("INFO ��� ����������� ������:");

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
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[0]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO ��������\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_FIRST_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_FIRST_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO ������� ������������� 1 ����� �� ��������\n");
        return false;
    }
    else
    {
        printf("INFO ������� ������������� 1 ����� ��������\n");
    }

    wait(TIME_DELAY::BTN_BEFORE_IGN_ON_DELAY_S);

    setIgn(PIN, ON);

    if (digits == BEFORE_SECOND_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[1]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO ��������\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_SECOND_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_SECOND_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO ������� ������������� 2 ����� �� ��������\n");
        return false;
    }
    else
    {
        printf("INFO ������� ������������� 2 ����� ��������\n");
    }

    wait(TIME_DELAY::BTN_BEFORE_IGN_ON_DELAY_S);

    setIgn(PIN, ON);

    if (digits == BEFORE_THIRD_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[2]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO ��������\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_THIRD_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_THIRD_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO ������� ������������� 3 ����� �� ��������\n");
        return false;
    }
    else
    {
        printf("INFO ������� ������������� 3 ����� ��������\n");
    }

    wait(TIME_DELAY::BTN_BEFORE_IGN_ON_DELAY_S);

    setIgn(PIN, ON);

    if (digits == BEFORE_FOURTH_DIGIT)
    {
        if (!waitServicePulseAmount(COUNT_STRUCT::MIN_BTN_LIGHTS_COUNT,
                                    TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    for (uint8_t i = 0; i < click_count[3]; ++i)
    {
        clickServiceBtn();
        wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S);
    }

    printf("INFO ��������\n");

    wait(TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    if (digits == AFTER_FOURTH_DIGIT)
    {
        if (!waitServicePulseAmount(1, TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S))
        {
            printf("INFO ������� ������ ����������� �� ��������\n");
            return true;
        }
        else
        {
            printf("INFO ������� ������ ����������� ��������\n");
            return false;
        }
    }

    setIgn(PIN, OFF);

    if (!waitServicePulseAmount(COUNT_STRUCT::BTN_PIN_CHANGE_FOUTH_NUM_LIGHTS_COUNT,
                                TIME_DELAY::BTN_LED_AFTER_PIN_NUM_CHANGE_WAIT_TIME_S))
    {
        printf("INFO ������� ������������� 4 ����� �� ��������\n");
        return false;
    }
    else
    {
        printf("INFO ������� ������������� 4 ����� ��������\n");
    }

    return true;
}

bool Gen6Basic::checkEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits)
{
    char click_count_ch[5] = { 0, 0, 0, 0, 0 };
    char click_count[4]    = { 0, 0, 0, 0 };

    uint32_t pin_digit = 0;

    printf("INFO �������� ���������� ������ � ������ ��� ������������� ���������\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL ��� ����������� ������ �� �����\n");
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

    printf("INFO ��� ����������� ������:");

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
    
    printf("INFO ��������\n");
    
    bool result = true;
    
    if (!waitServicePulseAmount(1, TIME_DELAY::AFTER_FIRST_PIN_NUM_SBTN_S))
    {
        printf("INFO ������� ����������� �� �������� ����� ����� 1 �����\n");
        result = false;
    }
    else
    {
        printf("INFO ������� ����������� �������� ����� ����� 1 �����\n");
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
    
    printf("INFO ��������\n");

    if (!waitServicePulseAmount(2, TIME_DELAY::AFTER_SECOND_PIN_NUM_SBTN_S))
    {
        printf("INFO ������� ����������� �� �������� ����� ����� 2 �����\n");
        result = false;
    }
    else
    {
        printf("INFO ������� ����������� �������� ����� ����� 2 �����\n");
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
    
    printf("INFO ��������\n");

    if (!waitServicePulseAmount(3, TIME_DELAY::AFTER_THIRD_PIN_NUM_SBTN_S))
    {
        printf("INFO ������� ����������� �� �������� ����� ����� 3 �����\n");
        result = false;
    }
    else
    {
        printf("INFO ������� ����������� �������� ����� ����� 3 �����\n");
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
    
    printf("INFO ��������\n");

    if (!waitServicePulseAmount(4, TIME_DELAY::AFTER_FOURTH_PIN_NUM_SBTN_S))
    {
        printf("INFO ������� ����������� �� �������� ����� ����� 4 �����\n");
        result = false;
    }
    else
    {
        printf("INFO ������� ����������� �������� ����� ����� 4 �����\n");
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

    printf("INFO �������� ���������� ������ � ������\n");

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL ��� ����������� ������ �� �����\n");
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

    printf("INFO ��� ����������� ������:");

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
                printf("INFO ����� �� ������ �����������\n");
                setIgn(PIN, OFF);

                // ����� �� ������� ��������� �� �����������
                if (reg_queue_mode == REGISTRATION_QUEUE_MODE_EN)
                {
                    return RegistrationQueue::removeFromQueue(p_serial);
                }
            }
            else
            {
                printf("INFO ���� � ����� �����������\n");

                // ������ � ������� ��������� �� �����������
                if (reg_queue_mode == REGISTRATION_QUEUE_MODE_EN)
                {
                    if (!RegistrationQueue::addToQueueAndWait(p_serial))
                    {
                        // ����� ������� �� ���������, ������� � ������� �� �������������
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
                printf("FAIL Test result: Fault. ����� �����������. ������ ������ ��� ���������.\n");
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
        // ����� ������� �� ���������, ������� � �������
        printf("FAIL ������ �� ������� AT: ��������� ���������� ���������� � ������� �� �����������\n");
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

    printf("INFO �������� ���������/���������� ��������� ��� �� ������ ��������\n");
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

    printf("INFO �������� ���������/���������� ���������� ������ �� ������ ��������\n");
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
                printf("INFO �������� ���������/���������� ���������� ������ � ������� ����� ��������\n");
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
    const bool status = out_status == ON; // �������� ������ ���������� ������


    printf("INFO ���������� ��������� �������\n");

    if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
    {
        printf("INFO ���������� ��������� ������� � ������� �����\n");
        setServiceModeTag(status);
    }
    else if ((m_device_modules & R868_MODULE) == R868_MODULE)
    {
        printf("INFO ���������� ��������� ������� � ������� �������\n");
        setServiceModeKeyring();
    }
    else
    {
        printf("INFO ��� ������� ��� ��������� ���������� ������\n");

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
            printf("INFO ������ ������� ������� �� ��������� �����������\n");
            result = NONE;
        }
    }

    printf("DEBUG ����� �������� ������� �������: %.2f\n", end);
    timer.stop();

    return result;
}

bool Gen6Basic::setBypassType(slp_imit_bp_type_t bp_type)
{
    printf("INFO ��������� ���� ������: %d\n", bp_type);

    return p_slp->setBypassType(bp_type);
}

bool Gen6Basic::setBypassMode(slp_imit_bp_mode_t bp_mode)
{
    printf("INFO ��������� ������ ������: %d\n", bp_mode);

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
    printf("INFO ��������� ������ ����������� ����� ������\n");

    if (   (    (reg_queue_mode == REGISTRATION_QUEUE_MODE_EN)
             &&  !addDeviceToRegistrationQueue())
        ||  !p_slp->setSlpStatus(DIAG_DEBUG_COMMAND, DIAG_DEBUG_BIT_REG))
    {
        printf("INFO ��������� ������ ����������� ��������\n");

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
        printf("INFO ������� � ����� ����������� �� ��������\n");

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
    const std::string status_str = status == ON ? "�������" 
                                                : "��������";


    printf("INFO ����� \"���. ������ 1\" %s\n", status_str.c_str());

    if (p_out_additsensor == NULL)
    {
        printf("INFO ��� \"���. ������ 1\" �� ���������, ���������� ����������� �� �����\n");
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
    const std::string status_str = status == ON ? "�������" 
                                                : "��������";


    printf("INFO ����� \"���. ������ 2\" %s\n", status_str.c_str());

    if (p_out_sensor_aux2 == NULL)
    {
        printf("INFO ��� \"���. ������ 2\" �� ���������, ���������� ����������� �� �����\n");
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
    printf("INFO ��������� ����������� ��������� �� SLP, ��������: %d* C\n", value);
    p_slp->setSlpStatus(DIAG_DATA_TEMP_ENG, value);
}

void Gen6Basic::setEngineTemperatureSensorNoDelay(int8_t value)
{
    printf("INFO ��������� ����������� ��������� �� SLP, ��������: %d* C\n", value);
    p_slp->setSlpStatusNoWaitAnsw(DIAG_DATA_TEMP_ENG, value);
}

void Gen6Basic::setBoardTemperatureSensor(int8_t value)
{
    printf("INFO ��������� ����������� �� ����� �� SLP, ��������: %d* C\n", value);
    p_slp->setSlpStatus(DIAG_DATA_TEMP_BOARD, value);
}

void Gen6Basic::setEngineTemperatureCanLadaVesta(int8_t value)
{
    printf("INFO ��������� ����������� ��������� �� CAN, ��������: %d* C\n", value);
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
        printf("INFO ������ ������� �� ��������� �����������\n");
        result = NONE;
    }

    switch (result)
    {
        case NONE:
            printf("TRACE ��������� ������ �� ����������\n");
            break;

        case PULSED:
            printf("TRACE ������ ������: ����� �����������\n");
            break;

        case CONST:
            printf("TRACE ������ ������ ���������\n");
            break;

        case NOSIGNAL:
            printf("TRACE ������ ������ �����������\n");
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

    float wait_time_s  = time_s; // ����� �������� ���������� � ������ �����������
    float block_time_s = 0.0f;   // ����� ��������� ������� ����������


    printf("INFO ����� �������� ����������: %.2f ���.\n", time_s);

    if (error_time_s >= TIME_DELAY::NULL_DELAY_S)
    {
        wait_time_s += error_time_s;

        printf("INFO ����������� ��������: %.2f ���.\n", error_time_s);
    }

    timer.start();

    while (timer.read() < wait_time_s)
    {
        if (status && getBlock(execution_method))
        {
            block_time_s = timer.read();
            result       = true;

            printf("INFO ����� ���������� ������� (��������� �������)\n");
            break;
        }
        else if (!status && !getBlock(execution_method))
        {
            block_time_s = timer.read();
            result       = true;

            printf("INFO ����� ���������� �������� (��������� ����)\n");
            break;
        }
    }

    if (status && !getBlock(execution_method))
    {
        printf("INFO ����� ���������� �� ������� (��������� ����)\n");
    }
    else if (!status && getBlock(execution_method))
    {
        printf("INFO ����� ���������� �� �������� (��������� �� ����)\n");
    }

    timer.stop();
    printf("INFO ������ �������: %.2f\n", timer.read());

    if (   (error_time_s                >= TIME_DELAY::NULL_DELAY_S)
        && (fabs(time_s - block_time_s) >  error_time_s))
    {
        printf("INFO ������ ���������� ������� �� ��������� ���������� ���������\n");
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
        printf("INFO ������� ���������� �� ��������\n");
        return false;
    }

    if (pin == NULL)
    {
        if (m_device_pin == NULL)
        {
            printf("FAIL ��� ����������� ������ �� �����\n");
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
        printf("INFO ������������ �������: %s\n", command_off);

        if (p_gsm->inspectionCyrillicSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                                         command_off,
                                         Sim900::combineStr(dis_sms,
                                                            SMS_MSG_STR::SMS_ANSW_STR_AHJ_OFF,
                                                            SMS_MSG_STR::SMS_ANSW_STR_USER_PIN_ENTERED).c_str()) != SIM900ACT_TRUESMS)
        {
            printf("INFO ������ ��� ������ ��� ���������\n");
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
        printf("INFO ������ ��� ������ ��� ���������\n");
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
        printf("INFO �� ������� ������� 31\n");
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

    printf("INFO ������� ��� ���: %s\n", buf_pin);

    strcpy(out_msg, gsm_pw);
    strcat(out_msg, "31");
    strcat(out_msg, buf_pin);

    printf("INFO ������������ ��������� %s\n", out_msg);

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
            printf("INFO ������ ��� ������ ��� ���������\n");
            return false;
        }
    }
    else
    {
        if (!p_gsm->sendSms(m_gen6_phones[DEVICE_PHONE_NO_1],
                            out_msg,
                            TIME_DELAY::MIN_SMS_STATUS_CONFIRMATION_TIME_MS))
        {
            printf("INFO ������ �������� ���\n");
            return false;            
        }
    }

    return true;
}

bool Gen6Basic::registerMobile()
{
    bool result = false;

    printf("INFO ����������� ���������� ���������� (BLE)\n");

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
        printf("INFO ��������� ���������� (BLE) �� ����������������\n");

        return false;
    }

    return true;
}

bool Gen6Basic::registerTag()
{
    printf("INFO ����������� �����\n");

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
    printf("INFO ����������� ���. ������\n");

    if (p_led_keyring == NULL)
    {
        printf("DEBUG LED ������ �� ���������\n");
        return false;
    }

    p_led_keyring->pushButtons12(KEYRING_SHORT);

    return waitServicePulseAmount(2, TIME_DELAY::STD_REGISTER_DEVICE_TIME_S);
}

bool Gen6Basic::registerLcdKeyring()
{
    printf("INFO ����������� �� ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
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


    printf("INFO ����������� R6\n");

    if (p_r6 == NULL)
    {
        printf("FAIL R6 �� ���������������\n");
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


    printf("INFO ����������� R6 ECO\n");

    if (p_r6eco == NULL)
    {
        printf("FAIL R6 ECO �� ���������������\n");
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
        printf("FAIL �������� ��������: ��� �� ���������\n");
    }

    timer.start();

    float    begin        = timer.read();
    float    end          = 0.0;
    float    pulse        = begin;
    float    curr_pulse   = 0.0;
    uint32_t buf_count    = count;

    printf("INFO �������� %d ��������(��) (����������: %d, �����: %.2f)\n",
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
                    printf("INFO %d-� ������ ������� ����� %.2f ���\n",
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
        printf("INFO �� �������� %d ���������\n", count);
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
        printf("INFO �������� ��������: ��� �� ���������\n");
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
            printf("INFO DTMF ������� %s ���������� �������\n", msg);
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
    printf("INFO �������� ��� ����������� ������: %s\n", m_device_pin);
}

char *Gen6Basic::getDevicePin()
{
    return m_device_pin;
}

char *Gen6Basic::getWrongDevicePin(emergency_dis_pin_num_t pin_num)
{
    printf("INFO ��������� ��������� ���� ����������� ������\n");

    // ������ ��� �������� ������� PIN, �.�. m_wrong_device_pin ��� ��� �������� �� �����
    strncpy(m_wrong_device_pin,
            m_device_pin,
            sizeof(m_wrong_device_pin));

    // ��� ����� � ���� ��������
    if (pin_num == EMERGENCY_DIS_PIN_NUM_ALL)
    {
        printf("INFO ��������� ��������� ���� ����������� ������: ������ ����� ����� ������� �� ������������\n");

        for (uint8_t i = 0; i < 4; ++i)
        {
            m_wrong_device_pin[i] -= 0x30;                           // ������������� ������ ����� � �����, �������� ASCII
            m_wrong_device_pin[i] = (m_wrong_device_pin[i] % 9) + 1; // ��������� 1, ������� ������������
            m_wrong_device_pin[i] += 0x30;                           // ������������� ����� � ������ �����, �������� ASCII
        }
    }
    // ���� ����� � ���� ��������
    else
    {
        if (   pin_num < 1
            || pin_num > 4)
        {
            printf("INFO �������� ����� ����� ���� ����������� ������: %d\n", pin_num);

            pin_num = EMERGENCY_DIS_PIN_NUM_FIRST;
        }

        printf("INFO ��������� ��������� ���� ����������� ������: ����� �%d ����� ������� �� ������������\n", pin_num);

        m_wrong_device_pin[pin_num - 1] -= 0x30;                                     // ������������� ������ ����� � �����, �������� ASCII
        m_wrong_device_pin[pin_num - 1] = (m_wrong_device_pin[pin_num - 1] % 9) + 1; // ��������� 1, ������� ������������
        m_wrong_device_pin[pin_num - 1] += 0x30;                                     // ������������� ����� � ������ �����, �������� ASCII
    }

    printf("INFO ������� �������� ��� ����������� ������: %s\n", m_wrong_device_pin);

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
    printf("INFO ����� �������� ��: %.2f (�����������: %.2f)\n",
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

                printf("FAIL ����������� �������� ��������� ����� %.2f\n", time_s);
            }
            else
            {
                printf("PASS ����������� �������� ��������� ����� %.2f\n", time_s);
            }
        }
        else 
        {
            result = false;

            printf("FAIL ����������� �������� �� ��������� ����� �������� %.2f\n", time_s);
        }            
    }
    else
    {
        if (pn_status == 0) 
        {
            printf("PASS ����������� �������� ��������\n");
        }
        else
        {
            result = false;

            printf("FAIL ����������� �������� �� ��������\n");
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
            printf("PASS �� ��������\n"); 
        }
        else
        {
            printf("FAIL �� ���������\n");   
        }
    }
    else
    {
        if (getSupportIgn() == 0) 
        {
            printf("PASS �� ���������\n"); 
        }
        else
        {
            printf("FAIL �� ��������\n");   
        }
    }
}

void Gen6Basic::checkTurboTimer(int8_t model)
{
    if (model != 0)
    {
        if (getTurboTimer() != 0) 
        {
            printf("PASS �� �������\n"); 
        }
        else
        {
            printf("FAIL �� ��������\n");   
        }
    }
    else
    {
        if (getTurboTimer() == 0) 
        {
            printf("PASS �� ��������\n"); 
        }
        else
        {
            printf("FAIL �� �������\n");   
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
            printf("PASS ������� ���������\n");
        }
        else
        {
            printf("FAIL ������� �� ���������\n");
        }
    }
    else
    {
        if (getAlarmSignal(timeout) == NOSIGNAL)
        {
            printf("PASS ������� �� ���������\n");
        }
        else
        {
            printf("FAIL ������� ���������\n");
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

    printf("INFO �������� ������� �� ��������: %s\n", firmware);
    printf("SET FIRMWARE %s \n", firmware);

    if (UartInteract::getString(p_serial, buf, time))
    {
        if (strstr(buf, "FIRMWARE_LOAD_OK") != NULL)
        {
            printf("INFO ����� �� �������� �������� �������\n");
            result = true;
        }
    }

    return result;
}

bool Gen6Basic::parseFirmwareLoadError(const char *buf)
{
    // ���������� ���������
    if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_OK) != NULL)
    {
        printf("INFO ����� �� �������� �������� �������\n");
    }
    else
    {
        printf("INFO ����� �� �������� �������� �� �������\n");

        // ���������� ����� ������
        if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_ERROR_SL_MASTER))
        {
            printf("FAIL ��������� ���������� �������� ����� StarLine ������\n");
        }
        else if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_ERROR_NOT_FOUND))
        {
            printf("FAIL ������������� �������� �� ������� �� �������\n");
        }
        else if (strstr(buf, FW_LOAD_ERROR::FW_LOAD_ERROR_ARG))
        {
            printf("FAIL �������� �� ��� ��������� ��� �������� ��������\n");
        }
        else
        {
            printf("FAIL ������� ����������� ����� ��� �������� ��������: %s\n", buf);
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

    printf("INFO �������� ������� �� �������� �� ����������\n");

    switch (fw_download_type)
    {
        case FW_DOWNLOAD_TYPE_OLDEST:
            printf("DEBUG �������� ������� �� ���������� ����� ������ ������ �� �� �������\n");
            printf("LOAD FIRMWARE OLDEST\n");
            break;

        case FW_DOWNLOAD_TYPE_SECOND_PREV:
            printf("DEBUG �������� ������� �� ���������� ������ ���������� ������ ��\n");

            if (special_fw_ver != NULL)
            {
                printf("DEBUG ����� ������� ������ ���������� ������ �� ������������ ��������: %s\n", special_fw_ver);
                printf("LOAD FIRMWARE SECOND_PREV %s\n", special_fw_ver);
            }
            else
            {
                printf("DEBUG ����� ������� ������ ���������� ������ �� ������������ �������\n");
                printf("LOAD FIRMWARE SECOND_PREV\n");
            }

            break;

        case FW_DOWNLOAD_TYPE_PREV:
            printf("DEBUG �������� ������� �� ���������� ���������� ������ ��\n");

            if (special_fw_ver != NULL)
            {
                printf("DEBUG ����� ������� ���������� ������ �� ������������ ��������: %s\n", special_fw_ver);
                printf("LOAD FIRMWARE PREV %s\n", special_fw_ver);
            }
            else
            {
                printf("DEBUG ����� ������� ���������� ������ �� ������������ �������\n");
                printf("LOAD FIRMWARE PREV\n");
            }

            break;

        case FW_DOWNLOAD_TYPE_LATEST_BETA:
            printf("DEBUG �������� ������� �� ���������� ��������� ���� ������ �� �� �������\n");
            printf("LOAD FIRMWARE LAST BETA\n");
            break;

        case FW_DOWNLOAD_TYPE_LATEST_RELEASE:
            printf("DEBUG �������� ������� �� ���������� ��������� �������� ������ �� �� �������\n");
            printf("LOAD FIRMWARE LAST RELEASE\n");
            break;

        default:
            printf("INFO ����������� ��� ���������� ��������: %d\n", fw_download_type);
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

    printf("INFO �������� ������� ��� ��������� ������ ��\n");
    printf("CHECK HOT_FIRMWARE \n");

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        if (strstr(buf, "FIRMWARE_CHECK_OK") != NULL)
        {
            printf("INFO ����� �� �������� �������� �������\n");
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

    printf("INFO �������� ������� �� �������� �� ����������\n");
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
        printf("INFO ������ �� �� �������\n");
        return false;
    }

    memset(buf,
           0,
           buf_size);

    printf("INFO �������� ������� �� �������� �� ���������� ������: %s\n", version);
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
    const char *delimiter = "_";   // ����������� �������
    char       *token;             // ��������� �� ������ ������
    char        buf[255];


    if (version == NULL)
    {
        printf("INFO ����� ��� ������ �� ���������������\n");
        return version;
    }

    memset(buf, 0, 255);
    printf("INFO �������� ������� �� ��������� ������ �� ����������\n");

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
        // ������ ������������ ����� "FIRMWARE_GET_%s_OK\n", ��� %s - ������ (����.: FIRMWARE_GET_2.14.0_OK)
        if (strstr(buf, "FIRMWARE_GET") != NULL)
        {
            token = strtok(buf,  delimiter); // �������� ������ ����� (FIRMWARE)
            token = strtok(NULL, delimiter); // �������� ������ ����� (GET)
            token = strtok(NULL, delimiter); // �������� ������ ����� (������, ����.: 2.14.0)
            sprintf(version, "%s", token);   // �������� �����

            if (version != NULL)
            {
                printf("INFO �������� ������ ��������: %s\n", version);
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

    printf("INFO �������� ������� �� ������������ ������ ��\n");
    printf("SET RELOAD_FIRMWARE \n");

    timer.start();
    float begin = timer.read();
    float end   = begin;

    while (end < time)
    {
        if (p_serial->readable() && p_serial->gets(command, c_max_command_size - 1))
        {
            printf("TRACE �������� ������: %s\n", command);
        }

        if (strstr(command, "FIRMWARE_RELOAD_OK") != NULL)
        {
            printf("INFO ����� �� �������� ������������ ������ �� �������\n");
            result = true;
            break;
        }
        else if (strstr(command, "FIRMWARE_RELOAD_ERROR") != NULL)
        {
            break;
        }

        end = timer.read() - begin;
    }

    printf("DEBUG �������� ������: %.2f\n", end);

    timer.stop();

    return result;
}

bool Gen6Basic::loadCanFirmware(const char *can_car_num_str, float timeout_s)
{
    const char    *cmd_can_update = "LOAD CAN"; // ������� �� ���������� �������������� �������� CAN
    const uint8_t  buf_size       = 255;
    char           buf[buf_size];


    memset(buf,
           0,
           buf_size);

    printf("INFO �������� ������� �� ���������� �������������� �������� CAN (����������: %s)\n", can_car_num_str);

    // ��������� ������� �� ���������� �������������� �������� CAN, ������ �������: LOAD CAN 9999
    printf("%s %s \n",
           cmd_can_update,
           can_car_num_str);

    if (UartInteract::getString(p_serial,
                                buf,
                                timeout_s))
    {
        if (strstr(buf, "CAN_LOAD_OK") != NULL)
        {
            printf("INFO ���������� �������������� �������� CAN �������\n");

            return true;
        }
    }

    printf("INFO ���������� �������������� �������� CAN ���������\n");

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
    printf("DEBUG ������������ ��������� ����� ����� ����������: %.2f\n", max_pulse_interval);

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
            printf("FAIL ���������� �������� ���. ������: ��������� �� ������ �� ���������\n");
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
            printf("FAIL ���������� �������� �� ������: ��������� �� ������ �� ���������\n");
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
                    printf("INFO ��� �� ��������. ���������� ����� ����������� �� CAN\n");
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
    printf("INFO ��������� �������� \n");
    p_can_actions->sendSpeed(value);
}

bool Gen6Basic::checkSpeed(int8_t speed_kmph, float timeout_s)
{
    uint16_t speed;

    speed = (uint16_t)speed_kmph; // �� ������������ �������, ������ DIAG_DATA_SPEED 16 ���

    // �� ������������ �������, ������� ��������� ��/� / 100
    // �� ��� ����������� ���������, ������� ��������� ����� ��/�(� ������������ �� �������)
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
            printf("INFO ������� ��������� ������ ��������������\n");
            result = true;
            return result;
        }
    }

    if (getAntiHijacking() == AHJ_STATUS_OFF)
    {
        printf("INFO �������������� ��������� (�������)\n");
    }
    else
    {
        if (getAntiHijacking() != status)
        {
            printf("INFO �� ������� ��������� ������ ��������������\n");
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
            printf("INFO �������� ������� ��������\n");
            break;
        }
    }
        
    while (timer.read() - begin < time)
    {
        if (getAlarm())
        {
            printf("INFO �������� ������ �������\n");
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
    printf("INFO ���������� ������ ��������������\n");

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
                printf("PASS ������� ������ �� ������ �������������� �������\n");
            }
            else
            {
                printf("FAIL ������� ������ �� ������ �������������� �� �������\n");
            }
            break;
                                    
        case SLO:
            setStarlineOnlineParam(StarlineOnline::HIJACK, OFF, m_device_pin);
            break;
                
        default:
            printf("FAIL ����������� ����� ��������� ��������������\n");
            return;
    }
}

void Gen6Basic::onAntiHijacking(command_type_t  execution_method,
                                bool            confirmation,
                                const char     *gsm_pass,
                                bool            ahj_check)
{
    printf("INFO ��������� ������ ��������������\n");
        
    switch (execution_method)
    {
        case KEYRING:
            printf("INFO ��������� ������ �������������� �� �������\n");
            setLcdKeyringVcc(OFF);
            setIgn(PIN, ON);
            setDoor(OPEN, PIN);
            setMoveSensor();
            printf("INFO �������� �������������� �� ������� ��������\n");
            break;

        case TAG:
            printf("INFO ��������� ������ �������������� �� �����\n");
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
                printf("INFO �������� �������������� �� ����� ��������\n");
            }
            else
            {
                printf("INFO ����� � ���� ���������\n");
            }

            break;

        case SMS:
            printf("INFO ��������� ������ �������������� �� ���\n");

            if (setAnj(true, m_device_pin))
            {
                printf("PASS ������� ����� � ����� �������������� �������\n");
            }
            else
            {
                printf("FAIL ������� ����� � ����� �������������� �� �������\n");
            }
            break;

        case DTMF:
            printf("INFO ��������� ������ �������������� �� DTMF\n");

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
                            printf("INFO DTMF ������� %s ���������� �������\n", gsm_pass);
                            wait(TIME_DELAY::FIVE_S);
                        }

                        if (p_gsm->sendDtmfTone(m_gen6_phones[DEVICE_PHONE_NO_1], "31", 2))
                        {
                            printf("INFO DTMF ������� 31 ���������� �������\n");

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
            printf("INFO ��������� ������ �������������� �� ����� ������\n");

            setStarlineOnlineParam(StarlineOnline::HIJACK, ON, m_device_pin);
            break;

        case MOBILE:
            printf("INFO ��������� ������ �������������� ����� ��������� ���������� (BLE)\n");
            p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_ANTIROBBERY_ON);
            break;

        default:
            printf("FAIL ����������� ����� ��������� ��������������\n");
            return;
    }
}

void Gen6Basic::setCapSensor(bool state)
{
    if (p_out_cap_sensor == NULL)
    {
        printf("DEBUG ������ ����� ����� �� ���������\n");
        return;
    }

    char msg[6];

    memset(msg, 0, 6);

    if (state)
    {
        strcpy(msg, "���.");
    }
    else
    {
        strcpy(msg, "����.");
    }

    printf("DEBUG ���������� �������� ����� �����: %s\n", msg);

    *p_out_cap_sensor = state;
}

void Gen6Basic::setCapSensor(cap_sensor_type_t cap_sensor_type, bool state)
{
    DigitalOut        *sensor_pin;                             // ��������� �� ���, ���������� �� ������������ ������
    std::string        sensor_type_str;                        // ��������� ������������� ���� ������������� �������
    const std::string  state_str = state == ON ? "���������"   // ��������� ������������� ������������ ��������� (���/����)
                                               : "����������";


    switch (cap_sensor_type)
    {
        case CAP_SENSOR_TYPE_DEFAULT:
            sensor_type_str = "����� �����";
            sensor_pin      = p_out_cap_sensor;
            break;

        case CAP_SENSOR_TYPE_ARM:
            sensor_type_str = "����� ����� (����������)";
            sensor_pin      = p_out_cap_sensor_arm;
            break;

        case CAP_SENSOR_TYPE_DISARM:
            sensor_type_str = "����� ����� (������)";
            sensor_pin      = p_out_cap_sensor_disarm;
            break;

        case CAP_SENSOR_TYPE_TRUNK:
            sensor_type_str = "����� ���������";
            sensor_pin      = p_out_cap_sensor_trunk;
            break;

        default:
            printf("FAIL ����������� ��� �������: %d\n", cap_sensor_type);
            return;
    }

    if (sensor_pin != NULL)
    {
        printf("INFO %s ������� %s\n",
               state_str.c_str(),
               sensor_type_str.c_str());

        *sensor_pin = state;
    }
}

void Gen6Basic::sendCapSensorPulse(bool polarity, float duration_s)
{
    printf("TRACE �������� �������� ������� ����� �����\n");
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
    printf("INFO ������� ������� ����� %d ���, ������������ ������� �������: %.2f ���.\n",
           count,
           press_time_s);

    for (int8_t i = 0; i < count; ++i)
    {
        setCapSensor(cap_sensor_type, ON);
        wait(press_time_s);
        setCapSensor(cap_sensor_type, OFF);

        wait(TIME_DELAY::STD_WIRE_EN_DELAY_S); // ����� ����� ���������
    }
}

bool Gen6Basic::waitCapSensor(cap_sensor_type_t cap_sensor_type,
                              bool              state,
                              float             timeout_s)
{
    const std::string            state_str = state ? "�������"   // ��������� ������������� ���������� ��������� �������
                                                   : "��������";

    std::string                  sensor_type_str;                // ��������� ������������� ���� ������������ �������
    slp_diag_data_zone_sysdata_t diag_data_zone;                 // ����������� ����


    switch (cap_sensor_type)
    {
        case CAP_SENSOR_TYPE_DEFAULT:
            sensor_type_str = "����� �����";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR;
            break;

        case CAP_SENSOR_TYPE_ARM:
            sensor_type_str = "����� ����� (����������)";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR_ARM;
            break;

        case CAP_SENSOR_TYPE_DISARM:
            sensor_type_str = "����� ����� (������)";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR_DISARM;
            break;

        case CAP_SENSOR_TYPE_TRUNK:
            sensor_type_str = "����� ���������";
            diag_data_zone  = SYSDATA_ID_ZONE_CAP_SENSOR_TRUNK;
            break;

        default:
            printf("FAIL ����������� ��� �������: %d\n", cap_sensor_type);
            return false;
    }

    printf("INFO �������� ��������� ������� %s, ��������� ���������: %s\n",
           sensor_type_str.c_str(),
           state_str.c_str());

    if (waitSlpCmdStatus(DIAG_DATA_ZONE_SYSDATA,
                         SLP_STATUSES_SIZES::DIAG_DATA_ZONE_SYSDATA_SIZE,
                         diag_data_zone,
                         state,
                         timeout_s))
    {
        printf("INFO ��������� �������������� ����������\n");
    }
    else
    {
        printf("INFO ��������� �� �������������� ����������\n");
        return false;
    }

    return true;
}

void Gen6Basic::setArmBeachMode(bool  polarity,
                                float p1_duration_s,
                                float p2_duration_s,
                                float p3_duration_s)
{
    printf("INFO �������� ���������� � ������ ��������� ������� (����)\n");

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

    printf("INFO �������� ������ � ������ (����)\n");

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
    
    printf("INFO ��������\n");

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
    
    printf("INFO ��������\n");

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
    
    printf("INFO ��������\n");

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
        printf("INFO ��������\n");

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
            printf("FAIL ��� ����������� ������ �� �����\n");
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

    printf("INFO ��� ����������� ������:");

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
                    printf("INFO ������ �������� �� ����� ����������. ������������ %.2f ���\n", delay_light_l);
                    *p_out_light_left = polarity;
                    wait(delay_light_l);
                    *p_out_light_left = !polarity;
                }

                break;
                
            case SLAVE_LIGHT_R:
                if (p_out_light_right != NULL)
                {
                    printf("INFO ������ �������� �� ������ ����������. ������������ %.2f ���\n", delay_light_r);
                    *p_out_light_right = polarity;
                    wait(delay_light_r);
                    *p_out_light_right = !polarity;
                }

                break;
              
            case SLAVE_LIGHTS:
                if (p_out_lights != NULL)
                {
                    printf("INFO ������ �������� �� ����� ���� ������������. ������������ %.2f ���\n", delay_light_r);
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
                        printf("INFO ������ �������� �� ����� � ������ �����������. ������������ %.2f ���\n", delay_light_r);
                        *p_out_light_right = polarity;
                        *p_out_light_left  = polarity;
                        wait(delay_light_r);
                        *p_out_light_right = !polarity;
                        *p_out_light_left  = !polarity;
                    } 
                    else if (delay_light_r > delay_light_l)
                    {
                        printf("INFO ������ �������� �� ����� � ������ �����������. ������������ �������=%.2f ���, ������=%.2f ���\n", delay_light_r, delay_light_l);
                        *p_out_light_right = polarity;
                        *p_out_light_left  = polarity;
                        wait(delay_light_l);
                        *p_out_light_right = !polarity;
                        wait(delay_light_r - delay_light_l);
                        *p_out_light_left  = !polarity;
                    }
                    else if (delay_light_r < delay_light_l)
                    {
                        printf("INFO ������ �������� �� ����� � ������ �����������. ������������ �������=%.2f ���, ������=%.2f ���\n", delay_light_r, delay_light_l);
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
    printf("INFO �������� ����� �� � �������������. ������������ %.2f ���\n",
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
    const char* message = value ? "INFO ������ ������� �� ��������� ������ ����� ���������� Slave\n"
                                : "INFO ������ ������� �� ���������� ������ ����� ���������� Slave\n";

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

        printf("INFO �������� ����� �� � �������������. ������������ %.2f ���\n",
               delay_between);
        wait(delay_between);
    }
    
    if (p_out_central_lock_open != NULL || p_out_central_lock_close != NULL)
    {
        if (value == ON) 
        {
            printf("INFO ������ �������� �� �� �������. ������������ %.2f ���\n",
                   delay_clock);
            *p_out_central_lock_close = polarity;
            wait(delay_clock);
            *p_out_central_lock_close = !polarity;
        }
        else
        {
            printf("INFO ������ �������� �� �� �������. ������������ %.2f ���\n",
                   delay_clock);
            *p_out_central_lock_open = polarity;
            wait(delay_clock);
            *p_out_central_lock_open = !polarity;
        }
    }
    else
    {
        printf("INFO �������� ��: ������� �� �������� ��� �������� ��: ������� �� ��������\n");
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
    Timer check_light_width_timer; // ��� ���������� ����

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
            printf("INFO ��� ��������� ����� ������/���������� ������ �� ���������\n");
            return result;
    }

    printf("INFO �������� ��������� ��������, �������� �������� � ��\n");
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
                printf("INFO ������� �������� �� �������\n");

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
                printf("INFO ������� �������� �� �������\n");

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
            printf("INFO %d �������� ������� �������\n", ++signal_no);

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
            printf("INFO %d ������� �������� �������� �������\n", ++light_no);

            is_light = false;
        }

        if (   (signal_no == signal_count)
            && (light_no == light_count)
            && is_clock)
        {
            printf("INFO ��������������� ���������� ��������/�������� �������� ��������\n");

            result = true;
            break;
        }

        curr_time = timer.read() - begin;
    }
    
    // ��� ������� ����� ������ ������������� �������� ��
    // ����� ������� �� ��� �����, ������� ��������� ����� �����(��������)
    if (indication == CAP_SENSOR_INDICATION)
    {
        if (   (signal_no == signal_count)
            && (light_no  == light_count))
        {
            if (lock_indication)
            {
                if (!is_clock)
                {
                    printf("INFO ��������������� ���������� �������� ������������/������ ��������\n"); // ����� �����, �.�. ��� is_clock ��� �������� ������(� �����)
                    printf("INFO ������� �� �����������\n");
                    
                    result = true;
                }
                else
                {
                    printf("INFO ������� �� ������������\n");
                    
                    result = false;
                }
            } // if lock_indication
        } // if signal_no == signal_count
        else
        {
            printf("INFO C������ ������������/������ ��������������� ���������� ����������\n");
            
            if (lock_indication && is_clock)
            {
                printf("INFO ������� �� ������������\n");
            }
            
            result = false;
        }
    } // if indication == CAP_SENSOR_INDICATION

    if ((time_error_s > TIME_DELAY::NULL_DELAY_S) && (curr_time < time_s - time_error))
    {
        printf("INFO �������� �������� �� ��������� ��������� %.2f +- %.2f � (�������� %.2f �)\n",
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
            printf("FAIL �������� ���� ������: %d\n", buff);
            return "1";
    }
}

bool Gen6Basic::getSysTime(time_t &sys_time)
{
    char *ch;

    char buf[255];

    memset(buf, 0, 255);

    printf("INFO ������ �� ��������� ���������� �������\n");

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

            printf("INFO ����� �� �������� ��������� ���������� ������� ������� (%d)\n", sys_time);

            return true;
        }
    }

    printf("INFO ����� �� �������� ��������� ������� �� �������\n");

    sys_time = time(NULL);

    return false;
}

bool Gen6Basic::waitGsmTime(float timeout_s)
{
    const char *default_hour  = "00"; // ��� � ������� ����� ������
    const char *default_day   = "01"; // ���� � ������� ����� ������
    const char *default_month = "01"; // ����� � ������� ����� ������

    char        hour_buf[3];          // ����� ��� �������� �������� ���� � �������
    char        day_buf[3];           // ����� ��� �������� �������� ��� � �������
    char        month_buf[3];         // ����� ��� �������� �������� ������ � �������
    char        datetime_buf[32];     // ����� ��� �������� ������ ������� ���� � �������

    Timer       timer;                // ������, ��� ������� ��������
    time_t      sys_time_s;           // ������� ����� �������
    struct tm  *timeinfo;             // ������� ����� ������� � ������ �������

    bool        is_gsm_init = false;  // ����������������� �� GSM (��������� ����� ������� �� �������� �� ���������)


    printf("INFO �������� ��������� �������� ������� �� GSM\n");
    printf("INFO ����������: ���� ������� ��� = %s, ���� = %s, ����� = %s, �� �������� ����� ����������\n",
           default_hour,
           default_day,
           default_month);

    if ((m_device_modules & GSM_MODULE) != GSM_MODULE)
    {
        printf("INFO ����������� ������ GSM\n");
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
        printf("INFO �������� ����� �������: %s\n", datetime_buf);

        // ������� � �������� ����
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

        // ������� � �������� ���
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

        // ������� � �������� ������
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

        printf("TRACE �������� ��������� ����� ����� ��������� �� ��������� ������� �������\n");
        wait(TIME_DELAY::WAITING_REGION_STATE_S); // ����� ����� ��������� ��������� �������, ����� �� ������� ����� �������� ����
    } // while

    timer.stop();

    if (is_gsm_init)
    {
        printf("INFO ����� �� GSM ��������\n");
    }
    else
    {
        printf("INFO ����� �� GSM �� ��������\n");
    }

    printf("INFO ������ �������: %.2f ���.\n", timer.read());

    return is_gsm_init;
}

void Gen6Basic::wait_s(float time_s)
{
    Timer timer;

    timer.start();

    float begin = timer.read();
    float end   = begin;

    printf("INFO ������ �������� %.2f ���.\n", time_s);

    while (end < time_s)
    {
        end = timer.read() - begin;
    }

    timer.stop();

    printf("INFO �������� ���������\n");
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
            printf("INFO ������� \"�������������� �� ���������� �����\" %s\n", state ? "���" : "����");
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
    uint8_t srv_led_count = COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT; // ���������� �������� ���������� ����������


    if (state)
    {
        printf("INFO ��������� ������ \"������� ��������\"\n");
        srv_led_count += COUNT_STRUCT::DANGER_PARKING_EN_SIGNALS_COUNT;
    }
    else
    {
        printf("INFO ���������� ������ \"������� ��������\"\n");
        srv_led_count += COUNT_STRUCT::DANGER_PARKING_DIS_SIGNALS_COUNT;
    }

    if (checkDangerParking(state))
    {
        printf("INFO ����� \"������� ��������\" ��� ��������� � �������� ���������\n");
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

    result = p_service_btn->waitPulseAmount(srv_led_count, TIME_DELAY::DANGER_PARKING_EN_TIME_S); // �������� ����� � �� (� �������� ���������� ����������)

    setIgn(PIN, OFF);
    setIgn(CAN_USE, OFF);

    // �� X96v2 ��������� �� �����������
    if (m_device_name != GEN6_X96V2)
    {
        if (result)
        {
            printf("INFO ������� ���������� ���������� ������������� ���������\n");
        }
        else
        {
            printf("INFO ������� ���������� ���������� ��������������� ���������\n");
        }
    }
    else
    {
        printf("INFO ��������� ��������� �� ����������� �� ������� X96v2\n");
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
    const uint8_t exp_srv_btn_led_count = state ? COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT + COUNT_STRUCT::DANGER_PARKING_EN_SIGNALS_COUNT   // ��������� ���������� ������� ���������� ����������
                                                : COUNT_STRUCT::BTN_MODE_DANGER_PARKING_CLICKS_COUNT + COUNT_STRUCT::DANGER_PARKING_DIS_SIGNALS_COUNT;

    const uint8_t exp_siren_count       = exp_srv_btn_led_count;                                                                                       // ��������� ���������� �������� ������

    const uint8_t exp_lights_count      = state ? COUNT_STRUCT::DANGER_PARKING_EN_SIGNALS_COUNT                                                        // ��������� ���������� �������� �����
                                                : COUNT_STRUCT::DANGER_PARKING_DIS_SIGNALS_COUNT;

    Timer   timer;
    bool    result                  = true;                     // ���������

    bool    is_srv_led_on           = false;                    // ������� �� ��������� ���������
    bool    is_siren_on             = false;                    // �������� �� ������
    bool    is_lights_on            = false;                    // ������� �� ����

    uint8_t srv_btn_led_count       = 0;                        // ���������� ������� ���������� ����������
    uint8_t siren_signal_count      = 0;                        // ���������� �������� ������
    uint8_t lights_signal_count     = 0;                        // ���������� �������� �����

    float   srv_led_time_s          = TIME_DELAY::NULL_DELAY_S; // ����� ��������� ���������� ���������� ��������� ������
    float   siren_potential_time_s  = TIME_DELAY::NULL_DELAY_S; // ����� ��������� ���������� ������
    float   lights_potential_time_s = TIME_DELAY::NULL_DELAY_S; // ����� ��������� ���������� �����

    // ����������� ������� ��������� �������� ��������
    int8_t (Gen6Basic::*getSirenSignal)(void) = m_device_name == GEN6_X96V2 ? &Gen6Basic::getBuzzerExt // ��� X96v2 ������������� ������� ������� �� �����
                                                                            : &Gen6Basic::getSiren;    // ��� ��������� ������ - �� ������

    timer.start();

    while (timer.read() < timeout_s)
    {
        // ��������� ���������� ����������
        if (   !is_srv_led_on
            &&  p_service_btn->isLedOn())
        {
            is_srv_led_on  = true;
            srv_led_time_s = timer.read();
        }

        // ������� ���������� ����������
        if (    is_srv_led_on
            && !p_service_btn->isLedOn()
            &&  timer.read() - srv_led_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            is_srv_led_on = false;
            printf("INFO ������� %d ������ ���������� ����������\n", ++srv_btn_led_count);
        }

        // ��������� ������
        if (   !is_siren_on
            && (this->*getSirenSignal)())
        {
            is_siren_on            = true;
            siren_potential_time_s = timer.read();
        }

        // ������� ������
        if (     is_siren_on
            && !(this->*getSirenSignal)()
            &&   timer.read() - siren_potential_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            is_siren_on = false;
            printf("INFO ������� %d ������ ������\n", ++siren_signal_count);
        }

        // ��������� �����
        if (   !is_lights_on
            &&  getLights())
        {
            is_lights_on            = true;
            lights_potential_time_s = timer.read();
        }

        // ������� �����
        if (    is_lights_on
            && !getLights()
            &&  timer.read() - lights_potential_time_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
        {
            is_lights_on = false;
            printf("INFO ������� %d ������ �����\n", ++lights_signal_count);
        }
    }

    // �� X96v2 ��������� ��������� �� �����������
    if (m_device_name != GEN6_X96V2)
    {
        if (srv_btn_led_count != exp_srv_btn_led_count)
        {
            result = false;
            printf("FAIL ���������� �������� ���������� ���������� ��������������� ����������. ��������: %d, ���������: %d\n",
                   srv_btn_led_count,
                   exp_srv_btn_led_count);
        }
    }
    else
    {
        printf("INFO ��������� ��������� �� ����������� �� ������� X96v2\n");
    }

    if (siren_signal_count != exp_siren_count)
    {
        result = false;
        printf("FAIL ���������� �������� ������ ��������������� ����������. ��������: %d, ���������: %d\n",
               siren_signal_count,
               exp_siren_count);

    }

    if (lights_signal_count != exp_lights_count)
    {
        result = false;
        printf("FAIL ���������� �������� ����� ��������������� ����������. ��������: %d, ���������: %d\n",
               lights_signal_count,
               exp_lights_count);

    }

    if (result)
    {
        printf("PASS ���������� ������� ������������� ���������\n");
    }
    else
    {
        printf("FAIL ���������� ������� ��������������� ���������\n");
    }

    return result;
}

bool Gen6Basic::setBleSettingMode(bool state)
{
    bool result = false;


    if (state)
    {
        printf("INFO ��������� ������ ��������� �� BLE\n");
    }
    else
    {
        printf("INFO ���������� ������ ��������� �� BLE\n");
    }

    if ((getModules() & BLE_MODULE) != BLE_MODULE)
    {
        printf("FAIL ����������� BLE ������\n");
        return false;
    }

    if (checkBleSettingMode(state))
    {
        printf("INFO ����� ��������� �� BLE ��� ��������� � �������� ���������\n");
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
    printf("INFO ���������� ������� ��� �������� ����� ����������. ����� ����������: %d\n", execution_method);

    execution_method = getExecutionMethod(execution_method, USER_ACTION);

    printf("INFO ����� ���������� ����� ��������� (��� ���������� ����������� �������): %d\n", execution_method);

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
            printf("FAIL ����������� ����� ���������� ��� ���������� �������: %d\n", execution_method);
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
        printf("INFO ��������� ��������� ����� ������\n");
    }
    else
    {
        printf("INFO ��������� ��������� �������\n");
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
            printf("INFO ��� ���������� �� ���������\n");
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
        printf("FAIL ���������� ����� �������� �� ��������\n");

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
        // ����������� ��� ���������� �������������
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
        printf("FAIL �� ������ � ������ ����������������� %d ���������� ���������� ���������� ��� ��������� ������������ ��������\n",
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
        printf("FAIL �� ������� � ������ ����������������� %d ���������� �������� ������������ ��������\n", gen6_functional);

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
        printf("FAIL �� ������� � ������ ����������������� %d ���������� �������� �������\n", gen6_functional);

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
                printf("FAIL ��������� �������: ������ ��������.\n");
                return false;
            }
            
            printf("PASS ��������� �������: ������ �� ��������.\n");
            break;

        case ON:
            setSecure(OFF, KEYRING);
            setSecure(ON, KEYRING);

            if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                                 SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                                 DIAG_GUARD_STATE_ARM))
            {
                printf("FAIL ��������� �������: ������ �� ��������.\n");
                return false;
            }
            
            printf("PASS ��������� �������: ������ ��������.\n");
            break;

        default:
            printf("PASS ��������� ������� �� ������.\n");
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


    printf("INFO ������ �������� �������� � �������� ��������\n");
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
            printf("INFO ��������� ������ � �������� �������� ������������� �����������\n");
            return true;
        }
    }

    timer.stop();

    if (!light_indication && !siren_indication)
    {
        printf("INFO ��������� ������ � �������� �������� ������������� �����������\n");
        return true;
    }

    printf("INFO ��������� ������ � �������� �������� �� ������������� �����������\n");

    return false;
}

bool Gen6Basic::waitLightsImpulseOrPotentialStart(float wait_time_s)
{
    Timer timer;


    printf("INFO �������� ��������� �������� �������� (���������� ��� ������������� ����������)\n");
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
    bool   result    =   true;                  // ���������
    float  timeout_s =   pulse_wait_time_s      // ����� �������� ��������
                       + pulse_width_s;
    int8_t err_perc  =   5;                     // ������� ���������� ������ (���������� �� ����)
                                                // ����� ����� ���������, ��� �� ����� ������� ������� ���� ����� ��-�� ������ ��������� + ������ ����

    float  impulse_width_error_s;               // ������ ������������ ��������               (���������� ���������� ������)
    float  impulse_pause_error_s;               // ������ ������������ ����� ����� ���������� (���������� ���������� ������)
    float  potential_len_error_s;               // ������ ������������ ����������             (������������� ���������� ������)

    float  min_impulse_pause_s = pulse_width_s; // ����������� ����� ����� ����������    (���������� ���������� ������)
    float  min_potential_len_s = pulse_width_s; // ����������� ������������ ����������   (������������� ���������� ������)

    bool   is_impulse_start    = false;         // ��� �� ������� ������ ������ �������� (���������� ���������� ������)
    bool   is_potential_start  = false;         // ��� �� ������� ���������              (������������� ���������� ������)

    int8_t curr_impulse_count  = 0;             // ���������� ���������� ��������� ����� (���������� ���������� ������)
    bool   is_potential_end    = false;         // �������� ��������� �����              (������������� ���������� ������)

    float  impulse_start_time_s;                // ����� ��������� ������ ��������       (���������� ���������� ������)
    float  impulse_end_time_s;                  // ����� ����� ����������� ��������      (���������� ���������� ������)
    float  impulse_pause_s;                     // ������������ ����� ����� ����������   (���������� ���������� ������)
    float  impulse_width_s;                     // ������������ ����������� ��������     (���������� ���������� ������)
    float  expected_impulse_pause_s;            // ��������� ����� ����� ����������      (���������� ���������� ������)

    float  potential_start_time_s;              // ����� ������ ����������� ����������   (������������� ���������� ������)
    float  potential_len_s;                     // ������������ ����������� ����������   (������������� ���������� ������)


    // ���� ��������� ����� ���������� ������ ����������� - �� �������� � ��������
    if (pulse_width_s < lights_potential_len_s)
    {
        min_potential_len_s = lights_potential_len_s;
    }

    // ���� ��������� ����� �������� ������ ����������� - �� �������� � ��������
    if (pulse_width_s < lights_impulse_pause_s)
    {
        min_impulse_pause_s = lights_impulse_pause_s;
    }

    // ������ ���������
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

    printf("INFO �������� ��������� ����� (���������� � ������������� ����������)\n");
    timer.start();

    // ��������� �����:
    // 0 ��������, ��� ���� �� ���� ������ ��� ������� � �������� ������� �������� (pulse_wait_time_s)
    // 1 �������� �������������� ����������
    //     1.1 ������� �������� �������
    //         1.1.1 ��������� ��� �� ��� �������
    //         1.1.2 ��������� ��� ��� ������� � �������� ����� (������) -> ����� �� �����
    //
    //     1.2 ������� ��������� �������
    //         1.2.1 ����� ���������� ������
    //         1.2.2 ����� ���������� �������� (������) -> ����� �� �����
    //
    // 2 �������� ����������� ����������
    //     2.1 ������� �������� �������
    //         2.1.1 �������� ������� ������� ��������
    //         2.1.2 �������� ������� ������� �������� � �����������
    //             2.1.2.1 ������ ����� ����� ����������
    //             2.1.2.2 ����� ����� ���������� ���������
    //             2.1.2.3 ����� ����� ���������� �� ��������� (������) -> ����� �� �����
    //
    //     2.2 ������� ��������� �������
    //         2.2.1 ����� �������� ������
    //         2.2.2 ����� �������� �������� (������) -> ����� �� �����
    while (timer.read() < timeout_s)
    {
        // 0 ��������, ��� ���� �� ���� ������ ��� ������� � �������� ������� ��������
        if (    timer.read()       >  pulse_wait_time_s
            &&  curr_impulse_count == 0
            && !is_impulse_start
            && !is_potential_start
            && !is_potential_end)
        {
            printf("FAIL � ������� ������� �������� �� ���� �������� �� ������ ������� �����\n");
            result = false;
            break;
        }

        // 1   �������� �������������� ����������
        // 1.1 �������� ������� �������������� ����������
        if (   !is_potential_start
            &&  getLightsPotential())
        {
            // 1.1.1 ��������� ����� ��� �� ��� ������� - ��� ��� � ������ ����
            if (!is_potential_end)
            {
                potential_start_time_s = timer.read();
                is_potential_start     = true;
                printf("PASS ������� ��������� ����� (������������� ����������)\n");
            }
            // 1.1.2 ������� ������ ��������� �����
            else
            {
                printf("FAIL ������� ������ ��������� ����� (������������� ����������)\n");
                result = false;
                break;
            }
        }

        // 1.2 ��������� ������� �������������� ����������
        if (    is_potential_start
            && !is_potential_end
            && !getLightsPotential())
        {
            potential_len_s = timer.read() - potential_start_time_s;

            if (potential_len_s > TIME_DELAY::MIN_PULSE_WIDTH_S) // ������ ��������
            {
                // 1.2.1 ��������, ��� ����� ����������� ���������� ������������� �������� (� ������������ potential_len_error_s)
                if (   potential_len_s < min_potential_len_s + potential_len_error_s
                    && potential_len_s > min_potential_len_s - potential_len_error_s)
                {
                    is_potential_end   = true;
                    is_potential_start = false;
                    printf("PASS ������� �������� ������ (������������� ����������)\n");
                }
                // 1.2.2 ����� �������� ��������
                else
                {
                    printf("FAIL ������������ ����������� ���������� ����� �������� (������������� ����������): %.6f\n", potential_len_s);
                    result = false;
                    break;
                }
            }
        }

        // 2   �������� ����������� ����������
        // 2.1 ������� �������� ������� ��������
        if (   !is_impulse_start
            &&  getLightsImpulse())
        {
            impulse_start_time_s = timer.read();

            // 2.1.1 ������ �������
            if (curr_impulse_count == 0)
            {
                is_impulse_start = true;
                printf("PASS ������� ������ ��������� ����� (���������� ����������)\n");
            }
            // 2.1.2 ����������� ��������, ����� ���� ������ ���� ��������� �����
            else
            {
                impulse_pause_s = impulse_start_time_s - impulse_end_time_s;

                // 2.1.2.1 ������ ����� ����� ����������:
                // ��� ������ � ������ ����� �������� 4 �������� (��� ���� ���������)
                // ����� ����� ���������� � ���� (����� 1 � 2 ���������; ����� 3 � 4 ���������) ����� ����� min_impulse_pause_s
                // ����� ����� ������ ��������� (����� 2 � 3 ���������) ����� ����� TIME_DELAY::MIN_DISARM_LIGHTS_IMPULSE_PAUSE_S
                if (curr_impulse_count == 2)
                {
                    expected_impulse_pause_s = TIME_DELAY::MIN_DISARM_LIGHTS_IMPULSE_PAUSE_S;
                }
                else
                {
                    expected_impulse_pause_s = min_impulse_pause_s;
                }

                if (impulse_pause_s > TIME_DELAY::MIN_PULSE_WIDTH_S) // ������ ��������
                {
                    // 2.1.2.2 ��������, ��� ����� ����� ����� ���������� ������������� �������� (� ������������ impulse_pause_error_s)
                    if (   impulse_pause_s < expected_impulse_pause_s + impulse_pause_error_s
                        && impulse_pause_s > expected_impulse_pause_s - impulse_pause_error_s)
                    {
                        is_impulse_start = true;
                        printf("PASS ������� ��������� ����� ����� ����� (���������� ����������)\n");
                    }
                    // 2.1.2.3 ����� �� ���������
                    else
                    {
                        printf("FAIL ����� ����� ���������� ����� �������� (���������� ����������): %.6f\n", impulse_pause_s);
                        result = false;
                        break;
                    }
                } // if (impulse_pause_s > TIME_DELAY::MIN_PULSE_WIDTH_S)
            } // if (curr_impulse_count == 0)
        } // if (!is_impulse_start)

        // 2.2 ������� ��������� ������� ��������
        if (    is_impulse_start
            && !getLightsImpulse())
        {
            impulse_end_time_s = timer.read();
            impulse_width_s    = impulse_end_time_s - impulse_start_time_s;

            if (impulse_width_s > TIME_DELAY::MIN_PULSE_WIDTH_S) // ������ ��������
            {
                // 2.2.1 ��������, ��� ����� �������� ������������� �������� (� ������������ impulse_width_error_s)
                if (   impulse_width_s < lights_impulse_width_s + impulse_width_error_s
                    && impulse_width_s > lights_impulse_width_s - impulse_width_error_s)
                {
                    impulse_end_time_s = timer.read();
                    is_impulse_start   = false;
                    printf("PASS ������� %d-� �������� ������ (���������� ����������)\n", ++curr_impulse_count);
                }
                // 2.2.2 ����� �������� ��������
                else
                {
                    printf("FAIL ������������ ����������� ��������� ������� �������� (���������� ����������): %.6f\n", impulse_width_s);
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
        printf("PASS ���������� � ������������� ���������� ������: �������� �������\n");
    }
    else
    {
        printf("FAIL ���������� ��� ������������� ����������: �������� ���������\n");
        result = false; // � ����� ����������� ������ �������� ���������, � �� �� ���������� (������� result ��� �������� true)
                        // ��� �������������� ���������� ��������� ��������� ������ ���� false
    }

    return result;
}

void Gen6Basic::setTemperatureValue(temperature_celsius_t temperature)
{
    float                               voltage_coeff                   = 0.0f;                            // ����������� ���������� ��� �������� ���������� ������������ ����������
                                                                                                           // ��������� ��-�� ������������ ����������

    const float                         voltage_coeff_a96               = (float)(0.18 / 3.3);             // ����������� ��� ���������� A96       (180 ��)
    const float                         voltage_coeff_e96_s96           = (float)(0.05 / 3.3);             // ����������� ��� ����������� E96, S96 (50  ��)


    // ������ ��������� ����������
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

    // ����� ��������� ���������� (��� �����������)
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


    printf("INFO ��������� ����������� ��������� ����� ������ �����������, ��������: %s* C\n", voltage_str_arr[temperature]);

    if (p_temp_sensor == NULL)
    {
        printf("FAIL ����� ��� ��� ������� ����������� �� ���������\n");
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
            printf("FAIL ��������� �� ������������ ������ ����������� �� ������ ����������: %d\n", m_device_name);
            return;
    }

    if (temperature < TEMP_CELSIUS_COUNT)
    {
        p_temp_sensor->write(voltage_arr[temperature] - voltage_coeff);
    }
    else
    {
        printf("FAIL ������ �������� ����������� �� ���������� - %d\n", temperature);
    }
}

void Gen6Basic::disableHumanInCar()
{
    // �������� ����� ����� ������ ��� ���������� ��������� ����� � ���� ������� �� � ������
    if (   getSlpCmdStatus(DIAG_DATA_FLAG,
                           SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SIZE,
                           DIAG_FLAG_BIT_HANDS_FREE)
        && !getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                            SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                            DIAG_GUARD_STATE_ARM))
    {
        // ������� ����� ����������� FB-1725
        // if (getSlpCmdStatus(DIAG_DATA_FLAG_SYSDATA,
        //                     SLP_STATUSES_SIZES::DIAG_DATA_FLAG_SYSDATA_SIZE,
        //                     DIAG_FLAG_SYSDATA::ID_FLAG_HUMAN_IN_CAR))

        printf("INFO ���������� ���������, ������������� �������, ��� ������� ����� �� ������\n");

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
        printf("INFO ����� ��� ���� ���������\n");

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
        printf("FAIL R6Eco �� ���������������\n");
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

    // ����� �� ������� ������� �� ������ �� ���������
    setLcdKeyringVcc(OFF);
    setLedKeyringVcc(OFF);

    // ���� ������ ��������� 6 ���, ��������� R6 � ����� ����������������
    // ����������� ����� ���������������� ����� ������ �� �������
    if ((m_device_modules & BLE_MODULE) == BLE_MODULE)
    {
        p_r6->setVcc(OFF);
        p_r6eco->setVcc(OFF);
    }

    // ��������� ����������� � ����
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

    // �������� ����������� � ���� (��� ���������� ��������� ��������� Sim900)
    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        p_gsm->enNetReg();
    }

    wait(TIME_DELAY::MIN_DELAY_AFTER_VCC_EN_S);

    // ���� �� ��������� � GSM ����
    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        wait(TIME_DELAY::DELAY_AFTER_VCC_EN_S);
        p_gsm->clearSms();
    }
}

void Gen6Basic::resetAuthAndImmo(ign_statuses_t ign_status)
{
    // ���-�� ������� �� ���������� �� 15 ���. ����� ���� ����������� ������
    const uint8_t c_disarm_attempts_num = 3;


    printf("INFO ����� ��������� �����������/������� �������\n");
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
        // ����������� ��� ������� �����������
        if ((getModules() & BLE_MODULE) == BLE_MODULE)
        {
            setTagVcc();
        }

        if ((getModules() & R868_MODULE) == R868_MODULE)
        {
            setLcdKeyringVcc(ON);
        }

        sendValidatorCode();

        // ������ ������� ������� ���������� ��� ���. ���������
        setIgn(PIN, ON);
        setIgn(CAN_USE, ON);

        // �������, ��� ������� ������� �����������
        if (waitGuardState(DIAG_GUARD_STATE_DISARM,
                           ON,
                           TIME_DELAY::MAX_WAITING_REGION_STATE_S))
        {
            printf("INFO ����� �� �������� �����������/������� ������� ����������\n");
        }
        else
        {
            printf("INFO ����� �� �������� �����������/������� ������� �� ����������\n");
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

        // �� ������, ���� �� ��� ������� ������ ��������� ��������,
        // � ������� ���������� ���� ��������� ���� ����������� �����
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

        // ���� ��� ��� �� � ����� - ���� ���� ��� ������������
        if (!getSlpCmdStatus(DIAG_DATA_GUARD_STATE,
                             SLP_STATUSES_SIZES::DIAG_DATA_GUARD_STATE_SIZE,
                             DIAG_GUARD_STATE_DISARM))
        {
            printf("INFO ��� ����������� ������ ������������. ������� �������������...\n");
            wait(TIME_DELAY::WAITING_UNBLOCKING_EMERGENCY_CODE_S);
        }

        if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
        {
            outputSms(TIME_DELAY::STD_WAITING_SMS_TIME_S);
        }
    } // if getSlpCmdStatus

    printf("INFO ����� ��������� �����������/������� ������� ����������\n");
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

        // ��� ������ �� ������ �� SLP ����� ��������-��������� ���������
        wait(TIME_DELAY::MIN_REGISTER_DEVICE_TIME_S);
        setIgn(PIN, ON);
        setIgn(PIN, OFF);
        wait(TIME_DELAY::MIN_REGISTER_DEVICE_TIME_S);
    }

    if ((m_device_modules & GSM_MODULE) == GSM_MODULE)
    {
        // SMS, ������������ ����� �����������, ������ �� ����������� ������
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

            printf("INFO ����� ������� ����� �� ������\n");
            break;

        case STATUS_OUT_1:

            if (waitStateStatusOut(p_in_tag_status_1,
                                   ID_OUT_TAG_STATUS_1,
                                   status_out_state,
                                   time_s))               
            {
                printf("INFO ��������� ���������� ������ 1 ������������� ����������\n");
                return true;
            }

            printf("INFO ��������� ���������� ������ 1 �� ������������� ����������\n");

            return false;

        case STATUS_OUT_2:

            if (waitStateStatusOut(p_in_tag_status_2,
                                   ID_OUT_TAG_STATUS_2,
                                   status_out_state,
                                   time_s))                 
            {
                printf("INFO ��������� ���������� ������ 2 ������������� ����������\n");
                return true;
            }

            printf("INFO ��������� ���������� ������ 2 �� ������������� ����������\n");

            return false;

        case STATUS_OUT_3:

            if (waitStateStatusOut(p_in_tag_status_3,
                                   ID_OUT_TAG_STATUS_3,
                                   status_out_state,
                                   time_s))                 
            {
                printf("INFO ��������� ���������� ������ 3 ������������� ����������\n");
                return true;
            }

            printf("INFO ��������� ���������� ������ 3 �� ������������� ����������\n");

            return false;
    }

    return false;
}

bool Gen6Basic::setAndCheckTag(bool tag_mode)
{
    if (tag_mode)
    {
        setTagVcc();

        printf("INFO ������� �� ����� ������\n");
    }
    else
    {
        resetTagVcc();

        printf("INFO ������� � ����� �����\n");
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
        printf("INFO ���������� ������� �� ��������� ���������\n");
    }
    else
    {
        printf("INFO ���������� ������� �� ���������� ���������\n");
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
            // �����������, �. �. ������� ��-��������� 0-� ��������� �� �������
            // ���� �� � 4F ������� �������� - ���������� true
            return !getSupportIgn();
        }

        printf("FAIL ���������� ���������. ��� \"��������� 1\" �� ���������\n");
        return -1;
    }

    return *p_in_ign1;
}

int8_t Gen6Basic::checkStatusIn(DigitalIn *in_tag_status)
{
    if (in_tag_status == NULL)
    {
        printf("FAIL ���������� ���������. ��� \"����� ������� �����\" �� ���������\n");
    
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
        printf("INFO ����������� �������\n");
    }
    else
    {
        printf("INFO ���������� �������\n");
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
        printf("INFO ��������� ��������\n");
    }
    else
    {
        printf("INFO ���������� ��������\n");
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
        printf("INFO ���������� �������\n");
    }
    else
    {
        if (!setAhjTagSetting(ON))
        {
            printf("INFO �������������� �� ����� �� ��������\n");
            return false;
        }
    }

    if (!setAndCheckSecure(OFF,
                           secure_method,
                           DIAG_GUARD_STATE_DISARM))
    {
        printf("INFO ������ �� ���������\n");
        return false;
    }

    if (!setAndCheckIgn(ON, ign_method))
    {
        printf("INFO ��������� �� ��������\n");
        return false;
    }

    if (!setAndCheckEngine(ON, engine_method))
    {
        printf("INFO ��������� �� �������\n");
        return false;
    }

    if (!setAndCheckHandBreak(OFF, hand_break_method))
    {
        printf("INFO ������ �� ������\n");
        return false;
    }

    if (!setAndCheckParking(OFF, parking_method))
    {
        printf("INFO ������� �� ��������\n");
        return false;
    }

    if ((getModules() & BLE_MODULE) == BLE_MODULE)
    {
        if (!setAndCheckTag(ON))
        {
            printf("INFO ����� ��� � ���� ���������\n");
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
            printf("INFO ����� � ���� ���������\n");
            return false;
        }
    }
    else
    {
        p_lcd_keyring->offVcc();
        printf("INFO ���������� �������\n");
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
    string status_str = status ? "������"
                               : "������";


    printf("INFO �������� ��������� ���������\n");

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
            printf("FAIL ����������� ��� �������� ��������� ���������\n");
            result = false;
    }

    if (result)
    {
        printf("INFO �������� %s\n", status_str.c_str());
    }
    else
    {
        printf("INFO �������� �� %s\n", status_str.c_str());
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
        printf("FAIL ���������� ���������. ��� \"����� �������� ��������� ����������\" �� ���������\n");
    
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
    const char* message = value ? "INFO ����� �������(�������� �����)\n" 
                                : "INFO ����� �������(�������� �����)\n";
    printf("%s", message);

    if (p_out_door_inv == NULL)
    {
        printf("FAIL ���������� ���������. ��� \"���� ����� �� ��������\" �� ���������\n");
        return ;
    }

    (value == ON) ? *p_out_door_inv = 0
                  : *p_out_door_inv = 1;

    wait(delay_s);
}

 void Gen6Basic::setHoodInvPin(int8_t value, 
                               float  delay_s)
{
    const char* message = value ? "INFO ����� ������(�������� �����)\n" 
                                : "INFO ����� ������(�������� �����)\n";
    printf("%s", message);

    if (p_out_hood_inv == NULL)
    {
        printf("FAIL ���������� ���������. ��� \"���� ������ �� ��������\" �� ���������\n");
        return ;
    }

    (value == OPEN) ? *p_out_hood_inv = 0
                    : *p_out_hood_inv = 1;

    wait(delay_s);
}

 void Gen6Basic::setTrunkInvPin(int8_t value, 
                                float  delay_s)
{
    const char* message = value ? "INFO �������� ������(�������� �����)\n" 
                                : "INFO �������� ������(�������� �����)\n";
    printf("%s", message);

    if (p_out_trunk_inv == NULL)
    {
        printf("FAIL ���������� ���������. ��� \"���� ��������� �� ��������\" �� ���������\n");
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
        printf("FAIL ������ ���������. ��� \"���� ������� ������� ��������� � ��������� \" �� ���������\n");
        return;
    }

    const char* message = value ? "INFO ��������� ����� ������� ������� � ���������\n" 
                                : "INFO ���������� ����� ������� ������� � ���������\n";
    printf("%s", message);

    (value == ON) ? *p_out_killengine_inv = 1
                  : *p_out_killengine_inv = 0;

    wait(delay_s);
}

void Gen6Basic::pushLcdKeyringBut33(float push_type_first,
                                    float push_type_second)
{
    printf("DEBUG ������� ������� ������ 3 ������\n");

    if (p_lcd_keyring == NULL)
    {
        printf("DEBUG �� ������ �� ���������\n");
        return;
    }

    p_lcd_keyring->pushButton33(push_type_first, push_type_second);
}

void Gen6Basic::setDriverDoor(int8_t         value,
                              command_type_t execution_method,
                              float          delay_s)
{
    const char* message = value ? "INFO ����� �������� �������\n" 
                                : "INFO ����� �������� �������\n"; 

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
                printf("FAIL ������ ���������. ��� \"���� ����� ��������\" �� ���������\n");
                return;
            }

            (value == ON) ? *p_out_driver_door = 1
                          : *p_out_driver_door = 0;

            break;

        default:
            printf("FAIL ���������� ������ ��������: ����� ���������� �� ���������\n");
            break;
    }

    wait(delay_s);
}

void Gen6Basic::setPassengerDoor(int8_t         value,
                                 command_type_t execution_method,
                                 float          delay_s)
{
    const char* message = value ? "INFO ����� ��������� �������\n" 
                                : "INFO ����� ��������� �������\n"; 

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
                printf("FAIL ������ ���������. ��� \"���� ����� ���������\" �� ���������\n");
                return;
            }

            (value == ON) ? *p_out_passenger_door = 1
                          : *p_out_passenger_door = 0;

            break;

        default:
            printf("FAIL ���������� ������ ���������: ����� ���������� �� ���������\n");
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
        printf("INFO ��������� ������� ������� �� CAN\n");
        p_can_actions->sendAlertOnMercedesCla();
    }
    else
    {
        printf("INFO ���������� ������� ������� �� CAN\n");
        p_can_actions->sendAlertOffMercedesCla();
    }
}

void Gen6Basic::setIgnRenaultCaptur(bool status, float delay_s)
{
    printf("INFO ���������� ���������� �� Renault Captur\n");


    if (status)
    {
        p_can_actions->sendIgnOnRenaultCapture();
        printf("INFO ��������� ��������\n");
    }
    else
    {
        p_can_actions->sendIgnOffRenaultCapture();
        printf("INFO ��������� ���������\n");
    }

    wait(delay_s);
}

void Gen6Basic::setRpmRenaultCaptur(rpm_statuses_t value, float delay_s)
{
    printf("INFO ���������� ��������� �� Renault Captur\n");
    p_can_actions->sendSetRpmRenaultCapture(value);
    wait(delay_s);
}

void Gen6Basic::setRemoteRunRenaultCaptur(float delay_s)
{
    printf("INFO ������������� ������ Renault Captur\n");
    p_can_actions->sendRemoteRunRenaultCapture();
    wait(delay_s);
}

bool Gen6Basic::waitKlaxonLexusRx(float timeout_s)
{
    bool result;


    printf("INFO �������� ������� ��������� �������� ��� ���������� Lexus RX, ����� ��������: %.2f\n", timeout_s);

    result = p_can_actions->waitKlaxonOnLexusRx(timeout_s);

    if (result)
    {
        printf("INFO ������ ��������\n");
    }
    else
    {
        printf("INFO ������ �� ��������\n");
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
        printf("INFO ��������� �� ��������\n");
        return false;
    }

    printf("INFO ��������� ��������\n");

    if (!setAndCheckSecure(ON,                                   
                           execution_method,
                           DIAG_GUARD_STATE_ARM))
    {
        printf("INFO ������ �� ��������.\n");
        return false;
    }

    printf("INFO ������ ��������.\n");
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
    const std::string status_str = status == ON ? "�������" 
                                                : "��������";


    printf("INFO ����� \"�������� ������ ������\" %s\n", status_str.c_str());

    if (p_out_glowplugs == NULL)
    {
        printf("INFO ��� \"�������� ������ ������\" �� ���������, ���������� ����������� �� �����\n");
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
    std::string setting_type_str;                           // ��� ��������� � ��������� ����
    char        buf[LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN]; // ����� ��� ������� ������


    if (   setting_name  == NULL
        || setting_value == NULL)
    {
        printf("INFO ��� ��� �������� ��������� �� ������\n");

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
            printf("INFO ��� ��������� ����������: %d\n", setting_type);

            return false;
    }

    memset(buf,
           0,
           LEN_STRUCT::MAX_PACKAGE_PARAM_BUF_LEN);

    printf("INFO �������� ������� �� �������� �������� ���������: %s\n", setting_name);
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
            printf("INFO �������� ��������� ������������� ����������\n");

            return true;
        }
    }

    printf("INFO �������� ��������� �� ������������� ����������\n");

    return false;
}

bool Gen6Basic::waitSideMirrorsCan(mirrors_statuses_t mirrors_status, float timeout_s)
{
    bool result = false;


    printf("INFO �������� ������� (CAN) ����������/�������� ������, ����� ��������: %.2f\n", timeout_s);

    switch (mirrors_status)
    {
        case MIRRORS_OPEN:
            printf("INFO �������� ���������� ������\n");

            result = p_can_actions->waitSideMirrorsOpen(timeout_s);
            break;

        case MIRRORS_CLOSE:
            printf("INFO �������� �������� ������\n");

            result = p_can_actions->waitSideMirrorsClose(timeout_s);
            break;

        case MIRRORS_NONE:
            printf("INFO �������� ���������� ���������� ���������\n");

            result = p_can_actions->waitNoSideMirrors(timeout_s);
            break;

        default:
            printf("INFO ����������� ��� �������� ������: %d\n", mirrors_status);
    }

    return result;
}

bool Gen6Basic::waitComfortCan(out_statuses_t status, float timeout_s)
{
    bool result;


    if (status == ON)
    {
        printf("INFO �������� ��������� ������� ������� (CAN)\n");

        result = p_can_actions->waitComfortOn(timeout_s);
    }
    else
    {
        printf("INFO �������� ���������� ������� ������� (CAN)\n");

        result = p_can_actions->waitComfortOff(timeout_s);
    }

    if (result)
    {
        printf("INFO ��������� ������ ������� ������� (CAN) ��������\n");
    }
    else
    {
        printf("INFO ��������� ������ ������� ������� (CAN) �� ��������\n");
    }

    return result;
}

bool Gen6Basic::waitSunroofCanLexusGx(doors_statuses_t sunroof_status, float timeout_s)
{
    bool result;


    printf("INFO �������� ������� (CAN) ��������/�������� ���� �� ����� ��� ���������� Lexus GX, ����� ��������: %.2f\n", timeout_s);


    if (sunroof_status == OPEN)
    {
        printf("INFO �������� �������� ����\n");
        printf("FAIL ������� �� ��������������\n");

        result = false;
    }
    else
    {
        printf("INFO �������� �������� ����\n");

        result = p_can_actions->waitSunroofCloseLexusGx(timeout_s);
    }

    if (result)
    {
        printf("INFO ��������� ������ (CAN) ��������\n");
    }
    else
    {
        printf("INFO ��������� ������ (CAN) �� ��������\n");
    }

    return result;
}

bool Gen6Basic::waitInspectorCan(float timeout_s)
{
    bool result;


    printf("INFO �������� ������� ��������� �������� ��������� CAN\n");

    result = p_can_actions->waitInspector(timeout_s);

    if (result)
    {
        printf("INFO ������ ��������\n");
    }
    else
    {
        printf("INFO ������ �� ��������\n");
    }

    return result;
}

bool Gen6Basic::waitObdErrorRequest(float timeout_s)
{
    bool result;


    printf("INFO �������� ��������� �������� OBD ������\n");

    result = p_can_actions->waitObdErrorRequest(timeout_s);

    if (result)
    {
        printf("INFO ��������� (������ CAN) ��������\n");
    }
    else
    {
        printf("INFO ��������� (������ CAN) �� ��������\n");
    }

    return result;
}

bool Gen6Basic::connectMobile()
{
    bool result;


    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

        return false;
    }

    return p_mobileapp->setMobileStatus(command);
}

bool Gen6Basic::setServiceModeMobile(out_statuses_t status)
{
    bool result;


    printf("INFO ���������� ��������� ������� ����� ��������� ���������� (BLE)\n");

    if ((m_device_modules & BLE_MODULE) != BLE_MODULE)
    {
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

        return false;
    }

    if (status == ON)
    {
        printf("INFO ��������� ���������� ������\n");

        result = p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_SERVICE_ON);
    }
    else
    {
        printf("INFO ���������� ���������� ������\n");

        result = p_mobileapp->setMobileStatus(MOBILEAPP_USER_CMD_SERVICE_OFF);
    }

    return result;
}

void Gen6Basic::setAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type)
{
    printf("INFO ���������� ��������� ��������� ������ �������������� ��������� ������\n");

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
                printf("INFO ����� �� �������\n");
            }

            break;

        case ALT_SRV_BTN_TYPE_STD_BTN:

            if (!setAndCheckIgn(ON,
                                PIN,
                                TIME_DELAY::STD_WIRE_EN_DELAY_S))
            {
                printf("INFO ��������� �� ��������\n");
            }

            break;

        default:
            printf("INFO ����������� ��� �������������� ��������� ������: %d\n", alt_srv_btn_type);
            return;
    }

    printf("INFO �������� ��������\n");
}

bool Gen6Basic::setAndCheckAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type)
{
    bool lights_polarity = getPolarity(ID_OUT_LIGHTS); // ���������� ������ �������� ��������


    printf("INFO ��������� � �������� ������ �������������� ��������� ������\n");
    setAltSrvBtnModeEn(alt_srv_btn_type);

    if (alt_srv_btn_type != ALT_SRV_BTN_TYPE_STD_BTN) // ���������� ������ ������� ������� ���������� ����� ���. ���������
                                                      // ������� �������� ������� �� ����� ��������� (�.�. ��� ������� ��� ����� ���������� ��������� �� ������ ����)
    {
        if (    waitLights(TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S, !lights_polarity)  // �������� ������� ���������
            && !waitLights(TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S,  lights_polarity)) // �������� ������� �� ����������
        {
            printf("INFO �������� ������� �����������\n");
        }
        else
        {
            printf("INFO �������� ������� �� ���������\n");
            return false;
        }
    }

    return true;
}

bool Gen6Basic::clickAltSrvBtn(alt_srv_btn_type_t alt_srv_btn_type,
                               uint8_t            count)
{
    printf("INFO ������� �������������� ��������� ������, ���������� �������: %d\n", count);

    switch (alt_srv_btn_type)
    {
        // �����. ������� ������: ������� � ������� ����� (����� ������� ����� �������)
        case ALT_SRV_BTN_TYPE_DOOR:
            printf("INFO ������� �������������� ��������� ������: �����\n");

            for (uint8_t i = 0; i < count; ++i)
            {
                setDoor(CLOSE, PIN); // ��� �������� �� SLP, ����� ��������� � �������
                setDoor(OPEN, PIN);
            }

            break;

        // ���������. ������� ������: �������� � ��������� ��������� (��������� ��������� ����� �������)
        case ALT_SRV_BTN_TYPE_IGN:
            printf("INFO ������� �������������� ��������� ������: ���������\n");

            for (uint8_t i = 0; i < count; ++i)
            {
                setIgn(PIN,                              // ��� �������� �� SLP, ����� ��������� � �������
                       ON,
                       TIME_DELAY::STD_WIRE_EN_DELAY_S);
                setIgn(PIN,
                       OFF,
                       TIME_DELAY::STD_WIRE_EN_DELAY_S);
            }

            break;

        // ������� ������. ������� ��������� ������: ������� ������� ������ "��" (����������: 9999 Batmobile)
        case ALT_SRV_BTN_TYPE_STD_BTN:
            printf("INFO ������� �������������� ��������� ������: ������� ������ ����������\n");
            p_can_actions->sendBtnOff();

            for (uint8_t i = 0; i < count; ++i)
            {
                p_can_actions->sendBtn(CAN_BTN4_OK);
                wait(TIME_DELAY::MAX_CAN_BTN_CLICK_DELAY_S);
            }

            break;

        default:
            printf("INFO ����������� ��� �������������� ��������� ������: %d\n", alt_srv_btn_type);
            return false;
    }

    return true;
}

bool Gen6Basic::setAndCheckAltSrvBtnArm(alt_srv_btn_type_t alt_srv_btn_type)
{
    float arm_en_err_time_s; // ����������� ��������� ������


    printf("INFO ��������� ������ ����� �������������� ��������� ������\n");


    if (alt_srv_btn_type == ALT_SRV_BTN_TYPE_IGN)
    {
        printf("INFO ���������� ��������� ���������� � ������ ����� ����. ��������� ������ \"���������\"\n");
        return false;
    }

    if (alt_srv_btn_type == ALT_SRV_BTN_TYPE_DOOR)
    {
        if (!setAndCheckDoor(OPEN, PIN))
        {
            printf("INFO ����� �� �������\n");
            return false;
        }
    }

    if (!setAndCheckIgn(ON,
                        PIN,
                        TIME_DELAY::STD_WIRE_EN_DELAY_S))
    {
        printf("INFO ��������� �� ���������� (������ ���������)\n");
        return false;
    }

    clickAltSrvBtn(alt_srv_btn_type, COUNT_STRUCT::ALT_SRV_BTN_MODE_ARM_CLICKS_COUNT);

    if (!setAndCheckIgn(OFF,
                        PIN,
                        TIME_DELAY::STD_WIRE_EN_DELAY_S))
    {
        printf("INFO ��������� �� ����������� (������ ����������)\n");
        return false;
    }

    setIgn(PIN,                       // ��� �������� �� SLP, �.�. ����� ����� ��������� ������
           ON,
           TIME_DELAY::NULL_DELAY_S);

    if (alt_srv_btn_type != ALT_SRV_BTN_TYPE_DOOR)
    {
        wait_s(TIME_DELAY::STD_WIRE_EN_DELAY_S);                                           // ����� �� ����������� �������� ������ ����� ���. ���������
        clickAltSrvBtn(alt_srv_btn_type, COUNT_STRUCT::ALT_SRV_BTN_MODE_ARM_CLICKS_COUNT);
    }

    // �� �������� ��� ������� - ���������� � ���������� ��������� �� ������ (���������� ���������� ����� ������� ��������� ������)
    if (m_device_name != GEN6_X96V2)
    {
        if (isChFunctionalSet(ID_OUT_SIREN))
        {
            if (!waitSiren(TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S, getPolarity(ID_OUT_SIREN)))
            {
                printf("INFO ������ �� ����������\n");
                return false;
            }

            for (uint8_t i = 0; i < COUNT_STRUCT::ALT_SRV_BTN_ARM_EN_SIGNALS_COUNT_TOTAL; ++i)
            {
                setPolarity(ID_OUT_SIREN);
                
                if (!checkPulseWithoutWidth(ID_OUT_SIREN, TIME_DELAY::MAX_WIRE_EN_DELAY_S))
                {
                    printf("INFO %d-� ������ ������ �� �������\n", i + 1);
                    return false;
                }
            }

            arm_en_err_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S;
        }
        else
        {
            printf("INFO ����� \"������\" �� ��������, �������� ����� ������� ����������\n");
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
                printf("INFO ������� ��������������� �� ���������\n");
                return false;
            }

            for (uint8_t i = 0; i < COUNT_STRUCT::BEACH_EN_SIGNALS_COUNT_TOTAL; ++i)
            {
                if (!(   waitBuzzerExt(ON,  TIME_DELAY::MAX_WIRE_EN_DELAY_S)
                      && waitBuzzerExt(OFF, TIME_DELAY::SIREN_DIS_WAIT_TIME_S)))
                {
                    printf("INFO %d-� ������ �������� ��������������� �� �������\n", i + 1);
                    return false;
                }
            }

            arm_en_err_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S;
        }
        else
        {
            printf("INFO ����� \"������� ���������������\" �� ��������, �������� ����� ������� ����������\n");
            wait_s(TIME_DELAY::BTN_MODE_EN_AND_LIGHTS_WAIT_TIME_S);

            arm_en_err_time_s = TIME_DELAY::WAITING_REGION_STATE_S;
        }
    } // if (m_device_name != GEN6_X96V2)

    if (!setAndCheckIgn(OFF,
                        PIN,
                        TIME_DELAY::STD_WIRE_EN_DELAY_S))
    {
        printf("INFO ��������� �� ����������� (������ ����������)\n");
        return false;
    }

    if (alt_srv_btn_type == ALT_SRV_BTN_TYPE_DOOR)
    {
        if (!setAndCheckDoor(CLOSE, PIN))
        {
            printf("INFO ����� �� �������\n");
            return false;
        }
    }

    if (waitGuardState(DIAG_GUARD_STATE_ARM,
                       ON,
                       TIME_DELAY::ALT_SRV_BTN_ARM_EN_DELAY_S,
                       arm_en_err_time_s))
    {
        printf("INFO ������ ��������\n");
    }
    else
    {
        printf("INFO ������ �� ����������\n");
        return false;
    }

    return true;
}

bool Gen6Basic::setAltSrvBtnDisarm(alt_srv_btn_type_t  alt_srv_btn_type,
                                   const char         *pin)
{
    bool    lights_polarity; // ���������� ������ �������� ��������
    uint8_t pin_num;         // ��������� ����� ���� ����������� ������


    printf("INFO ���� ���� ����������� ������ ����� �������������� ��������� ������\n");

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
        pin_num = pin[i] - 0x30; // ������������� ������ ����� � �����, �������� ASCII

        printf("INFO ���� %d-� ����� ����, �����: %d\n",
               i + 1,
               pin_num);
        clickAltSrvBtn(alt_srv_btn_type, pin_num);

        if (isChFunctionalSet(ID_OUT_LIGHTS))
        {
            // ��� ����� ������ ����� ���� ������� ������� ������� ����� ��������� ���������� �������� ��������
            if (   i                == 0
                && alt_srv_btn_type == ALT_SRV_BTN_TYPE_STD_BTN)
            {
                printf("INFO �������� ���������� �������� �������� ����� ����� ������ ����� ������� �������\n");
                wait_s(  TIME_DELAY::ALT_SRV_BTN_AFTER_PIN_NUM_DELAY_S
                       - TIME_DELAY::MAX_CAN_BTN_CLICK_DELAY_S              // ����� �������� ����� ������� ������� ������
                       - TIME_DELAY::MIN_ERROR_TIME_S);                     // �����������

                if (!waitLights(TIME_DELAY::MAX_WIRE_EN_DELAY_S, !lights_polarity))
                {
                    printf("INFO �������� ������� �� ����������� ��� ����� ���� ������� �������\n");
                    return false;
                }
            }

            if (waitLights(TIME_DELAY::ALT_SRV_BTN_ALARM_LIGHTS_WAIT_TIME_S, lights_polarity))
            {
                printf("INFO ������� �������� ������ ����� ����� %d-� �����\n", i + 1);

                if (!waitLights(TIME_DELAY::LIGHTS_DIS_WAIT_TIME_S, !lights_polarity))
                {
                    printf("INFO ��������� ����� �� ���� (����������� �� ��������)\n");
                    return false;
                }

                // �������� �������� �������� ����� ����� ��������� �����
                if (i == 3)
                {
                    // �������� ������������� ������ ����� ����� ��������� ����� ��������� ���� (� �� ������)
                    if (strstr(pin, m_device_pin) == NULL)
                    {
                        return false;
                    }

                    // �������� ��������� ������� � �������� ��������� �������
                    for (uint8_t z = 2; z <= COUNT_STRUCT::ALT_SRV_BTN_DISARM_LIGHTS_COUNT; ++z)
                    {
                        if (   waitLights(TIME_DELAY::BTN_CMD_SUCCESS_LIGHTS_WAIT_TIME_S,  lights_polarity)
                            && waitLights(TIME_DELAY::LIGHTS_DIS_WAIT_TIME_S,             !lights_polarity))
                        {
                            printf("INFO ������� %d-� �������� ������ ����� ����� ��������� �����\n", z);
                        }
                        else
                        {
                            printf("INFO %d-� �������� ������ �� ������� ����� ����� ��������� �����\n", z);
                            return false;
                        }
                    } // for
                } // if i == 3

                wait(TIME_DELAY::MIN_WIRE_EN_DELAY_S); // ����� �� ����������� ������� ��������� ����� ����� ��������� ������� �����
            }
            else
            {
                printf("INFO �������� ������� �� �������� ����� ����� %d-� �����\n", i + 1);

                // ������������� ������ ����������� ������ ���� ������� �������� ����� ��������� ���� (�.�. ������ �� ����������)
                // � ��������� ������� - ������ ���� ������������� ������
                if (   i                         != 3
                    || strstr(pin, m_device_pin) != NULL)
                {
                    return false;
                }
            } // if waitLights
        }
        else
        {
            printf("INFO ����� \"�������� �������: �����\" �� ��������, �������� ���������� ����� ��������� �����\n");
            wait_s(TIME_DELAY::ALT_SRV_BTN_AFTER_PIN_NUM_DELAY_S + TIME_DELAY::MAX_WIRE_EN_DELAY_S);
        } // if p_out_lights != NULL
    } // for

    printf("INFO �������� �������� ����� ���� ����������� ������ ����� �������������� ��������� ������\n");

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
    DigitalIn *in_acc;                                      // ���� ����������� (���� � ����. ��� ������ ��������, ���� ��� ����. ��� ������ ��������)

    bool       immo_polarity;                               // ���������� ���������
    bool       acc_polarity;                                // ���������� �����������

    float      acc_dis_time_s  = TIME_DELAY::NULL_DELAY_S;  // ����� ���������� �����������
    float      immo_dis_time_s = TIME_DELAY::NULL_DELAY_S;; // ����� ���������� ���������

    float      immo_after_acc_delay_s;                      // ���������� �������� ���������� ��������� ����� ���������� �����������


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
        printf("INFO ����� \"������ ���������: ����������\" �� ��������\n");
        return false;
    }

    if (isChFunctionalSet(ID_OUT_IMMO))
    {
        immo_polarity = getPolarity(ID_OUT_IMMO);
    }
    else
    {
        printf("INFO ����� \"���������� ����������� ������ ������ �������� �������������\" �� ��������\n");
        return false;
    }

    printf("INFO �������� �������� ���������� ��������� ����� ���������� �����������, ��������� ��������: %.6f, �����������: %.6f\n",
           immo_delay_s,
           err_time_s);

    timer.start();

    // ���� ���������� �����������
    while (timer.read() < timeout_s)
    {
        if (*in_acc == !acc_polarity)
        {
            acc_dis_time_s = timer.read();

            printf("INFO ���������� ��������� �����: %.6f ���.\n", acc_dis_time_s);
            break;
        }
    }

    // ���� ���������� ���������
    while (timer.read() < timeout_s)
    {
        if (*p_in_inspector == !immo_polarity)
        {
            immo_dis_time_s = timer.read();

            printf("INFO �������� �������� �����: %.6f ���.\n", immo_dis_time_s);
            break;
        }
    }

    timer.stop();

    if (   immo_dis_time_s == TIME_DELAY::NULL_DELAY_S
        || acc_dis_time_s  == TIME_DELAY::NULL_DELAY_S)
    {
        printf("INFO ���������� ��� �������� �� �����������\n");
        return false;
    }

    immo_after_acc_delay_s = immo_dis_time_s - acc_dis_time_s;

    printf("INFO �������� �������� ����� %.6f ���. � ������� ���������� �����������\n", immo_after_acc_delay_s);

    if (fabs(immo_after_acc_delay_s - immo_delay_s) > err_time_s)
    {
        printf("INFO �������� �������� ��� ���������� ������� �����������\n");
        return false;
    }

    printf("INFO �������� ��������� �������\n");

    return true;
}

bool Gen6Basic::openDebugRegion(slp_open_type_t slp_open_type)
{
    bool result;


    printf("INFO �������� ������� ��� �������\n");

    result = p_slp->openDebugRegion(slp_open_type);

    return result;
}

bool Gen6Basic::openEventsRegion()
{
    bool result;


    printf("INFO �������� ������� ��� ������ �������\n");

    result = p_slp->openEventsRegion();

    return result;
}

bool Gen6Basic::closeRegion()
{
    bool result;


    printf("INFO �������� �������\n");

    result = p_slp->closeRegion();

    return result;
}

bool Gen6Basic::waitSlpEvents(const std::vector<slp_user_event_t> &event_ids, float timeout_s)
{
    bool result;


    printf("INFO �������� ����������� ������� � �������\n");

    result = p_slp->waitEvents(event_ids, timeout_s);

    return result;
}

void Gen6Basic::setDismantlingSensor(out_statuses_t value,
                                     command_type_t execution_method,
                                     invers_logic_t logic_type,
                                     float          delay_s)
{
    bool sensor_value; // ��������������� �������� ������� ���������


    if (value == ON)
    {
        printf("INFO ��������� ������� ���������\n");

        sensor_value = 1;
    }
    else
    {
        printf("INFO ���������� ������� ���������\n");

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
            printf("FAIL ���������������� ����� ��������� ������� ���������\n");
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
        printf("INFO ��������� (�������) ������� ������ CAN (Audi A7)\n");

        p_can_actions->sendExtBtnOnAudiA7();
    }
    else
    {
        printf("INFO ���������� (����������) ������� ������ CAN (Audi A7)\n");

        p_can_actions->sendExtBtnOffAudiA7();
    }

    wait(delay_s);
}

bool Gen6Basic::checkSrvLedAndSiren(uint8_t pulses_count, float timeout_s)
{
    const float err_between_pulse_s    = TIME_DELAY::MIN_ERROR_TIME_S; // ����������� ����� ������������� ���������� ������ � ���������� ����������
    const bool  siren_polarity         = getPolarity(ID_OUT_SIREN);    // ���������� ������

    bool        is_led_potential       = false;                        // ������� �� ��������� ���������� ����������
    bool        is_siren_potential     = false;                        // ������� �� ��������� ������

    uint8_t     led_pulse_count        = 0;                            // ���-�� ���������� ��������� ���������� ����������
    uint8_t     siren_pulse_count      = 0;                            // ���-�� ���������� ��������� ������

    float       led_potential_time_s   = TIME_DELAY::NULL_DELAY_S;     // ����� ��������� ���������� ���������� ����������
    float       siren_potential_time_s = TIME_DELAY::NULL_DELAY_S;     // ����� ��������� ���������� ������

    float       led_pulse_time_s       = TIME_DELAY::NULL_DELAY_S;     // ����� ��������� �������� ���������� ����������
    float       siren_pulse_time_s     = TIME_DELAY::NULL_DELAY_S;     // ����� ��������� �������� ������

    float       led_and_siren_diff_s   = TIME_DELAY::NULL_DELAY_S;     // ������� ����� ���������� ������� (�������� ��� ����������) ���������� ���������� � ������

    Timer       timer;


    printf("INFO �������� ������������� �������� ���������� ���������� � ������. ���-�� ��������: %u, ����� ��������: %.6f ���.\n",
           pulses_count,
           timeout_s);
    timer.start();

    while (timer.read() < timeout_s)
    {
        // �������� ���������� ����������
        if (   !is_led_potential
            &&  p_service_btn->isLedOn())
        {
            printf("TRACE ������� ��������� ���������� ���������� �����: %.6f\n", timer.read());

            is_led_potential     = true;
            led_potential_time_s = timer.read();
        }

        if (    is_led_potential
            && !p_service_btn->isLedOn())
        {
            ++led_pulse_count;
            is_led_potential = false;
            led_pulse_time_s = timer.read();

            printf("INFO ������� %u-� ������ ���������� ���������� ����� %.6f ���.\n",
                   led_pulse_count,
                   timer.read());
        }

        // �������� ������
        if (   !is_siren_potential
            &&  getSiren() == siren_polarity)
        {
            printf("TRACE ������� ��������� ������ �����: %.6f ���.\n", timer.read());

            is_siren_potential     = true;
            siren_potential_time_s = timer.read();
        }

        if (   is_siren_potential
            && getSiren() == !siren_polarity)
        {
            ++siren_pulse_count;
            is_siren_potential = false;
            siren_pulse_time_s = timer.read();

            printf("INFO ������� %u-� ������ ������ ����� %.6f ���.\n",
                   led_pulse_count,
                   timer.read());
        }

        // �������� �������������� ��������� ������ � ���������� ����������
        if (   is_led_potential
            && is_siren_potential)
        {
            led_and_siren_diff_s = fabs(led_potential_time_s - siren_potential_time_s);

            if (led_and_siren_diff_s > err_between_pulse_s)
            {
                printf("INFO ������� ����� ���������� ��������� ���������� � ������: %.6f ���.\n", led_and_siren_diff_s);

                return false;
            }
        }

        if (   !is_led_potential
            && !is_siren_potential)
        {
            led_and_siren_diff_s = fabs(led_pulse_time_s - siren_pulse_time_s);

            if (led_and_siren_diff_s > err_between_pulse_s)
            {
                printf("INFO ������� ����� ����������� ���������� � ������: %.6f ���.\n", led_and_siren_diff_s);

                return false;
            }
        }
    } // while timer.read() < timeout_s

    timer.stop();

    if (   led_pulse_count   == pulses_count
        && siren_pulse_count == pulses_count)
    {
        printf("INFO ��� ������� ���������� ���������� � ������ ���� ��������\n");
    }
    else
    {
        printf("INFO ���-�� �������� ���������� ���������� ��� ������ �� ������������� ����������\n");

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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

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
        printf("INFO ����������� BLE ������ ��� ������ � ��������� ����������� (BLE)\n");

        return false;
    }

    result = p_mobileapp->checkSettings(settings);

    return result;
}
