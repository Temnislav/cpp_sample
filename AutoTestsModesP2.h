 /*!
 *  \file      AutoTestsModesP2.cpp
 *
 *  \brief     Содержит реализацию класса, включающего кейсы Режимы работы (часть 2)
 *
 *  \detailed  Предназначен для тестирования устройств 6-го поколения
 *
 */

#ifndef _AUTO_TESTS_MODES_P2_H_
#define _AUTO_TESTS_MODES_P2_H_

//! Подключение заголовочных файлов

#include "AutoTestsModesBase.h"
#include "AutoTestsTelematics.h"

//! Типы и классы

/*!
 *  \brief    Класс AutoTestsModesP2
 *   
 *  \detailed Содержит функционал для тестирования устройств 6-го поколения
 */
class AutoTestsModesP2 : public AutoTestsModesBase
{

//! \privatesection
private:

    /*!
     *  \brief Тип проверки тревоги, сколько времени проверять
     */
    typedef enum
    {
        ALARM_CHECK_TYPE_START, //!< Только начало (получение сигналов света/сирены)
        ALARM_CHECK_TYPE_HALF,  //!< 0,5 цикла
        ALARM_CHECK_TYPE_FULL   //!< Полный цикл
    } alarm_check_type_t;

    /*!
     *  \brief Различные действия для проверки алгоритма блокировки АКПП (для метода setStateAndCheckGearboxBlock)
     */
    typedef enum
    {
        ACT_TYPE_NO_ACT,     //!< Ничего не выполнять
        ACT_TYPE_DISARM,     //!< Снять с охраны
        ACT_TYPE_PARKING_ON, //!< Включить паркинг
        ACT_TYPE_AUTH        //!< Авторизоваться
    } act_types_t;

    /*!
     *  \brief Хранит номера шагов, на которых можно пройти авторизацию в алгоритме "Ожидание авторизации" (для метода setZoneAndCheckAuth)
     */
    typedef enum
    {
        AUTH_STEP_START, //!< На начальных условиях (до первого шага, т.е. не выключать устройство авторизации)
        AUTH_STEP_ONE,   //!< На первом шаге
        AUTH_STEP_TWO,   //!< На втором шаге
        AUTH_STEP_THREE  //!< На третьем шаге
    } auth_step_t;

    /*!
     *  \brief Тип активации АО (для методов resetTagOrRemoteAndCheckAhj, setAhjAndCheckSms)
     */
    typedef enum
    {
        AHJ_TYPE_TAG_OFF,   //!< По выключению метки
        AHJ_TYPE_REMOTE_OFF //!< По выключению брелока
    } ahj_type_t;

    /*!
     *  \brief Типы блокировок в режиме "Антиограбление" (для метода setAhjAndCheckBlock)
     */
    typedef enum
    {
        AHJ_BLOCK_TYPE_INSTANT_OFF, //!< Блокировка выключена (не включается сразу после Шага 4 АО)
        AHJ_BLOCK_TYPE_INSTANT_ON,  //!< Блокировка включается сразу после Шага 4 АО
        AHJ_BLOCK_TYPE_DELAYED_OFF, //!< Блокировка не включается в течение и через 180 секунд (внутренний тайминг) (TIME_DELAY::MAX_AHJ_BLOCK_DELAY_S) после Шага 4 АО
        AHJ_BLOCK_TYPE_DELAYED_ON   //!< Блокировка включается с задержкой 180 секунд (внутренний тайминг) (TIME_DELAY::MAX_AHJ_BLOCK_DELAY_S) после Шага 4 АО
    } ahj_block_type_t;

    /*!
     *  \brief Статусы скорости (скорость включена/выключена) (для метода setImmoAndSensorAndCheckBlock)
     */
    typedef enum
    {
        CAR_SPEED_OFF, //!< Скорость выключена
        CAR_SPEED_ON   //!< Скорость включена
    } car_speed_type_t;

    /*!
     *  \brief Тип управления паркингом
     */
    typedef enum
    {
        PARKING_TYPE_CAN,   //!< По CAN
        PARKING_TYPE_ANALOG //!< По аналогу
    } parking_type_t;

    /*!
     *  \brief Тип кода экстренного снятия (для метода setAndCheckAltSrvBtnDisarm)
     */
    typedef enum
    {
        SRV_BTN_PIN_TYPE_WRONG,  //!< Неверный код экстренного снятия
        SRV_BTN_PIN_TYPE_CORRECT //!< Верный код экстренного снятия
    } srv_btn_pin_type_t;

    /*!
     *  \brief Способы включения режима настройки по BLE (для метода setAndCheckBleSettingsMode)
     */
    typedef enum
    {
        BLE_SETTINGS_MODE_EN_TYPE_SRV_BTN,        //!< Через сервисную кнопку
        BLE_SETTINGS_MODE_EN_TYPE_SRV_BTN_NO_IGN, //!< Через сервисную кнопку без использования зажигания
        BLE_SETTINGS_MODE_EN_TYPE_SMS             //!< Через SMS
    } ble_settings_mode_en_type_t;

    /*!
     *  \brief Порядок регистрации кодовых реле в охранный комплекс
     */
    typedef enum
    {
        RELAY_REGISTRATION_SEQUENCE_R6_ONLY,       //!< Только R6
        RELAY_REGISTRATION_SEQUENCE_R6ECO_ONLY,    //!< Только R6 ECO
        RELAY_REGISTRATION_SEQUENCE_R6_THEN_R6ECO, //!< R6, затем R6 ECO
        RELAY_REGISTRATION_SEQUENCE_R6ECO_THEN_R6  //!< R6 ECO, затем R6
    } relay_registration_sequence_t;

    /*!
     *  \brief Типы обновления ПО
     */
    typedef enum
    {
        FW_UPDATE_FROM_PREV_TO_CURR,   //!< Обновление с предыдущей на текущую версию
        FW_UPDATE_FROM_CURR_TO_PREV,   //!< Обновление с текущей на предыдущую версию
        FW_UPDATE_FROM_SPECIAL_TO_CURR //!< Обновление с особой на текущую версию
    } fw_update_type_t;

    /*!
     *  \brief Триггеры для включения и проверки блокировки кнопкой Старт-Стоп (для метода setBlockTriggerAndCheckStartStopBtnBlock)
     */
    typedef enum
    {
        START_STOP_BLOCK_TRIGGER_IGN_ON,          //!< Включить зажигание
        START_STOP_BLOCK_TRIGGER_DOOR_OPEN,       //!< Открыть дверь
        START_STOP_BLOCK_TRIGGER_PARKING_OFF,     //!< Выключить паркинг
        START_STOP_BLOCK_TRIGGER_SENSOR_MOVE,     //!< Вызвать срабатывание ДД
        START_STOP_BLOCK_TRIGGER_SENSOR_SHOCK_LOW //!< Вызвать срабатывание ДУ: тревожный
    } start_stop_block_trigger_t;

    /*!
     *  \brief Состояния сервисного режима
     */
    typedef enum
    {
        SERVICE_NO_CONTROL, //!< Не управлять состоянием сервисного режима
        SERVICE_OFF,        //!< Выключить
        SERVICE_ON          //!< Включить
    } service_statuses_t;

    static AutoTestsModesP2 *s_at_modes_p2;                              //!< Указатель на класс

    Sim900                  *p_gsm;                                      //!< Указатель на Sim900
    vector<const char *>     m_shield_phones;                            //!< Номера телефонов GSM Shield-a
    char                     m_curr_fw_ver[LEN_STRUCT::STD_STR_BUF_LEN]; //!< Текущая версия ПО в виде строки

    int8_t  m_ppo_type;                        //!< Тип ППП
    int8_t  m_ppo_on_before_autostart;         //!< Включение ППП перед АЗ
    int8_t  m_is_tag_rem_for_trunk_needed;     //!< Разрешить в охране открытие багажника при наличии метки или брелока
    float   m_doors_imit_time_s;               //!< Время имитации дверей
    float   m_doors_imit_delay_s;              //!< Задержка имитации дверей после глушения двигателя
    float   m_trunk_tag_rem_wait_time_s;       //!< Время поиска метки и брелока при открытом багажнике
    float   m_eng_block_local0_delay_s;        //!< Задержка блокировки
    float   m_hf_arm_after_zone_open_delay_s;  //!< Постановка после открытия двери или багажника (свободные руки)
    float   m_hf_arm_after_zone_close_delay_s; //!< Постановка после закрытия двери или багажника (свободные руки)
    float   m_immo_on_after_ign_off_delay_s;   //!< Время до активации запрета поездки после выключения зажигания
    float   m_eng_btn_block_time_s;            //!< Длительность импульса на кнопку старт-стоп (блокировка двигателя)

    int8_t                          m_engine_block_type;    //!< Тип блокировки двигателя
    eng_start_gearbox_type_t        m_gearbox_type;         //!< Тип КПП
    eng_start_neutral_detect_type_t m_program_neutral_type; //!< Тип постановки на ПН

    SlmSettings                           *p_c168671_master_stg;
    options_t                             *p_c168671;
    
    SlmSettings                           *p_c168672_master_stg;
    options_t                             *p_c168672;
    
    SlmSettings                           *p_c168685_master_stg;
    options_t                             *p_c168685;
    
    SlmSettings                           *p_c168686_master_stg;
    options_t                             *p_c168686;
    
    SlmSettings                           *p_c168687_master_stg;
    options_t                             *p_c168687;
    
    SlmSettings                           *p_c168688_master_stg;
    options_t                             *p_c168688;
    
    SlmSettings                           *p_c168695_master_stg;
    options_t                             *p_c168695;
    
    SlmSettings                           *p_c168696_master_stg;
    options_t                             *p_c168696;
    
    SlmSettings                           *p_c168703_master_stg;
    options_t                             *p_c168703;

    SlmSettings                           *p_c168707_master_stg;
    options_t                             *p_c168707;
    
    SlmSettings                           *p_c168714_master_stg;
    options_t                             *p_c168714;
    
    SlmSettings                           *p_c168723_master_stg;
    options_t                             *p_c168723;
    
    SlmSettings                           *p_c170781_master_stg;
    options_t                             *p_c170781;
    
    SlmSettings                           *p_c170782_master_stg;
    options_t                             *p_c170782;
    
    SlmSettings                           *p_c170784_master_stg;
    options_t                             *p_c170784;
    
    SlmSettings                           *p_c170792_master_stg;
    options_t                             *p_c170792;
    
    SlmSettings                           *p_c170799_master_stg;
    options_t                             *p_c170799;
    
    SlmSettings                           *p_c170800_master_stg;
    options_t                             *p_c170800;
    
    options_t                             *p_c170808;
    
    SlmSettings                           *p_c170821_master_stg;
    options_t                             *p_c170821;

    SlmSettings                           *p_c171184_master_stg;
    options_t                             *p_c171184;
    
    SlmSettings                           *p_c169641_master_stg;
    options_t                             *p_c169641;
    
    SlmSettings                           *p_c169642_master_stg;
    options_t                             *p_c169642;
    
    SlmSettings                           *p_c169643_master_stg;
    options_t                             *p_c169643;
    
    SlmSettings                           *p_c169644_master_stg;
    options_t                             *p_c169644;
    
    SlmSettings                           *p_c169645_master_stg;
    options_t                             *p_c169645;
    
    SlmSettings                           *p_c169648_master_stg;
    options_t                             *p_c169648;
    
    SlmSettings                           *p_c169656_master_stg;
    options_t                             *p_c169656;
    
    SlmSettings                           *p_c1578122_master_stg;
    options_t                             *p_c1578122;
    
    SlmSettings                           *p_c169657_master_stg;
    options_t                             *p_c169657;
    
    SlmSettings                           *p_c169659_master_stg;
    options_t                             *p_c169659;
    
    SlmSettings                           *p_c1578123_master_stg;
    options_t                             *p_c1578123;
    
    SlmSettings                           *p_c169639_master_stg;
    options_t                             *p_c169639;
    
    SlmSettings                           *p_c169640_master_stg;
    options_t                             *p_c169640;
    
    SlmSettings                           *p_c1616162_master_stg;
    options_t                             *p_c1616162;
    
    SlmSettings                           *p_c1616163_master_stg;
    options_t                             *p_c1616163;
    
    SlmSettings                           *p_c1616164_master_stg;
    options_t                             *p_c1616164;
    
    SlmSettings                           *p_c1624514_master_stg;
    options_t                             *p_c1624514;
    
    SlmSettings                           *p_c1624516_master_stg;
    options_t                             *p_c1624516;
    
    SlmSettings                           *p_c1624517_master_stg;
    options_t                             *p_c1624517;
    
    SlmSettings                           *p_c1624518_master_stg;
    options_t                             *p_c1624518;

    SlmSettings                           *p_c172480_master_stg;
    options_t                             *p_c172480;

    SlmSettings                           *p_c172498_master_stg;
    options_t                             *p_c172498;

    SlmSettings                           *p_c172473_master_stg;
    options_t                             *p_c172473;

    SlmSettings                           *p_c2043655_master_stg;
    options_t                             *p_c2043655;

    SlmSettings                           *p_c2043657_master_stg;
    options_t                             *p_c2043657;

    SlmSettings                           *p_c2043659_master_stg;
    options_t                             *p_c2043659;

    SlmSettings                           *p_c2043661_master_stg;
    options_t                             *p_c2043661;

    SlmSettings                           *p_c1981106_master_stg;
    options_t                             *p_c1981106;

    SlmSettings                           *p_c1981107_master_stg;
    options_t                             *p_c1981107;

    SlmSettings                           *p_c1981112_master_stg;
    options_t                             *p_c1981112;

    SlmSettings                           *p_c1981115_master_stg;
    options_t                             *p_c1981115;

    SlmSettings                           *p_c1981121_master_stg;
    options_t                             *p_c1981121;

    SlmSettings                           *p_c1981125_master_stg;
    options_t                             *p_c1981125;

    SlmSettings                           *p_c1981136_master_stg;
    options_t                             *p_c1981136;
    
    SlmSettings                           *p_c2405376_master_stg;
    options_t                             *p_c2405376;

    options_t                             *p_c1827097;

    SlmSettings                           *p_c2405378_master_stg;
    options_t                             *p_c2405378;

    SlmSettings                           *p_c2405379_master_stg;
    options_t                             *p_c2405379;

    SlmSettings                           *p_c2424871_master_stg;
    options_t                             *p_c2424871;

    SlmSettings                           *p_c2424871_2_master_stg;
    options_t                             *p_c2424871_2;

    SlmSettings                           *p_c2424873_master_stg;
    options_t                             *p_c2424873;

    SlmSettings                           *p_c2424875_master_stg;
    options_t                             *p_c2424875;

    SlmSettings                           *p_c2424876_master_stg;
    options_t                             *p_c2424876;

    SlmSettings                           *p_c2438277_master_stg;
    options_t                             *p_c2438277;

    SlmSettings                           *p_c2424877_master_stg;
    options_t                             *p_c2424877;

    SlmSettings                           *p_c2424878_master_stg;
    options_t                             *p_c2424878;

    SlmSettings                           *p_c2424879_master_stg;
    options_t                             *p_c2424879;

    SlmSettings                           *p_c2424880_master_stg;
    options_t                             *p_c2424880;

    SlmSettings                           *p_c2424881_master_stg;
    options_t                             *p_c2424881;

    SlmSettings                           *p_c2424882_master_stg;
    options_t                             *p_c2424882;

    SlmSettings                           *p_c2424883_master_stg;
    options_t                             *p_c2424883;

    SlmSettings                           *p_c2424884_master_stg;
    options_t                             *p_c2424884;

    SlmSettings                           *p_c2424885_master_stg;
    options_t                             *p_c2424885;

    SlmSettings                           *p_c2424886_master_stg;
    options_t                             *p_c2424886;

    SlmSettings                           *p_c2424887_master_stg;
    options_t                             *p_c2424887;

    SlmSettings                           *p_c2424888_master_stg;
    options_t                             *p_c2424888;

    SlmSettings                           *p_c2424889_master_stg;
    options_t                             *p_c2424889;

    SlmSettings                           *p_c2728677_master_stg;
    options_t                             *p_c2728677;

    SlmSettings                           *p_c2728678_master_stg;
    options_t                             *p_c2728678;

    SlmSettings                           *p_c2728679_master_stg;
    options_t                             *p_c2728679;

    SlmSettings                           *p_c2728682_master_stg;
    options_t                             *p_c2728682;

    SlmSettings                           *p_c2728683_master_stg;
    options_t                             *p_c2728683;

    SlmSettings                           *p_c2728684_master_stg;
    options_t                             *p_c2728684;

    SlmSettings                           *p_c172464_master_stg;
    options_t                             *p_c172464;

    SlmSettings                           *p_c1662256_master_stg;
    options_t                             *p_c1662256;

    SlmSettings                           *p_c1662261_master_stg;
    options_t                             *p_c1662261;

    SlmSettings                           *p_c169588_master_stg;
    options_t                             *p_c169588;

    SlmSettings                           *p_c2198656_master_stg;
    options_t                             *p_c2198656;

    SlmSettings                           *p_c2198680_master_stg;
    options_t                             *p_c2198680;

    SlmSettings                           *p_c2198684_master_stg;
    options_t                             *p_c2198684;

    SlmSettings                           *p_c2198685_master_stg;
    options_t                             *p_c2198685;

    SlmSettings                           *p_c169591_master_stg;
    options_t                             *p_c169591;

    SlmSettings                           *p_c169592_master_stg;
    options_t                             *p_c169592;

    SlmSettings                           *p_c169593_master_stg;
    options_t                             *p_c169593;

    options_t                             *p_c169594;

    SlmSettings                           *p_c2889927_master_stg;
    options_t                             *p_c2889927;

    SlmSettings                           *p_c2889927_no_hf_master_stg;
    options_t                             *p_c2889927_no_hf;

    SlmSettings                           *p_c3052203_master_stg;
    options_t                             *p_c3052203;

    SlmSettings                           *p_c3052206_master_stg;
    options_t                             *p_c3052206;

    SlmSettings                           *p_c2799849_master_stg;
    options_t                             *p_c2799849;

    SlmSettings                           *p_c2799850_master_stg;
    options_t                             *p_c2799850;

    options_t                             *p_c2799851;

    SlmSettings                           *p_c2799852_master_stg;
    options_t                             *p_c2799852;

    SlmSettings                           *p_c2799853_master_stg;
    options_t                             *p_c2799853;

    SlmSettings                           *p_c2799854_master_stg;
    options_t                             *p_c2799854;

    SlmSettings                           *p_c3202904_master_stg;
    options_t                             *p_c3202904;

    SlmSettings                           *p_c20461_master_stg;
    options_t                             *p_c20461;

    SlmSettings                           *p_c20462_master_stg;
    options_t                             *p_c20462;

    SlmSettings                           *p_c20463_master_stg;
    options_t                             *p_c20463;

    SlmSettings                           *p_auth_no_tag_needed_master_stg;
    options_t                             *p_auth_no_tag_needed;

    SlmSettings                           *p_c3752984_master_stg;
    options_t                             *p_c3752984;

    SlmSettings                           *p_c3752985_master_stg;
    options_t                             *p_c3752985;

    SlmSettings                           *p_c3752986_master_stg;
    options_t                             *p_c3752986;

    SlmSettings                           *p_c3752987_master_stg;
    options_t                             *p_c3752987;

    SlmSettings                           *p_c3136858_master_stg;
    options_t                             *p_c3136858;

    SlmSettings                           *p_c3688077_master_stg;
    options_t                             *p_c3688077;

    SlmSettings                           *p_no_rearm_master_stg;
    options_t                             *p_no_rearm;

    SlmSettings                           *p_c3697245_master_stg;
    options_t                             *p_c3697245;

    SlmSettings                           *p_c3688075_master_stg;
    options_t                             *p_c3688075;

    SlmSettings                           *p_c3688076_master_stg;
    options_t                             *p_c3688076;

    SlmSettings                           *p_no_auth_master_stg;
    options_t                             *p_no_auth;

    SlmSettings                           *p_c3136863_master_stg;
    options_t                             *p_c3136863;

    SlmSettings                           *p_c3136864_master_stg;
    options_t                             *p_c3136864;

    SlmSettings                           *p_c170806_master_stg;
    options_t                             *p_c170806;

    SlmSettings                           *p_c187473_master_stg;
    options_t                             *p_c187473;

    SlmSettings                           *p_c187475_master_stg;
    options_t                             *p_c187475;

    SlmSettings                           *p_c187457_master_stg;
    options_t                             *p_c187457;

    SlmSettings                           *p_c187458_master_stg;
    options_t                             *p_c187458;

    SlmSettings                           *p_c187459_master_stg;
    options_t                             *p_c187459;

    SlmSettings                           *p_c187462_master_stg;
    options_t                             *p_c187462;

    SlmSettings                           *p_c187463_master_stg;
    options_t                             *p_c187463;

    SlmSettings                           *p_c187464_master_stg;
    options_t                             *p_c187464;

    SlmSettings                           *p_c187471_master_stg;
    options_t                             *p_c187471;

    options_t                             *p_c187478;

    SlmSettings                           *p_c4045282_master_stg;
    options_t                             *p_c4045282;

    SlmSettings                           *p_c4045283_master_stg;
    options_t                             *p_c4045283;

    SlmSettings                           *p_c4045286_master_stg;
    options_t                             *p_c4045286;

    SlmSettings                           *p_c4045290_master_stg;
    options_t                             *p_c4045290;

    SlmSettings                           *p_c4045291_master_stg;
    options_t                             *p_c4045291;

    SlmSettings                           *p_c4045294_master_stg;
    options_t                             *p_c4045294;

    SlmSettings                           *p_c2898585_master_stg;
    options_t                             *p_c2898585;

    SlmSettings                           *p_c170805_master_stg;
    options_t                             *p_c170805;

    SlmSettings                           *p_can_ovl_one_dis_master_stg;
    options_t                             *p_can_ovl_one_dis;

    SlmSettings                           *p_c4695722_master_stg;
    options_t                             *p_c4695722;

    SlmSettings                           *p_c4686621_master_stg;
    options_t                             *p_c4686621;

    SlmSettings                           *p_c4695725_master_stg;
    options_t                             *p_c4695725;

    SlmSettings                           *p_c4695726_master_stg;
    options_t                             *p_c4695726;

    SlmSettings                           *p_c4559714_master_stg;
    options_t                             *p_c4559714;

    SlmSettings                           *p_c4801978_master_stg;
    options_t                             *p_c4801978;

    SlmSettings                           *p_c4801979_master_stg;
    options_t                             *p_c4801979;

    SlmSettings                           *p_c4801981_master_stg;
    options_t                             *p_c4801981;

    SlmSettings                           *p_c4828410_master_stg;
    options_t                             *p_c4828410;

    SlmSettings                           *p_c4828414_master_stg;
    options_t                             *p_c4828414;

    SlmSettings                           *p_c4559732_master_stg;
    options_t                             *p_c4559732;

    SlmSettings                           *p_c4559733_master_stg;
    options_t                             *p_c4559733;

    SlmSettings                           *p_c4559734_master_stg;
    options_t                             *p_c4559734;

    SlmSettings                           *p_c4559735_master_stg;
    options_t                             *p_c4559735;

    SlmSettings                           *p_c3357809_master_stg;
    options_t                             *p_c3357809;

    SlmSettings                           *p_c3357810_master_stg;
    options_t                             *p_c3357810;

    SlmSettings                           *p_c4360828_master_stg;
    options_t                             *p_c4360828;

