#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "car.h"

#include <ulog.h>


static SWheel_Ctrl_t s_wheel_ctrl[Ews_Num] = 
{
    {1, 0},
    {0, 1},
    {0, 0},
};

#define WHEEL_CTRL_PIN_LEN sizeof(SWheel_Ctrl_t) / sizeof(unsigned int)

static void drv_one_wheel(EWheel_Number number, EWheel_Stat stat)
{
    SWheel_Ctrl_t *p = &s_wheel_ctrl[stat];
    unsigned int pin = WHEEL_PIN(number * WHEEL_CTRL_PIN_LEN);

    LOG_D("[drv_one_wheel]pin = %d, stat = %d\n\r", pin, stat);

    rt_pin_write(pin, p->ctrl1);
    rt_pin_write(pin + 1, p->ctrl2);
}

void car_sample(void)
{
    int i, j;

    LOG_D("car sample start\n");

    for (i = 0; i < 8; i++)
    {
        rt_pin_mode(WHEEL_PIN(i), PIN_MODE_OUTPUT);
    }

    // 所有的轮子验证前进、后退、停止功能
    /*for (j = Ewn_rb; j < Ewn_Num; j++)
    {
        for (i = Ews_forward; i < Ews_Num; i++)
        {
            drv_one_wheel(j, i);
            rt_thread_mdelay(3000);
        }
    }*/
}


void car_motion(ECar_Motion m)
{
    EWheel_Number idx;
    
    switch (m)
    {
            case Ecm_forward:
            case Ecm_backward:
            case Ecm_stop:
            {
                for (idx = Ewn_lf; idx < Ewn_Num; idx++)
                {
                    drv_one_wheel(idx, m);
                }
                
                break;
            }
    
            case Ecm_left:
            {
                for (idx = Ewn_rf; idx < Ewn_Num; idx += 2)
                {
                    drv_one_wheel(idx, Ecm_forward);
                }
    
                for (idx = Ewn_lf; idx < Ewn_Num; idx += 2)
                {
                    drv_one_wheel(idx, Ecm_stop);
                }
    
                
                break;
            }
    
            case Ecm_right:
            {
                for (idx = Ewn_lf; idx < Ewn_Num; idx += 2)
                {
                    drv_one_wheel(idx, Ecm_forward);
                }
    
                for (idx = Ewn_rf; idx < Ewn_Num; idx += 2)
                {
                    drv_one_wheel(idx, Ecm_stop);
                }
    
                
                break;
            }
    
            default:
            {
                break;
            }
    
        }

}


void car_motion_test(int argc, char **argv)
{
    ECar_Motion m;
    EWheel_Number idx;


    if (argc < 2)
    {
        rt_kprintf("Please input param \n");
        return;
    }

    if (!rt_strcmp(argv[1], "forward"))
    {
        m = Ecm_forward;
    }
    else if (!rt_strcmp(argv[1], "backward"))
    {
        m = Ecm_backward;
    }
    else if (!rt_strcmp(argv[1], "left"))
    {
        m = Ecm_left;
    }
    else if (!rt_strcmp(argv[1], "right"))
    {
        m = Ecm_right;
    }
    else if (!rt_strcmp(argv[1], "stop"))
    {
        m = Ecm_stop;
    }

    switch (m)
    {
        case Ecm_forward:
        case Ecm_backward:
        case Ecm_stop:
        {
            for (idx = Ewn_lf; idx < Ewn_Num; idx++)
            {
                drv_one_wheel(idx, m);
            }
            
            break;
        }

        case Ecm_left:
        {
            for (idx = Ewn_rf; idx < Ewn_Num; idx += 2)
            {
                drv_one_wheel(idx, Ecm_forward);
            }

            for (idx = Ewn_lf; idx < Ewn_Num; idx += 2)
            {
                drv_one_wheel(idx, Ecm_stop);
            }

            
            break;
        }

        case Ecm_right:
        {
            for (idx = Ewn_lf; idx < Ewn_Num; idx += 2)
            {
                drv_one_wheel(idx, Ecm_forward);
            }

            for (idx = Ewn_rf; idx < Ewn_Num; idx += 2)
            {
                drv_one_wheel(idx, Ecm_stop);
            }

            
            break;
        }

        default:
        {
            break;
        }

    }
}


MSH_CMD_EXPORT(car_motion_test, car motion test);





MSH_CMD_EXPORT(car_sample, car sample);
