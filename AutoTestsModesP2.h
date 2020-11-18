 /*!
 *  \file      AutoTestsModesP2.cpp
 *
 *  \brief     �������� ���������� ������, ����������� ����� ������ ������ (����� 2)
 *
 *  \detailed  ������������ ��� ������������ ��������� 6-�� ���������
 *
 */

#ifndef _AUTO_TESTS_MODES_P2_H_
#define _AUTO_TESTS_MODES_P2_H_

//! ����������� ������������ ������

#include "AutoTestsModesBase.h"
#include "AutoTestsTelematics.h"

//! ���� � ������

/*!
 *  \brief    ����� AutoTestsModesP2
 *   
 *  \detailed �������� ���������� ��� ������������ ��������� 6-�� ���������
 */
class AutoTestsModesP2 : public AutoTestsModesBase
{

//! \privatesection
private:

    /*!
     *  \brief ��� �������� �������, ������� ������� ���������
     */
    typedef enum
    {
        ALARM_CHECK_TYPE_START, //!< ������ ������ (��������� �������� �����/������)
        ALARM_CHECK_TYPE_HALF,  //!< 0,5 �����
        ALARM_CHECK_TYPE_FULL   //!< ������ ����
    } alarm_check_type_t;

    /*!
     *  \brief ��������� �������� ��� �������� ��������� ���������� ���� (��� ������ setStateAndCheckGearboxBlock)
     */
    typedef enum
    {
        ACT_TYPE_NO_ACT,     //!< ������ �� ���������
        ACT_TYPE_DISARM,     //!< ����� � ������
        ACT_TYPE_PARKING_ON, //!< �������� �������
        ACT_TYPE_AUTH        //!< ��������������
    } act_types_t;

    /*!
     *  \brief ������ ������ �����, �� ������� ����� ������ ����������� � ��������� "�������� �����������" (��� ������ setZoneAndCheckAuth)
     */
    typedef enum
    {
        AUTH_STEP_START, //!< �� ��������� �������� (�� ������� ����, �.�. �� ��������� ���������� �����������)
        AUTH_STEP_ONE,   //!< �� ������ ����
        AUTH_STEP_TWO,   //!< �� ������ ����
        AUTH_STEP_THREE  //!< �� ������� ����
    } auth_step_t;

    /*!
     *  \brief ��� ��������� �� (��� ������� resetTagOrRemoteAndCheckAhj, setAhjAndCheckSms)
     */
    typedef enum
    {
        AHJ_TYPE_TAG_OFF,   //!< �� ���������� �����
        AHJ_TYPE_REMOTE_OFF //!< �� ���������� �������
    } ahj_type_t;

    /*!
     *  \brief ���� ���������� � ������ "��������������" (��� ������ setAhjAndCheckBlock)
     */
    typedef enum
    {
        AHJ_BLOCK_TYPE_INSTANT_OFF, //!< ���������� ��������� (�� ���������� ����� ����� ���� 4 ��)
        AHJ_BLOCK_TYPE_INSTANT_ON,  //!< ���������� ���������� ����� ����� ���� 4 ��
        AHJ_BLOCK_TYPE_DELAYED_OFF, //!< ���������� �� ���������� � ������� � ����� 180 ������ (���������� �������) (TIME_DELAY::MAX_AHJ_BLOCK_DELAY_S) ����� ���� 4 ��
        AHJ_BLOCK_TYPE_DELAYED_ON   //!< ���������� ���������� � ��������� 180 ������ (���������� �������) (TIME_DELAY::MAX_AHJ_BLOCK_DELAY_S) ����� ���� 4 ��
    } ahj_block_type_t;

    /*!
     *  \brief ������� �������� (�������� ��������/���������) (��� ������ setImmoAndSensorAndCheckBlock)
     */
    typedef enum
    {
        CAR_SPEED_OFF, //!< �������� ���������
        CAR_SPEED_ON   //!< �������� ��������
    } car_speed_type_t;

    /*!
     *  \brief ��� ���������� ���������
     */
    typedef enum
    {
        PARKING_TYPE_CAN,   //!< �� CAN
        PARKING_TYPE_ANALOG //!< �� �������
    } parking_type_t;

    /*!
     *  \brief ��� ���� ����������� ������ (��� ������ setAndCheckAltSrvBtnDisarm)
     */
    typedef enum
    {
        SRV_BTN_PIN_TYPE_WRONG,  //!< �������� ��� ����������� ������
        SRV_BTN_PIN_TYPE_CORRECT //!< ������ ��� ����������� ������
    } srv_btn_pin_type_t;

    /*!
     *  \brief ������� ��������� ������ ��������� �� BLE (��� ������ setAndCheckBleSettingsMode)
     */
    typedef enum
    {
        BLE_SETTINGS_MODE_EN_TYPE_SRV_BTN,        //!< ����� ��������� ������
        BLE_SETTINGS_MODE_EN_TYPE_SRV_BTN_NO_IGN, //!< ����� ��������� ������ ��� ������������� ���������
        BLE_SETTINGS_MODE_EN_TYPE_SMS             //!< ����� SMS
    } ble_settings_mode_en_type_t;

    /*!
     *  \brief ������� ����������� ������� ���� � �������� ��������
     */
    typedef enum
    {
        RELAY_REGISTRATION_SEQUENCE_R6_ONLY,       //!< ������ R6
        RELAY_REGISTRATION_SEQUENCE_R6ECO_ONLY,    //!< ������ R6 ECO
        RELAY_REGISTRATION_SEQUENCE_R6_THEN_R6ECO, //!< R6, ����� R6 ECO
        RELAY_REGISTRATION_SEQUENCE_R6ECO_THEN_R6  //!< R6 ECO, ����� R6
    } relay_registration_sequence_t;

    /*!
     *  \brief ���� ���������� ��
     */
    typedef enum
    {
        FW_UPDATE_FROM_PREV_TO_CURR,   //!< ���������� � ���������� �� ������� ������
        FW_UPDATE_FROM_CURR_TO_PREV,   //!< ���������� � ������� �� ���������� ������
        FW_UPDATE_FROM_SPECIAL_TO_CURR //!< ���������� � ������ �� ������� ������
    } fw_update_type_t;

    /*!
     *  \brief �������� ��� ��������� � �������� ���������� ������� �����-���� (��� ������ setBlockTriggerAndCheckStartStopBtnBlock)
     */
    typedef enum
    {
        START_STOP_BLOCK_TRIGGER_IGN_ON,          //!< �������� ���������
        START_STOP_BLOCK_TRIGGER_DOOR_OPEN,       //!< ������� �����
        START_STOP_BLOCK_TRIGGER_PARKING_OFF,     //!< ��������� �������
        START_STOP_BLOCK_TRIGGER_SENSOR_MOVE,     //!< ������� ������������ ��
        START_STOP_BLOCK_TRIGGER_SENSOR_SHOCK_LOW //!< ������� ������������ ��: ���������
    } start_stop_block_trigger_t;

    /*!
     *  \brief ��������� ���������� ������
     */
    typedef enum
    {
        SERVICE_NO_CONTROL, //!< �� ��������� ���������� ���������� ������
        SERVICE_OFF,        //!< ���������
        SERVICE_ON          //!< ��������
    } service_statuses_t;

    static AutoTestsModesP2 *s_at_modes_p2;                              //!< ��������� �� �����

    Sim900                  *p_gsm;                                      //!< ��������� �� Sim900
    vector<const char *>     m_shield_phones;                            //!< ������ ��������� GSM Shield-a
    char                     m_curr_fw_ver[LEN_STRUCT::STD_STR_BUF_LEN]; //!< ������� ������ �� � ���� ������

    int8_t  m_ppo_type;                        //!< ��� ���
    int8_t  m_ppo_on_before_autostart;         //!< ��������� ��� ����� ��
    int8_t  m_is_tag_rem_for_trunk_needed;     //!< ��������� � ������ �������� ��������� ��� ������� ����� ��� �������
    float   m_doors_imit_time_s;               //!< ����� �������� ������
    float   m_doors_imit_delay_s;              //!< �������� �������� ������ ����� �������� ���������
    float   m_trunk_tag_rem_wait_time_s;       //!< ����� ������ ����� � ������� ��� �������� ���������
    float   m_eng_block_local0_delay_s;        //!< �������� ����������
    float   m_hf_arm_after_zone_open_delay_s;  //!< ���������� ����� �������� ����� ��� ��������� (��������� ����)
    float   m_hf_arm_after_zone_close_delay_s; //!< ���������� ����� �������� ����� ��� ��������� (��������� ����)
    float   m_immo_on_after_ign_off_delay_s;   //!< ����� �� ��������� ������� ������� ����� ���������� ���������
    float   m_eng_btn_block_time_s;            //!< ������������ �������� �� ������ �����-���� (���������� ���������)

    int8_t                          m_engine_block_type;    //!< ��� ���������� ���������
    eng_start_gearbox_type_t        m_gearbox_type;         //!< ��� ���
    eng_start_neutral_detect_type_t m_program_neutral_type; //!< ��� ���������� �� ��

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
     *  \brief         ����������� ������
     */
    AutoTestsModesP2();

    /*!
     *  \brief         ����������� �����������
     *
     *  \param  [in]  &se - ������ �� ����� AutoTestsModesP2
     */

    AutoTestsModesP2(const AutoTestsModesP2 &se);

    /*!
     *  \brief         �������� =
     *
     *  \param  [in]  &se - ������ �� ����� AutoTestsModesP2
     */
    AutoTestsModesP2 &operator=(AutoTestsModesP2 &se);

    /*!
     *  \brief         ������������� ���������� R6 ECO
     */
    void initR6EcoWires();

    /*!
     *  \brief         ������������� ���������� X96
     */
    void initX96Wires();

    /*!
     *  \brief         ������������� ���������� M96
     */
    void initM96Wires();

    /*!
     *  \brief         ������������� ���������� A96
     */
    void initA96Wires();

    /*!
     *  \brief         ������������� ���������� E96
     */
    void initE96Wires();

    /*!
     *  \brief         ������������� ���������� S96
     */
    void initS96Wires();

    /*!
     *  \brief         ������������� ���������� X96v2
     */
    void initX96v2Wires();

    /*!
     *  \brief         ������������� ���������� X96
     */
    void initX96MasterStg();

    /*!
     *  \brief         ������������� ���������� M96
     */
    void initM96MasterStg();

    /*!
     *  \brief         ������������� ���������� A96
     */
    void initA96MasterStg();

    /*!
     *  \brief         ������������� ���������� E96
     */
    void initE96MasterStg();

    /*!
     *  \brief         ������������� ���������� S96
     */
    void initS96MasterStg();

    /*!
     *  \brief         ������������� ���������� X96v2
     */
    void initX96v2MasterStg();

    /*!
     *  \brief         ������������� ���������� X96
     */
    void initX96Settings();

    /*!
     *  \brief         ������������� ���������� M96
     */
    void initM96Settings();

    /*!
     *  \brief         ������������� ���������� A96
     */
    void initA96Settings();

    /*!
     *  \brief         ������������� ���������� E96
     */
    void initE96Settings();

    /*!
     *  \brief         ������������� ���������� S96
     */
    void initS96Settings();

    /*!
     *  \brief         ������������� ���������� X96v2
     */
    void initX96v2Settings();

    /*!
     *  \brief         �������� �������� �������
     *
     *  \param  [in]  *option - ���������
     *
     *  \return        0 - ��������� �������� ����������, 1 - �������
     */
    int8_t settings_check(options_t *option);
    
//! \publicsection
public:

    /*!
     *  \brief         ���������� ������
     */
    ~AutoTestsModesP2();

     /*!
     *  \brief         ��������� ������������� ���������� ������
     *
     *  \return        ��������� �� ��� ������ AutoTestsModesP2
     */
    static AutoTestsModesP2 *getInstance();

    /*!
     *  \brief         ������������� ����������
     */
    void init();

    /*!
     *  \brief         ���������� ��������� �� Sim900
     *
     *  \param  [in]  *sim900 - ��������� �� Sim900
     */
    void addSim900(Sim900 *sim900);

    /*!
     *  \brief         ���������� �������� GSM Shield
     *
     *  \param  [in]  *phone - ����� ��������
     */
    void addShieldPhone(const char *phone);
    
    /*!
     *  \brief         ���������� ��������� �� CanBusActions
     *
     *  \param  [in]  *canbus_actions - ��������� �� CanBusActions
     */
    void addCanBusActions(CanBusActions *canbus_actions);
    
    /*!
     *  \brief         �������� ������ "��������� ���������� (����� �������)"
     *
     *  \param  [in]   phone           - ������� (M1-M4)
     *  \param  [in]  *cmd             - DTMF �������
     *  \param  [in]   gsm_password_en - ���������� GSM ������
     *  \param  [in]   m2_owner_access - ����� M2 ��� M1
     *  \param  [in]  *gsm_pass        - GSM ������
     *  \param  [in]   dtmf_sending    - ���������� �� DTMF
     *  \param  [in]   gsm_waiting_cmd - ������ �� ������ �� ����� �������� ������
     *  \param  [in]   time_s          - ����� ���������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - �������� ������
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
     *  \brief         ��������/���������� � �������� ������� ������ � ��������� ���� ���
     *
     *  \param  [in]   secure_mode      - ����� ������  (OFF/ON)
     *  \param  [in]   execution_method - ����� ����������
     *  \param  [in]   check_time_s     - ����� �������� ������� ������
     *
     *  \return        bool
     *  \retval        true             - �������� ��������
     *  \retval        false            - �������� ������
     */                          
    bool setSecureZoneClosed(bool           secure_mode,
                             command_type_t execution_method,
                             float          check_time_s     = TIME_DELAY::MAX_GUARD_STATE_EN_TIME_S);

    /*!
     *  \brief         �������� ��������� ������������ ��� ����� ��������� ����� �� ���
     *
     *  \param  [in]   zone_name_t     - ���� ������ ��������� ������� ���������
     *  \param  [in]   zone_new_state  - ��������� ���� ������
     *
     *  \return        uint8_t
     *  \retval        ������ � hex �� � ��������� ��� ������������ enum slp_diag_data_perim_t
     */    
    uint8_t setAndCheckMergeZone(zone_name_t      zome_name,
                                 doors_statuses_t zone_new_state);

    /*!
     *  \brief         ���������� � ��������� ��������� ��������� �� CAN ��� ��
     *
     *  \return        bool
     *  \retval        true             - �������� ��������
     *  \retval        false            - �������� ������
     */
    bool setAndCheckInitCanStateForRemoteRun();

    /*!
     *  \brief         ��������� ��������� ������� ��� ������ C2198656-C2198685
     *
     *  \param  [in]   enable_ign               - ��������� ���������
     *  \param  [in]   set_ign_execution_method - ������ ���������� ����������
     *  \param  [in]   can_speed_kmph           - �������� �� CAN
     */
    void setAhjTestsStartConditions(ign_statuses_t enable_ign,
                                    command_type_t set_ign_execution_method = PIN,
                                    uint8_t        can_speed_kmph           = 0);

    /*!
     *  \brief         �������� �������� ��� �������� �� � �������� �� � ��������������� ����������
     *
     *  \param  [in]  *option - ��������� �� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������� � �������� � ��������������� ���������� ��� ������������� � ������
     *  \retval        false - ��������� �� ������� (�� ���)
     */
    bool getAndSetAhjSettings(options_t *option);

    /*!
     *  \brief         ��������� �������� ���������� ���� ��� ������ C2198656-C2198669
     *
     *  \return        bool
     *  \retval        true  - ��� ������
     *  \retval        false - ��� �� ������
     */
    bool checkAhjTestsStepLast();

    /*!
     *  \brief         ��������� ���� � ����� ��������� �� BLE ��� ������ C3202904-C3202905
     *
     *  \param  [in]   check_btn_light        - ��������� �� ��������� ���������� ��������� ������
     *  \param  [in]   ign_status_after_check - ��������� ��������� ����� �������� ������ ������
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setAndCheckBleSettingsModeOn(bool check_btn_light, ign_statuses_t ign_status_after_check);

    /*!
     *  \brief         ��������� ����� �� ������ ��������� �� BLE ��� ������ C3202904-C3202905
     *
     *  \param  [in]   check_btn_light    - ��������� �� ��������� ���������� ��������� ������
     *  \param  [in]   region_wait_time_s - ����� �������� ��������� � �������
     *
     *  \return        bool
     *  \retval        true  - ��� �������
     *  \retval        false - ��� �� �������
     */
    bool checkBleSettingsTestsModeOff(bool check_btn_light, float region_wait_time_s = TIME_DELAY::MAX_WAITING_REGION_STATE_S);

