/*!
 *  \file      Gen6Basic.h
 *
 *  \brief     �������� ������� ����� ��� 6-�� ��������� �������� ������
 *
 *  \detailed  �������� ����������� ���������� ��� �������, ����������� �� ����
 *
 */

#ifndef _GEN6_BASIC_H_
#define _GEN6_BASIC_H_

//! ����������� ������������ ������

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


//! ���� � ������

/*!
 *  \brief    ��������� ���������� �������� �����
 */
typedef enum
{
    TAG_VOLTAGE_CTRL_DIS, //!< �� ��������� �������� �����
    TAG_VOLTAGE_CTRL_EN   //!< ��������� �������� �����
} tag_voltage_ctrl_t;

/*!
 *  \brief    ����� ���� (INFO/FAIL/PASS)
 */
typedef enum
{
    LOG_LABEL_INFO,      //!< INFO ���
    LOG_LABEL_FAIL,      //!< FAIL ���
    LOG_LABEL_PASS       //!< PASS ���
} log_label_t;

/*!
 *  \brief    ��������� ������ ��� �������� �������� � �������
 */
struct FW_LOAD_ERROR
{
    static const char *FW_LOAD_OK;              //!< ��� ������
    static const char *FW_LOAD_ERROR_SL_MASTER; //!< ������ ��� ���������� ����� StarLine ������
    static const char *FW_LOAD_ERROR_NOT_FOUND; //!< ������������� �������� �� ������� �� �������
    static const char *FW_LOAD_ERROR_ARG;       //!< ������������ ���������� ��� �������� ��������
};

/*!
 *  \brief    ���� �������� ��� ���������� � �������
 */
typedef enum
{
    FW_DOWNLOAD_TYPE_OLDEST,        //!< ����� ������ ������ �� ����������� �� �������
    FW_DOWNLOAD_TYPE_SECOND_PREV,   //!< ������ ���������� ������ ��
    FW_DOWNLOAD_TYPE_PREV,          //!< ���������� ������ ��
    FW_DOWNLOAD_TYPE_LATEST_BETA,   //!< ��������� ���� ������ �� ����������� �� �������
    FW_DOWNLOAD_TYPE_LATEST_RELEASE //!< ��������� �������� ������ �� ����������� �� �������
} fw_download_type_t;

/*!
 *  \brief    ��������� ���� �������� � ��
 */
typedef enum
{
    SETTING_TYPE_INT, //!< ������������� ��� ��������� (����. ������������ �������� ��)
    SETTING_TYPE_STR  //!< ��������� ��� ��������� (����. ����� M1)
} setting_type_t;

/*!
 *  \brief    �������� �� �� ������� �� ����������� ��� ���������� ���������
 */
typedef enum
{
    IGN_OFF_REG_QUEUE_NO_LEAVE, //!< �� ��������
    IGN_OFF_REG_QUEUE_LEAVE     //!< ��������
} reg_queue_leave_type_t;

/*!
 *  \brief    ����� Gen6Basic
 *
 *  \detailed �������� ���������� ��� �������, ����������� �� ����
 */
class Gen6Basic : public DevicesBasic
{
//! \privatesection
private:

    //! ����� ����������� ������
    typedef bool (Gen6Basic::*reg_remote_func_t)();

    /*!
     *  \brief         ��������� ����� ����
     *
     *  \param  [in]   digitalIn       - ��������� �� ������ DigitalIn
     *
     *  \return        PinName         - ��� ����
     */
    PinName getPinNameIn(DigitalIn *digitalIn);

    /*!
     *  \brief         ����������� ������ � ��
     *
     *  \param  [in]  *obj  - ��������� �� ������ ������ Gen6Basic
     *  \param  [in]  *func - ��������� �� ����� �����������
     */
    void registerRemote(Gen6Basic *obj, reg_remote_func_t func);

//! \protectedsection
protected:

    R6                   *p_r6;               //!< ��������� �� R6
    R6Eco                *p_r6eco;            //!< ��������� �� R6Eco
    R4                   *p_r4;               //!< ��������� �� R4
    LedKeyring           *p_led_keyring;      //!< ��������� �� LedKeyring
    LcdKeyring           *p_lcd_keyring;      //!< ��������� �� LcdKeyring
    SmartTag             *p_tag;              //!< ��������� �� SmartTag
    ServiceButton        *p_service_btn;      //!< ��������� �� ServiceButton
    Sim900               *p_gsm;              //!< ��������� �� Sim900
    CanBusActions        *p_can_actions;      //!< ��������� �� CanBusActions
    StarlineOnline       *p_st_online;        //!< ��������� �� StarlineOnline
    SlpCmd               *p_slp;              //!< ��������� �� SlpCmd
    Serial               *p_serial;           //!< ��������� �� Serial
    AnalogOut            *p_temp_sensor;      //!< ��������� �� ��� ��� ������������ ���������� �� ���� ������� ����������� 
    MobileApp            *p_mobileapp;        //!< ��������� �� MobileApp
    Validator            *p_validator;        //!< ��������� �� ���������

    vector<const char *>  m_gen6_phones;      //!< ��������

    int32_t               m_device_modules;      //!< ������������ ������
    char                  m_device_pin[5];       //!< ��� ����������� ������ � ������
    char                  m_wrong_device_pin[5]; //!< �������� ��� ����������� ������ � ������

//! \publicsection
public:
    
    /*!
     *  \brief         ����������� ������
     */
    Gen6Basic() {};

    /*!
     *  \brief         ���������� ������
     */
    ~Gen6Basic();

    /*!
     *  \brief         ����������� �����������
     *
     *  \param  [in]  &right - ������ �� Gen6Basic
     */
    Gen6Basic(const Gen6Basic &right);

    /*!
     *  \brief         �������� =
     *
     *  \param  [in]  &right - ������ �� Gen6Basic
     */
    Gen6Basic &operator=(const Gen6Basic &right);
    
    /*!
     *  \brief         ������������� ������������ ���� ACK-���
     */ 
    void initSigmaEnable();
    
    /*!
     *  \brief         ������� ACK-��� � ����� ������ ��������
     */ 
    void sigmaSettingMode();

    /*!
     *  \brief         ������� ACK-��� � ����� ������
     */ 
    void sigmaWorkMode();

    /*!
     *  \brief         �������� ��������� ������� ��������� �����
     *
     *  \return        bool  - ��������� �������
     *  \retval        true  - ������� ��������
     *  \retval        false - ������� ���������
     */
    bool getVcc();

    /*!
     *  \brief         �������� �������
     */ 
    void on();
    
    /*!
     *  \brief         ��������� �������
     */ 
    void off();
    
    /*!
     *  \brief         ���������� ��������� �� AnalogOut
     *
     *  \param  [in]  *temp_sensor - ��������� �� AnalogOut
     */
    void addTempSensor(AnalogOut *temp_sensor);

    /*!
     *  \brief         ���������� ��������� �� LedKeyring
     *
     *  \param  [in]  *ledkeyring - ��������� �� LedKeyring
     */
    void addKeyring(LedKeyring *ledkeyring);

    /*!
     *  \brief         ���������� ��������� �� LcdKeyring
     *
     *  \param  [in]  *lcdkeyring - ��������� �� LcdKeyring
     */
    void addKeyring(LcdKeyring *lcdkeyring);

    /*!
     *  \brief         ���������� ��������� �� SmartTag
     *
     *  \param  [in]  *tag - ��������� �� SmartTag
     */
    void addTag(SmartTag *tag);

    /*!
     *  \brief         ���������� ��������� �� R6
     *
     *  \param  [in]  *r6 - ��������� �� R6
     */
    void addR6(R6 *r6);

    /*!
     *  \brief         ���������� ��������� R6Eco
     *
     *  \param  [in]  *r6eco - ��������� �� R6Eco
     */
    void addR6Eco(R6Eco *r6eco);

    /*!
     *  \brief         ���������� ��������� �� R4
     *
     *  \param  [in]  *r4 - ��������� �� R4
     */
    void addR4(R4 *r4);

    /*!
     *  \brief         ���������� ��������� �� Sim900
     *
     *  \param  [in]  *sim900 - ��������� �� Sim900
     */
    void addGsm(Sim900 *sim900);

    /*!
     *  \brief         ���������� ��������� �� CanBusActions
     *
     *  \param  [in]  *can_actions - ��������� �� CanBusActions
     */
    void addCan(CanBusActions *can_actions);

    /*!
     *  \brief         ���������� ��������� �� CAN 1
     *
     *  \return        ��������� �� CanBus
     */
    CanBus* getCan1();

    /*!
     *  \brief         ���������� ��������� �� CanBusActions
     *
     *  \return        ��������� �� CanBusActions
     */
    CanBusActions* getCanBusActions();

    /*!
     *  \brief         ���������� ��������� �� ServiceButton
     *
     *  \param  [in]  *btn - ��������� �� ServiceButton
     */
    void addServiceButton(ServiceButton *btn);

    /*!
     *  \brief         ���������� ��������� �� StarlineOnline
     *
     *  \param  [in]  *st_online - ��������� �� StarlineOnline
     */
    void addStarlineOnline(StarlineOnline *st_online);

    /*!
     *  \brief         ���������� ��������� �� SlpCmd
     *
     *  \param  [in]  *slp - ��������� �� SlpCmd
     */
    void addSlpCmd(SlpCmd *slp);

    /*!
     *  \brief         ���������� ��������� �� Serial
     *
     *  \param  [in]  *serial - ��������� �� Serial
     */
    void addSerial(Serial *serial);

    /*!
     *  \brief         ���������� ����� ����������
     *
     *  \param  [in]   device_name - ��� ����������
     */
    void addDeviceName(devices_t device_name);

    /*!
     *  \brief         ��������� ����� ����������
     *
     *  \return        devices_t - ��� ����������
     */
    devices_t getDeviceName();
    
    /*!
     *  \brief         ���������� �������
     *
     *  \param  [in]   modules - ������ ����������
     */
    void addModules(int32_t modules);

    /*!
     *  \brief         ���������� �������� ����������
     *
     *  \param  [in]  *device_phone - ����� ��������
     */
    void addDevicePhone(const char *device_phone);

    /*!
     *  \brief         ���������� ��������� �� MobileApp
     *
     *  \param  [in]  *app - ��������� �� MobileApp
     */
    void addMobileApp(MobileApp *app);

    /*!
     *  \brief         ���������� ��������� �� Validator
     *
     *  \param  [in]  *validator - ��������� �� Validator
     */
    void addValidator(Validator *validator);

    /*!
     *  \brief         �������� ��������� ��������� ����������
     *
     *  \return        ������, ���������� ������ ����������
     */
    vector<const char *> &getDevicePhones();

    /*!
     *  \brief         ����������� �� SL-online
     *
     *  \return        1 - �������, 0 - ���
     */
    bool authStarlineOnline();

    /*!
     *  \brief         ���������� ����������
     *
     *  \param  [in]  *tel         - ����� ��������
     *  \param  [in]  *name        - ��� ����������
     *  \param  [in]  *device_type - ��� ���������� (gps-alarm)
     *
     *  \return        1 - �������, 0 - ���
     */
    bool addStarlineOnlineDevice(const char *tel,
                                 const char *name,
                                 const char *device_type = "gps-alarm");

    /*!
     *  \brief         ���������� �������������
     *
     *  \param  [in]   command_type - ��� ������� ����������
     *  \param  [in]   value        - ��������
     *  \param  [in]  *pin          - ��� ����������� ������
     *
     *  \return        1 - �������, 0 - ���
     */
    bool setStarlineOnlineParam(StarlineOnline::command_type_t command_type, bool value, char *pin = NULL);

    /*!
     *  \brief         �������� SMS (��� ��������)
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        ������ (X96_NOSMS, X96_TRUESMS)
     */
    sms_state_t outputSms(float timeout_s = Sim900::s_wait_time_s);

    /*!
     *  \brief         �������� SMS
     *
     *  \param  [in]  *checkmsg - ����������� ���������
     *
     *  \return        ������ (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t waitSms(const char *checkmsg);

    /*!
     *  \brief         �������� SMS � ��������� ������� ��������
     *
     *  \param  [in]  *checkmsg - ����������� ��������� � �������
     *  \param  [in]   time     - ����� ��������
     *
     *  \return        ������ (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t waitSmsFor(const char *checkmsg, float time);

    /*!
     *  \brief         �������� ��� � �������� ������
     *
     *  \param  [in]  *to       - ����� ��������
     *  \param  [in]  *message  - ���������� ��������� � �������
     *  \param  [in]  *checkmsg - ����������� ��������� � �������
     *
     *  \return        ������ (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t inspectionSms(const char *to,
                              const char *message,
                              const char *checkmsg);

    /*!
     *  \brief         �������� ��� � �������� ������ � ��������� ������� ��������
     *
     *  \param  [in]  *to       - ����� ��������
     *  \param  [in]  *message  - ���������� ��������� � �������
     *  \param  [in]  *checkmsg - ����������� ��������� � �������
     *  \param  [in]   time     - ����� ��������
     *
     *  \return        ������ (X96_NOSMS, X96_FALSESMS, X96_TRUESMS)
     */
    sms_state_t inspectionSmsFor(const char *to,
                                 const char *message,
                                 const char *checkmsg,
                                 float       time);

    /*!
     *  \brief         �������� ��������� �� Sim900
     *
     *  \return        ��������� �� Sim900
     */
    Sim900 *getSim900();

    /*!
     *  \brief         �������� ��������� �� StarlineOnline
     *
     *  \return        ��������� �� StarlineOnline
     */
    StarlineOnline *getStarlineOnline();

    /*!
     *  \brief         ������� �� ������ ���. ������ 1
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLedKeyringBut1(float type);

    /*!
     *  \brief         ������� �� ������ ���. ������ 2
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLedKeyringBut2(float type);

    /*!
     *  \brief         ������� �� ������ ���. ������ 3
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLedKeyringBut3(float type);

    /*!
     *  \brief         ������� �� ������ ��-������ 1
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLcdKeyringBut1(float type);

    /*!
     *  \brief         ������� �� ������ ��-������ 2
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLcdKeyringBut2(float type);

    /*!
     *  \brief         ������� �� ������ ��-������ 3
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLcdKeyringBut3(float type);

    /*!
     *  \brief         ������� �� ������ ��-������ 4
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLcdKeyringBut4(float type);

    /*!
     *  \brief         ������������� ������� �� 2 ������ ���. ������ (2 � 3)
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLedKeyringBut23(float type);

    /*!
     *  \brief         ������������� ������� �� 2 ������ ��-������ (2 � 3)
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushLcdKeyringBut23(float type);

    /*!
     *  \brief         ������������� ������� �� 2 ������ ��-������ (1 � 4)
     *
     *  \param  [in]   push_time_s - ������������ �������
     */
    void pushLcdKeyringBut14(float push_time_s);

    /*!
     *  \brief         ������������� ������� �� 2 ������ ��-������ (2 � 4)
     *
     *  \param  [in]   push_time_s - ������������ �������
     */
    void pushLcdKeyringBut24(float push_time_s);

    /*!
     *  \brief         ������������� ������� �� 2 ������ ��-������ (3 � 4)
     *
     *  \param  [in]   push_time_s - ������������ �������
     */
    void pushLcdKeyringBut34(float push_time_s);
    
