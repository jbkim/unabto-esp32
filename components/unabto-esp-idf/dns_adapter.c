#include <unabto/unabto_external_environment.h>

#include "lwip/dns.h"
#include "lwip/inet.h"
#include "lwip/ip_addr.h"
#include "lwip/ip4_addr.h"
#include "lwip/dns.h"


static uint32_t basestationIPV4 = 0; 

static void dns_callback(const char *name, const ip_addr_t *ipAddr, void *dummy)
{
  (void)dummy;
  if (ipAddr) {
    basestationIPV4 = ntohl(ip_addr_get_ip4_u32(ipAddr));
  }
}



void nabto_dns_resolve(const char* id)
{

  ip_addr_t resolvedIp;
  ip_addr_t* dummy = &resolvedIp; // To get past some stupid warnings treated as errors
  switch(dns_gethostbyname(id, dummy, dns_callback, NULL))
    {
    case ERR_OK:
      basestationIPV4 = ntohl(ip_addr_get_ip4_u32(dummy));
      break;
    case ERR_INPROGRESS:
      basestationIPV4 = 0;
      break;
    default:
      NABTO_LOG_ERROR(("DNS call failed"));
    }
}

//TBC
nabto_dns_status_t nabto_dns_is_resolved(const char *id, uint32_t* v4addr)
{
  if (basestationIPV4)
    {
      *v4addr = basestationIPV4;
      return NABTO_DNS_OK;
    }
  else
    return NABTO_DNS_ERROR;
}