    /*!
     *  \brief         ��������� � ������ � ����� ������� � ������ ��� �������� ������� ������ ����������� (����� C20445, C20461-C20463) � �������� ��������� ��� ����. �������
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setAndCheckArmAndDisarm();

    /*!
     *  \brief         �������� ��������� ����� � ����� �� ���� �� ��������
     *
     *  \param  [in]   speed_kmph - �������� ����������
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setAndResetServiceModeBySpeed(int8_t speed_kmph);

    /*!
     *  \brief         ��������/��������� ������, ��������� ��������� ������, ��������� ��������������
     *
     *  \param  [in]   sensor_type - ��� �������
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setSensorAndCheckRearm(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         ��������� ��, ����� ������������� ��������, ��������� �������� ����� � ����� � ������
     *
     *  \param  [out] &eng_stop_timer           - ������ �� ������, ����������� ��� �������� ���������
     *  \param  [in]   delay_after_doors_imit_s - �������� ����� ������� � ������ ����� �������� �������� ����� 
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setRemoteRunAndStopAndDisarm(Timer &eng_stop_timer, float delay_after_doors_imit_s);

    /*!
     *  \brief         ��������� � ������, ����� � ������ � ��������� ������ �������
     *
     *  \param  [in]   execution_method - ������ ����������/������ � ������
     *  \param  [in]   tag_status       - ��������� �����
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setArmAndDisarmAndCheckImmo(command_type_t execution_method,
                                     tag_statuses_t tag_status);

    /*!
     *  \brief         ��������� � ������, ����� � ������ � ��������� ������ �������. ����� �������� ��������� � ����� ���������
     *
     *  \param  [in]   execution_method - ������ ����������/������ � ������
     *  \param  [in]   tag_status       - ��������� �����
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setArmAndDisarmAndCheckImmoAndSetIgn(command_type_t execution_method,
                                              tag_statuses_t tag_status);

    /*!
     *  \brief         �������� ��������� �����, �������� � ������� ������������ �� � ��������� ��������� �������
     *
     *  \param  [in]   tag_status            - ������ �����
     *  \param  [in]   speed_kmph            - �������� ����������
     *  \param  [in]   delay_before_sensor_s - �������� ����� ���������� ��
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setServiceAndSpeedAndSensor(tag_statuses_t tag_status,
                                     int8_t         speed_kmph,
                                     float          delay_before_sensor_s);

    /*!
     *  \brief         �������� ������� R6, ��������/��������� ������, ��������� ���������, ��������� (� ����������) �������� �������� R6
     *
     *  \param  [in]   secure_status - �������� ��������� ������
     *
     *  \return        bool
     *  \retval        true  - ���� ������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setR6VccAndSecureAndIgnOff(secure_cmd_t secure_status);

    /*!
     *  \brief         ��������� ������� ��, ����� �������� � ��������� ���������� R6 (��)
     *
     *  \param  [in]   delay_s - �������� ����� ���������� ������� ��
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool resetAndSetGen6VccAndCheckR6Block(float delay_s);

    /*!
     *  \brief         ��������� ���������, ��������� (�������) � ��������� ���������� R6 (��)
     *
     *  \param  [in]   ign_status   - ��������� ���������
     *  \param  [in]   eng_status   - ��������� ��������� (��������)
     *  \param  [in]   block_status - ��������� ���������� R6 (��)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setIgnAndEngineAndCheckR6Block(ign_statuses_t   ign_status,
                                        eng_statuses_t   eng_status,
                                        block_statuses_t block_status);

    /*!
     *  \brief         ��������� ��������� ������ � ��������� ���������� R6 (��)
     *
     *  \param  [in]   secure_status - �������� ��������� ������
     *  \param  [in]   block_status  - ��������� ���������� R6 (��)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setSecureAndCheckR6Block(secure_cmd_t     secure_status,
                                  block_statuses_t block_status);

    /*!
     *  \brief         ��������� ������, ������� � ��������� ������� �� �������, � ����� �� ����������
     *
     *  \param  [in]   sensor_type - ��� ������� ��� ������ �������
     *
     *  \return        bool
     *  \retval        true  - ���� ��������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setSecureAndSensorAndCheckAlert(slp_diag_data_sensors_statuses_t sensor_type);

    /*!
     *  \brief         ������ � ������, ��������/��������� �����/������, ������� �������� � ��������� ��������� �������
     *
     *  \param  [in]   tag_status    - ��������� �����
     *  \param  [in]   remote_status - ��������� �������
     *  \param  [in]   alert_status  - ����������� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setSecureAndTrunkAndCheckAlarm(tag_statuses_t    tag_status,
                                        remote_statuses_t remote_status,
                                        alert_statuses_t  alert_status);

    /*!
     *  \brief         �������� �����/������ � ��������� ��������� �������
     *
     *  \param  [in]   tag_status    - ��������� �����
     *  \param  [in]   remote_status - ��������� �������
     *  \param  [in]   alert_status  - ����������� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setTagAndRemoteAndCheckAlarm(tag_statuses_t    tag_status,
                                      remote_statuses_t remote_status,
                                      alert_statuses_t  alert_status);

    /*!
     *  \brief         ��������� �������� CAN � ACK-��� �� Mercedes CLA � ��������� � ������
     *
     *  \return        bool
     *  \retval        true  - ���� ��������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setCarMercedesClaAndSecure();

    /*!
     *  \brief         ��������� �������� CAN � ACK-��� �� Batmobile � ��������� ������� �������
     *
     *  \return        bool
     *  \retval        true  - ����� �������� ������ �������
     *  \retval        false - �������� ������ ��� ����� ��������
     */
    bool setStdAlertOffAndCarBatmobile();

    /*!
     *  \brief         �������� ������� ������� � ��������� �� ���������
     *
     *  \param  [in]   alarm_check_type - ��� ����� ��������� ������� (������ ������, 0,5 �����, ������ ����)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setAndCheckStdAlarm(alarm_check_type_t alarm_check_type);

    /*!
     *  \brief         �������� ������� ������� � ��������� 8 ������ �������
     *
     *  \param  [in]   alarm_check_type - ��� ����� ��������� ������� (������ ������, 0,5 �����, ������ ����)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setAndCheckStdAlarmFull(alarm_check_type_t alarm_check_type);

    /*!
     *  \brief         �������� � ��������� ��������� ������ "�������������� (�� �����/�������)"
     *
     *  \param  [in]   execution_method - ������ ��������� (�����/�������)
     *
     *  \return        bool
     *  \retval        true  - ���� � ����� ���������
     *  \retval        false - ���� � ����� �� ���������
     */
    bool setAndCheckAhj(command_type_t execution_method);

    /*!
     *  \brief         �������� � ��������� ��������� ������ "������ � �������� ����������"
     *
     *  \return        bool
     *  \retval        true  - ���� � ����� ���������
     *  \retval        false - ���� � ����� �� ���������
     */
    bool setAndCheckArmTrunk();

    /*!
     *  \brief         �������� � ��������� ��������� ������ "�������� �����������"
     *
     *  \return        bool
     *  \retval        true  - ���� � ����� ���������
     *  \retval        false - ���� � ����� �� ���������
     */
    bool setAndCheckWaitingAuth();

    /*!
     *  \brief         �������� � ��������� ��������� ������ "������ �������"
     *
     *  \return        bool
     *  \retval        true  - ���� � ����� ���������
     *  \retval        false - ���� � ����� �� ���������
     */
    bool setAndCheckImmo();

    /*!
    *  \brief         �������� � ��������� ��������� ������ "�������: �����������������"
     *
     *  \return        bool
     *  \retval        true  - ���� � ����� ���������
     *  \retval        false - ���� � ����� �� ���������
     */
    bool setAndCheckAlertLow();

    /*!
     *  \brief         �������� � ��������� ��������� ������ "�������: ��������"
     *
     *  \return        bool
     *  \retval        true  - ���� � ����� ���������
     *  \retval        false - ���� � ����� �� ���������
     */
    bool setAndCheckAlertHigh();

    /*!
     *  \brief         ������� � ������������ ��������� � ��������� ���������� ����
     *
     *  \param  [in]   state    - ���������, � ������� �� ��������� ����� ��������� ����������
     *  \param  [in]   act_type - ��������, ������� �� ��������� ����� ���������� ��� �������������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setStateAndCheckGearboxBlock(guard_state_t state, act_types_t act_type);

    /*!
     *  \brief         ������� � ������ � ��������� ���������� ����
     *
     *  \param  [in]   parking_type - ��� �������� (CAN/������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setArmAndCheckGearboxNoBlock(parking_type_t parking_type);

    /*!
     *  \brief         ����� ������ �����������: �������� �����, ��� ���������� ������ ����� - �������� ������
     *
     *  \param  [in]   delay_s - �������� ����� ���������
     *
     *  \return        bool
     *  \retval        true  - ���������� ������������ ����������� ��������
     *  \retval        false - ��� ����������� ������� ��� ������������ �����������
     */
    bool authByTagOrRemote(float delay_s);

    /*!
     *  \brief         ������������� ���� � ��������� ����������� �� ��������� �����
     *
     *  \param  [in]   zone_type - ������������ ����
     *  \param  [in]   auth_step - ��� ��������� "�������� �����������", �� ������� ����� ������ �����������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool setZoneAndCheckAuth(zone_type_t zone_type, auth_step_t auth_step);

    /*!
     *  \brief         �������� ������ � ��������� �������� ����� + �������
     *
     *  \param  [in]   status - �������� ��������� ������� ����� + ������ (���� ��� ���������� ��������, ���� ���������)
     *
     *  \return        bool
     *  \retval        true  - �������� ����������
     *  \retval        false - ������ ��� ����������
     */
    bool setSecureOnAndTagVccAndRemoteVcc(bool status);

    /*!
     *  \brief         ������� �������� � ��������� �������
     *
     *  \param  [in]   alert_status - ��������� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������� ������������� ����������
     *  \retval        false - ��������� ������� �� ������������� ����������
     */
    bool openTrunkAndCheckAlarm(alert_statuses_t alert_status);

    /*!
     *  \brief         ��������� �����/������ � ��������� ��� ������� �� ����������
     *
     *  \param  [in]   auth_profile         - ���������� ��� ���������� (�����/������)
     *  \param  [in]   delay_before_reset_s - �������� ����� �����������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������
     */
    bool resetTagOrRemoteAndCheckAlarm(auth_profile_t auth_profile, float delay_before_reset_s = TIME_DELAY::NULL_DELAY_S);

    /*!
     *  \brief         �������� ������������ ������ ���������� ��������� ����� � R6
     *
     *  \param  [in]   block_status - ��������� ��������� ������ ����������
     *  \param  [in]   timeout_s    - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - ��������� ������ ���������� ������������� ����������
     *  \retval        false - ��������� ������ ���������� �� ������������� ����������
     */
    bool waitStdAndR6Block(block_statuses_t block_status, float timeout_s = TIME_DELAY::MAX_STD_BLOCK_WAIT_TIME_S);

    /*!
     *  \brief         �������� ����� ���������� ������� ������ � ������ (��������� ������� ���������� ������ �����, ������� ����� �����������)
     *
     *  \return        ����� ������ � ������
     */
    command_type_t getExecutionMethodDisarmNoTag();

    /*!
     *  \brief         ��������/��������� ����� � ������, ����� ��������� ������, ������� ����� � ���. ���������
     *
     *  \param  [in]   tag_status    - �������� ��������� �����
     *  \param  [in]   remote_status - �������� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - ���� ��������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setDisarmAndDoorAndIgn(tag_statuses_t    tag_status,
                                remote_statuses_t remote_status);

    /*!
     *  \brief         ��������� ������/�����, ������� ������������ �� � ��������� ������������ ��
     *
     *  \param  [in]   ahj_type   - ��� ��������� ��
     *  \param  [in]   ahj_status - ��������� ������ ��
     *
     *  \return        bool
     *  \retval        true  - ���� ��������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool resetTagOrRemoteAndCheckAhj(ahj_type_t   ahj_type,
                                     ahj_status_t ahj_status);

    /*!
     *  \brief         �������� ������, �������� ������, ��������� �����, ��������� ���������
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
    bool setSecureAndRemoteAndResetTagAndIgn();

    /*!
     *  \brief         ��������� ����� � ������ \"�������� �����������\"
     *
     *  \param  [in]   timeout_s     - ����� ��������
     *  \param  [in]   pulse_width_s - ��������� ������������ ���������
     *  \param  [in]   pulse_pause_s - ��������� ����� ����� ����������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - �������� �� ��������
     */
    bool checkBuzzerAuth(float timeout_s,
                         float pulse_width_s,
                         float pulse_pause_s);

    /*!
     *  \brief         ��������� ����� � ������ \"�������� �����������\" �� ���� 1
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - �������� �� ��������
     */
    bool checkBuzzerAuthStepOne(float timeout_s);

    /*!
     *  \brief         ��������� ����� � ������ \"�������� �����������\" �� ���� 2
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� ��������
     *  \retval        false - �������� �� ��������
     */
    bool checkBuzzerAuthStepTwo(float timeout_s);

    /*!
     *  \brief         ��������� ������� � ������, ����� ����� � �������� ����������� � ��������� ����
     *
     *  \return        bool
     *  \retval        true  - ���� ��������
     *  \retval        false - ���� �� ����� �� �������
     */
    bool setAndCheckAuthWithBuzzer();

    /*!
     *  \brief         ��������� ������� � ������, ����� �������� ������ � ��������� ����� (������� ����� �����������, ������� ������ - ���)
     *
     *  \param  [in]   secure_zone - ���������� ���� ��� ���������� (��� ������ � ���������� ����������)
     *
     *  \return        bool
     *  \retval        true  - ������ ��������
     *  \retval        false - ������ �� ��������
     */
    bool setArmAndRemoteAndResetTag(secure_zone_t secure_zone);

    /*!
     *  \brief         ��������� ������ ��� �������� � ��������� ������ ������ (�� ��������� �����)
     *
     *  \return        bool
     *  \retval        true  - ������ ���������
     *  \retval        false - ������ �� ���������
     */
    bool setAndCheckDisarmNoDelayNoTag();

    /*!
     *  \brief         ��������� ��������, ������ �������, ������� ��� ���� � �������� ������ (� ��������� ����� �������������� �������� ����� ����������)
     *
     *  \param  [in]   speed              - �������� ����������
     *  \param  [in]   brake_pedal_status - �������� ��������� ������ ������� (������/�� ������)
     *
     *  \return        bool
     *  \retval        true  - ��� ������� ������������� ���������
     *  \retval        false - �� ��� ������� ������������� ���������
     */
    bool setSpeedAndBrakeAndArmZoneClosed(int8_t speed, brake_pedal_status_t brake_pedal_status);

    /*!
     *  \brief         ������������ ���� � ���������, ��� ����� ���������� �������� (� ���������� ����� ����� ��������� '������������ ����������'), ��� �� ��������
     *
     *  \param  [in]   zone_type          - ������������ ����
     *  \param  [in]   block_status       - ��������� ������ ����������
     *  \param  [in]   alarm_check_status - ��������� �� ������� ����� ����������
     *
     *  \return        bool
     *  \retval        true  - ������ ���������� ������������� ����������
     *  \retval        false - ������ ���������� �� ������������� ����������
     */
    bool setZoneAndCheckBlockTime(zone_type_t          zone_type,
                                  block_statuses_t     block_status,
                                  alarm_check_status_t alarm_check_status = CHECK_ALARM);

    /*!
     *  \brief         ������� ������� (�� ��), ��������� ����������, ������������ ����, ��������� ����������
     *
     *  \param  [in]   zone_type - ������������ ����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setAlertAndZoneAndCheckBlock(zone_type_t zone_type);

    /*!
     *  \brief         ��������� � ��������� ����� �� (�� ��������� ������� �� �������� ����������)
     *
     *  \param  [in]   brake_pedal_status - �������� �� ������ ������� ����� ��������� ����� (� �� ��� ����� ��) ��� ���
     *  \param  [in]   ahj_block_type     - ��� ���������� �� (����� ��� ����� 180 ���.)
     *
     *  \return        bool
     *  \retval        true  - ��� ������� ������������� ���������
     *  \retval        false - �� ��� ������� ������������� ���������
     */
    bool setAhjAndCheckBlock(brake_pedal_status_t brake_pedal_status,
                             ahj_block_type_t     ahj_block_type);

    /*!
     *  \brief         ��������� ������ �������, ��������� + �������� ���������, ��������� ����������
     *
     *  \param  [in]   block_status - ��������� ������ ����������
     *
     *  \return        bool
     *  \retval        true  - ������ ���������� ������������� ����������
     *  \retval        false - ������ ���������� �� ������������� ����������
     */
    bool resetBrakeAndRestartIgnAndCheckBlock(block_statuses_t block_status);

    /*!
     *  \brief         �������� ������ ��������, ��������� 180 ���. (���������� �������, ����. �������� ���������� ����� ���� 4 ��), �������� ������ �������� � ��������� ����������
     *
     *  \detailed      ���������� ����������� ����� ������� ��������� ���� (������: �� ����������), �� � ��������� ����������� ������ ��������� ����������
     *
     *  \param  [in]   block_status - ��������� ������ ����������
     *
     *  \return        bool
     *  \retval        true  - ������ ���������� ������������� ����������
     *  \retval        false - ������ ���������� �� ������������� ����������
     */
    bool setSensorAndCheckBlockTwice(block_statuses_t block_status);

    /*!
     *  \brief         �������� ������ ��������, �������� ������ �������, �������� ������ �������� � ��������� ����������
     *
     *  \detailed      ���������� ����������� ����� ������� ��������� ���� (������: �� ����������), �� � ��������� ����������� ������ ��������� ����������
     *
     *  \param  [in]   block_status - ��������� ������ ����������
     *
     *  \return        bool
     *  \retval        true  - ������ ���������� ������������� ����������
     *  \retval        false - ������ ���������� �� ������������� ����������
     */
    bool setSensorAndBrakeAndSensorAndCheckBlock(block_statuses_t block_status);

    /*!
     *  \brief         ��������� ������ �������, �������� �� � ��������� ��������� ����������
     *
     *  \return        bool
     *  \retval        true  - ���������� ��������
     *  \retval        false - ���������� �� ��������
     */
    bool resetBrakeAndSetSensorAndCheckBlockOn();

    /*!
     *  \brief         ������ ������ ������� � ��������� ����������
     *
     *  \param  [in]   block_status - ��������� ������ ����������
     *
     *  \return        bool
     *  \retval        true  - ������ ���������� ������������� ����������
     *  \retval        false - ������ ���������� �� ������������� ����������
     */
    bool setBrakeAndCheckBlock(block_statuses_t block_status);

    /*!
     *  \brief         ��������� ���� � �� (������� � ���������� ��������� �������), �������� ������ �������� � ��������� ����������
     *
     *  \param  [in]   block_status   - ��������� ������ ����������
     *  \param  [in]   car_speed_type - ������������� �� �������� ����� ��������� ��
     *  \param  [in]   car_speed      - �������� ����������
     *
     *  \return        bool
     *  \retval        true  - ������ ���������� ������������� ����������
     *  \retval        false - ������ ���������� �� ������������� ����������
     */
    bool setImmoAndSensorAndCheckBlock(block_statuses_t block_status,
                                       car_speed_type_t car_speed_type,
                                       int8_t           car_speed       = 0);

    /*!
     *  \brief         ��������� ������, �������� ����� � ������ (������� ����� - �����������, ������� ������ - ���)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setDisarmAndSetRemoteAndTagAndIgn();
    
    /*!
     *  \brief         �������� ������ � ��������� ��� �� �� (�� ��������/����������) ��� ���������
     *
     *  \param  [in]   speed_kmph - ��������
     *  \param  [in]   delay_s    - �������� ����� ���������� ��
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setAndResetServiceBySensorNoIgn(int8_t speed_kmph, float delay_s);

    /*!
     *  \brief         �������� ����� ���������� CAN
     *
     *  \return        bool
     *  \retval        true  - ����� ����������
     *  \retval        false - ����� �� ����������
     */
    bool resetCanCarNum();

    /*!
     *  \brief         ������� ��������� ��������� �� ������� � CAN
     *
     *  \param  [in]   timeout_s - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� ��������� �� ������� ��� �� CAN
     */
    bool waitImmoAnalogAndCan(float timeout_s = TIME_DELAY::BYPASS_AFTER_SMS_TIME_S);

    /*!
     *  \brief         ��������� ��, ������� �����/�������� � ��������� ������ ������
     *
     *  \detailed      �� ����������� ���� �� ������ (��� ������� �����), ���� �� ��� (���� ����� �����������)
     *
     *  \param  [in]   zone_type   - ����������� ����
     *  \param  [in]   diag_status - ��������� ������ ������ (�������/�������� �����������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool runEngAndSetZoneAndCheckState(zone_type_t              zone_type,
                                       slp_diag_data_statuses_t diag_status);

    /*!
     *  \brief         �������� ��������� �������� �����������/������� �������
     *
     *  \return        bool
     *  \retval        true  - ��������� ��������
     *  \retval        false - ������ ��� ������ ��������
     */
    bool resetAuthAndImmo();

    /*!
     *  \brief         ��������� ������� ������ �� (��� ����������� ����������� �� ���)
     */
    void saveCurrFw();

