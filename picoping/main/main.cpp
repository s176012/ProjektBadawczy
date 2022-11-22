extern "C"
{
#include <hardware/gpio.h>

#include <hardware/spi.h>
#include <pico/critical_section.h>
#include <pico/stdio.h>
#include <pico/util/queue.h>

#include "lwipopts.h"
#include <lwip/init.h>
#include <lwip/netif.h>
#include <lwip/timeouts.h>

#include <pico/enc28j60/enc28j60.h>
#include <pico/enc28j60/ethernetif.h>

#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico-onewire/api/one_wire.h"
}

// #include "tcpecho_raw.h"

/* Configuration */
#define SPI spi0
#define SPI_BAUD 2000000
#define SCK_PIN 2
#define SI_PIN 3
#define SO_PIN 0
#define CS_PIN 5
#define INT_PIN 4
#define RX_QUEUE_SIZE 10
//2E-C1-78-AB-C6-E6
#define MAC_ADDRESS                        \
    {                                      \
        0x2e, 0xc1, 0x78, 0xab, 0xc6, 0xe6 \
    }
#define IP_ADDRESS IPADDR4_INIT_BYTES(172, 20, 98, 221)
#define NETWORK_MASK IPADDR4_INIT_BYTES(255, 255, 255, 0)
#define GATEWAY_ADDRESS IPADDR4_INIT_BYTES(172, 20, 98, 1)

// #define ETHERNET_DEBUG

queue_t rx_queue;
critical_section_t spi_cs;
struct netif netif;
struct enc28j60 enc28j60 = {
    .spi = SPI,
    .cs_pin = CS_PIN,
    .mac_address = MAC_ADDRESS,
    .critical_section = &spi_cs,
    .next_packet = 0,
};

void eth_irq(uint gpio, uint32_t events)
{
    enc28j60_isr_begin(&enc28j60);
    uint8_t flags = enc28j60_interrupt_flags(&enc28j60);

    if (flags & ENC28J60_PKTIF)
    {
        struct pbuf *packet = low_level_input(&netif);
        if (packet != NULL)
        {
            if (!queue_try_add(&rx_queue, &packet))
            {
                pbuf_free(packet);
            }
        }
    }

    if (flags & ENC28J60_TXERIF)
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("eth_irq: transmit error\n"));
    }

    if (flags & ENC28J60_RXERIF)
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("eth_irq: receive error\n"));
    }

    enc28j60_interrupt_clear(&enc28j60, flags);
    enc28j60_isr_end(&enc28j60);
}

int main()
{
    // sensor
    stdio_init_all();
    One_wire one_wire(15); // GP15 - Pin 20 on Pi Pico
    one_wire.init();
    rom_address_t address{};

    gpio_init_mask((1 << CS_PIN) | (1 << PICO_DEFAULT_LED_PIN));
    gpio_set_dir_out_masked((1 << CS_PIN) | (1 << PICO_DEFAULT_LED_PIN));
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    stdio_init_all();

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SO_PIN, GPIO_FUNC_SPI);
    spi_init(spi0, SPI_BAUD);

    queue_init(&rx_queue, sizeof(struct pbuf *), RX_QUEUE_SIZE);
    critical_section_init(&spi_cs);

    const struct ip4_addr ipaddr = IP_ADDRESS;
    const struct ip4_addr netmask = NETWORK_MASK;
    const struct ip4_addr gw = GATEWAY_ADDRESS;
    lwip_init();
    netif_add(&netif, &ipaddr, &netmask, &gw, &enc28j60, ethernetif_init, netif_input);
    netif_set_up(&netif);
    netif_set_link_up(&netif);

    gpio_set_irq_enabled_with_callback(INT_PIN, GPIO_IRQ_EDGE_FALL, true, eth_irq);
    enc28j60_interrupts(&enc28j60, ENC28J60_PKTIE | ENC28J60_TXERIE | ENC28J60_RXERIE);

    // tcpecho_raw_init();

    int i =0;
    while (true)
    {
        i++;
        struct pbuf *p = NULL;
        queue_try_remove(&rx_queue, &p);
        if (p != NULL)
        {
            if (netif.input(p, &netif) != ERR_OK)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                pbuf_free(p);
            }
        }

        sys_check_timeouts();
        gpio_put(PICO_DEFAULT_LED_PIN, false);
        best_effort_wfe_or_timeout(make_timeout_time_ms(sys_timeouts_sleeptime()));
        gpio_put(PICO_DEFAULT_LED_PIN, true);

        // sensor
        if(i==8e3){
         one_wire.single_device_read_rom(address);
         printf("Device Address: %02x%02x%02x%02x%02x%02x%02x%02x\n", address.rom[0], address.rom[1], address.rom[2], address.rom[3], address.rom[4], address.rom[5], address.rom[6], address.rom[7]);
         one_wire.convert_temperature(address, true, false);
         printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
         printf("Temperature: %3.1foC\n", one_wire.temperature(address));
         i=0;
        }
        
    }
}
