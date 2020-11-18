/*!
 *  \file      Gen6Basic.h
 *
 *  \brief     Содержит базовый класс для 6-го поколения основных блоков
 *
 *  \detailed  Содержит необходимый функционал для классов, производных от него
 *
 */

#ifndef _GEN6_BASIC_H_
#define _GEN6_BASIC_H_

//! Подключение заголовочных файлов

#include <vector>

#include "CanBusActions.h"
#include "CountStruct.h"
#include "Channels.h"
#include "DevicesBasic.h"
#include "enums_for_tests.h"
#include "LcdKeyring.h"
#include "LedKeyring.h"
#include "LenStruct.h"
#include "mbed.h"
#include "R4.h"
#include "R6.h"
#include "R6Eco.h"
#include "ServiceButton.h"
#include "SlpCmd.h"
#include "SmartTag.h"
#include "StarlineOnline.h"
#include "stdlib.h"
#include "StringConverter.h"
#include "Sim900.h"
#include "TimeDelay.h"
#include "MazdaImmoBypass.h"
#include "MobileApp.h"
#include "RegistrationQueue.h"
#include "Validator.h"


//! Типы и классы

/*!
 *  \brief    Параметры управления питанием метки
 */
typedef enum
{
    TAG_VOLTAGE_CTRL_DIS, //!< Не управлять питанием метки
    TAG_VOLTAGE_CTRL_EN   //!< Управлять питанием метки
} tag_voltage_ctrl_t;

/*!
 *  \brief    Метка лога (INFO/FAIL/PASS)
 */
typedef enum
{
    LOG_LABEL_INFO,      //!< INFO лог
    LOG_LABEL_FAIL,      //!< FAIL лог
    LOG_LABEL_PASS       //!< PASS лог
} log_label_t;

/*!
 *  \brief    Возможные ошибки при загрузке прошивки с сервера
 */
struct FW_LOAD_ERROR
{
    static const char *FW_LOAD_OK;              //!< Нет ошибок
    static const char *FW_LOAD_ERROR_SL_MASTER; //!< Ошибка при обновлении через StarLine Мастер
    static const char *FW_LOAD_ERROR_NOT_FOUND; //!< Запрашиваемая прошивка не найдена на сервере
    static const char *FW_LOAD_ERROR_ARG;       //!< Недостаточно аргументов для загрузки прошивки
};

/*!
 *  \brief    Типы прошивок для скачивания с сервера
 */
typedef enum
{
    FW_DOWNLOAD_TYPE_OLDEST,        //!< Самая старая версия ПО находащаяся на сервере
    FW_DOWNLOAD_TYPE_SECOND_PREV,   //!< Вторая предыдущая версия ПО
    FW_DOWNLOAD_TYPE_PREV,          //!< Предыдущая версия ПО
    FW_DOWNLOAD_TYPE_LATEST_BETA,   //!< Последняя бета версия ПО находящаяся на сервере
    FW_DOWNLOAD_TYPE_LATEST_RELEASE //!< Последняя релизная версия ПО находящаяся на сервере
} fw_download_type_t;

/*!
 *  \brief    Возможные типы настроек в ЦБ
 */
typedef enum
{
    SETTING_TYPE_INT, //!< Целочисленный тип настройки (напр. длительность импульса ЦЗ)
    SETTING_TYPE_STR  //!< Строковый тип настройки (напр. номер M1)
} setting_type_t;

/*!
 *  \brief    Выходить ли из очереди на регистрацию при выключении зажигания
 */
typedef enum
{
    IGN_OFF_REG_QUEUE_NO_LEAVE, //!< Не выходить
    IGN_OFF_REG_QUEUE_LEAVE     //!< Выходить
} reg_queue_leave_type_t;

/*!
 *  \brief    Класс Gen6Basic
 *
 *  \detailed Содержит функционал для классов, производных от него
 */
class Gen6Basic : public DevicesBasic
{
//! \privatesection
private:

    //! Метод регистрации брелка
    typedef bool (Gen6Basic::*reg_remote_func_t)();

    /*!
     *  \brief         Получение имени пина
     *
     *  \param  [in]   digitalIn       - указатель на класса DigitalIn
     *
     *  \return        PinName         - имя пина
     */
    PinName getPinNameIn(DigitalIn *digitalIn);

    /*!
     *  \brief         Регистрация брелка в ЦБ
     *
     *  \param  [in]  *obj  - указатель на объект класса Gen6Basic
     *  \param  [in]  *func - указатель на метод регистрации
     */
    void registerRemote(Gen6Basic *obj, reg_remote_func_t func);

//! \protectedsection
protected:

    R6                   *p_r6;               //!< Указатель на R6
    R6Eco                *p_r6eco;            //!< Указатель на R6Eco
    R4                   *p_r4;               //!< Указатель на R4
    LedKeyring           *p_led_keyring;      //!< Указатель на LedKeyring
    LcdKeyring           *p_lcd_keyring;      //!< Указатель на LcdKeyring
    SmartTag             *p_tag;              //!< Указатель на SmartTag
    ServiceButton        *p_service_btn;      //!< Указатель на ServiceButton
    Sim900               *p_gsm;              //!< Указатель на Sim900
    CanBusActions        *p_can_actions;      //!< Указатель на CanBusActions
    StarlineOnline       *p_st_online;        //!< Указатель на StarlineOnline
    SlpCmd               *p_slp;              //!< Указатель на SlpCmd
    Serial               *p_serial;           //!< Указатель на Serial
    AnalogOut            *p_temp_sensor;      //!< Указатель на ЦАП для формирования напряжения на вход датчика температуры 
    MobileApp            *p_mobileapp;        //!< Указатель на MobileApp
    Validator            *p_validator;        //!< Указатель на валидатор

    vector<const char *>  m_gen6_phones;      //!< Телефоны

    int32_t               m_device_modules;      //!< Подключенные модули
    char                  m_device_pin[5];       //!< Код экстренного снятия с охраны
    char                  m_wrong_device_pin[5]; //!< Неверный код экстренного снятия с охраны

//! \publicsection
public:
    
    /*!
     *  \brief         Конструктор класса
     */
    Gen6Basic() {};

    /*!
     *  \brief         Деструктор класса
     */
    ~Gen6Basic();

    /*!
     *  \brief         Конструктор копирования
     *
     *  \param  [in]  &right - ссылка на Gen6Basic
     */
    Gen6Basic(const Gen6Basic &right);

    /*!
     *  \brief         Оператор =
     *
     *  \param  [in]  &right - ссылка на Gen6Basic
     */
    Gen6Basic &operator=(const Gen6Basic &right);
    
    /*!
     *  \brief         Инициализация управляющего пина ACK-лки
     */ 
    void initSigmaEnable();
    
    /*!
     *  \brief         Перевод ACK-лки в режим приема настроек
     */ 
    void sigmaSettingMode();

    /*!
     *  \brief         Перевод ACK-лки в режим работы
     */ 
    void sigmaWorkMode();

    /*!
     *  \brief         Получить состояние питания основного блока
     *
     *  \return        bool  - состояние питания
     *  \retval        true  - питание включено
     *  \retval        false - питание выключено
     */
    bool getVcc();

    /*!
     *  \brief         Включить питание
     */ 
    void on();
    
    /*!
     *  \brief         Выключить питание
     */ 
    void off();
    
    /*!
     *  \brief         Добавление указателя на AnalogOut
     *
     *  \param  [in]  *temp_sensor - указатель на AnalogOut
     */
    void addTempSensor(AnalogOut *temp_sensor);

    /*!
     *  \brief         Добавление указателя на LedKeyring
     *
     *  \param  [in]  *ledkeyring - указатель на LedKeyring
     */
    void addKeyring(LedKeyring *ledkeyring);

    /*!
     *  \brief         Добавление указателя на LcdKeyring
     *
     *  \param  [in]  *lcdkeyring - указатель на LcdKeyring
     */
    void addKeyring(LcdKeyring *lcdkeyring);

    /*!
     *  \brief         Добавление указателя на SmartTag
     *
     *  \param  [in]  *tag - указатель на SmartTag
     */
    void addTag(SmartTag *tag);

    /*!
     *  \brief         Добавление указателя на R6
     *
     *  \param  [in]  *r6 - указатель на R6
     */
    void addR6(R6 *r6);

    /*!
     *  \brief         Добавление указателя R6Eco
     *
     *  \param  [in]  *r6eco - указатель на R6Eco
     */
    void addR6Eco(R6Eco *r6eco);

    /*!
     *  \brief         Добавление указателя на R4
     *
     *  \param  [in]  *r4 - указатель на R4
     */
    void addR4(R4 *r4);

    /*!
     *  \brief         Добавление указателя на Sim900
     *
     *  \param  [in]  *sim900 - указатель на Sim900
     */
    void addGsm(Sim900 *sim900);

    /*!
     *  \brief         Добавление указателя на CanBusActions
     *
     *  \param  [in]  *can_actions - указатель на CanBusActions
     */
    void addCan(CanBusActions *can_actions);

    /*!
     *  \brief         Возвращает указатель на CAN 1
     *
     *  \return        указатель на CanBus
     */
    CanBus* getCan1();

    /*!
     *  \brief         Возвращает указатель на CanBusActions
     *
     *  \return        указатель на CanBusActions
     */
    CanBusActions* getCanBusActions();

    /*!
     *  \brief         Добавление указателя на ServiceButton
     *
     *  \param  [in]  *btn - указатель на ServiceButton
     */
    void addServiceButton(ServiceButton *btn);

    /*!
     *  \brief         Добавление указателя на StarlineOnline
     *
     *  \param  [in]  *st_online - указатель на StarlineOnline
     */
    void addStarlineOnline(StarlineOnline *st_online);

    /*!
     *  \brief         Добавление указателя на SlpCmd
     *
     *  \param  [in]  *slp - указатель на SlpCmd
     */
    void addSlpCmd(SlpCmd *slp);

    /*!
     *  \brief         Добавление указателя на Serial
     *
     *  \param  [in]  *serial - указатель на Serial
     */
    void addSerial(Serial *serial);

    /*!
     *  \brief         Добавление имени устройства
     *
     *  \param  [in]   device_name - имя устройства
     */
    void addDeviceName(devices_t device_name);

    /*!
     *  \brief         Получение имени устройства
     *
     *  \return        devices_t - имя устройства
     */
    devices_t getDeviceName();
    
    /*!
     *  \brief         Добавление Модулей
     *
     *  \param  [in]   modules - Модули устройства
     */
    void addModules(int32_t modules);

    /*!
     *  \brief         Добавление телефона устройства
     *
     *  \param  [in]  *device_phone - номер телефона
     */
    void addDevicePhone(const char *device_phone);

    /*!
     *  \brief         Добавление указателя на MobileApp
     *
     *  \param  [in]  *app - указатель на MobileApp
     */
    void addMobileApp(MobileApp *app);

    /*!
     *  \brief         Добавление указателя на Validator
     *
     *  \param  [in]  *validator - указатель на Validator
     */
    void addValidator(Validator *validator);

    /*!
     *  \brief         Получить коллекцию телефонов устройства
     *
     *  \return        Вектор, содержащий номера устройства
     */
    vector<const char *> &getDevicePhones();

    /*!
     *  \brief         Авторизация на SL-online
     *
     *  \return        1 - успешно, 0 - нет
     */
    bool authStarlineOnline();

    /*!
     *  \brief         Добавление устройства
     *
     *  \param  [in]  *tel         - номер телефона
     *  \param  [in]  *name        - имя устройства
     *  \param  [in]  *device_type - тип устройства (gps-alarm)
     *
     *  \return        1 - успешно, 0 - нет
     */
    bool addStarlineOnlineDevice(const char *tel,
                                 const char *name,
                                 const char *device_type = "gps-alarm");

    /*!
     *  \brief         Управление сигнализацией
     *
     *  \param  [in]   command_type - тип команды управления
     *  \param  [in]   value        - значение
     *  \param  [in]  *pin          - код экстренного снятия
     *
     *  \return        1 - успешно, 0 - нет
     */
    bool setStarlineOnlineParam(StarlineOnline::command_type_t command_type, bool value, char *pin = NULL);

    /*!
     *  \brief         Ожидание SMS (без проверки)
     *
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        статус (X96_NOSMS, X96_TRUESMS)
     */
    sms_state_t outputSms(float timeout_s = Sim900::s_wait_time_s);

    /*!
     *  \brief         Ожидание SMS
     *
     *  \param  [in]  *checkmsg - проверочное сообщение
     *
     *  \return        статус (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t waitSms(const char *checkmsg);

    /*!
     *  \brief         Ожидание SMS с указанием времени ожидания
     *
     *  \param  [in]  *checkmsg - проверочное сообщение в юникоде
     *  \param  [in]   time     - время ожидания
     *
     *  \return        статус (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t waitSmsFor(const char *checkmsg, float time);

    /*!
     *  \brief         Отправка СМС и ожидание ответа
     *
     *  \param  [in]  *to       - номер телефона
     *  \param  [in]  *message  - отсылаемое сообщение в юникоде
     *  \param  [in]  *checkmsg - проверочное сообщение в юникоде
     *
     *  \return        статус (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t inspectionSms(const char *to,
                              const char *message,
                              const char *checkmsg);

    /*!
     *  \brief         Отправка СМС и ожидание ответа с указанием времени ожидания
     *
     *  \param  [in]  *to       - номер телефона
     *  \param  [in]  *message  - отсылаемое сообщение в юникоде
     *  \param  [in]  *checkmsg - проверочное сообщение в юникоде
     *  \param  [in]   time     - время ожидания
     *
     *  \return        статус (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t inspectionSmsFor(const char *to,
                                 const char *message,
                                 const char *checkmsg,
                                 float       time);

    /*!
     *  \brief         Получить указатель на Sim900
     *
     *  \return        указатель на Sim900
     */
    Sim900 *getSim900();

    /*!
     *  \brief         Получить указатель на StarlineOnline
     *
     *  \return        указатель на StarlineOnline
     */
    StarlineOnline *getStarlineOnline();

    /*!
     *  \brief         Нажатие на кнопку доп. брелка 1
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLedKeyringBut1(float type);

    /*!
     *  \brief         Нажатие на кнопку доп. брелка 2
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLedKeyringBut2(float type);

    /*!
     *  \brief         Нажатие на кнопку доп. брелка 3
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLedKeyringBut3(float type);

    /*!
     *  \brief         Нажатие на кнопку ЖК-брелка 1
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLcdKeyringBut1(float type);

    /*!
     *  \brief         Нажатие на кнопку ЖК-брелка 2
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLcdKeyringBut2(float type);

    /*!
     *  \brief         Нажатие на кнопку ЖК-брелка 3
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLcdKeyringBut3(float type);

    /*!
     *  \brief         Нажатие на кнопку ЖК-брелка 4
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLcdKeyringBut4(float type);

    /*!
     *  \brief         Одновременное нажатие на 2 кнопки доп. брелка (2 и 3)
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLedKeyringBut23(float type);

    /*!
     *  \brief         Одновременное нажатие на 2 кнопки ЖК-брелка (2 и 3)
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushLcdKeyringBut23(float type);

    /*!
     *  \brief         Одновременное нажатие на 2 кнопки ЖК-брелка (1 и 4)
     *
     *  \param  [in]   push_time_s - длительность нажатия
     */
    void pushLcdKeyringBut14(float push_time_s);

    /*!
     *  \brief         Одновременное нажатие на 2 кнопки ЖК-брелка (2 и 4)
     *
     *  \param  [in]   push_time_s - длительность нажатия
     */
    void pushLcdKeyringBut24(float push_time_s);