    /*!
     *  \brief         ��������� ���������� ������ ��
     *
     *  \return        bool
     *  \retval        true  - �������� ���������
     *  \retval        false - ������ ��� �������� ��������
     */
    bool loadPrevFw();

    /*!
     *  \brief         ��������� ������� ������ ��
     *
     *  \detailed      ����� �������������� ������ ���� ����������� ����� saveCurrFw, ����� ���������� � ������ �� �� ����� (�������� �������� �� ��������, �������� false)
     *
     *  \return        bool
     *  \retval        true  - �������� ���������
     *  \retval        false - ������ ��� �������� ��������
     */
    bool loadCurrFw();

    /*!
     *  \brief         ����� � ������ � ��������� ��������� ����� ������ �� �������� ����
     *
     *  \param  [in]   relay_type - ��� ����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setDisarmAndCheckRelayHoodUnlock(relay_type_t relay_type);

    /*!
     *  \brief         ��������� � ������ � ��������� ��������� ����� ������ �� �������� ����
     *
     *  \param  [in]   relay_type - ��� ����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setArmAndCheckRelayHoodLock(relay_type_t relay_type);

    /*!
     *  \brief         ������� ������� � ��������� ���������� ��������� �� �������� ����
     *
     *  \param  [in]   relay_type - ��� ����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setAlertAndCheckRelayBlock(relay_type_t relay_type);

    /*!
     *  \brief         ��������� �� � �������� ���� � ��������� ������ ����
     *
     *  \param  [in]   fw_update_type - ��� ���������� �� (����� �� ���������� ������/�������� ����� ������/����� �� ����������� ������)
     *  \param  [in]   relay_type     - ��� ����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setFwAndCheckRelay(fw_update_type_t fw_update_type,
                            relay_type_t     relay_type);

    /*!
     *  \brief         ��������� � ��������� ������ ����. ��������� �������
     *
     *  \param  [in]   guard_state                 - ��������� ������� ����� ������ ����� � ����� ����. ����. ������
     *  \param  [in]   alt_srv_btn_type            - ��� �������������� ��������� ������ (�����/���������/������� ������)
     *  \param  [in]   pin_type                    - ��� ��������� ���� ����������� ������ (������/��������)
     *  \param  [in]   emergency_dis_wrong_pin_num - ����� ��������� ���� (���� �������� �������� ���, ����� �������� �� �����������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setAndCheckAltSrvBtnDisarm(slp_diag_data_statuses_t diag_guard_state,
                                    alt_srv_btn_type_t       alt_srv_btn_type,
                                    srv_btn_pin_type_t       pin_type,
                                    emergency_dis_pin_num_t  emergency_dis_wrong_pin_num = EMERGENCY_DIS_PIN_NUM_ALL);

    /*!
     *  \brief         ��������� � ��������� ��������� ���������� � ������ �������������� ��������� �������
     *
     *  \param  [in]   alt_srv_btn_type - ��� �������������� ��������� ������ (�����/���������/������� ������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setAndCheckAltSrvBtnArmFail(alt_srv_btn_type_t alt_srv_btn_type);

    /*!
     *  \brief         ���������� ������ � ������
     *
     *  \detailed      ��� ������, ��� ����� ������� ������������ ���. ���� ��� ����� ������� ����� �������� 3 ����� ����� ������,
     *                 �� ����� ���������� ����� ���� �� 15 �����
     */
    void emergencyDisarming();

    /*!
     *  \brief         ����� � �������� ����������� � ��������� �������� ��������� ���
     *
     *  \param  [in]   zone_type          - ������������ ���� � �������� �����������
     *  \param  [in]   alert_check_status - ��� ����������� ������� (����������/�� ����������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setWaitingAuthAndCheckHandsFreeTimings(zone_type_t            zone_type,
                                                alert_check_statuses_t alert_check_status);

    /*!
     *  \brief         ����� � ������ � ��������� ������� BLE ���� (R6 � R6 ECO)
     *
     *  \return        bool
     *  \retval        true  - �������� ����������
     *  \retval        false - ������ ��� ����������
     */
    bool setDisarmAndResetBleRelayVcc();

    /*!
     *  \brief         ���������������� BLE ���� (R6/R6 ECO) ��������� ��� (�� ���� ���� �����������) � ��������� ���������� �� ��� ����� �����
     *
     *  \param  [in]   relay_registration_sequence - ������� ����������� ���� (����.: ������� R6, ����� R6 ECO)
     *  \param  [in]   registration_count          - ���-�� ����������� ���� (�� ���� ����)
     */
    bool registerBleRelayFewTimesAndCheckBlock(relay_registration_sequence_t relay_registration_sequence,
                                               uint8_t                       registartion_count);

    /*!
     *  \brief         ������ ��������� ������ ����������� ���-�� ��� ��� ���. ������ ��������� �� BLE � ��������� ��������� ���������� ���������� ����� 3 �.
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setBleSettingsModeClicksAndWaitLight();

    /*!
     *  \brief         ��������� ��������� ��������� ����������� ��� ���������/���������� ������ ��������� �� BLE
     *
     *  \brief         out_status - ����������� ������ (��������� ��� ��������� ������ ��� ����������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool checkBleSettingsModeIndication(out_statuses_t out_status);

    /*!
     *  \brief         ��������� � ��������� ���������/���������� ������ ��������� �� BLE
     *
     *  \param  [in]   out_status                - �������� ��������� ������ ��������� �� BLE (���./����.)
     *  \param  [in]   ble_settings_mode_en_type - ������ ��������� (���������� �������/����. ������� ��� ���������/�� ���)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setAndCheckBleSettingsMode(out_statuses_t              out_status,
                                    ble_settings_mode_en_type_t ble_settings_mode_en_type);

    /*!
     *  \brief         �������� �� �� �����/������ � ��������� ���� ��������� � ���
     *
     *  \param  [in]   ahj_type - ��� ��������� �� (�� �����/������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� �� �������
     */
    bool setAhjAndCheckSms(ahj_type_t ahj_type);

    /*!
     *  \brief         ���������� �������� ����� � ��� �������
     *
     *  \detailed      ��������� � ����������� �� �������, ���� ��� ������ - ���������� �������� �� ��������������� ���������� �� �����
     *
     *  \param  [in]   tag_status    - �������� ��������� ������� �����
     *  \param  [in]   remote_status - �������� ��������� ������� �������
     */
    void setTagAndRemoteVcc(tag_statuses_t    tag_status,
                            remote_statuses_t remote_status);

    /*!
     *  \brief         ������� �������� � ��������� �������
     *
     *  \param  [in]   alert_status - ��������� ��������� �������
     *  \param  [in]   time_s       - ����� �������� ������� (���� ���������)/����� �������� ��������� �������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setTrunkAndCheckAlarm(alert_statuses_t alert_status, float time_s);

    /*!
     *  \brief         ��������� ���������� ��������� ������� �����-����
     *
     *  \detailed      ����� ��������� ������ ������������ ���������� ��������� �� ��������� (��. start_btn_check_pulses_count),
     *                 ������������ ��������� (��������� "������������ �������� �� ������ �����-����")
     *                 � ����� ����� ���������� (���������� �������) ������ �����-����
     *
     *  \param  [in]   start_btn_check_pulses_count - ��������� ���������� ���������
     *  \param  [in]   no_pulses_check_time_s       - ����� �������� ��������� ������ ��������� ������ �����-����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkStartStopBtnBlock(start_btn_check_pulses_count_t start_btn_check_pulses_count,
                                float                          no_pulses_check_time_s        = TIME_DELAY::START_STOP_BTN_NO_BLOCK_CHECK_TIME_S);

    /*!
     *  \brief         ������� ������������ ���������� � ��������� ���������� ������� �����-����
     *
     *  \param  [in]   start_stop_block_trigger     - ������� ��� ��������� ����������
     *  \param  [in]   start_btn_check_pulses_count - ��������� ���������� ���������
     *  \param  [in]   no_pulses_check_time_s       - ����� �������� ��������� ������ ��������� ������ �����-����
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setBlockTriggerAndCheckStartStopBtnBlock(start_stop_block_trigger_t     start_stop_block_trigger,
                                                  start_btn_check_pulses_count_t start_btn_check_pulses_count,
                                                  float                          no_pulses_check_time_s        = TIME_DELAY::START_STOP_BTN_NO_BLOCK_CHECK_TIME_S);

    /*!
     *  \brief         ��������� ������� ������� ��� �������� � �������� ����������� �� �����/������ (��� ������ setAndCheckWaitingAuth)
     *
     *  \detailed      ����� ����������� �������� �� ��������� ���������� ������� ��� �������� � �������� ����������� �� �����/������:
     *                 ������ + �����: ����������� �� �����,  ������ �������
     *                 ������ + GSM:   ����������� �� ������, ������ �� ���
     *                 ����� + GSM:    ����������� �� �����,  ������ �� ���
     *
     *  \return        bool
     *  \retval        true  - ������� ����������� ������ ��� �������� � �������� ����������� �� �����/������
     *  \retval        false - �� ��� ������ ������� ��� �������� � �������� ����������� �� �����/������
     */
    bool checkModulesForWaitingAuthTagRem();

    /*!
     *  \brief         ������� � ����� �������� ����������� �� �����/������
     *
     *  \detailed      ����� ���������� ���� � �������� �����������, ������ �� ������� � �������:
     *                 ������ + �����: ����������� �� �����,  ������ �������
     *                 ������ + GSM:   ����������� �� ������, ������ �� ���
     *                 ����� + GSM:    ����������� �� �����,  ������ �� ���
     *
     *  \return        bool
     *  \retval        true  - ������� ����������
     *  \retval        false - ������� �� ����������
     */
    bool setAndCheckWaitingAuthTagRem();

    /*!
     *  \brief         ���������� �������� ����������, ������������ � �������� ���������� �����������
     *
     *  \detailed      ����� ����������� ���������� ����������, ������ �� ������� � �������:
     *                 ������ + �����: ����������� �� �����,  ������ ������� => ����������� �����
     *                 ������ + GSM:   ����������� �� ������, ������ �� ���  => ����������� ������
     *                 ����� + GSM:    ����������� �� �����,  ������ �� ���  => ����������� �����
     *
     *  \param  [in]   out_status - �������� ��������� ������� (���./����.)
     */
    void setAuthDeviceVcc(out_statuses_t out_status);

    /*!
     *  \brief         �������� �� �� ������� (�� ��� ��� ����� ���. ����������, ������ �� ������� � �������)
     *
     *  \return        bool
     *  \retval        true  - �� �� ������� �������� (������ � �������)
     *  \retval        false - ������ ��� ����� � ����� �� �� �������
     */
    bool setAndCheckAhjSmsOrMobile();

    /*!
     *  \brief         ���������� �������� ���������� �� CAN
     *
     *  \detailed      ����������� ��������: ���. ��������� -> ���������� �������� -> ��������� ���������
     *
     *  \param  [in]   speed - ��������������� ��������
     */
    void setCanCarSpeed(int8_t speed);

    /*!
     *  \brief         ��������/��������� �����, ����� � ������ �� Slave � ��������� ������ � ������
     *
     *  \param  [in]   tag_status       - �������� ��������� �����
     *  \param  [in]   slave_type       - ��� Slave (������/CAN)
     *  \param  [in]   diag_data_status - ��������� ��������� ������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setTagAndSlaveDisarmAndCheckDisarm(tag_statuses_t           tag_status,
                                            slave_type_t             slave_type,
                                            slp_diag_data_statuses_t diag_data_status);

    /*!
     *  \brief         �������� ���� ��������� ������ (����� � �����-���� �����)
     *
     *  \param  [in]   clicks_count         - ���-�� ������� ��������� ������
     *  \param  [in]   srv_btn_menu_on_type - ��� ��������� ������ ����� ���� ��������� ������ (�� ���. ��������� ��� ��� ���������)
     *  \param  [in]   diag_data_status     - ��������� ������ ������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setSrvBtnMenu(uint8_t                  clicks_count,
                       srv_btn_menu_on_type_t   srv_btn_menu_on_type,
                       slp_diag_data_statuses_t diag_data_status);

    /*!
     *  \brief         �������� ������ ������� �� ���������
     *
     *  \return        bool
     *  \retval        true  - ������ ������� �������
     *  \retval        false - ��������� ��������� ������� �������
     */
    bool setImmoByIgn();

    /*!
     *  \brief         ��������� � �������� ������ � BLE-����
     *
     *  \detailed      ���� ����������� ������ ��� ������� BLE ������
     *                 ����������� ������: LCD
     *                 ����������� ����: R6, R6 ECO
     */
    void resetAndSetKeyringAndRelayVcc();

    /*!
     *  \brief         �������� + ��������� ��������� (����. ��� ������ �� �����������)
     */
    void setAndResetIgn();

    /*!
     *  \brief         ��������� ������� ������ CAN (������ ��� ����������) � ��������� ������� ���������� ���������� � ������
     *
     *  \detailed      ������������ ������� ������ ����������� � ���������� � Gen6Basic::sendBtnSequence()
     *
     *  \param  [in]   click_count  - ���-�� ������� ������� ������
     *  \param  [in]   signal_count - ���-�� ��������� �������� ���������� ���������� � ������
     *  \param  [in]   siren_status - ����� �� ��������� ������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool sendBtnAndCheckSignals(uint8_t        click_count,
                                uint8_t        signal_count,
                                siren_status_t siren_status);

    /*!
     *  \brief         ��������/��������� ��������� � ��������� ��������� ������ �����������������
     *
     *  \param  [in]   ign_status - �������� ��������� ���������
     *  \param  [in]  *timer      - ��������� �� ������, ������� ����� ���������� ��� ���������� ����������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setIgnAndCheckVideoRegOn(ign_statuses_t ign_status, Timer *timer = NULL);

    /*!
     *  \brief         ��������/��������� ��������� ����� � ��������� ��������� ������ �����������������
     *
     *  \param  [in]   service_status - �������� ��������� ���������� ������ (���./����.)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setServiceAndCheckVideoRegOn(service_statuses_t service_status);

    /*!
     *  \brief         �������� �������� ���������� ������ �����������������
     *
     *  \param  [in]   delay_s    - �������� ����� �����������
     *  \param  [in]   err_time_s - ����������� ����������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool waitVideoRegOff(float delay_s, float err_time_s);

    /*!
     *  \brief         ��������� ��������� � ��������� �������� ���������� ������ �����������������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool resetIgnAndCheckVideoRegOff();

    /*!
     *  \brief         �������� �������, �������� ��������� ����� � ��������� ����� �����������������
     *
     *  \detailed      �������� �������, ��������� �������, �������� ������ (���������� �� ����������), ��������� ������ (service_status)
     *
     *  \param  [in]   service_status - ����� �� ��������� ��������� ����� ����� ���������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setAlertAndServiceAndCheckVideoReg(service_statuses_t service_status);

    /*!
     *  \brief         �������� ���������������� � ���./����. ��������� �����, � ����� ��������� ����� ������ �����������������
     *
     *  \param  [in]   service_status - �������� ��������� ���������� ������ (���./����./�� ���������)
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool setAndCheckServiceAndVideoReg(service_statuses_t service_status);

    /*!
     *  \brief         ��������� ��������� ��������� � ������ ��������� �� BLE
     *
     *  \param  [in]   mobile_conn_type - ��� ���������� � ���. ����������� (����������� �� ����� �� ���������� �� BLE)
     *  \param  [in]   timeout_s        - ����� ��������
     *
     *  \return        bool
     *  \retval        true  - �������� �������
     *  \retval        false - �������� ���������
     */
    bool checkServiceLedInBleSettingsMode(mobile_conn_type_t mobile_conn_type, float timeout_s = TIME_DELAY::BTN_LED_MODE_BLE_CHECK_TIME_S);

    /*!
     *  \brief         1. ������� ������� � ���������� �� �������� ���� 
     */
    void C168671_TestCase();
    
    /*!
     *  \brief         2. ���������� � ������ � ����������� ������ 
     */
    void C168672_TestCase();
    
    /*!
     *  \brief         3. ��������� ��������� �� 3 ������� ������� 
     */
    void C168673_TestCase();
    
    /*!
     *  \brief         4. ������� ������� � ����� �� �������� ����� 
     */
    void C168674_TestCase();
    
    /*!
     *  \brief         5. 1�� ������� ������� ���������� ����� 0,2 ���. 
     */
    void C168675_TestCase();
    
    /*!
     *  \brief         6. 2�� ������� ������� ���������� ����� 0,2 ���. 
     */
    void C168676_TestCase();
    
    /*!
     *  \brief         7. 3�� ������� ������� ���������� ����� 0,2 ���. 
     */
    void C168677_TestCase();
    
    /*!
     *  \brief         8. 3 �������� ������� ���������� ����� 0,2 ���. 
     */
    void C168678_TestCase();
    
    /*!
     *  \brief         9. 1�� ������� ������� ���������� ����� 1 ���. 
     */
    void C168679_TestCase();
    
    /*!
     *  \brief         10. 2�� ������� ������� ���������� ����� 1 ���. 
     */
    void C168680_TestCase();
    
    /*!
     *  \brief         11. 3�� ������� ������� ���������� ����� 1 ���. 
     */
    void C168681_TestCase();
    
    /*!
     *  \brief         12. 3 �������� ������� ���������� ����� 1 ���. 
     */
    void C168682_TestCase();
    
    /*!
     *  \brief         13. ��������� ������ ���� ��� ������� ����� 
     */
    void C168683_TestCase();
    
    /*!
     *  \brief         14. ��������� ���� � ������� ������ 
     */
    void C168684_TestCase();
    
    /*!
     *  \brief         15. ������� ��������� ������ � ���� �������(CAN) Slave 
     */
    void C168685_TestCase();
    
    /*!
     *  \brief         16. ������� ��������� ������ � ���� ������. Slave 
     */
    void C168686_TestCase();
    
    /*!
     *  \brief         17. ������� ��������� ������ ������� � ������ ������ 
     */
    void C168687_TestCase();
    
    /*!
     *  \brief         18. ������� ��������� ������ ������� � ������ ��������� 
     */
    void C168688_TestCase();
    
    /*!
     *  \brief         19. ������� ��������� ������ ��� � ������ ������ 
     */
    void C168689_TestCase();
    
    /*!
     *  \brief         20. ������� ��������� ������ ��� � ������ ��������� 
     */
    void C168690_TestCase();
    
    /*!
     *  \brief         21. ������� ��������� ������ ��������  
     */
    void C168691_TestCase();
    
    /*!
     *  \brief         22. ������� ��������� ������ ������ 
     */
    void C168692_TestCase();
    
    /*!
     *  \brief         23. ������� ��������� ������ ����� ������ ������� 
     */
    void C168693_TestCase();
    
    /*!
     *  \brief         24. ������� ��������� ������ ��������� ����������� 
     */
    void C168694_TestCase();
    
    /*!
     *  \brief         25. ������� ��������� ������ ��� � ������ ��������� 
     */
    void C168695_TestCase();
    