    SlmSettings                           *p_c4360832_master_stg;
    options_t                             *p_c4360832;

    SlmSettings                           *p_c4360836_master_stg;
    options_t                             *p_c4360836;

    SlmSettings                           *p_c4360840_master_stg;
    options_t                             *p_c4360840;

    SlmSettings                           *p_c9257883_master_stg;
    options_t                             *p_c9257883;

    options_t                             *p_c9257885;

    SlmSettings                           *p_c9257887_master_stg;
    options_t                             *p_c9257887;
    
    SlmSettings                           *p_c9257888_master_stg;
    options_t                             *p_c9257888;
    
    SlmSettings                           *p_c9257889_master_stg;
    options_t                             *p_c9257889;
    
    SlmSettings                           *p_c9257890_master_stg;
    options_t                             *p_c9257890;

    SlmSettings                           *p_c9377438_master_stg;
    options_t                             *p_c9377438;

    SlmSettings                           *p_c3407086_master_stg;
    options_t                             *p_c3407086;

    SlmSettings                           *p_c3407087_master_stg;
    options_t                             *p_c3407087;

    SlmSettings                           *p_c9501311_master_stg;
    options_t                             *p_c9501311;

    SlmSettings                           *p_c9501313_master_stg;
    options_t                             *p_c9501313;

    SlmSettings                           *p_c9491969_master_stg;
    options_t                             *p_c9491969;

    SlmSettings                           *p_c9491974_master_stg;
    options_t                             *p_c9491974;

    SlmSettings                           *p_c9491977_master_stg;
    options_t                             *p_c9491977;

    SlmSettings                           *p_c9491979_master_stg;
    options_t                             *p_c9491979;

    SlmSettings                           *p_c9491980_master_stg;
    options_t                             *p_c9491980;

    SlmSettings                           *p_c9491981_master_stg;
    options_t                             *p_c9491981;

    SlmSettings                           *p_c9491982_master_stg;
    options_t                             *p_c9491982;

    SlmSettings                           *p_c9501309_master_stg;
    options_t                             *p_c9501309;

    SlmSettings                           *p_c9501310_master_stg;
    options_t                             *p_c9501310;

    SlmSettings                           *p_c9501320_master_stg;
    options_t                             *p_c9501320;

    SlmSettings                           *p_c9501322_master_stg;
    options_t                             *p_c9501322;

    SlmSettings                           *p_c9508185_master_stg;
    options_t                             *p_c9508185;

    SlmSettings                           *p_c9508187_master_stg;
    options_t                             *p_c9508187;

    SlmSettings                           *p_c9645313_master_stg;
    options_t                             *p_c9645313;

    SlmSettings                           *p_c9645315_master_stg;
    options_t                             *p_c9645315;

    SlmSettings                           *p_c9645320_master_stg;
    options_t                             *p_c9645320;

    SlmSettings                           *p_c9645322_master_stg;
    options_t                             *p_c9645322;

    SlmSettings                           *p_c9658745_master_stg;
    options_t                             *p_c9658745;

    SlmSettings                           *p_c9658747_master_stg;
    options_t                             *p_c9658747;

    SlmSettings                           *p_c9658752_master_stg;
    options_t                             *p_c9658752;

    SlmSettings                           *p_c9658754_master_stg;
    options_t                             *p_c9658754;

    SlmSettings                           *p_c9745928_master_stg;
    options_t                             *p_c9745928;

    SlmSettings                           *p_c9745929_master_stg;
    options_t                             *p_c9745929;

    SlmSettings                           *p_c9745930_master_stg;
    options_t                             *p_c9745930;

    SlmSettings                           *p_c9745931_master_stg;
    options_t                             *p_c9745931;

    SlmSettings                           *p_c4930044_master_stg;
    options_t                             *p_c4930044;

    SlmSettings                           *p_c4930051_master_stg;
    options_t                             *p_c4930051;

    SlmSettings                           *p_c9801189_master_stg;
    options_t                             *p_c9801189;

    SlmSettings                           *p_c9801190_master_stg;
    options_t                             *p_c9801190;

    SlmSettings                           *p_c9913925_master_stg;
    options_t                             *p_c9913925;

    SlmSettings                           *p_c9913926_master_stg;
    options_t                             *p_c9913926;

    SlmSettings                           *p_c4363738_master_stg;
    options_t                             *p_c4363738;

    SlmSettings                           *p_c4363739_master_stg;
    options_t                             *p_c4363739;

    SlmSettings                           *p_c2874561_master_stg;
    options_t                             *p_c2874561;

    SlmSettings                           *p_c2907769_master_stg;
    options_t                             *p_c2907769;

    SlmSettings                           *p_c170801_master_stg;
    options_t                             *p_c170801;

    SlmSettings                           *p_c10369958_master_stg;
    options_t                             *p_c10369958;

    SlmSettings                           *p_c10369962_master_stg;
    options_t                             *p_c10369962;

    SlmSettings                           *p_c10369966_master_stg;
    options_t                             *p_c10369966;

    SlmSettings                           *p_c2874875_master_stg;
    options_t                             *p_c2874875;

    SlmSettings                           *p_c10288872_master_stg;
    options_t                             *p_c10288872;

    SlmSettings                           *p_c10419622_master_stg;
    options_t                             *p_c10419622;

    SlmSettings                           *p_c10419629_master_stg;
    options_t                             *p_c10419629;

    SlmSettings                           *p_c10419630_master_stg;
    options_t                             *p_c10419630;

    options_t                             *p_c10645338;

    SlmSettings                           *p_c10485319_master_stg;
    options_t                             *p_c10485319;

    SlmSettings                           *p_c10485321_master_stg;
    options_t                             *p_c10485321;

    SlmSettings                           *p_c10634826_master_stg;
    options_t                             *p_c10634826;

    SlmSettings                           *p_c10861710_master_stg;
    options_t                             *p_c10861710;

    SlmSettings                           *p_c10861711_master_stg;
    options_t                             *p_c10861711;

    SlmSettings                           *p_c10861712_master_stg;
    options_t                             *p_c10861712;

    SlmSettings                           *p_c10861713_master_stg;
    options_t                             *p_c10861713;

    SlmSettings                           *p_c10854614_master_stg;
    options_t                             *p_c10854614;

    SlmSettings                           *p_c10854615_master_stg;
    options_t                             *p_c10854615;

    SlmSettings                           *p_c10854594_master_stg;
    options_t                             *p_c10854594;

    SlmSettings                           *p_c10854596_master_stg;
    options_t                             *p_c10854596;

    SlmSettings                           *p_c10854607_master_stg;
    options_t                             *p_c10854607;

    SlmSettings                           *p_c10854603_master_stg;
    options_t                             *p_c10854603;

    SlmSettings                           *p_c10832739_master_stg;
    options_t                             *p_c10832739;

    SlmSettings                           *p_c10964449_master_stg;
    options_t                             *p_c10964449;

    SlmSettings                           *p_c10964450_master_stg;
    options_t                             *p_c10964450;

    SlmSettings                           *p_c10634817_master_stg;
    options_t                             *p_c10634817;

    options_t                             *p_c10415982;

    SlmSettings                           *p_c11110395_master_stg;
    options_t                             *p_c11110395;

    SlmSettings                           *p_c11846848_master_stg;
    options_t                             *p_c11846848;

    SlmSettings                           *p_c11946743_master_stg;
    options_t                             *p_c11946743;

    x96_channels_t                        *p_ppp_func_x96;
    m96_channels_t                        *p_ppp_func_m96;
    a96_channels_t                        *p_ppp_func_a96;
    e96_channels_t                        *p_ppp_func_e96;
    s96_channels_t                        *p_ppp_func_s96;
    x96v2_channels_t                      *p_ppp_func_x96v2;
                                          
    x96_channels_t                        *p_cap_sensor_func_x96;
    m96_channels_t                        *p_cap_sensor_func_m96;
    a96_channels_t                        *p_cap_sensor_func_a96;
    e96_channels_t                        *p_cap_sensor_func_e96;
    s96_channels_t                        *p_cap_sensor_func_s96;
    x96v2_channels_t                      *p_cap_sensor_func_x96v2;
                                          
    x96_channels_t                        *p_cap_can_slave_func_x96;
    a96_channels_t                        *p_cap_can_slave_func_a96;
    e96_channels_t                        *p_cap_can_slave_func_e96;
    s96_channels_t                        *p_cap_can_slave_func_s96;
    x96v2_channels_t                      *p_cap_can_slave_func_x96v2;
                                          
    x96_channels_t                        *p_cap_analog_slave_func_x96;
    a96_channels_t                        *p_cap_analog_slave_func_a96;
    e96_channels_t                        *p_cap_analog_slave_func_e96;
    s96_channels_t                        *p_cap_analog_slave_func_s96;
    x96v2_channels_t                      *p_cap_analog_slave_func_x96v2;
                                          
    x96_channels_t                        *p_hood_analog_slave_func_x96;
    a96_channels_t                        *p_hood_analog_slave_func_a96;
    e96_channels_t                        *p_hood_analog_slave_func_e96;
    s96_channels_t                        *p_hood_analog_slave_func_s96;
    x96v2_channels_t                      *p_hood_analog_slave_func_x96v2;
                                          
    m96_channels_t                        *p_rearm_breake_func_m96;
    x96v2_channels_t                      *p_rearm_breake_func_x96v2;
    
    x96_channels_t                        *p_start_1_1_func_x96;
    m96_channels_t                        *p_start_1_1_func_m96;
    a96_channels_t                        *p_start_1_1_func_a96;
    e96_channels_t                        *p_start_1_1_func_e96;
    s96_channels_t                        *p_start_1_1_func_s96;
    x96v2_channels_t                      *p_start_1_1_func_x96v2;

    a96_channels_t                        *p_c1827097_func_a96;
    e96_channels_t                        *p_c1827097_func_e96;
    s96_channels_t                        *p_c1827097_func_s96;
    x96v2_channels_t                      *p_c1827097_func_x96v2;

    a96_channels_t                        *p_analog_can_slave_func_a96;
    x96_channels_t                        *p_analog_can_slave_func_x96;
    e96_channels_t                        *p_analog_can_slave_func_e96;
    x96v2_channels_t                      *p_analog_can_slave_func_x96v2;
    s96_channels_t                        *p_analog_can_slave_func_s96;

    e96_channels_t                        *p_hood_trunk_doors_e96;
    s96_channels_t                        *p_hood_trunk_doors_s96;

    a96_channels_t                        *p_c2198656_func_a96;
    e96_channels_t                        *p_c2198656_func_e96;
    x96v2_channels_t                      *p_c2198656_func_x96v2;
    s96_channels_t                        *p_c2198656_func_s96; 

    a96_channels_t                        *p_c2799852_func_a96;
    e96_channels_t                        *p_c2799852_func_e96;
    s96_channels_t                        *p_c2799852_func_s96; 
    x96v2_channels_t                      *p_c2799852_func_x96v2;
    x96v2_channels_t                      *p_c2799854_func_x96v2;

    a96_channels_t                        *p_no_siren_func_a96;
    e96_channels_t                        *p_no_siren_func_e96;
    x96v2_channels_t                      *p_no_siren_func_x96v2;
    s96_channels_t                        *p_no_siren_func_s96;

    a96_channels_t                        *p_c187478_func_a96;
    e96_channels_t                        *p_c187478_func_e96;
    s96_channels_t                        *p_c187478_func_s96;
    x96v2_channels_t                      *p_c187478_func_x96v2;

    a96_channels_t                        *p_c4559714_func_a96;
    e96_channels_t                        *p_c4559714_func_e96;
    s96_channels_t                        *p_c4559714_func_s96;
    x96v2_channels_t                      *p_c4559714_func_x96v2;

    x96v2_channels_t                      *p_c4360828_func_x96v2;

    a96_channels_t                        *p_c3688075_func_a96;

    a96_channels_t                        *p_c10369958_func_a96;
    e96_channels_t                        *p_c10369958_func_e96;
    s96_channels_t                        *p_c10369958_func_s96;
    x96v2_channels_t                      *p_c10369958_func_x96v2;

    r6eco_channels_t                      *p_c10288872_func_r6eco;

    x96v2_channels_t                      *p_c10419622_func_x96v2;

    a96_channels_t                        *p_c10854614_func_a96;
    e96_channels_t                        *p_c10854614_func_e96;
    s96_channels_t                        *p_c10854614_func_s96;
    x96v2_channels_t                      *p_c10854614_func_x96v2;

    a96_channels_t                        *p_c10964449_func_a96;
    e96_channels_t                        *p_c10964449_func_e96;
    s96_channels_t                        *p_c10964449_func_s96;
    x96v2_channels_t                      *p_c10964449_func_x96v2;

    a96_channels_t                        *p_video_reg_func_a96;
    e96_channels_t                        *p_video_reg_func_e96;
    s96_channels_t                        *p_video_reg_func_s96;
    x96v2_channels_t                      *p_video_reg_func_x96v2;

    /*!
     *  \brief         Конструктор класса
     */
    AutoTestsModesP2();

    /*!
     *  \brief         Конструктор копирования
     *
     *  \param  [in]  &se - Ссылка на класс AutoTestsModesP2
     */

    AutoTestsModesP2(const AutoTestsModesP2 &se);

    /*!
     *  \brief         Оператор =
     *
     *  \param  [in]  &se - Ссылка на класс AutoTestsModesP2
     */
    AutoTestsModesP2 &operator=(AutoTestsModesP2 &se);

    /*!
     *  \brief         Инициализация параметров R6 ECO
     */
    void initR6EcoWires();

    /*!
     *  \brief         Инициализация параметров X96
     */
    void initX96Wires();

    /*!
     *  \brief         Инициализация параметров M96
     */
    void initM96Wires();

    /*!
     *  \brief         Инициализация параметров A96
     */
    void initA96Wires();

    /*!
     *  \brief         Инициализация параметров E96
     */
    void initE96Wires();

    /*!
     *  \brief         Инициализация параметров S96
     */
    void initS96Wires();

    /*!
     *  \brief         Инициализация параметров X96v2
     */
    void initX96v2Wires();

    /*!
     *  \brief         Инициализация параметров X96
     */
    void initX96MasterStg();

    /*!
     *  \brief         Инициализация параметров M96
     */
    void initM96MasterStg();

    /*!
     *  \brief         Инициализация параметров A96
     */
    void initA96MasterStg();

    /*!
     *  \brief         Инициализация параметров E96
     */
    void initE96MasterStg();

    /*!
     *  \brief         Инициализация параметров S96
     */
    void initS96MasterStg();

    /*!
     *  \brief         Инициализация параметров X96v2
     */
    void initX96v2MasterStg();

    /*!
     *  \brief         Инициализация параметров X96
     */
    void initX96Settings();

    /*!
     *  \brief         Инициализация параметров M96
     */
    void initM96Settings();

    /*!
     *  \brief         Инициализация параметров A96
     */
    void initA96Settings();

    /*!
     *  \brief         Инициализация параметров E96
     */
    void initE96Settings();

    /*!
     *  \brief         Инициализация параметров S96
     */
    void initS96Settings();

    /*!
     *  \brief         Инициализация параметров X96v2
     */
    void initX96v2Settings();

    /*!
     *  \brief         Отправка настроек Мастера
     *
     *  \param  [in]  *option - настройка
     *
     *  \return        0 - неудачная загрузка параметров, 1 - удачная
     */
    int8_t settings_check(options_t *option);
    
//! \publicsection
public:

    /*!
     *  \brief         Деструктор класса
     */
    ~AutoTestsModesP2();

     /*!
     *  \brief         Получение единственного экземпляра класса
     *
     *  \return        указатель на тип класса AutoTestsModesP2
     */
    static AutoTestsModesP2 *getInstance();

    /*!
     *  \brief         Инициализация параметров
     */
    void init();

    /*!
     *  \brief         Добавление указателя на Sim900
     *
     *  \param  [in]  *sim900 - указатель на Sim900
     */
    void addSim900(Sim900 *sim900);

    /*!
     *  \brief         Добавление телефона GSM Shield
     *
     *  \param  [in]  *phone - номер телефона
     */
    void addShieldPhone(const char *phone);
    
    /*!
     *  \brief         Добавление указателя на CanBusActions
     *
     *  \param  [in]  *canbus_actions - указатель на CanBusActions
     */
    void addCanBusActions(CanBusActions *canbus_actions);
    
    /*!
     *  \brief         Проверка секции "Настройки управления (права доступа)"
     *
     *  \param  [in]   phone           - телефон (M1-M4)
     *  \param  [in]  *cmd             - DTMF команда
     *  \param  [in]   gsm_password_en - требование GSM пароля
     *  \param  [in]   m2_owner_access - права M2 как M1
     *  \param  [in]  *gsm_pass        - GSM пароль
     *  \param  [in]   dtmf_sending    - отправлять ли DTMF
     *  \param  [in]   gsm_waiting_cmd - ложить ли трубку во время ожидания команд
     *  \param  [in]   time_s          - время разговора
     *
     *  \return        bool
     *  \retval        true  - успешная проверка
     *  \retval        false - возникла ошибка
     */
    bool checkDtmfOwnerAccess(phones_t           phone,
                              const char        *cmd,
                              gsm_password_en_t  gsm_password_en,
                              m2_owner_access_t  m2_owner_access,
                              const char        *gsm_pass               = s_gsm_pass,
                              dtmf_sending_t     dtmf_sending           = DTMF_SENDING_EN,
                              gsm_waiting_cmd_t  gsm_waiting_cmd        = WAITING_CMD_EN,
                              float              time_s                 = TIME_DELAY::MAX_PHONE_CALL_TIME_S);

    /*!
     *  \brief         Влючение/выключение и проверка статуса охраны с закрытием всех зон
     *
     *  \param  [in]   secure_mode      - режим охраны  (OFF/ON)
     *  \param  [in]   execution_method - метод исполнения
     *  \param  [in]   check_time_s     - время ожидания статуса охраны
     *
     *  \return        bool
     *  \retval        true             - успешная проверка
     *  \retval        false            - возникла ошибка
     */                          
    bool setSecureZoneClosed(bool           secure_mode,
                             command_type_t execution_method,
                             float          check_time_s     = TIME_DELAY::MAX_GUARD_STATE_EN_TIME_S);

    /*!
     *  \brief         Проверка состояния объединенных зон после изменения одной из них
     *
     *  \param  [in]   zone_name_t     - зона охраны состояние которой изменится
     *  \param  [in]   zone_new_state  - состояние зоны дверей
     *
     *  \return        uint8_t
     *  \retval        данные в hex об о состоянии зон используется enum slp_diag_data_perim_t
     */    
    uint8_t setAndCheckMergeZone(zone_name_t      zome_name,
                                 doors_statuses_t zone_new_state);

    /*!
     *  \brief         Установить и проверить начальное состояние по CAN для ДЗ
     *
     *  \return        bool
     *  \retval        true             - успешная проверка
     *  \retval        false            - возникла ошибка
     */
    bool setAndCheckInitCanStateForRemoteRun();

    /*!
     *  \brief         Выполнить начальные условия для кейсов C2198656-C2198685
     *
     *  \param  [in]   enable_ign               - состояние зажигания
     *  \param  [in]   set_ign_execution_method - способ управления зажиганием
     *  \param  [in]   can_speed_kmph           - скорость по CAN
     */
    void setAhjTestsStartConditions(ign_statuses_t enable_ign,
                                    command_type_t set_ign_execution_method = PIN,
                                    uint8_t        can_speed_kmph           = 0);

    /*!
     *  \brief         Получить тайминги для настроек АО и записать их в соответствующие переменные
     *
     *  \param  [in]  *option - указатель на настройки Мастера
     *
     *  \return        bool
     *  \retval        true  - настройки найдены и записаны в соответствующие переменные для использования в тестах
     *  \retval        false - настройки не найдены (не все)
     */
    bool getAndSetAhjSettings(options_t *option);

    /*!
     *  \brief         Выполнить проверки последнего шага для кейсов C2198656-C2198669
     *
     *  \return        bool
     *  \retval        true  - шаг прошел
     *  \retval        false - шаг не прошел
     */
    bool checkAhjTestsStepLast();

    /*!
     *  \brief         Выполнить вход в режим настройки по BLE для тестов C3202904-C3202905
     *
     *  \param  [in]   check_btn_light        - проверять ли состояние светодиода сервисной кнопки
     *  \param  [in]   ign_status_after_check - состояние зажигания после проверки работы режима
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setAndCheckBleSettingsModeOn(bool check_btn_light, ign_statuses_t ign_status_after_check);

    /*!
     *  \brief         Проверить выход из режима настройки по BLE для тестов C3202904-C3202905
     *
     *  \param  [in]   check_btn_light    - проверять ли состояние светодиода сервисной кнопки
     *  \param  [in]   region_wait_time_s - время ожидания состояния в регионе
     *
     *  \return        bool
     *  \retval        true  - шаг пройден
     *  \retval        false - шаг не пройден
     */
    bool checkBleSettingsTestsModeOff(bool check_btn_light, float region_wait_time_s = TIME_DELAY::MAX_WAITING_REGION_STATE_S);

    /*!
     *  \brief         Поставить в охрану и снять систему с охраны для проверки запрета снятия приложением (тесты C20445, C20461-C20463) и сбросить настройки для откл. запрета
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setAndCheckArmAndDisarm();

    /*!
     *  \brief         Включить сервисный режим и выйти из него по скорости
     *
     *  \param  [in]   speed_kmph - скорость автомобиля
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setAndResetServiceModeBySpeed(int8_t speed_kmph);

    /*!
     *  \brief         Включить/выключить охрану, заставить сработать датчик, проверить перепостановку
     *
     *  \param  [in]   sensor_type - тип датчика
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setSensorAndCheckRearm(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         Выполнить ДЗ, затем дистанционное глушение, проверить имитацию двери и снять с охраны
     *
     *  \param  [out] &eng_stop_timer           - ссылка на таймер, запускается при глушении двигателя
     *  \param  [in]   delay_after_doors_imit_s - задержка перед снятием с охраны после имитации открытия двери 
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setRemoteRunAndStopAndDisarm(Timer &eng_stop_timer, float delay_after_doors_imit_s);

    /*!
     *  \brief         Поставить в охрану, снять с охраны и проверить запрет поездки
     *
     *  \param  [in]   execution_method - способ постановки/снятия с охраны
     *  \param  [in]   tag_status       - состояние метки
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setArmAndDisarmAndCheckImmo(command_type_t execution_method,
                                     tag_statuses_t tag_status);

    /*!
     *  \brief         Поставить в охрану, снять с охраны и проверить запрет поездки. Затем включить зажигание и снова проверить
     *
     *  \param  [in]   execution_method - способ постановки/снятия с охраны
     *  \param  [in]   tag_status       - состояние метки
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setArmAndDisarmAndCheckImmoAndSetIgn(command_type_t execution_method,
                                              tag_statuses_t tag_status);

    /*!
     *  \brief         Включить сервисный режим, скорость и вызвать срабатывание ДД с проверкой состояния системы
     *
     *  \param  [in]   tag_status            - статус метки
     *  \param  [in]   speed_kmph            - скорость автомобиля
     *  \param  [in]   delay_before_sensor_s - задержка перед включением ДД
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setServiceAndSpeedAndSensor(tag_statuses_t tag_status,
                                     int8_t         speed_kmph,
                                     float          delay_before_sensor_s);

    /*!
     *  \brief         Включить питание R6, включить/выключить охрану, выключить зажигание, подождать (и пропустить) сработки сигналов R6
     *
     *  \param  [in]   secure_status - желаемое состояние охраны
     *
     *  \return        bool
     *  \retval        true  - шаги прошли
     *  \retval        false - один из шагов не пройден
     */
    bool setR6VccAndSecureAndIgnOff(secure_cmd_t secure_status);