    /*!
     *  \brief         Одновременное нажатие на 2 кнопки ЖК-брелка (3 и 4)
     *
     *  \param  [in]   push_time_s - длительность нажатия
     */
    void pushLcdKeyringBut34(float push_time_s);
    
    /*!
     *  \brief         Выбор режима имитатора обхода (выкл./обучение/обход)
     *
     *  \param  [in]   bp_type - режим имитатора
     *
     *  \return        bool
     *  \retval        true  - успешная запись
     *  \retval        false - запись не успешна
     */
    bool setBypassMode(slp_imit_bp_mode_t bp_mode);
    
    /*!
     *  \brief         Выбор типа имитатора обхода (KIA/Ford и т.д.)
     *
     *  \param  [in]   bp_type - тип имитатора
     *
     *  \return        bool
     *  \retval        true  - успешная запись
     *  \retval        false - запись не успешна
     */
    bool setBypassType(slp_imit_bp_type_t bp_type);
    
    /*!
     *  \brief         Запись в регион бит постановки в охрану
     */
    void setArmBit();
    
    /*!
     *  \brief         Запись в регион бит снятия с охраны
     */
    void setDisarmBit();

    /*!
     *  \brief         Запись в регион бит запуска двигателя
     */
    void setEngineStartBit();
    
    /*!
     *  \brief         Запись в регион бит остановки двигателя
     */
    void setEngineStopBit();
    
    /*!
     *  \brief         Запись в регион бит запуска ППП
     */
    void setPreheaterStartBit();
    
    /*!
     *  \brief         Запись в регион бит остановки ППП
     */
    void setPreheaterStopBit();
    
    /*!
     *  \brief         Запись в регион бит сработки сирены
     */
    void setSignalBit();
    
    /*!
     *  \brief         Запись в регион бит открытия багажника
     */
    void setOpenTrunkBit();
    
    /*!
     *  \brief         Запись в регион бит запуска обучения обходчика
     */
    void setBypassLearnBit();
    
    /*!
     *  \brief         Запись в регион бит стирания данных обучения
     */
    void setBypassLearnClearBit();