    /*!
     *  \brief         ����� ������ ��������� ������ (����./��������/�����)
     *
     *  \param  [in]   bp_type - ����� ���������
     *
     *  \return        bool
     *  \retval        true  - �������� ������
     *  \retval        false - ������ �� �������
     */
    bool setBypassMode(slp_imit_bp_mode_t bp_mode);
    
    /*!
     *  \brief         ����� ���� ��������� ������ (KIA/Ford � �.�.)
     *
     *  \param  [in]   bp_type - ��� ���������
     *
     *  \return        bool
     *  \retval        true  - �������� ������
     *  \retval        false - ������ �� �������
     */
    bool setBypassType(slp_imit_bp_type_t bp_type);
    
    /*!
     *  \brief         ������ � ������ ��� ���������� � ������
     */
    void setArmBit();
    
    /*!
     *  \brief         ������ � ������ ��� ������ � ������
     */
    void setDisarmBit();

    /*!
     *  \brief         ������ � ������ ��� ������� ���������
     */
    void setEngineStartBit();
    
    /*!
     *  \brief         ������ � ������ ��� ��������� ���������
     */
    void setEngineStopBit();
    
    /*!
     *  \brief         ������ � ������ ��� ������� ���
     */
    void setPreheaterStartBit();
    
    /*!
     *  \brief         ������ � ������ ��� ��������� ���
     */
    void setPreheaterStopBit();
    
    /*!
     *  \brief         ������ � ������ ��� �������� ������
     */
    void setSignalBit();
    
    /*!
     *  \brief         ������ � ������ ��� �������� ���������
     */
    void setOpenTrunkBit();
    
    /*!
     *  \brief         ������ � ������ ��� ������� �������� ���������
     */
    void setBypassLearnBit();
    
    /*!
     *  \brief         ������ � ������ ��� �������� ������ ��������
     */
    void setBypassLearnClearBit();