    /*!
     *  \brief         26. ������� ��������� ������ ����. ������ ����� ������ 
     */
    void C168696_TestCase();
    
    /*!
     *  \brief         27. ������� ��������� ������ �������� ���������� ������ 
     */
    void C168697_TestCase();
    
    /*!
     *  \brief         28. ��������� � ���������� ����� � ���� 
     */
    void C168698_TestCase();
    
    /*!
     *  \brief         29. ���������� ���� ������ ������� � ������� ������ 
     */
    void C168699_TestCase();
    
    /*!
     *  \brief         30. ���������� ���� ������ ������� ��� ����� 
     */
    void C168700_TestCase();
    
    /*!
     *  \brief         31. ��������� ���� ������ ������� � ������� ������ 
     */
    void C168701_TestCase();
    
    /*!
     *  \brief         32. ��������� ���� ������ ������� ��� ����� 
     */
    void C168702_TestCase();
    
    /*!
     *  \brief         33. ������� ��������� ���� �������� � ����������� ����� ����� ����� 5 ���. 
     */
    void C168703_TestCase();
    
    /*!
     *  \brief         34. ������� ����. ���� �������� � ������������ ��� 
     */
    void C168704_TestCase();
    
    /*!
     *  \brief         35. ���������� ���� � �������� ������ � ������������ ���������� ������ �� ������� 
     */
    void C168705_TestCase();
    
    /*!
     *  \brief         36. ���������� ���� � �������� ������ � ����������� ���������� ������ �� ������� 
     */
    void C168706_TestCase();
    
    /*!
     *  \brief         37. ���������� ���� ��� ����� � ������������ ���������� ������ �� ������� 
     */
    void C168707_TestCase();
    
    /*!
     *  \brief         38. ���������� ���� ��� ����� � ����������� ���������� ������ �� ������� 
     */
    void C168708_TestCase();
    
    /*!
     *  \brief         39. ������� ��������� ���� ��������� ������� � ����������� ����� ����� ����� 5 ���. 
     */
    void C168709_TestCase();
    
    /*!
     *  \brief         40. ������� ����. ���� ��������� ������� � ������������ ��� 
     */
    void C168710_TestCase();
    
    /*!
     *  \brief         1. ������� ������� 1 ��� � ������ � ����������� ������ ��� ����� ������ 
     */
    void C168711_TestCase();
    
    /*!
     *  \brief         2. ������� ������� 2 ���� � ������ � ����������� ������ ��� ����� ������ 
     */
    void C168712_TestCase();
    
    /*!
     *  \brief         3. ������� ������� 4 ���� � ������ � ����������� ������ ��� ����� ������ 
     */
    void C168713_TestCase();
    
    /*!
     *  \brief         4. ������� ������� 1 ��� � ������ � ����������� ������ ��� ����� ������ 
     */
    void C168714_TestCase();
    
    /*!
     *  \brief         5. ������� ������� 2 ���� � ������ � ����������� ������ ��� ����� ������  
     */
    void C168715_TestCase();
    
    /*!
     *  \brief         6. ������� ������� 4 ���� � ������ � ����������� ������ ��� ����� ������ 
     */
    void C168716_TestCase();
    
    /*!
     *  \brief         7. ��������� � ���������� ����� � ������ ��� �����  
     */
    void C168717_TestCase();
    
    /*!
     *  \brief         8. ���������� ������ ��� ������ ���� ������ 
     */
    void C168718_TestCase();
    
    /*!
     *  \brief         9. ���������� ������ ��� ������ ���� ��������  
     */
    void C168719_TestCase();
    
    /*!
     *  \brief         10. ���������� ������ �������� (��� ����� � � ������������ ���������� ������ �� �������) 
     */
    void C168720_TestCase();
    
    /*!
     *  \brief         11. ���������� ������ �������� (� ������ � � ������������ ���������� ������ �� �������)  
     */
    void C168721_TestCase();
    
    /*!
     *  \brief         12. ���������� ������ �������� (��� ����� � � ����������� ���������� ������ �� �������) 
     */
    void C168722_TestCase();
    
    /*!
     *  \brief         13. ���������� ������ �������� (� ������ � � ����������� ���������� ������ �� �������)  
     */
    void C168723_TestCase();
    
    /*!
     *  \brief         14. ���� ��� ��� ������������ ������ �������� 
     */
    void C168724_TestCase();
    
    /*!
     *  \brief         15. ��������� ������� �������� ����� ��� (5 ���.)  
     */
    void C168725_TestCase();
    
    /*!
     *  \brief         16. ���� ���� ��� � ����������� ����� 5 ���. 
     */
    void C168726_TestCase();
    
    /*!
     *  \brief         17. ���� ������������� ���  
     */
    void C168727_TestCase();
    
    /*!
     *  \brief         ���������� ��������� ��������� ��� �������� � ������ 
     */
    void C170779_TestCase();
    
    /*!
     *  \brief         ����� ����������� �������� ��� �������� � ������ 
     */
    void C170780_TestCase();
    
    /*!
     *  \brief         ���������� ����������� � ������� 
     */
    void C170781_TestCase();
    
    /*!
     *  \brief         ���������� ��������� �� � ������� � ������� 
     */
    void C170782_TestCase();
    
    /*!
     *  \brief         ���������� ��������� �� � ������� � ������� 
     */
    void C170783_TestCase();
    
    /*!
     *  \brief         ������������� ��������� �������������� �� ����� (������) � �������  
     */
    void C170784_TestCase();
    
    /*!
     *  \brief         ������������� ���������� �������������� �� ����� (������) � �������  
     */
    void C170785_TestCase();
    
    /*!
     *  \brief         ���������� � ������ ��������� �������� � ������� 
     */
    void C170786_TestCase();
    
    /*!
     *  \brief         ���������� � ������ SMS �������� � ������� 
     */
    void C170787_TestCase();
    
    /*!
     *  \brief         ���������� � ������ �������� SL � �������  
     */
    void C170788_TestCase();
    
    /*!
     *  \brief         ���������� � ������ ������� �������� � ������� 
     */
    void C170789_TestCase();
    
    /*!
     *  \brief         ���������� � ������ ������ � �������  
     */
    void C170790_TestCase();
    
    /*!
     *  \brief         ���������� � ������ ����� SLO � ������� 
     */
    void C170791_TestCase();
    
    /*!
     *  \brief         ���������� � ������ �� ��������� ����� � ������� 
     */
    void C170792_TestCase();
    
    /*!
     *  \brief         ������ � ������ ��������� �������� � �������  
     */
    void C170793_TestCase();
    
    /*!
     *  \brief         ������ � ������ SMS �������� � ������� 
     */
    void C170794_TestCase();
    
    /*!
     *  \brief         ������ � ������ �������� SL � ������� 
     */
    void C170795_TestCase();
    
    /*!
     *  \brief         ������ � ������ ������� �������� � ������� 
     */
    void C170796_TestCase();
    
    /*!
     *  \brief         ������ � ������ ������ � ������� 
     */
    void C170797_TestCase();
    
    /*!
     *  \brief         ������ � ������ ����� SLO � ������� 
     */
    void C170798_TestCase();
    
    /*!
     *  \brief         ������ � ������ �� ��������� ����� � ������� 
     */
    void C170799_TestCase();
    
    /*!
     *  \brief         �������� SMS "����������" � ������� 
     */
    void C170800_TestCase();
    
    /*!
     *  \brief         ������ OBD-������ � ������� 
     */
    void C170801_TestCase();
    
    /*!
     *  \brief         ������������� ������ ��������� �������� � ������� 
     */
    void C170802_TestCase();
    
    /*!
     *  \brief         ������������� ������ SMS �������� � ������� 
     */
    void C170803_TestCase();
    
    /*!
     *  \brief         ������������� ������ �������� SL � ������� 
     */
    void C170804_TestCase();
    
    /*!
     *  \brief         ������������� ������ ������� �������� � �������  
     */
    void C170805_TestCase();
    
    /*!
     *  \brief         ������������� ������ ������ ������� � �������  
     */
    void C170806_TestCase();
    
    /*!
     *  \brief         ������������� ������ ����� SLO � ������� 
     */
    void C170807_TestCase();
    
    /*!
     *  \brief         ������ ��� ��������� �������� � ������� 
     */
    void C170808_TestCase();
    
    /*!
     *  \brief         ������ ��� SMS �������� � ������� 
     */
    void C170809_TestCase();
    
    /*!
     *  \brief         ������ ��� �������� SL � ������� 
     */
    void C170810_TestCase();
    
    /*!
     *  \brief         ������ ��� ������� �������� � �������  
     */
    void C170811_TestCase();
    
    /*!
     *  \brief         ������ ��� ����� SLO � �������  
     */
    void C170812_TestCase();
    
    /*!
     *  \brief         ���������� �������� ��������� �� ������������ � ������� 
     */
    void C170813_TestCase();
    
    /*!
     *  \brief         ���������� �������� ��������� ��� ��������� ����������� �������� � ������� 
     */
    void C170814_TestCase();
    
    /*!
     *  \brief         ���������� �������� ��������� ��� ���������� � ������ � ���������� ���������� � ������� 
     */
    void C170815_TestCase();
    
    /*!
     *  \brief         �������� ��������� �������� SL � ������� 
     */
    void C170816_TestCase();
    
    /*!
     *  \brief         �������� ��������� ����� SLO � �������  
     */
    void C170817_TestCase();
    
    /*!
     *  \brief         ��������� ������ SMS �������� � �������  
     */
    void C170818_TestCase();
    
    /*!
     *  \brief         ��������� ������ �������� SL � ������� 
     */
    void C170819_TestCase();
    
    /*!
     *  \brief         ��������� ������ ����� SLO � ������� 
     */
    void C170820_TestCase();
    
    /*!
     *  \brief         ��������� ��������� ��� ��������� �������� � �������  
     */
    void C170821_TestCase();
    
    /*!
     *  \brief         ��������� ��������� ��� SMS �������� � ������� 
     */
    void C170822_TestCase();
    
    /*!
     *  \brief         ��������� ��������� ��� �������� SL � ������� 
     */
    void C170823_TestCase();
    
    /*!
     *  \brief         ��������� ��������� ��� ����� SLO � ������� 
     */
    void C170824_TestCase();
    
    /*!
     *  \brief         ���������� ��������� ��� ��������� �������� � �������  
     */
    void C170825_TestCase();
    
    /*!
     *  \brief         ���������� ��������� ��� SMS �������� � ������� 
     */
    void C170826_TestCase();
    
    /*!
     *  \brief         ���������� ��������� ��� �������� SL � �������  
     */
    void C170827_TestCase();
    
    /*!
     *  \brief         ���������� ��������� ��� ����� SLO � ������� 
     */
    void C170828_TestCase();

    /*!
     *  \brief         �������� ���������� �� CAN 
     */
    void C169639_TestCase();

    /*!
     *  \brief         �������� ������� ������� �� CAN 
     */
    void C169640_TestCase();

    /*!
     *  \brief         1. ���������� ������������ ��������� (����. ������ ����������� ����. ����.) 
     */
    void C169641_TestCase();

    /*!
     *  \brief         2. ���������� ������������ ��������� (����. � ������� ������� ���.) 
     */
    void C169642_TestCase();

    /*!
     *  \brief         3. ���������� ���������� ������������ ��������� (����. ������ ����������� ����. ���.) 
     */
    void C169643_TestCase();

    /*!
     *  \brief         4. ���������� ����������� ��������� (����. ������ ����������� ����. ����.) 
     */
    void C169644_TestCase();

    /*!
     *  \brief         5. ���������� ����������� ��������� (����. ������ ����������� ����. ���.) 
     */
    void C169645_TestCase();

    /*!
     *  \brief         6. ��������� ���������� � ������� �� ���������� ��������� 
     */
    void C169646_TestCase();

    /*!
     *  \brief         7. ���������� ���������� ��� �������� ���� ����������� 
     */
    void C169647_TestCase();

    /*!
     *  \brief         8. ���������� � �� ������� ������� ������� �� ��������� ������ 10 ��/� 
     */
    void C169648_TestCase();

    /*!
     *  \brief         9. ���������� � ������� ������� ������� (�������� 10 - �����������) 
     */
    void C169649_TestCase();

    /*!
     *  \brief         10. ���������� ���������� � �� ������� ������� ������� � ����������� �������� 
     */
    void C169650_TestCase();

    /*!
     *  \brief         11. �������� ��������� � ������� � ����������� ����������
     */
    void C169651_TestCase();

    /*!
     *  \brief         12. �������� ��������� � ������� ��� ���������� ���������� 
     */
    void C169652_TestCase();

    /*!
     *  \brief         13. ���������� ������ ������� � ������� ��� ���������� ���������� 
     */
    void C169653_TestCase();

    /*!
     *  \brief         14. ���������� �������� 10 ��/� � ������� ��� ���������� ���������� 
     */
    void C169654_TestCase();

    /*!
     *  \brief         15. ���������� �������� ���� ����������� � ������� ��� ���������� ���������� 
     */
    void C169655_TestCase();

    /*!
     *  \brief         16. ���������� ���������� 
     */
    void C169656_TestCase();

    /*!
     *  \brief         ���������� ������������ ��������� � ��������� 
     */
    void C1578122_TestCase();

    /*!
     *  \brief         1. ���������� ���������� ��� ���������� ������ 
     */
    void C169657_TestCase();

    /*!
     *  \brief         2. ��������� ���������� ��� ��������� ������   
     */
    void C169658_TestCase();

    /*!
     *  \brief         3. ���������� ���������� ���������� ��� ���������� ������ 
     */
    void C169659_TestCase();

    /*!
     *  \brief         4. ���������� ���������� ��� ��������� ��������� 
     */
    void C169660_TestCase();

    /*!
     *  \brief         5. ��������� ���������� ��� ���������� ��������� 
     */
    void C169661_TestCase();

    /*!
     *  \brief         ��������� ���������� ��� ���������� ��������� � ��������� 
     */
    void C1578123_TestCase();

    /*!
     *  \brief         22. ���������� �������������� ���������� �������  
     */
    void C171184_TestCase();

    /*!
     *  \brief         23. ���������� �������������� ����������� ������� 
     */
    void C171185_TestCase();

    /*!
     *  \brief         24. ���������� �������������� ���������� ������ �������  
     */
    void C171188_TestCase();

    /*!
     *  \brief         25. ���������� �������������� ����������� ������ �������  
     */
    void C171189_TestCase();

    /*!
     *  \brief         ����������� ��� ������ ������� �� ������ ������� 
     */
    void C1616162_TestCase();

    /*!
     *  \brief         ����������� ��� ������ ������� �� ��������  
     */
    void C1616163_TestCase();

    /*!
     *  \brief         ����������� ��� ������ ������� �� ������� �������  
     */
    void C1616164_TestCase();

    /*!
     *  \brief         ��� ���������� ������������� (�� ������) ��������� �������������� �� �����  
     */
    void C1624514_TestCase();

    /*!
     *  \brief         ��� ���������� ������������� (�� ��) ��������� �������������� �� �����  
     */
    void C1624515_TestCase();

    /*!
     *  \brief         ��� ���������� ������������� ��������� ������ ������� �� ������  
     */
    void C1624516_TestCase();

    /*!
     *  \brief         ��� ���������� ������������� ��������� ������ ������� �� ���������  
     */
    void C1624517_TestCase();

    /*!
     *  \brief         ���������� ������ ����� � ����������� ������������ �����������  
     */
    void C1624518_TestCase();

    /*!
     *  \brief         ��������� �� �������� ����� ������ � ������ �����  
     */
    void C1624519_TestCase();

    /*!
     *  \brief         ���.-����. ����������� ����� ������ ����� �� ��������������� ��  
     */
    void C1624520_TestCase();

    /*!
     *  \brief         ���.-����. ����������� ����� ������ ����� �� ��������������� ������ �������  
     */
    void C1624521_TestCase();

    /*!
     *  \brief         ���.-����. ������� ����� ������ ����� �� ��������������� ��  
     */
    void C1624522_TestCase();

    /*!
     *  \brief         ���.-����. ������� ����� ������ ����� �� ��������������� ������ �������
     */
    void C1624523_TestCase();

    /*!
     *  \brief         �������� �� ����� 10 ��� ����� ��������� ������
     */
    void C1691143_TestCase();

    /*!
     *  \brief         �������� �� ����� 10 ��� ����� ��������� ������
     */
    void C1691144_TestCase();

    /*!
     *  \brief         �������� �� ����������������� ����� 10 ��� ����� ��������� ������
     */
    void C1691145_TestCase();

    /*!
     *  \brief         �������� �� ��������� ����� 10 ��� ����� ��������� ������
     */
    void C1691146_TestCase();

    /*!
     *  \brief         �������� �� ����� 20 ��� ����� ��������� ������
     */
    void C1691147_TestCase();

    /*!
     *  \brief         �������� �� ����� 20 ��� ����� ��������� ������
     */
    void C1691148_TestCase();

    /*!
     *  \brief         �������� �� ����������������� ����� 20 ��� ����� ��������� ������
     */
    void C1691149_TestCase();

    /*!
     *  \brief         �������� �� ��������� ����� 20 ��� ����� ��������� ������
     */
    void C1691150_TestCase();

    /*!
     *  \brief         �������� �� ����� 10 ��� ����� ���������� ���������
     */
    void C1691151_TestCase();

    /*!
     *  \brief         �������� �� ����� 10 ��� ����� ���������� ���������
     */
    void C1691152_TestCase();

    /*!
     *  \brief         �������� �� ����������������� ����� 10 ��� ����� ���������� ���������
     */
    void C1691153_TestCase();

    /*!
     *  \brief         �������� �� ��������� ����� 10 ��� ����� ���������� ���������
     */
    void C1691154_TestCase();

    /*!
     *  \brief         �������� �� ����� 20 ��� ����� ���������� ���������
     */
    void C1691155_TestCase();

    /*!
     *  \brief         �������� �� ����� 20 ��� ����� ���������� ���������
     */
    void C1691156_TestCase();

    /*!
     *  \brief         �������� �� ����������������� ����� 20 ��� ����� ���������� ���������
     */
    void C1691157_TestCase();

    /*!
     *  \brief         �������� �� ��������� ����� 20 ��� ����� ���������� ���������
     */
    void C1691158_TestCase();

    /*!
     *  \brief         26. ���������� ������ � �������� ������
     */
    void C1827097_TestCase();

    /*!
     *  \brief         27. ���������� ������ � �������� ����������
     */
    void C1827098_TestCase();

    /*!
     *  \brief         28. ���������� ������ � �������� �������
     */
    void C1827099_TestCase();

    /*!
     *  \brief         1. ������� � ������ � �������� ������ � ���������� ����������
     */
    void C1950492_TestCase();

    /*!
     *  \brief         2. ������� � ������ � ����������� ����� � ���������� ����������
     */
    void C1950493_TestCase();

