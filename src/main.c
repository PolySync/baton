#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "baton.h"

int main(int argc, char **argv)
{
  char *portname = argv[1];

  char *relay_direction = argv[3];

  int fd;

  baton_result_t ret = baton_init(portname, B9600, &fd);

  if( ret == BATON_SUCCESS)
  {
    if (!strcmp(relay_direction, "on"))
    {
      int relay_num = atoi(argv[2]);
      baton_enable_relay(fd, relay_num);
    }
    else if (!strcmp(relay_direction, "off"))
    {
      int relay_num = atoi(argv[2]);
      baton_disable_relay(fd, relay_num);
    }
    else if (!strcmp(relay_direction, "all"))
    {
      long int bitfield = strtoul(argv[2], NULL, 16);

      baton_toggle_relays_by_bitfield(fd, bitfield);
    }

    char buf[25];

    // int relay_num = atoi(argv[2]);

    baton_set_id(fd, 5);
    // // // usleep(2000000);
    baton_get_id(fd, buf, sizeof(buf));

    // // printf("%s\n", buf);

    // baton_toggle_relays_by_bitfield(fd, relay_num);
    baton_relay_status_t status;
    baton_get_relay_status(fd, 5, &status);
    printf("%d\n", status);

    baton_get_id(fd, buf, sizeof(buf));
    printf("%s\n", buf);

    unsigned long statuses;

    baton_get_relay_status_by_bitfield(fd, &statuses);

    printf("%X\n", statuses);
  }



  return 0;
}
