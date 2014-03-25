#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "a10.h"
#include "core.h"

#ifdef DEBUG
#define DEBUG_MSG(format, arg...) printf(format, ##arg)
#else
#define DEBUG_MSG(...)
#endif

static int gpio_pin_fd[MAX_GPIO_NUM+1];
static int gpio_mode_fd[MAX_GPIO_NUM+1];
static int adc_fd[MAX_ADC_NUM+1];
static int core_init(void);

static void pabort(const char *s)
{
    perror(s);
    abort();
}

static int write_to_file(int fd, char *str, int len)
{
    int ret = -1;

    lseek(fd, 0, SEEK_SET);
    ret = write(fd, str, len);
    if ( ret <= 0 )
    {
        fprintf(stderr, "write %d failed\n", fd);
        return -1;
    }
    return ret;     
}

void pinMode(int pin, unsigned int mode)
{
    char buf[4];
    int ret = -1;
    core_init();
    DEBUG_MSG("%s (%d,%d)\n", __FUNCTION__, pin, mode);

    if ( (pin >= 0 && pin <=  MAX_GPIO_NUM) && (mode <= MAX_GPIO_MODE_NUM) )
    {
        memset((void *)buf, 0, sizeof(buf));
        sprintf(buf, "%d", mode);
        ret = write_to_file(gpio_mode_fd[pin], buf, sizeof(buf));
        if ( ret <= 0 )
        {
            fprintf(stderr, "write gpio %d mode failed\n", pin);
            exit(-1);
        }
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin or mode, pin=%d, mode=%d\n",
            __FUNCTION__, pin, mode);
        exit(-1);
     }
}

void digitalWrite(int pin, int value)
{
    char buf[4]; 
    int ret = -1;
    core_init();
    DEBUG_MSG("%s (%d,%d)\n", __FUNCTION__, pin, value);
    
    if ( (pin >= 0 && pin <=  MAX_GPIO_NUM) && (value == HIGH || value == LOW) )
    {
        memset((void *)buf, 0, sizeof(buf));
        sprintf(buf, "%d", value);
        ret = write_to_file(gpio_pin_fd[pin], buf, sizeof(buf));
        if ( ret <= 0 )
        {
            fprintf(stderr, "write gpio %d  failed\n", pin);
            exit(-1);
        }        
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin or mode, pin=%d, value=%d\n",
            __FUNCTION__, pin, value);
        exit(-1);
     }    
}

int digitalRead(int pin)
{
    char buf[4];    
    int ret = -1;
    core_init();
    DEBUG_MSG("%s(%d)\n", __FUNCTION__, pin);
        
    if ( pin >= 0 && pin <= MAX_GPIO_NUM )
    {
        memset((void *)buf, 0, sizeof(buf));
        lseek(gpio_pin_fd[pin], 0, SEEK_SET);
        ret = read(gpio_pin_fd[pin], buf, sizeof(buf));

        if ( ret <= 0 )
        {
            fprintf(stderr, "read gpio %d failed\n", pin);
            exit(-1);
        }
        
        ret = buf[0] - '0';
        switch( ret )
        {
            case LOW:
            case HIGH:
                break;
            default:
                ret = -1;
                break;
        }
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
        exit(-1);
    }      
    return ret;
}

void delay(unsigned long ms)
{
    usleep(ms*1000);
}

void delayMicroseconds(unsigned int us)
{
    usleep(us);
}

int analogRead(int pin)
{
    char str[10];
    char buf[32];    
    int ret = -1;
    char *p = NULL;
    core_init();
        
    if ( pin >= 0 && pin <= MAX_ADC_NUM )
    {
        memset(buf, 0, sizeof(buf));
        lseek(adc_fd[pin], 0, SEEK_SET);
        ret = read(adc_fd[pin], buf, sizeof(buf));
        
        if ( ret <= 0 )
        {
            fprintf(stderr, "read adc %d failed\n", pin);
            exit(-1);
        }

        memset((void *)str, 0, sizeof(str));
        sprintf(str, "adc%d", pin);
        p = strstr(buf, str) + strlen(str) + 1;
        sscanf(p, "%d", &ret);
    }
    else
    {
        fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
        exit(-1);
    }      
    return ret;

}

/*  
 * pin(3/9/10/11) support frequency[125-2000]Hz @different dutycycle
 * pin(5/6) support frequency[195,260,390,520,781] @256 dutycycle
 */