    /*!
     *  \brief         3. ������� � ������ � �������� ������ � ���������� ����������
     */
    void C1950494_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ����� � ���������
     */
    void C1981106_TestCase();

    /*!
     *  \brief         ������� �������� ��� ����������� ����� � ���������
     */
    void C1981107_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ����� � ���������
     */
    void C1981108_TestCase();

    /*!
     *  \brief         ������� ����� ��� ����������� ����� � ���������
     */
    void C1981109_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ����� � ���������
     */
    void C1981110_TestCase();

    /*!
     *  \brief         ������� ����� ��� ����������� ����� � ���������
     */
    void C1981111_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������ � ���������
     */
    void C1981112_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ������ � ���������
     */
    void C1981113_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ������ � ���������
     */
    void C1981114_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ �����, ������ � ���������
     */
    void C1981115_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ �����, ������ � ���������
     */
    void C1981116_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ �����, ������ � ���������
     */
    void C1981117_TestCase();

    /*!
     *  \brief         ������� ����� ��� ����������� ����� � ���������
     */
    void C1981118_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ����� � ��������� ��� �������, ����� � �������
     */
    void C1981119_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ����� � ��������� � �������� � ������� (2,2)
     */
    void C1981120_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ����� � ��������� � �������� � �������� ������/��������
     */
    void C1981121_TestCase();

    /*!
     *  \brief         ������� ����� ��� ����������� ������ � ���������
     */
    void C1981122_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ������ � ��������� ��� �������, ����� � �������
     */
    void C1981123_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ������ � ��������� � �������� � ������� (2,2)
     */
    void C1981124_TestCase();

    /*!
     *  \brief         ������� ����� � ������������ ������ � ��������� � �������� � �������� ������/��������
     */
    void C1981125_TestCase();

    /*!
     *  \brief         ������� �������� ��� ����������� ����� � ��������� ��� �������, ����� � �������
     */
    void C1981126_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ����� � ��������� ��� �������, ����� � �������
     */
    void C1981127_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ����� � ��������� � �������� � ������� (2,2)
     */
    void C1981128_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ����� � ��������� � �������� � �������� ������/��������
     */
    void C1981129_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������ � ��������� ��� �������, ����� � �������
     */
    void C1981131_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������ � ��������� � �������� � ������� (2,2)
     */
    void C1981132_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������ � ��������� � �������� � �������� ������/��������
     */
    void C1981133_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������, ����� � ��������� ��� �������, ����� � �������
     */
    void C1981134_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������, ����� � ��������� � �������� � ������� (2,2)
     */
    void C1981135_TestCase();

    /*!
     *  \brief         ������� �������� � ������������ ������, ����� � ��������� � �������� � �������� ������/��������
     */
    void C1981136_TestCase();

    /*!
     *  \brief         ������� ����� � ���������� Slave-CAN
     */
    void C2043655_TestCase();

    /*!
     *  \brief         ������� �������� � ���������� Slave-CAN
     */
    void C2043656_TestCase();

    /*!
     *  \brief         ������� ����� � ����������� Slave
     */
    void C2043657_TestCase();

    /*!
     *  \brief         ������� �������� � ����������� Slave
     */
    void C2043658_TestCase();

    /*!
     *  \brief         ������� ����� � ���������� ���������� Slave � ����������� ���������
     */
    void C2043659_TestCase();

    /*!
     *  \brief         ������� �������� � ���������� ���������� Slave � ����������� ���������
     */
    void C2043660_TestCase();

    /*!
     *  \brief         ������� ����� � ���������� ���������� Slave � ���������� ���������
     */
    void C2043661_TestCase();

    /*!
     *  \brief         ������� �������� � ���������� ���������� Slave � ���������� ���������
     */
    void C2043662_TestCase();

    /*!
     *  \brief         ��������� ����������������� ������� �� ����� �������� ����� �������� �����
     */
    void C2043663_TestCase();

    /*!
     *  \brief         ��������� ����������������� ������� �� ����� �������� ����� �������� ���������
     */
    void C2043664_TestCase();

    /*!
     *  \brief         1. �������������� �� ������� � ���������� � ���������, � ���������� �������� ����� ��������� ��������
     */
    void C2198656_TestCase();

    /*!
     *  \brief         2. �������������� �� ������� � ���������� � ��� ��������, � ���������� ��������
     */
    void C2198657_TestCase();

    /*!
     *  \brief         3. �������������� �� ������� ��� ��������� � ��� ��������, � ���������� ��������
     */
    void C2198658_TestCase();

    /*!
     *  \brief         4. �������������� �� ������� ��� ��������� � ��� ��������, � ���������� �������� ����� 30 ���
     */
    void C2198659_TestCase();

    /*!
     *  \brief         5. �������������� �� ������� � ���������� � ���������, � ���������� �������� � ����������� ��������� � ������� 30 ���
     */
    void C2198660_TestCase();

    /*!
     *  \brief         6. �������������� �� ������� � ���������� � ���������, � ���������� �������� ����� 30 ���
     */
    void C2198661_TestCase();

    /*!
     *  \brief         7. �������������� �� ������� � ���������� � ���������, � ���������� ����� ����� ��������� ��������
     */
    void C2198662_TestCase();

    /*!
     *  \brief         8. �������������� �� ������� � ���������� � ��� ��������, � ���������� �����
     */
    void C2198663_TestCase();

    /*!
     *  \brief         9. �������������� �� ������� ��� ��������� � ��� ��������, � ���������� �����
     */
    void C2198664_TestCase();

    /*!
     *  \brief         10. �������������� �� ������� ��� ��������� � ��� ��������, � ���������� ����� ����� 30 ��� 
     */
    void C2198665_TestCase();

    /*!
     *  \brief         11. �������������� �� ������� � ���������� � ���������, � ���������� ����� � ����������� ��������� � ������� 30 ���
     */
    void C2198666_TestCase();

    /*!
     *  \brief         12. �������������� �� ������� � ���������� � ���������, � ���������� ����� ����� 30 ���
     */
    void C2198667_TestCase();

    /*!
     *  \brief         13. �������������� �� ������� � ���������� � ���������, � ���������� �������� ��� ��������� ��������
     */
    void C2198668_TestCase();

    /*!
     *  \brief         14. �������������� �� ������� � ���������� � ���������, � ���������� ����� ��� ��������� ��������
     */
    void C2198669_TestCase();

    /*!
     *  \brief         15. �������������� �� ������� ��� ���. � ��� ����. � ���������� ���������(�������� ���� ������) ��� �������� ����� 30 ��� 
     */
    void C2198670_TestCase();

    /*!
     *  \brief         16. �������������� �� ������� ��� ���. � ��� ����. � ���������� ����� 30 ��� ��������� �� ���������
     */
    void C2198671_TestCase();

    /*!
     *  \brief         17. �������� �� � �������� ������
     */
    void C2198672_TestCase();

    /*!
     *  \brief         18. �������� �� � �������� � �������� ������ �� �����
     */
    void C2198673_TestCase();

    /*!
     *  \brief         19. ������������ ������� ��� ���������� �����������
     */
    void C2198674_TestCase();

    /*!
     *  \brief         20. ��������� ������� �� ������ ������ �������
     */
    void C2198675_TestCase();

    /*!
     *  \brief         21. ��������� ������� �� ����� ������ �������
     */
    void C2198676_TestCase();

    /*!
     *  \brief         22. ��������� ������� �� ����� ������ ����������� ��������������� 
     */
    void C2198677_TestCase();

    /*!
     *  \brief         23. ��������� ������� �� ����� ������ ������ 
     */
    void C2198678_TestCase();

    /*!
     *  \brief         24. �������������� �� ������� � ���������� � ����������� ����������������
     */
    void C2198679_TestCase();

    /*!
     *  \brief         25. �������. �� ���������� ��-�� ���������� ���������
     */
    void C2198680_TestCase();

    /*!
     *  \brief         26. ��������� ���. �������. ����� ���������� �������. ����� �����. ������
     */
    void C2198681_TestCase();

    /*!
     *  \brief         �������������� �� ����� � ����������� ���������� � ���������� ��������������� �� �������
     */
    void C2198682_TestCase();

    /*!
     *  \brief         �������������� �� ����� � ���������� ��������������� �� �������
     */
    void C2198683_TestCase();

    /*!
     *  \brief         26. ��������� ���. �������. ����� ���������� �������. ����� �����. ������
     */
    void C2198684_TestCase();

    /*!
     *  \brief         26. ��������� ���. �������. ����� ���������� �������. ����� �����. ������
     */
    void C2198685_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ����� �����������
     */
    void C172480_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ������ �����������
     */
    void C172481_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ��������� �����������
     */
    void C172482_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ��������� ������������
     */ 
    void C172483_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ������� �������� �����������
     */
    void C172484_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ������� ������� �����������
     */
    void C172485_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� �� ������� ����� �����������
     */
    void C172486_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ����� �����������
     */
    void C172487_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ������ �����������
     */
    void C172488_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ��������� �����������
     */
    void C172489_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ��������� �����������
     */
    void C172490_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ������� �������� �����������
     */
    void C172491_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ������� ������� �����������
     */
    void C172492_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ���������� ������� �� ������� ����� �����������
     */
    void C172493_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������������� ������� �����������
     */
    void C172494_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������������� �������� �����������
     */
    void C172495_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� ��� ���������� ��������� �������� 40
     *                 (30 �����������) ����������� (���������� � �������� ����)
     */
    void C172496_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� ��� ���������� ��������� �������� ���� 40 
     *                 ����� 20 (30 �����������) (���������� � �������� ����)
     */
    void C172497_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� ��� ���������� ��������� �������� ���� 40 
     *                 ����� 20 (30 �����������) (���������� � �������� ���) ������ �����
     */
    void C172498_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� ��� ���������� ��������� �������� ���� 40 
     *                 ����� 5 (30 �����������) (���������� � �������� ���)
     */ 
    void C172499_TestCase();

    /*!
     *  \brief         ��� ���������� �� CAN ��� ������� ��� ���������� ��������� �������� ����  
     *                 40 ����� 15 (30 �����������) (���������� � �������� ���) ������ �� �����
     */
    void C172500_TestCase();

    /*!
     *  \brief         ���������� �� CAN ��� ������� ��� ���������� ��������� �������� ����  
     *                 40 ����� 15 (30 �����������) (���������� � �������� ���) ������ �����
     */
    void C172501_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� �����
     */
    void C172473_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� ������
     */
    void C172474_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� ���������
     */
    void C172475_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� ���������
     */
    void C172476_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� ��
     */
    void C172477_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� ��
     */
    void C172478_TestCase();

    /*!
     *  \brief         �������� ������� ������� ������� ������� �� CAN ��� ������� �� ��
     */
    void C172479_TestCase();

    /*!
     *  \brief         �������� ���������� ��������� �� ��� ������ ��������
     */
    void C2405376_TestCase();

    /*!
     *  \brief         �������� ���������� ������� ������� ���������� ��� ������ ��������
     */
    void C2405377_TestCase();

    /*!
     *  \brief         ������� �������� � ������ � ����������� (� ���������� ������� ����� 15 ��� � ������� 60 ���)
     */
    void C2405378_TestCase();

    /*!
     *  \brief         ������� �������� � ������ ����� CAN ������� �� �������� ���������
     */
    void C2405379_TestCase();

    /*!
     *  \brief         ���������� ����������� ����������
     */
    void C2424871_TestCase();

    /*!
     *  \brief         ���������� �������� ��������
     */
    void C2424872_TestCase();

    /*!
     *  \brief         ���������� �������� �������� (����� �� ����������)
     */
    void C2424873_TestCase();

    /*!
     *  \brief         �������� �� �� ���������� ����������� ����������
     */
    void C2424874_TestCase();

    /*!
     *  \brief         ���������� ������� � ��� ����������� (���. ����������)
     */
    void C2424875_TestCase();

    /*!
     *  \brief         ���������� ������� ��� ��� ���������� (����. ����������)
     */
    void C2424876_TestCase();

    /*!
     *  \brief         ���������� ������� � ��� ����������� � ���������� ��������
     */
    void C2438277_TestCase();

    /*!
     *  \brief         ���������� ����������� ���������� (����� �� ��������)
     */
    void C2424877_TestCase();

    /*!
     *  \brief         �������� �� �� ���������� ��������
     */
    void C2424878_TestCase();

    /*!
     *  \brief         ���������� ������� � ��� ����������� (���. ��������)
     */
    void C2424879_TestCase();

    /*!
     *  \brief         ���������� ������� ��� ��� ���������� (����. ��������)
     */
    void C2424880_TestCase();

    /*!
     *  \brief         �������� �� �� 5 ���������� ���������� ��������
     */
    void C2424881_TestCase();

    /*!
     *  \brief         �������� �� �� ���������� ����������� ���������� � �� ���������� ��������
     */
    void C2424882_TestCase();

    /*!
     *  \brief         ���������� ������� �� ���������� � ��� ����������� (���. ����������) 
     */
    void C2424883_TestCase();

    /*!
     *  \brief         ���������� ������� �� ���������� ��� ��� ���������� (����. ����������)
     */
    void C2424884_TestCase();

    /*!
     *  \brief         ���������� ������� �� �������� � ��� ����������� (���. ��������) 
     */
    void C2424885_TestCase();

    /*!
     *  \brief         ���������� ������� �� �������� ��� ��� ���������� (����. ��������)
     */
    void C2424886_TestCase();

    /*!
     *  \brief         �������� �� �� 5 ���������� ���������� ��������
     */
    void C2424887_TestCase();

    /*!
     *  \brief         ���������� ������� �� ���������� � �� �������� � ��� �����������
     */
    void C2424888_TestCase();

    /*!
     *  \brief         ���������� ������� �� ���������� � �� �������� ��� ��� ����������
     */
    void C2424889_TestCase();

    /*!
     *  \brief         ���������� � ������ ����� ��������� ���������� �� BLE  
     */
    void C2514487_TestCase();
        
    /*!
     *  \brief         ������ � ������ ����� ��������� ���������� �� BLE
     */
    void C2514488_TestCase();

    /*!
     *  \brief         �������������� �� ����� ��
     */
    void C2514489_TestCase();

    /*!
     *  \brief         �������� ������� ������� � ��������� "������"
     */
    void C2728677_TestCase();

    /*!
     *  \brief         �������� ������� ������� � ��������� "�������� �����������"
     */
    void C2728678_TestCase();

    /*!
     *  \brief         �������� ������� ������� � ��������� "������ �������"
     */
    void C2728679_TestCase();

    /*!
     *  \brief         �������� ������ ������� ������� � ��
     */
    void C2728680_TestCase();

    /*!
     *  \brief         �������� ������� ��������� � ��������� "�����"
     */
    void C2728681_TestCase();

    /*!
     *  \brief         �������� ������� ������� � ��������� "������" (��������� ����)
     */
    void C2728682_TestCase();

    /*!
     *  \brief         �������� ������� ������� � ��������� "�������� �����������" (��������� ����)
     */
    void C2728683_TestCase();

    /*!
     *  \brief         �������� ������� ������� � ��������� "������ �������" (��������� ����)
     */
    void C2728684_TestCase();

    /*!
     *  \brief         �������� ������ ������� ������� � �� (��������� ����)
     */
    void C2728685_TestCase();

    /*!
     *  \brief         �������� ������� ��������� � ��������� "�����" (��������� ����)
     */
    void C2728686_TestCase();

    /*!
     *  \brief         ������� �� ������� (������ R6)
     */
    void C2799849_TestCase();

    /*!
     *  \brief         ������� �� ������� (������ �������)
     */
    void C2799850_TestCase();

    /*!
     *  \brief         ������� �� ������� (������ R6+�������)
     */
    void C2799851_TestCase();

    /*!
     *  \brief         ������� �� ����� �� (�����-���� 1 �������)
     */
    void C2799852_TestCase();

    /*!
     *  \brief         ������� �� ����� �� (�����-���� 2 ��������)
     */
    void C2799853_TestCase();

    /*!
     *  \brief         ������� �� ����� �� (�����-���� 3 ��������)
     */
    void C2799854_TestCase();

    /*!
     *  \brief         ���������� �� �� ��
     */
    void C2874561_TestCase();

    /*!
     *  \brief         ���������� �� �� �� � �������� �����
     */
    void C2874562_TestCase();

    /*!
     *  \brief         ���������� �� �� �����
     */
    void C2874563_TestCase();

    /*!
     *  \brief         ���������� �� �� ����� � �������� �����
     */
    void C2874564_TestCase();

    /*!
     *  \brief         ���������� �� �� ������
     */
    void C2874565_TestCase();

    /*!
     *  \brief         ���������� �� �� ������ � �������� �����
     */
    void C2874566_TestCase();

    /*!
     *  \brief         ���������� �� �� �������
     */
    void C2874567_TestCase();

    /*!
     *  \brief         ���������� �� �� ������� � �������� �����
     */
    void C2874568_TestCase();

    /*!
     *  \brief         ���������� �� �� �����������
     */
    void C2874569_TestCase();

    /*!
     *  \brief         ���������� �� �� ����������� � �������� �����
     */
    void C2874570_TestCase();

    /*!
     *  \brief         ������� � ������ ������� �� ������ ����� ��������� ���������� (������)
     */
    void C2874875_TestCase();

    /*!
     *  \brief         ������� � ������ ������� �� ������ ����� ��������� ���������� (BLE)
     */
    void C2874876_TestCase();

    /*!
     *  \brief         ������� � ����������� ��������� �� ������ ����� ��������� ���������� (������)
     */
    void C2874877_TestCase();

    /*!
     *  \brief         ������� � ���������� �� ������� ������� (������� ������)
     */
    void C2898585_TestCase();

    /*!
     *  \brief         ������� �� ������� ������� (������� ������)
     */
    void C2898586_TestCase();

    /*!
     *  \brief         ���� ������� � ���������� �� ������� ������� (������� ������)
     */
    void C2898587_TestCase();

    /*!
     *  \brief         ������������� ������� �� ������� ������� (������� ������) ����� 8 ������������ � ����� ������������� ��������������� � ������
     */
    void C2898588_TestCase();

    /*!
     *  \brief         ������������� ������� �� ������� ������� (������� ������) ����� 8 ������������ ����� � ����� ������������� ��������������� � ������
     */
    void C2898589_TestCase();

    /*!
     *  \brief         ������������� ������� �� ������� ������� (������� ������) ����� 8 ������������ ������� ����� � ����� ������������� ��������������� � ������
     */
    void C2907768_TestCase();

    /*!
     *  \brief         ������� ������ ��������� ����� �� �� �����
     */
    void C2907769_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ��������� 
     */
    void C172464_TestCase();

    /*!
     *  \brief         ���������� � ������ ����� �������� � ���������� ��������� (�� �����) 
     */
    void C172466_TestCase();

    /*!
     *  \brief         ���������� � ������ ����� �������� � ���������� ��������� (�����) 
     */
    void C172467_TestCase();

    /*!
     *  \brief         ������� ����� (� ������ � ���������� ���������� �� ������)
     */
    void C172468_TestCase();

