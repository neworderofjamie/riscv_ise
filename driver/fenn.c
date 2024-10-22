#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/module.h>

#define FENN_ITCM_BASE 0xA5000000
#define FENN_ITCM_SIZE 0x1000

#define FENN_DTCM_BASE 0xA6000000
#define FENN_DTCM_SIZE 0x20000

#define FENN_GPIO_BASE 0xA7000000
#define FENN_GPIO_SIZE 0x800

static struct resource fenn_resources[] = {
    {
        .start = FENN_ITCM_BASE,
        .end = FENN_ITCM_BASE + FENN_ITCM_SIZE - 1,
        .name = "itcm",
        .flags = IORESOURCE_MEM
    },
    {
        .start = FENN_DTCM_BASE,
        .end = FENN_DTCM_BASE + FENN_DTCM_SIZE - 1,
        .name = "dtcm",
        .flags = IORESOURCE_MEM
    },
    {
        .start = FENN_GPIO_BASE,
        .end = FENN_GPIO_BASE + FENN_GPIO_SIZE - 1,
        .name = "gpio",
        .flags = IORESOURCE_MEM
    }};

static struct uio_info fenn_uio_info = {
    .name = "fenn",
    .version = "0.1",
};

static struct platform_device *fenn_uio_pdev;

static int __init myfpga_init(void)
{
    fenn_uio_pdev = platform_device_register_resndata(
        NULL,
        "uio_pdrv",
        -1,
        fenn_resources, 3,
        &fenn_uio_info, sizeof(struct uio_info));
    
    if (IS_ERR(fenn_uio_pdev)) {
        return PTR_ERR(fenn_uio_pdev);
    }
    else {
        return 0;
    }
}

static void __exit fenn_exit(void)
{
    platform_device_unregister(fenn_uio_pdev);
}

module_init(fenn_init);
module_exit(fenn_exit); 