    /*!
     *  \brief         Запись в регион бит входа в режим регистрации
     *
     *  \param  [in]   reg_queue_mode - использовать ли очередь на регистрацию (вставать в очередь или регистрироваться без очереди)
     *
     *  \return        bool
     *  \retval        true  - вход выполнен (бит записан)
     *  \retval        false - вход не выполнен (бит не записан)
     */
    bool setRegBit(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         Запись в регион бит входа в режим регистрации и проверка перехода в режим регистрации
     *
     *  \param  [in]   reg_queue_mode - использовать ли очередь на регистрацию (вставать в очередь или регистрироваться без очереди)
     *
     *  \return        bool
     *  \retval        true  - вход выполнен
     *  \retval        false - вход не выполнен
     */
    bool setAndCheckRegBit(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         Запись в регион бит перезагрузки обучения
     */
    void setResetBit();

    /*!
     *  \brief         Запись в регион бит выхода из режима настройки по BLE
     */
    void setBleSettingModeOffBit();

    /*!
     *  \brief         Переход в режим паники
     */
    virtual void setPanicKeyring();
    
    /*!
     *  \brief         Управление ручником
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setHandBreak(int8_t         value,
                              command_type_t execution_method,
                              float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление дверью
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   logic_state      - логика работы 
     */
    virtual void setDoor(int8_t         value,
                         command_type_t execution_method,
                         float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление двигателем
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   rpm_val          - значение оборотов при включении двигателя
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setEngine(int8_t         value,
                           command_type_t execution_method,
                           int16_t        rpm_val           = RPM_ON,
                           float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Выставить обороты
     *
     *  \param  [in]   rpm_val          - значение
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setRpm(int16_t        rpm_val,
                        command_type_t execution_method,
                        float          delay             = TIME_DELAY::STD_RPM_EN_DELAY_S);

    /*!
     *  \brief         Управление капотом
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setHood(int8_t         value,
                         command_type_t execution_method,
                         float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление багажником
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual bool setTrunk(int8_t         value,
                          command_type_t execution_method,
                          float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление зажиганием
     *
     *  \param  [in]   execution_method     - метод исполнения
     *  \param  [in]   value                - значение (вкл/выкл)
     *  \param  [in]   delay_s              - задержка после управления
     *  \param  [in]   reg_queue_leave_type - выходить ли из очереди на регистрацию при выключении зажигания
     */
    virtual void setIgn(command_type_t         execution_method,
                        int8_t                 value,
                        float                  delay_s              = TIME_DELAY::MIN_WIRE_EN_DELAY_S,
                        reg_queue_leave_type_t reg_queue_leave_type = IGN_OFF_REG_QUEUE_LEAVE);

    /*!
     *  \brief         Включить зажигание на реле (дать реле понять, что зажигание включено)
     *
     *  \param  [in]  *relay  - указатель на реле
     *  \param  [in]   status - статус зажигания
     */
    void setRelayIgn(RelayBase *relay, bool status);

    /*!
     *  \brief         Включить контакт IGN на реле R6
     */
    void setR6Ign();

    /*!
     *  \brief         Выключить контакт IGN на реле R6
     */
    void resetR6Ign();

    /*!
     *  \brief         Включить контакт IGN на реле R4
     */
    void setR4Ign();

    /*!
     *  \brief         Выключить контакт IGN на реле R4
     */
    void resetR4Ign();

    /*!
     *  \brief         Нажать тормоз
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setBreak(int8_t         value,
                          command_type_t execution_method,
                          float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Включить правый свет
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setRLight(int8_t         value,
                           command_type_t execution_method,
                           float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Включить левый свет
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setLLight(int8_t         value,
                           command_type_t execution_method,
                           float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ЦЗ открыть
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setCLock(int8_t         value,
                          command_type_t execution_method,
                          float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ППО включить
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setPpp(int8_t         value,
                        command_type_t execution_method,
                        float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление паркингом
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay            - задержка после управления
     */
    virtual void setParking(int8_t         value,
                            command_type_t execution_method,
                            float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление охраной
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay_s          - задержка после управления
     *  \param  [in]   tag_voltage_ctrl - управлять ли питанием метки (при управлении меткой)
     */
    virtual void setSecure(int8_t             value,
                           command_type_t     execution_method,
                           float              delay_s          = TIME_DELAY::STD_REMOTE_STATUS_EN_TIME_S,
                           tag_voltage_ctrl_t tag_voltage_ctrl = TAG_VOLTAGE_CTRL_EN);

    /*!
     *  \brief         Дистанционный запуск
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   sms_delay_s      - время ожидание обходчика после отправки смс
     */
    virtual void setRemoteRun(command_type_t execution_method,
                              float          sms_delay_s       = TIME_DELAY::BYPASS_AFTER_SMS_TIME_S);

    /*!
     *  \brief         Дистанционное глушение
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   sms_delay_s      - время ожидания Старт-Стоп/выкл. ПЗ после отправки смс
     */
    virtual void setRemoteStop(command_type_t execution_method,
                               float sms_delay_s                = TIME_DELAY::MAX_WAITING_SMS_STATUS_TIME_S);

    /*!
     *  \brief         Запуск двигателя
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   engine_start_system - система запуска
     *  \param  [in]   acc_must_be_off     - должны ли отключаться аксессуары
     *  \param  [in]   rpm_val             - значение оборотов, при запуске
     */
    virtual void runEngine(command_type_t        execution_method,
                           engine_start_system_t engine_start_system = IGNITION_LOCK,
                           bool                  acc_must_be_off     = true,
                           int16_t               rpm_val             = RPM_ON);

    /*!
     *  \brief         Проверка импульсов после запуска
     *
     *  \param  [in]   engine_start_system   - система запуска двгателя
     *  \param  [in]   bypass_waiting_time_s - время ожидания имульса обхода
     *  \param  [in]   acc_must_be_off       - должны ли быть отлючены аксессуары
     *  \param  [in]   log_label             - метка лога, который выводится при неполученном импульсе
     *  \param  [in]   execution_method      - метод включения зажигания и оборотов
     *
     *  \return        engine_start_error_t - ошибка
     */
    engine_start_error_t checkEngine(engine_start_system_t engine_start_system,
                                     float                 bypass_waiting_time_s,
                                     bool                  acc_must_be_off        = true,
                                     log_label_t           log_label              = LOG_LABEL_FAIL,
                                     command_type_t        execution_method       = PIN);

    /*!
     *  \brief         Проверка импульсов после запуска (замок зажигания)
     *
     *  \param  [in]   bypass_waiting_time_s - время ожидания имульса обхода
     *  \param  [in]   acc_must_be_off       - должны ли быть отлючены аксессуары
     *  \param  [in]   log_label             - метка лога, который выводится при неполученном импульсе
     *
     *  \return        engine_start_error_t - ошибка
     */
    engine_start_error_t checkIgnLockEngine(float          bypass_waiting_time_s,
                                            bool           acc_must_be_off        = true,
                                            log_label_t    log_label              = LOG_LABEL_FAIL,
                                            command_type_t execution_method       = PIN);

    /*!
     *  \brief         Проверка импульсов после запуска (старт-стоп)
     *
     *  \param  [in]   engine_start_system   - система запуска двгателя
     *  \param  [in]   bypass_waiting_time_s - время ожидания имульса обхода в секундах
     *  \param  [in]   log_label             - метка лога, который выводится при неполученном импульсе
     *
     *  \return        engine_start_error_t - ошибка
     */
    engine_start_error_t checkStartStopEngine(engine_start_system_t engine_start_system,
                                              float                 bypass_waiting_time_s,
                                              log_label_t           log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         Проверка импульсов после запуска (старт-стоп один импульс)
     *
     *  \param  [in]   bypass_waiting_time_s - время ожидания имульса обхода
     *  \param  [in]   rpm_val               - значение оборотов, которое выставляется после получения определенного импульса
     *  \param  [in]   log_label             - метка лога, который выводится при неполученном импульсе
     *
     *  \return        engine_start_error_t  - ошибка
     */
    engine_start_error_t checkStartStopOneImpulseEngine(float       bypass_waiting_time_s,
                                                        int16_t     rpm_val                = RPM_ON,
                                                        log_label_t log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         Проверка импульсов после запуска (старт-стоп два импульса)
     *
     *  \param  [in]   bypass_waiting_time_s - время ожидания имульса обхода
     *  \param  [in]   rpm_val               - значение оборотов, которое выставляется после получения определенного импульса
     *  \param  [in]   log_label             - метка лога, который выводится при неполученном импульсе
     *
     *  \return        engine_start_error_t  - ошибка
     */
    engine_start_error_t checkStartStopTwoImpulseEngine(float       bypass_waiting_time_s,
                                                        int16_t     rpm_val                = RPM_ON,
                                                        log_label_t log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         Проверка импульсов после запуска (старт-стоп три импульса)
     *
     *  \param  [in]   bypass_waiting_time_s - время ожидания имульса обхода
     *  \param  [in]   rpm_val               - значение оборотов, которое выставляется после получения определенного импульса
     *  \param  [in]   log_label             - метка лога, который выводится при неполученном импульсе
     *
     *  \return        engine_start_error_t  - ошибка
     */
    engine_start_error_t checkStartStopThreeImpulseEngine(float       bypass_waiting_time_s,
                                                          int16_t     rpm_val                = RPM_ON,
                                                          log_label_t log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         включение поддержки зажигания sup_ign_var 0-ручник/ 1 - авто
     *
     *  \param  [in]   sup_ign_var         - тип поддержки зажигания
     *  \param  [in]   engine_start_system - система запуска
     *  \param  [in]   ign_status          - выключать зажигание после выполнения или нет
     */
    virtual void setSupportIgn(eng_start_ign_supp_type_t sup_ign_var         = IGN_SUPP_TYPE_HAND_BRAKE_OR_PARKING,
                               engine_start_system_t     engine_start_system = IGNITION_LOCK,
                               ign_statuses_t            ign_status          = IGN_OFF);

    /*!
     *  \brief         Программная нейтраль
     *
     *  \param  [in]   execution_method     - метод исполнения
     *  \param  [in]   program_neutral_type - тип подхвата программной нейтрали true - по охране, false - по двери
     *  \param  [in]   sup_ign_var          - тип поддержки зажигания true - по зажиганию, false - по ручнику
     *  \param  [in]   engine_start_system  - система запуска, замок - 0, не замок > 0
     *  \param  [in]   engine_gearbox_type  - тип КПП false - МКПП, true - АКПП
     *  \param  [in]   ign_status           - выключать зажигание после выполнения ПЗ или нет
     *  \param  [in]   delay_s              - задержка после выполнения
     */
    virtual void setProgramNeutral(command_type_t                  execution_method,
                                   eng_start_neutral_detect_type_t program_neutral_type = NEUTRAL_DETECT_TYPE_DOOR,
                                   eng_start_ign_supp_type_t       sup_ign_var          = IGN_SUPP_TYPE_HAND_BRAKE_OR_PARKING,
                                   engine_start_system_t           engine_start_system  = IGNITION_LOCK,
                                   eng_start_gearbox_type_t        engine_gearbox_type  = ENG_GEARBOX_MANUAL,
                                   ign_statuses_t                  ign_status           = IGN_OFF,
                                   float                           delay_s              = TIME_DELAY::MIN_DELAY_AFTER_PROGRAM_NEUTRAL_S);

    /*!
     *  \brief         Включить/отключить доп. датчик
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   state            - состояние (1 - включить, 0 - выключить)
     */
    void setAdditionalSensor(command_type_t execution_method, bool state);
                                     
    /*!
     *  \brief         Проверка Программной нейтрали
     *
     *  \return        bool
     *  \retval        true  - статус в регионе: ПН выполнена
     *  \retval        false - статус в регионе: ПН не выполнена
     */
    bool getProgramNeutral();
                                    
    /*!
     *  \brief         Проверка поддержка зажигания
     *
     *  \return        bool
     *  \retval        true  - статус в регионе: ПЗ выполнена
     *  \retval        false - статус в регионе: ПЗ не выполнена
     */
    bool getSupportIgn();
    
    /*!
     *  \brief         Проверка Статуса BLE
     *
     *  \return        bool
     *  \retval        true  - статус в регионе: метка и R6 на связи
     *  \retval        false - статус в регионе: метка и/или R6 не на связи
     */
    virtual bool getBleStatus(command_type_t execution_method = SLP);
    
    /*!
     *  \brief         Проверка Статуса R6
     *
     *  \return        bool
     *  \retval        true  - статус в регионе: R6 на связи
     *  \retval        false - статус в регионе: R6 не на связи
     */
    virtual bool getR6Status(command_type_t execution_method = SLP);

    /*!
     *  \brief         Проверка Статуса метки
     *
     *  \return        bool
     *  \retval        true  - статус в регионе: метка на связи
     *  \retval        false - статус в регионе: метка не на связи
     */
    virtual bool getTagStatus(command_type_t execution_method = SLP);
                                    
    /*!
     *  \brief         Проверка ТТ
     */
    virtual bool getTurboTimer();

    /*!
     *  \brief         Установить состояние "Снято"
     */
    void setDisarmSt();

    /*!
     *  \brief         Установка начального состояния
     *
     *  \param  [in]   state_num - номер состояния
     */
    virtual void setState(int8_t state_num = FULL);
    
    /*!
     *  \brief         Статус сирены
     *
     *  \return        Статус
     */
    virtual int8_t getAlarm();

    /*!
     *  \brief         Статус блокировки
     *
     *  \param  [in]   execution_method - метод исполнения
     *
     *  \return        Статус
     */
    virtual int8_t getBlock(command_type_t execution_method = PIN);

    /*!
     *  \brief         Ожидание статуса метки и R6
     *
     *  \param  [in]   time     - время ожидания
     *  \param  [in]   polarity - полярность
     *
     *  \return        Статус
     */
    bool waitBleStatus(float      time,
                       bool       error_flag = false,
                       float      error      = TIME_DELAY::NULL_DELAY_S);
                       
    /*!
     *  \brief         Ожидание статуса R6
     *
     *  \param  [in]   time           - время ожидания
     *  \param  [in]   polarity       - полярность
     *  \param  [in]   waiting_status - ожидаемый статус
     *
     *  \return        Статус
     */
    bool waitR6Status(float      time,
                      bool       error_flag     = false,
                      float      error          = TIME_DELAY::NULL_DELAY_S,
                      bool       waiting_status = true);

    /*!
     *  \brief         Ожидание импульсов от подкапотного реле (R4, R6) отпирания и запирания замка капота
     *
     *  \param  [in]  *relay              - указатель на реле, с которого будем считывать импульсы
     *  \param  [in]   lock_pulse_count   - ожидаемое количество импульсов запирания (заданы в r6_pulse_t)
     *  \param  [in]   unlock_pulse_count - ожидаемое количество импульсов отпирания (заданы в r6_pulse_t)
     *  \param  [in]   timeout_s          - время ожидания
     *
     *  \return        Соответствует ли количество импульсов ожидаемому
     */
    bool waitRelayLockAndUnlock(RelayBase *relay,
                                uint8_t    lock_pulse_count,
                                uint8_t    unlock_pulse_count,
                                float      timeout_s);

    /*!
     *  \brief         Ожидание импульсов R4 отпирания и запирания замка капота
     *
     *  \param  [in]   lock_pulse_count   - ожидаемое количество импульсов запирания (заданы в relay_pulse_t)
     *  \param  [in]   unlock_pulse_count - ожидаемое количество импульсов отпирания (заданы в relay_pulse_t)
     *  \param  [in]   timeout_s          - время ожидания
     *
     *  \return        Соответствует ли количество импульсов ожидаемому
     */
    bool waitR4LockAndUnlock(uint8_t lock_pulse_count,
                             uint8_t unlock_pulse_count,
                             float   timeout_s = TIME_DELAY::MAX_R4_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание сигнала сирены (штатной и R6)
     *
     *  \param  [in]   type      - тип проверяемой сирены
     *  \param  [in]   timeout_s - время ожидания сигнала в секундах
     *
     *  \return        bool
     *  \retval        true  - ожидаемые сигналы получены
     *  \retval        false - ожидаемые сигналы не получены или получен лишний сигнал (напр. если выбран R6 и будет получен импульс штатной сирены, то вернется false)
     */
    bool waitSirenType(siren_type_mask_t type, float timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание импульсов R6 отпирания и запирания замка капота
     *
     *  \param  [in]   lock_pulse_count   - ожидаемое количество импульсов запирания (заданы в relay_pulse_t)
     *  \param  [in]   unlock_pulse_count - ожидаемое количество импульсов отпирания (заданы в relay_pulse_t)
     *  \param  [in]   timeout_s          - время ожидания
     *
     *  \return        Соответствует ли количество импульсов ожидаемому
     */
    bool waitR6LockAndUnlock(uint8_t lock_pulse_count,
                             uint8_t unlock_pulse_count,
                             float   timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание потенциала R6 на контакте запирания замка капота
     *
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        Соответствует ли количество импульсов ожидаемому
     */
    bool waitR6LockPotential(float timeout_s = TIME_DELAY::MIN_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание потенциала R6 на контакте отпирания замка капота
     *
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        Соответствует ли количество импульсов ожидаемому
     */
    bool waitR6UnlockPotential(float timeout_s = TIME_DELAY::MIN_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание сигналов сирены R6
     *
     *  \param  [in]   signal_count - количество ожидаемых сигналов (relay_pulse_t)
     *  \param  [in]   timeout_s    - время ожидания
     *
     *  \return        bool
     *  \retval        true  - количество сигналов соответствует ожидаемому
     *  \retval        false - количество сигналов не соответствует ожидаемому
     */
    bool waitR6SirenSignals(uint8_t signal_count, float timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание срабатывания блокировки реле (R4 или R6) (тип блокировки: НР)
     *
     *  \param  [in]  *relay     - указатель на реле
     *  \param  [in]   status    - ожидаемое состояние блокировки
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - блокировка в желаемом состоянии
     *  \retval        false - блокировка не в желаемом состоянии
     */
    bool waitRelayBlockNormalOpen(RelayBase *relay,
                                  bool       status,
                                  float      timeout_s);

    /*!
     *  \brief         Ожидание срабатывания блокировки R6 (тип блокировки: НР)
     *
     *  \param  [in]   status    - ожидаемое состояние блокировки
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - блокировка в желаемом состоянии
     *  \retval        false - блокировка не в желаемом состоянии
     */
    bool waitR6BlockNormalOpen(bool status, float timeout_s = TIME_DELAY::MAX_R6_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание срабатывания блокировки R6 (тип блокировки: НЗ)
     *
     *  \param  [in]   status    - ожидаемое состояние блокировки
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - блокировка в желаемом состоянии
     *  \retval        false - блокировка не в желаемом состоянии
     */
    bool waitR6BlockNormalClose(bool status, float timeout_s = TIME_DELAY::MAX_R6_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание срабатывания блокировки R4 (тип блокировки: НР)
     *
     *  \param  [in]   status    - ожидаемое состояние блокировки
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - блокировка в желаемом состоянии
     *  \retval        false - блокировка не в желаемом состоянии
     */
    bool waitR4Block(bool status, float timeout_s = TIME_DELAY::MAX_R4_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание срабатывания внешней блокировки R4 (контакт Output)
     *
     *  \param  [in]   status    - ожидаемое состояние
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \retval        true  - блокировка в желаемом состоянии
     *  \retval        false - блокировка не в желаемом состоянии
     */
    bool waitR4Output(bool status, float timeout_s = TIME_DELAY::MAX_R4_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание сигнала сирены или клаксона R6
     *
     *  \param  [in]   timeout_s - время ожидания
     *  \param  [in]   type      - тип сигнала (R6_SIREN - сирена, R6_KLAXON - клаксон)
     *
     *  \return        Был ли получен сигнал
     */
    bool waitR6Alarm(r6_alarm_signal_t type      = R6_SIREN,
                     float             timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание потенциала сирены R6
     *
     *  \param  [in]   polarity  - полярность сигнала
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - потенциал получен
     *  \retval        false - потенциал не получен
     */
    bool waitR6SirenPotential(r6_siren_potential_t polarity, float timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         Проверка статусного светодиода на R6 ECO
     *
     *  \param  [in]   led_ext_mode - режим работы
     *  \param  [in]   timeout_s    - время проверки
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkR6EcoLedExt(r6eco_led_ext_mode_t led_ext_mode, float timeout_s);

    /*!
     *  \brief         Ожидание статуса метки
     *
     *  \param  [in]   time_s     - время ожидания
     *  \param  [in]   error_flag - учитывать ли погрешность
     *  \param  [in]   error_s    - погрешность
     *
     *  \return        Статус
     */
    bool waitTagStatus(float time_s,
                       bool  error_flag = false,
                       float error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Статус капота
     *
     *  \param  [in]   execution_method - метод исполнения
     *
     *  \return        Статус
     */
    virtual int8_t getHood(command_type_t execution_method);

    /*!
     *  \brief         Статус зажигания
     *
     *  \param  [in]   execution_method - метод исполнения
     *
     *  \return        Статус
     */
    virtual int8_t getIgn(command_type_t execution_method);

    /*!
     *  \brief         Включена ли блокировка двигателя
     *
     *  \return        1 - вкл., 0 - выкл.
     */
    bool isBlockSet();

    /*!
     *  \brief         Видима ли метка
     *
     *  \param  [in]   execution_method - метод исполнения
     *
     *  \return        1 - да, 0 - нет
     */
    bool isTagVisible(command_type_t execution_method = PIN);

    /*!
     *  \brief         Дистанционное глушение
     *
     *  \param  [in]   engine_start_system    - система запуска
     *  \param  [in]   stop_after_impulse_num - остановка после импульса
     *  \param  [in]   time_s                 - время ожидания первого импульса
     */
    virtual void checkEngineStop(engine_start_system_t engine_start_system    = IGNITION_LOCK,
                                 stop_impulse_t        stop_after_impulse_num = STOP_AFTER_SECOND_IMPULSE_REMOVED,
                                 float                 time_s                 = TIME_DELAY::FIRST_STOP_IMPULSE_RECV_WAIT_S);

    /*!
     *  \brief         Проверка поддержки зажигания
     *
     *  \param  [in]   ign_status - нужно ли выключить зажигание после проверки
     *  \param  [in]   state      - ожидаемое состояние ПЗ
     */
    virtual void checkSupIgn(ign_statuses_t ign_status, out_statuses_t state);

    /*!
     *  \brief         Проверка охраны
     *
     *  \return        состояние
     */
    int8_t checkSecure(int8_t model = ON);
    
    /*!
     *  \brief         Проверка охраны через мобильное приложение по BLE
     *
     *  \param  [in]   model     - Желаемый статус
     *  
     *  \return        Удовлетворяет ли желаемому состоянию
     */
    int8_t checkSecureMobile(int8_t model = ON);
    

    /*!
     *  \brief         Ожидание статуса системы
     *
     *  \param  [in]   status       - ожидаемый статус
     *  \param  [in]   state        - ожидание появления (true) или снятия (false) указанного статуса
     *  \param  [in]   time_s       - время ожидания в секундах
     *  \param  [in]   error_time_s - допустимая погрешность в секундах
     *
     *  \return        bool
     *  \retval        true   - статус соответствует ожидаемому
     *  \retval        false  - статус не соответствует ожидаемому
     */
    bool waitGuardState(slp_diag_data_statuses_t status,
                        bool                     state,
                        float                    time_s,
                        float                    error_time_s = TIME_DELAY::NO_ERROR_TIME_S);

    /*!
     *  \brief         Проверка аналогового обходчика
     *
     *  \param  [in]   state - ожидаемое значение
     */
    virtual void checkInspector(int8_t state);

    /*!
     *  \brief         Синхронизация по IGN1
     *
     *  \param  [in]   time_s - время ожидания
     */
    void synByIgn1(float time_s = TIME_DELAY::FIVE_MINUTE);

    /*!
     *  \brief         Синхронизация по аналоговому обходчику
     */
    virtual void synByInspector();

    /*!
     *  \brief         Бдящая задержка по турботаймеру
     *
     *  \param  [in]   turbotimer_time_s   - время работы турботаймера
     *  \param  [in]   engine_start_system - тип запуска
     *  \param  [in]   error_time_s        - погрешность окончания турботаймера
     */
    virtual void waitTurbotimerTime(float                 turbotimer_time_s,
                                    engine_start_system_t engine_start_system,
                                    float                 error_time_s = TIME_DELAY::TT_ERROR_TIME_S);

    /*!
     *  \brief         Бдящая задержка по времени запуска
     *
     *  \param  [in]   autostart_time_s    - время работы автозапуска
     *  \param  [in]   engine_start_system - тип запуска
     *  \param  [in]   error_time_s        - погрешность окончания автозапуска
     */
    virtual void waitAutoStartTime(float                 autostart_time_s,
                                   engine_start_system_t engine_start_system,
                                   float                 error_time_s = TIME_DELAY::AUTO_START_REMOTE_ERROR_TIME_S);

    /*!
     *  \brief         Ожидание сигнала с провода запуска ППП
     *
     *  \param  [in]   polarity    - полярность провода
     *  \param  [in]   wait_time_s - время ожидания сигнала
     *
     *  \return        bool
     *  \retval        true  - сигнал получен
     *  \retval        false - сигнал не получен
     */
    bool waitPppStartSignal(bool polarity, float wait_time_s = TIME_DELAY::WAITING_HEAT_CTRL_PULSE_ERROR_S);

    /*!
     *  \brief         Ожидание импульса ППП
     *
     *  \param  [in]   ppp_max_time_s   - макс. время работы
     *  \param  [in]   polarity         - полярность провода
     *  \param  [in]   max_error_time_s - макс. допустимая погрешность
     */
    virtual void waitPppImpulse(float ppp_max_time_s,
                                bool  polarity,
                                float max_error_time_s);

    /*!
     *  \brief         Ожидание импульса ППП
     *
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - общее время ожидания 
     *  \param  [in]   pulse_time - время ожидания импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        bool
     *  \retval        true       - ожидаемые импульсы получены
     *  \retval        false      - ожидаемые импульсы не получены
     */
    bool waitPppPulse(uint32_t count,
                      bool     polarity,
                      float    time,
                      float    pulse_time,
                      bool     error_flag = false,
                      float    error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Ожидание потенциала ППП
     *
     *  \param  [in]   time_s    - время ожидания (время работы ППП)
     *  \param  [in]   polarity  - полярность провода
     *  \param  [in]   max_error - макс. допустимая погрешность
     *
     *  \return        bool
     *  \retval        true       - ожидаемый потенциал получен
     *  \retval        false      - ожидаемый потенциал не получен
     */
    virtual bool waitPppPotential(float time_s,
                                  bool  polarity,
                                  float max_error_s = TIME_DELAY::NO_ERROR_TIME_S);

    /*!
     *  \brief         Включение питания метки
     */
    void setTagVcc();

    /*!
     *  \brief         Выключение питания метки
     */
    void resetTagVcc();
    
    /*!
     *  \brief         Включение питания R4
     */
    void setR4Vcc();

    /*!
     *  \brief         Выключение питания R4
     */
    void resetR4Vcc();
    
    /*!
     *  \brief         Включение питания R6
     */
    void setR6Vcc();

    /*!
     *  \brief         Получение состояния питания R6
     *
     *  \return        bool
     *  \retval        true  - питание подано
     *  \retval        fasle - питание не подано
     */
    bool getR6Vcc();

    /*!
     *  \brief         Выключение питания R6
     */
    void resetR6Vcc();

    /*!
     *  \brief         Нажать на кнопку метки
     */
    void setTagBtn();

    /*!
     *  \brief         Отпустить кнопку метки
     */
    void resetTagBtn();

    /*!
     *  \brief         Нажатие на кнопку метки
     *
     *  \param  [in]   type - длительность нажатия
     */
    void pushTagBtn(float type);

    /*!
     *  \brief         Запуск ППП
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   btn1_time_s      - длительность нажатия 1ой кнопки брелока
     */
    virtual void startPpp(command_type_t execution_method,
                          float          btn1_time_s = KEYRING_SHORT);

    /*!
     *  \brief         Остановка ППП
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   ppp_type         - тип ППП
     *  \param  [in]   btn1_time_s      - длительность нажатия 2ой кнопки брелока
     */
    virtual void stopPpp(command_type_t execution_method,
                         int8_t         ppp_type    = 0,
                         float          btn2_time_s = KEYRING_SHORT);    

    /*!
     *  \brief         Отправить команду на запуск ППП
     *
     *  \detailed      Отличие от метода startPpp в том, что время ожидания вкл. провода ППП после отправки СМС можно задать параметром
     *                 Полярность провода определяется автоматически (если провод не назначен - ожидания вкл. ППП не будет)
     *
     *  \param  [in]   execution_method               - метод исполнения
     *  \param  [in]   eng_heat_after_sms_wait_time_s - время ожидания включения выхода контроля ППП после СМС
     */
    void setPppCmdStart(command_type_t execution_method, float eng_heat_after_sms_wait_time_s = TIME_DELAY::MAX_WAITING_SMS_STATUS_TIME_S);

    /*!
     *  \brief         Отправить команду на остановку ППП
     *
     *  \detailed      Отличие от stopPpp в том, что время ожидания выкл. провода ППП после отправки СМС можно задать параметром
     *                 Полярность провода определяется автоматически (если провод назначен, если не назначен - ожидания выкл. ППП не будет)
     *
     *  \param  [in]   execution_method               - метод исполнения
     *  \param  [in]   eng_heat_after_sms_wait_time_s - время ожидания выключения выхода контроля ППП после СМС
     */
    void setPppCmdStop(command_type_t execution_method, float eng_heat_after_sms_wait_time_s = TIME_DELAY::MAX_WAITING_SMS_STATUS_TIME_S);

    /*!
     *  \brief         Управление сервисной кнопкой
     *
     *  \param  [in]   state - состояние
     */
    void setServiceBtn(bool state);

    /*!
     *  \brief         Нажать сервисную кнопку
     *
     *  \param  [in]   count   - кол-во нажатий
     *  \param  [in]   delay_s - задержка между нажатиями
     */
    void clickServiceBtn(uint32_t count   = COUNT_STRUCT::MIN_BTN_CLICKS_COUNT, 
                         float    delay_s = TIME_DELAY::MIN_BTN_BETWEEN_CLICK_S);

    /*!
     *  \brief         Включен ли светодиод у сервисной кнопки
     *
     *  \return        1 - включен, 0 - нет
     */
    bool isServiceLedOn();

    /*!
     *  \brief         Ожидание статуса сервисного светодиода
     *
     *  \param  [in]   out_status - ожидаемый статус (включен/выключен)
     *  \param  [in]   timeout_s  - время ожидания
     *
     *  \return        bool
     *  \retval        true  - ожидаемый статус получен
     *  \retval        false - ожидаемый статус не получен
     */
    bool waitServiceLed(out_statuses_t out_status, float timeout_s);

    /*!
     *  \brief         Ожидание импульса светодиода сервисной кнопки
     *
     *  \param  [in]   pulse_width_s - ожидаемая длительность импульса
     *  \param  [in]   err_time_s    - погрешность импульса
     *  \param  [in]   timeout_s     - время ожидания
     *
     *  \return        bool
     *  \retval        true  - импульс получен
     *  \retval        false - импульс не получен или погрешность не соответствует ожидаемой
     */
    bool waitSrvBtnPulse(float pulse_width_s,
                         float err_time_s,
                         float timeout_s = TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    /*!
     *  \brief         Ожидание импульса
     *
     *  \param  [in]   amount - количество импульсов
     *  \param  [in]   time_s - время ожидания
     *
     *  \return        1 - импульс получен, 0 - нет
     */
    bool waitServicePulseAmount(uint8_t amount, float time_s);

    /*!
     *  \brief         Экстренное снятие с охраны без зажигания
     *
     *  \param  [in]  *pin   - код экстренного снятия
     *
     *  \return        1 - удачно, 0 - нет
     */
    bool emergencyDisarming(const char *pin = NULL);
    
    /*!
     *  \brief         Проверка смены кода экстренного снятия с охраны без зажигания
     *
     *  \param  [in]   digits - выход из алгоритма после определнной цифры
     *  \param  [in]  *pin    - код экстренного снятия
     *
     *  \return        1 - удачно, 0 - нет
     */
    bool checkChangeEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits);
    
    /*!
     *  \brief         Cмена кода экстренного снятия с охраны без зажигания
     *
     *  \param  [in]  *old_pin    - код экстренного снятия
     *  \param  [in]  *new_pin
     *
     *  \return        1 - удачно, 0 - нет
     */
    void changeEmergencyDisarming(const char *old_pin, const char *new_pin);
    
    /*!
     *  \brief         Проверка экстренного снятия с охраны без зажигания
     *
     *  \param  [in]   digits - выход из алгоритма после определнной цифры
     *  \param  [in]  *pin    - код экстренного снятия
     *
     *  \return        1 - удачно, 0 - нет
     */
    bool checkEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits);

    /*!
     *  \brief         Экстренное снятие с охраны
     *
     *  \param  [in]  *pin - код экстренного снятия
     *
     *  \return        1 - удачно, 0 - нет
     */
    bool emergencyDisarmingIgn(const char *pin = NULL);

    /*!
     *  \brief         Вход в режим регистрации
     *
     *  \param  [in]   state          - состояние (1 - включение, 0 - выход из режима)
     *  \param  [in]   reg_queue_mode - использовать ли очередь на регистрацию (вставать в очередь или регистрироваться без очереди)
     *
     *  \return        1 - удачно, 0 - нет
     */
    virtual bool registrationMode(bool state, registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         Встать в очередь на регистрацию устройств
     *
     *  \return        bool
     *  \retval        true  - постановка в очередь удачна (конец очереди достигнут)
     *  \retval        false - постановка в очередь неудачна (конец очереди не достигнут)
     */
    bool addDeviceToRegistrationQueue();

    /*!
     *  \brief         Посылка команды для включения свободных рук через меню брелока
     */
    void setHandsFreeKeyring();
    
    /*!
     *  \brief         Включение сервисного режима через брелок
     */
    void setServiceModeKeyring();

    /*!
     *  \brief         Посылка команды на вход в сервисный режим через метку
     *
     *  \param  [in]   state - статус (1 - вкл., 0 - выкл.)
     *
     *  \return        bool
     *  \retval        true           - статус в регионе соответствует ожидаемому
     *  \retval        false          - статус в регионе не соответствует ожидаемому
     */
    bool setServiceModeTag(bool state);

    /*!
     *  \brief         Управление сервисным режимом
     *
     *  \param  [in]   out_status - желаемое состояние (вкл./выкл.)
     *
     *  \return        bool
     *  \retval        true  - алгоритм выполнен
     *  \retval        false - алгоритм не выполнен (нет необходимых модулей)
     */
    bool setServiceMode(out_statuses_t out_status);

    /*!
     *  \brief         Получение статуса SLP
     *
     *  \param  [in]   addr   - адрес региона
     *  \param  [in]   size   - размер
     *  \param  [in]   status - ожидаемый статус
     *
     *  \return        bool
     *  \retval        true  - статус соответствует ожидаемому
     *  \retval        false - статус не соответствует ожидаемому
     */
    bool getSlpCmdStatus(slp_statuses_t addr,
                         uint32_t       size,
                         uint32_t       status);

    /*!
     *  \brief         Ожидание статуса SLP
     *
     *  \param  [in]   addr           - адрес региона
     *  \param  [in]   size           - размер
     *  \param  [in]   status         - ожидаемый статус
     *  \param  [in]   state          - ожидаемое состояние статуса (установлено/нет)
     *  \param  [in]   time_s         - время ожидания
     *  \param  [in]   error_time_s   - допустимая погрешность в секундах
     *  \param  [in]   error_time_use - влияет ли погрешность на допустимый диапазон времени, когда приходит статус
     *
     *  \return        bool
     *  \retval        true           - статус соответствует ожидаемому
     *  \retval        false          - статус не соответствует ожидаемому
     */
    bool waitSlpCmdStatus(slp_statuses_t addr,
                          uint32_t       size,
                          uint32_t       status,
                          bool           state,
                          float          time_s,
                          float          error_time_s = TIME_DELAY::NO_ERROR_TIME_S,
                          bool           error_time_use = true);
    
    /*!
     *  \brief         Имитация срабатывания дополнительного датчика 1 (по региону)
     */
    void setAuxSensor1();

    /*!
     *  \brief         Имитация срабатывания дополнительного датчика 1 (по проводу)
     *
     *  \param  [in]   status  - желаемое состояние (вкл. / выкл.)
     *  \param  [in]   delay_s - задержка после управления
     */
    void setAuxSensorFirstAnalog(out_statuses_t status, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Имитация срабатывания дополнительного датчика 2
     */
    void setAuxSensor2();

    /*!
     *  \brief         Имитация срабатывания дополнительного датчика 2 (по проводу)
     *
     *  \param  [in]   status  - желаемое состояние (вкл. / выкл.)
     *  \param  [in]   delay_s - задержка после управления
     */
    void setAuxSensorSecondAnalog(out_statuses_t status, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Имитация срабатывания датчика удара по верхнему порогу
     */
    void setShockSensorHigh();

    /*!
     *  \brief         Имитация срабатывания датчика удара по нижнему порогу
     */
    void setShockSensorLow();
    
    /*!
     *  \brief         Имитация срабатывания датчика движения
     */
    void setMoveSensor();
    
    /*!
     *  \brief         Имитация срабатывания датчика наклона
     */
    void setTiltSensor();

    /*!
     *  \brief         Имитировать один из датчиков
     *
     *  \param  [in]   sensor_type - имитируемый датчик
     */
    void setSensor(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         Имитировать один из датчиков (без задержки после управления)
     *
     *  \param  [in]   sensor_type - имитируемый датчик
     */
    void setSensorNoDelay(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         Выставить температуру двигателя
     *
     *  \param  [in]   value - значение температуры
     */
    virtual void setEngineTemperatureSensor(int8_t value);

    /*!
     *  \brief         Выставить температуру двигателя (без задержки после управления)
     *
     *  \param  [in]   value - значение температуры
     */
    virtual void setEngineTemperatureSensorNoDelay(int8_t value);

    /*!
     *  \brief         Выставить температуру на плате
     *
     *  \param  [in]   value - значение температуры
     */
    virtual void setBoardTemperatureSensor(int8_t value);

    /*!
     *  \brief         Выставить температуру двигателя по CAN на автомобиле Lada Vesta (7114)
     *
     *  \param  [in]   value - значение температуры
     */
    virtual void setEngineTemperatureCanLadaVesta(int8_t value);

    /*!
     *  \brief         Получение состояния звукового сигнала
     *
     *  \param  [in]   time       - время ожидания
     *  \param  [in]   time_error - допустимая погрешность
     *
     *  \return        статус (NOSIGNAL, PULSED, CONST)
     */
    alarm_signal_t getAlarmSignal(float time, float time_error = TIME_DELAY::NO_ERROR_TIME_S);

    /*!
     *  \brief         Ожидание блокировки
     *
     *  \param  [in]   time_s           - время ожидания
     *  \param  [in]   error_time_s     - погрешность (TIME_DELAY::NO_ERROR_TIME_S - не учитывается)
     *  \param  [in]   status           - ожидаемый статус (true - включение)
     *  \param  [in]   execution_method - метод исполнения
     *
     *  \return        bool
     *  \retval        true - статус соответствует ожидаемому
     */
    bool waitBlock(float          time_s,
                   float          error_time_s     = TIME_DELAY::NO_ERROR_TIME_S,
                   bool           status           = true,
                   command_type_t execution_method = PIN);

    /*!
     *  \brief         Посылка команды на вход/выход в режим Антиограбления
     *
     *  \param  [in]   state               - состояние
     *  \param  [in]  *pin                 - код экстренного снятия
     *  \param  [in]  *gsm_owner_access_pw - гостевой GSM пароль
     *
     *  \return        1 - команда выполнена успешно
     */
    bool setAnj(bool        state,
                const char *pin                 = NULL,
                const char *gsm_owner_access_pw = NULL,
                bool        no_wait_message     = OFF);
                
    /*!
     *  \brief         Регистрация мобильного приложения
     *
     *  \return        1 - команда выполнена успешно
     */
    bool registerMobile();

    /*!
     *  \brief         Проверка готовности к работе моб. приложения
     *
     *  \return        1 - устройство готово к работе
     */
    bool checkMobile();

    /*!
     *  \brief         Регистрация метки
     *
     *  \return        1 - команда выполнена успешно
     */
    bool registerTag();

    /*!
     *  \brief         Регистрация доп. брелка
     *
     *  \return        1 - команда выполнена успешно
     */
    bool registerLedKeyring();

    /*!
     *  \brief         Регистрация ЖК брелка
     *
     *  \return        1 - команда выполнена успешно
     */
    bool registerLcdKeyring();

    /*!
     *  \brief         Регистрация R6
     *
     *  \return        true - команда выполнена успешно
     */
    bool registerR6();

    /*!
     *  \brief         Регистрация R6 ECO
     *
     *  \return        Статус регистрации
     */
    bool registerR6Eco();

    /*!
     *  \brief         Получить указатель на R6
     *
     *  \return        указатель на R6
     */
    R6 *getR6();

    /*!
     *  \brief         Получить указатель на R6Eco
     *
     *  \return        указатель на R6Eco
     */
    R6Eco *getR6Eco();

    /*!
     *  \brief         Получение статуса SLP
     *
     *  \param  [in]   mode - отключать ли сервисный, если он включен
     *
     *  \return        1 - статус соответствует ожидаемому, 0 - нет
     */
    bool isServiceModeOn(bool mode);

    /*!
     *  \brief         Получение импульса
     *
     *  \param  [in]   pin        - пин, с которого считывается импульс
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - общее время ожидания 
     *  \param  [in]   pulse_time - время ожидания импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitPulse(DigitalIn *pin,
                   uint32_t   count,
                   bool       polarity,
                   float      time,
                   float      pulse_time,
                   bool       error_flag = false,
                   float      error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Отправка импульса
     *
     *  \param  [in]   pin          - пин, на который подается импульс
     *  \param  [in]   count        - количество импульсов
     *  \param  [in]   polarity     - полярность импульсов
     *  \param  [in]   pulse_time_s - время импульсов в секундах
     *  \param  [in]   interval_s   - время между импульсами в секундах
     */
    void sendPulse(DigitalOut *pin,
                   uint32_t    count,
                   bool        polarity,
                   float       pulse_time_s,
                   float       interval_s);

    /*!
     *  \brief         Перезапуск метки
     */
    void resetTag();

    /*!
     *  \brief         Получить подключенные модули
     *
     *  \return        Число, символизирующее подключенные модули
     */
    int32_t getModules();

    /*!
     *  \brief         Отправка DTMF команды
     *
     *  \param  [in]   to     - кому
     *  \param  [in]   msg    - текстовое сообщение
     *  \param  [in]   lenght - длина сообщения
     *
     *  \return        true - отправка прошла успешно, false - нет
     */
    bool sendDtmf(const char *to,
                  const char *msg,
                  uint16_t    lenght);

    /*!
     *  \brief         Обход штатного иммобилайзера
     *
     *  \param  [in]   state - вкл/выкл
     */
    void bypassLearn(bool state);

    /*!
     *  \brief         Добавить код экстренного снятия
     *
     *  \param  [in]  &pin - код экстренного снятия в виде строки
     */
    void addDevicePin(const string &pin);

    /*!
     *  \brief         Получить код экстренного снятия
     *
     *  \return        pin код экстренного снятия в виде строки
     */
    char *getDevicePin();

    /*!
     *  \brief         Получить неверный код экстренного снятия
     *
     *  \detailed      Для использования в тестах, где нужно вводить неверный код экстренного снятия
     *
     *  \param  [in]   pin_num - номер цифры кода, которую нужно изменить (напр. при коде 1111 и номере 3 - будет возвращен пин 1121)
     *
     *  \return        неверный pin код экстренного снятия в виде строки
     */
    char *getWrongDevicePin(emergency_dis_pin_num_t pin_num = EMERGENCY_DIS_PIN_NUM_ALL);

    /*!
     *  \brief         Проверка ПН
     *
     *  \param  [in]   model       - Ожидаемое состояние
     *  \param  [in]   timeout_s   - Время ожидания до включения (если включится раньше, то FAIL)
     *  \param  [in]   max_error_s - максимальная погрешность включения
     *
     *  \return        bool
     *  \retval        true        - успешная проверка
     *  \retval        false       - произошла ошибка
     */
    bool checkProgramNeutral(int8_t model,
                             float  timeout_s   = TIME_DELAY::NULL_DELAY_S,
                             float  max_error_s = TIME_DELAY::MAX_NEUTRAL_ON_DOOR_TOUT_ERROR_S);

    /*!
     *  \brief         Проверка ПЗ
     *
     *  \param  [in]   model - Ожидаемое состояние
     */
    void checkSupportIgn(int8_t model);
        
    /*!
     *  \brief         Проверка ТТ
     *
     *  \param  [in]   model - Ожидаемое состояние
     */
    void checkTurboTimer(int8_t model);
        
    /*!
     *  \brief         Проверка сирены
     *
     *  \param  [in]   model   - Ожидаемое состояние
     *  \param  [in]   timeout - Время ожидания состояния
     */
    void checkAlarm(int8_t model, float timeout = TIME_DELAY::STD_ALARM_SIGNAL_TIME_S);

    /*!
     *  \brief         Получить код валидатора (вводимые штатные кнопки при регистрации)
     *
     *  \return        ссылка на вектор с кодом валидатора
     */
    const std::vector<can_sts_btn_t>& getValidatorCode();

    /*!
     *  \brief         Получить неверный код валидатора (для неудачной авторизации)
     *
     *  \return        ссылка на вектор с кодом валидатора
     */
    const std::vector<can_sts_btn_t>& getWrongValidatorCode();

    /*!
     *  \brief         Отправка кода валидатора (штатные кнопки автомобиля)
     */
    void sendValidatorCode();

    /*!
     *  \brief         Отправка неверного кода валидатора (штатные кнопки автомобиля) (для неудачной авторизации)
     */
    void sendWrongValidatorCode();

    /*!
     *  \brief         Регистрация штатных кнопок
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - не удачно
     */
    bool registerValidatorCode();

    /*!
     *  \brief         Регистрация всех устройств
     *
     *  \param  [in]   reg_queue_mode - использовать ли очередь на регистрацию (вставать в очередь или регистрироваться без очереди)
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - не удачно
     */
    virtual bool registerAll(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         Загрузка прошивки основного блока
     *
     *  \param  [in]   firmware     - имя прошивки
     *  \param  [in]   time         - время ожидания
     *  \param  [in]   from_dropbox - из Dropbox
     *  \param  [in]  *file_path    - путь до файла
     *
     *  \return        1 - удачно, 0 - нет
     */
    bool loadFirmware(const char *firmware,
                      float       time         = TIME_DELAY::TEN_MINUTE,
                      bool        from_dropbox = false,
                      const char *file_path    = NULL);

    /*!
     *  \brief         Из входной строки извлечь статус успеха загрузки прошивки
     *
     *  \param  [in]  *buf - строка с информацией
     *
     *  \return        bool
     *  \retval        true  - прошивка загружена успешно
     *  \retval        false - ошибка при загрузке прошивки
     */
    bool parseFirmwareLoadError(const char *buf);

    /*!
     *  \brief         Загрузка прошивки основного блока с сервера apps.starline.ru
     *
     *  \param  [in]   fw_download_type - тип скачиваемой версии ПО
     *  \param  [in]   timeout_s        - время ожидания ответа
     *  \param  [in]  *special_fw_ver   - указатель на строку с особой версией ПО. Если версия ПО указана, то предыдущие версии будут скачиваться относительно нее
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - нет
     */
    bool loadFirmwareFromServer(fw_download_type_t  fw_download_type = FW_DOWNLOAD_TYPE_SECOND_PREV,
                                float               timeout_s        = TIME_DELAY::MAX_WAITING_FW_TIME_S,
                                const char         *special_fw_ver   = NULL);

    /*!
     *  \brief         Загрузка последней версии прошивки основного блока
     *
     *  \param  [in]   timeout_s - время ожидания ответа
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - нет
     */
    bool loadLastFirmware(float timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         Загрузка прошивки в устройство
     *
     *  \param  [in]  *ver       - версия прошивки (напр.: 2.15.0.5037)
     *  \param  [in]   timeout_s - время ожидания ответа
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - нет
     */
    bool loadFirmwareByVersion(const char *version, float timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         Получить версию прошивки
     *
     *  \param  [in]  *version   - буфер для записи версии
     *  \param  [in]   type      - версия какой прошивки (текущей, поледней бета или последней релизной)
     *  \param  [in]   timeout_s - время ожидания ответа
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - нет
     */
    bool getFirmwareVersion(char     *version,
                            fw_type_t type      = FW_TYPE_CURRENT,
                            float     timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         Узнать, является ли текущая прошивка Hot
     *
     *  \param  [in]   time - время ожидания ответа
     *
     *  \return        fw_type_t
     *  \retval        FW_TYPE_UNKNOWN - неизвестно (таймаут получения ответа)
     *  \retval        FW_TYPE_HOT     - версия Hot
     *  \retval        FW_TYPE_NOT_HOT - версия не Hot
     */
    fw_type_t checkHotFirmware(float time = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         Переключить прошивку устройства
     *
     *  \param  [in]   time - время ожидания ответа
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - нет
     */
    bool resetFirmware(float time = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         Загрузить индивидуальную прошивку CAN
     *
     *  \param  [in]   can_car_num_str - номер автомобиля CAN (строка)
     *  \param  [in]   timeout_s       - время ожидания ответа
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - неудачно
     */
    bool loadCanFirmware(const char *can_car_num_str, float timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         Аналоговый slave

     *  \param  [in]   lights             - режим поворотников
     *  \param  [in]   state              - управление (вкл./выкл.)
     *  \param  [in]   lock_pulse_time_s  - время импульсов ЦЗ в секундах
     *  \param  [in]   light_pulse_time_s - время импульсов поворотников в секундах
     *  \param  [in]   interval_s         - промежуток времени, в течении которого отправляются импульсы
     */
    void analogSlave(slave_lights_t lights,
                     bool           state,
                     float          lock_pulse_time_s,
                     float          light_pulse_time_s,
                     float          interval_s = TIME_DELAY::FIVE_S);

    /*!
     *  \brief         Проверка на работоспособность Shield-a
     *
     *  \param  [in]   time  - время ожидания ответа в секундах
     *
     *  \return        bool
     *  \retval        true  - удачная проверка
     *  \retval        false - неудачная проверка
     */
    bool checkShield(float time = 9.0f);

    /*!
     *  \brief         Ожидание статуса звукоизлучателя
     *
     *  \param  [in]   status - ожидаемый статус (вкл./выкл.)
     *  \param  [in]   time   - время ожидания ответа в секундах
     *
     *  \return        buzzer_status_t
     *  \retval        BUZZER_NONE - не определено
     *  \retval        BUZZER_ON   - включен
     *  \retval        BUZZER_OFF  - отключен
     */
    buzzer_status_t waitBuzzerStatus(bool status, float time);

    /*!
     *  \brief         Управление питанием доп. брелка
     *
     *  \param  [in]   state - вкл./выкл.
     */
    void setLedKeyringVcc(bool state);

    /*!
     *  \brief         Управление питанием ЖК брелка
     *
     *  \param  [in]   state   - вкл./выкл.
     *  \param  [in]   delay_s - задержка после исполнения
     */
    void setLcdKeyringVcc(bool  state,
                          float delay_s = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Проверка метода исполнения команды
     *
     *  \param  [in]   execution_method - метод исполнения
     *
     *  \return        Окончательный метод исполнения
     */
    command_type_t getExecutionMethod(command_type_t  execution_method,
                                      action_type_t   action_type,
                                      DigitalOut     *pin = NULL);

    /*!
     *  \brief         Установка скорости
     *
     *  \param  [in]   value - значение
     */
    void setSpeed(int8_t value);

    /*!
     *  \brief         Проверка скорости
     *
     *  \param  [in]   speed_kmph - значение скорости (км/ч)
     *  \param  [in]   timeout_s  - максимальное время ожидания (секунд)
     *
     *  \retval        bool
     *  \return        true       - значение соответствует ожидаемому
     *  \return        false      - значение не соответствует ожидаемому
     */
    bool checkSpeed(int8_t speed_kmph, float timeout_s = TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    /*!
     *  \brief         Статус Антиограбления
     *
     *  \return        Статус
     */
    ahj_status_t getAntiHijacking();

    /*!
     *  \brief         Ожидание включение режима Антиограбление
     *
     *  \param  [in]   status  - ожидаемый статус
     *  \param  [in]   timeout - максимальное время ожидания
     *
     *  \return        Статус
     */
    bool waitAntiHijacking(ahj_status_t status,
                           float        timeout = TIME_DELAY::NULL_DELAY_S);
    
    /*!
     *  \brief         Ожидание предупрждения сиреной и световыми сигланали
     *
     *  \param  [in]   timeout - максимальное время ожидания
     *
     *  \return        Статус
     */
    bool waitLightAndAlarm(float timeout);
    
    /*!
     *  \brief         Выключение антиограбления кодом
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   confirmation     - подтверждение
     */
    void offAntiHijacking(command_type_t execution_method = EMERGENCY,
                          bool           confirmation     = true);
    
    /*!
     *  \brief         Включение антиограбления
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   confirmation     - подтверждение
     *  \param  [in]  *gsm_pass         - GSM пароль
     *  \param  [in]   ahj_check        - проверка антиограбления 
     */
    void onAntiHijacking(command_type_t  execution_method,
                         bool            confirmation      = ON,
                         const char     *gsm_pass          = NULL,
                         bool            ahj_check         = ON);

    /*!
     *  \brief         Активация сенсора ручки двери
     *
     *  \param  [in]   state - вкл/выкл
     */
    void setCapSensor(bool state);

    /*!
     *  \brief         Активация различных сенсоров ручки
     *
     *  \param  [in]   cap_sensor_type - тип активируемого сенсора
     *  \param  [in]   state           - вкл/выкл
     */
    void setCapSensor(cap_sensor_type_t cap_sensor_type, bool state);

    /*!
     *  \brief         Отправить импульс сенсора ручки двери
     *
     *  \param  [in]   polarity   - полярность импульса
     *  \param  [in]   duration_s - длительность импульса
     */
    void sendCapSensorPulse(bool polarity, float duration_s);

    /*!
     *  \brief         Нажать (дотронуться) до датчика заданного типа
     *
     *  \param  [in]   cap_sensor_type - тип датчика
     *  \param  [in]   count           - количество нажатий
     *  \param  [in]   press_time_s    - длительность нажатия
     */
     void pushCapSensor(cap_sensor_type_t cap_sensor_type,
                        int8_t            count        = COUNT_STRUCT::MIN_CAP_SENSOR_CLICKS_COUNT,
                        float             press_time_s = TIME_DELAY::STD_CAP_SENSOR_PRESSED_TIME_S);

     /*!
      *  \brief        Проверка статуса емкостного датчика
      *
      *  \param  [in]  cap_sensor_type - тип датчика
      *  \param  [in]  state           - ожидаемое состояние
      *  \param  [in]  timeout_s       - время ожидания статуса
      *
      *  \retval       bool
      *  \return       true  - датчик в ожидаемом состоянии
      *  \retval       false - датчик не в ожидаемом состоянии
      */
     bool waitCapSensor(cap_sensor_type_t cap_sensor_type,
                        bool              state,
                        float             timeout_s = TIME_DELAY::WAITING_REGION_STATE_S);

    /*!
     *  \brief         Постановка в охрану при включенном режиме Пляж
     *
     *  \param  [in]   polarity      - полярность импульса
     *  \param  [in]   p1_duration_s - длительность первого импульса
     *  \param  [in]   p2_duration_s - длительность второго импульса
     *  \param  [in]   p3_duration_s - длительность третьего импульса
     */
    void setArmBeachMode(bool  polarity,
                         float p1_duration_s,
                         float p2_duration_s,
                         float p3_duration_s);

    /*!
     *  \brief         Снятие с охраны при включенном режиме Пляж
     *
     *  \param  [in]   polarity              - полярность импульса
     *  \param  [in]   duration_s            - длительность импульса
     *  \param  [in]  *pin                   - код экстренного снятия
     *  \param  [in]   pin_num_count         - количество цифр пин-кода, которые будут введены
     *  \param  [in]   wait_last_signal_flag - ожидать ли мигание светодиодом после ввода последней цифры кода
     */
    void setDisarmBeachMode(bool        polarity,
                            float       duration_s,
                            const char *pin                   = NULL,
                            uint8_t     pin_num_count         = 4,
                            bool        wait_last_signal_flag = true);

    /*!
     *  \brief         Распознать код экстренного снятия из строки
     *
     *  \param  [in]  *pin     - код экстренного снятия (строка)
     *  \param  [out] *pin_arr - код экстренного снятия (выходной массив)
     *
     *  \return        bool
     *  \retval        true    - удачное распознавание
     *  \retval        false   - ошибка распознавания
     */
    bool getPinFromChar(const char *pin, uint8_t *pin_arr);

    /*!
     *  \brief         Отправить импульсы поворотников для аналогово Slave
     *
     *  \param  [in]   polarity           - полярность
     *  \param  [in]   delay_light_r      - длительность имульса правого поворотника
     *  \param  [in]   delay_light_l      - длительность имульса левого поворотника
     *  \param  [in]   lights             - тип поворотников
     */                            
    void sendSlaveLightsPulses(bool           polarity,
                               float          delay_light_r, 
                               float          delay_light_l, 
                               slave_lights_t lights);

    /*!
     *  \brief         Выполнить действие (алгоритм для кейсов на аналоговый Slave)
     *
     *  \param  [in]   delay_between      - длительность между импульсами
     *  \param  [in]   need_open          - необходимость открытия багажника или двери
     */                            
    void doSlaveAct(float           delay_between, 
                    slave_actions_t need_open = SLAVE_ACT_NONE);

    /*!
     *  \brief         Выполнить алгоритм для кейсов на аналоговый Slave
     *
     *  \param  [in]   value              - значение
     *  \param  [in]   delay_clock        - длительность импульса ЦЗ
     *  \param  [in]   delay_between      - длительность между импульсами
     *  \param  [in]   delay_light_r      - длительность имульса правого поворотника
     *  \param  [in]   delay_light_l      - длительность имульса левого поворотника
     *  \param  [in]   lights             - тип поворотников
     *  \param  [in]   need_open          - необходимость открытия багажника или двери
     *  \param  [in]   light_before_clock - импульсы поворотников до ЦЗ
     */                            
    void sendAnalogSlaveStatus(int8_t          value, 
                               float           delay_clock, 
                               float           delay_between, 
                               float           delay_light_r, 
                               float           delay_light_l, 
                               slave_lights_t  lights, 
                               slave_actions_t need_open          = SLAVE_ACT_NONE,
                               bool            light_before_clock = false);

    /*!
     *  \brief         Ожидание импульсов ЦЗ, света и звука после снятия/постановки с охраны
     *
     *  \param  [in]   indication      - ожидаемый тип индикации
     *  \param  [in]   time_s          - время ожидания
     *  \param  [in]   time_error_s    - погрешность
     *  \param  [in]   lock_indication - ожидание импульса ЦЗ (true - ожидаем)
     *
     *  \return        bool
     *  \retval        true         - все импульсы получены
     *  \retval        false        - получены не все импульсы
     */
    bool waitClockAndSignal(arm_disarm_indication_t indication,
                            float                   time_s,
                            float                   time_error_s    = TIME_DELAY::NO_ERROR_TIME_S,
                            bool                    lock_indication = true);

    /*!
     *  \brief         Получить день недели
     *
     *  \param  [in]   day_no - номер дня недели
     *
     *  \return        строка
     */
    string getWeekDay(uint8_t day_no);

    /*!
     *  \brief         Получить системное время
     *
     *  \param  [out] &sys_time - системное время
     *
     *  \return        bool
     *  \retval        true  - время получено успешно
     *  \retval        false - время не получено
     */
    bool getSysTime(time_t &sys_time);

    /*!
     *  \brief         Ожидание инициализации GSM
     *
     *  \detailed      Считается инициализированным при получении системного времени, отличного от значение по-умолчанию
     *
     *  \return        bool
     *  \retval        true  - инициализация успешна
     *  \retval        false - инициализация неуспешна или текущее время совпадает со временем системы по-умолчанию
     */
    bool waitGsmTime(float timeout_s = TIME_DELAY::MAX_GSM_INIT_WAIT_TIME_S);

    /*!
     *  \brief         Ожидание в скеундах
     *
     *  \param  [in]   time_s - время ожидания
     */
    void wait_s(float time_s);

    /*!
     *  \brief         Получить строку по метке лога
     *
     *  \param  [in]   log_label - метка лога, который выводится при неполученном импульсе
     *  \param  [out] *str       - полученная строка
     *  \param  [in]   len       - длина строки
     *
     *  \return        bool
     *  \retval        true  - строка получена успешно
     *  \retval        false - строка не получена
     */
    bool getLogLabelStr(log_label_t  log_label,
                        char        *str,
                        uint32_t     len = LEN_STRUCT::MAX_STR_LOG_LABEL_LEN);

    /*!
     *  \brief         Получение импульса световых сигналов
     *
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - общее время ожидания 
     *  \param  [in]   pulse_time - длительность импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */

    bool waitLightsPulse(int32_t count,
                         bool    polarity,
                         float   time,
                         float   pulse_time,
                         bool    error_flag = false,
                         float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса альтернативных световых сигналов
     *
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - общее время ожидания 
     *  \param  [in]   pulse_time - длительность импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitAltLightPulse(int32_t count,
                           bool    polarity,
                           float   time,
                           float   pulse_time,
                           bool    error_flag = false,
                           float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса "ЦЗ открыть дверь водителя"
     *
     *  \param  {in]   count      - количество импульсов
     *  \param  {in]   polarity   - полярность импульсов
     *  \param  {in]   time       - общее время ожидания 
     *  \param  {in]   pulse_time - длительность импульса
     *  \param  {in]   error_flag - необходимо ли учитывать погрешность
     *  \param  {in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitUnlockDriverDoor(int32_t count,
                              bool    polarity,
                              float   time,
                              float   pulse_time,
                              bool    error_flag = false,
                              float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса альтернативного ЦЗ
     *
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - время ожидания импульсов
     *  \param  [in]   pulse_time - длительность импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitAltLockPulse(int32_t count,
                          bool    polarity,
                          float   time,
                          float   pulse_time,
                          bool    error_flag = false,
                          float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса сирены
     *
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - общее время ожидания 
     *  \param  [in]   pulse_time - длительность импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitSirenPulse(int32_t count,
                        bool    polarity,
                        float   time,
                        float   pulse_time,
                        bool    error_flag = false,
                        float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса клаксона
     *
     *  \param  [in]   count        - количество импульсов
     *  \param  [in]   polarity     - полярность импульсов
     *  \param  [in]   time_s       - общее время ожидания 
     *  \param  [in]   pulse_time_s - длительность импульса
     *  \param  [in]   error_flag   - необходимо ли учитывать погрешность
     *  \param  [in]   error_s      - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitKlaxonPulse(int32_t count,
                         bool    polarity,
                         float   time_s,
                         float   pulse_time_s,
                         bool    error_flag = false,
                         float   error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульсов буззера
     *
     *  \param  [in]   count      - количество импульсов
     *  \param  [in]   polarity   - полярность импульсов
     *  \param  [in]   time       - общее время ожидания 
     *  \param  [in]   pulse_time - длительность импульса
     *  \param  [in]   error_flag - необходимо ли учитывать погрешность
     *  \param  [in]   pulse_time - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitBuzzerExtPulse(int32_t count,
                            bool    polarity,
                            float   time,
                            float   pulse_time,
                            bool    error_flag = false,
                            float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса "Открыть капот"
     *
     *  \param  {in]   count        - количество импульсов
     *  \param  {in]   polarity     - полярность импульсов
     *  \param  {in]   time         - общее время ожидания 
     *  \param  {in]   pulse_time_s - длительность импульса
     *  \param  {in]   error_flag   - необходимо ли учитывать погрешность
     *  \param  {in]   error_s      - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitHoodOpenPulse(int32_t count,
                           bool    polarity,
                           float   time,
                           float   pulse_time_s,
                           bool    error_flag = false,
                           float   error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Получение импульса "Закрыть капот"
     *
     *  \param  {in]   count        - количество импульсов
     *  \param  {in]   polarity     - полярность импульсов
     *  \param  {in]   time         - общее время ожидания 
     *  \param  {in]   pulse_time_s - длительность импульса
     *  \param  {in]   error_flag   - необходимо ли учитывать погрешность
     *  \param  {in]   error_s      - величина погрешности
     *
     *  \return        статус (1 - все импульсы получены)
     */
    bool waitHoodClosePulse(int32_t count,
                            bool    polarity,
                            float   time,
                            float   pulse_time_s,
                            bool    error_flag = false,
                            float   error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Проверка цифрового входа
     *
     *  \param  [in]   dig_pin  - пин, с которого считывается потенциал
     *  \param  [in]   polarity - полярность
     *
     *  \return        bool
     *  \retval        true  - удачная проверка
     *  \retval        false - неудачная проверка
     */
    bool checkDigIn(PinName dig_pin, channel_polarity_t polarity);

    /*!
     *  \brief         Установить потенциал цифрового выхода
     *
     *  \param  [in]   dig_pin - пин, у которого задается потенциал
     *  \param  [in]   state   - значение потенциала
     *
     *  \return        bool
     *  \retval        true    - потенциал удачно задан
     *  \retval        false   - возникли ошибки
     */
    bool setDigOut(PinName dig_pin, bool state);
    
    /*!
     *  \brief         Отпереть багажник
     *
     *  \param  [in]   command_type - тип команды управления
     *  \param  [in]   delay_s      - задержка после отправки команды
     *  \param  [in]   repeat_mode  - повторять ли до успеха (проверка по SLP, максимум 3 попытки, только для брелока)
     *
     *  \return        bool
     *  \retval        true         - команда отправлена
     *  \retval        false        - команда не отправлена
     */
    bool unlockTrunk(command_type_t command_type,
                     float          delay_s      = TIME_DELAY::NULL_DELAY_S,
                     repeat_mode_t  repeat_mode  = REPEAT_MODE_OFF);

    /*!
     *  \brief         Установить полярность для проверки длительности импульса сервисного светодиода
     */
    void setPolaritySrvBtn();

    /*!
     *  \brief         Проверка длительности импульса сервисного светодиода
     *
     *  \param  [in]   pulse_time_s - длительность импульса
     *  \param  [in]   timeout_s    - таймаут
     *  \param  [in]   error_time_s - величина погрешности импульса
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkPulseWidthSrvBtn(float pulse_time_s,
                               float timeout_s,
                               float error_time_s);

    /*!
     *  \brief         Установить полярность для проверки длительности импульса на провод с данной функциональностью
     *
     *  \param  [in]   gen6_functional - функциональность провода
     */
    void setPolarity(gen6_functional_t gen6_functional);
    
    /*!
     *  \brief         Проверка длительности импульса на проводе с данной функциональностью
     *
     *  \param  [in]   gen6_functional - функциональность провода
     *  \param  [in]   pulse_time_s    - длительность импульса
     *  \param  [in]   timeout_s       - таймаут
     *  \param  [in]   error_time_s    - величина погрешности импульса
     *
     *  \return        bool
     *  \retval        true            - длительность импульса соответствует ожидаемому
     *  \retval        false           - длительность импульса не соответствует ожидаемому
     */
    bool checkPulseWidth(gen6_functional_t gen6_functional,
                         float             pulse_time_s,
                         float             timeout_s,
                         float             error_time_s = TIME_DELAY::MIN_PULSE_ERROR_S);  

    /*!
     *  \brief         Проверка наличия импульса на проводе
     *
     *  \param  [in]   gen6_functional - функциональность провода
     *  \param  [in]   timeout_s       - таймаут ожидания импульса
     *
     *  \return        bool
     *  \retval        true            - имульс получен
     *  \retval        false           - импульс не получен
     */
    bool checkPulseWithoutWidth(gen6_functional_t gen6_functional,
                                float             timeout_s);                      

    /*!
     *  \brief         Управление режимом Паника
     *
     *  \param  [in]   method - способ включения
     */
    void setPanic(panic_method_t method);

    /*!
     *  \brief         Управление настройкой "Антиограбление по метке"
     *
     *  \param  [in]   state - вкл/выкл
     *
     *  \return        bool
     *  \retval        true  - успешное изменение настройки
     *  \retval        false - возникла ошибка при изменении настройки
     */
    bool setAhjTagSetting(bool state);

    /*!
     *  \brief         Включить/выключить режим "Опасная парковка"
     *
     *  \param  [in]   state - вкл/выкл
     *
     *  \return        bool
     *  \retval        true  - соответствующее количество импульсов светодиода сервисной кнопки получено
     *  \retval        false - соответствующее количество импульсов светодиода сервисной кнопки не получено
     */
    bool setDangerParking(bool state);

    /*!
     *  \brief         Проверить состояние режима "Опасная парковка" по региону
     *
     *  \param  [in]   state - ожидаемое состояние
     *
     *  \return        bool
     *  \retval        true  - состояние соответствует ожидаемому
     *  \retval        false - состояние не соответствует ожидаемому
     */
    bool checkDangerParking(bool state);

    /*!
     *  \brief         Проверить состояние режима "Опасная парковка" по сигналам автомобиля (серв. кнопка, сирена и световые сигналы)
     *
     *  \param  [in]   state     - проверяемое состояние
     *  \param  [in]   timeout_s - время ожидания сигналов
     *
     *  \return        bool
     *  \retval        true  - все сигналы получены
     *  \retval        false - сигналы несоответствуют ожидаемым
     */
    bool checkDangerParkingSignals(bool state, float timeout_s = TIME_DELAY::MAX_WAIT_CL_SIGNALS_S);

    /*!
     *  \brief         Включить/выключить режим настройки по BLE
     *
     *  \param  [in]   state - вкл/выкл
     *
     *  \return        bool
     *  \retval        true  - соответствующее количество импульсов светодиода сервисной кнопки получено
     *  \retval        false - соответствующее количество импульсов светодиода сервисной кнопки не получено
     */
    bool setBleSettingMode(bool state);

    /*!
     *  \brief         Проверить состояние режима настройки по BLE
     *
     *  \param  [in]   state - ожидаемое состояние (вкл/выкл)
     *
     *  \return        bool
     *  \retval        true  - состояние соответствует ожидаемому
     *  \retval        false - состояние не соответствует ожидаемому
     */
    bool checkBleSettingMode(bool state);

    /*!
     *  \brief         Управление и проверка режима "Опасная парковка"
     *
     *  \param  [in]   state       - желаемое состояние
     *  \param  [in]   wait_time_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - состояние соответствует желаемому
     *  \retval        false - состояние не соответствует желаемому
     */
    bool setAndCheckDangerParking(bool state, float wait_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Проверка начального состояния "Снято с охраны"/"В охране"
     *
     *  \param  [in]   state - вкл/выкл(ON/OFF)
     *  \param  [in]   delay - задержка выполнения      
     *
     *  \return        bool
     *  \retval        true  - снято с охраны
     *  \retval        false - возникла ошибка
     */
    bool checkArmState(bool     state,
                       float    delay          = TIME_DELAY::THREE_S);
    
    /*!
     *  \brief         Ворзврат системы, в состояние, когда ни один датчик не сработал
     */
    void clearSensors();
    
    /*!
     *  \brief         Проверка прихода сообщений по CAN для постановки в штатку
     *
     *  \param  [in]   time - время ожидания
     *
     *  \return        bool
     *  \retval        true  - сообщения верны
     *  \retval        false - сообщения либо не пришли, либо не верны
     */
    bool checkStdOnMessages(float time = TIME_DELAY::FIVE_S);
    
    /*!
     *  \brief         Проверка прихода сообщений по CAN для снятия штатки
     *
     *  \param  [in]   time - время ожидания
     *
     *  \return        bool
     *  \retval        true  - сообщения верны
     *  \retval        false - сообщения либо не пришли, либо не верны
     */
    bool checkStdOffMessages(float time = TIME_DELAY::FIVE_S);
    
    /*!
     *  \brief         Проверка прихода сообщений по CAN для закрытия ЦЗ
     *
     *  \param  [in]   time - время ожидания
     *
     *  \return        bool
     *  \retval        true  - сообщения верны
     *  \retval        false - сообщения либо не пришли, либо не верны
     */
    bool checkLockEnabledMessages(float time = TIME_DELAY::FIVE_S);
    
    /*!
     *  \brief         Проверка прихода сообщений по CAN для открытия ЦЗ
     *
     *  \param  [in]   time - время ожидания
     *
     *  \return        bool
     *  \retval        true  - сообщения верны
     *  \retval        false - сообщения либо не пришли, либо не верны
     */
    bool checkLockDisabledMessages(float time = TIME_DELAY::FIVE_S);

    /*!
     *  \brief         Управление охраной без задержки после выполнения
     *
     *  \param  [in]   value            - значение (ON/OFF)
     *  \param  [in]   execution_method - метод исполнения
     */
    virtual void setSecureNoDelay(int8_t         value,
                                  command_type_t execution_method);
    
    /*!
     *  \brief         Запись состояния входа запрета запуска двигателя
     *
     *  \param  [in]   value            - значение (true/false)
     *  \param  [in]   delay_s          - задержка после исполнения
     */
    void setKillEngine(int8_t value, float delay_s = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Ожидание импульсов света и звука после снятия/постановки с охраны
     *
     *  \param  [in]   light_indication - состояние световой индикации (ON/OFF)
     *  \param  [in]   siren_indication - состояние звуковой индикации (ON/OFF)
     *  \param  [in]   time_s           - время ожидания
     * 
     *  \return        bool
     *  \retval        true             - индикация соответствует своему статусу
     *  \retval        false            - индикация не соответствует своему статусу
     */
    bool waitSirenAndLightSignal(light_status_t light_indication,
                                 siren_status_t siren_indication,
                                 float          time_s);

    /*!
     *  \brief         Ожидать импульсный или потенциальный сигнал света и сразу выйти после получения (напр. для синхронизации после отправки СМС)
     *
     *  \param  [in]   pulse_width_s - длительность световых сигналов (напр. при Паника - 10 с, при Тревога - 30 с)
     */
    bool waitLightsImpulseOrPotentialStart(float pulse_width_s);

    /*!
     *  \brief         Проверить импульсное и потенциальное управление светом
     *
     *  \param  [in]   pulse_width_s            - длительность световых сигналов (напр. при Паника - 10 с, при Тревога - 30 с). При 0 с - берется значение из параметров
     *  \param  [in]   pulse_wait_time_s        - время ожидания появления сигналов
     *  \param  [in]   ligth_pulse_width_s      - длительность импульса   (импульсное управление светом)
     *  \param  [in]   light_pulse_pause_s      - пауза между импульсами  (импульсное управление светом)
     *  \param  [in]   light_potential_len_s    - длительность потенциала (потенциальное управление светом)
     *  \param  [in]   pulse_count              - количество импульсов    (импульсное управление светом)
     *  \param  [in]   pulse_width_error_time_s - погрешность длительности световых сигналов. При 0 с - рассчитывается автоматически
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkLightsImpulseAndPotential(float                pulse_width_s,
                                        float                pulse_wait_time_s,
                                        float                ligth_pulse_width_s,
                                        float                light_pulse_pause_s,
                                        float                light_potential_len_s,
                                        lights_pulse_count_t pulse_count              = LIGHTS_PULSE_COUNT_TWO,
                                        float                pulse_width_error_time_s = TIME_DELAY::NULL_DELAY_S);    

    /*!
     *  \brief         Запись значения температуры в ЦБ
     *
     *  \param  [in]   temperature - значение температуры
     */
    void setTemperatureValue(temperature_celsius_t temperature);

    /*!
     *  \brief         Показываем системе, что человек вышел из машины
     */
    void disableHumanInCar();

    /*!
     *  \brief         Включение метки и отключение состояния системы Human in Car для того, чтобы СР работали после снятия с охраны
     *
     *  \return        bool
     *  \retval        true  - алгоритм выполнен успешно
     *  \retval        false - ошибка при выполнении алгоритма
     */
    bool setTagAndDisableHumanInCar();

    /*!
     *  \brief         Вывести метку из транспортного режима
     */
    void activateTag();

    /*!
     *  \brief         Обновление r6eco через SLM
     *
     */
    void updateR6ecoFw();

    /*!
     *  \brief         Статус капота
     *
     *  \return        Статус
     */
    int8_t getOutHood();

    /*!
     *  \brief         Статус ЦЗ (открыть)
     *
     *  \return        Статус
     */
    int8_t getLockOpen();

    /*!
     *  \brief         Статус ЦЗ (закрыть)
     *
     *  \return        Статус
     */
    int8_t getLockClose();

    /*!
     *  \brief         Сброс по питанию
     */
    void resetGen6Power();

    /*!
     *  \brief         Сбросить состояние авторизации и запрета поездки
     *
     *  \param  [in]   ign_status - статус зажигания после авторизации
     */
    void resetAuthAndImmo(ign_statuses_t ign_status = IGN_OFF);

    /*!
     *  \brief         Регистрация штатных кнопок и брелков
     *
     *  \param  [in]   req_queue_mode - использовать ли очередь на регистрацию (вставать в очередь или регистрироваться без очереди)
     *
     *  \return        bool
     *  \retval        true  - удачно
     *  \retval        false - не удачно
     */
    bool registerBtnAndRemote(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         Проверка состояния выхода статуса метки
     *
     *  \param  [in]   status_out          - номер статусного выхода
     *  \param  [in]   time_s              - время ожидания
     *  \param  [in]   status_out_state    - проверяемое состояние ON/OFF
     *
     *  \return        bool
     *  \retval        true    - состояние выхода статуса метки соответствует ожидаемому
     *  \retval        false   - состояние выхода статуса метки соответствует ожидаемому
     */                          
    bool waitStatusOut(status_out_t status_out,
                       float        time_s,
                       bool         status_out_state);

    /*!
     *  \brief         Влючение/выключение метки и проверка статуса видимости
     *
     *  \param  [in]   tag_mode  - режим работы метки (OFF/ON)
     *
     *  \return        bool
     *  \retval        true      - статус видимости метки соответствует её состоянию
     *  \retval        false     - статус видимости метки не соответствует её состоянию
     */                          
    bool setAndCheckTag(bool tag_mode);

    /*!
     *  \brief         Влючение/выключение охраны и проверка статуса охраны
     *
     *  \param  [in]   secure_mode      - режим охраны  (OFF/ON)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   arm_state        - ожидаемое состояние охраны
     *  \param  [in]   expect_state     - ожидаемое состояние охраны
     *  \param  [in]   check_time_s     - время ожидания статуса охраны
     *
     *  \return        bool
     *  \retval        true             - статус охраны соответствует её состоянию
     *  \retval        false            - статус охраны не соответствует её состоянию
     */                          
    bool setAndCheckSecure(bool                     secure_mode,
                           command_type_t           execution_method,
                           slp_diag_data_statuses_t arm_state,
                           bool                     expect_state     = ON,
                           float                    check_time_s     = TIME_DELAY::MAX_GUARD_STATE_EN_TIME_S);

    /*!
     *  \brief         Управление и проверка зажигания
     *
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   delay_s          - задержка после управления
     *
     *  \return        bool
     *  \retval        true             - статус зажигания соответствует её состоянию
     *  \retval        false            - статус зажигания не соответствует её состоянию
     */
    bool setAndCheckIgn(int8_t         ign_value,
                        command_type_t execution_method,
                        float          delay_s           = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Нажать и проверить педаль тормоза
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay_s          - задержка после управления
     *
     *  \return        bool
     *  \retval        true             - статус тормоза соответствует его состоянию
     *  \retval        false            - статус тормоза не соответствует его состоянию
     */
    bool setAndCheckBreak(int8_t         value,
                          command_type_t execution_method,
                          float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Статус зажигания 1
     *
     *  \return        Статус
     */
    int8_t getIgn1();

    /*!
     *  \brief         Статус выхода метки
     *
     *  \param  [in]   in_tag_status        - пин входа статуса который мы проверяем
     *
     *  \return        Статус
     */
    int8_t checkStatusIn(DigitalIn *in_tag_status);

    /*!
     *  \brief         Ожидание состояния статусного выхода
     *
     *  \param  [in]   in_tag_status       - пин входа статуса который мы проверяем
     *  \param  [in]   gen6_functional_t   - функционал назначенный проводу
     *  \param  [in]   status_out_state    - проверяемое состояние ON/OFF
     *  \param  [in]   waiting_time_s      - время ожидания
     *
     *  \return        bool
     *  \retval        true                - статус статусного выхода соответствует её состоянию
     *  \retval        false               - статус статусного выхода не соответствует её состоянию
     */
    bool waitStateStatusOut(DigitalIn          *in_tag_status,
                            gen6_functional_t   status_out,
                            bool                status_out_state,
                            float               waiting_time_s    = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief        Двойное нажатие кнопки 3 брелка
     *
     *  \param push_type_first  - длительность первого нажатия
     *  \param push_type_second - длительность второго нажатия
     */
    void pushLcdKeyringBut33(float push_type_first, 
                             float push_type_second);

    /*!
     *  \brief         Управление и проверка ручного тормоза
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   hand_break_value    - значение (вкл/выкл)
     *  \param  [in]   delay               - задержка после управления
     *  \param  [in]   waiting_time_s      - время ожидания статуса в регионе
     *
     *  \return        bool
     *  \retval        true                - статус ручника соответствует его состоянию
     *  \retval        false               - статус ручника не соответствует его состоянию
     */
    bool setAndCheckHandBreak(int8_t         hand_break_value,
                              command_type_t execution_method,
                              float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                              float          waiting_time_s    = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Управление и проверка ручного тормоза по CAN
     *
     *  \param  [in]   hand_break_value    - значение (вкл/выкл)
     *  \param  [in]   delay               - задержка после управления
     *
     *  \return        bool
     *  \retval        true                - статус ручника соответствует его состоянию
     *  \retval        false               - статус ручника не соответствует его состоянию
     */
    bool setAndCheckHandBreakCan(int8_t hand_break_value,
                                 float  delay             = TIME_DELAY::ONE_S);

    /*!
     *  \brief         Управление и проверка паркинга
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   hand_break_value    - значение (вкл/выкл)
     *  \param  [in]   delay               - задержка после управления
     *  \param  [in]   waiting_time_s      - время ожидания статуса в регионе
     *
     *  \return        bool
     *  \retval        true                - статус паркинга соответствует её состоянию
     *  \retval        false               - статус паркинга не соответствует её состоянию
     */
    bool setAndCheckParking(int8_t         parking_value,
                            command_type_t execution_method,
                            float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                            float          waiting_time_s    = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Управление и проверка паркинга по CAN
     *
     *  \param  [in]   hand_break_value    - значение (вкл/выкл)
     *  \param  [in]   delay               - задержка после управления
     *
     *  \return        bool
     *  \retval        true                - статус паркинга соответствует его состоянию
     *  \retval        false               - статус паркинга не соответствует его состоянию
     */
    bool setAndCheckParkingCan(int8_t parking_value,
                               float  delay_s        = TIME_DELAY::ONE_S);

    /*!
     *  \brief         Управление и проверка заведённого двигателя
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   hand_break_value    - значение (вкл/выкл)
     *  \param  [in]   delay_s             - задержка после управления
     *
     *  \return        bool
     *  \retval        true                - статус двигателя соответствует его состоянию
     *  \retval        false               - статус двигателя не соответствует его состоянию
     */
    bool setAndCheckEngine(int8_t         engine_value,
                           command_type_t execution_method,
                           int16_t        rpm_val           = RPM_ON,
                           float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Подготовка автомобиля к антиограблению по метке
     *
     *  \param  [in]   secure_method       - метод включения охраны
     *  \param  [in]   ign_method          - метод включения зажигания
     *  \param  [in]   engine_method       - метод запуска двигателя
     *  \param  [in]   hand_break_method   - метод выключения ручника
     *  \param  [in]   parking_method      - метод выключения паркинга
     *
     *  \return        bool
     *  \retval        true                - устройство готово к отработке алгоритма 
     *                                       антиограбления по метке
     *  \retval        false               - антиограбление по метке невозможно
     */
    bool prepAhjTag(command_type_t secure_method     = TAG,
                    command_type_t ign_method        = PIN,
                    command_type_t engine_method     = PIN,
                    command_type_t hand_break_method = PIN,
                    command_type_t parking_method    = PIN);

    /*!
     *  \brief         Выполнение действий приводящих к антиограблению (если есть метка - по метке, если нет - по брелку)
     *
     *  \param  [in]   door_method    - метод открытия двери
     *
     *  \return        bool
     *  \retval        true           - действия для антиограбления по метке выполнены
     *  \retval        false          - действия для антиограбления по метке не выполнены
     */
    bool doAhj(command_type_t door_method = PIN);

    /*!
     *  \brief         Ожидание кан пакетов блокировки двигателя (151) с очисткой буфера FiFo
     *
     *  \param  [in]   time_s         - время ожидания
     *
     *  \return        bool
     *  \retval        true           - пакеты блокирвки получены
     *  \retval        false          - пакеты блокировки не получены
     */
    bool waitEngineBlockCan(float time_s);

    /*!
     *  \brief         Включение датчиков удара\движения\наклона с регулируемой задержкой
     *
     *  \param  [in]   sensor_type     - название датчика
     *  \param  [in]   delay_s         - время ожидания
     *
     */
    void setAccelerometerAfterDelay(sensor_type_t sensor_type,
                                    float         delay_s     = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Управление и проверка багажника
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   trunk_value         - значение (вкл/выкл)
     *  \param  [in]   logic_state         - логика работы (инверсная/нормальная)
     *  \param  [in]   delay_s             - задержка после управления
     *
     *  \return        bool
     *  \retval        true                - статус багажника соответствует его состоянию
     *  \retval        false               - статус багажника не соответствует его состоянию
     */
    bool setAndCheckTrunk(int8_t           trunk_value,
                          command_type_t   execution_method,
                          invers_logic_t   logic_state      = NORMAL,
                          float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Проверить состояние багажника
     *
     *  \param  [in]   status           - ожидаемое состояние
     *  \param  [in]   execution_method - через что проверять (SLP, CAN и т.д.)
     *  \param  [in]   timeout_s        - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true  - статус багажника соответствует ожидаемому состоянию
     *  \retval        false - статус багажника не соответствует ожидаемому состоянию
     */
    bool checkTrunk(bool           status,
                    command_type_t execution_method = SLP,
                    float          timeout_s        = TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    /*!
     *  \brief         Проверка блокировки АКПП по CAN
     *
     *  \param  [in]   timeout_s - время ожидания блокировки
     *
     *  \return        bool
     *  \retval        true  - команда блокировки получена
     *  \retval        false - команда блокировки не получена
     */
    bool waitGearboxAutoBlock(float timeout_s = TIME_DELAY::GEARBOX_AUTO_BLOCK_TIME_S);

    /*!
     *  \brief         Проверка разблокировки КПП (АКПП) по CAN
     *
     *  \param  [in]   timeout_s - время ожидания разблокировки
     *
     *  \return        bool
     *  \retval        true  - команда разблокировки получена
     *  \retval        false - команда разблокировки неполучена
     */
    bool waitGearboxAutoUnblock(float timeout_s = TIME_DELAY::GEARBOX_AUTO_BLOCK_TIME_S);

    /*!
     *  \brief         Управление и проверка двери
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   door_value          - значение (вкл/выкл)
     *  \param  [in]   logic_state         - логика работы (инверсная/нормальная)
     *  \param  [in]   delay_s             - задержка после управления
     *  \param  [in]   wait_time_s         - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true                - статус багажника соответствует его состоянию
     *  \retval        false               - статус багажника не соответствует его состоянию
     */
    bool setAndCheckDoor(int8_t           door_value,
                         command_type_t   execution_method,
                         invers_logic_t   logic_state      = NORMAL,
                         float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                         float            wait_time_s      = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Управление и проверка капотом
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   hood_value          - значение (вкл/выкл)
     *  \param  [in]   logic_state         - логика работы (инверсная/нормальная)
     *  \param  [in]   delay_s             - задержка после управления
     *
     *  \return        bool
     *  \retval        true                - статус багажника соответствует его состоянию
     *  \retval        false               - статус багажника не соответствует его состоянию
     */
    bool setAndCheckHood(int8_t           hood_value,
                         command_type_t   execution_method,
                         invers_logic_t   logic_state      = NORMAL,
                         float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Ожидание состояния статусного выхода
     *
     *  \param  [in]   Buzzer_state        - проверяемое состояние ON/OFF
     *  \param  [in]   waiting_time_s      - время ожидания
     *
     *  \return        bool
     *  \retval        true                - статус статусного выхода соответствует её состоянию
     *  \retval        false               - статус статусного выхода не соответствует её состоянию
     */
    bool waitBuzzerExt(bool  buzzer_state,
                       float waiting_time_s = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление дверью инвертированное
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   delay_s          - задержка после управления 
     */
    virtual void setDoorInvPin(int8_t value,
                               float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление капотом инвертированное
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   delay_s          - задержка после управления 
     */
    virtual void setHoodInvPin(int8_t value,
                               float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление багажником инвертированное
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   delay_s          - задержка после управления 
     */
    virtual void setTrunkInvPin(int8_t value,
                                float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление входом запрета запуска инвертированное
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   delay_s          - задержка после управления 
     */
    virtual void setKillEngineInv(int8_t value,
                                  float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление дверью водителя
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay_s          - задержка после выполнения
     */
    virtual void setDriverDoor(int8_t         value,
                               command_type_t execution_method,
                               float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление дверью пассажира
     *
     *  \param  [in]   value            - значение (вкл/выкл)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   delay_s          - задержка после выполнения
     */
    virtual void setPassengerDoor(int8_t         value,
                                  command_type_t execution_method,
                                  float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление и проверка двери водителя или пассажира
     *
     *  \param  [in]   execution_method    - метод исполнения
     *  \param  [in]   door_value          - значение (вкл/выкл)
     *  \param  [in]   door_name           - название двери которой управляем, проверяем
     *  \param  [in]   delay_s             - задержка после управления
     *  \param  [in]   wait_time_s         - время ожидания статуса в регионе
     *
     *  \return        bool
     *  \retval        true                - статус двери соответствует её состоянию
     *  \retval        false               - статус двери не соответствует её состоянию
     */
    bool setAndCheckSpecialDoor(int8_t           door_value,
                                command_type_t   execution_method,
                                special_door_t   door_name,
                                float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                                float            wait_time_s      = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Ожидание статуса турботаймера в регионе
     *
     *  \param  [in]   tt_value            - состояние ТТ
     *  \param  [in]   wait_time_s         - время ожидания статуса в регионе
     *
     *  \return        bool
     *  \retval        true                - статус TT соответствует её состоянию
     *  \retval        false               - статус TT не соответствует её состоянию
     */
    bool waitTurboTimer(bool  tt_value,
                        float wait_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Отправить по CAN пакет о штатной тревоге
     *
     *  \param  [in]   state - состояние тревоги: вкл/выкл
     */
    void setStdAlertMercedesCla(bool state);

    /*!
     *  \brief         Управление зажигание на Renault Captur (CAN)
     *
     *  \param  [in]   status  - состояние зажигания
     *  \param  [in]   delay_s - задержка после управления
     */
    void setIgnRenaultCaptur(bool status, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление оборотами на Renault Captur (CAN)
     *
     *  \param  [in]   value   - значение оборотов
     *  \param  [in]   delay_s - задержка после управления
     */
    void setRpmRenaultCaptur(rpm_statuses_t value, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Подать команду на дистанционный запуск на Renault Captur (CAN)
     *
     *  \param  [in]   delay_s - задержка после команды
     */
    void setRemoteRunRenaultCaptur(float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Ожидание клаксона Lexus RX (кнопка Start-Stop) 2016-2018
     *
     *  \param  [in]   timeout_s - время ожидания
     */
    bool waitKlaxonLexusRx(float timeout_s = TIME_DELAY::ALARM_EN_TIME_S);

    /*!
     *  \brief         Проверка алгоритма переода в охрану с работающим двигателем
     *
     *  \param  [in]   arm_supp_trig    - Условие включения поддержки зажигания для перехода в охрану с работающим двигателем
     *  \param  [in]   execution_method - Метод постановки в охрану
     *
     *  \return        bool
     *  \retval        true             - алгоритм перехода в охрану с работающим двигателем выполнен
     *  \retval        false            - алгоритм перехода в охрану с работающим двигателем не выполнен
     */
    bool setArmSuppIgn(arm_supp_trig_t arm_supp_trig,
                       command_type_t  execution_method = KEYRING);

    /*!
     *  \brief         Проверка изменения состояния охраны после срабатывания одного из датчиков
     *
     *  \param  [in]   sensor_type - Имя датчика
     *  \param  [in]   arm_state   - Ожидаемое состояние охраны
     *  \param  [in]   delay_s     - задержка перед активацией датчика
     *
     *  \return        bool
     *  \retval        true        - Состояние охраны изменилось согласно алгоритму после срабатывания датчика
     *  \retval        false       - Состояние охраны не изменилось согласно алгоритму после срабатывания датчика
     */
    bool checkAlertStateAfterAccel(sensor_type_t            sensor_type,
                                   slp_diag_data_statuses_t arm_state,
                                   float                    delay_s);

    /*!
     *  \brief         Закрыть все нарушенные зоны охраны по SLP
     *
     *  \return        bool
     *  \retval        true  - успешная запись в регион
     *  \retval        false - запись не успешна
     */
    bool resetDiagDataPerim();

    /*!
     *  \brief         Управление выходом свечей накала
     *
     *  \param  [in]   status  - желаемое состояние (вкл. / выкл.)
     *  \param  [in]   delay_s - задержка после управления
     */
    void setGlowPlugs(out_statuses_t status, float delay_s = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление выходом свечей накала и проверка статуса по SLP
     *
     *  \param  [in]   status  - желаемое состояние (вкл. / выкл.)
     *  \param  [in]   delay_s - задержка после управления
     */
    bool setAndCheckGlowPlugs(out_statuses_t status, float timeout_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Проверить, соответствует ли значение настройки тому, что загружено в ЦБ
     *
     *  \param  [in]   setting_name  - имя настройки (id из xml, напр.: outs_ctrl_doors_lock_duration_ms)
     *  \param  [in]   setting_value - ожидаемое значение настройки
     *  \param  [in]   setting_type  - тип настройки (целочисленная или строковая)
     *
     *  \return        bool
     *  \retval        true  - значение настройки соответствует ожидаемому
     *  \retval        false - значение настройки не соответствует ожидаемому
     */
    bool checkSettingValue(const char     *setting_name,
                           const char     *setting_value,
                           setting_type_t  setting_type);

    /*!
     *  \brief         Ожидание разложения/сложения зеркал (пакеты CAN)
     *
     *  \param  [in]   mirrors_status - ожидаемый статус (открыть/закрыть зеркала)
     *  \param  [in]   timeout_s      - время ожидания
     *
     *  \return        bool
     *  \retval        true  - пакеты получены
     *  \retval        false - пакеты не получены
     */
    bool waitSideMirrorsCan(mirrors_statuses_t mirrors_status, float timeout_s = TIME_DELAY::COMFORT_EN_TIME_S);

    /*!
     *  \brief         Ожидание включения/выключения функции Комфорт (CAN)
     *
     *  \param  [in]   status    - ожидаемый статус (вкл./выкл.)
     *  \param  [in]   timeout_s - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true  - пакеты получены
     *  \retval        false - пакеты не получены
     */
     bool waitComfortCan(out_statuses_t status, float timeout_s = TIME_DELAY::COMFORT_EN_TIME_S);

    /*!
     *  \brief         Ожидание открытия/закрытия люка на крыше для автомобиля Lexus GX 2019-2020
     *
     *  \param  [in]   status    - ожидаемый статус (вкл./выкл.)
     *  \param  [in]   timeout_s - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true  - пакеты получены
     *  \retval        false - пакеты не получены
     */
     bool waitSunroofCanLexusGx(doors_statuses_t sunroof_status, float timeout_s = TIME_DELAY::COMFORT_EN_TIME_S);

    /*!
     *  \brief         Ожидание пакетов обходчика CAN
     *
     *  \param  [in]   timeout_s - время ожидания пакетов
     *
     *  \return        bool
     *  \retval        true  - пакеты получены
     *  \retval        false - пакеты не получены
     */
    bool waitInspectorCan(float timeout_s);

    /*!
     *  \brief         Ожидание сообщений запросов OBD ошибок
     *
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - пакеты получены
     *  \retval        false - пакеты не получены
     */
    bool waitObdErrorRequest(float timeout_s);

    /*!
     *  \brief         Установить связь мобильного приложения с основным блоком
     *
     *  \return        bool
     *  \retval        true  - связь установлена
     *  \retval        false - связь не установлена
     */
    bool connectMobile();

    /*!
     *  \brief         Разорвать связь мобильного приложения с основным блоком
     *
     *  \return        bool
     *  \retval        true  - связь разорвана
     *  \retval        false - связь не разорвана
     */
    bool disconnectMobile();

    /*!
     *  \brief         Очистить буфер сообщений донгла
     *
     *  \return        bool
     *  \retval        true  - очистка успешна
     *  \retval        false - очистка неуспешна
     */
    bool clearMobileBuf();

    /*!
     *  \brief         Прочитать текущее состояние системы через мобильное приложение (BLE) (из диагностического пакета MOBILEAPP_DIAG_PACKAGE_BLE_CUR_STATE)
     *
     *  \param  [in]   shift     - смещение для диагностического пакета
     *  \param  [in]   status    - ожидаемый статус (см. соответствующие статусы в SlpCmd.h)
     *  \param  [in]   read_type - тип чтения диагностического пакета (отправить запрос на получение пакета или прочитать данные, сохраненные в буфере донгла)
     * 
     *  \return        bool
     *  \retval        true  - ожидаемый статус получен
     *  \retval        false - ожидаемый статус не получен
     */
    bool getMobileStatusCurState(mobileapp_cur_state_shift_t shift,
                                 int32_t                     status,
                                 mobileapp_read_type_t       read_type = MOBILEAPP_READ_FROM_CMD);

    /*!
     *  \brief        Отправить команду основному блоку через мобильное приложение (BLE)
     *
     *  \param  [in]  command - пользовательская команда для управления
     *
     *  \return       bool
     *  \retval       true  - команда выполнена
     *  \retval       false - команда не выполнена
     */
    bool setMobileStatus(mobileapp_user_cmd_t command);

    /*!
     *  \brief        Управлять сервисным режимом через мобильное приложение (BLE)
     *
     *  \param  [in]  status - желаемый статус (вкл./выкл.)
     *
     *  \return       bool
     *  \retval       true  - команда выполнена
     *  \retval       false - команда не выполнена
     */
    bool setServiceModeMobile(out_statuses_t status);

    /*!
     *  \brief         Выполнить алгоритм входа в режим ввода кода экстр. снятия через альтернативную сервисную кнопку
     *
     *  \param  [in]   alt_srv_btn_type - тип альтернативной сервисной кнопки
     */
    void setAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         Включить и проверить охрану через альтернативную сервисную кнопку
     *
     *  \param  [in]   alt_srv_btn_type - тип альтернативной сервисной кнопки
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setAndCheckAltSrvBtnArm(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         Включить и проверить режим альтернативной сервисной кнопки
     *
     *  \description   Не работает для типа Штатные кнопки, т.к. алгоритм различается
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setAndCheckAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         Нажатие альтернативной сервисной кнопки
     *
     *  \param  [in]   alt_srv_btn_type - тип альтернативной сервисной кнопки
     *  \param  [in]   count            - количество нажатий
     *
     *  \return        bool
     *  \retval        true  - нажатие успешно
     *  \retval        false - нажатие неуспешно
     */
    bool clickAltSrvBtn(alt_srv_btn_type_t alt_srv_btn_type,
                        uint8_t            count);

    /*!
     *  \brief         Экстренное снятие с охраны в режиме альтернативной сервисной кнопки
     *
     *  \detailed      Система уже должна находиться в режиме альтернативной сервисной кнопки
     *                 Метод сам определяет, верно ли вводится код или нет и на основе этого проверяются световые сигналы
     *                 Если введен верный код - все световые сигналы должны быть получены
     *                 Если введен неверный код - получены световые сигналы от трех первых цифр и отсутствует у последней
     *
     *  \param  [in]   alt_srv_btn_type - тип альтернативной сервисной кнопки
     *  \param  [in]  *pin              - код экстренного снятия
     *
     *  \return        bool
     *  \retval        true  - все сигналы света получены
     *  \retval        false - не все сигналы света получены
     */
    bool setAltSrvBtnDisarm(alt_srv_btn_type_t  alt_srv_btn_type,
                            const char         *pin               = NULL);

    /*!
     *  \brief         Получение полярности выхода с функционалом \"Выход блокировки рулевого вала\"
     *
     *  \detailed      Автоматически определяет настроенный функционал (для устройств V1 или V2)
     *
     *  \return        полярность
     */
    bool getSlpPolarity();

    /*!
     *  \brief         Проверить задержку отключения обходчика после выкл. аксессуаров
     *
     *  \detailed      Метод нужен для точных измерений задержки выкл. обходчика (меньше секунды)
     *                 Метод должен применяться перед выключением аксессуаров (т.к. отсчет будет идти относительно этого времени)
     *
     *  \param  [in]   immo_delay_s - ожидаемое время задержки выкл. обходчика после откл. аксессуаров
     *  \param  [in]   err_time_s   - погрешность
     *  \param  [in]   timeout_s    - время проверки
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkImmoDisDelayAfterAcc(float immo_delay_s,
                                   float err_time_s,
                                   float timeout_s);

    /*!
     *  \brief         Открыть регион для отладки
     *
     *  \detailed      Открыть регион заранее при выполнении команд на чтение или запись в регион
     *                 После этого потребуется ручное закрытие
     *
     *  \param  [in]   slp_open_type - причина открытия региона (чтение/запись)
     *
     *  \return        bool
     *  \retval        true  - успешное открытие
     *  \retval        false - неуспешное открытие
     */
    bool openDebugRegion(slp_open_type_t slp_open_type = SLP_OPEN_TYPE_RW);

    /*!
     *  \brief         Открыть регион для чтения событий
     *
     *  \detailed      Нужно открывать до выполнения действия, которое вызывает событие
     *                 Если открыть регион после выполнения действия - то событие по нему может быть пропущено
     *
     *  \return        bool
     *  \retval        true  - успешное открытие
     *  \retval        false - неуспешное открытие
     */
    bool openEventsRegion();

    /*!
     *  \brief         Закрыть регион
     *
     *  \return        bool
     *  \retval        true  - успешное закрытие
     *  \retval        false - неуспешное закрытие
     */
    bool closeRegion();

    /*!
     *  \brief         Ожидание наступления событий
     *
     *  \param  [in]   timeout_s - время ожидания наступления событий
     *  \param  [in]  &event_ids - коды ожидаемых событий
     *
     *  \return        bool
     *  \retval        true  - одно из событий наступило
     *  \retval        false - ни одно из событий не наступило
     */
    bool waitSlpEvents(const std::vector<slp_user_event_t> &event_ids, float timeout_s);

    /*!
     *  \brief         Управление выходом датчик демонтажа
     *
     *  \param  [in]   value            - желаемое состояние
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   logic_type       - тип логики работы датчика (обычная/инверсная)
     *  \param  [in]   delay_s          - задержка после выполнения
     */
    void setDismantlingSensor(out_statuses_t value,
                              command_type_t execution_method,
                              invers_logic_t logic_type        = NORMAL,
                              float          delay_s           = TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    /*!
     *  \brief         Управление выходом датчик демонтажа и проверка его состояния (по SLP)
     *
     *  \param  [in]   value            - желаемое состояние
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   logic_type       - тип логики работы датчика (обычная/инверсная)
     *  \param  [in]   wait_time_s      - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setAndCheckDismantlingSensor(out_statuses_t value,
                                      command_type_t execution_method,
                                      invers_logic_t logic_type        = NORMAL,
                                      float          wait_time_s       = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Проверить одновременную работу сервисного светодиода и сирены
     *
     *  \param  [in]   pulses_count - ожидаемое кол-во импульсов
     *  \param  [in]   timeout_s    - время проверки
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkSrvLedAndSiren(uint8_t pulses_count, float timeout_s);

    /*!
     *  \brief         Управление внешней кнопкой CAN
     *
     *  \param  [in]   value   - желаемое состояние
     *  \param  [in]   delay_s - задержка после управления
     */
    void setCanExtBtnAudiA7(out_statuses_t value, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);


    /*!
     *  \brief         Управление внешней кнопкой CAN и проверка ее состояния (по SLP)
     *
     *  \param  [in]   value       - желаемое состояние
     *  \param  [in]   wait_time_s - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    void setAndCheckCanExtBtnAudiA7(out_statuses_t value, float wait_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Ожидание статуса работы двигателя
     *
     *  \param  [in]   status    - ожидаемый статус (двигатель работает/заглушен)
     *  \param  [in]   timeout_s - время ожидания статуса
     *
     *  \return        bool
     *  \retval        true  - ожидаемый статус получен
     *  \retval        false - ожидаемый статус не получен
     */
    bool waitEngineStatus(out_statuses_t status, float timeout_s = TIME_DELAY::WAITING_REGION_STATE_S);

    /*!
     *  \brief         Задать настройки через мобильное приложение (BLE)
     *
     *  \param  [in]  &settings - ссылка на задаваемые настройки
     *
     *  \return        bool
     *  \retval        true  - настройки заданы
     *  \retval        false - настройки не заданы
     */
    bool setSettingsMobile(const SlmSettings &settings);

    /*!
     *  \brief         Прочитать и сравнить настройки через мобильное приложение (BLE)
     *
     *  \param  [in]  &settings - ссылка на проверяемые настройки
     *
     *  \return        bool
     *  \retval        true  - настройки соответствуют ожидаемым
     *  \retval        false - настройки не соответствуют ожидаемым
     */
    bool checkSettingsMobile(const SlmSettings &settings);

};

#endif // _GEN6_BASIC_H_