    /*!
     *  \brief         ������ � ������
     */
    void C172469_TestCase();

    /*!
     *  \brief         ������� (�� �� ������)
     */
    void C172470_TestCase();

    /*!
     *  \brief         ������� (�� ������, ����� ������� ������)
     */
    void C172471_TestCase();

    /*!
     *  \brief         ������� (�� ������, ����� ��������)
     */
    void C172472_TestCase();

    /*!
     *  \brief         ���������� � ������ (������ ��� ��������)
     */
    void C1662249_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ���������
     */
    void C1662256_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ���������
     */
    void C1662257_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ���������
     */
    void C1662258_TestCase();

    /*!
     *  \brief         ������ � ������
     */
    void C1662259_TestCase();

    /*!
     *  \brief         ���������� � ������ (������ ��� ��������)
     */
    void C1662260_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ���������
     */
    void C1662261_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ���������
     */
    void C1662262_TestCase();

    /*!
     *  \brief         ���������� � ������ ��� ��������� ���������
     */
    void C1662263_TestCase();

    /*!
     *  \brief         ������ � ������
     */
    void C1662264_TestCase();

    /*!
     *  \brief         ���������� � ������ (������ ��� ��������)
     */
    void C1662265_TestCase();

    /*!
     *  \brief         ���������� ������ � �����
     */
    void C169588_TestCase();

    /*!
     *  \brief         ���������� ������ � ������
     */
    void C169589_TestCase();

    /*!
     *  \brief         ��������� ���������� ��� �������
     */
    void C169590_TestCase();

    /*!
     *  \brief         ��������� ������
     */
    void C169591_TestCase();

    /*!
     *  \brief         ��������� ������
     */
    void C169592_TestCase();

    /*!
     *  \brief         ��������� ��������
     */
    void C169593_TestCase();

    /*!
     *  \brief         ��������� ������ ��� ��������� ������
     */
    void C169594_TestCase();

    /*!
     *  \brief         ��������� ������ � ���������� ������������ � R6
     */
    void C169595_TestCase();

    /*!
     *  \brief         ��������� ������ � ���������� ������������ � ��������� �����
     */
    void C169596_TestCase();

    /*!
     *  \brief         ���������� ������ ��� ��������� ������
     */
    void C169597_TestCase();

    /*!
     *  \brief         ���������� ������ � ���������� ������������ � R6
     */
    void C169598_TestCase();

    /*!
     *  \brief         ���������� ������ � ���������� ������������ � ��������� �����
     */
    void C169599_TestCase();

    /*!
     *  \brief         ��������� ��������� ������� �������� ��-�� �� ����������� ������
     */
    void C3052200_TestCase();

    /*!
     *  \brief         ��������� ������� ��������
     */
    void C3052201_TestCase();

    /*!
     *  \brief         ���������� ������� ��������
     */
    void C3052202_TestCase();

    /*!
     *  \brief         ���������� ������� ������� (�� ������) ����� ������� ������ � ������� ��������
     */
    void C3052203_TestCase();

    /*!
     *  \brief         ��������� ���������� ������� ������� (�� ������) ������ � ������� ��������
     */
    void C3052204_TestCase();

    /*!
     *  \brief         ��������� ���������� ������� �������� � ������� ������� (�� ������)
     */
    void C3052205_TestCase();

    /*!
     *  \brief         ���������� ������� ������� (�� ���������) ����� ������� ������ � ������� ��������
     */
    void C3052206_TestCase();

    /*!
     *  \brief         ��������� ���������� ������� ������� (�� ���������) ������ � ������� ��������
     */
    void C3052207_TestCase();

    /*!
     *  \brief         ��������� ���������� ������� �������� � ������� ������� (�� ���������)
     */
    void C3052208_TestCase();

    /*!
     *  \brief         ��������� ���������� ������� �������� � ������� ������� (�� ���������)
     */
    void C2889927_TestCase();

    /*!
     *  \brief         ��������� ����� �� ��������� ���������
     */
    void C3136858_TestCase();

    /*!
     *  \brief         ��������� ����� ����� ��������� ���������
     */
    void C3136859_TestCase();

    /*!
     *  \brief         ��������� ����� �� ���������� ������
     */
    void C3136860_TestCase();

    /*!
     *  \brief         ������ �� R6 ��� ��������� � ���������� ������
     */
    void C3136861_TestCase();

    /*!
     *  \brief         ������ �� R6 �� ����� �������
     */
    void C3136862_TestCase();

    /*!
     *  \brief         ����� �� ������� �� ���������� � ���������� ������ � �����������
     */
    void C3136863_TestCase();

    /*!
     *  \brief         ����� �� ������� �� �������� � ���������� ������ � �����������
     */
    void C3136864_TestCase();

    /*!
     *  \brief         ����� �� ������� �� ���������� � ����������� ������ � �����������
     */
    void C3136865_TestCase();

    /*!
     *  \brief         ����� �� ������� �� �������� � ����������� ������ � �����������
     */
    void C3136866_TestCase();

    /*!
     *  \brief         ���������� �� �� ����� ��� ���������� ��������
     */
    void C3186729_TestCase();

    /*!
     *  \brief         ���������� �� �� ������� ��� ���������� ��������
     */
    void C3186730_TestCase();

    /*!
     *  \brief         ��������� ��������� �� BLE � ���������� �������
     */
    void C3202904_TestCase();

    /*!
     *  \brief         ��������� ��������� �� BLE � ���������� �� �������
     */
    void C3202905_TestCase();

    /*!
     *  \brief         ���������� �� �� ����� ��� ���������� ��
     */
    void C3357809_TestCase();

    /*!
     *  \brief         ���������� �� �� ������� ���������� ��
     */
    void C3357810_TestCase();

    /*!
     *  \brief         ���������� � ������ �� ��������� ����� �� ������� ����� �������� �����
     */
    void C3407086_TestCase();

    /*!
     *  \brief         ���������� � ������ �� ��������� ����� �� ������� ����� �������� �����
     */
    void C3407087_TestCase();

    /*!
     *  \brief         ���������� ������ � ���������� ���������� � ����������� ���������� ������
     */
    void C3413361_TestCase();

    /*!
     *  \brief         ��������� � �������������� ��������� ��� ������ � ����������� ���������� ������
     */
    void C3413362_TestCase();

    /*!
     *  \brief         19. ���������� ������ ����������� � ����������� �������� ������
     */
    void C20445_TestCase();

    /*!
     *  \brief         30. ���������� ������ ����������� � ���������� �������� ������ �������� � �������� ������
     */
    void C20461_TestCase();

    /*!
     *  \brief         31. ���������� ������ ����������� � ���������� �������� ������ �������/��� � �������� ������
     */
    void C20462_TestCase();

    /*!
     *  \brief         32. ���������� ������ ����������� � ���������� �������� ������ �����������/SLO � �������� ������
     */
    void C20463_TestCase();

    /*!
     *  \brief         ��������� ����� �� ����� �������� ����� ����� ��
     */
    void C3688075_TestCase();

    /*!
     *  \brief         �������� �������� �� ����� ������ � �������� ����������
     */
    void C3688076_TestCase();

    /*!
     *  \brief         �� �� ������ �� ��������������
     */
    void C3688077_TestCase();

    /*!
     *  \brief         �� �� ������ �� ��������������
     */
    void C3688078_TestCase();

    /*!
     *  \brief         �� �� ������ �� ��������������
     */
    void C3688079_TestCase();

    /*!
     *  \brief         �������������� ����������� �� ����� �������� ����� ����� ��
     */
    void C3697245_TestCase();

    /*!
     *  \brief         �������� �� (�� ��������) ����� ��������������� ������ �� �������
     */
    void C3752984_TestCase();

    /*!
     *  \brief         �� (�� �������) ����� ��������������� ������ �� �������
     */
    void C3752985_TestCase();

    /*!
     *  \brief         �� (�������������) ����� ��������������� ������ �� �������
     */
    void C3752986_TestCase();

    /*!
     *  \brief         �������� �� (�� �������) ����� ��������������� ������ �� �������
     */
    void C3752987_TestCase();

    /*!
     *  \brief         1. ���������� ���������� ��� ���������� ������  
     */
    void C187473_TestCase();

    /*!
     *  \brief         2. ��������� ���������� ��� ��������� ������
     */
    void C187474_TestCase();

    /*!
     *  \brief         3. ���������� ���������� ���������� ��� ���������� ������
     */
    void C187475_TestCase();

    /*!
     *  \brief         4. ���������� ���������� ��� ��������� ���������
     */
    void C187476_TestCase();

    /*!
     *  \brief         5. ��������� ���������� ��� ���������� ���������
     */
    void C187477_TestCase();

    /*!
     *  \brief         1. ���������� ������������ ��������� (����. ������ ����������� ����. ����.)
     */
    void C187457_TestCase();

    /*!
     *  \brief         2. ���������� ������������ ��������� (����. � ������� ������� ���.)
     */
    void C187458_TestCase();

    /*!
     *  \brief         3. ���������� ���������� ������������ ��������� (����. ������ ����������� ����. ���.)
     */
    void C187459_TestCase();

    /*!
     *  \brief         4. ���������� ����������� ��������� (����. ������ ����������� ����. ����.)
     */
    void C187460_TestCase();

    /*!
     *  \brief         5. ���������� ����������� ��������� (����. ������ ����������� ����. ���.)
     */
    void C187461_TestCase();

    /*!
     *  \brief         6. ��������� ���������� � ������� �� ���������� ���������
     */
    void C187462_TestCase();

    /*!
     *  \brief         7. ���������� ���������� ��� �������� ���� �����������
     */
    void C187463_TestCase();

    /*!
     *  \brief         8. ���������� � �� ������� ������� ������� �� ��������� ������ 10 ��/�
     */
    void C187464_TestCase();

    /*!
     *  \brief         9. ���������� � ������� ������� ������� (�������� 10 - �����������)
     */
    void C187465_TestCase();

    /*!
     *  \brief         10. ���������� ���������� � �� ������� ������� ������� � ����������� ��������
     */
    void C187466_TestCase();

    /*!
     *  \brief         11. �������� ��������� � ������� � ����������� ����������
     */
    void C187467_TestCase();

    /*!
     *  \brief         12. �������� ��������� � ������� ��� ���������� ����������
     */
    void C187468_TestCase();

    /*!
     *  \brief         13. ���������� ������ ������� � ������� ��� ���������� ����������
     */
    void C187469_TestCase();

    /*!
     *  \brief         14. ���������� �������� 10 ��/� � ������� ��� ���������� ����������
     */
    void C187470_TestCase();

    /*!
     *  \brief         15. ���������� �������� ���� ����������� � ������� ��� ���������� ����������
     */
    void C187471_TestCase();

    /*!
     *  \brief         16. ���������� ����������
     */
    void C187472_TestCase();

    /*!
     *  \brief         17. ������� ����� � ��, ��������� ���
     */
    void C187478_TestCase();

    /*!
     *  \brief         18. ������� ����� � ��, ��������� ����, �������� ���
     */
    void C187479_TestCase();

    /*!
     *  \brief         20. ������� ����� � �� � �������������� ����� ��� ����� 60 ���
     */
    void C187481_TestCase();

    /*!
     *  \brief         21. ����� � �� �������������� ����� ��� ����� 60 ���
     */
    void C187482_TestCase();

    /*!
     *  \brief         ���������� ������� �� �� ��� �����������
     */
    void C3777171_TestCase();

    /*!
     *  \brief         ���������� ������� �� �� ��� �����������
     */
    void C3777172_TestCase();

    /*!
     *  \brief         ���������� ������� �� �� ��� �����������
     */
    void C3777173_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ��������
     */
    void C4045282_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ������
     */
    void C4045283_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� �������� � ����������� ����������
     */
    void C4045284_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� ������ � ����������� ����������
     */
    void C4045285_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ��������
     */
    void C4045286_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ������
     */
    void C4045287_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� �������� � ����������� ����������
     */
    void C4045288_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� ������ � ����������� ����������
     */
    void C4045289_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ��������
     */
    void C4045290_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ������
     */
    void C4045291_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� �������� � ����������� ����������
     */
    void C4045292_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� ������ � ����������� ����������
     */
    void C4045293_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ��������
     */
    void C4045294_TestCase();

    /*!
     *  \brief         �������� ��������� � ���������� ������
     */
    void C4045295_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� �������� � ����������� ����������
     */
    void C4045296_TestCase();

    /*!
     *  \brief         �������� ��������� � ����������� ������ � ����������� ����������
     */
    void C4045297_TestCase();

    /*!
     *  \brief         1. ��������� ���������
     */
    void C187456_TestCase();

    /*!
     *  \brief         ���������� ������� �� ����������
     */
    void C4363738_TestCase();

    /*!
     *  \brief         ���������� ������� �� ��������
     */
    void C4363739_TestCase();

    /*!
     *  \brief         1. ��������� ��������� � ������� ������� ��� ������� �����
     */
    void C4466808_TestCase();

    /*!
     *  \brief         2. ������������ ����� � ������� ������� ��� ����������� ���������
     */
    void C4466809_TestCase();

    /*!
     *  \brief         3. ��������� ����� � ������� ������� ��� ����������� ���������
     */
    void C4466810_TestCase();

    /*!
     *  \brief         4. ��������� ��������� � ������� ������� ��� ���������� �����
     */
    void C4466811_TestCase();

    /*!
     *  \brief         5. ���������� ��������� � ������� ������� ��� ���������� �����
     */
    void C4466812_TestCase();

    /*!
     *  \brief         6. ��������� ����� � ������� ������� ��� ���������� ���������
     */
    void C4466813_TestCase();

    /*!
     *  \brief         7. ���������� ��������� � ����� ��� ���������� �����
     */
    void C4466814_TestCase();

    /*!
     *  \brief         8. ��������� ����� � ����� ��� ���������� ���������
     */
    void C4466815_TestCase();

    /*!
     *  \brief         9. ������������ ����� � ����� ��� ���������� ���������
     */
    void C4466816_TestCase();

    /*!
     *  \brief         10. ���������� ��������� � ����� ��� ������� �����
     */
    void C4466817_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ������� �������
     */
    void C4360828_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ������� �����
     */
    void C4360829_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ���������� �������
     */
    void C4360830_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ���������� �����
     */
    void C4360831_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ������� �������
     */
    void C4360832_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ������� �����
     */
    void C4360833_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ���������� �������
     */
    void C4360834_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ���������� �����
     */
    void C4360835_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ������� �������
     */
    void C4360836_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ������� �����
     */
    void C4360837_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ���������� �������
     */
    void C4360838_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ���������� �����
     */
    void C4360839_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ������� �������
     */
    void C4360840_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ������� �����
     */
    void C4360841_TestCase();

    /*!
     *  \brief         ������������ ����� ��� ���������� �������
     */
    void C4360842_TestCase();

    /*!
     *  \brief         ������������ ������� ��� ���������� �����
     */
    void C4360843_TestCase();

    /*!
     *  \brief         �������� ����� � ������ � ���������� ������
     */
    void C4559714_TestCase();

    /*!
     *  \brief         �������� ����� �������� � ������ � ���������� ������ 
     */
    void C4559715_TestCase();

    /*!
     *  \brief         �������� ����� ��������� � ������ � ���������� ������
     */
    void C4559716_TestCase();

    /*!
     *  \brief         �������� ��������� � ������ � ���������� ������
     */
    void C4559717_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 1 ����������� ����� �������� �����
     */
    void C4559718_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 1 ����������� ����� �������� ����� ��������
     */
    void C4559719_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 1 ����������� ����� �������� ����� ���������
     */
    void C4559720_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 1 ����������� ����� �������� ���������
     */
    void C4559721_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 2 ����������� ����� �������� �����
     */
    void C4559722_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 2 ����������� ����� �������� ����� ��������
     */
    void C4559723_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 2 ����������� ����� �������� ����� ���������
     */
    void C4559724_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 2 ����������� ����� �������� ���������
     */
    void C4559725_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 3 ����������� ����� �������� �����
     */
    void C4559726_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 3 ����������� ����� �������� ����� ��������
     */
    void C4559727_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 3 ����������� ����� �������� ����� ���������
     */
    void C4559728_TestCase();

    /*!
     *  \brief         �������� ����� �� ����� ���� 3 ����������� ����� �������� ���������
     */
    void C4559729_TestCase();

    /*!
     *  \brief         �� ���������� ������������ ���������
     */
    void C4559732_TestCase();

    /*!
     *  \brief         �� ���������� ����������� ��������� 
     */
    void C4559733_TestCase();

    /*!
     *  \brief         �� ���������� ��� ������ � ������
     */
    void C4559734_TestCase();

    /*!
     *  \brief         �� ���������� ��� ��������� ���������
     */
    void C4559735_TestCase();

    /*!
     *  \brief         �� ���������� ��� ��������� ��������� � ������
     */
    void C4559751_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ��������������
     */
    void C4686621_TestCase();

    /*!
     *  \brief         ������������ �� �������� � �������������� �� �����
     */
    void C4686622_TestCase();

    /*!
     *  \brief         ������������ �� �������� � �����
     */
    void C4695722_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������� � ������������� � �������
     */
    void C4695723_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������ � ������������� � �������
     */
    void C4695724_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ����������� � ������������� � �������
     */
    void C4695725_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������� ������� � ������������� � �������
     */
    void C4695726_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������ � �������� ���������� � ������������� � �������
     */
    void C4695727_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ����������������� ������� � ������������� � �������
     */
    void C4695728_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������� � ��������� � �����
     */
    void C4695729_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������ � ��������� � �����
     */
    void C4695730_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ����������� � ��������� � �����
     */
    void C4695731_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������� ������� � ��������� � �����
     */
    void C4695732_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ������ � �������� ���������� � ��������� � �����
     */
    void C4695733_TestCase();

    /*!
     *  \brief         ������������ �� �������� � ����������������� ������� � ��������� � �����
     */
    void C4695734_TestCase();

    /*!
     *  \brief         ����������� �� ����� �� ������� �������� ��������� (����� ��������)
     */
    void C4801978_TestCase();

    /*!
     *  \brief         ����������� �� ������� �� ������� �������� ��������� (������ ��������)
     */
    void C4801979_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� ������� �������� ��������� (����� ��������)
     */
    void C4801980_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� ������� �������� ��������� (������ ��������)
     */
    void C4801981_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� �������� ��������� (����� ��������)
     */
    void C4801982_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� �������� ��������� (������ ��������)
     */
    void C4801983_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� ����������� ������� (����� ��������)
     */
    void C4801984_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� ����������� ������� (������ ��������)
     */
    void C4801985_TestCase();

    /*!
     *  \brief         ����������� �� ����� �� ������� �������� ��������� (����� ���������� � ��������)
     */
    void C4828410_TestCase();