    /*!
     *  \brief         Выключить питание ЦБ, затем включить и проверить блокировку R6 (НЗ)
     *
     *  \param  [in]   delay_s - задержка перед включением питания ЦБ
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool resetAndSetGen6VccAndCheckR6Block(float delay_s);

    /*!
     *  \brief         Выставить зажигание, двигатель (обороты) и проверить блокировку R6 (НЗ)
     *
     *  \param  [in]   ign_status   - состояние зажигания
     *  \param  [in]   eng_status   - состояние двигателя (оборотов)
     *  \param  [in]   block_status - состояние блокировки R6 (НЗ)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setIgnAndEngineAndCheckR6Block(ign_statuses_t   ign_status,
                                        eng_statuses_t   eng_status,
                                        block_statuses_t block_status);

    /*!
     *  \brief         Выставить состояние охраны и проверить блокировку R6 (НР)
     *
     *  \param  [in]   secure_status - желаемое состояние охраны
     *  \param  [in]   block_status  - состояние блокировки R6 (НР)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setSecureAndCheckR6Block(secure_cmd_t     secure_status,
                                  block_statuses_t block_status);

    /*!
     *  \brief         Поставить охрану, вызвать и проверить тревогу по датчику, а затем ее прерывание
     *
     *  \param  [in]   sensor_type - тип датчика для вызова тревоги
     *
     *  \return        bool
     *  \retval        true  - шаги пройдены
     *  \retval        false - один из шагов не пройден
     */
    bool setSecureAndSensorAndCheckAlert(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         Встать в охрану, включить/выключить метку/брелок, открыть багажник и проверить состояние тревоги
     *
     *  \param  [in]   tag_status    - состояние метки
     *  \param  [in]   remote_status - состояние брелока
     *  \param  [in]   alert_status  - проверяемое состояние тревоги
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setSecureAndTrunkAndCheckAlarm(tag_statuses_t    tag_status,
                                        remote_statuses_t remote_status,
                                        alert_statuses_t  alert_status);

    /*!
     *  \brief         Включить метку/брелок и проверить состояние тревоги
     *
     *  \param  [in]   tag_status    - состояние метки
     *  \param  [in]   remote_status - состояние брелока
     *  \param  [in]   alert_status  - проверяемое состояние тревоги
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setTagAndRemoteAndCheckAlarm(tag_statuses_t    tag_status,
                                      remote_statuses_t remote_status,
                                      alert_statuses_t  alert_status);

    /*!
     *  \brief         Настроить скорость CAN и ACK-лки на Mercedes CLA и поставить в охрану
     *
     *  \return        bool
     *  \retval        true  - шаги пройдены
     *  \retval        false - один из шагов не пройден
     */
    bool setCarMercedesClaAndSecure();

    /*!
     *  \brief         Настроить скорость CAN и ACK-лки на Batmobile и отключить штатную тревогу
     *
     *  \return        bool
     *  \retval        true  - смена настроек прошла успешна
     *  \retval        false - возникла ошибка при смене настроек
     */
    bool setStdAlertOffAndCarBatmobile();

    /*!
     *  \brief         Включить штатную тревогу и проверить ее включение
     *
     *  \param  [in]   alarm_check_type - как долго проверять тревогу (только начало, 0,5 цикла, полный цикл)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setAndCheckStdAlarm(alarm_check_type_t alarm_check_type);

    /*!
     *  \brief         Включить штатную тревогу и проверить 8 циклов тревоги
     *
     *  \param  [in]   alarm_check_type - как долго проверять тревогу (только начало, 0,5 цикла, полный цикл)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setAndCheckStdAlarmFull(alarm_check_type_t alarm_check_type);

    /*!
     *  \brief         Включить и проверить включение режима "Антиограбление (по метке/команде)"
     *
     *  \param  [in]   execution_method - способ включения (метка/команда)
     *
     *  \return        bool
     *  \retval        true  - вход в режим произошел
     *  \retval        false - вход в режим не произошел
     */
    bool setAndCheckAhj(command_type_t execution_method);

    /*!
     *  \brief         Включить и проверить включение режима "Охрана с открытым багажником"
     *
     *  \return        bool
     *  \retval        true  - вход в режим произошел
     *  \retval        false - вход в режим не произошел
     */
    bool setAndCheckArmTrunk();

    /*!
     *  \brief         Включить и проверить включение режима "Ожидание авторизации"
     *
     *  \return        bool
     *  \retval        true  - вход в режим произошел
     *  \retval        false - вход в режим не произошел
     */
    bool setAndCheckWaitingAuth();

    /*!
     *  \brief         Включить и проверить включение режима "Запрет поездки"
     *
     *  \return        bool
     *  \retval        true  - вход в режим произошел
     *  \retval        false - вход в режим не произошел
     */
    bool setAndCheckImmo();

    /*!
    *  \brief         Включить и проверить включение режима "Тревога: предупредительная"
     *
     *  \return        bool
     *  \retval        true  - вход в режим произошел
     *  \retval        false - вход в режим не произошел
     */
    bool setAndCheckAlertLow();

    /*!
     *  \brief         Включить и проверить включение режима "Тревога: основная"
     *
     *  \return        bool
     *  \retval        true  - вход в режим произошел
     *  \retval        false - вход в режим не произошел
     */
    bool setAndCheckAlertHigh();

    /*!
     *  \brief         Перейти в определенное состояние и проверить блокировку АКПП
     *
     *  \param  [in]   state    - состояние, в которое мы переходим перед проверкой блокировки
     *  \param  [in]   act_type - действие, которые мы выполняем после блокировки для разблокировки
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setStateAndCheckGearboxBlock(guard_state_t state, act_types_t act_type);

    /*!
     *  \brief         Перейти в охрану и проверить блокировку АКПП
     *
     *  \param  [in]   parking_type - тип паркинга (CAN/аналог)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setArmAndCheckGearboxNoBlock(parking_type_t parking_type);

    /*!
     *  \brief         Чтобы пройти авторизацию: включить метку, при отсутствии модуля метки - включить брелок
     *
     *  \param  [in]   delay_s - задержка после включения
     *
     *  \return        bool
     *  \retval        true  - устройство беспроводной авторизации включено
     *  \retval        false - нет необходимых модулей для беспроводной авторизации
     */
    bool authByTagOrRemote(float delay_s);

    /*!
     *  \brief         Задействовать зону и проверить авторизацию на различных шагах
     *
     *  \param  [in]   zone_type - активируемая зона
     *  \param  [in]   auth_step - шаг алгоритма "Ожидание авторизации", на котором нужно пройти авторизацию
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool setZoneAndCheckAuth(zone_type_t zone_type, auth_step_t auth_step);

    /*!
     *  \brief         Включить охрану и управлять питанием метки + брелока
     *
     *  \param  [in]   status - желаемое состояние питания метки + брелка (либо оба устройства включены, либо выключены)
     *
     *  \return        bool
     *  \retval        true  - успешное выполнение
     *  \retval        false - ошибка при выполнении
     */
    bool setSecureOnAndTagVccAndRemoteVcc(bool status);

    /*!
     *  \brief         Открыть багажник и проверить тревогу
     *
     *  \param  [in]   alert_status - ожидаемое состояние тревоги
     *
     *  \return        bool
     *  \retval        true  - состояние тревоги соответствует ожидаемому
     *  \retval        false - состояние тревоги не соответствует ожидаемому
     */
    bool openTrunkAndCheckAlarm(alert_statuses_t alert_status);

    /*!
     *  \brief         Выключить метку/брелок и проверить что тревога не включилась
     *
     *  \param  [in]   auth_profile         - устройство для выключения (метка/брелок)
     *  \param  [in]   delay_before_reset_s - задержка перед выключением
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не пройдена
     */
    bool resetTagOrRemoteAndCheckAlarm(auth_profile_t auth_profile, float delay_before_reset_s = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         Ожидание срабатывания выхода блокировки основного блока и R6
     *
     *  \param  [in]   block_status - ожидаемое состояние выхода блокировки
     *  \param  [in]   timeout_s    - время ожидания
     *
     *  \return        bool
     *  \retval        true  - состояние выхода блокировки соответствует ожидаемому
     *  \retval        false - состояние выхода блокировки не соответствует ожидаемому
     */
    bool waitStdAndR6Block(block_statuses_t block_status, float timeout_s = TIME_DELAY::MAX_STD_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         Получить метод исполнения команды снятия с охраны (доступный данному устройству помимо метки, наличие метки обязательно)
     *
     *  \return        метод снятия с охраны
     */
    command_type_t getExecutionMethodDisarmNoTag();

    /*!
     *  \brief         Включить/выключить метку и брелок, затем выключить охрану, открыть дверь и вкл. зажигание
     *
     *  \param  [in]   tag_status    - желаемое состояние метки
     *  \param  [in]   remote_status - желаемое состояние брелока
     *
     *  \return        bool
     *  \retval        true  - шаги пройдены
     *  \retval        false - один из шагов не пройден
     */
    bool setDisarmAndDoorAndIgn(tag_statuses_t    tag_status,
                                remote_statuses_t remote_status);

    /*!
     *  \brief         Выключить брелок/метку, вызвать срабатывание ДД и проверить срабатывание АО
     *
     *  \param  [in]   ahj_type   - тип активации АО
     *  \param  [in]   ahj_status - ожидаемый статус АО
     *
     *  \return        bool
     *  \retval        true  - шаги пройдены
     *  \retval        false - один из шагов не пройден
     */
    bool resetTagOrRemoteAndCheckAhj(ahj_type_t   ahj_type,
                                     ahj_status_t ahj_status);

    /*!
     *  \brief         Включить охрану, включить брелок, выключить метку, выключить зажигание
     *
     *  \return        bool
     *  \retval        true  - охрана включена
     *  \retval        false - охрана не включена
     */
    bool setSecureAndRemoteAndResetTagAndIgn();

    /*!
     *  \brief         Проверить бузер в режиме \"Ожидание авторизации\"
     *
     *  \param  [in]   timeout_s     - время проверки
     *  \param  [in]   pulse_width_s - ожидаемая длительность импульсов
     *  \param  [in]   pulse_pause_s - ожидаемая пауза между импульсами
     *
     *  \return        bool
     *  \retval        true  - проверка пройдена
     *  \retval        false - проверка не пройдена
     */
    bool checkBuzzerAuth(float timeout_s,
                         float pulse_width_s,
                         float pulse_pause_s);

    /*!
     *  \brief         Проверить бузер в режиме \"Ожидание авторизации\" на шаге 1
     *
     *  \param  [in]   timeout_s - время проверки
     *
     *  \return        bool
     *  \retval        true  - проверка пройдена
     *  \retval        false - проверка не пройдена
     */
    bool checkBuzzerAuthStepOne(float timeout_s);

    /*!
     *  \brief         Проверить бузер в режиме \"Ожидание авторизации\" на шаге 2
     *
     *  \param  [in]   timeout_s - время проверки
     *
     *  \return        bool
     *  \retval        true  - проверка пройдена
     *  \retval        false - проверка не пройдена
     */
    bool checkBuzzerAuthStepTwo(float timeout_s);

    /*!
     *  \brief         Поставить систему в охрану, затем войти в ожидание авторизации и проверить шаги
     *
     *  \return        bool
     *  \retval        true  - шаги пройдены
     *  \retval        false - один из шагов не пройден
     */
    bool setAndCheckAuthWithBuzzer();

    /*!
     *  \brief         Поставить систему в охрану, затем включить брелок и выключить метку (наличие метки обязательно, наличие брелка - нет)
     *
     *  \param  [in]   secure_zone - нарушаемая зона при постановке (для охраны с нарушенным периметром)
     *
     *  \return        bool
     *  \retval        true  - охрана включена
     *  \retval        false - охрана не включена
     */
    bool setArmAndRemoteAndResetTag(secure_zone_t secure_zone);

    /*!
     *  \brief         Выключить охрану без задержки и проверить статус охраны (не используя метку)
     *
     *  \return        bool
     *  \retval        true  - охрана выключена
     *  \retval        false - охрана не выключена
     */
    bool setAndCheckDisarmNoDelayNoTag();

    /*!
     *  \brief         Настроить скорость, педаль тормоза, закрыть все зоны и включить охрану (и подождать время демаскирования датчиков после постановки)
     *
     *  \param  [in]   speed              - скорость автомобиля
     *  \param  [in]   brake_pedal_status - желаемое состояние педали тормоза (нажата/не нажата)
     *
     *  \return        bool
     *  \retval        true  - все статусы соответствуют ожидаемым
     *  \retval        false - не все статусы соответствуют ожидаемым
     */
    bool setSpeedAndBrakeAndArmZoneClosed(int8_t speed, brake_pedal_status_t brake_pedal_status);

    /*!
     *  \brief         Активировать зону и проверить, что выход блокировки сработал (и выключился через время настройки 'Длительность блокировки'), или не сработал
     *
     *  \param  [in]   zone_type          - активируемая зона
     *  \param  [in]   block_status       - ожидаемый статус блокировки
     *  \param  [in]   alarm_check_status - проверять ли тревогу после блокировки
     *
     *  \return        bool
     *  \retval        true  - статус блокировки соответствует ожидаемому
     *  \retval        false - статус блокировки не соответствует ожидаемому
     */
    bool setZoneAndCheckBlockTime(zone_type_t          zone_type,
                                  block_statuses_t     block_status,
                                  alarm_check_status_t alarm_check_status = CHECK_ALARM);

    /*!
     *  \brief         Вызвать тревогу (по ДД), проверить блокировку, активировать зону, проверить блокировку
     *
     *  \param  [in]   zone_type - активируемая зона
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setAlertAndZoneAndCheckBlock(zone_type_t zone_type);

    /*!
     *  \brief         Выполнить и проверить режим АО (от начальных условий до проверки блокировки)
     *
     *  \param  [in]   brake_pedal_status - зажимать ли педаль тормоза перед открытием двери (и на все время АО) или нет
     *  \param  [in]   ahj_block_type     - тип блокировки АО (сразу или через 180 сек.)
     *
     *  \return        bool
     *  \retval        true  - все статусы соответствуют ожидаемым
     *  \retval        false - не все статусы соответствуют ожидаемым
     */
    bool setAhjAndCheckBlock(brake_pedal_status_t brake_pedal_status,
                             ahj_block_type_t     ahj_block_type);

    /*!
     *  \brief         Отпустить педаль тормоза, выключить + включить зажигание, проверить блокировку
     *
     *  \param  [in]   block_status - ожидаемый статус блокировки
     *
     *  \return        bool
     *  \retval        true  - статус блокировки соответствует ожидаемому
     *  \retval        false - статус блокировки не соответствует ожидаемому
     */
    bool resetBrakeAndRestartIgnAndCheckBlock(block_statuses_t block_status);

    /*!
     *  \brief         Включить датчик движения, подождать 180 сек. (внутренний тайминг, макс. задержка блокировки после Шага 4 АО), включить датчик движения и проверить блокировку
     *
     *  \detailed      Блокировка проверяется после каждого нарушения зоны (статус: не включилась), но в параметре указывается статус последней блокировки
     *
     *  \param  [in]   block_status - ожидаемый статус блокировки
     *
     *  \return        bool
     *  \retval        true  - статус блокировки соответствует ожидаемому
     *  \retval        false - статус блокировки не соответствует ожидаемому
     */
    bool setSensorAndCheckBlockTwice(block_statuses_t block_status);

    /*!
     *  \brief         Включить датчик движения, включить педаль тормоза, включить датчик движения и проверить блокировку
     *
     *  \detailed      Блокировка проверяется после каждого нарушения зоны (статус: не включилась), но в параметре указывается статус последней блокировки
     *
     *  \param  [in]   block_status - ожидаемый статус блокировки
     *
     *  \return        bool
     *  \retval        true  - статус блокировки соответствует ожидаемому
     *  \retval        false - статус блокировки не соответствует ожидаемому
     */
    bool setSensorAndBrakeAndSensorAndCheckBlock(block_statuses_t block_status);

    /*!
     *  \brief         Выключить педаль тормоза, включить ДД и проверить включение блокировки
     *
     *  \return        bool
     *  \retval        true  - блокировка включена
     *  \retval        false - блокировка не включена
     */
    bool resetBrakeAndSetSensorAndCheckBlockOn();

    /*!
     *  \brief         Нажать педаль тормоза и проверить блокировку
     *
     *  \param  [in]   block_status - ожидаемый статус блокировки
     *
     *  \return        bool
     *  \retval        true  - статус блокировки соответствует ожидаемому
     *  \retval        false - статус блокировки не соответствует ожидаемому
     */
    bool setBrakeAndCheckBlock(block_statuses_t block_status);

    /*!
     *  \brief         Выполнить вход в ЗП (начиная с выполнения начальных условий), включить датчик движения и проверить блокировку
     *
     *  \param  [in]   block_status   - ожидаемый статус блокировки
     *  \param  [in]   car_speed_type - устанавливать ли скорость после включения ЗП
     *  \param  [in]   car_speed      - скорость автомобиля
     *
     *  \return        bool
     *  \retval        true  - статус блокировки соответствует ожидаемому
     *  \retval        false - статус блокировки не соответствует ожидаемому
     */
    bool setImmoAndSensorAndCheckBlock(block_statuses_t block_status,
                                       car_speed_type_t car_speed_type,
                                       int8_t           car_speed       = 0);

    /*!
     *  \brief         Выключить охрану, включить метку и брелок (наличие метки - обязательно, наличие брелка - нет)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setDisarmAndSetRemoteAndTagAndIgn();
    
    /*!
     *  \brief         Включить сервис и выключить его по ДД (по скорости/расстоянию) без зажигания
     *
     *  \param  [in]   speed_kmph - скорость
     *  \param  [in]   delay_s    - задержка перед включением ДД
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setAndResetServiceBySensorNoIgn(int8_t speed_kmph, float delay_s);

    /*!
     *  \brief         Сбросить номер автомобиля CAN
     *
     *  \return        bool
     *  \retval        true  - сброс произведен
     *  \retval        false - сброс не произведен
     */
    bool resetCanCarNum();

    /*!
     *  \brief         Ожидать включения обходчика по аналогу и CAN
     *
     *  \param  [in]   timeout_s - время ожидания
     *
     *  \return        bool
     *  \retval        true  - обходчик включен
     *  \retval        false - обходчик не включился по аналогу или по CAN
     */
    bool waitImmoAnalogAndCan(float timeout_s = TIME_DELAY::BYPASS_AFTER_SMS_TIME_S);

    /*!
     *  \brief         Выполнить ДЗ, открыть дверь/багажник и проверить статус охраны
     *
     *  \detailed      ДЗ выполняется либо по брелку (при наличии метки), либо по СМС (если метка отсутствует)
     *
     *  \param  [in]   zone_type   - открываемая зона
     *  \param  [in]   diag_status - ожидаемый статус охраны (тревога/ожидание авторизации)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool runEngAndSetZoneAndCheckState(zone_type_t              zone_type,
                                       slp_diag_data_statuses_t diag_status);

    /*!
     *  \brief         Сбросить настройки ожидания авторизации/запрета поездки
     *
     *  \return        bool
     *  \retval        true  - настройки сброшены
     *  \retval        false - ошибка при сбросе настроек
     */
    bool resetAuthAndImmo();

    /*!
     *  \brief         Сохранить текущую версию ПО (для возможности возвращения на нее)
     */
    void saveCurrFw();

    /*!
     *  \brief         Загрузить предыдущую версию ПО
     *
     *  \return        bool
     *  \retval        true  - прошивка загружена
     *  \retval        false - ошибка при загрузке прошивки
     */
    bool loadPrevFw();

    /*!
     *  \brief         Загрузить текущую версию ПО
     *
     *  \detailed      Перед использованием должен быть использован метод saveCurrFw, иначе информации о версии ПО не будет (загрузка прошивки не начнется, вернется false)
     *
     *  \return        bool
     *  \retval        true  - прошивка загружена
     *  \retval        false - ошибка при загрузке прошивки
     */
    bool loadCurrFw();

    /*!
     *  \brief         Снять с охраны и проверить отпирание замка капота на контакте реле
     *
     *  \param  [in]   relay_type - тип реле
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setDisarmAndCheckRelayHoodUnlock(relay_type_t relay_type);

    /*!
     *  \brief         Поставить в охрану и проверить запирание замка капота на контакте реле
     *
     *  \param  [in]   relay_type - тип реле
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setArmAndCheckRelayHoodLock(relay_type_t relay_type);

    /*!
     *  \brief         Вызвать тревогу и проверить блокировку двигателя на контакте реле
     *
     *  \param  [in]   relay_type - тип реле
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setAlertAndCheckRelayBlock(relay_type_t relay_type);

    /*!
     *  \brief         Загрузить ПО в основной блок и проверить работу реле
     *
     *  \param  [in]   fw_update_type - тип обновления ПО (откат на предыдущую версию/загрузка новой версии/откат на специальную версию)
     *  \param  [in]   relay_type     - тип реле
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setFwAndCheckRelay(fw_update_type_t fw_update_type,
                            relay_type_t     relay_type);

    /*!
     *  \brief         Выполнить и проверить снятие альт. сервисной кнопкой
     *
     *  \param  [in]   guard_state                 - состояние системы после начала входа в режим альт. серв. кнопки
     *  \param  [in]   alt_srv_btn_type            - тип альтернативной сервисной кнопки (дверь/зажигание/штатная кнопка)
     *  \param  [in]   pin_type                    - тип вводимого кода экстренного снятия (верный/неверный)
     *  \param  [in]   emergency_dis_wrong_pin_num - цифра неверного кода (если вводится неверный код, иначе параметр не учитывается)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setAndCheckAltSrvBtnDisarm(slp_diag_data_statuses_t diag_guard_state,
                                    alt_srv_btn_type_t       alt_srv_btn_type,
                                    srv_btn_pin_type_t       pin_type,
                                    emergency_dis_pin_num_t  emergency_dis_wrong_pin_num = EMERGENCY_DIS_PIN_NUM_ALL);

    /*!
     *  \brief         Выполнить и проверить неудачную постановку в охрану альтернативной сервисной кнопкой
     *
     *  \param  [in]   alt_srv_btn_type - тип альтернативной сервисной кнопки (дверь/зажигание/штатная кнопка)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setAndCheckAltSrvBtnArmFail(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         Экстренное снятие с охраны
     *
     *  \detailed      Для кейсов, где нужно вводить неправильный код. Если без ввода верного будет прогнано 3 таких кейса подряд,
     *                 то будет блокировка ввода кода на 15 минут
     */
    void emergencyDisarming();

    /*!
     *  \brief         Войти в ожидание авторизации и проверить тайминги свободных рук
     *
     *  \param  [in]   zone_type          - активируемая зона в ожидании авторизации
     *  \param  [in]   alert_check_status - тип проверяемой тревоги (включилась/не включилась)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setWaitingAuthAndCheckHandsFreeTimings(zone_type_t            zone_type,
                                                alert_check_statuses_t alert_check_status);

    /*!
     *  \brief         Снять с охраны и отключить питание BLE реле (R6 и R6 ECO)
     *
     *  \return        bool
     *  \retval        true  - успешное выполнение
     *  \retval        false - ошибка при выполнении
     */
    bool setDisarmAndResetBleRelayVcc();

    /*!
     *  \brief         Зарегистрировать BLE реле (R6/R6 ECO) несколько раз (за один цикл регистрации) и проверить блокировку на них после этого
     *
     *  \param  [in]   relay_registration_sequence - порядок регистрации реле (напр.: сначала R6, затем R6 ECO)
     *  \param  [in]   registration_count          - кол-во регистраций реле (за один цикл)
     */
    bool registerBleRelayFewTimesAndCheckBlock(relay_registration_sequence_t relay_registration_sequence,
                                               uint8_t                       registartion_count);

    /*!
     *  \brief         Нажать сервисную кнопку необходимое кол-во раз для вкл. режима настройки по BLE и проверить включение сервисного светодиода через 3 с.
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setBleSettingsModeClicksAndWaitLight();

    /*!
     *  \brief         Проверить индикацию сервисным светодиодом при включении/выключении режима настройки по BLE
     *
     *  \brief         out_status - проверяемый статус (индикация при включении режима или отключении)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool checkBleSettingsModeIndication(out_statuses_t out_status);

    /*!
     *  \brief         Выполнить и проверить включение/выключение режима настройки по BLE
     *
     *  \param  [in]   out_status                - желаемое состояние режима настройки по BLE (вкл./выкл.)
     *  \param  [in]   ble_settings_mode_en_type - способ включения (сервиснсой кнопкой/серв. кнопкой без зажигания/по СМС)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setAndCheckBleSettingsMode(out_statuses_t              out_status,
                                    ble_settings_mode_en_type_t ble_settings_mode_en_type);

    /*!
     *  \brief         Включить АО по метке/брелку и проверить шаги алгоритма и СМС
     *
     *  \param  [in]   ahj_type - тип включения АО (по метке/брелку)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка не успешна
     */
    bool setAhjAndCheckSms(ahj_type_t ahj_type);

    /*!
     *  \brief         Управление питанием метки и ЖКИ брелока
     *
     *  \detailed      Управляет в зависимости от модулей, если нет модуля - управления питанием на соответствующем устройстве не будет
     *
     *  \param  [in]   tag_status    - желаемое состояние питания метки
     *  \param  [in]   remote_status - желаемое состояние питания брелока
     */
    void setTagAndRemoteVcc(tag_statuses_t    tag_status,
                            remote_statuses_t remote_status);

    /*!
     *  \brief         Открыть багажник и проверить тревогу
     *
     *  \param  [in]   alert_status - ожидаемое состояние тревоги
     *  \param  [in]   time_s       - время проверки тревоги (если выключена)/время задержки включения тревоги
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setTrunkAndCheckAlarm(alert_statuses_t alert_status, float time_s);

    /*!
     *  \brief         Проверить блокировку двигателя кнопкой Старт-Стоп
     *
     *  \detailed      Метод проверяет точное соответствие количеству импульсов из параметра (см. start_btn_check_pulses_count),
     *                 длительность импульсов (настройка "Длительность импульса на кнопку старт-стоп")
     *                 и паузу между импульсами (внутренний тайминг) кнопки Старт-Стоп
     *
     *  \param  [in]   start_btn_check_pulses_count - ожидаемое количество импульсов
     *  \param  [in]   no_pulses_check_time_s       - время проверки отсуствия лишних импульсов кнопки Старт-Стоп
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkStartStopBtnBlock(start_btn_check_pulses_count_t start_btn_check_pulses_count,
                                float                          no_pulses_check_time_s        = TIME_DELAY::START_STOP_BTN_NO_BLOCK_CHECK_TIME_S);

    /*!
     *  \brief         Вызвать срабатывание блокировки и проверить блокировку кнопкой Старт-Стоп
     *
     *  \param  [in]   start_stop_block_trigger     - триггер для включения блокировки
     *  \param  [in]   start_btn_check_pulses_count - ожидаемое количество импульсов
     *  \param  [in]   no_pulses_check_time_s       - время проверки отсуствия лишних импульсов кнопки Старт-Стоп
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setBlockTriggerAndCheckStartStopBtnBlock(start_stop_block_trigger_t     start_stop_block_trigger,
                                                  start_btn_check_pulses_count_t start_btn_check_pulses_count,
                                                  float                          no_pulses_check_time_s        = TIME_DELAY::START_STOP_BTN_NO_BLOCK_CHECK_TIME_S);

    /*!
     *  \brief         Проверить наличие модулей для перехода в ожидание авторизации по метке/брелку (для метода setAndCheckWaitingAuth)
     *
     *  \detailed      Будет произведена проверка на возможные комбинации модулей для перехода в ожидание авторизации по метке/брелку:
     *                 Брелок + метка: авторизация по метке,  снятие брелком
     *                 Брелок + GSM:   авторизация по брелку, снятие по СМС
     *                 Метка + GSM:    авторизация по метке,  снятие по СМС
     *
     *  \return        bool
     *  \retval        true  - имеются необходимые модули для перехода в ожидание авторизации по метке/брелку
     *  \retval        false - не все модули имеются для перехода в ожидание авторизации по метке/брелку
     */
    bool checkModulesForWaitingAuthTagRem();

    /*!
     *  \brief         Перейти в режим ожидание авторизации по метке/брелку
     *
     *  \detailed      Будет произведен вход в ожидание авторизации, исходя из модулей в наличии:
     *                 Брелок + метка: авторизация по метке,  снятие брелком
     *                 Брелок + GSM:   авторизация по брелку, снятие по СМС
     *                 Метка + GSM:    авторизация по метке,  снятие по СМС
     *
     *  \return        bool
     *  \retval        true  - переход произведен
     *  \retval        false - переход не произведен
     */
    bool setAndCheckWaitingAuthTagRem();

    /*!
     *  \brief         Управление питанием устройства, выступающего в качестве устройства авторизации
     *
     *  \detailed      Будет произведено отключение устройства, исходя из модулей в наличии:
     *                 Брелок + метка: авторизация по метке,  снятие брелком => управляется метка
     *                 Брелок + GSM:   авторизация по брелку, снятие по СМС  => управляется брелок
     *                 Метка + GSM:    авторизация по метке,  снятие по СМС  => управляется метка
     *
     *  \param  [in]   out_status - желаемое состояние питания (вкл./выкл.)
     */
    void setAuthDeviceVcc(out_statuses_t out_status);

    /*!
     *  \brief         Включить АО по команде (по СМС или через моб. приложение, исходя из модулей в наличии)
     *
     *  \return        bool
     *  \retval        true  - АО по команде включено (статус в регионе)
     *  \retval        false - ошибка при входе в режим АО по команде
     */
    bool setAndCheckAhjSmsOrMobile();

    /*!
     *  \brief         Установить скорость автомобиля по CAN
     *
     *  \detailed      Выполняется алгоритм: вкл. зажигание -> установить скорость -> отключить зажигание
     *
     *  \param  [in]   speed - устанавливаемая скорость
     */
    void setCanCarSpeed(int8_t speed);

    /*!
     *  \brief         Включить/выключить метку, снять с охраны по Slave и проверить снятие с охраны
     *
     *  \param  [in]   tag_status       - желаемое состояние метки
     *  \param  [in]   slave_type       - тип Slave (аналог/CAN)
     *  \param  [in]   diag_data_status - ожидаемое состояние охраны
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setTagAndSlaveDisarmAndCheckDisarm(tag_statuses_t           tag_status,
                                            slave_type_t             slave_type,
                                            slp_diag_data_statuses_t diag_data_status);

    /*!
     *  \brief         Включить меню сервисный кнопки (войти в какой-либо режим)
     *
     *  \param  [in]   clicks_count         - кол-во нажатий сервисной кнопки
     *  \param  [in]   srv_btn_menu_on_type - тип включения режима через меню сервисной кнопки (по вкл. зажигания или без зажигания)
     *  \param  [in]   diag_data_status     - ожидаемый статус охраны
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setSrvBtnMenu(uint8_t                  clicks_count,
                       srv_btn_menu_on_type_t   srv_btn_menu_on_type,
                       slp_diag_data_statuses_t diag_data_status);

    /*!
     *  \brief         Включить запрет поездки по зажиганию
     *
     *  \return        bool
     *  \retval        true  - запрет поездки включен
     *  \retval        false - неудачное включение запрета поездки
     */
    bool setImmoByIgn();

    /*!
     *  \brief         Выключить и включить брелки и BLE-реле
     *
     *  \detailed      Реле управляются только при наличии BLE модуля
     *                 Управляемые брелки: LCD
     *                 Управляемые реле: R6, R6 ECO
     */
    void resetAndSetKeyringAndRelayVcc();

    /*!
     *  \brief         Включить + выключить зажигание (напр. для выхода из регистрации)
     */
    void setAndResetIgn();

    /*!
     *  \brief         Отправить штатные кнопки CAN (ввести код валидатора) и проверить сигналы сервисного светодиода и сирены
     *
     *  \detailed      Отправляемые штатные кнопки фиксированы и определены в Gen6Basic::sendBtnSequence()
     *
     *  \param  [in]   click_count  - кол-во нажатий штатных кнопок
     *  \param  [in]   signal_count - кол-во ожидаемых сигналов сервисного светодиода и сирены
     *  \param  [in]   siren_status - нужно ли проверять сирену
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool sendBtnAndCheckSignals(uint8_t        click_count,
                                uint8_t        signal_count,
                                siren_status_t siren_status);

    /*!
     *  \brief         Включить/выключить зажигание и проверить включение выхода видеорегистратора
     *
     *  \param  [in]   ign_status - желаемое состояние зажигания
     *  \param  [in]  *timer      - указатель на таймер, который будет включаться при управлении зажиганием
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setIgnAndCheckVideoRegOn(ign_statuses_t ign_status, Timer *timer = NULL);

    /*!
     *  \brief         Включить/выключить сервисный режим и проверить включение выхода видеорегистратора
     *
     *  \param  [in]   service_status - желаемое состояние сервисного режима (вкл./выкл.)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setServiceAndCheckVideoRegOn(service_statuses_t service_status);

    /*!
     *  \brief         Проверка задержки выключения выхода видеорегистратора
     *
     *  \param  [in]   delay_s    - задержка перед выключением
     *  \param  [in]   err_time_s - погрешность выключения
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool waitVideoRegOff(float delay_s, float err_time_s);

    /*!
     *  \brief         Выключить зажигание и проверить задержку выключения выхода видеорегистратора
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool resetIgnAndCheckVideoRegOff();

    /*!
     *  \brief         Включить тревогу, включить сервисный режим и проверить выход видеорегистратора
     *
     *  \detailed      Включить тревогу, выключить тревогу, включить сервис (независимо от параметров), отключить сервис (service_status)
     *
     *  \param  [in]   service_status - нужно ли отключать сервисный режим после включения
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setAlertAndServiceAndCheckVideoReg(service_statuses_t service_status);

    /*!
     *  \brief         Включить видеорегистратор и вкл./выкл. сервисный режим, а также проверить время работы видеорегистратора
     *
     *  \param  [in]   service_status - желаемое состояние сервисного режима (вкл./выкл./не управлять)
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool setAndCheckServiceAndVideoReg(service_statuses_t service_status);

    /*!
     *  \brief         Проверить сервисный светодиод в режиме настройки по BLE
     *
     *  \param  [in]   mobile_conn_type - тип соединения с моб. приложением (установлена ли связь со смартфоном по BLE)
     *  \param  [in]   timeout_s        - время проверки
     *
     *  \return        bool
     *  \retval        true  - проверка успешна
     *  \retval        false - проверка неуспешна
     */
    bool checkServiceLedInBleSettingsMode(mobile_conn_type_t mobile_conn_type, float timeout_s = TIME_DELAY::BTN_LED_MODE_BLE_CHECK_TIME_S);

    /*!
     *  \brief         1. Нажатие сенсора с зажиганием не включает Пляж 
     */
    void C168671_TestCase();
    
    /*!
     *  \brief         2. Постановка в Охрану с отключенным Пляжем 
     */
    void C168672_TestCase();
    
    /*!
     *  \brief         3. Включение зажигания до 3 нажатий сенсора 
     */
    void C168673_TestCase();
    
    /*!
     *  \brief         4. Нажатия сенсора в Снято до ожидания ввода 
     */
    void C168674_TestCase();
    
    /*!
     *  \brief         5. 1ый импульс сенсора постановки менее 0,2 сек. 
     */
    void C168675_TestCase();
    
    /*!
     *  \brief         6. 2ой импульс сенсора постановки менее 0,2 сек. 
     */
    void C168676_TestCase();
    
    /*!
     *  \brief         7. 3ий импульс сенсора постановки менее 0,2 сек. 
     */
    void C168677_TestCase();
    
    /*!
     *  \brief         8. 3 импульса сенсора постановки менее 0,2 сек. 
     */
    void C168678_TestCase();
    
    /*!
     *  \brief         9. 1ый импульс сенсора постановки более 1 сек. 
     */
    void C168679_TestCase();
    
    /*!
     *  \brief         10. 2ой импульс сенсора постановки более 1 сек. 
     */
    void C168680_TestCase();
    
    /*!
     *  \brief         11. 3ий импульс сенсора постановки более 1 сек. 
     */
    void C168681_TestCase();
    
    /*!
     *  \brief         12. 3 импульса сенсора постановки более 1 сек. 
     */
    void C168682_TestCase();
    
    /*!
     *  \brief         13. Включение режима пляж без видимой метки 
     */
    void C168683_TestCase();
    
    /*!
     *  \brief         14. Включение Пляж с видимой меткой 
     */
    void C168684_TestCase();
    
    /*!
     *  \brief         15. Попытка выключить охрану в Пляж штатным(CAN) Slave 
     */
    void C168685_TestCase();
    
    /*!
     *  \brief         16. Попытка выключить охрану в Пляж аналог. Slave 
     */
    void C168686_TestCase();
    
    /*!
     *  \brief         17. Попытка выключить охрану звонком с чужого номера 
     */
    void C168687_TestCase();
    
    /*!
     *  \brief         18. Попытка выключить охрану звонком с номера владельца 
     */
    void C168688_TestCase();
    
    /*!
     *  \brief         19. Попытка выключить охрану СМС с чужого номера 
     */
    void C168689_TestCase();
    
    /*!
     *  \brief         20. Попытка выключить охрану СМС с номера владельца 
     */
    void C168690_TestCase();
    
    /*!
     *  \brief         21. Попытка выключить охрану брелоком  
     */
    void C168691_TestCase();
    
    /*!
     *  \brief         22. Попытка выключить охрану меткой 
     */
    void C168692_TestCase();
    
    /*!
     *  \brief         23. Попытка выключить охрану через личный кабинет 
     */
    void C168693_TestCase();
    
    /*!
     *  \brief         24. Попытка выключить охрану мобильным приложением 
     */
    void C168694_TestCase();
    
    /*!
     *  \brief         25. Попытка выключить охрану СМС с номера владельца 
     */
    void C168695_TestCase();
    
    /*!
     *  \brief         26. Попытка выключить охрану Своб. руками через сенсор 
     */
    void C168696_TestCase();
    
    /*!
     *  \brief         27. Попытка выключить охрану штатными Свободными руками 
     */
    void C168697_TestCase();
    
    /*!
     *  \brief         28. Появление и пропадание метки в Пляж 
     */
    void C168698_TestCase();
    
    /*!
     *  \brief         29. Выключение Пляж гибкой логикой с видимой меткой 
     */
    void C168699_TestCase();
    
    /*!
     *  \brief         30. Выключение Пляж гибкой логикой без метки 
     */
    void C168700_TestCase();
    
    /*!
     *  \brief         31. Включение Пляж гибкой логикой с видимой меткой 
     */
    void C168701_TestCase();
    
    /*!
     *  \brief         32. Включение Пляж гибкой логикой без метки 
     */
    void C168702_TestCase();
    
    /*!
     *  \brief         33. Попытка выключить Пляж сенсором с запозданием ввода цифры более 5 сек. 
     */
    void C168703_TestCase();
    
    /*!
     *  \brief         34. Попытка выкл. Пляж сенсором с неправильным ПИН 
     */
    void C168704_TestCase();
    
    /*!
     *  \brief         35. Выключение Пляж с активной меткой с выключенными свободными руками по сенсору 
     */
    void C168705_TestCase();
    
    /*!
     *  \brief         36. Выключение Пляж с активной меткой с включенными свободными руками по сенсору 
     */
    void C168706_TestCase();
    
    /*!
     *  \brief         37. Выключение Пляж без метки с выключенными свободными руками по сенсору 
     */
    void C168707_TestCase();
    
    /*!
     *  \brief         38. Выключение Пляж без метки с включенными свободными руками по сенсору 
     */
    void C168708_TestCase();
    
    /*!
     *  \brief         39. Попытка выключить Пляж сервисной кнопкой с запозданием ввода цифры более 5 сек. 
     */
    void C168709_TestCase();
    
    /*!
     *  \brief         40. Попытка выкл. Пляж сервисной кнопкой с неправильным ПИН 
     */
    void C168710_TestCase();
    
    /*!
     *  \brief         1. Нажатие сенсора 1 раз в охране с разрешенным вводом ПИН через сенсор 
     */
    void C168711_TestCase();
    
    /*!
     *  \brief         2. Нажатие сенсора 2 раза в охране с разрешенным вводом ПИН через сенсор 
     */
    void C168712_TestCase();
    
    /*!
     *  \brief         3. Нажатие сенсора 4 раза в охране с разрешенным вводом ПИН через сенсор 
     */
    void C168713_TestCase();
    
    /*!
     *  \brief         4. Нажатие сенсора 1 раз в охране с запрещенным вводом ПИН через сенсор 
     */
    void C168714_TestCase();
    
    /*!
     *  \brief         5. Нажатие сенсора 2 раза в охране с запрещенным вводом ПИН через сенсор  
     */
    void C168715_TestCase();
    
    /*!
     *  \brief         6. Нажатие сенсора 4 раза в охране с запрещенным вводом ПИН через сенсор 
     */
    void C168716_TestCase();
    
    /*!
     *  \brief         7. Появление и пропадание метки в охране без Пляжа  
     */
    void C168717_TestCase();
    
    /*!
     *  \brief         8. Выключение охраны без режима Пляж меткой 
     */
    void C168718_TestCase();
    
    /*!
     *  \brief         9. Выключение охраны без режима Пляж брелоком  
     */
    void C168719_TestCase();
    
    /*!
     *  \brief         10. Выключение охраны сенсором (без метки и с отключенными свободными руками по сенсору) 
     */
    void C168720_TestCase();
    
    /*!
     *  \brief         11. Выключение охраны сенсором (с меткой и с отключенными свободными руками по сенсору)  
     */
    void C168721_TestCase();
    
    /*!
     *  \brief         12. Выключение охраны сенсором (без метки и с включенными свободными руками по сенсору) 
     */
    void C168722_TestCase();
    
    /*!
     *  \brief         13. Выключение охраны сенсором (с меткой и с включенными свободными руками по сенсору)  
     */
    void C168723_TestCase();
    
    /*!
     *  \brief         14. Ввод ПИН без разрашенного снятия сенсором 
     */
    void C168724_TestCase();
    
    /*!
     *  \brief         15. Окончание времени ожидания ввода ПИН (5 мин.)  
     */
    void C168725_TestCase();
    
    /*!
     *  \brief         16. Ввод цифр ПИН с промежутком более 5 сек. 
     */
    void C168726_TestCase();
    
    /*!
     *  \brief         17. Ввод неправильного ПИН  
     */
    void C168727_TestCase();
    
    /*!
     *  \brief         Выключение поддержки зажигания при переходе в сервис 
     */
    void C170779_TestCase();
    
    /*!
     *  \brief         Сброс программной нейтрали при переходе в сервис 
     */
    void C170780_TestCase();
    
    /*!
     *  \brief         Отсутствие автозапуска в сервисе 
     */
    void C170781_TestCase();
    
    /*!
     *  \brief         Отсутствие отпирания ЦЗ в поездке в сервисе 
     */
    void C170782_TestCase();
    
    /*!
     *  \brief         Отсутствие запирания ЦЗ в поездке в сервисе 
     */
    void C170783_TestCase();
    
    /*!
     *  \brief         Невозможность включения антиограбления по метке (меткой) в сервисе  
     */
    void C170784_TestCase();
    
    /*!
     *  \brief         Невозможность выключения антиограбления по метке (меткой) в сервисе  
     */
    void C170785_TestCase();
    
    /*!
     *  \brief         Постановка в охрану голосовой командой в сервисе 
     */
    void C170786_TestCase();
    
    /*!
     *  \brief         Постановка в охрану SMS командой в сервисе 
     */
    void C170787_TestCase();
    
    /*!
     *  \brief         Постановка в охрану брелоком SL в сервисе  
     */
    void C170788_TestCase();
    
    /*!
     *  \brief         Постановка в охрану штатным брелоком в сервисе 
     */
    void C170789_TestCase();
    
    /*!
     *  \brief         Постановка в охрану меткой в сервисе  
     */
    void C170790_TestCase();
    
    /*!
     *  \brief         Постановка в охрану через SLO в сервисе 
     */
    void C170791_TestCase();
    
    /*!
     *  \brief         Постановка в охрану по свободным рукам в сервисе 
     */
    void C170792_TestCase();
    
    /*!
     *  \brief         Снятие с охраны голосовой командой в сервисе  
     */
    void C170793_TestCase();
    
    /*!
     *  \brief         Снятие с охраны SMS командой в сервисе 
     */
    void C170794_TestCase();
    
    /*!
     *  \brief         Снятие с охраны брелоком SL в сервисе 
     */
    void C170795_TestCase();
    
    /*!
     *  \brief         Снятие с охраны штатным брелоком в сервисе 
     */
    void C170796_TestCase();
    
    /*!
     *  \brief         Снятие с охраны меткой в сервисе 
     */
    void C170797_TestCase();
    
    /*!
     *  \brief         Снятие с охраны через SLO в сервисе 
     */
    void C170798_TestCase();
    
    /*!
     *  \brief         Снятие с охраны по свободным рукам в сервисе 
     */
    void C170799_TestCase();
    
    /*!
     *  \brief         Отправка SMS "калибровка" в сервисе 
     */
    void C170800_TestCase();
    
    /*!
     *  \brief         Запрос OBD-ошибок в сервисе 
     */
    void C170801_TestCase();
    
    /*!
     *  \brief         Дистанционный запуск голосовой командой в сервисе 
     */
    void C170802_TestCase();
    
    /*!
     *  \brief         Дистанционный запуск SMS командой в сервисе 
     */
    void C170803_TestCase();
    
    /*!
     *  \brief         Дистанционный запуск брелоком SL в сервисе 
     */
    void C170804_TestCase();
    
    /*!
     *  \brief         Дистанционный запуск штатным брелоком в сервисе  
     */
    void C170805_TestCase();
    
    /*!
     *  \brief         Дистанционный запуск гибкой логикой в сервисе  
     */
    void C170806_TestCase();
    
    /*!
     *  \brief         Дистанционный запуск через SLO в сервисе 
     */
    void C170807_TestCase();
    
    /*!
     *  \brief         Запуск ППП голосовой командой в сервисе 
     */
    void C170808_TestCase();
    
    /*!
     *  \brief         Запуск ППП SMS командой в сервисе 
     */
    void C170809_TestCase();
    
    /*!
     *  \brief         Запуск ППП брелоком SL в сервисе 
     */
    void C170810_TestCase();
    
    /*!
     *  \brief         Запуск ППП штатным брелоком в сервисе  
     */
    void C170811_TestCase();
    
    /*!
     *  \brief         Запуск ППП через SLO в сервисе  
     */
    void C170812_TestCase();
    
    /*!
     *  \brief         Отсутствие подхвата зажигания по турботаймеру в сервисе 
     */
    void C170813_TestCase();
    
    /*!
     *  \brief         Отсутствие подхвата зажигания при установке программной нейтрали в сервисе 
     */
    void C170814_TestCase();
    
    /*!
     *  \brief         Отсутствие подхвата зажигания при постановке в охрану с работающим двигателем в сервисе 
     */
    void C170815_TestCase();
    
    /*!
     *  \brief         Открытие багажника брелоком SL в сервисе 
     */
    void C170816_TestCase();
    
    /*!
     *  \brief         Открытие багажника через SLO в сервисе  
     */
    void C170817_TestCase();
    
    /*!
     *  \brief         Включение паники SMS командой в сервисе  
     */
    void C170818_TestCase();
    
    /*!
     *  \brief         Включение паники брелоком SL в сервисе 
     */
    void C170819_TestCase();
    
    /*!
     *  \brief         Включение паники через SLO в сервисе 
     */
    void C170820_TestCase();
    
    /*!
     *  \brief         Включение свободных рук голосовой командой в сервисе  
     */
    void C170821_TestCase();
    
    /*!
     *  \brief         Включение свободных рук SMS командой в сервисе 
     */
    void C170822_TestCase();
    
    /*!
     *  \brief         Включение свободных рук брелоком SL в сервисе 
     */
    void C170823_TestCase();
    
    /*!
     *  \brief         Включение свободных рук через SLO в сервисе 
     */
    void C170824_TestCase();
    
    /*!
     *  \brief         Выключение свободных рук голосовой командой в сервисе  
     */
    void C170825_TestCase();
    
    /*!
     *  \brief         Выключение свободных рук SMS командой в сервисе 
     */
    void C170826_TestCase();
    
    /*!
     *  \brief         Выключение свободных рук брелоком SL в сервисе  
     */
    void C170827_TestCase();
    
    /*!
     *  \brief         Выключение свободных рук через SLO в сервисе 
     */
    void C170828_TestCase();

    /*!
     *  \brief         Проверка блокировки по CAN 
     */
    void C169639_TestCase();

    /*!
     *  \brief         Проверка запрета запуска по CAN 
     */
    void C169640_TestCase();

    /*!
     *  \brief         1. Блокировка заглушенного двигателя (блок. только заведенного двиг. откл.) 
     */
    void C169641_TestCase();

    /*!
     *  \brief         2. Блокировка заглушенного двигателя (блок. с педалью тормоза вкл.) 
     */
    void C169642_TestCase();

    /*!
     *  \brief         3. Отсутствие блокировки заглушенного двигателя (блок. только заведенного двиг. вкл.) 
     */
    void C169643_TestCase();

    /*!
     *  \brief         4. Блокировка заведенного двигателя (блок. только заведенного двиг. откл.) 
     */
    void C169644_TestCase();

    /*!
     *  \brief         5. Блокировка заведенного двигателя (блок. только заведенного двиг. вкл.) 
     */
    void C169645_TestCase();

    /*!
     *  \brief         6. Включение блокировки в тревоге на заведенном двигателе 
     */
    void C169646_TestCase();

    /*!
     *  \brief         7. Отсутствие блокировки при скорости выше разрешенной 
     */
    void C169647_TestCase();

    /*!
     *  \brief         8. Блокировка с не нажатой педалью тормоза со скоростью меньше 10 км/ч 
     */
    void C169648_TestCase();

    /*!
     *  \brief         9. Блокировка с нажатой педалью тормоза (скорость 10 - разрешенная) 
     */
    void C169649_TestCase();

    /*!
     *  \brief         10. Отсутствие блокировки с не нажатой педалью тормоза с последующим нажатием 
     */
    void C169650_TestCase();

    /*!
     *  \brief         11. Глушение двигателя в тревоге с выключением блокировки
     */
    void C169651_TestCase();

    /*!
     *  \brief         12. Глушение двигателя в тревоге без выключения блокировки 
     */
    void C169652_TestCase();

    /*!
     *  \brief         13. Выключение педали тормоза в тревоге без выключения блокировки 
     */
    void C169653_TestCase();

    /*!
     *  \brief         14. Превышение скорости 10 км/ч в тревоге без выключения блокировки 
     */
    void C169654_TestCase();

    /*!
     *  \brief         15. Превышение скорости выше разрешенной в тревоге без выключения блокировки 
     */
    void C169655_TestCase();

    /*!
     *  \brief         16. Выключение блокировки 
     */
    void C169656_TestCase();

    /*!
     *  \brief         Блокировка заглушенного двигателя с задержкой 
     */
    void C1578122_TestCase();

    /*!
     *  \brief         1. Выключение блокировки при выключении охраны 
     */
    void C169657_TestCase();

    /*!
     *  \brief         2. Включение блокировки при включении охраны   
     */
    void C169658_TestCase();

    /*!
     *  \brief         3. Отсутствие выключения блокировки при выключении охраны 
     */
    void C169659_TestCase();

    /*!
     *  \brief         4. Выключение блокировки при включении зажигания 
     */
    void C169660_TestCase();

    /*!
     *  \brief         5. Включение блокировки при выключении зажигания 
     */
    void C169661_TestCase();

    /*!
     *  \brief         Включение блокировки при выключении зажигания с задержкой 
     */
    void C1578123_TestCase();

    /*!
     *  \brief         22. Выключение перепостановки включением ручника  
     */
    void C171184_TestCase();

    /*!
     *  \brief         23. Выключение перепостановки вЫключением ручника 
     */
    void C171185_TestCase();

    /*!
     *  \brief         24. Выключение перепостановки включением педали тормоза  
     */
    void C171188_TestCase();

    /*!
     *  \brief         25. Выключение перепостановки вЫключением педали тормоза  
     */
    void C171189_TestCase();

    /*!
     *  \brief         Авторизация при мягкой посадке по педали тормоза 
     */
    void C1616162_TestCase();

    /*!
     *  \brief         Авторизация при мягкой посадке по паркингу  
     */
    void C1616163_TestCase();

    /*!
     *  \brief         Авторизация при мягкой посадке по ручному тормозу  
     */
    void C1616164_TestCase();

    /*!
     *  \brief         Код экстренной разблокировки (из охраны) отключает антиограбление по метке  
     */
    void C1624514_TestCase();

    /*!
     *  \brief         Код экстренной разблокировки (из АО) отключает антиограбление по метке  
     */
    void C1624515_TestCase();

    /*!
     *  \brief         Код экстренной разблокировки отключает запрет поездки по снятию  
     */
    void C1624516_TestCase();

    /*!
     *  \brief         Код экстренной разблокировки отключает запрет поездки по зажиганию  
     */
    void C1624517_TestCase();

    /*!
     *  \brief         Выключение охраны кодом с включенными требованиями авторизации  
     */
    void C1624518_TestCase();

    /*!
     *  \brief         Включение АО командой после снятия с охраны кодом  
     */
    void C1624519_TestCase();

    /*!
     *  \brief         Вкл.-выкл. регистрации после снятия кодом не восстанавливает АО  
     */
    void C1624520_TestCase();

    /*!
     *  \brief         Вкл.-выкл. регистрации после снятия кодом не восстанавливает запрет поездки  
     */
    void C1624521_TestCase();

    /*!
     *  \brief         Вкл.-выкл. сервиса после снятия кодом не восстанавливает АО  
     */
    void C1624522_TestCase();

    /*!
     *  \brief         Вкл.-выкл. сервиса после снятия кодом не восстанавливает запрет поездки
     */
    void C1624523_TestCase();

    /*!
     *  \brief         Сработал ДД через 10 сек после включения охраны
     */
    void C1691143_TestCase();

    /*!
     *  \brief         Сработал ДН через 10 сек после включения охраны
     */
    void C1691144_TestCase();

    /*!
     *  \brief         Сработал ДУ предупредительный через 10 сек после включения охраны
     */
    void C1691145_TestCase();

    /*!
     *  \brief         Сработал ДУ тревожный через 10 сек после включения охраны
     */
    void C1691146_TestCase();

    /*!
     *  \brief         Сработал ДД через 20 сек после включения охраны
     */
    void C1691147_TestCase();

    /*!
     *  \brief         Сработал ДН через 20 сек после включения охраны
     */
    void C1691148_TestCase();

    /*!
     *  \brief         Сработал ДУ предупредительный через 20 сек после включения охраны
     */
    void C1691149_TestCase();

    /*!
     *  \brief         Сработал ДУ тревожный через 20 сек после включения охраны
     */
    void C1691150_TestCase();

    /*!
     *  \brief         Сработал ДД через 10 сек после закрывания багажника
     */
    void C1691151_TestCase();

    /*!
     *  \brief         Сработал ДН через 10 сек после закрывания багажника
     */
    void C1691152_TestCase();

    /*!
     *  \brief         Сработал ДУ предупредительный через 10 сек после закрывания багажника
     */
    void C1691153_TestCase();

    /*!
     *  \brief         Сработал ДУ тревожный через 10 сек после закрывания багажника
     */
    void C1691154_TestCase();

    /*!
     *  \brief         Сработал ДД через 20 сек после закрывания багажника
     */
    void C1691155_TestCase();

    /*!
     *  \brief         Сработал ДН через 20 сек после закрывания багажника
     */
    void C1691156_TestCase();

    /*!
     *  \brief         Сработал ДУ предупредительный через 20 сек после закрывания багажника
     */
    void C1691157_TestCase();

    /*!
     *  \brief         Сработал ДУ тревожный через 20 сек после закрывания багажника
     */
    void C1691158_TestCase();

    /*!
     *  \brief         26. Выключение охраны с открытой дверью
     */
    void C1827097_TestCase();

    /*!
     *  \brief         27. Выключение охраны с открытым багажником
     */
    void C1827098_TestCase();

    /*!
     *  \brief         28. Выключение охраны с открытым капотом
     */
    void C1827099_TestCase();

    /*!
     *  \brief         1. Переход в охрану с открытой дверью с включенной настройкой
     */
    void C1950492_TestCase();

    /*!
     *  \brief         2. Переход в охрану с закрыванием двери с включенной настройкой
     */
    void C1950493_TestCase();

    /*!
     *  \brief         3. Переход в охрану с закрытой дверью с включенной настройкой
     */
    void C1950494_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением двери и багажника
     */
    void C1981106_TestCase();

    /*!
     *  \brief         Открыть багажник без объединения двери и багажника
     */
    void C1981107_TestCase();

    /*!
     *  \brief         Открыть дверь с объединением двери и багажника
     */
    void C1981108_TestCase();

    /*!
     *  \brief         Открыть дверь без объединения двери и багажника
     */
    void C1981109_TestCase();

    /*!
     *  \brief         Открыть капот с объединением двери и багажника
     */
    void C1981110_TestCase();

    /*!
     *  \brief         Открыть капот без объединения двери и багажника
     */
    void C1981111_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота и багажника
     */
    void C1981112_TestCase();

    /*!
     *  \brief         Открыть дверь с объединением капота и багажника
     */
    void C1981113_TestCase();

    /*!
     *  \brief         Открыть капот с объединением капота и багажника
     */
    void C1981114_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением двери, капота и багажника
     */
    void C1981115_TestCase();

    /*!
     *  \brief         Открыть дверь с объединением двери, капота и багажника
     */
    void C1981116_TestCase();

    /*!
     *  \brief         Открыть капот с объединением двери, капота и багажника
     */
    void C1981117_TestCase();

    /*!
     *  \brief         Открыть дверь без объединения двери и багажника
     */
    void C1981118_TestCase();

    /*!
     *  \brief         Открыть дверь с объединением двери и багажника без команды, метки и брелока
     */
    void C1981119_TestCase();

    /*!
     *  \brief         Открыть дверь с объединением двери и багажника с командой с брелока (2,2)
     */
    void C1981120_TestCase();

    /*!
     *  \brief         Открыть дверь с объединением двери и багажника с командой с активной меткой/брелоком
     */
    void C1981121_TestCase();

    /*!
     *  \brief         Открыть капот без объединения капота и багажника
     */
    void C1981122_TestCase();

    /*!
     *  \brief         Открыть капот с объединением капота и багажника без команды, метки и брелока
     */
    void C1981123_TestCase();

    /*!
     *  \brief         Открыть капот с объединением капота и багажника с командой с брелока (2,2)
     */
    void C1981124_TestCase();

    /*!
     *  \brief         Открыть капот с объединением капота и багажника с командой с активной меткой/брелоком
     */
    void C1981125_TestCase();

    /*!
     *  \brief         Открыть багажник без объединения двери и багажника без команды, метки и брелока
     */
    void C1981126_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением двери и багажника без команды, метки и брелока
     */
    void C1981127_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением двери и багажника с командой с брелока (2,2)
     */
    void C1981128_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением двери и багажника с командой с активной меткой/брелоком
     */
    void C1981129_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота и багажника без команды, метки и брелока
     */
    void C1981131_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота и багажника с командой с брелока (2,2)
     */
    void C1981132_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота и багажника с командой с активной меткой/брелоком
     */
    void C1981133_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота, двери и багажника без команды, метки и брелока
     */
    void C1981134_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота, двери и багажника с командой с брелока (2,2)
     */
    void C1981135_TestCase();

    /*!
     *  \brief         Открыть багажник с объединением капота, двери и багажника с командой с активной меткой/брелоком
     */
    void C1981136_TestCase();

    /*!
     *  \brief         Открыть дверь с включенным Slave-CAN
     */
    void C2043655_TestCase();

    /*!
     *  \brief         Открыть багажник с включенным Slave-CAN
     */
    void C2043656_TestCase();

    /*!
     *  \brief         Открыть дверь с отключенным Slave
     */
    void C2043657_TestCase();

    /*!
     *  \brief         Открыть багажник с отключенным Slave
     */
    void C2043658_TestCase();

    /*!
     *  \brief         Открыть дверь с включенным аналоговым Slave с выключенной задержкой
     */
    void C2043659_TestCase();

    /*!
     *  \brief         Открыть багажник с включенным аналоговым Slave с выключенной задержкой
     */
    void C2043660_TestCase();

    /*!
     *  \brief         Открыть дверь с включенным аналоговым Slave с включенной задержкой
     */
    void C2043661_TestCase();

    /*!
     *  \brief         Открыть багажник с включенным аналоговым Slave с включенной задержкой
     */
    void C2043662_TestCase();

    /*!
     *  \brief         Сработала предупредительная тревога во время задержки после открытия двери
     */
    void C2043663_TestCase();

    /*!
     *  \brief         Сработала предупредительная тревога во время задержки после открытия багажника
     */
    void C2043664_TestCase();

    /*!
     *  \brief         1. Антиограбление по брелоку с зажиганием и скоростью, и выключение брелоком после понижения скорости
     */
    void C2198656_TestCase();

    /*!
     *  \brief         2. Антиограбление по брелоку с зажиганием и без скорости, и выключение брелоком
     */
    void C2198657_TestCase();

    /*!
     *  \brief         3. Антиограбление по брелоку без зажигания и без скорости, и выключение брелоком
     */
    void C2198658_TestCase();

    /*!
     *  \brief         4. Антиограбление по брелоку без зажигания и без скорости, и выключение брелоком через 30 сек
     */
    void C2198659_TestCase();

    /*!
     *  \brief         5. Антиограбление по брелоку с зажиганием и скоростью, и выключение брелоком с выключением зажигания в течение 30 сек
     */
    void C2198660_TestCase();

    /*!
     *  \brief         6. Антиограбление по брелоку с зажиганием и скоростью, и выключение брелоком через 30 сек
     */
    void C2198661_TestCase();

    /*!
     *  \brief         7. Антиограбление по брелоку с зажиганием и скоростью, и выключение кодом после понижения скорости
     */
    void C2198662_TestCase();

    /*!
     *  \brief         8. Антиограбление по брелоку с зажиганием и без скорости, и выключение кодом
     */
    void C2198663_TestCase();

    /*!
     *  \brief         9. Антиограбление по брелоку без зажигания и без скорости, и выключение кодом
     */
    void C2198664_TestCase();

    /*!
     *  \brief         10. Антиограбление по брелоку без зажигания и без скорости, и выключение кодом через 30 сек 
     */
    void C2198665_TestCase();

    /*!
     *  \brief         11. Антиограбление по брелоку с зажиганием и скоростью, и выключение кодом с выключением зажигания в течение 30 сек
     */
    void C2198666_TestCase();

    /*!
     *  \brief         12. Антиограбление по брелоку с зажиганием и скоростью, и выключение кодом через 30 сек
     */
    void C2198667_TestCase();

    /*!
     *  \brief         13. Антиограбление по брелоку с зажиганием и скоростью, и выключение брелоком без понижения скорости
     */
    void C2198668_TestCase();

    /*!
     *  \brief         14. Антиограбление по брелоку с зажиганием и скоростью, и выключение кодом без понижения скорости
     */
    void C2198669_TestCase();

    /*!
     *  \brief         15. Антиограбление по брелоку без заж. и без скор. с включением зажигания(нарушена зона охраны) без скорости через 30 сек 
     */
    void C2198670_TestCase();

    /*!
     *  \brief         16. Антиограбление по брелоку без заж. и без скор. с включением через 30 сек зажигания со скоростью
     */
    void C2198671_TestCase();

    /*!
     *  \brief         17. Сработал ДД с закрытой дверью
     */
    void C2198672_TestCase();

    /*!
     *  \brief         18. Сработал ДД с открытой и закрытой дверью до этого
     */
    void C2198673_TestCase();

    /*!
     *  \brief         19. Исчезновение брелока без дальнейших последствий
     */
    void C2198674_TestCase();

    /*!
     *  \brief         20. Появление брелока до начала поиска брелока
     */
    void C2198675_TestCase();

    /*!
     *  \brief         21. Появление брелока во время поиска брелока
     */
    void C2198676_TestCase();

    /*!
     *  \brief         22. Появление брелока во время работы внутреннего звукоизлучателя 
     */
    void C2198677_TestCase();

    /*!
     *  \brief         23. Появление брелока во время работы сирены 
     */
    void C2198678_TestCase();

    /*!
     *  \brief         24. Антиограбление по брелоку с зажиганием с выключенным звукоизлучателем
     */
    void C2198679_TestCase();

    /*!
     *  \brief         25. Антиогр. не включилось из-за отключеной настройки
     */
    void C2198680_TestCase();

    /*!
     *  \brief         26. Повторное вкл. антиогр. после отключения антиогр. кодом экстр. снятия
     */
    void C2198681_TestCase();

    /*!
     *  \brief         Антиограбление по метке с отключенной настройкой и включенным антиограблением по брелоку
     */
    void C2198682_TestCase();

    /*!
     *  \brief         Антиограбление по метке с включенным антиограблением по брелоку
     */
    void C2198683_TestCase();

    /*!
     *  \brief         26. Повторное вкл. антиогр. после отключения антиогр. кодом экстр. снятия
     */
    void C2198684_TestCase();

    /*!
     *  \brief         26. Повторное вкл. антиогр. после отключения антиогр. кодом экстр. снятия
     */
    void C2198685_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по двери отсутствует
     */
    void C172480_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по капоту отсутствует
     */
    void C172481_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по багажнику отсутствует
     */
    void C172482_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по зажиганию присутствует
     */ 
    void C172483_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по датчику движения отсутствует
     */
    void C172484_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по датчику наклона отсутствует
     */
    void C172485_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревоге по датчику удара отсутствует
     */
    void C172486_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по двери отсутствует
     */
    void C172487_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по капоту отсутствует
     */
    void C172488_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по багажнику отсутствует
     */
    void C172489_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по зажиганию отсутствует
     */
    void C172490_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по датчику движения отсутствует
     */
    void C172491_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по датчику наклона отсутствует
     */
    void C172492_TestCase();

    /*!
     *  \brief         Блокировка по CAN при прерывании тревоги по датчику удара отсутствует
     */
    void C172493_TestCase();

    /*!
     *  \brief         Блокировка по CAN при дистанционном запуске отсутствует
     */
    void C172494_TestCase();

    /*!
     *  \brief         Блокировка по CAN при дистанционном глушении отсутствует
     */
    void C172495_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревога при заведенном двигателе скорость 40
     *                 (30 разрешенная) отсутствует (блокировка с тормозом выкл)
     */
    void C172496_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревога при заведенном двигателе скорость была 40 
     *                 стала 20 (30 разрешенная) (блокировка с тормозом выкл)
     */
    void C172497_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревога при заведенном двигателе скорость была 40 
     *                 стала 20 (30 разрешенная) (блокировка с тормозом вкл) тормоз нажат
     */
    void C172498_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревога при заведенном двигателе скорость была 40 
     *                 стала 5 (30 разрешенная) (блокировка с тормозом вкл)
     */ 
    void C172499_TestCase();

    /*!
     *  \brief         Нет блокировки по CAN при тревога при заведенном двигателе скорость была  
     *                 40 стала 15 (30 разрешенная) (блокировка с тормозом вкл) тормоз не нажат
     */
    void C172500_TestCase();

    /*!
     *  \brief         Блокировка по CAN при тревога при заведенном двигателе скорость была  
     *                 40 стала 15 (30 разрешенная) (блокировка с тормозом вкл) тормоз нажат
     */
    void C172501_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по двери
     */
    void C172473_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по капоту
     */
    void C172474_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по багажнику
     */
    void C172475_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по зажиганию
     */
    void C172476_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по ДД
     */
    void C172477_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по ДН
     */
    void C172478_TestCase();

    /*!
     *  \brief         Проверка наличия пакетов запрета запуска по CAN при тревоге по ДУ
     */
    void C172479_TestCase();

    /*!
     *  \brief         Проверка отсутствия запирания ЦЗ при снятии сенсором
     */
    void C2405376_TestCase();

    /*!
     *  \brief         Проверка отсутствия лишнего сигнала светодиода при снятии сенсором
     */
    void C2405377_TestCase();

    /*!
     *  \brief         Открыть багажник в охране с разрешением (с включением брелока через 15 сек и поиском 60 сек)
     */
    void C2405378_TestCase();

    /*!
     *  \brief         Открыть багажник в охране после CAN команды на открытие багажника
     */
    void C2405379_TestCase();

    /*!
     *  \brief         Превышение пройденного расстояние
     */
    void C2424871_TestCase();

    /*!
     *  \brief         Превышение заданной скорости
     */
    void C2424872_TestCase();

    /*!
     *  \brief         Превышение заданной скорости (выход По расстоянию)
     */
    void C2424873_TestCase();

    /*!
     *  \brief         Сработал ДД до превышения пройденного расстояния
     */
    void C2424874_TestCase();

    /*!
     *  \brief         Выключение сервиса с СМС оповещением (мин. расстояние)
     */
    void C2424875_TestCase();

    /*!
     *  \brief         Выключение сервиса без СМС оповещения (макс. расстояние)
     */
    void C2424876_TestCase();

    /*!
     *  \brief         Выключение сервиса с СМС оповещением с перерывами движения
     */
    void C2438277_TestCase();

    /*!
     *  \brief         Превышение пройденного расстояния (выход По скорости)
     */
    void C2424877_TestCase();

    /*!
     *  \brief         Сработал ДД до превышения скорости
     */
    void C2424878_TestCase();

    /*!
     *  \brief         Выключение сервиса с СМС оповещением (мин. скорость)
     */
    void C2424879_TestCase();

    /*!
     *  \brief         Выключение сервиса без СМС оповещения (макс. скорость)
     */
    void C2424880_TestCase();

    /*!
     *  \brief         Сработал ДД до 5 секундного превышения скорости
     */
    void C2424881_TestCase();

    /*!
     *  \brief         Сработал ДД до превышения пройденного расстояния и до превышения скорости
     */
    void C2424882_TestCase();

    /*!
     *  \brief         Выключение сервиса по расстоянию с СМС оповещением (мин. расстояние) 
     */
    void C2424883_TestCase();

    /*!
     *  \brief         Выключение сервиса по расстоянию без СМС оповещения (макс. расстояние)
     */
    void C2424884_TestCase();

    /*!
     *  \brief         Выключение сервиса по скорости с СМС оповещением (мин. скорость) 
     */
    void C2424885_TestCase();

    /*!
     *  \brief         Выключение сервиса по скорости без СМС оповещения (макс. скорость)
     */
    void C2424886_TestCase();

    /*!
     *  \brief         Сработал ДД до 5 секундного превышения скорости
     */
    void C2424887_TestCase();

    /*!
     *  \brief         Выключение сервиса по расстоянию и по скорости с СМС оповещением
     */
    void C2424888_TestCase();

    /*!
     *  \brief         Выключение сервиса по расстоянию и по скорости без СМС оповещения
     */
    void C2424889_TestCase();

    /*!
     *  \brief         Постановка в охрану через мобильное приложение по BLE  
     */
    void C2514487_TestCase();
        
    /*!
     *  \brief         Снятие с охраны через мобильное приложение по BLE
     */
    void C2514488_TestCase();

    /*!
     *  \brief         Перепостановка во время ДЗ
     */
    void C2514489_TestCase();

    /*!
     *  \brief         Проверка запрета запуска в состоянии "Охрана"
     */
    void C2728677_TestCase();

    /*!
     *  \brief         Проверка запрета запуска в состоянии "Ожидание авторизации"
     */
    void C2728678_TestCase();

    /*!
     *  \brief         Проверка запрета запуска в состоянии "Запрет поездки"
     */
    void C2728679_TestCase();

    /*!
     *  \brief         Проверка отмены запрета запуска в ДЗ
     */
    void C2728680_TestCase();

    /*!
     *  \brief         Проверка запуска двигателя в состоянии "Снято"
     */
    void C2728681_TestCase();

    /*!
     *  \brief         Проверка запрета запуска в состоянии "Охрана" (настройки выкл)
     */
    void C2728682_TestCase();

    /*!
     *  \brief         Проверка запрета запуска в состоянии "Ожидание авторизации" (настройки выкл)
     */
    void C2728683_TestCase();

    /*!
     *  \brief         Проверка запрета запуска в состоянии "Запрет поездки" (настройки выкл)
     */
    void C2728684_TestCase();

    /*!
     *  \brief         Проверка отмены запрета запуска в ДЗ (настройки выкл)
     */
    void C2728685_TestCase();

    /*!
     *  \brief         Проверка запуска двигателя в состоянии "Снято" (настройки выкл)
     */
    void C2728686_TestCase();

    /*!
     *  \brief         Тревога по питанию (сирена R6)
     */
    void C2799849_TestCase();

    /*!
     *  \brief         Тревога по питанию (сирена штатная)
     */
    void C2799850_TestCase();

    /*!
     *  \brief         Тревога по питанию (сирена R6+штатная)
     */
    void C2799851_TestCase();

    /*!
     *  \brief         Тревога во время ДЗ (старт-стоп 1 импульс)
     */
    void C2799852_TestCase();

    /*!
     *  \brief         Тревога во время ДЗ (старт-стоп 2 импульса)
     */
    void C2799853_TestCase();

    /*!
     *  \brief         Тревога во время ДЗ (старт-стоп 3 импульса)
     */
    void C2799854_TestCase();

    /*!
     *  \brief         Выключение АО из АО
     */
    void C2874561_TestCase();

    /*!
     *  \brief         Выключение АО из АО с неверным кодом
     */
    void C2874562_TestCase();

    /*!
     *  \brief         Выключение АО из Снято
     */
    void C2874563_TestCase();

    /*!
     *  \brief         Выключение АО из Снято с неверным кодом
     */
    void C2874564_TestCase();

    /*!
     *  \brief         Выключение АО из Охраны
     */
    void C2874565_TestCase();

    /*!
     *  \brief         Выключение АО из Охраны с неверным кодом
     */
    void C2874566_TestCase();

    /*!
     *  \brief         Выключение АО из Сервиса
     */
    void C2874567_TestCase();

    /*!
     *  \brief         Выключение АО из Сервиса с неверным кодом
     */
    void C2874568_TestCase();

    /*!
     *  \brief         Выключение АО из Регистрации
     */
    void C2874569_TestCase();

    /*!
     *  \brief         Выключение АО из Регистрации с неверным кодом
     */
    void C2874570_TestCase();

    /*!
     *  \brief         Переход в запрет поездки из охраны через мобильное приложение (сервер)
     */
    void C2874875_TestCase();

    /*!
     *  \brief         Переход в запрет поездки из охраны через мобильное приложение (BLE)
     */
    void C2874876_TestCase();

    /*!
     *  \brief         Переход в авторизацию владельца из охраны через мобильное приложение (сервер)
     */
    void C2874877_TestCase();

    /*!
     *  \brief         Тревога с окончанием по Штатной тревоге (тревога штатки)
     */
    void C2898585_TestCase();

    /*!
     *  \brief         Тревога по Штатной тревоге (тревога штатки)
     */
    void C2898586_TestCase();

    /*!
     *  \brief         Цикл тревоги с окончанием по Штатной тревоге (тревога штатки)
     */
    void C2898587_TestCase();

    /*!
     *  \brief         Игнорирование тревоги по Штатной тревоге (тревога штатки) после 8 срабатываний и сброс игнорирования перепостановкой в охрану
     */
    void C2898588_TestCase();

    /*!
     *  \brief         Игнорирование тревоги по Штатной тревоге (тревога штатки) после 8 срабатываний цикла и сброс игнорирования перепостановкой в охрану
     */
    void C2898589_TestCase();

    /*!
     *  \brief         Игнорирование тревоги по Штатной тревоге (тревога штатки) после 8 срабатываний полного цикла и сброс игнорирования перепостановкой в охрану
     */
    void C2907768_TestCase();

    /*!
     *  \brief         Удачный запуск двигателя после АО по метке
     */
    void C2907769_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра 
     */
    void C172464_TestCase();

    /*!
     *  \brief         Постановка в охрану любым способом с нарушением периметра (не капот) 
     */
    void C172466_TestCase();

    /*!
     *  \brief         Постановка в охрану любым способом с нарушением периметра (капот) 
     */
    void C172467_TestCase();

    /*!
     *  \brief         Закрыли капот (в охране с нарушенным периметром по капоту)
     */
    void C172468_TestCase();

    /*!
     *  \brief         Снятие с охраны
     */
    void C172469_TestCase();

    /*!
     *  \brief         Тревога (не по капоту)
     */
    void C172470_TestCase();

    /*!
     *  \brief         Тревога (по капоту, капот остался открыт)
     */
    void C172471_TestCase();

    /*!
     *  \brief         Тревога (по капоту, капот закрылся)
     */
    void C172472_TestCase();

    /*!
     *  \brief         Постановка в охрану (охрана уже включена)
     */
    void C1662249_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра
     */
    void C1662256_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра
     */
    void C1662257_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра
     */
    void C1662258_TestCase();

    /*!
     *  \brief         Снятие с охраны
     */
    void C1662259_TestCase();

    /*!
     *  \brief         Постановка в охрану (охрана уже включена)
     */
    void C1662260_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра
     */
    void C1662261_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра
     */
    void C1662262_TestCase();

    /*!
     *  \brief         Постановка в охрану без нарушения периметра
     */
    void C1662263_TestCase();

    /*!
     *  \brief         Снятие с охраны
     */
    void C1662264_TestCase();

    /*!
     *  \brief         Постановка в охрану (охрана уже включена)
     */
    void C1662265_TestCase();

    /*!
     *  \brief         Закрывание капота в Снято
     */
    void C169588_TestCase();

    /*!
     *  \brief         Закрывание капота в Охране
     */
    void C169589_TestCase();

    /*!
     *  \brief         Включение блокировки при тревоге
     */
    void C169590_TestCase();

    /*!
     *  \brief         Выключная сирена
     */
    void C169591_TestCase();

    /*!
     *  \brief         Включение Сирены
     */
    void C169592_TestCase();

    /*!
     *  \brief         Включение Клаксона
     */
    void C169593_TestCase();

    /*!
     *  \brief         Включение охраны без концевика капота
     */
    void C169594_TestCase();

    /*!
     *  \brief         Включение охраны с концевиком подключенным к R6
     */
    void C169595_TestCase();

    /*!
     *  \brief         Включение охраны с концевиком подключенным к основному блоку
     */
    void C169596_TestCase();

    /*!
     *  \brief         Выключение охраны без концевика капота
     */
    void C169597_TestCase();

    /*!
     *  \brief         Выключение охраны с концевиком подключенным к R6
     */
    void C169598_TestCase();

    /*!
     *  \brief         Выключение охраны с концевиком подключенным к основному блоку
     */
    void C169599_TestCase();

    /*!
     *  \brief         Неудачное включение опасной парковки из-за не прописанных кнопок
     */
    void C3052200_TestCase();

    /*!
     *  \brief         Включение опасной парковки
     */
    void C3052201_TestCase();

    /*!
     *  \brief         Выключение опасной парковки
     */
    void C3052202_TestCase();

    /*!
     *  \brief         Выключение запрета поездки (по охране) кодом штатных кнопок в опасной парковке
     */
    void C3052203_TestCase();

    /*!
     *  \brief         Неудачное выключение запрета поездки (по охране) меткой в опасной парковке
     */
    void C3052204_TestCase();

    /*!
     *  \brief         Неудачное выключение опасной парковки в запрете поездки (по охране)
     */
    void C3052205_TestCase();

    /*!
     *  \brief         Выключение запрета поездки (по зажиганию) кодом штатных кнопок в опасной парковке
     */
    void C3052206_TestCase();

    /*!
     *  \brief         Неудачное выключение запрета поездки (по зажиганию) меткой в опасной парковке
     */
    void C3052207_TestCase();

    /*!
     *  \brief         Неудачное выключение опасной парковки в запрете поездки (по зажиганию)
     */
    void C3052208_TestCase();

    /*!
     *  \brief         Неудачное выключение опасной парковки в запрете поездки (по зажиганию)
     */
    void C2889927_TestCase();

    /*!
     *  \brief         Появление метки до включения зажигания
     */
    void C3136858_TestCase();

    /*!
     *  \brief         Появление метки после включения зажигания
     */
    void C3136859_TestCase();

    /*!
     *  \brief         Появление метки до выключения охраны
     */
    void C3136860_TestCase();

    /*!
     *  \brief         Сирена на R6 при включении и выключении охраны
     */
    void C3136861_TestCase();

    /*!
     *  \brief         Сирена на R6 во время тревоги
     */
    void C3136862_TestCase();

    /*!
     *  \brief         Выход из сервиса по расстоянию с включенной меткой и авторизация
     */
    void C3136863_TestCase();

    /*!
     *  \brief         Выход из сервиса по скорости с включенной меткой и авторизация
     */
    void C3136864_TestCase();

    /*!
     *  \brief         Выход из сервиса по расстоянию с выключенной меткой и авторизация
     */
    void C3136865_TestCase();

    /*!
     *  \brief         Выход из сервиса по скорости с выключенной меткой и авторизация
     */
    void C3136866_TestCase();

    /*!
     *  \brief         Отсутствие АО по метке при включенном паркинге
     */
    void C3186729_TestCase();

    /*!
     *  \brief         Отсутствие АО по брелоку при включенном паркинге
     */
    void C3186730_TestCase();

    /*!
     *  \brief         Включение настройки по BLE и выключение кнопкой
     */
    void C3202904_TestCase();

    /*!
     *  \brief         Включение настройки по BLE и выключение по времени
     */
    void C3202905_TestCase();

    /*!
     *  \brief         Отсутствие АО по метке при включенной ПЗ
     */
    void C3357809_TestCase();

    /*!
     *  \brief         Отсутствие АО по брелоку включенной ПЗ
     */
    void C3357810_TestCase();

    /*!
     *  \brief         Постановка в охрану по свободным рукам по таймеру после открытия двери
     */
    void C3407086_TestCase();

    /*!
     *  \brief         Постановка в охрану по свободным рукам по таймеру после закрытия двери
     */
    void C3407087_TestCase();

    /*!
     *  \brief         Выключение охраны с нарушенным периметром с включенными свободными руками
     */
    void C3413361_TestCase();

    /*!
     *  \brief         Нарушение и восстановление периметра вне охраны с включенными свободными руками
     */
    void C3413362_TestCase();

    /*!
     *  \brief         19. Выключение охраны приложением с выключенным запретом снятия
     */
    void C20445_TestCase();

    /*!
     *  \brief         30. Выключение охраны приложением с включенным запретом снятия брелоком с активной меткой
     */
    void C20461_TestCase();

    /*!
     *  \brief         31. Выключение охраны приложением с включенным запретом снятия звонком/смс с активной меткой
     */
    void C20462_TestCase();

    /*!
     *  \brief         32. Выключение охраны приложением с включенным запретом снятия приложением/SLO с активной меткой
     */
    void C20463_TestCase();

    /*!
     *  \brief         Открылась дверь во время имитации двери после ДЗ
     */
    void C3688075_TestCase();

    /*!
     *  \brief         Открылся багажник во время охраны с открытым багажником
     */
    void C3688076_TestCase();

    /*!
     *  \brief         ДД не влияет на перепостановку
     */
    void C3688077_TestCase();

    /*!
     *  \brief         ДН не влияет на перепостановку
     */
    void C3688078_TestCase();

    /*!
     *  \brief         ДУ не влияет на перепостановку
     */
    void C3688079_TestCase();

    /*!
     *  \brief         Перепостановка срабатывает во время имитации двери после ДЗ
     */
    void C3697245_TestCase();

    /*!
     *  \brief         Закрытие ЦЗ (по скорости) после автоматического выхода из сервиса
     */
    void C3752984_TestCase();

    /*!
     *  \brief         ПН (по ручнику) после автоматического выхода из сервиса
     */
    void C3752985_TestCase();

    /*!
     *  \brief         ПН (автоматически) после автоматического выхода из сервиса
     */
    void C3752986_TestCase();

    /*!
     *  \brief         Закрытие ЦЗ (по ручнику) после автоматического выхода из сервиса
     */
    void C3752987_TestCase();

    /*!
     *  \brief         1. Выключение блокировки при выключении охраны  
     */
    void C187473_TestCase();

    /*!
     *  \brief         2. Включение блокировки при включении охраны
     */
    void C187474_TestCase();

    /*!
     *  \brief         3. Отсутствие выключения блокировки при выключении охраны
     */
    void C187475_TestCase();

    /*!
     *  \brief         4. Выключение блокировки при включении зажигания
     */
    void C187476_TestCase();

    /*!
     *  \brief         5. Включение блокировки при выключении зажигания
     */
    void C187477_TestCase();

    /*!
     *  \brief         1. Блокировка заглушенного двигателя (блок. только заведенного двиг. откл.)
     */
    void C187457_TestCase();

    /*!
     *  \brief         2. Блокировка заглушенного двигателя (блок. с педалью тормоза вкл.)
     */
    void C187458_TestCase();

    /*!
     *  \brief         3. Отсутствие блокировки заглушенного двигателя (блок. только заведенного двиг. вкл.)
     */
    void C187459_TestCase();

    /*!
     *  \brief         4. Блокировка заведенного двигателя (блок. только заведенного двиг. откл.)
     */
    void C187460_TestCase();

    /*!
     *  \brief         5. Блокировка заведенного двигателя (блок. только заведенного двиг. вкл.)
     */
    void C187461_TestCase();

    /*!
     *  \brief         6. Включение блокировки в тревоге на заведенном двигателе
     */
    void C187462_TestCase();

    /*!
     *  \brief         7. Отсутствие блокировки при скорости выше разрешенной
     */
    void C187463_TestCase();

    /*!
     *  \brief         8. Блокировка с не нажатой педалью тормоза со скоростью меньше 10 км/ч
     */
    void C187464_TestCase();

    /*!
     *  \brief         9. Блокировка с нажатой педалью тормоза (скорость 10 - разрешенная)
     */
    void C187465_TestCase();

    /*!
     *  \brief         10. Отсутствие блокировки с не нажатой педалью тормоза с последующим нажатием
     */
    void C187466_TestCase();

    /*!
     *  \brief         11. Глушение двигателя в тревоге с выключением блокировки
     */
    void C187467_TestCase();

    /*!
     *  \brief         12. Глушение двигателя в тревоге без выключения блокировки
     */
    void C187468_TestCase();

    /*!
     *  \brief         13. Выключение педали тормоза в тревоге без выключения блокировки
     */
    void C187469_TestCase();

    /*!
     *  \brief         14. Превышение скорости 10 км/ч в тревоге без выключения блокировки
     */
    void C187470_TestCase();

    /*!
     *  \brief         15. Превышение скорости выше разрешенной в тревоге без выключения блокировки
     */
    void C187471_TestCase();

    /*!
     *  \brief         16. Выключение блокировки
     */
    void C187472_TestCase();

    /*!
     *  \brief         17. Пропала связь с ЦБ, зажигание вкл
     */
    void C187478_TestCase();

    /*!
     *  \brief         18. Пропала связь с ЦБ, зажигание выкл, движения нет
     */
    void C187479_TestCase();

    /*!
     *  \brief         20. Пропала связь с ЦБ и восстановилась менее чем через 60 сек
     */
    void C187481_TestCase();

    /*!
     *  \brief         21. Связь с ЦБ восстановилась более чем через 60 сек
     */
    void C187482_TestCase();

    /*!
     *  \brief         Прерывание тревоги по ДД без дозапирания
     */
    void C3777171_TestCase();

    /*!
     *  \brief         Прерывание тревоги по ДН без дозапирания
     */
    void C3777172_TestCase();

    /*!
     *  \brief         Прерывание тревоги по ДУ без дозапирания
     */
    void C3777173_TestCase();

    /*!
     *  \brief         Открытие багажника с включенным брелоком
     */
    void C4045282_TestCase();

    /*!
     *  \brief         Открытие багажника с включенной меткой
     */
    void C4045283_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенным брелоком с последующим включением
     */
    void C4045284_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенной меткой с последующим включением
     */
    void C4045285_TestCase();

    /*!
     *  \brief         Открытие багажника с включенным брелоком
     */
    void C4045286_TestCase();

    /*!
     *  \brief         Открытие багажника с включенной меткой
     */
    void C4045287_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенным брелоком с последующим включением
     */
    void C4045288_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенной меткой с последующим включением
     */
    void C4045289_TestCase();

    /*!
     *  \brief         Открытие багажника с включенным брелоком
     */
    void C4045290_TestCase();

    /*!
     *  \brief         Открытие багажника с включенной меткой
     */
    void C4045291_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенным брелоком с последующим включением
     */
    void C4045292_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенной меткой с последующим включением
     */
    void C4045293_TestCase();

    /*!
     *  \brief         Открытие багажника с включенным брелоком
     */
    void C4045294_TestCase();

    /*!
     *  \brief         Открытие багажника с включенной меткой
     */
    void C4045295_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенным брелоком с последующим включением
     */
    void C4045296_TestCase();

    /*!
     *  \brief         Открытие багажника с выключенной меткой с последующим включением
     */
    void C4045297_TestCase();

    /*!
     *  \brief         1. Включение зажигания
     */
    void C187456_TestCase();

    /*!
     *  \brief         Выключение сервиса по расстоянию
     */
    void C4363738_TestCase();

    /*!
     *  \brief         Выключение сервиса по скорости
     */
    void C4363739_TestCase();

    /*!
     *  \brief         1. Включение зажигания в запрете поездки при наличии метки
     */
    void C4466808_TestCase();

    /*!
     *  \brief         2. Исчезновение метки в запрете поездки при выключенном зажигании
     */
    void C4466809_TestCase();

    /*!
     *  \brief         3. Появление метки в запрете поездки при выключенном зажигании
     */
    void C4466810_TestCase();

    /*!
     *  \brief         4. Включение зажигания в запрете поездки при отсутствии метки
     */
    void C4466811_TestCase();

    /*!
     *  \brief         5. Выключение зажигания в запрете поездки при отсутствии метки
     */
    void C4466812_TestCase();

    /*!
     *  \brief         6. Появление метки в запрете поездки при включенном зажигании
     */
    void C4466813_TestCase();

    /*!
     *  \brief         7. Выключение зажигания в снято при отсутствии метки
     */
    void C4466814_TestCase();

    /*!
     *  \brief         8. Появление метки в снято при включенном зажигании
     */
    void C4466815_TestCase();

    /*!
     *  \brief         9. Исчезновение метки в снято при включенном зажигании
     */
    void C4466816_TestCase();

    /*!
     *  \brief         10. Выключение зажигания в снято при наличии метки
     */
    void C4466817_TestCase();

    /*!
     *  \brief         Исчезновение метки при наличии брелока
     */
    void C4360828_TestCase();

    /*!
     *  \brief         Исчезновение брелока при наличии метки
     */
    void C4360829_TestCase();

    /*!
     *  \brief         Исчезновение метки при отсутствии брелока
     */
    void C4360830_TestCase();

    /*!
     *  \brief         Исчезновение брелока при отсутствии метки
     */
    void C4360831_TestCase();

    /*!
     *  \brief         Исчезновение метки при наличии брелока
     */
    void C4360832_TestCase();

    /*!
     *  \brief         Исчезновение брелока при наличии метки
     */
    void C4360833_TestCase();

    /*!
     *  \brief         Исчезновение метки при отсутствии брелока
     */
    void C4360834_TestCase();

    /*!
     *  \brief         Исчезновение брелока при отсутствии метки
     */
    void C4360835_TestCase();

    /*!
     *  \brief         Исчезновение метки при наличии брелока
     */
    void C4360836_TestCase();

    /*!
     *  \brief         Исчезновение брелока при наличии метки
     */
    void C4360837_TestCase();

    /*!
     *  \brief         Исчезновение метки при отсутствии брелока
     */
    void C4360838_TestCase();

    /*!
     *  \brief         Исчезновение брелока при отсутствии метки
     */
    void C4360839_TestCase();

    /*!
     *  \brief         Исчезновение метки при наличии брелока
     */
    void C4360840_TestCase();

    /*!
     *  \brief         Исчезновение брелока при наличии метки
     */
    void C4360841_TestCase();

    /*!
     *  \brief         Исчезновение метки при отсутствии брелока
     */
    void C4360842_TestCase();

    /*!
     *  \brief         Исчезновение брелока при отсутствии метки
     */
    void C4360843_TestCase();

    /*!
     *  \brief         Открытие двери в охране с включенной меткой
     */
    void C4559714_TestCase();

    /*!
     *  \brief         Открытие двери водителя в охране с включенной меткой 
     */
    void C4559715_TestCase();

    /*!
     *  \brief         Открытие двери пассажира в охране с включенной меткой
     */
    void C4559716_TestCase();

    /*!
     *  \brief         Открытие багажника в охране с включенной меткой
     */
    void C4559717_TestCase();

    /*!
     *  \brief         Включить метку во время шага 1 авторизации после открытия двери
     */
    void C4559718_TestCase();

    /*!
     *  \brief         Включить метку во время шага 1 авторизации после открытия двери водителя
     */
    void C4559719_TestCase();

    /*!
     *  \brief         Включить метку во время шага 1 авторизации после открытия двери пассажира
     */
    void C4559720_TestCase();

    /*!
     *  \brief         Включить метку во время шага 1 авторизации после открытия багажника
     */
    void C4559721_TestCase();

    /*!
     *  \brief         Включить метку во время шага 2 авторизации после открытия двери
     */
    void C4559722_TestCase();

    /*!
     *  \brief         Включить метку во время шага 2 авторизации после открытия двери водителя
     */
    void C4559723_TestCase();

    /*!
     *  \brief         Включить метку во время шага 2 авторизации после открытия двери пассажира
     */
    void C4559724_TestCase();

    /*!
     *  \brief         Включить метку во время шага 2 авторизации после открытия багажника
     */
    void C4559725_TestCase();

    /*!
     *  \brief         Включить метку во время шага 3 авторизации после открытия двери
     */
    void C4559726_TestCase();

    /*!
     *  \brief         Включить метку во время шага 3 авторизации после открытия двери водителя
     */
    void C4559727_TestCase();

    /*!
     *  \brief         Включить метку во время шага 3 авторизации после открытия двери пассажира
     */
    void C4559728_TestCase();

    /*!
     *  \brief         Включить метку во время шага 3 авторизации после открытия багажника
     */
    void C4559729_TestCase();

    /*!
     *  \brief         НЗ блокировка заглушенного двигателя
     */
    void C4559732_TestCase();

    /*!
     *  \brief         НЗ блокировка заведенного двигателя 
     */
    void C4559733_TestCase();

    /*!
     *  \brief         НР блокировка при снятии с охраны
     */
    void C4559734_TestCase();

    /*!
     *  \brief         НР блокировка при включении зажигания
     */
    void C4559735_TestCase();

    /*!
     *  \brief         НР блокировка при включении зажигания в охране
     */
    void C4559751_TestCase();

    /*!
     *  \brief         Переключение из паркинга в антиограблении
     */
    void C4686621_TestCase();

    /*!
     *  \brief         Переключение из паркинга в антиограблении по метке
     */
    void C4686622_TestCase();

    /*!
     *  \brief         Переключение из паркинга в снято
     */
    void C4695722_TestCase();

    /*!
     *  \brief         Переключение из паркинга в тревоге с переключением в паркинг
     */
    void C4695723_TestCase();

    /*!
     *  \brief         Переключение из паркинга в охране с переключением в паркинг
     */
    void C4695724_TestCase();

    /*!
     *  \brief         Переключение из паркинга в авторизации с переключением в паркинг
     */
    void C4695725_TestCase();

    /*!
     *  \brief         Переключение из паркинга в запрете поездки с переключением в паркинг
     */
    void C4695726_TestCase();

    /*!
     *  \brief         Переключение из паркинга в охране с открытым багажником с переключением в паркинг
     */
    void C4695727_TestCase();

    /*!
     *  \brief         Переключение из паркинга в предупредительной тревоге с переключением в паркинг
     */
    void C4695728_TestCase();

    /*!
     *  \brief         Переключение из паркинга в тревоге с переходом в снято
     */
    void C4695729_TestCase();

    /*!
     *  \brief         Переключение из паркинга в охране с переходом в снято
     */
    void C4695730_TestCase();

    /*!
     *  \brief         Переключение из паркинга в авторизации с переходом в снято
     */
    void C4695731_TestCase();

    /*!
     *  \brief         Переключение из паркинга в запрете поездки с переходом в снято
     */
    void C4695732_TestCase();

    /*!
     *  \brief         Переключение из паркинга в охране с открытым багажником с переходом в снято
     */
    void C4695733_TestCase();

    /*!
     *  \brief         Переключение из паркинга в предупредительной тревоге с переходом в снято
     */
    void C4695734_TestCase();

    /*!
     *  \brief         Авторизация по метке до команды открытия багажника (метка исчезает)
     */
    void C4801978_TestCase();

    /*!
     *  \brief         Авторизация по брелоку до команды открытия багажника (брелок исчезает)
     */
    void C4801979_TestCase();

    /*!
     *  \brief         Авторизация по метке после команды открытия багажника (метка исчезает)
     */
    void C4801980_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после команды открытия багажника (брелок исчезает)
     */
    void C4801981_TestCase();

    /*!
     *  \brief         Авторизация по метке после открытия багажника (метка исчезает)
     */
    void C4801982_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после открытия багажника (брелок исчезает)
     */
    void C4801983_TestCase();

    /*!
     *  \brief         Авторизация по метке после наступления тревоги (метка исчезает)
     */
    void C4801984_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после наступления тревоги (брелок исчезает)
     */
    void C4801985_TestCase();

    /*!
     *  \brief         Авторизация по метке до команды открытия багажника (метка появляется и исчезает)
     */
    void C4828410_TestCase();

    /*!
     *  \brief         Авторизация по брелоку до команды открытия багажника (брелок появляется и исчезает)
     */
    void C4828411_TestCase();

    /*!
     *  \brief         Авторизация по метке после команды открытия багажника (метка появляется и исчезает)
     */
    void C4828412_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после команды открытия багажника (брелок появляется и исчезает)
     */
    void C4828413_TestCase();

    /*!
     *  \brief         Авторизация по метке после открытия багажника (метка появляется и исчезает)
     */
    void C4828414_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после открытия багажника (брелок появляется и исчезает)
     */
    void C4828415_TestCase();

    /*!
     *  \brief         Авторизация по метке после наступления тревоги (метка появляется и исчезает)
     */
    void C4828416_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после наступления тревоги (брелок появляется и исчезает)
     */
    void C4828417_TestCase();

    /*!
     *  \brief         Авторизация по метке до команды открытия багажника (метка появляется)
     */
    void C4828418_TestCase();

    /*!
     *  \brief         Авторизация по брелоку до команды открытия багажника (брелок появляется)
     */
    void C4828419_TestCase();

    /*!
     *  \brief         Авторизация по метке после команды открытия багажника (метка появляется)
     */
    void C4828420_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после команды открытия багажника (брелок появляется)
     */
    void C4828421_TestCase();

    /*!
     *  \brief         Авторизация по метке после открытия багажника (метка появляется)
     */
    void C4828422_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после открытия багажника (брелок появляется)
     */
    void C4828423_TestCase();

    /*!
     *  \brief         Авторизация по метке после наступления тревоги (метка появляется)
     */
    void C4828424_TestCase();

    /*!
     *  \brief         Авторизация по брелоку после наступления тревоги (брелок появляется)
     */
    void C4828425_TestCase();

    /*!
     *  \brief         Сброс питания ЦБ и R6
     */
    void C4869745_TestCase();

    /*!
     *  \brief         Сброс питания ЦБ
     */
    void C4869746_TestCase();

    /*!
     *  \brief         Сброс питания R6
     */
    void C4869747_TestCase();

    /*!
     *  \brief         Закрытие ЦЗ
     */
    void C4886622_TestCase();

    /*!
     *  \brief         Открытие ЦЗ
     */
    void C4886623_TestCase();

    /*!
     *  \brief         1 Потеря метки не приводит к включению ЗП
     */
    void C4930044_TestCase();

    /*!
     *  \brief         2 Включение зажигания с меткой до включения ЗП
     */
    void C4930045_TestCase();

    /*!
     *  \brief         3 Включение ЗП с меткой после выключения зажигания
     */
    void C4930046_TestCase();

    /*!
     *  \brief         4 Потеря метки до включения ЗП
     */
    void C4930047_TestCase();

    /*!
     *  \brief         5 Включение зажигания без метки до включения ЗП
     */
    void C4930048_TestCase();

    /*!
     *  \brief         6 Включение ЗП без метки после выключения зажигания
     */
    void C4930049_TestCase();

    /*!
     *  \brief         7 Метка найдена до включения ЗП
     */
    void C4930050_TestCase();

    /*!
     *  \brief         Макс. время до активации ЗП
     */
    void C4930051_TestCase();

    /*!
     *  \brief         Звуковые извещатели отсутствуют
     */
    void C9257883_TestCase();

    /*!
     *  \brief         Есть внутренний звуковой извещатель
     */
    void C9257884_TestCase();

    /*!
     *  \brief         Есть внешний звуковой извещатель
     */
    void C9257885_TestCase();

    /*!
     *  \brief         Есть внутренний и внешний звуковые извещатели
     */
    void C9257886_TestCase();

    /*!
     *  \brief         Мин. время ожидания авторизации
     */
    void C9257887_TestCase();

    /*!
     *  \brief         Макс. время ожидания авторизации
     */
    void C9257888_TestCase();

    /*!
     *  \brief         Время поиска метки больше времени ввода кода
     */
    void C9257889_TestCase();

    /*!
     *  \brief         Время поиска метки меньше времени ввода кода
     */
    void C9257890_TestCase();

    /*!
     *  \brief         Появление метки без брелока
     */
    void C9377438_TestCase();

    /*!
     *  \brief         Появление метки с брелоком
     */
    void C9377439_TestCase();

    /*!
     *  \brief         Появление брелока без метки
     */
    void C9377440_TestCase();

    /*!
     *  \brief         Появление брелока с меткой
     */
    void C9377441_TestCase();

    /*!
     *  \brief         1. Тревога не по ДД
     */
    void C9491969_TestCase();

    /*!
     *  \brief         2. Сработал ДД в тревоге с превышенной скоростью
     */
    void C9491970_TestCase();

    /*!
     *  \brief         3. Сработал ДД в тревоге с допустимой скоростью
     */
    void C9491971_TestCase();

    /*!
     *  \brief         4. Скорость снизилась после тревоги по ДД
     */
    void C9491972_TestCase();

    /*!
     *  \brief         5. Скорость снизилась после тревоги не по ДД
     */
    void C9491973_TestCase();

    /*!
     *  \brief         5. Перезапуск блокировки по ДД
     */
    void C9491974_TestCase();

    /*!
     *  \brief         6. Перезапуск блокировки не по ДД
     */
    void C9491975_TestCase();

    /*!
     *  \brief         7. Блокировка истекла в тревоге
     */
    void C9491976_TestCase();

    /*!
     *  \brief         8. Блокировка истекла в охране
     */
    void C9491977_TestCase();

    /*!
     *  \brief         9. Блокировка с отключенным ДД
     */
    void C9491978_TestCase();

    /*!
     *  \brief         Блокировка с максимальным временем
     */
    void C9491979_TestCase();

    /*!
     *  \brief         Блокировка с минимальным временем
     */
    void C9491980_TestCase();

    /*!
     *  \brief         Блокировка с максимальной задержкой
     */
    void C9491981_TestCase();

    /*!
     *  \brief         Блокировка с минимальной задержкой
     */
    void C9491982_TestCase();

    /*!
     *  \brief         10. Тревога по ДД с превышенной скоростью
     */
    void C9501309_TestCase();

    /*!
     *  \brief         11. Тревога по ДД с допустимой скоростью
     */
    void C9501310_TestCase();

    /*!
     *  \brief         Блокировка с нажатой педалью с разрешением на блокировку без педали
     */
    void C9501311_TestCase();

    /*!
     *  \brief         Блокировка с не нажатой педалью с разрешением на блокировку без педали
     */
    void C9501312_TestCase();

    /*!
     *  \brief         Блокировка с нажатой педалью с запретом на блокировку без педали
     */
    void C9501313_TestCase();

    /*!
     *  \brief         Блокировка с не нажатой педалью с запретом на блокировку без педали
     */
    void C9501314_TestCase();

    /*!
     *  \brief         1. Включение АО без нажатой педали и без требования нажатия педали (скорость от 10 до допустимой)
     */
    void C9501315_TestCase();

    /*!
     *  \brief         2. Включение АО с нажатой педалью и без требования нажатия педали (скорость от 10 до допустимой)
     */
    void C9501316_TestCase();

    /*!
     *  \brief         3. Включение АО с нажатой педалью с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9501317_TestCase();

    /*!
     *  \brief         4. Повторное включение блокировки в АО без педали (скорость от 10 до допустимой)
     */
    void C9501318_TestCase();

    /*!
     *  \brief         5. Включение АО без нажатой педали с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9501319_TestCase();

    /*!
     *  \brief         6. Включение АО без нажатой педали и без требования нажатия педали (нет данных о скорости)
     */
    void C9501320_TestCase();

    /*!
     *  \brief         7. Включение АО с нажатой педалью и без требования нажатия педали (нет данных о скорости)
     */
    void C9501321_TestCase();

    /*!
     *  \brief         8. Включение АО с нажатой педалью с требованием нажатия педали (нет данных о скорости)
     */
    void C9501322_TestCase();

    /*!
     *  \brief         9. Повторное включение блокировки в АО без педали (нет данных о скорости)
     */
    void C9501323_TestCase();

    /*!
     *  \brief         10. Включение АО без нажатой педали с требованием нажатия педали (нет данных о скорости)
     */
    void C9501324_TestCase();

    /*!
     *  \brief         11. Включение АО без нажатой педали и без требования нажатия педали (скорость выше допустимой)
     */
    void C9508181_TestCase();

    /*!
     *  \brief         12. Включение АО с нажатой педалью и без требования нажатия педали (скорость выше допустимой)
     */
    void C9508182_TestCase();

    /*!
     *  \brief         13. Включение АО с нажатой педалью с требованием нажатия педали (скорость выше допустимой)
     */
    void C9508183_TestCase();

    /*!
     *  \brief         14. Включение АО без нажатой педали с требованием нажатия педали (скорость выше допустимой)
     */
    void C9508184_TestCase();

    /*!
     *  \brief         15. Включение АО без нажатой педали и без требования нажатия педали (скорость ниже 10)
     */
    void C9508185_TestCase();

    /*!
     *  \brief         16. Включение АО с нажатой педалью и без требования нажатия педали (скорость ниже 10)
     */
    void C9508186_TestCase();

    /*!
     *  \brief         17. Включение АО с нажатой педалью с требованием нажатия педали (скорость ниже 10)
     */
    void C9508187_TestCase();

    /*!
     *  \brief         18. Включение АО без нажатой педали с требованием нажатия педали (скорость ниже 10)
     */
    void C9508188_TestCase();

    /*!
     *  \brief         19. 180 сек. АО без нажатой педали и без требования нажатия педали (скорость выше допустимой)
     */
    void C9508189_TestCase();

    /*!
     *  \brief         20. 180 сек. АО с нажатой педалью и без требования нажатия педали (скорость выше допустимой)
     */
    void C9508190_TestCase();

    /*!
     *  \brief         21. 180 сек. АО с нажатой педалью с требованием нажатия педали (скорость выше допустимой)
     */
    void C9508191_TestCase();

    /*!
     *  \brief         22. 180 сек. АО без нажатой педали с требованием нажатия педали (скорость выше допустимой)
     */
    void C9508192_TestCase();

    /*!
     *  \brief         23. 180 сек. АО без нажатой педали с требованием нажатия педали (нет данных о скорости)
     */
    void C9508193_TestCase();

    /*!
     *  \brief         24. 180 сек. АО без нажатой педали с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9508194_TestCase();

    /*!
     *  \brief         25. Нажатие педали в АО с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9518545_TestCase();

    /*!
     *  \brief         26. Нажатие педали в АО с требованием нажатия педали (нет данных о скорости)
     */
    void C9518546_TestCase();

    /*!
     *  \brief         27. Нажатие педали в АО с требованием нажатия педали (скорость выше допустимой)
     */
    void C9518547_TestCase();

    /*!
     *  \brief         28. Нажатие педали в АО без требования нажатия педали (скорость выше допустимой)
     */
    void C9518548_TestCase();

    /*!
     *  \brief         1. Включение АО без нажатой педали и без требования нажатия педали (скорость от 10 до допустимой)
     */
    void C9645327_TestCase();

    /*!
     *  \brief         2. Включение АО с нажатой педалью и без требования нажатия педали (скорость от 10 до допустимой)
     */
    void C9645328_TestCase();

    /*!
     *  \brief         3. Включение АО с нажатой педалью с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9645329_TestCase();

    /*!
     *  \brief         4. Повторное включение блокировки в АО без педали (скорость от 10 до допустимой)
     */
    void C9645330_TestCase();

    /*!
     *  \brief         5. Включение АО без нажатой педали с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9645331_TestCase();

    /*!
     *  \brief         6. Включение АО без нажатой педали и без требования нажатия педали (нет данных о скорости)
     */
    void C9645313_TestCase();

    /*!
     *  \brief         7. Включение АО с нажатой педалью и без требования нажатия педали (нет данных о скорости)
     */
    void C9645314_TestCase();

    /*!
     *  \brief         8. Включение АО с нажатой педалью с требованием нажатия педали (нет данных о скорости)
     */
    void C9645315_TestCase();

    /*!
     *  \brief         9. Повторное включение блокировки в АО без педали (нет данных о скорости)
     */
    void C9645316_TestCase();

    /*!
     *  \brief         10. Включение АО без нажатой педали с требованием нажатия педали (нет данных о скорости)
     */
    void C9645317_TestCase();

    /*!
     *  \brief         11. Включение АО без нажатой педали и без требования нажатия педали (скорость выше допустимой)
     */
    void C9645334_TestCase();

    /*!
     *  \brief         12. Включение АО с нажатой педалью и без требования нажатия педали (скорость выше допустимой)
     */
    void C9645335_TestCase();

    /*!
     *  \brief         13. Включение АО с нажатой педалью с требованием нажатия педали (скорость выше допустимой)
     */
    void C9645336_TestCase();

    /*!
     *  \brief         14. Включение АО без нажатой педали с требованием нажатия педали (скорость выше допустимой)
     */
    void C9645337_TestCase();

    /*!
     *  \brief         15. Включение АО без нажатой педали и без требования нажатия педали (скорость ниже 10)
     */
    void C9645320_TestCase();

    /*!
     *  \brief         16. Включение АО с нажатой педалью и без требования нажатия педали (скорость ниже 10)
     */
    void C9645321_TestCase();

    /*!
     *  \brief         17. Включение АО с нажатой педалью с требованием нажатия педали (скорость ниже 10)
     */
    void C9645322_TestCase();

    /*!
     *  \brief         18. Включение АО без нажатой педали с требованием нажатия педали (скорость ниже 10)
     */
    void C9645323_TestCase();

    /*!
     *  \brief         19. 180 сек. АО без нажатой педали и без требования нажатия педали (скорость выше допустимой)
     */
    void C9645338_TestCase();

    /*!
     *  \brief         20. 180 сек. АО с нажатой педалью и без требования нажатия педали (скорость выше допустимой)
     */
    void C9645339_TestCase();

    /*!
     *  \brief         21. 180 сек. АО с нажатой педалью с требованием нажатия педали (скорость выше допустимой)
     */
    void C9645340_TestCase();

    /*!
     *  \brief         22. 180 сек. АО без нажатой педали с требованием нажатия педали (скорость выше допустимой)
     */
    void C9645341_TestCase();

    /*!
     *  \brief         23. 180 сек. АО без нажатой педали с требованием нажатия педали (нет данных о скорости)
     */
    void C9645318_TestCase();

    /*!
     *  \brief         24. 180 сек. АО без нажатой педали с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9645332_TestCase();

    /*!
     *  \brief         25. Нажатие педали в АО с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9645333_TestCase();

    /*!
     *  \brief         26. Нажатие педали в АО с требованием нажатия педали (нет данных о скорости)
     */
    void C9645319_TestCase();

    /*!
     *  \brief         27. Нажатие педали в АО с требованием нажатия педали (скорость выше допустимой)
     */
    void C9645342_TestCase();

    /*!
     *  \brief         28. Нажатие педали в АО без требования нажатия педали (скорость выше допустимой)
     */
    void C9645343_TestCase();

    /*!
     *  \brief         1. Включение АО без нажатой педали и без требования нажатия педали (скорость от 10 до допустимой)
     */
    void C9658759_TestCase();

    /*!
     *  \brief         2. Включение АО с нажатой педалью и без требования нажатия педали (скорость от 10 до допустимой)
     */
    void C9658760_TestCase();

    /*!
     *  \brief         3. Включение АО с нажатой педалью с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9658761_TestCase();

    /*!
     *  \brief         4. Повторное включение блокировки в АО без педали (скорость от 10 до допустимой)
     */
    void C9658762_TestCase();

    /*!
     *  \brief         5. Включение АО без нажатой педали с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9658763_TestCase();

    /*!
     *  \brief         6. Включение АО без нажатой педали и без требования нажатия педали (нет данных о скорости)
     */
    void C9658745_TestCase();

    /*!
     *  \brief         7. Включение АО с нажатой педалью и без требования нажатия педали (нет данных о скорости)
     */
    void C9658746_TestCase();

    /*!
     *  \brief         8. Включение АО с нажатой педалью с требованием нажатия педали (нет данных о скорости)
     */
    void C9658747_TestCase();

    /*!
     *  \brief         9. Повторное включение блокировки в АО без педали (нет данных о скорости)
     */
    void C9658748_TestCase();

    /*!
     *  \brief         10. Включение АО без нажатой педали с требованием нажатия педали (нет данных о скорости)
     */
    void C9658749_TestCase();

    /*!
     *  \brief         11. Включение АО без нажатой педали и без требования нажатия педали (скорость выше допустимой)
     */
    void C9658766_TestCase();

    /*!
     *  \brief         12. Включение АО с нажатой педалью и без требования нажатия педали (скорость выше допустимой)
     */
    void C9658767_TestCase();

    /*!
     *  \brief         13. Включение АО с нажатой педалью с требованием нажатия педали (скорость выше допустимой)
     */
    void C9658768_TestCase();

    /*!
     *  \brief         14. Включение АО без нажатой педали с требованием нажатия педали (скорость выше допустимой)
     */
    void C9658769_TestCase();

    /*!
     *  \brief         15. Включение АО без нажатой педали и без требования нажатия педали (скорость ниже 10)
     */
    void C9658752_TestCase();

    /*!
     *  \brief         16. Включение АО с нажатой педалью и без требования нажатия педали (скорость ниже 10)
     */
    void C9658753_TestCase();

    /*!
     *  \brief         17. Включение АО с нажатой педалью с требованием нажатия педали (скорость ниже 10)
     */
    void C9658754_TestCase();

    /*!
     *  \brief         18. Включение АО без нажатой педали с требованием нажатия педали (скорость ниже 10)
     */
    void C9658755_TestCase();

    /*!
     *  \brief         19. 180 сек. АО без нажатой педали и без требования нажатия педали (скорость выше допустимой)
     */
    void C9658770_TestCase();

    /*!
     *  \brief         20. 180 сек. АО с нажатой педалью и без требования нажатия педали (скорость выше допустимой)
     */
    void C9658771_TestCase();

    /*!
     *  \brief         21. 180 сек. АО с нажатой педалью с требованием нажатия педали (скорость выше допустимой)
     */
    void C9658772_TestCase();

    /*!
     *  \brief         22. 180 сек. АО без нажатой педали с требованием нажатия педали (скорость выше допустимой)
     */
    void C9658773_TestCase();

    /*!
     *  \brief         23. 180 сек. АО без нажатой педали с требованием нажатия педали (нет данных о скорости)
     */
    void C9658750_TestCase();

    /*!
     *  \brief         24. 180 сек. АО без нажатой педали с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9658764_TestCase();

    /*!
     *  \brief         25. Нажатие педали в АО с требованием нажатия педали (скорость от 10 до допустимой)
     */
    void C9658765_TestCase();

    /*!
     *  \brief         26. Нажатие педали в АО с требованием нажатия педали (нет данных о скорости)
     */
    void C9658751_TestCase();

    /*!
     *  \brief         27. Нажатие педали в АО с требованием нажатия педали (скорость выше допустимой)
     */
    void C9658774_TestCase();

    /*!
     *  \brief         28. Нажатие педали в АО без требования нажатия педали (скорость выше допустимой)
     */
    void C9658775_TestCase();

    /*!
     *  \brief         Блокировка по ДД в ЗП при допустимой скорости
     */
    void C9745928_TestCase();

    /*!
     *  \brief         Блокировка по ДД с задержкой в ЗП при допустимой скорости
     */
    void C9745929_TestCase();

    /*!
     *  \brief         Блокировка по ДД в ЗП без данных о скорости
     */
    void C9745930_TestCase();

    /*!
     *  \brief         Блокировка по ДД с задержкой в ЗП без данных о скорости
     */
    void C9745931_TestCase();

    /*!
     *  \brief         Блокировка по ДД в ЗП при недопустимой скорости
     */
    void C9745932_TestCase();

    /*!
     *  \brief         Отсутствие блокировки АКПП при выключенной настройке
     */
    void C9801189_TestCase();

    /*!
     *  \brief         Отсутствие блокировки АКПП на аналоговом паркинге
     */
    void C9801190_TestCase();

    /*!
     *  \brief         Нарушение зоны охраны в тревоге после открытия багажника (авторизация по брелоку)
     */
    void C9913925_TestCase();

    /*!
     *  \brief         Нарушение зоны охраны в тревоге после открытия багажника (авторизация по метке)
     */
    void C9913926_TestCase();

    /*!
     *  \brief         1. Выключение паркинга без зажигания с включенной настройкой
     */
    void C10271054_TestCase();

    /*!
     *  \brief         2. Выключение ручника без зажигания с включенной настройкой
     */
    void C10271055_TestCase();

    /*!
     *  \brief         3. Включение тормоза без зажигания с включенной настройкой
     */
    void C10271056_TestCase();

    /*!
     *  \brief         4. Выключение паркинга без зажигания с отключенной настройкой
     */
    void C10271057_TestCase();

    /*!
     *  \brief         5. Выключение ручника без зажигания с отключенной настройкой
     */
    void C10271058_TestCase();

    /*!
     *  \brief         6. Включение тормоза без зажигания с отключенной настройкой
     */
    void C10271059_TestCase();

    /*!
     *  \brief         7. Выключение паркинга с зажиганием с включенной настройкой с последующим выключением зажигания
     */
    void C10271060_TestCase();

    /*!
     *  \brief         8. Выключение ручника с зажиганием с включенной настройкой с последующим выключением зажигания
     */
    void C10271061_TestCase();

    /*!
     *  \brief         9. Включение тормоза с зажиганием с включенной настройкой с последующим выключением зажигания
     */
    void C10271062_TestCase();

    /*!
     *  \brief         10. Выключение паркинга с зажиганием с включенной настройкой с последующим включением паркинга
     */
    void C10271063_TestCase();

    /*!
     *  \brief         11. Выключение ручника с зажиганием с включенной настройкой с последующим включением ручника
     */
    void C10271064_TestCase();

    /*!
     *  \brief         12. Включение тормоза с зажиганием с включенной настройкой с последующим выключением тормоза
     */
    void C10271065_TestCase();

    /*!
     *  \brief         13. Выключение паркинга с зажиганием с отключенной настройкой
     */
    void C10271066_TestCase();

    /*!
     *  \brief         14. Выключение ручника с зажиганием с отключенной настройкой
     */
    void C10271067_TestCase();

    /*!
     *  \brief         15. Включение тормоза с зажиганием с отключенной настройкой
     */
    void C10271068_TestCase();

    /*!
     *  \brief         16. Включение зажигания с включенной настройкой с последующим выключением зажигания
     */
    void C10271069_TestCase();

    /*!
     *  \brief         17. Включение паркинга с зажиганием с включенной настройкой с активным извещением
     */
    void C10271070_TestCase();

    /*!
     *  \brief         18. Включение ручника с зажиганием с включенной настройкой с активным извещением
     */
    void C10271071_TestCase();

    /*!
     *  \brief         19. Выключение тормоза с зажиганием с включенной настройкой с активным извещением
     */
    void C10271072_TestCase();

    /*!
     *  \brief         20. Включение паркинга с зажиганием с отключенной настройкой с активным извещением
     */
    void C10271073_TestCase();

    /*!
     *  \brief         21. Включение ручника с зажиганием с отключенной настройкой с активным извещением
     */
    void C10271074_TestCase();

    /*!
     *  \brief         22. Выключение тормоза с зажиганием с отключенной настройкой с активным извещением
     */
    void C10271075_TestCase();

    /*!
     *  \brief         Обновление прошивки с R4
     */
    void C10288872_TestCase();

    /*!
     *  \brief         Обновление прошивки с R4v2
     */
    void C10288873_TestCase();

    /*!
     *  \brief         Обновление прошивки с R6
     */
    void C10288874_TestCase();

    /*!
     *  \brief         Обновление прошивки с R6eco
     */
    void C10288875_TestCase();

    /*!
     *  \brief         Обновление прошивки с R4 с версии не выше 2.18
     */
    void C10288876_TestCase();

    /*!
     *  \brief         Обновление прошивки с R4v2 с версии не выше 2.18
     */
    void C10288877_TestCase();

    /*!
     *  \brief         Обновление прошивки с R6 с версии не выше 2.18
     */
    void C10288878_TestCase();

    /*!
     *  \brief         Обновление прошивки с R6eco с версии не выше 2.18
     */
    void C10288879_TestCase();

    /*!
     *  \brief         Откат прошивки с R4
     */
    void C10288880_TestCase();

    /*!
     *  \brief         Откат прошивки с R4v2
     */
    void C10288881_TestCase();

    /*!
     *  \brief         Откат прошивки с R6
     */
    void C10288882_TestCase();

    /*!
     *  \brief         Откат прошивки с R6eco
     */
    void C10288883_TestCase();

    /*!
     *  \brief         Открыть дверь
     */
    void C10369958_TestCase();

    /*!
     *  \brief         Открыть багажник
     */
    void C10369959_TestCase();

    /*!
     *  \brief         Открыть дверь с запущенным двигателем
     */
    void C10369960_TestCase();

    /*!
     *  \brief         Открыть багажник с запущенным двигателем
     */
    void C10369961_TestCase();

    /*!
     *  \brief         Открыть дверь
     */
    void C10369962_TestCase();

    /*!
     *  \brief         Открыть багажник
     */
    void C10369963_TestCase();

    /*!
     *  \brief         Открыть дверь с запущенным двигателем
     */
    void C10369964_TestCase();

    /*!
     *  \brief        Открыть багажник с запущенным двигателем
     */
    void C10369965_TestCase();

    /*!
     *  \brief         Открыть дверь с запущенным двигателем
     */
    void C10369966_TestCase();

    /*!
     *  \brief         Открыть багажник с запущенным двигателем
     */
    void C10369967_TestCase();

    /*!
     *  \brief         21. Включение зажигания и переход в сервис
     */
    void C10415982_TestCase();

    /*!
     *  \brief         22. Включение зажигания в сервисе
     */
    void C10415983_TestCase();

    /*!
     *  \brief         23. Включение зажигания в сервисе и переход из сервиса
     */
    void C10415984_TestCase();

    /*!
     *  \brief         24. Включение зажигания не в сервисе и выключение зажигания в сервисе
     */
    void C10415985_TestCase();

    /*!
     *  \brief         25. Включение и выключение зажигания в сервисе
     */
    void C10415986_TestCase();

    /*!
     *  \brief         26. Включение зажигания в сервисе и выключение зажигания не в сервисе
     */
    void C10415987_TestCase();

    /*!
     *  \brief         27. Включение и отмена тревоги и переход в сервис
     */
    void C10415988_TestCase();

    /*!
     *  \brief         28. Включение и отмена тревоги и переход в сервис и из сервиса
     */
    void C10415989_TestCase();

    /*!
     *  \brief         29. Включение регистратора через смс и переход в сервис
     */
    void C10415990_TestCase();

    /*!
     *  \brief         30. Включение регистратора через смс в сервисе
     */
    void C10415991_TestCase();

    /*!
     *  \brief         31. Включение регистратора через смс в сервисе и выход из сервиса
     */
    void C10415992_TestCase();

    /*!
     *  \brief         Закрыть дверь позднее чем через 10 сек
     */
    void C10419622_TestCase();

    /*!
     *  \brief         Ввести неправильно 1ую цифру
     */
    void C10419623_TestCase();

    /*!
     *  \brief         Ввести неправильно 2ую цифру
     */
    void C10419624_TestCase();

    /*!
     *  \brief         Ввести неправильно 3ую цифру
     */
    void C10419625_TestCase();

    /*!
     *  \brief         Ввести неправильно 4ую цифру
     */
    void C10419626_TestCase();

    /*!
     *  \brief         Ввести неправильный код
     */
    void C10419627_TestCase();

    /*!
     *  \brief         Ввести правильный код
     */
    void C10419628_TestCase();

    /*!
     *  \brief         Выключение авторизации кодом
     */
    void C10419629_TestCase();

    /*!
     *  \brief         Выключение запрета поездки кодом
     */
    void C10419630_TestCase();

    /*!
     *  \brief         Ввести неправильно 1ую цифру
     */
    void C10419632_TestCase();

    /*!
     *  \brief         Ввести неправильно 2ую цифру
     */
    void C10419633_TestCase();

    /*!
     *  \brief         Ввести неправильно 3ую цифру
     */
    void C10419634_TestCase();

    /*!
     *  \brief         Ввести неправильно 4ую цифру
     */
    void C10419635_TestCase();

    /*!
     *  \brief         Ввести неправильный код
     */
    void C10419636_TestCase();

    /*!
     *  \brief         Ввести правильный код
     */
    void C10419637_TestCase();

    /*!
     *  \brief         Выключение авторизации кодом
     */
    void C10419638_TestCase();

    /*!
     *  \brief         Выключение запрета поездки кодом
     */
    void C10419639_TestCase();

    /*!
     *  \brief         Ввести неправильно 1ую цифру
     */
    void C10419640_TestCase();

    /*!
     *  \brief         Ввести неправильно 2ую цифру
     */
    void C10419641_TestCase();

    /*!
     *  \brief         Ввести неправильно 3ую цифру
     */
    void C10419642_TestCase();

    /*!
     *  \brief         Ввести неправильно 4ую цифру
     */
    void C10419643_TestCase();

    /*!
     *  \brief         Ввести неправильный код
     */
    void C10419644_TestCase();

    /*!
     *  \brief         Ввести правильный код
     */
    void C10419645_TestCase();

    /*!
     *  \brief         Выключение авторизации кодом
     */
    void C10419646_TestCase();

    /*!
     *  \brief         Выключение запрета поездки кодом
     */
    void C10419647_TestCase();

    /*!
     *  \brief         Постановка в охрану штатными кнопками
     */
    void C10419648_TestCase();

    /*!
     *  \brief         Несоблюдение таймаутов при постановке кнопками
     */
    void C10419649_TestCase();

    /*!
     *  \brief         Постановка в охрану с помощью двери
     */
    void C10419650_TestCase();

    /*!
     *  \brief         Несоблюдение таймаутов при постановке дверью
     */
    void C10419651_TestCase();

    /*!
     *  \brief         Переход в тревогу по двери раньше чем в охрану с нарушенным периметром
     */
    void C10485319_TestCase();

    /*!
     *  \brief         Переход в тревогу по багажнику раньше чем в охрану с нарушенным периметром
     */
    void C10485320_TestCase();

    /*!
     *  \brief         Переход в охрану с нарушенным периметром по двери раньше чем в тревогу
     */
    void C10485321_TestCase();

    /*!
     *  \brief         Переход в охрану с нарушенным периметром по багажнику раньше чем в тревогу
     */
    void C10485322_TestCase();

    /*!
     *  \brief         Включение настройки по BLE и выключение кнопкой без зажигания
     */
    void C10489015_TestCase();

    /*!
     *  \brief         Включение настройки по BLE и выключение по СМС
     */
    void C10489016_TestCase();

    /*!
     *  \brief         1.5.Вход через мобильное приложение
     */
    void C15185_TestCase();

    /*!
     *  \brief         1.29.Выход через мобильное приложение
     */
    void C15209_TestCase();

    /*!
     *  \brief         2.5.Вход через мобильное приложение
     */
    void C15215_TestCase();

    /*!
     *  \brief         3.5.Вход через мобильное приложение
     */
    void C15221_TestCase();

    /*!
     *  \brief         4.5.Вход через мобильное приложение
     */
    void C15227_TestCase();

    /*!
     *  \brief         5.5.Вход через мобильное приложение
     */
    void C15233_TestCase();

    /*!
     *  \brief         Постановка в охрану сервисной кнопкой без включения зажигания
     */
    void C10634813_TestCase();

    /*!
     *  \brief         Переход в меню программирования брелоком без зажигания
     */
    void C10634814_TestCase();

    /*!
     *  \brief         Перевод R6 в режим обновления прошивки без зажигания
     */
    void C10634815_TestCase();

    /*!
     *  \brief         Переход в режим регистрации без зажигания
     */
    void C10634816_TestCase();

    /*!
     *  \brief         Постановка в охрану сервисной кнопкой после включения запрета поездки
     */
    void C10634817_TestCase();

    /*!
     *  \brief         Переход в меню программирования брелоком после включения запрета поездки
     */
    void C10634818_TestCase();

    /*!
     *  \brief         Перевод R6 в режим обновления прошивки после включения запрета поездки
     */
    void C10634819_TestCase();

    /*!
     *  \brief         Переход в режим регистрации после включения запрета поездки
     */
    void C10634820_TestCase();

    /*!
     *  \brief         Переход в режим настройки по BLE после включения запрета поездки
     */
    void C10634821_TestCase();

    /*!
     *  \brief         СМС оповещение о АО по метке
     */
    void C10634826_TestCase();

    /*!
     *  \brief         СМС оповещение о АО по брелоку
     */
    void C10634827_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 1 раз
     */
    void C10645333_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 2 раза
     */
    void C10645334_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 3 раза
     */
    void C10645335_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 4 раза
     */
    void C10645336_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 5 раз
     */
    void C10645337_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco 1 раз
     */
    void C10645338_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco 2 раза
     */
    void C10645339_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco 3 раза
     */
    void C10645340_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco 4 раза
     */
    void C10645341_TestCase();

    /*!
     *  \brief         Зарегистрироватьeco R6 5 раз
     */
    void C10645342_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 и R6eco 1 раз
     */
    void C10645343_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 и R6eco 2 раза
     */
    void C10645344_TestCase();

    /*!
     *  \brief         Зарегистрировать R6 и R6eco 3 раза
     */
    void C10645345_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco и R6 1 раз
     */
    void C10645346_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco и R6 2 раза
     */
    void C10645347_TestCase();

    /*!
     *  \brief         Зарегистрировать R6eco и R6 3 раза
     */
    void C10645348_TestCase();

    /*!
     *  \brief         Выключение охраны аналоговым Slave без метки
     */
    void C10832739_TestCase();

    /*!
     *  \brief         Выключение охраны Slave по CAN без метки
     */
    void C10832740_TestCase();

    /*!
     *  \brief         Выключение охраны аналоговым Slave с меткой
     */
    void C10832741_TestCase();

    /*!
     *  \brief         Выключение охраны Slave по CAN с меткой
     */
    void C10832742_TestCase();

    /*!
     *  \brief         Таймер открытия багажника меньше таймера авторизации
     */
    void C10861710_TestCase();

    /*!
     *  \brief         Таймер авторизации меньше таймера открытия багажника
     */
    void C10861711_TestCase();

    /*!
     *  \brief         Таймер авторизации продолжает работать после сброса таймера багажника меткой
     */
    void C10861712_TestCase();

    /*!
     *  \brief         Таймер авторизации продолжает работать после сброса таймера багажника брелоком
     */
    void C10861713_TestCase();

    /*!
     *  \brief         Сработал ДД с отключенным зажиганием в охране (Минимальный импульс)
     */
    void C10854614_TestCase();

    /*!
     *  \brief         Переход в тревогу с включенным зажиганием из охраны (Максимальный импульс)
     */
    void C10854615_TestCase();

    /*!
     *  \brief         Минимальные 3 импульса
     */
    void C10854616_TestCase();

    /*!
     *  \brief         Максимальные 3 импульса
     */
    void C10854617_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга в авторизации
     */
    void C10854594_TestCase();

    /*!
     *  \brief         Сработал ДД с отключенным зажиганием в авторизации
     */
    void C10854588_TestCase();

    /*!
     *  \brief         Сработал ДД с включенным зажиганием в авторизации
     */
    void C10854595_TestCase();

    /*!
     *  \brief         Переход в тревогу с отключенным зажиганием в авторизации
     */
    void C10854589_TestCase();

    /*!
     *  \brief         Переход в тревогу с включенным зажиганием в авторизации
     */
    void C10854590_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга в запрете поездки
     */
    void C10854596_TestCase();

    /*!
     *  \brief         Сработал ДД с отключенным зажиганием в запрете поездки
     */
    void C10854591_TestCase();

    /*!
     *  \brief         Сработал ДД с включенным зажиганием в запрете поездки
     */
    void C10854597_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга в тревоге
     */
    void C10854607_TestCase();

    /*!
     *  \brief         Сработал ДД с отключенным зажиганием в тревоге
     */
    void C10854608_TestCase();

    /*!
     *  \brief         Сработал ДД с включенным зажиганием в тревоге
     */
    void C10854609_TestCase();

    /*!
     *  \brief         Сработал ДД с отключенным зажиганием в предупредительной тревоге
     */
    void C10854601_TestCase();

    /*!
     *  \brief         Переход в тревогу с включенным зажиганием из предупредительной тревоге
     */
    void C10854602_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга с зажиганием включенным ранее чем АО
     */
    void C10854603_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга с АО включенным ранее чем зажигание
     */
    void C10854604_TestCase();

    /*!
     *  \brief         Сработал ДД с зажиганием включенным ранее чем АО
     */
    void C10854605_TestCase();

    /*!
     *  \brief         Сработал ДД с АО включенным ранее чем зажигание
     */
    void C10854606_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга в АО без скорости
     */
    void C10854598_TestCase();

    /*!
     *  \brief         Сработал ДД в АО с включенным зажиганием без скорости
     */
    void C10854600_TestCase();

    /*!
     *  \brief         Переключение АКПП из паркинга в АО с скоростью
     */
    void C10854610_TestCase();

    /*!
     *  \brief         Сработал ДД в АО с включенным зажиганием с скоростью
     */
    void C10854612_TestCase();

    /*!
     *  \brief         Сработал ДД в АО с отключенным зажиганием
     */
    void C10854613_TestCase();

    /*!
     *  \brief         НР блокировка во время ДЗ на Замке Зажигания
     */
    void C10964449_TestCase();

    /*!
     *  \brief         НР блокировка во время ДЗ на Старт-Стоп
     */
    void C10964450_TestCase();

    /*!
     *  \brief         Регистрация валидатора
     */
    void C11110395_TestCase();

    /*!
     *  \brief         Неудачная регистрация валидатора
     */
    void C11110396_TestCase();

    /*!
     *  \brief         Неудачное подтверждение регистрации валидатора
     */
    void C11110397_TestCase();

    /*!
     *  \brief         Оповещение о тревоге из-за не пройденной авторизации после открытия аналогового багажника
     */
    void C11446206_TestCase();

    /*!
     *  \brief         Оповещение о тревоге из-за не пройденной авторизации после открытия CAN багажника
     */
    void C11446207_TestCase();

    /*!
     *  \brief         Выключение АО по СМС после пропажи метки
     */
    void C11516269_TestCase();

    /*!
     *  \brief         Выключение АО через www.starline-online.ru после пропажи метки
     */
    void C11516270_TestCase();

    /*!
     *  \brief         Выключение АО по СМС после пропажи брелока
     */
    void C11516271_TestCase();

    /*!
     *  \brief         Выключение АО через www.starline-online.ru после пропажи брелока
     */
    void C11516272_TestCase();

    /*!
     *  \brief         Включение HIC по двери и зажиганию и выключение по двери
     */
    void C11846848_TestCase();

    /*!
     *  \brief         Включение HIC по багажнику и включению ручника и выключение по багажнику
     */
    void C11846849_TestCase();

    /*!
     *  \brief         Включение HIC по двери и выключению ручника и выключение по охране
     */
    void C11846850_TestCase();

    /*!
     *  \brief         Включение HIC по багажнику и зажиганию без закрытия багажника
     */
    void C11846851_TestCase();

    /*!
     *  \brief         Включение HIC по двери и включению ручника без закрытия двери
     */
    void C11846852_TestCase();

    /*!
     *  \brief         Включение HIC по багажнику и зажиганию с открытым багажником
     */
    void C11846853_TestCase();

    /*!
     *  \brief         Включение HIC по двери и включению ручника с открытой дверью
     */
    void C11846854_TestCase();

    /*!
     *  \brief         Индикация настройки по BLE при подключении смартфона
     */
    void C11946743_TestCase();

    /*!
     *  \brief         Переход в авторизацию с помощью Slave
     */
    void C12037386_TestCase();

    /*!
     *  \brief         Переход в запрет поездки с помощью Slave
     */
    void C12037387_TestCase();

    /*!
     *  \brief         Появление метки в авторизации
     */
    void C12037388_TestCase();

    /*!
     *  \brief         Появление метки в запрете поездки
     */
    void C12037389_TestCase();

    /*!
     *  \brief         Переход в запрет поездки (по снятию с охраны) по выключению охраны и зажигания
     */
    void C12037397_TestCase();

    /*!
     *  \brief         Переход в запрет поездки (по выключению зажигания) по выключению охраны и зажигания
     */
    void C12037398_TestCase();

    /*!
     *  \brief         Переход в отключенный запрет поездки (по снятию с охраны) по выключению охраны и зажигания
     */
    void C12037401_TestCase();

    /*!
     *  \brief         Переход в отключенный запрет поездки (по выключению зажигания) по выключению охраны и зажигания
     */
    void C12037402_TestCase();

    /*!
     *  \brief         При снятии с охраны (М)
     */
    void C12116944_TestCase();

    /*!
     *  \brief         При снятии с охраны и При каждом выключении зажигания (М)
     */
    void C12116945_TestCase();

    /*!
     *  \brief         При снятии с охраны (Ш)
     */
    void C12116946_TestCase();

    /*!
     *  \brief         При снятии с охраны и При каждом выключении зажигания (Ш)
     */
    void C12116947_TestCase();

    /*!
     *  \brief         При снятии с охраны (М+Ш)
     */
    void C12116948_TestCase();

    /*!
     *  \brief         При снятии с охраны и При каждом выключении зажигания (М+Ш)
     */
    void C12116949_TestCase();

    /*!
     *  \brief         При снятии с охраны (М+Ш) (выключение кнопками)
     */
    void C12116950_TestCase();

    /*!
     *  \brief         При снятии с охраны и При каждом выключении зажигания (М+Ш) (выключение кнопками)
     */
    void C12116951_TestCase();

};

#endif  // _AUTO_TESTS_MODES_P2_H_
