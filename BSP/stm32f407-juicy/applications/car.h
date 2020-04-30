#ifndef CAR_H__
#define CAR_H__


#define WHEEL_PIN(x)            GET_PIN(E, 8 + x)

#define LEFT_FRONT_CTRL1        GET_PIN(E, 8)
#define LEFT_FRONT_CTRL2        GET_PIN(E, 9)

#define RIGHT_FRONT_CTRL1       GET_PIN(E, 10)
#define RIGHT_FRONT_CTRL2       GET_PIN(E, 11)

#define LEFT_BACK_CTRL1         GET_PIN(E, 12)
#define LEFT_BACK_CTRL2         GET_PIN(E, 13)

#define RIGHT_BACK_CTRL1        GET_PIN(E, 14)
#define RIGHT_BACK_CTRL2        GET_PIN(E, 15)


typedef enum
{
    Ecm_forward = 0,
    Ecm_backward,
    Ecm_stop,
    Ecm_left,
    Ecm_right,
    Ecm_num,

} ECar_Motion;


typedef enum
{
    Ews_forward = 0,    // ǰ��
    Ews_backward,       // ����
    Ews_stop,           // ֹͣ
    Ews_Num,
    
} EWheel_Stat;

typedef enum
{
    Ewn_lf = 0, // ��ǰ��
    Ewn_rf,     // ��ǰ��
    Ewn_lb,     // �����
    Ewn_rb,     // �Һ���
    Ewn_Num,
    
} EWheel_Number;

typedef struct
{
    unsigned int ctrl1;
    unsigned int ctrl2;

} SWheel_Ctrl_t;


extern void car_motion(ECar_Motion m);
extern void car_sample(void);

#endif