    /*!
     *  \brief         ����������� �� ������� �� ������� �������� ��������� (������ ���������� � ��������)
     */
    void C4828411_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� ������� �������� ��������� (����� ���������� � ��������)
     */
    void C4828412_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� ������� �������� ��������� (������ ���������� � ��������)
     */
    void C4828413_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� �������� ��������� (����� ���������� � ��������)
     */
    void C4828414_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� �������� ��������� (������ ���������� � ��������)
     */
    void C4828415_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� ����������� ������� (����� ���������� � ��������)
     */
    void C4828416_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� ����������� ������� (������ ���������� � ��������)
     */
    void C4828417_TestCase();

    /*!
     *  \brief         ����������� �� ����� �� ������� �������� ��������� (����� ����������)
     */
    void C4828418_TestCase();

    /*!
     *  \brief         ����������� �� ������� �� ������� �������� ��������� (������ ����������)
     */
    void C4828419_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� ������� �������� ��������� (����� ����������)
     */
    void C4828420_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� ������� �������� ��������� (������ ����������)
     */
    void C4828421_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� �������� ��������� (����� ����������)
     */
    void C4828422_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� �������� ��������� (������ ����������)
     */
    void C4828423_TestCase();

    /*!
     *  \brief         ����������� �� ����� ����� ����������� ������� (����� ����������)
     */
    void C4828424_TestCase();

    /*!
     *  \brief         ����������� �� ������� ����� ����������� ������� (������ ����������)
     */
    void C4828425_TestCase();

    /*!
     *  \brief         ����� ������� �� � R6
     */
    void C4869745_TestCase();

    /*!
     *  \brief         ����� ������� ��
     */
    void C4869746_TestCase();

    /*!
     *  \brief         ����� ������� R6
     */
    void C4869747_TestCase();

    /*!
     *  \brief         �������� ��
     */
    void C4886622_TestCase();

    /*!
     *  \brief         �������� ��
     */
    void C4886623_TestCase();

    /*!
     *  \brief         1 ������ ����� �� �������� � ��������� ��
     */
    void C4930044_TestCase();

    /*!
     *  \brief         2 ��������� ��������� � ������ �� ��������� ��
     */
    void C4930045_TestCase();

    /*!
     *  \brief         3 ��������� �� � ������ ����� ���������� ���������
     */
    void C4930046_TestCase();

    /*!
     *  \brief         4 ������ ����� �� ��������� ��
     */
    void C4930047_TestCase();

    /*!
     *  \brief         5 ��������� ��������� ��� ����� �� ��������� ��
     */
    void C4930048_TestCase();

    /*!
     *  \brief         6 ��������� �� ��� ����� ����� ���������� ���������
     */
    void C4930049_TestCase();

    /*!
     *  \brief         7 ����� ������� �� ��������� ��
     */
    void C4930050_TestCase();

    /*!
     *  \brief         ����. ����� �� ��������� ��
     */
    void C4930051_TestCase();

    /*!
     *  \brief         �������� ���������� �����������
     */
    void C9257883_TestCase();

    /*!
     *  \brief         ���� ���������� �������� ����������
     */
    void C9257884_TestCase();

    /*!
     *  \brief         ���� ������� �������� ����������
     */
    void C9257885_TestCase();

    /*!
     *  \brief         ���� ���������� � ������� �������� ����������
     */
    void C9257886_TestCase();

    /*!
     *  \brief         ���. ����� �������� �����������
     */
    void C9257887_TestCase();

    /*!
     *  \brief         ����. ����� �������� �����������
     */
    void C9257888_TestCase();

    /*!
     *  \brief         ����� ������ ����� ������ ������� ����� ����
     */
    void C9257889_TestCase();

    /*!
     *  \brief         ����� ������ ����� ������ ������� ����� ����
     */
    void C9257890_TestCase();

    /*!
     *  \brief         ��������� ����� ��� �������
     */
    void C9377438_TestCase();

    /*!
     *  \brief         ��������� ����� � ��������
     */
    void C9377439_TestCase();

    /*!
     *  \brief         ��������� ������� ��� �����
     */
    void C9377440_TestCase();

    /*!
     *  \brief         ��������� ������� � ������
     */
    void C9377441_TestCase();

    /*!
     *  \brief         1. ������� �� �� ��
     */
    void C9491969_TestCase();

    /*!
     *  \brief         2. �������� �� � ������� � ����������� ���������
     */
    void C9491970_TestCase();

    /*!
     *  \brief         3. �������� �� � ������� � ���������� ���������
     */
    void C9491971_TestCase();

    /*!
     *  \brief         4. �������� ��������� ����� ������� �� ��
     */
    void C9491972_TestCase();

    /*!
     *  \brief         5. �������� ��������� ����� ������� �� �� ��
     */
    void C9491973_TestCase();

    /*!
     *  \brief         5. ���������� ���������� �� ��
     */
    void C9491974_TestCase();

    /*!
     *  \brief         6. ���������� ���������� �� �� ��
     */
    void C9491975_TestCase();

    /*!
     *  \brief         7. ���������� ������� � �������
     */
    void C9491976_TestCase();

    /*!
     *  \brief         8. ���������� ������� � ������
     */
    void C9491977_TestCase();

    /*!
     *  \brief         9. ���������� � ����������� ��
     */
    void C9491978_TestCase();

    /*!
     *  \brief         ���������� � ������������ ��������
     */
    void C9491979_TestCase();

    /*!
     *  \brief         ���������� � ����������� ��������
     */
    void C9491980_TestCase();

    /*!
     *  \brief         ���������� � ������������ ���������
     */
    void C9491981_TestCase();

    /*!
     *  \brief         ���������� � ����������� ���������
     */
    void C9491982_TestCase();

    /*!
     *  \brief         10. ������� �� �� � ����������� ���������
     */
    void C9501309_TestCase();

    /*!
     *  \brief         11. ������� �� �� � ���������� ���������
     */
    void C9501310_TestCase();

    /*!
     *  \brief         ���������� � ������� ������� � ����������� �� ���������� ��� ������
     */
    void C9501311_TestCase();

    /*!
     *  \brief         ���������� � �� ������� ������� � ����������� �� ���������� ��� ������
     */
    void C9501312_TestCase();

    /*!
     *  \brief         ���������� � ������� ������� � �������� �� ���������� ��� ������
     */
    void C9501313_TestCase();

    /*!
     *  \brief         ���������� � �� ������� ������� � �������� �� ���������� ��� ������
     */
    void C9501314_TestCase();

    /*!
     *  \brief         1. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9501315_TestCase();

    /*!
     *  \brief         2. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9501316_TestCase();

    /*!
     *  \brief         3. ��������� �� � ������� ������� � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9501317_TestCase();

    /*!
     *  \brief         4. ��������� ��������� ���������� � �� ��� ������ (�������� �� 10 �� ����������)
     */
    void C9501318_TestCase();

    /*!
     *  \brief         5. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9501319_TestCase();

    /*!
     *  \brief         6. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (��� ������ � ��������)
     */
    void C9501320_TestCase();

    /*!
     *  \brief         7. ��������� �� � ������� ������� � ��� ���������� ������� ������ (��� ������ � ��������)
     */
    void C9501321_TestCase();

    /*!
     *  \brief         8. ��������� �� � ������� ������� � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9501322_TestCase();

    /*!
     *  \brief         9. ��������� ��������� ���������� � �� ��� ������ (��� ������ � ��������)
     */
    void C9501323_TestCase();

    /*!
     *  \brief         10. ��������� �� ��� ������� ������ � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9501324_TestCase();

    /*!
     *  \brief         11. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9508181_TestCase();

    /*!
     *  \brief         12. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9508182_TestCase();

    /*!
     *  \brief         13. ��������� �� � ������� ������� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9508183_TestCase();

    /*!
     *  \brief         14. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9508184_TestCase();

    /*!
     *  \brief         15. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� 10)
     */
    void C9508185_TestCase();

    /*!
     *  \brief         16. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� 10)
     */
    void C9508186_TestCase();

    /*!
     *  \brief         17. ��������� �� � ������� ������� � ����������� ������� ������ (�������� ���� 10)
     */
    void C9508187_TestCase();

    /*!
     *  \brief         18. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� ���� 10)
     */
    void C9508188_TestCase();

    /*!
     *  \brief         19. 180 ���. �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9508189_TestCase();

    /*!
     *  \brief         20. 180 ���. �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9508190_TestCase();

    /*!
     *  \brief         21. 180 ���. �� � ������� ������� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9508191_TestCase();

    /*!
     *  \brief         22. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9508192_TestCase();

    /*!
     *  \brief         23. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9508193_TestCase();

    /*!
     *  \brief         24. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9508194_TestCase();

    /*!
     *  \brief         25. ������� ������ � �� � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9518545_TestCase();

    /*!
     *  \brief         26. ������� ������ � �� � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9518546_TestCase();

    /*!
     *  \brief         27. ������� ������ � �� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9518547_TestCase();

    /*!
     *  \brief         28. ������� ������ � �� ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9518548_TestCase();

    /*!
     *  \brief         1. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9645327_TestCase();

    /*!
     *  \brief         2. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9645328_TestCase();

    /*!
     *  \brief         3. ��������� �� � ������� ������� � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9645329_TestCase();

    /*!
     *  \brief         4. ��������� ��������� ���������� � �� ��� ������ (�������� �� 10 �� ����������)
     */
    void C9645330_TestCase();

    /*!
     *  \brief         5. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9645331_TestCase();

    /*!
     *  \brief         6. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (��� ������ � ��������)
     */
    void C9645313_TestCase();

    /*!
     *  \brief         7. ��������� �� � ������� ������� � ��� ���������� ������� ������ (��� ������ � ��������)
     */
    void C9645314_TestCase();

    /*!
     *  \brief         8. ��������� �� � ������� ������� � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9645315_TestCase();

    /*!
     *  \brief         9. ��������� ��������� ���������� � �� ��� ������ (��� ������ � ��������)
     */
    void C9645316_TestCase();

    /*!
     *  \brief         10. ��������� �� ��� ������� ������ � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9645317_TestCase();

    /*!
     *  \brief         11. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9645334_TestCase();

    /*!
     *  \brief         12. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9645335_TestCase();

    /*!
     *  \brief         13. ��������� �� � ������� ������� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9645336_TestCase();

    /*!
     *  \brief         14. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9645337_TestCase();

    /*!
     *  \brief         15. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� 10)
     */
    void C9645320_TestCase();

    /*!
     *  \brief         16. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� 10)
     */
    void C9645321_TestCase();

    /*!
     *  \brief         17. ��������� �� � ������� ������� � ����������� ������� ������ (�������� ���� 10)
     */
    void C9645322_TestCase();

    /*!
     *  \brief         18. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� ���� 10)
     */
    void C9645323_TestCase();

    /*!
     *  \brief         19. 180 ���. �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9645338_TestCase();

    /*!
     *  \brief         20. 180 ���. �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9645339_TestCase();

    /*!
     *  \brief         21. 180 ���. �� � ������� ������� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9645340_TestCase();

    /*!
     *  \brief         22. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9645341_TestCase();

    /*!
     *  \brief         23. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9645318_TestCase();

    /*!
     *  \brief         24. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9645332_TestCase();

    /*!
     *  \brief         25. ������� ������ � �� � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9645333_TestCase();

    /*!
     *  \brief         26. ������� ������ � �� � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9645319_TestCase();

    /*!
     *  \brief         27. ������� ������ � �� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9645342_TestCase();

    /*!
     *  \brief         28. ������� ������ � �� ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9645343_TestCase();

    /*!
     *  \brief         1. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9658759_TestCase();

    /*!
     *  \brief         2. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9658760_TestCase();

    /*!
     *  \brief         3. ��������� �� � ������� ������� � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9658761_TestCase();

    /*!
     *  \brief         4. ��������� ��������� ���������� � �� ��� ������ (�������� �� 10 �� ����������)
     */
    void C9658762_TestCase();

    /*!
     *  \brief         5. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9658763_TestCase();

    /*!
     *  \brief         6. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (��� ������ � ��������)
     */
    void C9658745_TestCase();

    /*!
     *  \brief         7. ��������� �� � ������� ������� � ��� ���������� ������� ������ (��� ������ � ��������)
     */
    void C9658746_TestCase();

    /*!
     *  \brief         8. ��������� �� � ������� ������� � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9658747_TestCase();

    /*!
     *  \brief         9. ��������� ��������� ���������� � �� ��� ������ (��� ������ � ��������)
     */
    void C9658748_TestCase();

    /*!
     *  \brief         10. ��������� �� ��� ������� ������ � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9658749_TestCase();

    /*!
     *  \brief         11. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9658766_TestCase();

    /*!
     *  \brief         12. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9658767_TestCase();

    /*!
     *  \brief         13. ��������� �� � ������� ������� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9658768_TestCase();

    /*!
     *  \brief         14. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9658769_TestCase();

    /*!
     *  \brief         15. ��������� �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� 10)
     */
    void C9658752_TestCase();

    /*!
     *  \brief         16. ��������� �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� 10)
     */
    void C9658753_TestCase();

    /*!
     *  \brief         17. ��������� �� � ������� ������� � ����������� ������� ������ (�������� ���� 10)
     */
    void C9658754_TestCase();

    /*!
     *  \brief         18. ��������� �� ��� ������� ������ � ����������� ������� ������ (�������� ���� 10)
     */
    void C9658755_TestCase();

    /*!
     *  \brief         19. 180 ���. �� ��� ������� ������ � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9658770_TestCase();

    /*!
     *  \brief         20. 180 ���. �� � ������� ������� � ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9658771_TestCase();

    /*!
     *  \brief         21. 180 ���. �� � ������� ������� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9658772_TestCase();

    /*!
     *  \brief         22. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9658773_TestCase();

    /*!
     *  \brief         23. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9658750_TestCase();

    /*!
     *  \brief         24. 180 ���. �� ��� ������� ������ � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9658764_TestCase();

    /*!
     *  \brief         25. ������� ������ � �� � ����������� ������� ������ (�������� �� 10 �� ����������)
     */
    void C9658765_TestCase();

    /*!
     *  \brief         26. ������� ������ � �� � ����������� ������� ������ (��� ������ � ��������)
     */
    void C9658751_TestCase();

    /*!
     *  \brief         27. ������� ������ � �� � ����������� ������� ������ (�������� ���� ����������)
     */
    void C9658774_TestCase();

    /*!
     *  \brief         28. ������� ������ � �� ��� ���������� ������� ������ (�������� ���� ����������)
     */
    void C9658775_TestCase();

    /*!
     *  \brief         ���������� �� �� � �� ��� ���������� ��������
     */
    void C9745928_TestCase();

    /*!
     *  \brief         ���������� �� �� � ��������� � �� ��� ���������� ��������
     */
    void C9745929_TestCase();

    /*!
     *  \brief         ���������� �� �� � �� ��� ������ � ��������
     */
    void C9745930_TestCase();

    /*!
     *  \brief         ���������� �� �� � ��������� � �� ��� ������ � ��������
     */
    void C9745931_TestCase();

    /*!
     *  \brief         ���������� �� �� � �� ��� ������������ ��������
     */
    void C9745932_TestCase();

    /*!
     *  \brief         ���������� ���������� ���� ��� ����������� ���������
     */
    void C9801189_TestCase();

    /*!
     *  \brief         ���������� ���������� ���� �� ���������� ��������
     */
    void C9801190_TestCase();

    /*!
     *  \brief         ��������� ���� ������ � ������� ����� �������� ��������� (����������� �� �������)
     */
    void C9913925_TestCase();

    /*!
     *  \brief         ��������� ���� ������ � ������� ����� �������� ��������� (����������� �� �����)
     */
    void C9913926_TestCase();

    /*!
     *  \brief         1. ���������� �������� ��� ��������� � ���������� ����������
     */
    void C10271054_TestCase();

    /*!
     *  \brief         2. ���������� ������� ��� ��������� � ���������� ����������
     */
    void C10271055_TestCase();

    /*!
     *  \brief         3. ��������� ������� ��� ��������� � ���������� ����������
     */
    void C10271056_TestCase();

    /*!
     *  \brief         4. ���������� �������� ��� ��������� � ����������� ����������
     */
    void C10271057_TestCase();

    /*!
     *  \brief         5. ���������� ������� ��� ��������� � ����������� ����������
     */
    void C10271058_TestCase();

    /*!
     *  \brief         6. ��������� ������� ��� ��������� � ����������� ����������
     */
    void C10271059_TestCase();

    /*!
     *  \brief         7. ���������� �������� � ���������� � ���������� ���������� � ����������� ����������� ���������
     */
    void C10271060_TestCase();

    /*!
     *  \brief         8. ���������� ������� � ���������� � ���������� ���������� � ����������� ����������� ���������
     */
    void C10271061_TestCase();

    /*!
     *  \brief         9. ��������� ������� � ���������� � ���������� ���������� � ����������� ����������� ���������
     */
    void C10271062_TestCase();

    /*!
     *  \brief         10. ���������� �������� � ���������� � ���������� ���������� � ����������� ���������� ��������
     */
    void C10271063_TestCase();

    /*!
     *  \brief         11. ���������� ������� � ���������� � ���������� ���������� � ����������� ���������� �������
     */
    void C10271064_TestCase();

    /*!
     *  \brief         12. ��������� ������� � ���������� � ���������� ���������� � ����������� ����������� �������
     */
    void C10271065_TestCase();

    /*!
     *  \brief         13. ���������� �������� � ���������� � ����������� ����������
     */
    void C10271066_TestCase();

    /*!
     *  \brief         14. ���������� ������� � ���������� � ����������� ����������
     */
    void C10271067_TestCase();

    /*!
     *  \brief         15. ��������� ������� � ���������� � ����������� ����������
     */
    void C10271068_TestCase();

    /*!
     *  \brief         16. ��������� ��������� � ���������� ���������� � ����������� ����������� ���������
     */
    void C10271069_TestCase();

    /*!
     *  \brief         17. ��������� �������� � ���������� � ���������� ���������� � �������� ����������
     */
    void C10271070_TestCase();

    /*!
     *  \brief         18. ��������� ������� � ���������� � ���������� ���������� � �������� ����������
     */
    void C10271071_TestCase();

    /*!
     *  \brief         19. ���������� ������� � ���������� � ���������� ���������� � �������� ����������
     */
    void C10271072_TestCase();

    /*!
     *  \brief         20. ��������� �������� � ���������� � ����������� ���������� � �������� ����������
     */
    void C10271073_TestCase();

    /*!
     *  \brief         21. ��������� ������� � ���������� � ����������� ���������� � �������� ����������
     */
    void C10271074_TestCase();

    /*!
     *  \brief         22. ���������� ������� � ���������� � ����������� ���������� � �������� ����������
     */
    void C10271075_TestCase();

    /*!
     *  \brief         ���������� �������� � R4
     */
    void C10288872_TestCase();

    /*!
     *  \brief         ���������� �������� � R4v2
     */
    void C10288873_TestCase();

    /*!
     *  \brief         ���������� �������� � R6
     */
    void C10288874_TestCase();

    /*!
     *  \brief         ���������� �������� � R6eco
     */
    void C10288875_TestCase();

    /*!
     *  \brief         ���������� �������� � R4 � ������ �� ���� 2.18
     */
    void C10288876_TestCase();