int pwmfreq_set(int pin, unsigned int freq)
{
     int ret = -1;
     int fd = -1;
     PWM_Freq pwmfreq;
         
     if ( (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) && (freq > 0) )
     {
         pwmfreq.channel = pin;
         pwmfreq.freq = freq;
         pwmfreq.step = 0;
         fd = open(pwm_dev, O_RDONLY);
         if ( fd < 0 )
             pabort("open pwm device fail");

         switch (pin) 
         {
         case 5:
         case 6:
            if ( (freq == 195) || (freq == 260) || (freq == 390) 
                || (freq == 520) || (freq == 781) )
            {
               ret = ioctl(fd, PWM_FREQ, &pwmfreq);
               if (ret < 0)
                  pabort("can't set PWM_FREQ");
            }else
               fprintf(stderr, "%s ERROR: invalid frequency, should be [195,260,390,520,781], pin=%d\n", __FUNCTION__, pin);

            break;
         case 3:
         case 9:    
         case 10:   
         case 11:   
            if ((freq >= MIN_PWMTMR_FREQ) && (freq <= MAX_PWMTMR_FREQ)){
               pin = 
               ret = ioctl(fd, PWMTMR_STOP, &pwmfreq.channel);
               if (ret < 0)
                  pabort("can't set PWMTMR_STOP");  
                          
               ret = ioctl(fd, PWM_FREQ, &pwmfreq);
               if (ret < 0)
                  pabort("can't set PWM_FREQ");   
            }else
               fprintf(stderr, "%s ERROR: invalid frequency[%d,%d], pin=%d\n", __FUNCTION__, MIN_PWMTMR_FREQ, MAX_PWMTMR_FREQ, pin);

            break;

         default:   
            break;
         }

         if(fd)
             close(fd);
         return pwmfreq.step;
     }
     else
     {
         fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
         exit(-1);
     } 
     return 0;
}


void analogWrite(int pin, int value)
{
     int ret = -1;
     int fd = -1;
     int val = 0;
     PWM_Config pwmconfig;
     DEBUG_MSG("%s(%d, %d)\n", __FUNCTION__, pin, value);
         
     pwmconfig.channel = pin;
     pwmconfig.dutycycle = value;
     if ( value > MAX_PWM_LEVEL )
        value = MAX_PWM_LEVEL;

     if ( (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) && 
         (value >= 0 && value <= MAX_PWM_LEVEL) )
     {
         fd = open(pwm_dev, O_RDONLY);
         if ( fd < 0 )
             pabort("open pwm device fail");
                
         switch (pin) 
         {
         case 5:
         case 6:
            ret = ioctl(fd, HWPWM_DUTY, &pwmconfig);
            if (ret < 0)
               pabort("can't set HWPWM_DUTY");
            break;
         case 3:
         case 9:    
         case 10:   
         case 11:   
            //ret = ioctl(fd, PWMTMR_STOP, &pwmconfig.channel);
            //if (ret < 0)
            //   pabort("can't set PWMTMR_STOP");  
                       
            ret = ioctl(fd, PWM_CONFIG, &pwmconfig);
            if (ret < 0)
               pabort("can't set PWM_CONFIG");   

            ret = ioctl(fd, PWMTMR_START, &val);
            if (ret < 0)
               pabort("can't set PWMTMR_START");
            break;

         default:   
            break;
         }
         if(fd)
            close(fd);
     }
     else
     {
         fprintf(stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
         exit(-1);
     }      
}

int analogGetMaxLevel(int pin)
{
    DEBUG_MSG("%s(%d)\n", __FUNCTION__, pin);
    return 255;    
}
void analogSetFreq(int pin, int freq)
{
    DEBUG_MSG("%s(%d, %d)\n", __FUNCTION__, pin, freq);
    pwmfreq_set(pin, freq);
}

int analogReadVoltage(int pin)
{
    int i;
    float level = 0;
    int value;
    for( i = 0; i<3; ++i)
        value = analogRead(pin);
    if ( value && (pin == 0 || pin == 1) )
    {
        level = 2.0*1000/64 * (value + 1);  
    }
    else if ( value && (pin > 1 && pin < 6) )
    {
        level = 3.3*1000/4096 * (value + 1);
    }
    DEBUG_MSG("%s voltage = %d mV\n", __FUNCTION__, (int)level);
    return (int)level;
}

static int core_init(void)
{
    static int first_run = 1;
    int i;
    char path[1024];
    if ( !first_run )
        return 0;
    DEBUG_MSG("%s\n", __FUNCTION__);
    first_run = 0;
    for( i = 0; i<= MAX_GPIO_NUM; ++i)
    {
        memset(path, 0, sizeof(path));
        sprintf(path, "%s%s%d", GPIO_PIN_DIR, GPIO_IF_PREFIX, i);
        gpio_pin_fd[i] = open(path, O_RDWR);
        if ( gpio_pin_fd[i] < 0 )
        {
            fprintf(stderr, "open %s failed\n", path);
            return -1;
        }
        
        memset(path, 0, sizeof(path));
        sprintf(path, "%s%s%d", GPIO_MODE_DIR, GPIO_IF_PREFIX, i);
        gpio_mode_fd[i] = open(path, O_RDWR);
        if ( gpio_mode_fd[i] < 0 )
        {
            fprintf(stderr, "open %s failed\n", path);
            return -1;
        } 
    }
    
    for( i = 0; i<= MAX_ADC_NUM; ++i)
    {
        memset(path, 0, sizeof(path));
        sprintf(path, "%s%d", ADC_IF, i);
        adc_fd[i] = open(path, O_RDONLY);
        if ( adc_fd[i] < 0 )
        {
            fprintf(stderr, "open %s failed\n", path);
            return -1;
        }
    }
    return 0;
}