    /*!
     *  \brief         ������ � ������ ��� ����� � ����� �����������
     *
     *  \param  [in]   reg_queue_mode - ������������ �� ������� �� ����������� (�������� � ������� ��� ���������������� ��� �������)
     *
     *  \return        bool
     *  \retval        true  - ���� �������� (��� �������)
     *  \retval        false - ���� �� �������� (��� �� �������)
     */
    bool setRegBit(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         ������ � ������ ��� ����� � ����� ����������� � �������� �������� � ����� �����������
     *
     *  \param  [in]   reg_queue_mode - ������������ �� ������� �� ����������� (�������� � ������� ��� ���������������� ��� �������)
     *
     *  \return        bool
     *  \retval        true  - ���� ��������
     *  \retval        false - ���� �� ��������
     */
    bool setAndCheckRegBit(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         ������ � ������ ��� ������������ ��������
     */
    void setResetBit();

    /*!
     *  \brief         ������ � ������ ��� ������ �� ������ ��������� �� BLE
     */
    void setBleSettingModeOffBit();

    /*!
     *  \brief         ������� � ����� ������
     */
    virtual void setPanicKeyring();
    
    /*!
     *  \brief         ���������� ��������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setHandBreak(int8_t         value,
                              command_type_t execution_method,
                              float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   logic_state      - ������ ������ 
     */
    virtual void setDoor(int8_t         value,
                         command_type_t execution_method,
                         float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ����������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   rpm_val          - �������� �������� ��� ��������� ���������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setEngine(int8_t         value,
                           command_type_t execution_method,
                           int16_t        rpm_val           = RPM_ON,
                           float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ��������� �������
     *
     *  \param  [in]   rpm_val          - ��������
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setRpm(int16_t        rpm_val,
                        command_type_t execution_method,
                        float          delay             = TIME_DELAY::STD_RPM_EN_DELAY_S);

    /*!
     *  \brief         ���������� �������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setHood(int8_t         value,
                         command_type_t execution_method,
                         float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ����������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual bool setTrunk(int8_t         value,
                          command_type_t execution_method,
                          float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ����������
     *
     *  \param  [in]   execution_method     - ����� ����������
     *  \param  [in]   value                - �������� (���/����)
     *  \param  [in]   delay_s              - �������� ����� ����������
     *  \param  [in]   reg_queue_leave_type - �������� �� �� ������� �� ����������� ��� ���������� ���������
     */
    virtual void setIgn(command_type_t         execution_method,
                        int8_t                 value,
                        float                  delay_s              = TIME_DELAY::MIN_WIRE_EN_DELAY_S,
                        reg_queue_leave_type_t reg_queue_leave_type = IGN_OFF_REG_QUEUE_LEAVE);

    /*!
     *  \brief         �������� ��������� �� ���� (���� ���� ������, ��� ��������� ��������)
     *
     *  \param  [in]  *relay  - ��������� �� ����
     *  \param  [in]   status - ������ ���������
     */
    void setRelayIgn(RelayBase *relay, bool status);

    /*!
     *  \brief         �������� ������� IGN �� ���� R6
     */
    void setR6Ign();

    /*!
     *  \brief         ��������� ������� IGN �� ���� R6
     */
    void resetR6Ign();

    /*!
     *  \brief         �������� ������� IGN �� ���� R4
     */
    void setR4Ign();

    /*!
     *  \brief         ��������� ������� IGN �� ���� R4
     */
    void resetR4Ign();

    /*!
     *  \brief         ������ ������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setBreak(int8_t         value,
                          command_type_t execution_method,
                          float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� ������ ����
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setRLight(int8_t         value,
                           command_type_t execution_method,
                           float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� ����� ����
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setLLight(int8_t         value,
                           command_type_t execution_method,
                           float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �� �������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setCLock(int8_t         value,
                          command_type_t execution_method,
                          float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ��� ��������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setPpp(int8_t         value,
                        command_type_t execution_method,
                        float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ���������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay            - �������� ����� ����������
     */
    virtual void setParking(int8_t         value,
                            command_type_t execution_method,
                            float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� �������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay_s          - �������� ����� ����������
     *  \param  [in]   tag_voltage_ctrl - ��������� �� �������� ����� (��� ���������� ������)
     */
    virtual void setSecure(int8_t             value,
                           command_type_t     execution_method,
                           float              delay_s          = TIME_DELAY::STD_REMOTE_STATUS_EN_TIME_S,
                           tag_voltage_ctrl_t tag_voltage_ctrl = TAG_VOLTAGE_CTRL_EN);

    /*!
     *  \brief         ������������� ������
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   sms_delay_s      - ����� �������� ��������� ����� �������� ���
     */
    virtual void setRemoteRun(command_type_t execution_method,
                              float          sms_delay_s       = TIME_DELAY::BYPASS_AFTER_SMS_TIME_S);

    /*!
     *  \brief         ������������� ��������
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   sms_delay_s      - ����� �������� �����-����/����. �� ����� �������� ���
     */
    virtual void setRemoteStop(command_type_t execution_method,
                               float sms_delay_s                = TIME_DELAY::MAX_WAITING_SMS_STATUS_TIME_S);

    /*!
     *  \brief         ������ ���������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   engine_start_system - ������� �������
     *  \param  [in]   acc_must_be_off     - ������ �� ����������� ����������
     *  \param  [in]   rpm_val             - �������� ��������, ��� �������
     */
    virtual void runEngine(command_type_t        execution_method,
                           engine_start_system_t engine_start_system = IGNITION_LOCK,
                           bool                  acc_must_be_off     = true,
                           int16_t               rpm_val             = RPM_ON);

    /*!
     *  \brief         �������� ��������� ����� �������
     *
     *  \param  [in]   engine_start_system   - ������� ������� ��������
     *  \param  [in]   bypass_waiting_time_s - ����� �������� ������� ������
     *  \param  [in]   acc_must_be_off       - ������ �� ���� �������� ����������
     *  \param  [in]   log_label             - ����� ����, ������� ��������� ��� ������������ ��������
     *  \param  [in]   execution_method      - ����� ��������� ��������� � ��������
     *
     *  \return        engine_start_error_t - ������
     */
    engine_start_error_t checkEngine(engine_start_system_t engine_start_system,
                                     float                 bypass_waiting_time_s,
                                     bool                  acc_must_be_off        = true,
                                     log_label_t           log_label              = LOG_LABEL_FAIL,
                                     command_type_t        execution_method       = PIN);

    /*!
     *  \brief         �������� ��������� ����� ������� (����� ���������)
     *
     *  \param  [in]   bypass_waiting_time_s - ����� �������� ������� ������
     *  \param  [in]   acc_must_be_off       - ������ �� ���� �������� ����������
     *  \param  [in]   log_label             - ����� ����, ������� ��������� ��� ������������ ��������
     *
     *  \return        engine_start_error_t - ������
     */
    engine_start_error_t checkIgnLockEngine(float          bypass_waiting_time_s,
                                            bool           acc_must_be_off        = true,
                                            log_label_t    log_label              = LOG_LABEL_FAIL,
                                            command_type_t execution_method       = PIN);

    /*!
     *  \brief         �������� ��������� ����� ������� (�����-����)
     *
     *  \param  [in]   engine_start_system   - ������� ������� ��������
     *  \param  [in]   bypass_waiting_time_s - ����� �������� ������� ������ � ��������
     *  \param  [in]   log_label             - ����� ����, ������� ��������� ��� ������������ ��������
     *
     *  \return        engine_start_error_t - ������
     */
    engine_start_error_t checkStartStopEngine(engine_start_system_t engine_start_system,
                                              float                 bypass_waiting_time_s,
                                              log_label_t           log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         �������� ��������� ����� ������� (�����-���� ���� �������)
     *
     *  \param  [in]   bypass_waiting_time_s - ����� �������� ������� ������
     *  \param  [in]   rpm_val               - �������� ��������, ������� ������������ ����� ��������� ������������� ��������
     *  \param  [in]   log_label             - ����� ����, ������� ��������� ��� ������������ ��������
     *
     *  \return        engine_start_error_t  - ������
     */
    engine_start_error_t checkStartStopOneImpulseEngine(float       bypass_waiting_time_s,
                                                        int16_t     rpm_val                = RPM_ON,
                                                        log_label_t log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         �������� ��������� ����� ������� (�����-���� ��� ��������)
     *
     *  \param  [in]   bypass_waiting_time_s - ����� �������� ������� ������
     *  \param  [in]   rpm_val               - �������� ��������, ������� ������������ ����� ��������� ������������� ��������
     *  \param  [in]   log_label             - ����� ����, ������� ��������� ��� ������������ ��������
     *
     *  \return        engine_start_error_t  - ������
     */
    engine_start_error_t checkStartStopTwoImpulseEngine(float       bypass_waiting_time_s,
                                                        int16_t     rpm_val                = RPM_ON,
                                                        log_label_t log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         �������� ��������� ����� ������� (�����-���� ��� ��������)
     *
     *  \param  [in]   bypass_waiting_time_s - ����� �������� ������� ������
     *  \param  [in]   rpm_val               - �������� ��������, ������� ������������ ����� ��������� ������������� ��������
     *  \param  [in]   log_label             - ����� ����, ������� ��������� ��� ������������ ��������
     *
     *  \return        engine_start_error_t  - ������
     */
    engine_start_error_t checkStartStopThreeImpulseEngine(float       bypass_waiting_time_s,
                                                          int16_t     rpm_val                = RPM_ON,
                                                          log_label_t log_label              = LOG_LABEL_FAIL);

    /*!
     *  \brief         ��������� ��������� ��������� sup_ign_var 0-������/ 1 - ����
     *
     *  \param  [in]   sup_ign_var         - ��� ��������� ���������
     *  \param  [in]   engine_start_system - ������� �������
     *  \param  [in]   ign_status          - ��������� ��������� ����� ���������� ��� ���
     */
    virtual void setSupportIgn(eng_start_ign_supp_type_t sup_ign_var         = IGN_SUPP_TYPE_HAND_BRAKE_OR_PARKING,
                               engine_start_system_t     engine_start_system = IGNITION_LOCK,
                               ign_statuses_t            ign_status          = IGN_OFF);

    /*!
     *  \brief         ����������� ��������
     *
     *  \param  [in]   execution_method     - ����� ����������
     *  \param  [in]   program_neutral_type - ��� �������� ����������� �������� true - �� ������, false - �� �����
     *  \param  [in]   sup_ign_var          - ��� ��������� ��������� true - �� ���������, false - �� �������
     *  \param  [in]   engine_start_system  - ������� �������, ����� - 0, �� ����� > 0
     *  \param  [in]   engine_gearbox_type  - ��� ��� false - ����, true - ����
     *  \param  [in]   ign_status           - ��������� ��������� ����� ���������� �� ��� ���
     *  \param  [in]   delay_s              - �������� ����� ����������
     */
    virtual void setProgramNeutral(command_type_t                  execution_method,
                                   eng_start_neutral_detect_type_t program_neutral_type = NEUTRAL_DETECT_TYPE_DOOR,
                                   eng_start_ign_supp_type_t       sup_ign_var          = IGN_SUPP_TYPE_HAND_BRAKE_OR_PARKING,
                                   engine_start_system_t           engine_start_system  = IGNITION_LOCK,
                                   eng_start_gearbox_type_t        engine_gearbox_type  = ENG_GEARBOX_MANUAL,
                                   ign_statuses_t                  ign_status           = IGN_OFF,
                                   float                           delay_s              = TIME_DELAY::MIN_DELAY_AFTER_PROGRAM_NEUTRAL_S);

    /*!
     *  \brief         ��������/��������� ���. ������
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   state            - ��������� (1 - ��������, 0 - ���������)
     */
    void setAdditionalSensor(command_type_t execution_method, bool state);
                                     
    /*!
     *  \brief         �������� ����������� ��������
     *
     *  \return        bool
     *  \retval        true  - ������ � �������: �� ���������
     *  \retval        false - ������ � �������: �� �� ���������
     */
    bool getProgramNeutral();
                                    
    /*!
     *  \brief         �������� ��������� ���������
     *
     *  \return        bool
     *  \retval        true  - ������ � �������: �� ���������
     *  \retval        false - ������ � �������: �� �� ���������
     */
    bool getSupportIgn();
    
    /*!
     *  \brief         �������� ������� BLE
     *
     *  \return        bool
     *  \retval        true  - ������ � �������: ����� � R6 �� �����
     *  \retval        false - ������ � �������: ����� �/��� R6 �� �� �����
     */
    virtual bool getBleStatus(command_type_t execution_method = SLP);
    
    /*!
     *  \brief         �������� ������� R6
     *
     *  \return        bool
     *  \retval        true  - ������ � �������: R6 �� �����
     *  \retval        false - ������ � �������: R6 �� �� �����
     */
    virtual bool getR6Status(command_type_t execution_method = SLP);

    /*!
     *  \brief         �������� ������� �����
     *
     *  \return        bool
     *  \retval        true  - ������ � �������: ����� �� �����
     *  \retval        false - ������ � �������: ����� �� �� �����
     */
    virtual bool getTagStatus(command_type_t execution_method = SLP);
                                    
    /*!
     *  \brief         �������� ��
     */
    virtual bool getTurboTimer();

    /*!
     *  \brief         ���������� ��������� "�����"
     */
    void setDisarmSt();

    /*!
     *  \brief         ��������� ���������� ���������
     *
     *  \param  [in]   state_num - ����� ���������
     */
    virtual void setState(int8_t state_num = FULL);
    
    /*!
     *  \brief         ������ ������
     *
     *  \return        ������
     */
    virtual int8_t getAlarm();

    /*!
     *  \brief         ������ ����������
     *
     *  \param  [in]   execution_method - ����� ����������
     *
     *  \return        ������
     */
    virtual int8_t getBlock(command_type_t execution_method = PIN);

    /*!
     *  \brief         �������� ������� ����� � R6
     *
     *  \param  [in]   time     - ����� ��������
     *  \param  [in]   polarity - ����������
     *
     *  \return        ������
     */
    bool waitBleStatus(float      time,
                       bool       error_flag = false,
                       float      error      = TIME_DELAY::NULL_DELAY_S);
                       
    /*!
     *  \brief         �������� ������� R6
     *
     *  \param  [in]   time           - ����� ��������
     *  \param  [in]   polarity       - ����������
     *  \param  [in]   waiting_status - ��������� ������
     *
     *  \return        ������
     */
    bool waitR6Status(float      time,
                      bool       error_flag     = false,
                      float      error          = TIME_DELAY::NULL_DELAY_S,
                      bool       waiting_status = true);

    /*!
     *  \brief         �������� ��������� �� ������������ ���� (R4, R6) ��������� � ��������� ����� ������
     *
     *  \param  [in]  *relay              - ��������� �� ����, � �������� ����� ��������� ��������
     *  \param  [in]   lock_pulse_count   - ��������� ���������� ��������� ��������� (������ � r6_pulse_t)
     *  \param  [in]   unlock_pulse_count - ��������� ���������� ��������� ��������� (������ � r6_pulse_t)
     *  \param  [in]   timeout_s          - ����� ��������
     *
     *  \return        ������������� �� ���������� ��������� ����������
     */
    bool waitRelayLockAndUnlock(RelayBase *relay,
                                uint8_t    lock_pulse_count,
                                uint8_t    unlock_pulse_count,
                                float      timeout_s);

    /*!
     *  \brief         �������� ��������� R4 ��������� � ��������� ����� ������
     *
     *  \param  [in]   lock_pulse_count   - ��������� ���������� ��������� ��������� (������ � relay_pulse_t)
     *  \param  [in]   unlock_pulse_count - ��������� ���������� ��������� ��������� (������ � relay_pulse_t)
     *  \param  [in]   timeout_s          - ����� ��������
     *
     *  \return        ������������� �� ���������� ��������� ����������
     */
    bool waitR4LockAndUnlock(uint8_t lock_pulse_count,
                             uint8_t unlock_pulse_count,
                             float   timeout_s = TIME_DELAY::MAX_R4_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ������� ������ (������� � R6)
     *
     *  \param  [in]   type      - ��� ����������� ������
     *  \param  [in]   timeout_s - ����� �������� ������� � ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������� ��������
     *  \retval        false - ��������� ������� �� �������� ��� ������� ������ ������ (����. ���� ������ R6 � ����� ������� ������� ������� ������, �� �������� false)
     */
    bool waitSirenType(siren_type_mask_t type, float timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ��������� R6 ��������� � ��������� ����� ������
     *
     *  \param  [in]   lock_pulse_count   - ��������� ���������� ��������� ��������� (������ � relay_pulse_t)
     *  \param  [in]   unlock_pulse_count - ��������� ���������� ��������� ��������� (������ � relay_pulse_t)
     *  \param  [in]   timeout_s          - ����� ��������
     *
     *  \return        ������������� �� ���������� ��������� ����������
     */
    bool waitR6LockAndUnlock(uint8_t lock_pulse_count,
                             uint8_t unlock_pulse_count,
                             float   timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ���������� R6 �� �������� ��������� ����� ������
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        ������������� �� ���������� ��������� ����������
     */
    bool waitR6LockPotential(float timeout_s = TIME_DELAY::MIN_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ���������� R6 �� �������� ��������� ����� ������
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        ������������� �� ���������� ��������� ����������
     */
    bool waitR6UnlockPotential(float timeout_s = TIME_DELAY::MIN_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� �������� ������ R6
     *
     *  \param  [in]   signal_count - ���������� ��������� �������� (relay_pulse_t)
     *  \param  [in]   timeout_s    - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ���������� �������� ������������� ����������
     *  \retval        false - ���������� �������� �� ������������� ����������
     */
    bool waitR6SirenSignals(uint8_t signal_count, float timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ������������ ���������� ���� (R4 ��� R6) (��� ����������: ��)
     *
     *  \param  [in]  *relay     - ��������� �� ����
     *  \param  [in]   status    - ��������� ��������� ����������
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ���������� � �������� ���������
     *  \retval        false - ���������� �� � �������� ���������
     */
    bool waitRelayBlockNormalOpen(RelayBase *relay,
                                  bool       status,
                                  float      timeout_s);

    /*!
     *  \brief         �������� ������������ ���������� R6 (��� ����������: ��)
     *
     *  \param  [in]   status    - ��������� ��������� ����������
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ���������� � �������� ���������
     *  \retval        false - ���������� �� � �������� ���������
     */
    bool waitR6BlockNormalOpen(bool status, float timeout_s = TIME_DELAY::MAX_R6_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         �������� ������������ ���������� R6 (��� ����������: ��)
     *
     *  \param  [in]   status    - ��������� ��������� ����������
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ���������� � �������� ���������
     *  \retval        false - ���������� �� � �������� ���������
     */
    bool waitR6BlockNormalClose(bool status, float timeout_s = TIME_DELAY::MAX_R6_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         �������� ������������ ���������� R4 (��� ����������: ��)
     *
     *  \param  [in]   status    - ��������� ��������� ����������
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ���������� � �������� ���������
     *  \retval        false - ���������� �� � �������� ���������
     */
    bool waitR4Block(bool status, float timeout_s = TIME_DELAY::MAX_R4_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         �������� ������������ ������� ���������� R4 (������� Output)
     *
     *  \param  [in]   status    - ��������� ���������
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \retval        true  - ���������� � �������� ���������
     *  \retval        false - ���������� �� � �������� ���������
     */
    bool waitR4Output(bool status, float timeout_s = TIME_DELAY::MAX_R4_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ������� ������ ��� �������� R6
     *
     *  \param  [in]   timeout_s - ����� ��������
     *  \param  [in]   type      - ��� ������� (R6_SIREN - ������, R6_KLAXON - �������)
     *
     *  \return        ��� �� ������� ������
     */
    bool waitR6Alarm(r6_alarm_signal_t type      = R6_SIREN,
                     float             timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ���������� ������ R6
     *
     *  \param  [in]   polarity  - ���������� �������
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� �������
     *  \retval        false - ��������� �� �������
     */
    bool waitR6SirenPotential(r6_siren_potential_t polarity, float timeout_s = TIME_DELAY::MAX_R6_SIGNAL_WAIT_TIME_S);

    /*!
     *  \brief         �������� ���������� ���������� �� R6 ECO
     *
     *  \param  [in]   led_ext_mode - ����� ������
     *  \param  [in]   timeout_s    - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkR6EcoLedExt(r6eco_led_ext_mode_t led_ext_mode, float timeout_s);

    /*!
     *  \brief         �������� ������� �����
     *
     *  \param  [in]   time_s     - ����� ��������
     *  \param  [in]   error_flag - ��������� �� �����������
     *  \param  [in]   error_s    - �����������
     *
     *  \return        ������
     */
    bool waitTagStatus(float time_s,
                       bool  error_flag = false,
                       float error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ������ ������
     *
     *  \param  [in]   execution_method - ����� ����������
     *
     *  \return        ������
     */
    virtual int8_t getHood(command_type_t execution_method);

    /*!
     *  \brief         ������ ���������
     *
     *  \param  [in]   execution_method - ����� ����������
     *
     *  \return        ������
     */
    virtual int8_t getIgn(command_type_t execution_method);

    /*!
     *  \brief         �������� �� ���������� ���������
     *
     *  \return        1 - ���., 0 - ����.
     */
    bool isBlockSet();

    /*!
     *  \brief         ������ �� �����
     *
     *  \param  [in]   execution_method - ����� ����������
     *
     *  \return        1 - ��, 0 - ���
     */
    bool isTagVisible(command_type_t execution_method = PIN);

    /*!
     *  \brief         ������������� ��������
     *
     *  \param  [in]   engine_start_system    - ������� �������
     *  \param  [in]   stop_after_impulse_num - ��������� ����� ��������
     *  \param  [in]   time_s                 - ����� �������� ������� ��������
     */
    virtual void checkEngineStop(engine_start_system_t engine_start_system    = IGNITION_LOCK,
                                 stop_impulse_t        stop_after_impulse_num = STOP_AFTER_SECOND_IMPULSE_REMOVED,
                                 float                 time_s                 = TIME_DELAY::FIRST_STOP_IMPULSE_RECV_WAIT_S);

    /*!
     *  \brief         �������� ��������� ���������
     *
     *  \param  [in]   ign_status - ����� �� ��������� ��������� ����� ��������
     *  \param  [in]   state      - ��������� ��������� ��
     */
    virtual void checkSupIgn(ign_statuses_t ign_status, out_statuses_t state);

    /*!
     *  \brief         �������� ������
     *
     *  \return        ���������
     */
    int8_t checkSecure(int8_t model = ON);
    
    /*!
     *  \brief         �������� ������ ����� ��������� ���������� �� BLE
     *
     *  \param  [in]   model     - �������� ������
     *  
     *  \return        ������������� �� ��������� ���������
     */
    int8_t checkSecureMobile(int8_t model = ON);
    

    /*!
     *  \brief         �������� ������� �������
     *
     *  \param  [in]   status       - ��������� ������
     *  \param  [in]   state        - �������� ��������� (true) ��� ������ (false) ���������� �������
     *  \param  [in]   time_s       - ����� �������� � ��������
     *  \param  [in]   error_time_s - ���������� ����������� � ��������
     *
     *  \return        bool
     *  \retval        true   - ������ ������������� ����������
     *  \retval        false  - ������ �� ������������� ����������
     */
    bool waitGuardState(slp_diag_data_statuses_t status,
                        bool                     state,
                        float                    time_s,
                        float                    error_time_s = TIME_DELAY::NO_ERROR_TIME_S);

    /*!
     *  \brief         �������� ����������� ���������
     *
     *  \param  [in]   state - ��������� ��������
     */
    virtual void checkInspector(int8_t state);

    /*!
     *  \brief         ������������� �� IGN1
     *
     *  \param  [in]   time_s - ����� ��������
     */
    void synByIgn1(float time_s = TIME_DELAY::FIVE_MINUTE);

    /*!
     *  \brief         ������������� �� ����������� ���������
     */
    virtual void synByInspector();

    /*!
     *  \brief         ������ �������� �� ������������
     *
     *  \param  [in]   turbotimer_time_s   - ����� ������ ������������
     *  \param  [in]   engine_start_system - ��� �������
     *  \param  [in]   error_time_s        - ����������� ��������� ������������
     */
    virtual void waitTurbotimerTime(float                 turbotimer_time_s,
                                    engine_start_system_t engine_start_system,
                                    float                 error_time_s = TIME_DELAY::TT_ERROR_TIME_S);

    /*!
     *  \brief         ������ �������� �� ������� �������
     *
     *  \param  [in]   autostart_time_s    - ����� ������ �����������
     *  \param  [in]   engine_start_system - ��� �������
     *  \param  [in]   error_time_s        - ����������� ��������� �����������
     */
    virtual void waitAutoStartTime(float                 autostart_time_s,
                                   engine_start_system_t engine_start_system,
                                   float                 error_time_s = TIME_DELAY::AUTO_START_REMOTE_ERROR_TIME_S);

    /*!
     *  \brief         �������� ������� � ������� ������� ���
     *
     *  \param  [in]   polarity    - ���������� �������
     *  \param  [in]   wait_time_s - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - ������ �������
     *  \retval        false - ������ �� �������
     */
    bool waitPppStartSignal(bool polarity, float wait_time_s = TIME_DELAY::WAITING_HEAT_CTRL_PULSE_ERROR_S);

    /*!
     *  \brief         �������� �������� ���
     *
     *  \param  [in]   ppp_max_time_s   - ����. ����� ������
     *  \param  [in]   polarity         - ���������� �������
     *  \param  [in]   max_error_time_s - ����. ���������� �����������
     */
    virtual void waitPppImpulse(float ppp_max_time_s,
                                bool  polarity,
                                float max_error_time_s);

    /*!
     *  \brief         �������� �������� ���
     *
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� ����� �������� 
     *  \param  [in]   pulse_time - ����� �������� ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        bool
     *  \retval        true       - ��������� �������� ��������
     *  \retval        false      - ��������� �������� �� ��������
     */
    bool waitPppPulse(uint32_t count,
                      bool     polarity,
                      float    time,
                      float    pulse_time,
                      bool     error_flag = false,
                      float    error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         �������� ���������� ���
     *
     *  \param  [in]   time_s    - ����� �������� (����� ������ ���)
     *  \param  [in]   polarity  - ���������� �������
     *  \param  [in]   max_error - ����. ���������� �����������
     *
     *  \return        bool
     *  \retval        true       - ��������� ��������� �������
     *  \retval        false      - ��������� ��������� �� �������
     */
    virtual bool waitPppPotential(float time_s,
                                  bool  polarity,
                                  float max_error_s = TIME_DELAY::NO_ERROR_TIME_S);

    /*!
     *  \brief         ��������� ������� �����
     */
    void setTagVcc();

    /*!
     *  \brief         ���������� ������� �����
     */
    void resetTagVcc();
    
    /*!
     *  \brief         ��������� ������� R4
     */
    void setR4Vcc();

    /*!
     *  \brief         ���������� ������� R4
     */
    void resetR4Vcc();
    
    /*!
     *  \brief         ��������� ������� R6
     */
    void setR6Vcc();

    /*!
     *  \brief         ��������� ��������� ������� R6
     *
     *  \return        bool
     *  \retval        true  - ������� ������
     *  \retval        fasle - ������� �� ������
     */
    bool getR6Vcc();

    /*!
     *  \brief         ���������� ������� R6
     */
    void resetR6Vcc();

    /*!
     *  \brief         ������ �� ������ �����
     */
    void setTagBtn();

    /*!
     *  \brief         ��������� ������ �����
     */
    void resetTagBtn();

    /*!
     *  \brief         ������� �� ������ �����
     *
     *  \param  [in]   type - ������������ �������
     */
    void pushTagBtn(float type);

    /*!
     *  \brief         ������ ���
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   btn1_time_s      - ������������ ������� 1�� ������ �������
     */
    virtual void startPpp(command_type_t execution_method,
                          float          btn1_time_s = KEYRING_SHORT);

    /*!
     *  \brief         ��������� ���
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   ppp_type         - ��� ���
     *  \param  [in]   btn1_time_s      - ������������ ������� 2�� ������ �������
     */
    virtual void stopPpp(command_type_t execution_method,
                         int8_t         ppp_type    = 0,
                         float          btn2_time_s = KEYRING_SHORT);    

    /*!
     *  \brief         ��������� ������� �� ������ ���
     *
     *  \detailed      ������� �� ������ startPpp � ���, ��� ����� �������� ���. ������� ��� ����� �������� ��� ����� ������ ����������
     *                 ���������� ������� ������������ ������������� (���� ������ �� �������� - �������� ���. ��� �� �����)
     *
     *  \param  [in]   execution_method               - ����� ����������
     *  \param  [in]   eng_heat_after_sms_wait_time_s - ����� �������� ��������� ������ �������� ��� ����� ���
     */
    void setPppCmdStart(command_type_t execution_method, float eng_heat_after_sms_wait_time_s = TIME_DELAY::MAX_WAITING_SMS_STATUS_TIME_S);

    /*!
     *  \brief         ��������� ������� �� ��������� ���
     *
     *  \detailed      ������� �� stopPpp � ���, ��� ����� �������� ����. ������� ��� ����� �������� ��� ����� ������ ����������
     *                 ���������� ������� ������������ ������������� (���� ������ ��������, ���� �� �������� - �������� ����. ��� �� �����)
     *
     *  \param  [in]   execution_method               - ����� ����������
     *  \param  [in]   eng_heat_after_sms_wait_time_s - ����� �������� ���������� ������ �������� ��� ����� ���
     */
    void setPppCmdStop(command_type_t execution_method, float eng_heat_after_sms_wait_time_s = TIME_DELAY::MAX_WAITING_SMS_STATUS_TIME_S);

    /*!
     *  \brief         ���������� ��������� �������
     *
     *  \param  [in]   state - ���������
     */
    void setServiceBtn(bool state);

    /*!
     *  \brief         ������ ��������� ������
     *
     *  \param  [in]   count   - ���-�� �������
     *  \param  [in]   delay_s - �������� ����� ���������
     */
    void clickServiceBtn(uint32_t count   = COUNT_STRUCT::MIN_BTN_CLICKS_COUNT, 
                         float    delay_s = TIME_DELAY::MIN_BTN_BETWEEN_CLICK_S);

    /*!
     *  \brief         ������� �� ��������� � ��������� ������
     *
     *  \return        1 - �������, 0 - ���
     */
    bool isServiceLedOn();

    /*!
     *  \brief         �������� ������� ���������� ����������
     *
     *  \param  [in]   out_status - ��������� ������ (�������/��������)
     *  \param  [in]   timeout_s  - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������ �������
     *  \retval        false - ��������� ������ �� �������
     */
    bool waitServiceLed(out_statuses_t out_status, float timeout_s);

    /*!
     *  \brief         �������� �������� ���������� ��������� ������
     *
     *  \param  [in]   pulse_width_s - ��������� ������������ ��������
     *  \param  [in]   err_time_s    - ����������� ��������
     *  \param  [in]   timeout_s     - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ������� �������
     *  \retval        false - ������� �� ������� ��� ����������� �� ������������� ���������
     */
    bool waitSrvBtnPulse(float pulse_width_s,
                         float err_time_s,
                         float timeout_s = TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    /*!
     *  \brief         �������� ��������
     *
     *  \param  [in]   amount - ���������� ���������
     *  \param  [in]   time_s - ����� ��������
     *
     *  \return        1 - ������� �������, 0 - ���
     */
    bool waitServicePulseAmount(uint8_t amount, float time_s);

    /*!
     *  \brief         ���������� ������ � ������ ��� ���������
     *
     *  \param  [in]  *pin   - ��� ����������� ������
     *
     *  \return        1 - ������, 0 - ���
     */
    bool emergencyDisarming(const char *pin = NULL);
    
    /*!
     *  \brief         �������� ����� ���� ����������� ������ � ������ ��� ���������
     *
     *  \param  [in]   digits - ����� �� ��������� ����� ����������� �����
     *  \param  [in]  *pin    - ��� ����������� ������
     *
     *  \return        1 - ������, 0 - ���
     */
    bool checkChangeEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits);
    
    /*!
     *  \brief         C���� ���� ����������� ������ � ������ ��� ���������
     *
     *  \param  [in]  *old_pin    - ��� ����������� ������
     *  \param  [in]  *new_pin
     *
     *  \return        1 - ������, 0 - ���
     */
    void changeEmergencyDisarming(const char *old_pin, const char *new_pin);
    
    /*!
     *  \brief         �������� ����������� ������ � ������ ��� ���������
     *
     *  \param  [in]   digits - ����� �� ��������� ����� ����������� �����
     *  \param  [in]  *pin    - ��� ����������� ������
     *
     *  \return        1 - ������, 0 - ���
     */
    bool checkEmergencyDisarming(const char *pin, emergency_code_exit_digits_t digits);

    /*!
     *  \brief         ���������� ������ � ������
     *
     *  \param  [in]  *pin - ��� ����������� ������
     *
     *  \return        1 - ������, 0 - ���
     */
    bool emergencyDisarmingIgn(const char *pin = NULL);

    /*!
     *  \brief         ���� � ����� �����������
     *
     *  \param  [in]   state          - ��������� (1 - ���������, 0 - ����� �� ������)
     *  \param  [in]   reg_queue_mode - ������������ �� ������� �� ����������� (�������� � ������� ��� ���������������� ��� �������)
     *
     *  \return        1 - ������, 0 - ���
     */
    virtual bool registrationMode(bool state, registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         ������ � ������� �� ����������� ���������
     *
     *  \return        bool
     *  \retval        true  - ���������� � ������� ������ (����� ������� ���������)
     *  \retval        false - ���������� � ������� �������� (����� ������� �� ���������)
     */
    bool addDeviceToRegistrationQueue();

    /*!
     *  \brief         ������� ������� ��� ��������� ��������� ��� ����� ���� �������
     */
    void setHandsFreeKeyring();
    
    /*!
     *  \brief         ��������� ���������� ������ ����� ������
     */
    void setServiceModeKeyring();

    /*!
     *  \brief         ������� ������� �� ���� � ��������� ����� ����� �����
     *
     *  \param  [in]   state - ������ (1 - ���., 0 - ����.)
     *
     *  \return        bool
     *  \retval        true           - ������ � ������� ������������� ����������
     *  \retval        false          - ������ � ������� �� ������������� ����������
     */
    bool setServiceModeTag(bool state);

    /*!
     *  \brief         ���������� ��������� �������
     *
     *  \param  [in]   out_status - �������� ��������� (���./����.)
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - �������� �� �������� (��� ����������� �������)
     */
    bool setServiceMode(out_statuses_t out_status);

    /*!
     *  \brief         ��������� ������� SLP
     *
     *  \param  [in]   addr   - ����� �������
     *  \param  [in]   size   - ������
     *  \param  [in]   status - ��������� ������
     *
     *  \return        bool
     *  \retval        true  - ������ ������������� ����������
     *  \retval        false - ������ �� ������������� ����������
     */
    bool getSlpCmdStatus(slp_statuses_t addr,
                         uint32_t       size,
                         uint32_t       status);

    /*!
     *  \brief         �������� ������� SLP
     *
     *  \param  [in]   addr           - ����� �������
     *  \param  [in]   size           - ������
     *  \param  [in]   status         - ��������� ������
     *  \param  [in]   state          - ��������� ��������� ������� (�����������/���)
     *  \param  [in]   time_s         - ����� ��������
     *  \param  [in]   error_time_s   - ���������� ����������� � ��������
     *  \param  [in]   error_time_use - ������ �� ����������� �� ���������� �������� �������, ����� �������� ������
     *
     *  \return        bool
     *  \retval        true           - ������ ������������� ����������
     *  \retval        false          - ������ �� ������������� ����������
     */
    bool waitSlpCmdStatus(slp_statuses_t addr,
                          uint32_t       size,
                          uint32_t       status,
                          bool           state,
                          float          time_s,
                          float          error_time_s = TIME_DELAY::NO_ERROR_TIME_S,
                          bool           error_time_use = true);
    
    /*!
     *  \brief         �������� ������������ ��������������� ������� 1 (�� �������)
     */
    void setAuxSensor1();

    /*!
     *  \brief         �������� ������������ ��������������� ������� 1 (�� �������)
     *
     *  \param  [in]   status  - �������� ��������� (���. / ����.)
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setAuxSensorFirstAnalog(out_statuses_t status, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� ������������ ��������������� ������� 2
     */
    void setAuxSensor2();

    /*!
     *  \brief         �������� ������������ ��������������� ������� 2 (�� �������)
     *
     *  \param  [in]   status  - �������� ��������� (���. / ����.)
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setAuxSensorSecondAnalog(out_statuses_t status, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� ������������ ������� ����� �� �������� ������
     */
    void setShockSensorHigh();

    /*!
     *  \brief         �������� ������������ ������� ����� �� ������� ������
     */
    void setShockSensorLow();
    
    /*!
     *  \brief         �������� ������������ ������� ��������
     */
    void setMoveSensor();
    
    /*!
     *  \brief         �������� ������������ ������� �������
     */
    void setTiltSensor();

    /*!
     *  \brief         ����������� ���� �� ��������
     *
     *  \param  [in]   sensor_type - ����������� ������
     */
    void setSensor(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         ����������� ���� �� �������� (��� �������� ����� ����������)
     *
     *  \param  [in]   sensor_type - ����������� ������
     */
    void setSensorNoDelay(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         ��������� ����������� ���������
     *
     *  \param  [in]   value - �������� �����������
     */
    virtual void setEngineTemperatureSensor(int8_t value);

    /*!
     *  \brief         ��������� ����������� ��������� (��� �������� ����� ����������)
     *
     *  \param  [in]   value - �������� �����������
     */
    virtual void setEngineTemperatureSensorNoDelay(int8_t value);

    /*!
     *  \brief         ��������� ����������� �� �����
     *
     *  \param  [in]   value - �������� �����������
     */
    virtual void setBoardTemperatureSensor(int8_t value);

    /*!
     *  \brief         ��������� ����������� ��������� �� CAN �� ���������� Lada Vesta (7114)
     *
     *  \param  [in]   value - �������� �����������
     */
    virtual void setEngineTemperatureCanLadaVesta(int8_t value);

    /*!
     *  \brief         ��������� ��������� ��������� �������
     *
     *  \param  [in]   time       - ����� ��������
     *  \param  [in]   time_error - ���������� �����������
     *
     *  \return        ������ (NOSIGNAL, PULSED, CONST)
     */
    alarm_signal_t getAlarmSignal(float time, float time_error = TIME_DELAY::NO_ERROR_TIME_S);

    /*!
     *  \brief         �������� ����������
     *
     *  \param  [in]   time_s           - ����� ��������
     *  \param  [in]   error_time_s     - ����������� (TIME_DELAY::NO_ERROR_TIME_S - �� �����������)
     *  \param  [in]   status           - ��������� ������ (true - ���������)
     *  \param  [in]   execution_method - ����� ����������
     *
     *  \return        bool
     *  \retval        true - ������ ������������� ����������
     */
    bool waitBlock(float          time_s,
                   float          error_time_s     = TIME_DELAY::NO_ERROR_TIME_S,
                   bool           status           = true,
                   command_type_t execution_method = PIN);

    /*!
     *  \brief         ������� ������� �� ����/����� � ����� ��������������
     *
     *  \param  [in]   state               - ���������
     *  \param  [in]  *pin                 - ��� ����������� ������
     *  \param  [in]  *gsm_owner_access_pw - �������� GSM ������
     *
     *  \return        1 - ������� ��������� �������
     */
    bool setAnj(bool        state,
                const char *pin                 = NULL,
                const char *gsm_owner_access_pw = NULL,
                bool        no_wait_message     = OFF);
                
    /*!
     *  \brief         ����������� ���������� ����������
     *
     *  \return        1 - ������� ��������� �������
     */
    bool registerMobile();

    /*!
     *  \brief         �������� ���������� � ������ ���. ����������
     *
     *  \return        1 - ���������� ������ � ������
     */
    bool checkMobile();

    /*!
     *  \brief         ����������� �����
     *
     *  \return        1 - ������� ��������� �������
     */
    bool registerTag();

    /*!
     *  \brief         ����������� ���. ������
     *
     *  \return        1 - ������� ��������� �������
     */
    bool registerLedKeyring();

    /*!
     *  \brief         ����������� �� ������
     *
     *  \return        1 - ������� ��������� �������
     */
    bool registerLcdKeyring();

    /*!
     *  \brief         ����������� R6
     *
     *  \return        true - ������� ��������� �������
     */
    bool registerR6();

    /*!
     *  \brief         ����������� R6 ECO
     *
     *  \return        ������ �����������
     */
    bool registerR6Eco();

    /*!
     *  \brief         �������� ��������� �� R6
     *
     *  \return        ��������� �� R6
     */
    R6 *getR6();

    /*!
     *  \brief         �������� ��������� �� R6Eco
     *
     *  \return        ��������� �� R6Eco
     */
    R6Eco *getR6Eco();

    /*!
     *  \brief         ��������� ������� SLP
     *
     *  \param  [in]   mode - ��������� �� ���������, ���� �� �������
     *
     *  \return        1 - ������ ������������� ����������, 0 - ���
     */
    bool isServiceModeOn(bool mode);

    /*!
     *  \brief         ��������� ��������
     *
     *  \param  [in]   pin        - ���, � �������� ����������� �������
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� ����� �������� 
     *  \param  [in]   pulse_time - ����� �������� ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitPulse(DigitalIn *pin,
                   uint32_t   count,
                   bool       polarity,
                   float      time,
                   float      pulse_time,
                   bool       error_flag = false,
                   float      error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         �������� ��������
     *
     *  \param  [in]   pin          - ���, �� ������� �������� �������
     *  \param  [in]   count        - ���������� ���������
     *  \param  [in]   polarity     - ���������� ���������
     *  \param  [in]   pulse_time_s - ����� ��������� � ��������
     *  \param  [in]   interval_s   - ����� ����� ���������� � ��������
     */
    void sendPulse(DigitalOut *pin,
                   uint32_t    count,
                   bool        polarity,
                   float       pulse_time_s,
                   float       interval_s);

    /*!
     *  \brief         ���������� �����
     */
    void resetTag();

    /*!
     *  \brief         �������� ������������ ������
     *
     *  \return        �����, ��������������� ������������ ������
     */
    int32_t getModules();

    /*!
     *  \brief         �������� DTMF �������
     *
     *  \param  [in]   to     - ����
     *  \param  [in]   msg    - ��������� ���������
     *  \param  [in]   lenght - ����� ���������
     *
     *  \return        true - �������� ������ �������, false - ���
     */
    bool sendDtmf(const char *to,
                  const char *msg,
                  uint16_t    lenght);

    /*!
     *  \brief         ����� �������� �������������
     *
     *  \param  [in]   state - ���/����
     */
    void bypassLearn(bool state);

    /*!
     *  \brief         �������� ��� ����������� ������
     *
     *  \param  [in]  &pin - ��� ����������� ������ � ���� ������
     */
    void addDevicePin(const string &pin);

    /*!
     *  \brief         �������� ��� ����������� ������
     *
     *  \return        pin ��� ����������� ������ � ���� ������
     */
    char *getDevicePin();

    /*!
     *  \brief         �������� �������� ��� ����������� ������
     *
     *  \detailed      ��� ������������� � ������, ��� ����� ������� �������� ��� ����������� ������
     *
     *  \param  [in]   pin_num - ����� ����� ����, ������� ����� �������� (����. ��� ���� 1111 � ������ 3 - ����� ��������� ��� 1121)
     *
     *  \return        �������� pin ��� ����������� ������ � ���� ������
     */
    char *getWrongDevicePin(emergency_dis_pin_num_t pin_num = EMERGENCY_DIS_PIN_NUM_ALL);

    /*!
     *  \brief         �������� ��
     *
     *  \param  [in]   model       - ��������� ���������
     *  \param  [in]   timeout_s   - ����� �������� �� ��������� (���� ��������� ������, �� FAIL)
     *  \param  [in]   max_error_s - ������������ ����������� ���������
     *
     *  \return        bool
     *  \retval        true        - �������� ��������
     *  \retval        false       - ��������� ������
     */
    bool checkProgramNeutral(int8_t model,
                             float  timeout_s   = TIME_DELAY::NULL_DELAY_S,
                             float  max_error_s = TIME_DELAY::MAX_NEUTRAL_ON_DOOR_TOUT_ERROR_S);

    /*!
     *  \brief         �������� ��
     *
     *  \param  [in]   model - ��������� ���������
     */
    void checkSupportIgn(int8_t model);
        
    /*!
     *  \brief         �������� ��
     *
     *  \param  [in]   model - ��������� ���������
     */
    void checkTurboTimer(int8_t model);
        
    /*!
     *  \brief         �������� ������
     *
     *  \param  [in]   model   - ��������� ���������
     *  \param  [in]   timeout - ����� �������� ���������
     */
    void checkAlarm(int8_t model, float timeout = TIME_DELAY::STD_ALARM_SIGNAL_TIME_S);

    /*!
     *  \brief         �������� ��� ���������� (�������� ������� ������ ��� �����������)
     *
     *  \return        ������ �� ������ � ����� ����������
     */
    const std::vector<can_sts_btn_t>& getValidatorCode();

    /*!
     *  \brief         �������� �������� ��� ���������� (��� ��������� �����������)
     *
     *  \return        ������ �� ������ � ����� ����������
     */
    const std::vector<can_sts_btn_t>& getWrongValidatorCode();

    /*!
     *  \brief         �������� ���� ���������� (������� ������ ����������)
     */
    void sendValidatorCode();

    /*!
     *  \brief         �������� ��������� ���� ���������� (������� ������ ����������) (��� ��������� �����������)
     */
    void sendWrongValidatorCode();

    /*!
     *  \brief         ����������� ������� ������
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - �� ������
     */
    bool registerValidatorCode();

    /*!
     *  \brief         ����������� ���� ���������
     *
     *  \param  [in]   reg_queue_mode - ������������ �� ������� �� ����������� (�������� � ������� ��� ���������������� ��� �������)
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - �� ������
     */
    virtual bool registerAll(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         �������� �������� ��������� �����
     *
     *  \param  [in]   firmware     - ��� ��������
     *  \param  [in]   time         - ����� ��������
     *  \param  [in]   from_dropbox - �� Dropbox
     *  \param  [in]  *file_path    - ���� �� �����
     *
     *  \return        1 - ������, 0 - ���
     */
    bool loadFirmware(const char *firmware,
                      float       time         = TIME_DELAY::TEN_MINUTE,
                      bool        from_dropbox = false,
                      const char *file_path    = NULL);

    /*!
     *  \brief         �� ������� ������ ������� ������ ������ �������� ��������
     *
     *  \param  [in]  *buf - ������ � �����������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������� �������
     *  \retval        false - ������ ��� �������� ��������
     */
    bool parseFirmwareLoadError(const char *buf);

    /*!
     *  \brief         �������� �������� ��������� ����� � ������� apps.starline.ru
     *
     *  \param  [in]   fw_download_type - ��� ����������� ������ ��
     *  \param  [in]   timeout_s        - ����� �������� ������
     *  \param  [in]  *special_fw_ver   - ��������� �� ������ � ������ ������� ��. ���� ������ �� �������, �� ���������� ������ ����� ����������� ������������ ���
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - ���
     */
    bool loadFirmwareFromServer(fw_download_type_t  fw_download_type = FW_DOWNLOAD_TYPE_SECOND_PREV,
                                float               timeout_s        = TIME_DELAY::MAX_WAITING_FW_TIME_S,
                                const char         *special_fw_ver   = NULL);

    /*!
     *  \brief         �������� ��������� ������ �������� ��������� �����
     *
     *  \param  [in]   timeout_s - ����� �������� ������
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - ���
     */
    bool loadLastFirmware(float timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         �������� �������� � ����������
     *
     *  \param  [in]  *ver       - ������ �������� (����.: 2.15.0.5037)
     *  \param  [in]   timeout_s - ����� �������� ������
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - ���
     */
    bool loadFirmwareByVersion(const char *version, float timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         �������� ������ ��������
     *
     *  \param  [in]  *version   - ����� ��� ������ ������
     *  \param  [in]   type      - ������ ����� �������� (�������, �������� ���� ��� ��������� ��������)
     *  \param  [in]   timeout_s - ����� �������� ������
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - ���
     */
    bool getFirmwareVersion(char     *version,
                            fw_type_t type      = FW_TYPE_CURRENT,
                            float     timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         ������, �������� �� ������� �������� Hot
     *
     *  \param  [in]   time - ����� �������� ������
     *
     *  \return        fw_type_t
     *  \retval        FW_TYPE_UNKNOWN - ���������� (������� ��������� ������)
     *  \retval        FW_TYPE_HOT     - ������ Hot
     *  \retval        FW_TYPE_NOT_HOT - ������ �� Hot
     */
    fw_type_t checkHotFirmware(float time = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         ����������� �������� ����������
     *
     *  \param  [in]   time - ����� �������� ������
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - ���
     */
    bool resetFirmware(float time = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         ��������� �������������� �������� CAN
     *
     *  \param  [in]   can_car_num_str - ����� ���������� CAN (������)
     *  \param  [in]   timeout_s       - ����� �������� ������
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - ��������
     */
    bool loadCanFirmware(const char *can_car_num_str, float timeout_s = TIME_DELAY::MAX_WAITING_FW_TIME_S);

    /*!
     *  \brief         ���������� slave

     *  \param  [in]   lights             - ����� ������������
     *  \param  [in]   state              - ���������� (���./����.)
     *  \param  [in]   lock_pulse_time_s  - ����� ��������� �� � ��������
     *  \param  [in]   light_pulse_time_s - ����� ��������� ������������ � ��������
     *  \param  [in]   interval_s         - ���������� �������, � ������� �������� ������������ ��������
     */
    void analogSlave(slave_lights_t lights,
                     bool           state,
                     float          lock_pulse_time_s,
                     float          light_pulse_time_s,
                     float          interval_s = TIME_DELAY::FIVE_S);

    /*!
     *  \brief         �������� �� ����������������� Shield-a
     *
     *  \param  [in]   time  - ����� �������� ������ � ��������
     *
     *  \return        bool
     *  \retval        true  - ������� ��������
     *  \retval        false - ��������� ��������
     */
    bool checkShield(float time = 9.0f);

    /*!
     *  \brief         �������� ������� ���������������
     *
     *  \param  [in]   status - ��������� ������ (���./����.)
     *  \param  [in]   time   - ����� �������� ������ � ��������
     *
     *  \return        buzzer_status_t
     *  \retval        BUZZER_NONE - �� ����������
     *  \retval        BUZZER_ON   - �������
     *  \retval        BUZZER_OFF  - ��������
     */
    buzzer_status_t waitBuzzerStatus(bool status, float time);

    /*!
     *  \brief         ���������� �������� ���. ������
     *
     *  \param  [in]   state - ���./����.
     */
    void setLedKeyringVcc(bool state);

    /*!
     *  \brief         ���������� �������� �� ������
     *
     *  \param  [in]   state   - ���./����.
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setLcdKeyringVcc(bool  state,
                          float delay_s = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         �������� ������ ���������� �������
     *
     *  \param  [in]   execution_method - ����� ����������
     *
     *  \return        ������������� ����� ����������
     */
    command_type_t getExecutionMethod(command_type_t  execution_method,
                                      action_type_t   action_type,
                                      DigitalOut     *pin = NULL);

    /*!
     *  \brief         ��������� ��������
     *
     *  \param  [in]   value - ��������
     */
    void setSpeed(int8_t value);

    /*!
     *  \brief         �������� ��������
     *
     *  \param  [in]   speed_kmph - �������� �������� (��/�)
     *  \param  [in]   timeout_s  - ������������ ����� �������� (������)
     *
     *  \retval        bool
     *  \return        true       - �������� ������������� ����������
     *  \return        false      - �������� �� ������������� ����������
     */
    bool checkSpeed(int8_t speed_kmph, float timeout_s = TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    /*!
     *  \brief         ������ ��������������
     *
     *  \return        ������
     */
    ahj_status_t getAntiHijacking();

    /*!
     *  \brief         �������� ��������� ������ ��������������
     *
     *  \param  [in]   status  - ��������� ������
     *  \param  [in]   timeout - ������������ ����� ��������
     *
     *  \return        ������
     */
    bool waitAntiHijacking(ahj_status_t status,
                           float        timeout = TIME_DELAY::NULL_DELAY_S);
    
    /*!
     *  \brief         �������� ������������� ������� � ��������� ���������
     *
     *  \param  [in]   timeout - ������������ ����� ��������
     *
     *  \return        ������
     */
    bool waitLightAndAlarm(float timeout);
    
    /*!
     *  \brief         ���������� �������������� �����
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   confirmation     - �������������
     */
    void offAntiHijacking(command_type_t execution_method = EMERGENCY,
                          bool           confirmation     = true);
    
    /*!
     *  \brief         ��������� ��������������
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   confirmation     - �������������
     *  \param  [in]  *gsm_pass         - GSM ������
     *  \param  [in]   ahj_check        - �������� �������������� 
     */
    void onAntiHijacking(command_type_t  execution_method,
                         bool            confirmation      = ON,
                         const char     *gsm_pass          = NULL,
                         bool            ahj_check         = ON);

    /*!
     *  \brief         ��������� ������� ����� �����
     *
     *  \param  [in]   state - ���/����
     */
    void setCapSensor(bool state);

    /*!
     *  \brief         ��������� ��������� �������� �����
     *
     *  \param  [in]   cap_sensor_type - ��� ������������� �������
     *  \param  [in]   state           - ���/����
     */
    void setCapSensor(cap_sensor_type_t cap_sensor_type, bool state);

    /*!
     *  \brief         ��������� ������� ������� ����� �����
     *
     *  \param  [in]   polarity   - ���������� ��������
     *  \param  [in]   duration_s - ������������ ��������
     */
    void sendCapSensorPulse(bool polarity, float duration_s);

    /*!
     *  \brief         ������ (�����������) �� ������� ��������� ����
     *
     *  \param  [in]   cap_sensor_type - ��� �������
     *  \param  [in]   count           - ���������� �������
     *  \param  [in]   press_time_s    - ������������ �������
     */
     void pushCapSensor(cap_sensor_type_t cap_sensor_type,
                        int8_t            count        = COUNT_STRUCT::MIN_CAP_SENSOR_CLICKS_COUNT,
                        float             press_time_s = TIME_DELAY::STD_CAP_SENSOR_PRESSED_TIME_S);

     /*!
      *  \brief        �������� ������� ���������� �������
      *
      *  \param  [in]  cap_sensor_type - ��� �������
      *  \param  [in]  state           - ��������� ���������
      *  \param  [in]  timeout_s       - ����� �������� �������
      *
      *  \retval       bool
      *  \return       true  - ������ � ��������� ���������
      *  \retval       false - ������ �� � ��������� ���������
      */
     bool waitCapSensor(cap_sensor_type_t cap_sensor_type,
                        bool              state,
                        float             timeout_s = TIME_DELAY::WAITING_REGION_STATE_S);

    /*!
     *  \brief         ���������� � ������ ��� ���������� ������ ����
     *
     *  \param  [in]   polarity      - ���������� ��������
     *  \param  [in]   p1_duration_s - ������������ ������� ��������
     *  \param  [in]   p2_duration_s - ������������ ������� ��������
     *  \param  [in]   p3_duration_s - ������������ �������� ��������
     */
    void setArmBeachMode(bool  polarity,
                         float p1_duration_s,
                         float p2_duration_s,
                         float p3_duration_s);

    /*!
     *  \brief         ������ � ������ ��� ���������� ������ ����
     *
     *  \param  [in]   polarity              - ���������� ��������
     *  \param  [in]   duration_s            - ������������ ��������
     *  \param  [in]  *pin                   - ��� ����������� ������
     *  \param  [in]   pin_num_count         - ���������� ���� ���-����, ������� ����� �������
     *  \param  [in]   wait_last_signal_flag - ������� �� ������� ����������� ����� ����� ��������� ����� ����
     */
    void setDisarmBeachMode(bool        polarity,
                            float       duration_s,
                            const char *pin                   = NULL,
                            uint8_t     pin_num_count         = 4,
                            bool        wait_last_signal_flag = true);

    /*!
     *  \brief         ���������� ��� ����������� ������ �� ������
     *
     *  \param  [in]  *pin     - ��� ����������� ������ (������)
     *  \param  [out] *pin_arr - ��� ����������� ������ (�������� ������)
     *
     *  \return        bool
     *  \retval        true    - ������� �������������
     *  \retval        false   - ������ �������������
     */
    bool getPinFromChar(const char *pin, uint8_t *pin_arr);

    /*!
     *  \brief         ��������� �������� ������������ ��� ��������� Slave
     *
     *  \param  [in]   polarity           - ����������
     *  \param  [in]   delay_light_r      - ������������ ������� ������� �����������
     *  \param  [in]   delay_light_l      - ������������ ������� ������ �����������
     *  \param  [in]   lights             - ��� ������������
     */                            
    void sendSlaveLightsPulses(bool           polarity,
                               float          delay_light_r, 
                               float          delay_light_l, 
                               slave_lights_t lights);

    /*!
     *  \brief         ��������� �������� (�������� ��� ������ �� ���������� Slave)
     *
     *  \param  [in]   delay_between      - ������������ ����� ����������
     *  \param  [in]   need_open          - ������������� �������� ��������� ��� �����
     */                            
    void doSlaveAct(float           delay_between, 
                    slave_actions_t need_open = SLAVE_ACT_NONE);

    /*!
     *  \brief         ��������� �������� ��� ������ �� ���������� Slave
     *
     *  \param  [in]   value              - ��������
     *  \param  [in]   delay_clock        - ������������ �������� ��
     *  \param  [in]   delay_between      - ������������ ����� ����������
     *  \param  [in]   delay_light_r      - ������������ ������� ������� �����������
     *  \param  [in]   delay_light_l      - ������������ ������� ������ �����������
     *  \param  [in]   lights             - ��� ������������
     *  \param  [in]   need_open          - ������������� �������� ��������� ��� �����
     *  \param  [in]   light_before_clock - �������� ������������ �� ��
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
     *  \brief         �������� ��������� ��, ����� � ����� ����� ������/���������� � ������
     *
     *  \param  [in]   indication      - ��������� ��� ���������
     *  \param  [in]   time_s          - ����� ��������
     *  \param  [in]   time_error_s    - �����������
     *  \param  [in]   lock_indication - �������� �������� �� (true - �������)
     *
     *  \return        bool
     *  \retval        true         - ��� �������� ��������
     *  \retval        false        - �������� �� ��� ��������
     */
    bool waitClockAndSignal(arm_disarm_indication_t indication,
                            float                   time_s,
                            float                   time_error_s    = TIME_DELAY::NO_ERROR_TIME_S,
                            bool                    lock_indication = true);

    /*!
     *  \brief         �������� ���� ������
     *
     *  \param  [in]   day_no - ����� ��� ������
     *
     *  \return        ������
     */
    string getWeekDay(uint8_t day_no);

    /*!
     *  \brief         �������� ��������� �����
     *
     *  \param  [out] &sys_time - ��������� �����
     *
     *  \return        bool
     *  \retval        true  - ����� �������� �������
     *  \retval        false - ����� �� ��������
     */
    bool getSysTime(time_t &sys_time);

    /*!
     *  \brief         �������� ������������� GSM
     *
     *  \detailed      ��������� ������������������ ��� ��������� ���������� �������, ��������� �� �������� ��-���������
     *
     *  \return        bool
     *  \retval        true  - ������������� �������
     *  \retval        false - ������������� ��������� ��� ������� ����� ��������� �� �������� ������� ��-���������
     */
    bool waitGsmTime(float timeout_s = TIME_DELAY::MAX_GSM_INIT_WAIT_TIME_S);

    /*!
     *  \brief         �������� � ��������
     *
     *  \param  [in]   time_s - ����� ��������
     */
    void wait_s(float time_s);

    /*!
     *  \brief         �������� ������ �� ����� ����
     *
     *  \param  [in]   log_label - ����� ����, ������� ��������� ��� ������������ ��������
     *  \param  [out] *str       - ���������� ������
     *  \param  [in]   len       - ����� ������
     *
     *  \return        bool
     *  \retval        true  - ������ �������� �������
     *  \retval        false - ������ �� ��������
     */
    bool getLogLabelStr(log_label_t  log_label,
                        char        *str,
                        uint32_t     len = LEN_STRUCT::MAX_STR_LOG_LABEL_LEN);

    /*!
     *  \brief         ��������� �������� �������� ��������
     *
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� ����� �������� 
     *  \param  [in]   pulse_time - ������������ ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */

    bool waitLightsPulse(int32_t count,
                         bool    polarity,
                         float   time,
                         float   pulse_time,
                         bool    error_flag = false,
                         float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� �������������� �������� ��������
     *
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� ����� �������� 
     *  \param  [in]   pulse_time - ������������ ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitAltLightPulse(int32_t count,
                           bool    polarity,
                           float   time,
                           float   pulse_time,
                           bool    error_flag = false,
                           float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� "�� ������� ����� ��������"
     *
     *  \param  {in]   count      - ���������� ���������
     *  \param  {in]   polarity   - ���������� ���������
     *  \param  {in]   time       - ����� ����� �������� 
     *  \param  {in]   pulse_time - ������������ ��������
     *  \param  {in]   error_flag - ���������� �� ��������� �����������
     *  \param  {in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitUnlockDriverDoor(int32_t count,
                              bool    polarity,
                              float   time,
                              float   pulse_time,
                              bool    error_flag = false,
                              float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� ��������������� ��
     *
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� �������� ���������
     *  \param  [in]   pulse_time - ������������ ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitAltLockPulse(int32_t count,
                          bool    polarity,
                          float   time,
                          float   pulse_time,
                          bool    error_flag = false,
                          float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� ������
     *
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� ����� �������� 
     *  \param  [in]   pulse_time - ������������ ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitSirenPulse(int32_t count,
                        bool    polarity,
                        float   time,
                        float   pulse_time,
                        bool    error_flag = false,
                        float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� ��������
     *
     *  \param  [in]   count        - ���������� ���������
     *  \param  [in]   polarity     - ���������� ���������
     *  \param  [in]   time_s       - ����� ����� �������� 
     *  \param  [in]   pulse_time_s - ������������ ��������
     *  \param  [in]   error_flag   - ���������� �� ��������� �����������
     *  \param  [in]   error_s      - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitKlaxonPulse(int32_t count,
                         bool    polarity,
                         float   time_s,
                         float   pulse_time_s,
                         bool    error_flag = false,
                         float   error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� ��������� �������
     *
     *  \param  [in]   count      - ���������� ���������
     *  \param  [in]   polarity   - ���������� ���������
     *  \param  [in]   time       - ����� ����� �������� 
     *  \param  [in]   pulse_time - ������������ ��������
     *  \param  [in]   error_flag - ���������� �� ��������� �����������
     *  \param  [in]   pulse_time - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitBuzzerExtPulse(int32_t count,
                            bool    polarity,
                            float   time,
                            float   pulse_time,
                            bool    error_flag = false,
                            float   error      = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� "������� �����"
     *
     *  \param  {in]   count        - ���������� ���������
     *  \param  {in]   polarity     - ���������� ���������
     *  \param  {in]   time         - ����� ����� �������� 
     *  \param  {in]   pulse_time_s - ������������ ��������
     *  \param  {in]   error_flag   - ���������� �� ��������� �����������
     *  \param  {in]   error_s      - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitHoodOpenPulse(int32_t count,
                           bool    polarity,
                           float   time,
                           float   pulse_time_s,
                           bool    error_flag = false,
                           float   error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ��������� �������� "������� �����"
     *
     *  \param  {in]   count        - ���������� ���������
     *  \param  {in]   polarity     - ���������� ���������
     *  \param  {in]   time         - ����� ����� �������� 
     *  \param  {in]   pulse_time_s - ������������ ��������
     *  \param  {in]   error_flag   - ���������� �� ��������� �����������
     *  \param  {in]   error_s      - �������� �����������
     *
     *  \return        ������ (1 - ��� �������� ��������)
     */
    bool waitHoodClosePulse(int32_t count,
                            bool    polarity,
                            float   time,
                            float   pulse_time_s,
                            bool    error_flag = false,
                            float   error_s    = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         �������� ��������� �����
     *
     *  \param  [in]   dig_pin  - ���, � �������� ����������� ���������
     *  \param  [in]   polarity - ����������
     *
     *  \return        bool
     *  \retval        true  - ������� ��������
     *  \retval        false - ��������� ��������
     */
    bool checkDigIn(PinName dig_pin, channel_polarity_t polarity);

    /*!
     *  \brief         ���������� ��������� ��������� ������
     *
     *  \param  [in]   dig_pin - ���, � �������� �������� ���������
     *  \param  [in]   state   - �������� ����������
     *
     *  \return        bool
     *  \retval        true    - ��������� ������ �����
     *  \retval        false   - �������� ������
     */
    bool setDigOut(PinName dig_pin, bool state);
    
    /*!
     *  \brief         �������� ��������
     *
     *  \param  [in]   command_type - ��� ������� ����������
     *  \param  [in]   delay_s      - �������� ����� �������� �������
     *  \param  [in]   repeat_mode  - ��������� �� �� ������ (�������� �� SLP, �������� 3 �������, ������ ��� �������)
     *
     *  \return        bool
     *  \retval        true         - ������� ����������
     *  \retval        false        - ������� �� ����������
     */
    bool unlockTrunk(command_type_t command_type,
                     float          delay_s      = TIME_DELAY::NULL_DELAY_S,
                     repeat_mode_t  repeat_mode  = REPEAT_MODE_OFF);

    /*!
     *  \brief         ���������� ���������� ��� �������� ������������ �������� ���������� ����������
     */
    void setPolaritySrvBtn();

    /*!
     *  \brief         �������� ������������ �������� ���������� ����������
     *
     *  \param  [in]   pulse_time_s - ������������ ��������
     *  \param  [in]   timeout_s    - �������
     *  \param  [in]   error_time_s - �������� ����������� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkPulseWidthSrvBtn(float pulse_time_s,
                               float timeout_s,
                               float error_time_s);

    /*!
     *  \brief         ���������� ���������� ��� �������� ������������ �������� �� ������ � ������ �����������������
     *
     *  \param  [in]   gen6_functional - ���������������� �������
     */
    void setPolarity(gen6_functional_t gen6_functional);
    
    /*!
     *  \brief         �������� ������������ �������� �� ������� � ������ �����������������
     *
     *  \param  [in]   gen6_functional - ���������������� �������
     *  \param  [in]   pulse_time_s    - ������������ ��������
     *  \param  [in]   timeout_s       - �������
     *  \param  [in]   error_time_s    - �������� ����������� ��������
     *
     *  \return        bool
     *  \retval        true            - ������������ �������� ������������� ����������
     *  \retval        false           - ������������ �������� �� ������������� ����������
     */
    bool checkPulseWidth(gen6_functional_t gen6_functional,
                         float             pulse_time_s,
                         float             timeout_s,
                         float             error_time_s = TIME_DELAY::MIN_PULSE_ERROR_S);  

    /*!
     *  \brief         �������� ������� �������� �� �������
     *
     *  \param  [in]   gen6_functional - ���������������� �������
     *  \param  [in]   timeout_s       - ������� �������� ��������
     *
     *  \return        bool
     *  \retval        true            - ������ �������
     *  \retval        false           - ������� �� �������
     */
    bool checkPulseWithoutWidth(gen6_functional_t gen6_functional,
                                float             timeout_s);                      

    /*!
     *  \brief         ���������� ������� ������
     *
     *  \param  [in]   method - ������ ���������
     */
    void setPanic(panic_method_t method);

    /*!
     *  \brief         ���������� ���������� "�������������� �� �����"
     *
     *  \param  [in]   state - ���/����
     *
     *  \return        bool
     *  \retval        true  - �������� ��������� ���������
     *  \retval        false - �������� ������ ��� ��������� ���������
     */
    bool setAhjTagSetting(bool state);

    /*!
     *  \brief         ��������/��������� ����� "������� ��������"
     *
     *  \param  [in]   state - ���/����
     *
     *  \return        bool
     *  \retval        true  - ��������������� ���������� ��������� ���������� ��������� ������ ��������
     *  \retval        false - ��������������� ���������� ��������� ���������� ��������� ������ �� ��������
     */
    bool setDangerParking(bool state);

    /*!
     *  \brief         ��������� ��������� ������ "������� ��������" �� �������
     *
     *  \param  [in]   state - ��������� ���������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������������� ����������
     *  \retval        false - ��������� �� ������������� ����������
     */
    bool checkDangerParking(bool state);

    /*!
     *  \brief         ��������� ��������� ������ "������� ��������" �� �������� ���������� (����. ������, ������ � �������� �������)
     *
     *  \param  [in]   state     - ����������� ���������
     *  \param  [in]   timeout_s - ����� �������� ��������
     *
     *  \return        bool
     *  \retval        true  - ��� ������� ��������
     *  \retval        false - ������� ��������������� ���������
     */
    bool checkDangerParkingSignals(bool state, float timeout_s = TIME_DELAY::MAX_WAIT_CL_SIGNALS_S);

    /*!
     *  \brief         ��������/��������� ����� ��������� �� BLE
     *
     *  \param  [in]   state - ���/����
     *
     *  \return        bool
     *  \retval        true  - ��������������� ���������� ��������� ���������� ��������� ������ ��������
     *  \retval        false - ��������������� ���������� ��������� ���������� ��������� ������ �� ��������
     */
    bool setBleSettingMode(bool state);

    /*!
     *  \brief         ��������� ��������� ������ ��������� �� BLE
     *
     *  \param  [in]   state - ��������� ��������� (���/����)
     *
     *  \return        bool
     *  \retval        true  - ��������� ������������� ����������
     *  \retval        false - ��������� �� ������������� ����������
     */
    bool checkBleSettingMode(bool state);

    /*!
     *  \brief         ���������� � �������� ������ "������� ��������"
     *
     *  \param  [in]   state       - �������� ���������
     *  \param  [in]   wait_time_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������������� ���������
     *  \retval        false - ��������� �� ������������� ���������
     */
    bool setAndCheckDangerParking(bool state, float wait_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         �������� ���������� ��������� "����� � ������"/"� ������"
     *
     *  \param  [in]   state - ���/����(ON/OFF)
     *  \param  [in]   delay - �������� ����������      
     *
     *  \return        bool
     *  \retval        true  - ����� � ������
     *  \retval        false - �������� ������
     */
    bool checkArmState(bool     state,
                       float    delay          = TIME_DELAY::THREE_S);
    
    /*!
     *  \brief         �������� �������, � ���������, ����� �� ���� ������ �� ��������
     */
    void clearSensors();
    
    /*!
     *  \brief         �������� ������� ��������� �� CAN ��� ���������� � ������
     *
     *  \param  [in]   time - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� �����
     *  \retval        false - ��������� ���� �� ������, ���� �� �����
     */
    bool checkStdOnMessages(float time = TIME_DELAY::FIVE_S);
    
    /*!
     *  \brief         �������� ������� ��������� �� CAN ��� ������ ������
     *
     *  \param  [in]   time - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� �����
     *  \retval        false - ��������� ���� �� ������, ���� �� �����
     */
    bool checkStdOffMessages(float time = TIME_DELAY::FIVE_S);
    
    /*!
     *  \brief         �������� ������� ��������� �� CAN ��� �������� ��
     *
     *  \param  [in]   time - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� �����
     *  \retval        false - ��������� ���� �� ������, ���� �� �����
     */
    bool checkLockEnabledMessages(float time = TIME_DELAY::FIVE_S);
    
    /*!
     *  \brief         �������� ������� ��������� �� CAN ��� �������� ��
     *
     *  \param  [in]   time - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� �����
     *  \retval        false - ��������� ���� �� ������, ���� �� �����
     */
    bool checkLockDisabledMessages(float time = TIME_DELAY::FIVE_S);

    /*!
     *  \brief         ���������� ������� ��� �������� ����� ����������
     *
     *  \param  [in]   value            - �������� (ON/OFF)
     *  \param  [in]   execution_method - ����� ����������
     */
    virtual void setSecureNoDelay(int8_t         value,
                                  command_type_t execution_method);
    
    /*!
     *  \brief         ������ ��������� ����� ������� ������� ���������
     *
     *  \param  [in]   value            - �������� (true/false)
     *  \param  [in]   delay_s          - �������� ����� ����������
     */
    void setKillEngine(int8_t value, float delay_s = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� ��������� ����� � ����� ����� ������/���������� � ������
     *
     *  \param  [in]   light_indication - ��������� �������� ��������� (ON/OFF)
     *  \param  [in]   siren_indication - ��������� �������� ��������� (ON/OFF)
     *  \param  [in]   time_s           - ����� ��������
     * 
     *  \return        bool
     *  \retval        true             - ��������� ������������� ������ �������
     *  \retval        false            - ��������� �� ������������� ������ �������
     */
    bool waitSirenAndLightSignal(light_status_t light_indication,
                                 siren_status_t siren_indication,
                                 float          time_s);

    /*!
     *  \brief         ������� ���������� ��� ������������� ������ ����� � ����� ����� ����� ��������� (����. ��� ������������� ����� �������� ���)
     *
     *  \param  [in]   pulse_width_s - ������������ �������� �������� (����. ��� ������ - 10 �, ��� ������� - 30 �)
     */
    bool waitLightsImpulseOrPotentialStart(float pulse_width_s);

    /*!
     *  \brief         ��������� ���������� � ������������� ���������� ������
     *
     *  \param  [in]   pulse_width_s            - ������������ �������� �������� (����. ��� ������ - 10 �, ��� ������� - 30 �). ��� 0 � - ������� �������� �� ����������
     *  \param  [in]   pulse_wait_time_s        - ����� �������� ��������� ��������
     *  \param  [in]   ligth_pulse_width_s      - ������������ ��������   (���������� ���������� ������)
     *  \param  [in]   light_pulse_pause_s      - ����� ����� ����������  (���������� ���������� ������)
     *  \param  [in]   light_potential_len_s    - ������������ ���������� (������������� ���������� ������)
     *  \param  [in]   pulse_count              - ���������� ���������    (���������� ���������� ������)
     *  \param  [in]   pulse_width_error_time_s - ����������� ������������ �������� ��������. ��� 0 � - �������������� �������������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkLightsImpulseAndPotential(float                pulse_width_s,
                                        float                pulse_wait_time_s,
                                        float                ligth_pulse_width_s,
                                        float                light_pulse_pause_s,
                                        float                light_potential_len_s,
                                        lights_pulse_count_t pulse_count              = LIGHTS_PULSE_COUNT_TWO,
                                        float                pulse_width_error_time_s = TIME_DELAY::NULL_DELAY_S);    

    /*!
     *  \brief         ������ �������� ����������� � ��
     *
     *  \param  [in]   temperature - �������� �����������
     */
    void setTemperatureValue(temperature_celsius_t temperature);

    /*!
     *  \brief         ���������� �������, ��� ������� ����� �� ������
     */
    void disableHumanInCar();

    /*!
     *  \brief         ��������� ����� � ���������� ��������� ������� Human in Car ��� ����, ����� �� �������� ����� ������ � ������
     *
     *  \return        bool
     *  \retval        true  - �������� �������� �������
     *  \retval        false - ������ ��� ���������� ���������
     */
    bool setTagAndDisableHumanInCar();

    /*!
     *  \brief         ������� ����� �� ������������� ������
     */
    void activateTag();

    /*!
     *  \brief         ���������� r6eco ����� SLM
     *
     */
    void updateR6ecoFw();

    /*!
     *  \brief         ������ ������
     *
     *  \return        ������
     */
    int8_t getOutHood();

    /*!
     *  \brief         ������ �� (�������)
     *
     *  \return        ������
     */
    int8_t getLockOpen();

    /*!
     *  \brief         ������ �� (�������)
     *
     *  \return        ������
     */
    int8_t getLockClose();

    /*!
     *  \brief         ����� �� �������
     */
    void resetGen6Power();

    /*!
     *  \brief         �������� ��������� ����������� � ������� �������
     *
     *  \param  [in]   ign_status - ������ ��������� ����� �����������
     */
    void resetAuthAndImmo(ign_statuses_t ign_status = IGN_OFF);

    /*!
     *  \brief         ����������� ������� ������ � �������
     *
     *  \param  [in]   req_queue_mode - ������������ �� ������� �� ����������� (�������� � ������� ��� ���������������� ��� �������)
     *
     *  \return        bool
     *  \retval        true  - ������
     *  \retval        false - �� ������
     */
    bool registerBtnAndRemote(registration_queue_mode_t reg_queue_mode = REGISTRATION_QUEUE_MODE_EN);

    /*!
     *  \brief         �������� ��������� ������ ������� �����
     *
     *  \param  [in]   status_out          - ����� ���������� ������
     *  \param  [in]   time_s              - ����� ��������
     *  \param  [in]   status_out_state    - ����������� ��������� ON/OFF
     *
     *  \return        bool
     *  \retval        true    - ��������� ������ ������� ����� ������������� ����������
     *  \retval        false   - ��������� ������ ������� ����� ������������� ����������
     */                          
    bool waitStatusOut(status_out_t status_out,
                       float        time_s,
                       bool         status_out_state);

    /*!
     *  \brief         ��������/���������� ����� � �������� ������� ���������
     *
     *  \param  [in]   tag_mode  - ����� ������ ����� (OFF/ON)
     *
     *  \return        bool
     *  \retval        true      - ������ ��������� ����� ������������� � ���������
     *  \retval        false     - ������ ��������� ����� �� ������������� � ���������
     */                          
    bool setAndCheckTag(bool tag_mode);

    /*!
     *  \brief         ��������/���������� ������ � �������� ������� ������
     *
     *  \param  [in]   secure_mode      - ����� ������  (OFF/ON)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   arm_state        - ��������� ��������� ������
     *  \param  [in]   expect_state     - ��������� ��������� ������
     *  \param  [in]   check_time_s     - ����� �������� ������� ������
     *
     *  \return        bool
     *  \retval        true             - ������ ������ ������������� � ���������
     *  \retval        false            - ������ ������ �� ������������� � ���������
     */                          
    bool setAndCheckSecure(bool                     secure_mode,
                           command_type_t           execution_method,
                           slp_diag_data_statuses_t arm_state,
                           bool                     expect_state     = ON,
                           float                    check_time_s     = TIME_DELAY::MAX_GUARD_STATE_EN_TIME_S);

    /*!
     *  \brief         ���������� � �������� ���������
     *
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   delay_s          - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true             - ������ ��������� ������������� � ���������
     *  \retval        false            - ������ ��������� �� ������������� � ���������
     */
    bool setAndCheckIgn(int8_t         ign_value,
                        command_type_t execution_method,
                        float          delay_s           = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ������ � ��������� ������ �������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay_s          - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true             - ������ ������� ������������� ��� ���������
     *  \retval        false            - ������ ������� �� ������������� ��� ���������
     */
    bool setAndCheckBreak(int8_t         value,
                          command_type_t execution_method,
                          float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ������ ��������� 1
     *
     *  \return        ������
     */
    int8_t getIgn1();

    /*!
     *  \brief         ������ ������ �����
     *
     *  \param  [in]   in_tag_status        - ��� ����� ������� ������� �� ���������
     *
     *  \return        ������
     */
    int8_t checkStatusIn(DigitalIn *in_tag_status);

    /*!
     *  \brief         �������� ��������� ���������� ������
     *
     *  \param  [in]   in_tag_status       - ��� ����� ������� ������� �� ���������
     *  \param  [in]   gen6_functional_t   - ���������� ����������� �������
     *  \param  [in]   status_out_state    - ����������� ��������� ON/OFF
     *  \param  [in]   waiting_time_s      - ����� ��������
     *
     *  \return        bool
     *  \retval        true                - ������ ���������� ������ ������������� � ���������
     *  \retval        false               - ������ ���������� ������ �� ������������� � ���������
     */
    bool waitStateStatusOut(DigitalIn          *in_tag_status,
                            gen6_functional_t   status_out,
                            bool                status_out_state,
                            float               waiting_time_s    = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief        ������� ������� ������ 3 ������
     *
     *  \param push_type_first  - ������������ ������� �������
     *  \param push_type_second - ������������ ������� �������
     */
    void pushLcdKeyringBut33(float push_type_first, 
                             float push_type_second);

    /*!
     *  \brief         ���������� � �������� ������� �������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   hand_break_value    - �������� (���/����)
     *  \param  [in]   delay               - �������� ����� ����������
     *  \param  [in]   waiting_time_s      - ����� �������� ������� � �������
     *
     *  \return        bool
     *  \retval        true                - ������ ������� ������������� ��� ���������
     *  \retval        false               - ������ ������� �� ������������� ��� ���������
     */
    bool setAndCheckHandBreak(int8_t         hand_break_value,
                              command_type_t execution_method,
                              float          delay             = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                              float          waiting_time_s    = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ���������� � �������� ������� ������� �� CAN
     *
     *  \param  [in]   hand_break_value    - �������� (���/����)
     *  \param  [in]   delay               - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true                - ������ ������� ������������� ��� ���������
     *  \retval        false               - ������ ������� �� ������������� ��� ���������
     */
    bool setAndCheckHandBreakCan(int8_t hand_break_value,
                                 float  delay             = TIME_DELAY::ONE_S);

    /*!
     *  \brief         ���������� � �������� ��������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   hand_break_value    - �������� (���/����)
     *  \param  [in]   delay               - �������� ����� ����������
     *  \param  [in]   waiting_time_s      - ����� �������� ������� � �������
     *
     *  \return        bool
     *  \retval        true                - ������ �������� ������������� � ���������
     *  \retval        false               - ������ �������� �� ������������� � ���������
     */
    bool setAndCheckParking(int8_t         parking_value,
                            command_type_t execution_method,
                            float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                            float          waiting_time_s    = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ���������� � �������� �������� �� CAN
     *
     *  \param  [in]   hand_break_value    - �������� (���/����)
     *  \param  [in]   delay               - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true                - ������ �������� ������������� ��� ���������
     *  \retval        false               - ������ �������� �� ������������� ��� ���������
     */
    bool setAndCheckParkingCan(int8_t parking_value,
                               float  delay_s        = TIME_DELAY::ONE_S);

    /*!
     *  \brief         ���������� � �������� ���������� ���������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   hand_break_value    - �������� (���/����)
     *  \param  [in]   delay_s             - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true                - ������ ��������� ������������� ��� ���������
     *  \retval        false               - ������ ��������� �� ������������� ��� ���������
     */
    bool setAndCheckEngine(int8_t         engine_value,
                           command_type_t execution_method,
                           int16_t        rpm_val           = RPM_ON,
                           float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ���������� � �������������� �� �����
     *
     *  \param  [in]   secure_method       - ����� ��������� ������
     *  \param  [in]   ign_method          - ����� ��������� ���������
     *  \param  [in]   engine_method       - ����� ������� ���������
     *  \param  [in]   hand_break_method   - ����� ���������� �������
     *  \param  [in]   parking_method      - ����� ���������� ��������
     *
     *  \return        bool
     *  \retval        true                - ���������� ������ � ��������� ��������� 
     *                                       �������������� �� �����
     *  \retval        false               - �������������� �� ����� ����������
     */
    bool prepAhjTag(command_type_t secure_method     = TAG,
                    command_type_t ign_method        = PIN,
                    command_type_t engine_method     = PIN,
                    command_type_t hand_break_method = PIN,
                    command_type_t parking_method    = PIN);

    /*!
     *  \brief         ���������� �������� ���������� � �������������� (���� ���� ����� - �� �����, ���� ��� - �� ������)
     *
     *  \param  [in]   door_method    - ����� �������� �����
     *
     *  \return        bool
     *  \retval        true           - �������� ��� �������������� �� ����� ���������
     *  \retval        false          - �������� ��� �������������� �� ����� �� ���������
     */
    bool doAhj(command_type_t door_method = PIN);

    /*!
     *  \brief         �������� ��� ������� ���������� ��������� (151) � �������� ������ FiFo
     *
     *  \param  [in]   time_s         - ����� ��������
     *
     *  \return        bool
     *  \retval        true           - ������ ��������� ��������
     *  \retval        false          - ������ ���������� �� ��������
     */
    bool waitEngineBlockCan(float time_s);

    /*!
     *  \brief         ��������� �������� �����\��������\������� � ������������ ���������
     *
     *  \param  [in]   sensor_type     - �������� �������
     *  \param  [in]   delay_s         - ����� ��������
     *
     */
    void setAccelerometerAfterDelay(sensor_type_t sensor_type,
                                    float         delay_s     = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         ���������� � �������� ���������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   trunk_value         - �������� (���/����)
     *  \param  [in]   logic_state         - ������ ������ (���������/����������)
     *  \param  [in]   delay_s             - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true                - ������ ��������� ������������� ��� ���������
     *  \retval        false               - ������ ��������� �� ������������� ��� ���������
     */
    bool setAndCheckTrunk(int8_t           trunk_value,
                          command_type_t   execution_method,
                          invers_logic_t   logic_state      = NORMAL,
                          float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ��������� ��������� ���������
     *
     *  \param  [in]   status           - ��������� ���������
     *  \param  [in]   execution_method - ����� ��� ��������� (SLP, CAN � �.�.)
     *  \param  [in]   timeout_s        - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������� ������������� ���������� ���������
     *  \retval        false - ������ ��������� �� ������������� ���������� ���������
     */
    bool checkTrunk(bool           status,
                    command_type_t execution_method = SLP,
                    float          timeout_s        = TIME_DELAY::MAX_WIRE_STATUS_EN_TIME_S);

    /*!
     *  \brief         �������� ���������� ���� �� CAN
     *
     *  \param  [in]   timeout_s - ����� �������� ����������
     *
     *  \return        bool
     *  \retval        true  - ������� ���������� ��������
     *  \retval        false - ������� ���������� �� ��������
     */
    bool waitGearboxAutoBlock(float timeout_s = TIME_DELAY::GEARBOX_AUTO_BLOCK_TIME_S);

    /*!
     *  \brief         �������� ������������� ��� (����) �� CAN
     *
     *  \param  [in]   timeout_s - ����� �������� �������������
     *
     *  \return        bool
     *  \retval        true  - ������� ������������� ��������
     *  \retval        false - ������� ������������� ����������
     */
    bool waitGearboxAutoUnblock(float timeout_s = TIME_DELAY::GEARBOX_AUTO_BLOCK_TIME_S);

    /*!
     *  \brief         ���������� � �������� �����
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   door_value          - �������� (���/����)
     *  \param  [in]   logic_state         - ������ ������ (���������/����������)
     *  \param  [in]   delay_s             - �������� ����� ����������
     *  \param  [in]   wait_time_s         - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true                - ������ ��������� ������������� ��� ���������
     *  \retval        false               - ������ ��������� �� ������������� ��� ���������
     */
    bool setAndCheckDoor(int8_t           door_value,
                         command_type_t   execution_method,
                         invers_logic_t   logic_state      = NORMAL,
                         float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                         float            wait_time_s      = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ���������� � �������� �������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   hood_value          - �������� (���/����)
     *  \param  [in]   logic_state         - ������ ������ (���������/����������)
     *  \param  [in]   delay_s             - �������� ����� ����������
     *
     *  \return        bool
     *  \retval        true                - ������ ��������� ������������� ��� ���������
     *  \retval        false               - ������ ��������� �� ������������� ��� ���������
     */
    bool setAndCheckHood(int8_t           hood_value,
                         command_type_t   execution_method,
                         invers_logic_t   logic_state      = NORMAL,
                         float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� ��������� ���������� ������
     *
     *  \param  [in]   Buzzer_state        - ����������� ��������� ON/OFF
     *  \param  [in]   waiting_time_s      - ����� ��������
     *
     *  \return        bool
     *  \retval        true                - ������ ���������� ������ ������������� � ���������
     *  \retval        false               - ������ ���������� ������ �� ������������� � ���������
     */
    bool waitBuzzerExt(bool  buzzer_state,
                       float waiting_time_s = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������ ���������������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   delay_s          - �������� ����� ���������� 
     */
    virtual void setDoorInvPin(int8_t value,
                               float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������� ���������������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   delay_s          - �������� ����� ���������� 
     */
    virtual void setHoodInvPin(int8_t value,
                               float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ���������� ���������������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   delay_s          - �������� ����� ���������� 
     */
    virtual void setTrunkInvPin(int8_t value,
                                float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������ ������� ������� ���������������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   delay_s          - �������� ����� ���������� 
     */
    virtual void setKillEngineInv(int8_t value,
                                  float  delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������ ��������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay_s          - �������� ����� ����������
     */
    virtual void setDriverDoor(int8_t         value,
                               command_type_t execution_method,
                               float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������ ���������
     *
     *  \param  [in]   value            - �������� (���/����)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   delay_s          - �������� ����� ����������
     */
    virtual void setPassengerDoor(int8_t         value,
                                  command_type_t execution_method,
                                  float          delay_s           = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� � �������� ����� �������� ��� ���������
     *
     *  \param  [in]   execution_method    - ����� ����������
     *  \param  [in]   door_value          - �������� (���/����)
     *  \param  [in]   door_name           - �������� ����� ������� ���������, ���������
     *  \param  [in]   delay_s             - �������� ����� ����������
     *  \param  [in]   wait_time_s         - ����� �������� ������� � �������
     *
     *  \return        bool
     *  \retval        true                - ������ ����� ������������� � ���������
     *  \retval        false               - ������ ����� �� ������������� � ���������
     */
    bool setAndCheckSpecialDoor(int8_t           door_value,
                                command_type_t   execution_method,
                                special_door_t   door_name,
                                float            delay_s          = TIME_DELAY::STD_WIRE_EN_DELAY_S,
                                float            wait_time_s      = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         �������� ������� ������������ � �������
     *
     *  \param  [in]   tt_value            - ��������� ��
     *  \param  [in]   wait_time_s         - ����� �������� ������� � �������
     *
     *  \return        bool
     *  \retval        true                - ������ TT ������������� � ���������
     *  \retval        false               - ������ TT �� ������������� � ���������
     */
    bool waitTurboTimer(bool  tt_value,
                        float wait_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ��������� �� CAN ����� � ������� �������
     *
     *  \param  [in]   state - ��������� �������: ���/����
     */
    void setStdAlertMercedesCla(bool state);

    /*!
     *  \brief         ���������� ��������� �� Renault Captur (CAN)
     *
     *  \param  [in]   status  - ��������� ���������
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setIgnRenaultCaptur(bool status, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ��������� �� Renault Captur (CAN)
     *
     *  \param  [in]   value   - �������� ��������
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setRpmRenaultCaptur(rpm_statuses_t value, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ������ ������� �� ������������� ������ �� Renault Captur (CAN)
     *
     *  \param  [in]   delay_s - �������� ����� �������
     */
    void setRemoteRunRenaultCaptur(float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);

    /*!
     *  \brief         �������� �������� Lexus RX (������ Start-Stop) 2016-2018
     *
     *  \param  [in]   timeout_s - ����� ��������
     */
    bool waitKlaxonLexusRx(float timeout_s = TIME_DELAY::ALARM_EN_TIME_S);

    /*!
     *  \brief         �������� ��������� ������� � ������ � ���������� ����������
     *
     *  \param  [in]   arm_supp_trig    - ������� ��������� ��������� ��������� ��� �������� � ������ � ���������� ����������
     *  \param  [in]   execution_method - ����� ���������� � ������
     *
     *  \return        bool
     *  \retval        true             - �������� �������� � ������ � ���������� ���������� ��������
     *  \retval        false            - �������� �������� � ������ � ���������� ���������� �� ��������
     */
    bool setArmSuppIgn(arm_supp_trig_t arm_supp_trig,
                       command_type_t  execution_method = KEYRING);

    /*!
     *  \brief         �������� ��������� ��������� ������ ����� ������������ ������ �� ��������
     *
     *  \param  [in]   sensor_type - ��� �������
     *  \param  [in]   arm_state   - ��������� ��������� ������
     *  \param  [in]   delay_s     - �������� ����� ���������� �������
     *
     *  \return        bool
     *  \retval        true        - ��������� ������ ���������� �������� ��������� ����� ������������ �������
     *  \retval        false       - ��������� ������ �� ���������� �������� ��������� ����� ������������ �������
     */
    bool checkAlertStateAfterAccel(sensor_type_t            sensor_type,
                                   slp_diag_data_statuses_t arm_state,
                                   float                    delay_s);

    /*!
     *  \brief         ������� ��� ���������� ���� ������ �� SLP
     *
     *  \return        bool
     *  \retval        true  - �������� ������ � ������
     *  \retval        false - ������ �� �������
     */
    bool resetDiagDataPerim();

    /*!
     *  \brief         ���������� ������� ������ ������
     *
     *  \param  [in]   status  - �������� ��������� (���. / ����.)
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setGlowPlugs(out_statuses_t status, float delay_s = TIME_DELAY::MIN_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������� ������ ������ � �������� ������� �� SLP
     *
     *  \param  [in]   status  - �������� ��������� (���. / ����.)
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    bool setAndCheckGlowPlugs(out_statuses_t status, float timeout_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ���������, ������������� �� �������� ��������� ����, ��� ��������� � ��
     *
     *  \param  [in]   setting_name  - ��� ��������� (id �� xml, ����.: outs_ctrl_doors_lock_duration_ms)
     *  \param  [in]   setting_value - ��������� �������� ���������
     *  \param  [in]   setting_type  - ��� ��������� (������������� ��� ���������)
     *
     *  \return        bool
     *  \retval        true  - �������� ��������� ������������� ����������
     *  \retval        false - �������� ��������� �� ������������� ����������
     */
    bool checkSettingValue(const char     *setting_name,
                           const char     *setting_value,
                           setting_type_t  setting_type);

    /*!
     *  \brief         �������� ����������/�������� ������ (������ CAN)
     *
     *  \param  [in]   mirrors_status - ��������� ������ (�������/������� �������)
     *  \param  [in]   timeout_s      - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
    bool waitSideMirrorsCan(mirrors_statuses_t mirrors_status, float timeout_s = TIME_DELAY::COMFORT_EN_TIME_S);

    /*!
     *  \brief         �������� ���������/���������� ������� ������� (CAN)
     *
     *  \param  [in]   status    - ��������� ������ (���./����.)
     *  \param  [in]   timeout_s - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
     bool waitComfortCan(out_statuses_t status, float timeout_s = TIME_DELAY::COMFORT_EN_TIME_S);

    /*!
     *  \brief         �������� ��������/�������� ���� �� ����� ��� ���������� Lexus GX 2019-2020
     *
     *  \param  [in]   status    - ��������� ������ (���./����.)
     *  \param  [in]   timeout_s - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
     bool waitSunroofCanLexusGx(doors_statuses_t sunroof_status, float timeout_s = TIME_DELAY::COMFORT_EN_TIME_S);

    /*!
     *  \brief         �������� ������� ��������� CAN
     *
     *  \param  [in]   timeout_s - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
    bool waitInspectorCan(float timeout_s);

    /*!
     *  \brief         �������� ��������� �������� OBD ������
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
    bool waitObdErrorRequest(float timeout_s);

    /*!
     *  \brief         ���������� ����� ���������� ���������� � �������� ������
     *
     *  \return        bool
     *  \retval        true  - ����� �����������
     *  \retval        false - ����� �� �����������
     */
    bool connectMobile();

    /*!
     *  \brief         ��������� ����� ���������� ���������� � �������� ������
     *
     *  \return        bool
     *  \retval        true  - ����� ���������
     *  \retval        false - ����� �� ���������
     */
    bool disconnectMobile();

    /*!
     *  \brief         �������� ����� ��������� ������
     *
     *  \return        bool
     *  \retval        true  - ������� �������
     *  \retval        false - ������� ���������
     */
    bool clearMobileBuf();

    /*!
     *  \brief         ��������� ������� ��������� ������� ����� ��������� ���������� (BLE) (�� ���������������� ������ MOBILEAPP_DIAG_PACKAGE_BLE_CUR_STATE)
     *
     *  \param  [in]   shift     - �������� ��� ���������������� ������
     *  \param  [in]   status    - ��������� ������ (��. ��������������� ������� � SlpCmd.h)
     *  \param  [in]   read_type - ��� ������ ���������������� ������ (��������� ������ �� ��������� ������ ��� ��������� ������, ����������� � ������ ������)
     * 
     *  \return        bool
     *  \retval        true  - ��������� ������ �������
     *  \retval        false - ��������� ������ �� �������
     */
    bool getMobileStatusCurState(mobileapp_cur_state_shift_t shift,
                                 int32_t                     status,
                                 mobileapp_read_type_t       read_type = MOBILEAPP_READ_FROM_CMD);

    /*!
     *  \brief        ��������� ������� ��������� ����� ����� ��������� ���������� (BLE)
     *
     *  \param  [in]  command - ���������������� ������� ��� ����������
     *
     *  \return       bool
     *  \retval       true  - ������� ���������
     *  \retval       false - ������� �� ���������
     */
    bool setMobileStatus(mobileapp_user_cmd_t command);

    /*!
     *  \brief        ��������� ��������� ������� ����� ��������� ���������� (BLE)
     *
     *  \param  [in]  status - �������� ������ (���./����.)
     *
     *  \return       bool
     *  \retval       true  - ������� ���������
     *  \retval       false - ������� �� ���������
     */
    bool setServiceModeMobile(out_statuses_t status);

    /*!
     *  \brief         ��������� �������� ����� � ����� ����� ���� �����. ������ ����� �������������� ��������� ������
     *
     *  \param  [in]   alt_srv_btn_type - ��� �������������� ��������� ������
     */
    void setAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         �������� � ��������� ������ ����� �������������� ��������� ������
     *
     *  \param  [in]   alt_srv_btn_type - ��� �������������� ��������� ������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setAndCheckAltSrvBtnArm(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         �������� � ��������� ����� �������������� ��������� ������
     *
     *  \description   �� �������� ��� ���� ������� ������, �.�. �������� �����������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setAndCheckAltSrvBtnModeEn(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         ������� �������������� ��������� ������
     *
     *  \param  [in]   alt_srv_btn_type - ��� �������������� ��������� ������
     *  \param  [in]   count            - ���������� �������
     *
     *  \return        bool
     *  \retval        true  - ������� �������
     *  \retval        false - ������� ���������
     */
    bool clickAltSrvBtn(alt_srv_btn_type_t alt_srv_btn_type,
                        uint8_t            count);

    /*!
     *  \brief         ���������� ������ � ������ � ������ �������������� ��������� ������
     *
     *  \detailed      ������� ��� ������ ���������� � ������ �������������� ��������� ������
     *                 ����� ��� ����������, ����� �� �������� ��� ��� ��� � �� ������ ����� ����������� �������� �������
     *                 ���� ������ ������ ��� - ��� �������� ������� ������ ���� ��������
     *                 ���� ������ �������� ��� - �������� �������� ������� �� ���� ������ ���� � ����������� � ���������
     *
     *  \param  [in]   alt_srv_btn_type - ��� �������������� ��������� ������
     *  \param  [in]  *pin              - ��� ����������� ������
     *
     *  \return        bool
     *  \retval        true  - ��� ������� ����� ��������
     *  \retval        false - �� ��� ������� ����� ��������
     */
    bool setAltSrvBtnDisarm(alt_srv_btn_type_t  alt_srv_btn_type,
                            const char         *pin               = NULL);

    /*!
     *  \brief         ��������� ���������� ������ � ������������ \"����� ���������� �������� ����\"
     *
     *  \detailed      ������������� ���������� ����������� ���������� (��� ��������� V1 ��� V2)
     *
     *  \return        ����������
     */
    bool getSlpPolarity();

    /*!
     *  \brief         ��������� �������� ���������� ��������� ����� ����. �����������
     *
     *  \detailed      ����� ����� ��� ������ ��������� �������� ����. ��������� (������ �������)
     *                 ����� ������ ����������� ����� ����������� ����������� (�.�. ������ ����� ���� ������������ ����� �������)
     *
     *  \param  [in]   immo_delay_s - ��������� ����� �������� ����. ��������� ����� ����. �����������
     *  \param  [in]   err_time_s   - �����������
     *  \param  [in]   timeout_s    - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkImmoDisDelayAfterAcc(float immo_delay_s,
                                   float err_time_s,
                                   float timeout_s);

    /*!
     *  \brief         ������� ������ ��� �������
     *
     *  \detailed      ������� ������ ������� ��� ���������� ������ �� ������ ��� ������ � ������
     *                 ����� ����� ����������� ������ ��������
     *
     *  \param  [in]   slp_open_type - ������� �������� ������� (������/������)
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - ���������� ��������
     */
    bool openDebugRegion(slp_open_type_t slp_open_type = SLP_OPEN_TYPE_RW);

    /*!
     *  \brief         ������� ������ ��� ������ �������
     *
     *  \detailed      ����� ��������� �� ���������� ��������, ������� �������� �������
     *                 ���� ������� ������ ����� ���������� �������� - �� ������� �� ���� ����� ���� ���������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - ���������� ��������
     */
    bool openEventsRegion();

    /*!
     *  \brief         ������� ������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - ���������� ��������
     */
    bool closeRegion();

    /*!
     *  \brief         �������� ����������� �������
     *
     *  \param  [in]   timeout_s - ����� �������� ����������� �������
     *  \param  [in]  &event_ids - ���� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - ���� �� ������� ���������
     *  \retval        false - �� ���� �� ������� �� ���������
     */
    bool waitSlpEvents(const std::vector<slp_user_event_t> &event_ids, float timeout_s);

    /*!
     *  \brief         ���������� ������� ������ ���������
     *
     *  \param  [in]   value            - �������� ���������
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   logic_type       - ��� ������ ������ ������� (�������/���������)
     *  \param  [in]   delay_s          - �������� ����� ����������
     */
    void setDismantlingSensor(out_statuses_t value,
                              command_type_t execution_method,
                              invers_logic_t logic_type        = NORMAL,
                              float          delay_s           = TIME_DELAY::MAX_WIRE_EN_DELAY_S);

    /*!
     *  \brief         ���������� ������� ������ ��������� � �������� ��� ��������� (�� SLP)
     *
     *  \param  [in]   value            - �������� ���������
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   logic_type       - ��� ������ ������ ������� (�������/���������)
     *  \param  [in]   wait_time_s      - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setAndCheckDismantlingSensor(out_statuses_t value,
                                      command_type_t execution_method,
                                      invers_logic_t logic_type        = NORMAL,
                                      float          wait_time_s       = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ��������� ������������� ������ ���������� ���������� � ������
     *
     *  \param  [in]   pulses_count - ��������� ���-�� ���������
     *  \param  [in]   timeout_s    - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkSrvLedAndSiren(uint8_t pulses_count, float timeout_s);

    /*!
     *  \brief         ���������� ������� ������� CAN
     *
     *  \param  [in]   value   - �������� ���������
     *  \param  [in]   delay_s - �������� ����� ����������
     */
    void setCanExtBtnAudiA7(out_statuses_t value, float delay_s = TIME_DELAY::STD_WIRE_EN_DELAY_S);


    /*!
     *  \brief         ���������� ������� ������� CAN � �������� �� ��������� (�� SLP)
     *
     *  \param  [in]   value       - �������� ���������
     *  \param  [in]   wait_time_s - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    void setAndCheckCanExtBtnAudiA7(out_statuses_t value, float wait_time_s = TIME_DELAY::MIN_WAITING_REGION_STATE_S);

    /*!
     *  \brief         �������� ������� ������ ���������
     *
     *  \param  [in]   status    - ��������� ������ (��������� ��������/��������)
     *  \param  [in]   timeout_s - ����� �������� �������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������ �������
     *  \retval        false - ��������� ������ �� �������
     */
    bool waitEngineStatus(out_statuses_t status, float timeout_s = TIME_DELAY::WAITING_REGION_STATE_S);

    /*!
     *  \brief         ������ ��������� ����� ��������� ���������� (BLE)
     *
     *  \param  [in]  &settings - ������ �� ���������� ���������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������
     *  \retval        false - ��������� �� ������
     */
    bool setSettingsMobile(const SlmSettings &settings);

    /*!
     *  \brief         ��������� � �������� ��������� ����� ��������� ���������� (BLE)
     *
     *  \param  [in]  &settings - ������ �� ����������� ���������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������������� ���������
     *  \retval        false - ��������� �� ������������� ���������
     */
    bool checkSettingsMobile(const SlmSettings &settings);

};

#endif // _GEN6_BASIC_H_
