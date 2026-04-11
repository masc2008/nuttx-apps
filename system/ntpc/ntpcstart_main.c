/****************************************************************************
 * apps/system/ntpc/ntpcstart_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netutils/ntpclient.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * ntpcstart_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  FAR char *server_list = NULL;
  FAR const char *ntp_servers = CONFIG_NETUTILS_NTPCLIENT_SERVER;
  int pid;
  int i;

  if (argc > 1)
    {
      size_t total = 1;
      size_t pos = 0;

      for (i = 1; i < argc; i++)
        {
          total += strlen(argv[i]) + 1;
        }

      server_list = malloc(total);
      if (server_list == NULL)
        {
          fprintf(stderr, "ERROR: failed to allocate server list\n");
          return EXIT_FAILURE;
        }

      for (i = 1; i < argc; i++)
        {
          size_t len = strlen(argv[i]);

          memcpy(server_list + pos, argv[i], len);
          pos += len;

          if (i + 1 < argc)
            {
              server_list[pos++] = ';';
            }
        }

      server_list[pos] = '\0';
      ntp_servers = server_list;
    }

  printf("Starting NTP client...\n");
  printf("Using NTP servers: %s\n", ntp_servers);

  pid = ntpc_start_with_list(ntp_servers);
  if (pid < 0)
    {
      fprintf(stderr, "ERROR: ntpc_start_with_list() failed\n");
      free(server_list);
      return EXIT_FAILURE;
    }

  printf("NTP client started successfully (task ID: %d)\n", pid);
  printf("NTP client is now running in the background\n");
  free(server_list);

  return EXIT_SUCCESS;
}