    /*!
     *  \brief         ���������� �������� � R4v2 � ������ �� ���� 2.18
     */
    void C10288877_TestCase();

    /*!
     *  \brief         ���������� �������� � R6 � ������ �� ���� 2.18
     */
    void C10288878_TestCase();

    /*!
     *  \brief         ���������� �������� � R6eco � ������ �� ���� 2.18
     */
    void C10288879_TestCase();

    /*!
     *  \brief         ����� �������� � R4
     */
    void C10288880_TestCase();

    /*!
     *  \brief         ����� �������� � R4v2
     */
    void C10288881_TestCase();

    /*!
     *  \brief         ����� �������� � R6
     */
    void C10288882_TestCase();

    /*!
     *  \brief         ����� �������� � R6eco
     */
    void C10288883_TestCase();

    /*!
     *  \brief         ������� �����
     */
    void C10369958_TestCase();

    /*!
     *  \brief         ������� ��������
     */
    void C10369959_TestCase();

    /*!
     *  \brief         ������� ����� � ���������� ����������
     */
    void C10369960_TestCase();

    /*!
     *  \brief         ������� �������� � ���������� ����������
     */
    void C10369961_TestCase();

    /*!
     *  \brief         ������� �����
     */
    void C10369962_TestCase();

    /*!
     *  \brief         ������� ��������
     */
    void C10369963_TestCase();

    /*!
     *  \brief         ������� ����� � ���������� ����������
     */
    void C10369964_TestCase();

    /*!
     *  \brief        ������� �������� � ���������� ����������
     */
    void C10369965_TestCase();

    /*!
     *  \brief         ������� ����� � ���������� ����������
     */
    void C10369966_TestCase();

    /*!
     *  \brief         ������� �������� � ���������� ����������
     */
    void C10369967_TestCase();

    /*!
     *  \brief         21. ��������� ��������� � ������� � ������
     */
    void C10415982_TestCase();

    /*!
     *  \brief         22. ��������� ��������� � �������
     */
    void C10415983_TestCase();

    /*!
     *  \brief         23. ��������� ��������� � ������� � ������� �� �������
     */
    void C10415984_TestCase();

    /*!
     *  \brief         24. ��������� ��������� �� � ������� � ���������� ��������� � �������
     */
    void C10415985_TestCase();

    /*!
     *  \brief         25. ��������� � ���������� ��������� � �������
     */
    void C10415986_TestCase();

    /*!
     *  \brief         26. ��������� ��������� � ������� � ���������� ��������� �� � �������
     */
    void C10415987_TestCase();

    /*!
     *  \brief         27. ��������� � ������ ������� � ������� � ������
     */
    void C10415988_TestCase();

    /*!
     *  \brief         28. ��������� � ������ ������� � ������� � ������ � �� �������
     */
    void C10415989_TestCase();

    /*!
     *  \brief         29. ��������� ������������ ����� ��� � ������� � ������
     */
    void C10415990_TestCase();

    /*!
     *  \brief         30. ��������� ������������ ����� ��� � �������
     */
    void C10415991_TestCase();

    /*!
     *  \brief         31. ��������� ������������ ����� ��� � ������� � ����� �� �������
     */
    void C10415992_TestCase();

    /*!
     *  \brief         ������� ����� ������� ��� ����� 10 ���
     */
    void C10419622_TestCase();

    /*!
     *  \brief         ������ ����������� 1�� �����
     */
    void C10419623_TestCase();

    /*!
     *  \brief         ������ ����������� 2�� �����
     */
    void C10419624_TestCase();

    /*!
     *  \brief         ������ ����������� 3�� �����
     */
    void C10419625_TestCase();

    /*!
     *  \brief         ������ ����������� 4�� �����
     */
    void C10419626_TestCase();

    /*!
     *  \brief         ������ ������������ ���
     */
    void C10419627_TestCase();

    /*!
     *  \brief         ������ ���������� ���
     */
    void C10419628_TestCase();

    /*!
     *  \brief         ���������� ����������� �����
     */
    void C10419629_TestCase();

    /*!
     *  \brief         ���������� ������� ������� �����
     */
    void C10419630_TestCase();

    /*!
     *  \brief         ������ ����������� 1�� �����
     */
    void C10419632_TestCase();

    /*!
     *  \brief         ������ ����������� 2�� �����
     */
    void C10419633_TestCase();

    /*!
     *  \brief         ������ ����������� 3�� �����
     */
    void C10419634_TestCase();

    /*!
     *  \brief         ������ ����������� 4�� �����
     */
    void C10419635_TestCase();

    /*!
     *  \brief         ������ ������������ ���
     */
    void C10419636_TestCase();

    /*!
     *  \brief         ������ ���������� ���
     */
    void C10419637_TestCase();

    /*!
     *  \brief         ���������� ����������� �����
     */
    void C10419638_TestCase();

    /*!
     *  \brief         ���������� ������� ������� �����
     */
    void C10419639_TestCase();

    /*!
     *  \brief         ������ ����������� 1�� �����
     */
    void C10419640_TestCase();

    /*!
     *  \brief         ������ ����������� 2�� �����
     */
    void C10419641_TestCase();

    /*!
     *  \brief         ������ ����������� 3�� �����
     */
    void C10419642_TestCase();

    /*!
     *  \brief         ������ ����������� 4�� �����
     */
    void C10419643_TestCase();

    /*!
     *  \brief         ������ ������������ ���
     */
    void C10419644_TestCase();

    /*!
     *  \brief         ������ ���������� ���
     */
    void C10419645_TestCase();

    /*!
     *  \brief         ���������� ����������� �����
     */
    void C10419646_TestCase();

    /*!
     *  \brief         ���������� ������� ������� �����
     */
    void C10419647_TestCase();

    /*!
     *  \brief         ���������� � ������ �������� ��������
     */
    void C10419648_TestCase();

    /*!
     *  \brief         ������������ ��������� ��� ���������� ��������
     */
    void C10419649_TestCase();

    /*!
     *  \brief         ���������� � ������ � ������� �����
     */
    void C10419650_TestCase();

    /*!
     *  \brief         ������������ ��������� ��� ���������� ������
     */
    void C10419651_TestCase();

    /*!
     *  \brief         ������� � ������� �� ����� ������ ��� � ������ � ���������� ����������
     */
    void C10485319_TestCase();

    /*!
     *  \brief         ������� � ������� �� ��������� ������ ��� � ������ � ���������� ����������
     */
    void C10485320_TestCase();

    /*!
     *  \brief         ������� � ������ � ���������� ���������� �� ����� ������ ��� � �������
     */
    void C10485321_TestCase();

    /*!
     *  \brief         ������� � ������ � ���������� ���������� �� ��������� ������ ��� � �������
     */
    void C10485322_TestCase();

    /*!
     *  \brief         ��������� ��������� �� BLE � ���������� ������� ��� ���������
     */
    void C10489015_TestCase();

    /*!
     *  \brief         ��������� ��������� �� BLE � ���������� �� ���
     */
    void C10489016_TestCase();

    /*!
     *  \brief         1.5.���� ����� ��������� ����������
     */
    void C15185_TestCase();

    /*!
     *  \brief         1.29.����� ����� ��������� ����������
     */
    void C15209_TestCase();

    /*!
     *  \brief         2.5.���� ����� ��������� ����������
     */
    void C15215_TestCase();

    /*!
     *  \brief         3.5.���� ����� ��������� ����������
     */
    void C15221_TestCase();

    /*!
     *  \brief         4.5.���� ����� ��������� ����������
     */
    void C15227_TestCase();

    /*!
     *  \brief         5.5.���� ����� ��������� ����������
     */
    void C15233_TestCase();

    /*!
     *  \brief         ���������� � ������ ��������� ������� ��� ��������� ���������
     */
    void C10634813_TestCase();

    /*!
     *  \brief         ������� � ���� ���������������� �������� ��� ���������
     */
    void C10634814_TestCase();

    /*!
     *  \brief         ������� R6 � ����� ���������� �������� ��� ���������
     */
    void C10634815_TestCase();

    /*!
     *  \brief         ������� � ����� ����������� ��� ���������
     */
    void C10634816_TestCase();

    /*!
     *  \brief         ���������� � ������ ��������� ������� ����� ��������� ������� �������
     */
    void C10634817_TestCase();

    /*!
     *  \brief         ������� � ���� ���������������� �������� ����� ��������� ������� �������
     */
    void C10634818_TestCase();

    /*!
     *  \brief         ������� R6 � ����� ���������� �������� ����� ��������� ������� �������
     */
    void C10634819_TestCase();

    /*!
     *  \brief         ������� � ����� ����������� ����� ��������� ������� �������
     */
    void C10634820_TestCase();

    /*!
     *  \brief         ������� � ����� ��������� �� BLE ����� ��������� ������� �������
     */
    void C10634821_TestCase();

    /*!
     *  \brief         ��� ���������� � �� �� �����
     */
    void C10634826_TestCase();

    /*!
     *  \brief         ��� ���������� � �� �� �������
     */
    void C10634827_TestCase();

    /*!
     *  \brief         ���������������� R6 1 ���
     */
    void C10645333_TestCase();

    /*!
     *  \brief         ���������������� R6 2 ����
     */
    void C10645334_TestCase();

    /*!
     *  \brief         ���������������� R6 3 ����
     */
    void C10645335_TestCase();

    /*!
     *  \brief         ���������������� R6 4 ����
     */
    void C10645336_TestCase();

    /*!
     *  \brief         ���������������� R6 5 ���
     */
    void C10645337_TestCase();

    /*!
     *  \brief         ���������������� R6eco 1 ���
     */
    void C10645338_TestCase();

    /*!
     *  \brief         ���������������� R6eco 2 ����
     */
    void C10645339_TestCase();

    /*!
     *  \brief         ���������������� R6eco 3 ����
     */
    void C10645340_TestCase();

    /*!
     *  \brief         ���������������� R6eco 4 ����
     */
    void C10645341_TestCase();

    /*!
     *  \brief         ����������������eco R6 5 ���
     */
    void C10645342_TestCase();

    /*!
     *  \brief         ���������������� R6 � R6eco 1 ���
     */
    void C10645343_TestCase();

    /*!
     *  \brief         ���������������� R6 � R6eco 2 ����
     */
    void C10645344_TestCase();

    /*!
     *  \brief         ���������������� R6 � R6eco 3 ����
     */
    void C10645345_TestCase();

    /*!
     *  \brief         ���������������� R6eco � R6 1 ���
     */
    void C10645346_TestCase();

    /*!
     *  \brief         ���������������� R6eco � R6 2 ����
     */
    void C10645347_TestCase();

    /*!
     *  \brief         ���������������� R6eco � R6 3 ����
     */
    void C10645348_TestCase();

    /*!
     *  \brief         ���������� ������ ���������� Slave ��� �����
     */
    void C10832739_TestCase();

    /*!
     *  \brief         ���������� ������ Slave �� CAN ��� �����
     */
    void C10832740_TestCase();

    /*!
     *  \brief         ���������� ������ ���������� Slave � ������
     */
    void C10832741_TestCase();

    /*!
     *  \brief         ���������� ������ Slave �� CAN � ������
     */
    void C10832742_TestCase();

    /*!
     *  \brief         ������ �������� ��������� ������ ������� �����������
     */
    void C10861710_TestCase();

    /*!
     *  \brief         ������ ����������� ������ ������� �������� ���������
     */
    void C10861711_TestCase();

    /*!
     *  \brief         ������ ����������� ���������� �������� ����� ������ ������� ��������� ������
     */
    void C10861712_TestCase();

    /*!
     *  \brief         ������ ����������� ���������� �������� ����� ������ ������� ��������� ��������
     */
    void C10861713_TestCase();

    /*!
     *  \brief         �������� �� � ����������� ���������� � ������ (����������� �������)
     */
    void C10854614_TestCase();

    /*!
     *  \brief         ������� � ������� � ���������� ���������� �� ������ (������������ �������)
     */
    void C10854615_TestCase();

    /*!
     *  \brief         ����������� 3 ��������
     */
    void C10854616_TestCase();

    /*!
     *  \brief         ������������ 3 ��������
     */
    void C10854617_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � �����������
     */
    void C10854594_TestCase();

    /*!
     *  \brief         �������� �� � ����������� ���������� � �����������
     */
    void C10854588_TestCase();

    /*!
     *  \brief         �������� �� � ���������� ���������� � �����������
     */
    void C10854595_TestCase();

    /*!
     *  \brief         ������� � ������� � ����������� ���������� � �����������
     */
    void C10854589_TestCase();

    /*!
     *  \brief         ������� � ������� � ���������� ���������� � �����������
     */
    void C10854590_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � ������� �������
     */
    void C10854596_TestCase();

    /*!
     *  \brief         �������� �� � ����������� ���������� � ������� �������
     */
    void C10854591_TestCase();

    /*!
     *  \brief         �������� �� � ���������� ���������� � ������� �������
     */
    void C10854597_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � �������
     */
    void C10854607_TestCase();

    /*!
     *  \brief         �������� �� � ����������� ���������� � �������
     */
    void C10854608_TestCase();

    /*!
     *  \brief         �������� �� � ���������� ���������� � �������
     */
    void C10854609_TestCase();

    /*!
     *  \brief         �������� �� � ����������� ���������� � ����������������� �������
     */
    void C10854601_TestCase();

    /*!
     *  \brief         ������� � ������� � ���������� ���������� �� ����������������� �������
     */
    void C10854602_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � ���������� ���������� ����� ��� ��
     */
    void C10854603_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � �� ���������� ����� ��� ���������
     */
    void C10854604_TestCase();

    /*!
     *  \brief         �������� �� � ���������� ���������� ����� ��� ��
     */
    void C10854605_TestCase();

    /*!
     *  \brief         �������� �� � �� ���������� ����� ��� ���������
     */
    void C10854606_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � �� ��� ��������
     */
    void C10854598_TestCase();

    /*!
     *  \brief         �������� �� � �� � ���������� ���������� ��� ��������
     */
    void C10854600_TestCase();

    /*!
     *  \brief         ������������ ���� �� �������� � �� � ���������
     */
    void C10854610_TestCase();

    /*!
     *  \brief         �������� �� � �� � ���������� ���������� � ���������
     */
    void C10854612_TestCase();

    /*!
     *  \brief         �������� �� � �� � ����������� ����������
     */
    void C10854613_TestCase();

    /*!
     *  \brief         �� ���������� �� ����� �� �� ����� ���������
     */
    void C10964449_TestCase();

    /*!
     *  \brief         �� ���������� �� ����� �� �� �����-����
     */
    void C10964450_TestCase();

    /*!
     *  \brief         ����������� ����������
     */
    void C11110395_TestCase();

    /*!
     *  \brief         ��������� ����������� ����������
     */
    void C11110396_TestCase();

    /*!
     *  \brief         ��������� ������������� ����������� ����������
     */
    void C11110397_TestCase();

    /*!
     *  \brief         ���������� � ������� ��-�� �� ���������� ����������� ����� �������� ����������� ���������
     */
    void C11446206_TestCase();

    /*!
     *  \brief         ���������� � ������� ��-�� �� ���������� ����������� ����� �������� CAN ���������
     */
    void C11446207_TestCase();

    /*!
     *  \brief         ���������� �� �� ��� ����� ������� �����
     */
    void C11516269_TestCase();

    /*!
     *  \brief         ���������� �� ����� www.starline-online.ru ����� ������� �����
     */
    void C11516270_TestCase();

    /*!
     *  \brief         ���������� �� �� ��� ����� ������� �������
     */
    void C11516271_TestCase();

    /*!
     *  \brief         ���������� �� ����� www.starline-online.ru ����� ������� �������
     */
    void C11516272_TestCase();

    /*!
     *  \brief         ��������� HIC �� ����� � ��������� � ���������� �� �����
     */
    void C11846848_TestCase();

    /*!
     *  \brief         ��������� HIC �� ��������� � ��������� ������� � ���������� �� ���������
     */
    void C11846849_TestCase();

    /*!
     *  \brief         ��������� HIC �� ����� � ���������� ������� � ���������� �� ������
     */
    void C11846850_TestCase();

    /*!
     *  \brief         ��������� HIC �� ��������� � ��������� ��� �������� ���������
     */
    void C11846851_TestCase();

    /*!
     *  \brief         ��������� HIC �� ����� � ��������� ������� ��� �������� �����
     */
    void C11846852_TestCase();

    /*!
     *  \brief         ��������� HIC �� ��������� � ��������� � �������� ����������
     */
    void C11846853_TestCase();

    /*!
     *  \brief         ��������� HIC �� ����� � ��������� ������� � �������� ������
     */
    void C11846854_TestCase();

    /*!
     *  \brief         ��������� ��������� �� BLE ��� ����������� ���������
     */
    void C11946743_TestCase();

    /*!
     *  \brief         ������� � ����������� � ������� Slave
     */
    void C12037386_TestCase();

    /*!
     *  \brief         ������� � ������ ������� � ������� Slave
     */
    void C12037387_TestCase();

    /*!
     *  \brief         ��������� ����� � �����������
     */
    void C12037388_TestCase();

    /*!
     *  \brief         ��������� ����� � ������� �������
     */
    void C12037389_TestCase();

    /*!
     *  \brief         ������� � ������ ������� (�� ������ � ������) �� ���������� ������ � ���������
     */
    void C12037397_TestCase();

    /*!
     *  \brief         ������� � ������ ������� (�� ���������� ���������) �� ���������� ������ � ���������
     */
    void C12037398_TestCase();

    /*!
     *  \brief         ������� � ����������� ������ ������� (�� ������ � ������) �� ���������� ������ � ���������
     */
    void C12037401_TestCase();

    /*!
     *  \brief         ������� � ����������� ������ ������� (�� ���������� ���������) �� ���������� ������ � ���������
     */
    void C12037402_TestCase();

    /*!
     *  \brief         ��� ������ � ������ (�)
     */
    void C12116944_TestCase();

    /*!
     *  \brief         ��� ������ � ������ � ��� ������ ���������� ��������� (�)
     */
    void C12116945_TestCase();

    /*!
     *  \brief         ��� ������ � ������ (�)
     */
    void C12116946_TestCase();

    /*!
     *  \brief         ��� ������ � ������ � ��� ������ ���������� ��������� (�)
     */
    void C12116947_TestCase();

    /*!
     *  \brief         ��� ������ � ������ (�+�)
     */
    void C12116948_TestCase();

    /*!
     *  \brief         ��� ������ � ������ � ��� ������ ���������� ��������� (�+�)
     */
    void C12116949_TestCase();

    /*!
     *  \brief         ��� ������ � ������ (�+�) (���������� ��������)
     */
    void C12116950_TestCase();

    /*!
     *  \brief         ��� ������ � ������ � ��� ������ ���������� ��������� (�+�) (���������� ��������)
     */
    void C12116951_TestCase();

};

#endif  // _AUTO_TESTS_MODES_P2_H_
