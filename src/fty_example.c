/*  =========================================================================
    fty_example - Binary

    Copyright (C) 2014 - 2017 Eaton                                        
                                                                           
    This program is free software; you can redistribute it and/or modify   
    it under the terms of the GNU General Public License as published by   
    the Free Software Foundation; either version 2 of the License, or      
    (at your option) any later version.                                    
                                                                           
    This program is distributed in the hope that it will be useful,        
    but WITHOUT ANY WARRANTY; without even the implied warranty of         
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
    GNU General Public License for more details.                           
                                                                           
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.            
    =========================================================================
*/

/*
@header
    fty_example - Binary
@discuss
@end
*/

#include "fty_example_classes.h"

int main (int argc, char *argv [])
{
    const char * CONFIGFILE = "";
    const char * LOGCONFIGFILE = "";
    
    ftylog_setInstance("fty-example","");
    bool verbose = false;
    int argn;
    for (argn = 1; argn < argc; argn++) {
        if (streq (argv [argn], "--help")
        ||  streq (argv [argn], "-h")) {
            puts ("fty-example [options] ...");
            puts ("  --config / -c          configuration file");
            puts ("  --verbose / -v         verbose test output");
            puts ("  --help / -h            this information");
            return 0;
        }
        else
        if (streq (argv [argn], "--verbose")
        ||  streq (argv [argn], "-v"))
            verbose = true;
        else
        if (streq (argv [argn], "--config")
        ||  streq (argv [argn], "-c")) {
            CONFIGFILE = argv [argn + 1];
            ++argn;
        }
        else {
            log_error ("Unknown option: %s\n", argv [argn]);
            return 1;
        }
    }
    //  Insert main code here
    if (!streq(CONFIGFILE,"")) {
        zconfig_t *cfg = zconfig_load(CONFIGFILE);
        if (cfg) {
            LOGCONFIGFILE = zconfig_get(cfg, "log/config", "");
        }
    }
        
    if (!streq(LOGCONFIGFILE,"")) {
        ftylog_setConfigFile(ftylog_getInstance(),LOGCONFIGFILE);
    }
    
    if (verbose)
    {
        ftylog_setVeboseMode(ftylog_getInstance());
        log_trace ("Verbose mode OK");
    }
    log_info ("fty-example starting");
    const char *endpoint = "ipc://@/malamute";
    zactor_t *example_server = zactor_new (fty_example_server, (void *) endpoint);

    //  Accept and print any message back from server
    //  copy from src/malamute.c under MPL license
    while (true) {
        char *message = zstr_recv (example_server);
        if (message) {
            puts (message);
            free (message);
        }
        else {
            log_info ("interrupted");
            break;
        }
    }
    zactor_destroy (&example_server);
    return EXIT_SUCCESS;
}
